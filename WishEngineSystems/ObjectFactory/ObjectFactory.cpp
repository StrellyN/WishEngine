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

#include "ObjectFactory.hpp"

namespace WishEngine{
    ObjectFactory::ObjectFactory(){
        setSystemType("OBJECTFACTORY");
        std::fstream initialStateConfig; //Pasar a object factory
        std::string file;
        initialStateConfig.open("data/INISTATE.config", std::ios::in); //we open the config file
        if(initialStateConfig){ //Check if file can be accessed
            initialStateConfig >> file; //get the state configuration file path from the file
            initialStateConfig.close(); //close the file
            loadObjects(file);//Push a state that will read it's object configuration from the file
        }
        else{ //If it can't be accessed
            initialStateConfig.close(); //close the file
            std::fstream errorLog; //open a stream to the error log
            errorLog.open("ERROR.txt", std::ios::out); //Open the error log
            errorLog << "Couldn't access data/INISTATE.config file."; //write the error
            errorLog.close(); //close the stream
            postMessage(new Message(MESSAGETYPES::QUIT));
        }
        file.clear();
    }

    ObjectFactory::~ObjectFactory(){
        clearData();
        destroySystem();
    }

    void ObjectFactory::clearData(){
        objects.clear();
        for(std::unordered_map<int, BaseCollection*>::iterator it = componentCollections.begin(); it != componentCollections.end(); it++){
            delete it->second;
            it->second = nullptr;
        }
        componentCollections.clear();
    }

    /**
        This is its separate method (from clearData()) because in the future, I'll implement a way to have objects
        stay between states, so, in this method, I'll check for that, and also, I'll have to do the change position
        of owners and components when doing it. (Like I do when sorting the cameras and graphics in the Framework)

        for(unsigned i=0; i<graphics->size(); i++){
            (*objects)[(*graphics)[i].getOwnerPos()].setComponentPosition(i, (*graphics)[i].getType());
        }

        Only I'll have to do it for both the objects and the components.

        But for now, it stays like this.
    **/
    void ObjectFactory::deleteEverything(){
        objects.clear();
        availableObjects.clear();
        for(std::unordered_map<int, BaseCollection*>::iterator it = componentCollections.begin(); it != componentCollections.end(); it++){
            delete it->second;
            it->second = nullptr;
        }
        componentCollections.clear();
        destroySystem();
    }

    void ObjectFactory::update(double dt){
        //Send the objects and components pointer.
        postMessage(new ObjectListMessage(MESSAGETYPES::OBJECTLIST, &objects));
        postMessage(new ComponentListMessage(MESSAGETYPES::COMPONENTLIST, &componentCollections));
        postMessage(new AvailableObjectsMessage(MESSAGETYPES::AVAILABLEOBJECTS, &availableObjects));
    }

    void ObjectFactory::handleMessage(Message* msg){
        if(msg->getType() == MESSAGETYPES::LOADOBJECTS){
            loadObjects(msg->getValue());
        }
        else if(msg->getType() == MESSAGETYPES::GOTOSTATE){ //Esto hacerlo en la object factory tambi�n
            deleteEverything();
            loadObjects(msg->getValue());
        }
    }

    /**
        Method to check if an object name is already in use
    **/
    std::string ObjectFactory::checkObjectName(std::string &name){
        if(name == ""){
            name = "Object";
        }
        std::string finalName = name;
        int modifier = 1;
        for(int i=0; i<objects.size(); i++){
            if(!objects[i].getDeleted() && objects[i].getName() == finalName){
                finalName = name;
                finalName += Utils::intToString(modifier);
                i = -1;
                modifier++;
            }
        }
        return finalName;
    }

    /**
        Method that reads a file in search of configuration structures to create objects, windows or cameras.
    **/
    void ObjectFactory::loadObjects(std::string file){ //And you'd format the data map here for each object in the file
        std::fstream objectStream;
        objectStream.open(file, std::ios::in); //Open the stream
        if(objectStream){ //if file could be accessed
            //Load objects and stuff
            std::string objType;
            while(objectStream >> objType){ //While an object, a window or a camera is read
                if(objType == "OBJECT"){ //If you read an object
                    unsigned objPos = 0, objId = 0;
                    std::string compName = "", objName = "";
                    bool objEnabled, hasName;

                    //Basics of the objects
                    objectStream >> objEnabled;
                    objectStream >> hasName;
                    if(hasName){
                        objectStream >> objName;
                    }
                    objName = checkObjectName(objName);
                    if(availableObjects.empty()){
                        objects.emplace_back(objName, objEnabled);
                        objPos = objects.size() - 1;
                        if(objPos <= 0){
                            objects[objPos].setId(0);
                        }
                        else{
                            objects[objPos].setId(objects[objPos-1].getId() + 1);
                        }
                    }
                    else{
                        objPos = availableObjects[availableObjects.size() - 1];
                        unsigned oldId = objects[objPos].getId();
                        objects[objPos] = GameObject(objName, objEnabled);
                        objects[objPos].setId(oldId);
                        availableObjects.pop_back();
                    }
                    objId = objects[objPos].getId();

                    //Components
                    while(objectStream >> compName){ //While you don't reach the end of the object declaration
                        bool compEnabled;
                        if(compName == "END"){ //If you read END, that means end of declaration so you quit the loop
                            break;
                        }
                        if(compName == "ANIMATION"){
                            double x, y, w, h;
                            objectStream >> compEnabled;
                            objectStream >> x;
                            objectStream >> y;
                            objectStream >> w;
                            objectStream >> h;
                            if(componentCollections.count(COMPONENTTYPES::ANIMATION) == 0){
                                componentCollections[COMPONENTTYPES::ANIMATION] = new Collection<AnimationComponent>(COMPONENTTYPES::ANIMATION);
                            }
                            Collection<AnimationComponent> *col = dynamic_cast<Collection<AnimationComponent>*>(componentCollections[COMPONENTTYPES::ANIMATION]);
                            unsigned result = col->addItem(AnimationComponent(x, y, w, h), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "ANIMATOR"){
                            std::string animatorStates, initialState;
                            AnimatorComponent newAnimator;
                            objectStream >> compEnabled;
                            objectStream >> initialState;
                            newAnimator.setCurrentState(initialState);
                            newAnimator.setEnabled(compEnabled);
                            while(objectStream >> animatorStates){
                                if(animatorStates == "END"){ //If you read END, that means end of declaration, so you quit the loop
                                    break;
                                }
                                if(animatorStates == "STATE"){
                                    std::string stateName = "State";
                                    AnimationState newState;
                                    std::string stateVals;
                                    int stateType;
                                    objectStream >> stateVals; //Get the first value

                                    if(stateVals == "NAME"){ //If its the name we set the name and advance to the next value
                                        objectStream >> stateName;
                                        objectStream >> stateVals;
                                    }
                                    if(stateVals == "TYPE"){ //If current value is type
                                        objectStream >> stateType; //We load the type
                                    }
                                    if(stateType == 2){ //If type is 2 we load all the attributes
                                        bool revAnim;
                                        double inix, iniy, frw, frh, texw, texh, animspeed, animframes;
                                        objectStream >> revAnim;
                                        objectStream >> inix;
                                        objectStream >> iniy;
                                        objectStream >> frw;
                                        objectStream >> frh;
                                        objectStream >> texw;
                                        objectStream >> texh;
                                        objectStream >> animspeed;
                                        objectStream >> animframes;
                                        newState = AnimationState(revAnim, inix, iniy, frw, frh, texw, texh, animspeed, animframes);
                                    }
                                    newAnimator.addAnimationState(stateName, newState); //We add the state
                                }
                            }
                            if(componentCollections.count(COMPONENTTYPES::ANIMATOR) == 0){
                                componentCollections[COMPONENTTYPES::ANIMATOR] = new Collection<AnimatorComponent>(COMPONENTTYPES::ANIMATOR);
                            }
                            Collection<AnimatorComponent> *col = dynamic_cast<Collection<AnimatorComponent>*>(componentCollections[COMPONENTTYPES::ANIMATOR]);
                            unsigned result = col->addItem(newAnimator, objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "AUDIO"){
                            std::string audioFile, name="AUDIO";
                            bool isSong, isPlaying;
                            int loops;
                            objectStream >> compEnabled;
                            objectStream >> name;
                            objectStream >> audioFile;
                            objectStream >> isSong;
                            objectStream >> isPlaying;
                            objectStream >> loops;
                            if(componentCollections.count(COMPONENTTYPES::AUDIO) == 0){
                                componentCollections[COMPONENTTYPES::AUDIO] = new Collection<AudioComponent>(COMPONENTTYPES::AUDIO);
                            }
                            Collection<AudioComponent> *col = dynamic_cast<Collection<AudioComponent>*>(componentCollections[COMPONENTTYPES::AUDIO]);
                            unsigned result = col->addItem(AudioComponent(audioFile, isSong, isPlaying, loops), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "CAMERA"){
                            double x, y;
                            int prior;
                            objectStream >> compEnabled;
                            objectStream >> prior;
                            objectStream >> x;
                            objectStream >> y;
                            if(componentCollections.count(COMPONENTTYPES::CAMERA) == 0){
                                componentCollections[COMPONENTTYPES::CAMERA] = new Collection<CameraComponent>(COMPONENTTYPES::CAMERA);
                            }
                            Collection<CameraComponent> *col = dynamic_cast<Collection<CameraComponent>*>(componentCollections[COMPONENTTYPES::CAMERA]);
                            unsigned result = col->addItem(CameraComponent(prior, x, y), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "DIMENSION"){
                            double x, y, w, h;
                            objectStream >> compEnabled;
                            objectStream >> x;
                            objectStream >> y;
                            objectStream >> w;
                            objectStream >> h;
                            if(componentCollections.count(COMPONENTTYPES::DIMENSION) == 0){
                                componentCollections[COMPONENTTYPES::DIMENSION] = new Collection<DimensionComponent>(COMPONENTTYPES::DIMENSION);
                            }
                            Collection<DimensionComponent> *col = dynamic_cast<Collection<DimensionComponent>*>(componentCollections[COMPONENTTYPES::DIMENSION]);
                            unsigned result = col->addItem(DimensionComponent(x, y, w, h), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "GRAPHIC"){
                            int type;
                            bool isU;
                            objectStream >> compEnabled;
                            objectStream >> isU;
                            objectStream >> type;
                            if(type == 1){
                                if(componentCollections.count(COMPONENTTYPES::GRAPHIC) == 0){
                                    componentCollections[COMPONENTTYPES::GRAPHIC] = new Collection<GraphicComponent>(COMPONENTTYPES::GRAPHIC);
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections[COMPONENTTYPES::GRAPHIC]);
                                unsigned result = col->addItem(GraphicComponent(isU), objId);
                                if(result){
                                    unsigned compPos = result-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                            else if(type == 2){
                                int r, g, b, a, pr;
                                objectStream >> r;
                                objectStream >> g;
                                objectStream >> b;
                                objectStream >> a;
                                objectStream >> pr;
                                if(componentCollections.count(COMPONENTTYPES::GRAPHIC) == 0){
                                    componentCollections[COMPONENTTYPES::GRAPHIC] = new Collection<GraphicComponent>(COMPONENTTYPES::GRAPHIC);
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections[COMPONENTTYPES::GRAPHIC]);
                                unsigned result = col->addItem(GraphicComponent(isU, r, g, b, a, pr), objId);
                                if(result){
                                    unsigned compPos = result-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                            else if(type == 3){
                                std::string textureFile;
                                int a, pr;
                                objectStream >> textureFile;
                                objectStream >> a;
                                objectStream >> pr;
                                if(componentCollections.count(COMPONENTTYPES::GRAPHIC) == 0){
                                    componentCollections[COMPONENTTYPES::GRAPHIC] = new Collection<GraphicComponent>(COMPONENTTYPES::GRAPHIC);
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections[COMPONENTTYPES::GRAPHIC]);
                                unsigned result = col->addItem(GraphicComponent(isU, textureFile, a, pr), objId);
                                if(result){
                                    unsigned compPos = result-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                            else if(type == 4){
                                std::string text, font;
                                bool isPlain;
                                int maxlines, linespacing, fontsize, r, g, b, a, priority;
                                std::string command;
                                objectStream >> command;
                                if(command == "BEGIN"){
                                    objectStream >> command;
                                    while(command != "END"){
                                        text += command;
                                        objectStream >> command;
                                        if(command != "END"){
                                            text += " ";
                                        }
                                    }
                                }
                                objectStream >> font;
                                objectStream >> maxlines;
                                objectStream >> linespacing;
                                objectStream >> fontsize;
                                objectStream >> r;
                                objectStream >> g;
                                objectStream >> b;
                                objectStream >> a;
                                objectStream >> priority;
                                objectStream >> isPlain;
                                if(componentCollections.count(COMPONENTTYPES::GRAPHIC) == 0){
                                    componentCollections[COMPONENTTYPES::GRAPHIC] = new Collection<GraphicComponent>(COMPONENTTYPES::GRAPHIC);
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections[COMPONENTTYPES::GRAPHIC]);
                                unsigned result = col->addItem(GraphicComponent(isU, text, font, maxlines, linespacing, fontsize, r, g, b, a, priority, isPlain), objId);
                                if(result){
                                    unsigned compPos = result-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                        }
                        if(compName == "HITBOX"){
                            double offx, offy, w, h;
                            bool checkForColl, isSolid;
                            objectStream >> compEnabled;
                            objectStream >> offx;
                            objectStream >> offy;
                            objectStream >> w;
                            objectStream >> h;
                            objectStream >> isSolid;
                            objectStream >> checkForColl;
                            if(componentCollections.count(COMPONENTTYPES::HITBOX) == 0){
                                componentCollections[COMPONENTTYPES::HITBOX] = new Collection<HitboxComponent>(COMPONENTTYPES::HITBOX);
                            }
                            Collection<HitboxComponent> *col = dynamic_cast<Collection<HitboxComponent>*>(componentCollections[COMPONENTTYPES::HITBOX]);
                            unsigned result = col->addItem(HitboxComponent(offx, offy, w, h, isSolid, checkForColl), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "NETWORK"){
                            objectStream >> compEnabled;
                            bool isS, isT;
                            unsigned maxPacketSize, elapsedTime, maxConnections;
                            int type;
                            objectStream >> type;
                            if(type == 0){
                                if(componentCollections.count(COMPONENTTYPES::NETWORK) == 0){
                                    componentCollections[COMPONENTTYPES::NETWORK] = new Collection<NetworkComponent>(COMPONENTTYPES::NETWORK);
                                }
                                Collection<NetworkComponent> *col = dynamic_cast<Collection<NetworkComponent>*>(componentCollections[COMPONENTTYPES::NETWORK]);
                                unsigned result = col->addItem(NetworkComponent(), objId);
                                if(result){
                                    unsigned compPos = result-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                            else if(type == 1){
                                objectStream >> isS;
                                objectStream >> isT;
                                objectStream >> maxPacketSize;
                                objectStream >> elapsedTime;
                                objectStream >> maxConnections;
                                if(componentCollections.count(COMPONENTTYPES::NETWORK) == 0){
                                    componentCollections[COMPONENTTYPES::NETWORK] = new Collection<NetworkComponent>(COMPONENTTYPES::NETWORK);
                                }
                                Collection<NetworkComponent> *col = dynamic_cast<Collection<NetworkComponent>*>(componentCollections[COMPONENTTYPES::NETWORK]);
                                unsigned result = col->addItem(NetworkComponent(isS, isT, maxPacketSize, elapsedTime, maxConnections), objId);
                                if(result){
                                    unsigned compPos = result-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                        }
                        //Add a pushable, emitted force and gravity flag
                        //Also, the force value and direction for both emitted and gravity
                        if(compName == "PHYSICS"){
                            bool isPushable = false, hasGravity = false, emitsForce = false;
                            double gravityForce = 0, emittedForce = 0;
                            double gravX = 0, gravY = 0, emitX = 0, emitY = 0, effectDistance;
                            objectStream >> compEnabled;
                            objectStream >> isPushable;
                            objectStream >> hasGravity;
                            if(hasGravity){
                                objectStream >> gravityForce;
                                objectStream >> gravX;
                                objectStream >> gravY;
                            }
                            objectStream >> emitsForce;
                            if(emitsForce){
                                objectStream >> emittedForce;
                                objectStream >> effectDistance;
                                objectStream >> emitX;
                                objectStream >> emitY;
                            }
                            if(componentCollections.count(COMPONENTTYPES::PHYSICS) == 0){
                                componentCollections[COMPONENTTYPES::PHYSICS] = new Collection<PhysicsComponent>(COMPONENTTYPES::PHYSICS);
                            }
                            Collection<PhysicsComponent> *col = dynamic_cast<Collection<PhysicsComponent>*>(componentCollections[COMPONENTTYPES::PHYSICS]);
                            unsigned result = col->addItem(PhysicsComponent(isPushable, hasGravity, emitsForce, gravityForce, gravX, gravY, emittedForce, effectDistance, emitX, emitY), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "PROPERTIES"){
                            objectStream >> compEnabled;
                            PropertiesComponent aux;
                            aux.setEnabled(compEnabled);
                            std::string property;
                            objectStream >> property;
                            while(property != "END"){
                                aux.addProperty(property);
                                objectStream >> property;
                            }
                            if(componentCollections.count(COMPONENTTYPES::PROPERTIES) == 0){
                                componentCollections[COMPONENTTYPES::PROPERTIES] = new Collection<PropertiesComponent>(COMPONENTTYPES::PROPERTIES);
                            }
                            Collection<PropertiesComponent> *col = dynamic_cast<Collection<PropertiesComponent>*>(componentCollections[COMPONENTTYPES::PROPERTIES]);
                            unsigned result = col->addItem(aux, objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "SCRIPT"){
                            std::string name;
                            std::string args = "";
                            objectStream >> compEnabled;
                            objectStream >> name;
                            std::getline(objectStream, args);
                            postMessage(new CreateScriptMessage(&componentCollections, &objects[objPos], objPos, compEnabled, name, args));
                        }
                        if(compName == "INPUT"){
                            objectStream >> compEnabled;
                            if(componentCollections.count(COMPONENTTYPES::INPUT) == 0){
                                componentCollections[COMPONENTTYPES::INPUT] = new Collection<InputComponent>(COMPONENTTYPES::INPUT);
                            }
                            Collection<InputComponent> *col = dynamic_cast<Collection<InputComponent>*>(componentCollections[COMPONENTTYPES::INPUT]);
                            unsigned result = col->addItem(InputComponent(), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        if(compName == "TIMER"){
                            double counter, maxTime;
                            bool countDown, paused;
                            objectStream >> compEnabled;
                            objectStream >> counter;
                            objectStream >> maxTime;
                            objectStream >> countDown;
                            objectStream >> paused;
                            if(componentCollections.count(COMPONENTTYPES::TIMER) == 0){
                                componentCollections[COMPONENTTYPES::TIMER] = new Collection<TimerComponent>(COMPONENTTYPES::TIMER);
                            }
                            Collection<TimerComponent> *col = dynamic_cast<Collection<TimerComponent>*>(componentCollections[COMPONENTTYPES::TIMER]);
                            unsigned result = col->addItem(TimerComponent(counter, maxTime, countDown, paused), objId);
                            if(result){
                                unsigned compPos = result-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                    }
                }
                if(objType == "WINDOW"){ //If you read a window
                    std::string title, name, icon;
                    int x, y, w, h;
                    std::string command;
                    objectStream >> command;
                    if(command == "BEGIN"){
                        objectStream >> command;
                        while(command != "END"){
                            title += command;
                            objectStream >> command;
                            if(command != "END"){
                                title += " ";
                            }
                        }
                    }
                    objectStream >> name;
                    objectStream >> icon;
                    objectStream >> x;
                    objectStream >> y;
                    objectStream >> w;
                    objectStream >> h;
                    postMessage(new CreateWindowMessage(title, name, icon, x, y, w, h));
                }
            }
        }
        objectStream.close();
    }
}
