/**
    Copyright 2017 Strelly

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

#include "HppHeaders.hpp"

namespace WishEngine{
    State::State(std::string configFile){
        //Maybe change the adding of the systems as plugins, so the coder can use
        //whatever he wants and also modders can code more systems more easily.
        //ObjectFactory and Framework will always be loaded though.
        addSystem(new ScriptSystem()); //Add all the systems and everything needed
        addSystem(new InputSystem());
        addSystem(new GraphicsSystem());
        addSystem(new PhysicsSystem());
        addSystem(new AudioSystem());
        addSystem(new AnimationSystem());
        addSystem(new NetworkSystem());
        addSystem(new TimerSystem());
        addSystem(new CollisionSystem());
        ObjectFactory::getObjectFactory()->destroyObjectFactory();
        setObjFac(ObjectFactory::getObjectFactory());
        fw = Framework::getFramework();
        getObjFac()->createObjects(configFile); //We pass the file received to the object factory so it can create everything
    }

    State::~State(){
        for(unsigned i=0; i<systems.size(); i++){ //Delete all the systems
            delete systems[i];
            systems[i] = nullptr;
        }
        systems.clear();
        if(nextState != nullptr){ //Delete the next state if there's any
            delete nextState;
            nextState = nullptr;
        }
        Framework::getFramework()->clearData();
        objFac = nullptr;
    }

    /**
        Game loop, this loops is the heart of the game, it updates all the logic X times per second
        and renders the game as much as it can.
        Made looking at: https://gafferongames.com/post/fix_your_timestep/
    **/
    void State::update(){
        double dt = 0.01, accumulator = 0; //Sets some variables for the timing of the loop, like the current ms
        double currentDT = fw->getTicks(), newDT, interpolation;
        while(!quit){ //While the state isn't set to quit
            newDT = fw->getTicks(); //Get the ticks before all the updates
            double frameTime = (newDT - currentDT) / double(1000); // Amount of seconds it takes a frame to finish
            if ( frameTime > double(TICKS_PER_SECOND)/double(100) ) // Capping the frame time so the game skips things if it lags too much
                frameTime = double(TICKS_PER_SECOND)/double(100);
            currentDT = newDT; //Set the current ticks as the new ones for the next frames calculations
            accumulator += frameTime; //Add the time passed to the accumulator
            while( accumulator >= dt ){ //While the accumulator is greater than the fixed time value
                sendMessage(new Message(M_TYPES::HANDLEINPUT));
                for(unsigned i=0; i<getSystems().size(); i++){ //Update all the systems and check for quit being true
                    getSystems()[i]->update(dt);
                    if(getQuit() == true){ //If quit is true quit the state
                        break;
                    }
                }
                if(getQuit() == true){
                    break;
                }
                handleMessages();
                if(getQuit() == true){
                    break;
                }
                accumulator -= dt; //Substract from the accumulator to avoid the INFINITE WHILE LOOP!!!!
            }
            if(getQuit() == true){
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
        sendMessage(new Message(M_TYPES::SFRAME));
        sendMessage(new RenderMessage(M_TYPES::RENDER, interpolation));
        sendMessage(new Message(M_TYPES::FFRAME));
    }

    /**
        Method that collects all the messages posted by the systems and handles them.
    **/
    void State::handleMessages(){
        for(unsigned i=0; i<systems.size(); i++){
            std::vector<Message*> &sentMessages = systems[i]->getMessages();
            for(unsigned j=0; j<sentMessages.size(); j++){
                handleMessage(sentMessages[j]);
                if(getQuit())
                    break;
                if(sentMessages[j] != nullptr){ //Avoinding the MEMORY LEAKS (It already happened once :( Over 1k messages leaked
                    delete sentMessages[j];
                    sentMessages[j] = nullptr;
                }
            }
            sentMessages.clear();
        }
        bool clearVector = true;
        std::vector<Message*> &factoryMessages = ObjectFactory::getObjectFactory()->getMessages();
        for(unsigned j=0; j<factoryMessages.size(); j++){
            handleMessage(factoryMessages[j]);
            if(getQuit()){
                clearVector = false;
                break;
            }
            if(factoryMessages[j] != nullptr){ //Avoinding the MEMORY LEAKS (It already happened once :( Over 1k messages leaked
                delete factoryMessages[j];
                factoryMessages[j] = nullptr;
            }
        }
        if(clearVector){
            factoryMessages.clear();
        }
    }

    /**
        Method used to handle received messages.
    **/
    void State::handleMessage(Message *msg){
        switch(msg->getType()){ //Check the message type and does whatever it needs to do depending on it
            case M_TYPES::QUIT:{
                setQuit(true);
                break;
            }
            case M_TYPES::FULLSCREEN:{ //Fix for it to work
                std::string windowName = msg->getValue();
                fw->fullScreen(windowName);
                break;
            }
            case M_TYPES::NEWSTATE:{
                nextState = new State(msg->getValue());
                quit = true;
                break;
            }
            case M_TYPES::LOADSTATE:{
                std::fstream file(msg->getValue(), std::ios::in);
                if(file){
                    file.close();
                    Framework::getFramework()->stopMusic();
                    ObjectFactory::getObjectFactory()->clearData(); //Destroy all the objects and cameras
                    int j=0;
                    for(unsigned i=0; i<Framework::getFramework()->getWindowCount()-1; i++){ //Destroy all the windows (Except main window)
                        if(Framework::getFramework()->getWindowName(i) == "mainWindow"){
                            j++;
                            continue;
                        }
                        else{
                            Framework::getFramework()->deleteWindow(Framework::getFramework()->getWindowName(j));
                        }
                    }
                    ObjectFactory::getObjectFactory()->createObjects(msg->getValue());
                }
                else{
                    file.close();
                }
                break;
            }
        }
    }

    /**
        Method to send messages to all the systems.
    **/
    void State::sendMessage(Message *mes){
        handleMessage(mes); //First it handles it itself
        if(objFac != nullptr){
            objFac->handleMessage(mes); //Then sends it to the object factory
        }
        for(unsigned i=0; i<getSystems().size(); i++){ //And finaly to the rest of the systems
            getSystems()[i]->handleMessage(mes);
        }
        delete mes; //Then it deletes the message because memory leaks, am I right?
        mes = nullptr;
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
        Method that returns the next state and then sets it to nullptr.
        This is done to avoid the cycle of comming back to the current
        state only to inmediately load the next one again.
    **/
    State* State::getNextState(){
        State *aux = nextState;
        nextState = nullptr;
        return aux;
    }

    /**
        Method that sets the next state and deletes the current next state
        if necesary.
    **/
    void State::setNextState(State* nState){
        if(nextState != nullptr){
            delete nextState;
            nextState = nullptr;
        }
        nextState = nState;
    }

    /**
        Method used to check if there's a next state.
    **/
    bool State::hasNextState(){
        if(nextState != nullptr){
            return true;
        }
        return false;
    }

    /**
        Method that returns the systems vector.
    **/
    std::vector<GameSystem*>& State::getSystems(){
        return systems;
    }

    /**
        Method that sets the system vector as the new one deleteing
        the old ones in the process.
    **/
    void State::setSystems(std::vector<GameSystem*>& nSystems){
        for(unsigned i=0; i<systems.size(); i++){ //Delete all the systems
            delete systems[i];
            systems[i] = nullptr;
        }
        systems.clear(); //Clear the vector
        systems = nSystems; //Set the new systems
    }

    /**
        Method used to check if the state has a certain system.
    **/
    bool State::hasSystemType(S_TYPES type){
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
    GameSystem* State::getSystem(S_TYPES type){
        for(unsigned i=0; i<getSystems().size(); i++){
            if(getSystems()[i]->getSystemType() == type){
                return getSystems()[i];
            }
        }
        return nullptr;
    }

    /**
        Method that returns the object factory pointer.
    **/
    ObjectFactory* State::getObjFac(){
        return objFac;
    }

    /**
        Method that sets the new object factory, deleteing the old one in the process.
    **/
    void State::setObjFac(ObjectFactory* of){
        if(objFac != nullptr){
            delete objFac;
            objFac = nullptr;
        }
        objFac = of;
    }
}
