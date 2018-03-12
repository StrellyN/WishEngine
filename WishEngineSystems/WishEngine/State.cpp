/**
    Copyright 2018 Strelly

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
**/

#include "State.hpp"

namespace WishEngine{
    State::State(){
        loadSystems();
        handleInput = Message(MESSAGETYPES::HANDLEINPUT);
        startFrame = Message(MESSAGETYPES::SFRAME);
        renderFrame = Message(MESSAGETYPES::RFRAME, 0);
        finishFrame = Message(MESSAGETYPES::FFRAME);
    }

    State::~State(){
        #ifdef _WIN32
            for(unsigned i=0; i<systemsDLL.size(); i++){
                typedef void (__cdecl *ObjProc)(void);
                ObjProc ProcAdd = (ObjProc)GetProcAddress(systemsDLL[i], "destroySystem");
                if(ProcAdd){
                    ProcAdd();
                }
                FreeLibrary(systemsDLL[i]);
            }
            systemsDLL.clear();
        #elif defined(__unix__) || defined(__linux__)
            for(unsigned i=0; i<systemsDLL.size(); i++){
                typedef void (*ObjProc)(void);
                ObjProc fn = (ObjProc)dlsym(systemsDLL[i], "destroySystem");
                if(fn){
                    fn();
                }
                dlclose(systemsDLL[i]);
            }
            systemsDLL.clear();
        #endif
        systems.clear();
    }

    void State::loadSystems(){
        #ifdef _WIN32
            WIN32_FIND_DATA fileData;
            HANDLE fileHandle = FindFirstFile(R"(.\systems\*.dll)", &fileData);
            typedef GameSystem* (__cdecl *ObjProc)(void);

            if(fileHandle == (void*)ERROR_INVALID_HANDLE || fileHandle == (void*)ERROR_FILE_NOT_FOUND){
                return;
            }

            do{
                HINSTANCE temp = LoadLibrary((R"(systems\)" +  std::string(fileData.cFileName)) .c_str());

                if(temp){
                    ObjProc ProcAdd = (ObjProc)GetProcAddress(temp, "getSystem");
                    if(ProcAdd){
                        addSystem(ProcAdd());
                        systemsDLL.push_back(temp);
                    }
                }
            }while (FindNextFile(fileHandle, &fileData));
        #elif defined(__unix__) || defined(__linux__)
            DIR *dir;
            struct dirent *ent;
            if((dir = opendir ("./systems")) != NULL){
                while((ent = readdir (dir)) != NULL){ //This gives only the file name. ent->d_name
                    void *lib_handle;
                    typedef GameSystem* (*ObjProc)(void);

                    lib_handle = dlopen((std::string("./systems/") + std::string(ent->d_name)).c_str(), RTLD_LAZY);
                    if (lib_handle){
                        ObjProc fn = (ObjProc)dlsym(lib_handle, "getSystem");
                        if(fn){
                            addSystem(fn());
                            systemsDLL.push_back(lib_handle);
                        }
                    }
                }
                closedir (dir);
            }
        #endif
    }

    bool systemPrioritySorting(GameSystem *a, GameSystem *b){
        return a->getUpdatePriority() < b->getUpdatePriority();
    }

    /**
        Game loop, this loops is the heart of the game, it updates all the logic X times per second
        and renders the game as much as it can.
        Made looking at: https://gafferongames.com/post/fix_your_timestep/
    **/
    void State::update(){
        if(systems.empty()){
            quit = true;
        }
        else{
            std::sort(systems.begin(), systems.end(), systemPrioritySorting);
        }

        double dt = 0.01, accumulator = 0; //Sets some variables for the timing of the loop, like the current ms

        auto now = std::chrono::high_resolution_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto epoch = now_ms.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

        double currentDT = value.count(), newDT, interpolation;
        while(!quit){ //While the state isn't set to quit
            now = std::chrono::high_resolution_clock::now();
            now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
            epoch = now_ms.time_since_epoch();
            value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

            newDT = value.count(); //Get the ticks before all the updates
            double frameTime = (newDT - currentDT) / double(1000); // Amount of seconds it takes a frame to finish
            if ( frameTime > double(TICKS_PER_SECOND)/double(100) ) // Capping the frame time so the game skips things if it lags too much
                frameTime = double(TICKS_PER_SECOND)/double(100);
            currentDT = newDT; //Set the current ticks as the new ones for the next frames calculations
            accumulator += frameTime; //Add the time passed to the accumulator
            while( accumulator >= dt ){ //While the accumulator is greater than the fixed time value
                sendMessage(handleInput);
                for(unsigned i=0; i<getSystems().size(); i++){ //Update all the systems and check for quit being true
                    getSystems()[i]->update(dt);
                    if(getQuit()){ //If quit is true quit the state
                        break;
                    }
                }
                if(getQuit()){
                    break;
                }
                handleMessages();
                if(getQuit()){
                    break;
                }
                accumulator -= dt; //Substract from the accumulator to avoid the INFINITE WHILE LOOP!!!!
            }
            if(getQuit()){
                break;
            }
            interpolation = accumulator/dt; //Calculate the interpolation value for the rendering (Used to interpolate between the last and current
            render(interpolation); //frame so it's a smooth rendering), and then render the frame.
        }
    }

    /**
        Method that calls everything needed to render the frame.
    **/
    void State::render(double interpolation){
        sendMessage(startFrame);
        renderFrame.setNumericValue(interpolation);
        sendMessage(renderFrame);
        sendMessage(finishFrame);
    }

    /**
        Method that collects all the messages posted by the systems and handles them.
    **/
    void State::handleMessages(){
        for(unsigned i=0; i<systems.size(); i++){
            while(systems[i]->getMessagesAmount() > 0){
                Message *aux = systems[i]->getMessage(0);
                if(aux != nullptr){
                    sendMessage(*aux);
                }
                aux = nullptr;
                if(getQuit() || systems[i]->getMessagesAmount() == 0){
                    break;
                }
                systems[i]->deleteMessage(0);
            }
            if(getQuit())
                break;
        }
    }

    /**
        Method used to handle received messages.
    **/
    void State::handleMessage(Message &msg){
        if(msg.getType() == MESSAGETYPES::QUIT){
            setQuit(true);
        }
    }

    /**
        Method to send messages to all the systems.
    **/
    void State::sendMessage(Message &mes){
        handleMessage(mes); //First it handles it itself
        for(unsigned i=0; i<getSystems().size(); i++){ //And finaly to the rest of the systems
            getSystems()[i]->handleMessage(&mes);
        }
    }

    /**
        Method that deletes hunger from the world.
    **/
    bool State::getQuit(){
        return quit;
    }

    /**
        Method that deletes poverty from the world.
    **/
    void State::setQuit(bool q){
        quit = q;
    }

    /**
        Method that returns the systems vector.
    **/
    std::vector<GameSystem*>& State::getSystems(){
        return systems;
    }

    /**
        Method used to check if the state has a certain system.
    **/
    bool State::hasSystemType(std::string &type){
        for(unsigned i=0; i<getSystems().size(); i++){
            if(getSystems()[i]->getSystemType() == type){
                return true;
            }
        }
        return false;
    }

    /**
        Method to add a system to the state if it doesn't have it already.
    **/
    void State::addSystem(GameSystem *nSystem){
        if(!hasSystemType(nSystem->getSystemType())){
            systems.push_back(nSystem);
        }
    }

    /**
        Method that returns a pointer to the system if the state has it.
    **/
    GameSystem *State::getSystem(std::string &type){
        for(unsigned i=0; i<getSystems().size(); i++){
            if(getSystems()[i]->getSystemType() == type){
                return getSystems()[i];
            }
        }
    }
}
