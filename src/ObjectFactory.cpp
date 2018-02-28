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

#include "ObjectFactory.hpp"

namespace WishEngine{
    ObjectFactory::ObjectFactory(){
        setSystemType("OBJECTFACTORY");
    }

    ObjectFactory::~ObjectFactory(){
        clearData();
        destroySystem();
    }

    void ObjectFactory::clearData(){
        objects.clear();
        for(std::map<std::string, BaseCollection*>::iterator it = componentCollections.begin(); it != componentCollections.end(); it++){
            delete it->second;
            it->second = nullptr;
        }
        componentCollections.clear();
    }

    void ObjectFactory::update(double dt){
        //Send the objects and components pointer.
        postMessage(new ObjectListMessage("OBJECTLIST", &objects));
        postMessage(new ComponentListMessage("COMPONENTLIST", &componentCollections));
    }

    void ObjectFactory::handleMessage(Message* msg){
        if(msg->getType() == "LOADOBJECTS"){
            loadObjects(msg->getValue());
        }
        else if(msg->getType() == "OBJECTDELETED"){
            deleteObject(Utils::stringToInt(msg->getValue()));
        }
    }

    void ObjectFactory::deleteObject(unsigned objPos){
        //Set all the objects components as Deleted
    }

    /**
        Method to check if an object name is already in use
    **/
    std::string ObjectFactory::checkObjectName(std::string &name){
        std::string finalName = name;
        if(finalName == ""){
            finalName = "Object";
        }
        int modifier = 1;
        for(int i=0; i<objects.size(); i++){
            if(objects[i].getName() == finalName){
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
                    objects.emplace_back(objName, objEnabled);
                    objPos = objects.size() - 1;
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
                            if(componentCollections.count("ANIMATION") == 0){
                                componentCollections["ANIMATION"] = new Collection<AnimationComponent>("ANIMATION");
                            }
                            Collection<AnimationComponent> *col = dynamic_cast<Collection<AnimationComponent>*>(componentCollections["ANIMATION"]);
                            unsigned result = col->addItem(AnimationComponent(x, y, w, h), objId);
                            if(result){
                                unsigned compPos = col->getCollection().size()-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        /**if(compName == "ANIMATOR"){
                            std::string animatorStates, initialState;
                            AnimatorComponent* newAnimator = new AnimatorComponent();
                            objectStream >> compEnabled;
                            objectStream >> initialState;
                            newAnimator->setCurrentState(initialState);
                            newAnimator->setEnabled(compEnabled);
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
                                    newAnimator->addAnimationState(stateName, newState); //We add the state
                                }
                            }
                            newObject.addComponent(newAnimator);
                            newAnimator = nullptr;
                        }**/
                        /**if(compName == "AUDIO"){
                            std::string audioFile, name="Audio";
                            bool isSong, isPlaying;
                            int loops;
                            objectStream >> compEnabled;
                            objectStream >> name;
                            objectStream >> audioFile;
                            objectStream >> isSong;
                            objectStream >> isPlaying;
                            objectStream >> loops;
                            AudioComponent *aux = new AudioComponent(audioFile, isSong, isPlaying, loops);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux, name);
                            aux = nullptr;
                        }**/
                        if(compName == "CAMERA"){
                            double x, y;
                            int prior;
                            objectStream >> compEnabled;
                            objectStream >> prior;
                            objectStream >> x;
                            objectStream >> y;
                            if(componentCollections.count("CAMERA") == 0){
                                componentCollections["CAMERA"] = new Collection<CameraComponent>("CAMERA");
                            }
                            Collection<CameraComponent> *col = dynamic_cast<Collection<CameraComponent>*>(componentCollections["CAMERA"]);
                            unsigned result = col->addItem(CameraComponent(prior, x, y), objId);
                            if(result){
                                unsigned compPos = col->getCollection().size()-1;
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
                            if(componentCollections.count("DIMENSION") == 0){
                                componentCollections["DIMENSION"] = new Collection<DimensionComponent>("DIMENSION");
                            }
                            Collection<DimensionComponent> *col = dynamic_cast<Collection<DimensionComponent>*>(componentCollections["DIMENSION"]);
                            unsigned result = col->addItem(DimensionComponent(x, y, w, h), objId);
                            if(result){
                                unsigned compPos = col->getCollection().size()-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        /**if(compName == "EMITEDFORCE"){
                            double x, y, force;
                            objectStream >> compEnabled;
                            objectStream >> x;
                            objectStream >> y;
                            objectStream >> force;
                            EmittedForceComponent *aux = new EmittedForceComponent(x, y, force);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }**/
                        if(compName == "GRAPHIC"){
                            int type;
                            bool isU;
                            objectStream >> compEnabled;
                            objectStream >> isU;
                            objectStream >> type;
                            if(type == 1){
                                if(componentCollections.count("GRAPHIC") == 0){
                                    componentCollections["GRAPHIC"] = new Collection<GraphicComponent>("GRAPHIC");
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections["GRAPHIC"]);
                                unsigned result = col->addItem(GraphicComponent(isU), objId);
                                if(result){
                                    unsigned compPos = col->getCollection().size()-1;
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
                                if(componentCollections.count("GRAPHIC") == 0){
                                    componentCollections["GRAPHIC"] = new Collection<GraphicComponent>("GRAPHIC");
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections["GRAPHIC"]);
                                unsigned result = col->addItem(GraphicComponent(isU, r, g, b, a, pr), objId);
                                if(result){
                                    unsigned compPos = col->getCollection().size()-1;
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
                                if(componentCollections.count("GRAPHIC") == 0){
                                    componentCollections["GRAPHIC"] = new Collection<GraphicComponent>("GRAPHIC");
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections["GRAPHIC"]);
                                unsigned result = col->addItem(GraphicComponent(isU, textureFile, a, pr), objId);
                                if(result){
                                    unsigned compPos = col->getCollection().size()-1;
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
                                if(componentCollections.count("GRAPHIC") == 0){
                                    componentCollections["GRAPHIC"] = new Collection<GraphicComponent>("GRAPHIC");
                                }
                                Collection<GraphicComponent> *col = dynamic_cast<Collection<GraphicComponent>*>(componentCollections["GRAPHIC"]);
                                unsigned result = col->addItem(GraphicComponent(isU, text, font, maxlines, linespacing, fontsize, r, g, b, a, priority, isPlain), objId);
                                if(result){
                                    unsigned compPos = col->getCollection().size()-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                        }
                        /**if(compName == "GRAVITY"){
                            double x, y, force;
                            objectStream >> compEnabled;
                            objectStream >> x;
                            objectStream >> y;
                            objectStream >> force;
                            GravityComponent *aux = new GravityComponent(x, y, force);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }**/
                        /**if(compName == "HITBOX"){
                            double offx, offy, w, h;
                            bool checkForColl;
                            objectStream >> compEnabled;
                            objectStream >> offx;
                            objectStream >> offy;
                            objectStream >> w;
                            objectStream >> h;
                            objectStream >> checkForColl;
                            HitboxComponent *aux = new HitboxComponent(offx, offy, w, h, checkForColl);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }**/
                        if(compName == "NETWORK"){
                            objectStream >> compEnabled;
                            bool isS, isT;
                            unsigned maxPacketSize, elapsedTime, maxConnections;
                            int type;
                            objectStream >> type;
                            if(type == 0){
                                if(componentCollections.count("NETWORK") == 0){
                                    componentCollections["NETWORK"] = new Collection<NetworkComponent>("NETWORK");
                                }
                                Collection<NetworkComponent> *col = dynamic_cast<Collection<NetworkComponent>*>(componentCollections["NETWORK"]);
                                unsigned result = col->addItem(NetworkComponent(), objId);
                                if(result){
                                    unsigned compPos = col->getCollection().size()-1;
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
                                if(componentCollections.count("NETWORK") == 0){
                                    componentCollections["NETWORK"] = new Collection<NetworkComponent>("NETWORK");
                                }
                                Collection<NetworkComponent> *col = dynamic_cast<Collection<NetworkComponent>*>(componentCollections["NETWORK"]);
                                unsigned result = col->addItem(NetworkComponent(isS, isT, maxPacketSize, elapsedTime, maxConnections), objId);
                                if(result){
                                    unsigned compPos = col->getCollection().size()-1;
                                    std::string compN = col->getCollection()[compPos].getName();
                                    objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                    col->getCollection()[compPos].setEnabled(compEnabled);
                                    col->getCollection()[compPos].setOwnerId(objId);
                                    col->getCollection()[compPos].setOwnerPos(objPos);
                                }
                                col = nullptr;
                            }
                        }
                        /**if(compName == "PHYSICS"){
                            objectStream >> compEnabled;
                            PhysicsComponent *aux = new PhysicsComponent();
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                            VelocityComponent *aux2 = new VelocityComponent();
                            aux2->setEnabled(compEnabled);
                            newObject.addComponent(aux2);
                            aux2 = nullptr;
                        }**/
                        /**if(compName == "PROPERTIES"){
                            objectStream >> compEnabled;
                            PropertiesComponent *aux = new PropertiesComponent();
                            aux->setEnabled(compEnabled);
                            std::string property;
                            objectStream >> property;
                            while(property != "END"){
                                aux->addProperty(property);
                                objectStream >> property;
                            }
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }**/
                        /**if(compName == "PUSHABLE"){
                            objectStream >> compEnabled;
                            PushableComponent *aux = new PushableComponent();
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }**/
                        if(compName == "SCRIPT"){
                            std::string name;
                            std::string args = "";
                            objectStream >> compEnabled;
                            objectStream >> name;
                            std::getline(objectStream, args);
                            if(componentCollections.count("SCRIPT") == 0){
                                componentCollections["SCRIPT"] = new Collection<BaseCollection*>("SCRIPT");
                            }
                            Collection<BaseCollection*> *col = dynamic_cast<Collection<BaseCollection*>*>(componentCollections["SCRIPT"]);
                            postMessage(new CreateScriptMessage(col, &objects[objPos], objPos, compEnabled, name, args));
                            col = nullptr;
                        }
                        if(compName == "INPUT"){
                            objectStream >> compEnabled;
                            InputComponent *aux = new InputComponent();
                            if(componentCollections.count("INPUT") == 0){
                                componentCollections["INPUT"] = new Collection<InputComponent>("INPUT");
                            }
                            Collection<InputComponent> *col = dynamic_cast<Collection<InputComponent>*>(componentCollections["INPUT"]);
                            unsigned result = col->addItem(InputComponent(), objId);
                            if(result){
                                unsigned compPos = col->getCollection().size()-1;
                                std::string compN = col->getCollection()[compPos].getName();
                                objects[objPos].getComponents().emplace_back(col->getCollection()[compPos].getType(), compN, compPos);
                                col->getCollection()[compPos].setEnabled(compEnabled);
                                col->getCollection()[compPos].setOwnerId(objId);
                                col->getCollection()[compPos].setOwnerPos(objPos);
                            }
                            col = nullptr;
                        }
                        /**if(compName == "TIMER"){
                            double counter, maxTime;
                            bool countDown, paused;
                            objectStream >> compEnabled;
                            objectStream >> counter;
                            objectStream >> maxTime;
                            objectStream >> countDown;
                            objectStream >> paused;
                            TimerComponent *aux = new TimerComponent(counter, maxTime, countDown, paused);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }**/
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

    /**
        Method that reads a string in search of a configuration structure to create an object, a window or a camera.
    **/
    /**void ObjectFactory::createObject(std::string data){
        std::stringstream objectStream(data);
        std::string objType, objName;
        while(objectStream >> objType){ //While an object, a window or a camera is read
            if(objType == "OBJECT"){ //If you read an object
                std::string compName;
                bool objEnabled;
                GameObject newObject = GameObject();
                objectStream >> objEnabled;
                newObject.setEnabled(objEnabled);
                while(objectStream >> compName){ //While you don't reach the end of the object declaration
                    bool compEnabled;
                    if(compName == "END"){ //If you read END, that means end of declaration so you quit the loop
                        break;
                    }
                    if(compName == "NAME"){
                        std::string strValue;
                        objectStream >> strValue;
                        newObject.setName(strValue);
                    }
                    if(compName == "ANIMATION"){
                        double x, y, w, h;
                        objectStream >> compEnabled;
                        objectStream >> x;
                        objectStream >> y;
                        objectStream >> w;
                        objectStream >> h;
                        AnimationComponent *aux = new AnimationComponent(x, y, w, h);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "ANIMATOR"){
                        std::string animatorStates, initialState;
                        AnimatorComponent* newAnimator = new AnimatorComponent();
                        objectStream >> compEnabled;
                        objectStream >> initialState;
                        newAnimator->setCurrentState(initialState);
                        newAnimator->setEnabled(compEnabled);
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
                                newAnimator->addAnimationState(stateName, newState); //We add the state
                            }
                        }
                        newObject.addComponent(newAnimator);
                        newAnimator = nullptr;
                    }
                    if(compName == "AUDIO"){
                        std::string audioFile, name="Audio";
                        bool isSong, isPlaying;
                        int loops;
                        objectStream >> compEnabled;
                        objectStream >> name;
                        objectStream >> audioFile;
                        objectStream >> isSong;
                        objectStream >> isPlaying;
                        objectStream >> loops;
                        AudioComponent *aux = new AudioComponent(audioFile, isSong, isPlaying, loops);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux, name);
                        aux = nullptr;
                    }
                    if(compName == "DIMENTION"){
                        double x, y, w, h;
                        objectStream >> compEnabled;
                        objectStream >> x;
                        objectStream >> y;
                        objectStream >> w;
                        objectStream >> h;
                        DimensionComponent *aux = new DimensionComponent(x, y, w, h);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "EMITEDFORCE"){
                        double x, y, force;
                        objectStream >> compEnabled;
                        objectStream >> x;
                        objectStream >> y;
                        objectStream >> force;
                        EmittedForceComponent *aux = new EmittedForceComponent(x, y, force);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "GRAPHIC"){
                        int type;
                        objectStream >> compEnabled;
                        objectStream >> type;
                        if(type == 1){
                            GraphicComponent *aux = new GraphicComponent();
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }
                        if(type == 2){
                            int r, g, b, a, pr;
                            objectStream >> r;
                            objectStream >> g;
                            objectStream >> b;
                            objectStream >> a;
                            objectStream >> pr;
                            GraphicComponent *aux = new GraphicComponent(r, g, b, a, pr);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }
                        if(type == 3){
                            std::string textureFile;
                            int a, pr;
                            objectStream >> textureFile;
                            objectStream >> a;
                            objectStream >> pr;
                            GraphicComponent *aux = new GraphicComponent(textureFile, a, pr);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }
                        if(type == 4){
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
                            GraphicComponent *aux = new GraphicComponent(text, font, maxlines, linespacing, fontsize, r, g, b, a, priority, isPlain);
                            aux->setEnabled(compEnabled);
                            newObject.addComponent(aux);
                            aux = nullptr;
                        }
                    }
                    if(compName == "GRAVITY"){
                        double x, y, force;
                        objectStream >> compEnabled;
                        objectStream >> x;
                        objectStream >> y;
                        objectStream >> force;
                        GravityComponent *aux = new GravityComponent(x, y, force);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "HITBOX"){
                        double offx, offy, w, h;
                        bool checkForColl;
                        objectStream >> compEnabled;
                        objectStream >> offx;
                        objectStream >> offy;
                        objectStream >> w;
                        objectStream >> h;
                        objectStream >> checkForColl;
                        HitboxComponent *aux = new HitboxComponent(offx, offy, w, h, checkForColl);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "PHYSICS"){
                        objectStream >> compEnabled;
                        PhysicsComponent *aux = new PhysicsComponent();
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                        VelocityComponent *aux2 = new VelocityComponent();
                        aux2->setEnabled(compEnabled);
                        newObject.addComponent(aux2);
                        aux2 = nullptr;
                    }
                    if(compName == "PLAYER"){
                        objectStream >> compEnabled;
                        PlayerComponent *aux = new PlayerComponent();
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "PROPERTIES"){
                        objectStream >> compEnabled;
                        PropertiesComponent *aux = new PropertiesComponent();
                        aux->setEnabled(compEnabled);
                        std::string property;
                        objectStream >> property;
                        while(property != "END"){
                            aux->addProperty(property);
                            objectStream >> property;
                        }
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "PUSHABLE"){
                        objectStream >> compEnabled;
                        PushableComponent *aux = new PushableComponent();
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "SCRIPT"){
                        std::string name;
                        std::string args;
                        objectStream >> compEnabled;
                        objectStream >> name;
                        std::getline(objectStream, args);
                        ScriptComponent* newScript = getScript(name, args);
                        if(newScript != nullptr){
                            newScript->setEnabled(compEnabled);
                            newObject.addComponent(newScript, name);
                        }
                        newScript = nullptr;
                    }
                    if(compName == "SOLID"){
                        objectStream >> compEnabled;
                        SolidComponent *aux = new SolidComponent();
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "UI"){
                        objectStream >> compEnabled;
                        UIComponent *aux = new UIComponent();
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "INPUT"){
                        objectStream >> compEnabled;
                        InputComponent *aux = new InputComponent();
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "TIMER"){
                        double counter, maxTime;
                        bool countDown, paused;
                        objectStream >> compEnabled;
                        objectStream >> counter;
                        objectStream >> maxTime;
                        objectStream >> countDown;
                        objectStream >> paused;
                        TimerComponent *aux = new TimerComponent(counter, maxTime, countDown, paused);
                        aux->setEnabled(compEnabled);
                        newObject.addComponent(aux);
                        aux = nullptr;
                    }
                }
                insertObject(newObject);
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
                Framework::getFramework()->createWindow(title, name, icon, x, y, w, h);
            }
            //Make the cameras building thing
            if(objType == "CAMERA"){
                bool camEnabled;
                std::string compName, camName;
                Camera newCamera = Camera();
                objectStream >> camEnabled;
                newCamera.setEnabled(camEnabled);
                objectStream >> camName;
                newCamera.setName(camName);
                while(objectStream >> compName){ //While you don't reach the end of the object declaration
                    bool compEnabled;
                    if(compName == "END"){ //If you read END, that means end of declaration so you quit the loop
                        break;
                    }
                    if(compName == "DIMENTION"){
                        double x, y, w, h;
                        objectStream >> compEnabled;
                        objectStream >> x;
                        objectStream >> y;
                        objectStream >> w;
                        objectStream >> h;
                        DimensionComponent *aux = new DimensionComponent(x, y, w, h);
                        aux->setEnabled(compEnabled);
                        newCamera.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "SCRIPT"){
                        std::string name;
                        std::string args;
                        objectStream >> compEnabled;
                        objectStream >> name;
                        std::getline(objectStream, args);
                        ScriptComponent* newScript = getScript(name, args);
                        if(newScript != nullptr){
                            newScript->setEnabled(compEnabled);
                            newCamera.addComponent(newScript, name);
                        }
                        newScript = nullptr;
                    }
                    if(compName == "CAMERACOMP"){
                        double x, y;
                        objectStream >> compEnabled;
                        objectStream >> x;
                        objectStream >> y;
                        CameraComponent *aux = new CameraComponent(x, y);
                        aux->setEnabled(compEnabled);
                        newCamera.addComponent(aux);
                        aux = nullptr;
                    }
                    if(compName == "TIMER"){
                        double counter, maxTime;
                        bool countDown, paused;
                        objectStream >> compEnabled;
                        objectStream >> counter;
                        objectStream >> maxTime;
                        objectStream >> countDown;
                        objectStream >> paused;
                        TimerComponent *aux = new TimerComponent(counter, maxTime, countDown, paused);
                        aux->setEnabled(compEnabled);
                        newCamera.addComponent(aux);
                        aux = nullptr;
                    }
                }
                addCamera(newCamera);
            }
        }
        objectStream.clear();
    }**/

    /**
        Deletes an object in the passed position destroying its components in the process.
    **/
    /**void ObjectFactory::deleteObject(unsigned id){
        for(unsigned i=0; i<objects.size(); i++){
            if(objects[i].getId() == id){
                //Send message saying the object with id id was deleted
                objects.erase(objects.begin() + i);
                break;
            }
        }
    }**/

    /**
        Deletes an object with the passed name destroying its components in the process.
    **/
    /**void ObjectFactory::deleteObject(std::string name){
        for(unsigned i=0; i<objects.size(); i++){
            if(objects[i].getName() == name){
                //Send message saying the object with id getId() was deleted
                objects.erase(objects.begin() + i);
                break;
            }
        }
    }**/

    /**
        Inserts an object into the object pool, but before doing that, it checks if the name
        is unique and if it isn't it changes it until it is.
    **/
    /**void ObjectFactory::insertObject(GameObject obj){
        int i=1;
        std::string auxName = obj.getName();
        while(getObject(auxName) != NULL || auxName == ""){
            auxName = obj.getName();
            if(auxName == ""){
                auxName = "Object";
                obj.setName("Object");
            }
            else{
                auxName += Utils::intToString(i);
                i++;
            }
        }
        obj.setName(auxName);
        objects.push_back(obj); //This for some reason makes the objects component count to 0
    }**/

    /**
        Returns an object by position or a nullptr if the object doesn't exist.
    **/
    /**GameObject *ObjectFactory::getObject(unsigned id){
        for(unsigned i=0; i<objects.size(); i++){
            if(objects[i].getId() == id){
                return &objects[i];
            }
        }
        return nullptr;
    }**/

    /**
        Returns an object by name or a nullptr if the object doesn't exist.
    **/
    /**GameObject *ObjectFactory::getObject(std::string name){
        for(unsigned i=0; i<objects.size(); i++){
            if(objects[i].getName() == name){
                return &objects[i];
            }
        }
        return nullptr;
    }**/

    /**
        Returns the objects vector.
    **/
    /**std::vector<GameObject>& ObjectFactory::getObjects(){
        return objects;
    }

    std::map<std::string, BaseCollection*> &ObjectFactory::getComponentCollections(){
        return componentCollections;
    }**/

    /**
        Layer to delete a window for the scripts to be able to call it without accessing the framework.
    **/
    /**void ObjectFactory::deleteWindow(std::string name){
        Framework::getFramework()->deleteWindow(name);
    }**/

    /**
        Layer to create a window for the scripts to be able to call it without accessing the framework.
    **/
    /**void ObjectFactory::createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h){
        Framework::getFramework()->createWindow(title, name, icon, x, y, w, h);
    }**/

    /**
        Layer to get a windows size for the scripts to be able to call it without accessing the framework.
    **/
    /**void ObjectFactory::getWindowSize(std::string windowN, int &w, int &h){
        Framework::getFramework()->getWindowSize(windowN, w, h);
    }**/
}
