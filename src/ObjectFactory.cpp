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
    ObjectFactory* ObjectFactory::objFactory = nullptr;

    ObjectFactory::ObjectFactory(){

    }

    /**
        Method to get the Object Factory instance, creating it if it doesn't exist yet.
    **/
    ObjectFactory* ObjectFactory::getObjectFactory(){
        if(objFactory == nullptr){
            objFactory = new ObjectFactory();
        }
        return objFactory;
    }

    ObjectFactory::~ObjectFactory(){

    }

    /**
        Like with the cameras, this is the true way of destroying an object factory.
        But in this case it's not because ownership, but because it's a singleton
        class and you have to do it this way.
    **/
    void ObjectFactory::destroyObjectFactory(){
        clearData();
        delete objFactory;
        objFactory = nullptr;
    }

    void ObjectFactory::clearData(){
        for(unsigned i=0; i<objects.size(); i++){
            objects[i].destroyComponents();
        }
        objects.clear();
        for(unsigned i=0; i<cameras.size(); i++){
            cameras[i].destroyComponents();
        }
        cameras.clear();
        for(unsigned i=0; i<postedMessages.size(); i++){
            delete postedMessages[i];
            postedMessages[i] = nullptr;
        }
        postedMessages.clear();
    }

    void ObjectFactory::postMessage(Message *toPost){
        postedMessages.push_back(toPost);
    }

    std::vector<Message*>& ObjectFactory::getMessages(){
        return postedMessages;
    }

    void ObjectFactory::goToState(std::string stateFile){
        postMessage(new InputMessage(M_TYPES::NEWSTATE, E_TYPES::ENULL, stateFile));
    }

    void ObjectFactory::setMaxFPS(int mFPS){
        Framework::getFramework()->setMaxFPS(mFPS);
    }

    void ObjectFactory::setFrameCapFlag(bool frameCap){
        Framework::getFramework()->setFrameCapFlag(frameCap);
    }

    bool ObjectFactory::getFrameCapFlag(){
        return Framework::getFramework()->getFrameCapFlag();
    }

    int ObjectFactory::getMaxFPS(){
        return Framework::getFramework()->getMaxFPS();
    }


    void ObjectFactory::createSaveState(std::string file){
        //export to the file the structure of all the objects existing right now.
        std::fstream objectStream;
        objectStream.open(file, std::ios::out); //Open the stream
        for(unsigned i=0; i<objects.size(); i++){
            objectStream << "OBJECT \n";
            objectStream << objects[i].getEnabled() << " \n";
            objectStream << "NAME " << objects[i].getName() << " \n";
            for(unsigned j=0; j<objects[i].getComponents().size(); j++){
                Component *comp = objects[i].getComponents()[j];
                if(comp->getType() == C_TYPES::ANIMATION){
                    AnimationComponent *aux = dynamic_cast<AnimationComponent*>(comp);
                    objectStream << "ANIMATION \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getX() << " \n";
                    objectStream << aux->getY() << " \n";
                    objectStream << aux->getW() << " \n";
                    objectStream << aux->getH() << " \n";
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::ANIMATOR){
                    AnimatorComponent *aux = dynamic_cast<AnimatorComponent*>(comp);
                    objectStream << "ANIMATOR \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getCurrentState() << " \n";
                    for(std::map<std::string, AnimationState>::iterator k=aux->getStates().begin(); k!=aux->getStates().end(); k++){
                        objectStream << "STATE \n";
                        objectStream << "NAME " << k->first << " \n";
                        objectStream << "TYPE 2 \n";
                        objectStream << k->second.getReverseAnimation() << " \n";
                        objectStream << k->second.getInitialX() << " \n";
                        objectStream << k->second.getInitialY() << " \n";
                        objectStream << k->second.getFrameW() << " \n";
                        objectStream << k->second.getFrameH() << " \n";
                        objectStream << k->second.getTextureW() << " \n";
                        objectStream << k->second.getTextureH() << " \n";
                        objectStream << k->second.getAnimationSpeed() << " \n";
                        objectStream << k->second.getAnimationFrames() << " \n";
                    }
                    objectStream << "END \n";
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::AUDIOC){
                    AudioComponent *aux = dynamic_cast<AudioComponent*>(comp);
                    if(aux->getIsCurrentSong()){
                        aux->setIsPlaying(true);
                    }
                    objectStream << "AUDIO \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << comp->getName() << " \n";
                    objectStream << aux->getAudioFile() << " \n";
                    objectStream << aux->getIsSong() << " \n";
                    objectStream << aux->getIsPlaying() << " \n";
                    objectStream << aux->getLoops() << " \n";
                    aux->setIsPlaying(false);
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::DIMENTION){
                    DimentionComponent *aux = dynamic_cast<DimentionComponent*>(comp);
                    objectStream << "DIMENTION \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getX() << " \n";
                    objectStream << aux->getY() << " \n";
                    objectStream << aux->getW() << " \n";
                    objectStream << aux->getH() << " \n";
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::EMITEDFORCE){
                    EmittedForceComponent *aux = dynamic_cast<EmittedForceComponent*>(comp);
                    objectStream << "EMITEDFORCE \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getX() << " \n";
                    objectStream << aux->getY() << " \n";
                    objectStream << aux->getForce() << " \n";
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::GRAPHIC){
                    GraphicComponent *aux = dynamic_cast<GraphicComponent*>(comp);
                    objectStream << "GRAPHIC \n";
                    objectStream << aux->getEnabled() << " \n";
                    if(!aux->getIsTexture() && !aux->getIsText()){
                        objectStream << "2 \n";
                        objectStream << aux->getR() << " \n";
                        objectStream << aux->getG() << " \n";
                        objectStream << aux->getB() << " \n";
                        objectStream << aux->getAlpha() << " \n";
                        objectStream << aux->getPriority() << " \n";
                    }
                    else if(aux->getIsTexture() && !aux->getIsText()){
                        objectStream << "3 \n";
                        objectStream << aux->getTextureFile() << " \n";
                        objectStream << aux->getAlpha() << " \n";
                        objectStream << aux->getPriority() << " \n";
                    }
                    else if(aux->getIsText()){
                        objectStream << "4 \n";
                        objectStream << "BEGIN " << aux->getText().getText() << " END \n";
                        objectStream << aux->getText().getFont() << " \n";
                        objectStream << aux->getText().getMaxLines() << " \n";
                        objectStream << aux->getText().getLineSpacing() << " \n";
                        objectStream << aux->getText().getIndividualCharacterSize()[0] << " \n";
                        objectStream << aux->getR() << " \n";
                        objectStream << aux->getG() << " \n";
                        objectStream << aux->getB() << " \n";
                        objectStream << aux->getAlpha() << " \n";
                        objectStream << aux->getPriority() << " \n";
                        objectStream << aux->getText().getIsPlain() << " \n";
                    }
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::GRAVITY){
                    GravityComponent *aux = dynamic_cast<GravityComponent*>(comp);
                    objectStream << "GRAVITY \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getX() << " \n";
                    objectStream << aux->getY() << " \n";
                    objectStream << aux->getForce() << " \n";
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::HITBOX){
                    HitboxComponent *aux = dynamic_cast<HitboxComponent*>(comp);
                    objectStream << "HITBOX \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getOffsetX() << " \n";
                    objectStream << aux->getOffsetY() << " \n";
                    objectStream << aux->getW() << " \n";
                    objectStream << aux->getH() << " \n";
                    objectStream << aux->getCheckForCollisions() << " \n";
                    aux = nullptr;
                }
                if(comp->getType() == C_TYPES::PHYSICS){
                    objectStream << "PHYSICS \n";
                    objectStream << comp->getEnabled() << " \n";
                }
                if(comp->getType() == C_TYPES::PLAYER){
                    objectStream << "PLAYER \n";
                    objectStream << comp->getEnabled() << " \n";
                }
                if(comp->getType() == C_TYPES::PUSHABLE){
                    objectStream << "PUSHABLE \n";
                    objectStream << comp->getEnabled() << " \n";
                }
                if(comp->getType() == C_TYPES::SCRIPT){
                    objectStream << "SCRIPT \n";
                    objectStream << comp->getEnabled() << " \n";
                    objectStream << comp->getName() << " \n";
                }
                if(comp->getType() == C_TYPES::SOLID){
                    objectStream << "SOLID \n";
                    objectStream << comp->getEnabled() << " \n";
                }
                if(comp->getType() == C_TYPES::UI){
                    objectStream << "UI \n";
                    objectStream << comp->getEnabled() << " \n";
                }
                if(comp->getType() == C_TYPES::INPUTC){
                    objectStream << "INPUT \n";
                    objectStream << comp->getEnabled() << " \n";
                }
                if(comp->getType() == C_TYPES::TIMERC){
                    TimerComponent *aux = dynamic_cast<TimerComponent*>(comp);
                    objectStream << "TIMER \n";
                    objectStream << aux->getEnabled() << " \n";
                    objectStream << aux->getCounter() << " \n";
                    objectStream << aux->getMaxTime() << " \n";
                    objectStream << aux->getCountDown() << " \n";
                    objectStream << aux->getPaused() << " \n";
                    aux = nullptr;
                }
                comp = nullptr;
            }
            objectStream << "END \n";
        }
        for(unsigned i=0; i<cameras.size(); i++){
            objectStream << "CAMERA \n";
            objectStream << cameras[i].getEnabled() << " \n";
            objectStream << cameras[i].getName() << " \n";
            for(unsigned j=0; j<cameras[i].getComponents().size(); j++){
                Component* comp = cameras[i].getComponents()[j];
                if(comp->getType() == C_TYPES::DIMENTION){
                    DimentionComponent *dim = dynamic_cast<DimentionComponent*>(comp);
                    objectStream << "DIMENTION \n";
                    objectStream << dim->getEnabled() << " \n";
                    objectStream << dim->getX() << " \n";
                    objectStream << dim->getY() << " \n";
                    objectStream << dim->getW() << " \n";
                    objectStream << dim->getH() << " \n";
                    dim = nullptr;
                }
                if(comp->getType() == C_TYPES::SCRIPT){
                    objectStream << "SCRIPT \n";
                    objectStream << comp->getEnabled() << " \n";
                    objectStream << comp->getName() << " \n";
                }
                if(comp->getType() == C_TYPES::CAMERA){
                    CameraComponent *dim = dynamic_cast<CameraComponent*>(comp);
                    objectStream << "CAMERACOMP \n";
                    objectStream << dim->getEnabled() << " \n";
                    objectStream << dim->getX() << " \n";
                    objectStream << dim->getY() << " \n";
                    dim = nullptr;
                }
                if(comp->getType() == C_TYPES::TIMERC){
                    TimerComponent *dim = dynamic_cast<TimerComponent*>(comp);
                    objectStream << "TIMER \n";
                    objectStream << dim->getEnabled() << " \n";
                    objectStream << dim->getCounter() << " \n";
                    objectStream << dim->getMaxTime() << " \n";
                    objectStream << dim->getCountDown() << " \n";
                    objectStream << dim->getPaused() << " \n";
                    dim = nullptr;
                }
                comp = nullptr;
            }
            objectStream << "END \n";
        }
        for(unsigned i=0; i<Framework::getFramework()->getWindowCount(); i++){
            std::string wName = Framework::getFramework()->getWindowName(i);
            if(wName != "mainWindow"){
                int x, y, w, h;
                Framework::getFramework()->getWindowSize(wName, w, h);
                Framework::getFramework()->getWindowPosition(wName, x, y);
                objectStream << "WINDOW \n";
                objectStream << "BEGIN " << Framework::getFramework()->getWindowTitle(wName) << " END \n";
                objectStream << wName << " \n";
                objectStream << x << " \n";
                objectStream << y << " \n";
                objectStream << w << " \n";
                objectStream << h << " \n";
                objectStream << "END \n";
            }
        }
        objectStream.close();
    }

    void ObjectFactory::loadSaveState(std::string file){
        postMessage(new InputMessage(M_TYPES::LOADSTATE, E_TYPES::ENULL, file));
    }

    /**
        Method that reads a string in search of a configuration structure to create an object, a window or a camera.
    **/
    void ObjectFactory::createObject(std::string data){
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
                        DimentionComponent *aux = new DimentionComponent(x, y, w, h);
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
                        DimentionComponent *aux = new DimentionComponent(x, y, w, h);
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
    }

    /**
        Method that reads a file in search of configuration structures to create objects, windows or cameras.
    **/
    void ObjectFactory::createObjects(std::string file){ //And you'd format the data map here for each object in the file
        std::fstream objectStream;
        objectStream.open(file, std::ios::in); //Open the stream
        if(objectStream){ //if file could be accessed
            //Load objects and stuff
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
                            DimentionComponent *aux = new DimentionComponent(x, y, w, h);
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
                            std::string args = "";
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
                            DimentionComponent *aux = new DimentionComponent(x, y, w, h);
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
        }
        objectStream.close();
    }

    /**
        Deletes an object in the passed position destroying its components in the process.
    **/
    void ObjectFactory::deleteObject(int pos){
        if(pos > -1 && pos < objects.size())
            objects[pos].destroyComponents();
            objects.erase(objects.begin() + pos);
    }

    /**
        Deletes an object with the passed name destroying its components in the process.
    **/
    void ObjectFactory::deleteObject(std::string name){
        int pos = getObjectPos(name);
        if(pos > -1 && pos < objects.size())
            objects[pos].destroyComponents();
            objects.erase(objects.begin() + pos);
    }

    /**
        Inserts an object into the object pool, but before doing that, it checks if the name
        is unique and if it isn't it changes it until it is.
    **/
    void ObjectFactory::insertObject(GameObject obj){
        int i=1;
        std::string auxName = obj.getName();
        while(getObject(auxName) != nullptr || auxName == ""){
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
    }

    /**
        Destroys all the objects and sets them to the objects passed.
    **/
    void ObjectFactory::setObjects(std::vector<GameObject>& obj){
        for(unsigned i=0; i<objects.size(); i++){
            objects[i].destroyComponents();
        }
        objects.clear();
        objects = obj;
    }

    /**
        Returns an object by position or a nullptr if the object doesn't exist.
    **/
    GameObject* ObjectFactory::getObject(int pos){
        if(pos > -1 && pos < objects.size()){
            return &objects[pos];
        }
        return nullptr;
    }

    /**
        Returns an object by name or a nullptr if the object doesn't exist.
    **/
    GameObject* ObjectFactory::getObject(std::string name){
        int pos = getObjectPos(name);
        if(pos > -1 && pos < objects.size()){
            return &objects[pos];
        }
        return nullptr;
    }

    /**
        Returns the object position for that name if it exists.
        Returns -1 if it doesn't exist.
    **/
    int ObjectFactory::getObjectPos(std::string name){
        for(unsigned i=0; i<objects.size(); i++){
            if(objects[i].getName() == name){
                return i;
            }
        }
        return -1;
    }

    /**
        Returns the objects vector.
    **/
    std::vector<GameObject>& ObjectFactory::getObjects(){
        return objects;
    }

    /**
        Method used to determine if a camera is greater than the other in terms of priority.
        A lower priority will make the graphic system render that camera first, being behind of the next ones.
    **/
    bool camerasSorting(Camera &a, Camera &b){
        return a.getPriority() < b.getPriority();
    }

    /**
        Method to sort the cameras by priority.
    **/
    void ObjectFactory::sortCameras(){
        std::sort(cameras.begin(), cameras.end(), camerasSorting);
    }

    /**
        Method to add a camera.
        It checks if there's a camera already using that name or if it has a name.
        If the name exists or it doesn't have one, it changes it until it fins an
        available name.
    **/
    void ObjectFactory::addCamera(Camera cam){
        int i=1;
        std::string auxName = cam.getName();
        while(getCamera(auxName) != nullptr || auxName == ""){
            auxName = cam.getName();
            if(auxName == ""){
                auxName = "Camera";
                cam.setName("Camera");
            }
            else{
                auxName.append(Utils::intToString(i));
                i++;
            }
        }
        cam.setName(auxName);
        cameras.push_back(cam);
        sortCameras();
    }

    /**
        Method to get a camera by its name.
    **/
    Camera* ObjectFactory::getCamera(std::string name){
        for(unsigned i=0; i<cameras.size(); i++){
            if(cameras[i].getName() == name){
                return &cameras[i];
            }
        }
        return nullptr;
    }

    /**
        Method to get a camera by its position on the vector.
    **/
    Camera* ObjectFactory::getCamera(int pos){
        if(pos > -1 && pos < cameras.size())
            return &cameras[pos];
        return nullptr;
    }

    /**
        Method to delete a camera by its name.
    **/
    void ObjectFactory::deleteCamera(std::string name){
        for(unsigned i=0; i<cameras.size(); i++){
            if(cameras[i].getName() == name){
                cameras[i].destroyComponents();
                cameras.erase(cameras.begin() + i);
            }
        }
    }

    /**
        Method to return the cameras vector.
    **/
    std::vector<Camera>& ObjectFactory::getCameras(){
        return cameras;
    }

    /**
        Layer to delete a window for the scripts to be able to call it without accessing the framework.
    **/
    void ObjectFactory::deleteWindow(std::string name){
        Framework::getFramework()->deleteWindow(name);
    }

    /**
        Layer to create a window for the scripts to be able to call it without accessing the framework.
    **/
    void ObjectFactory::createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h){
        Framework::getFramework()->createWindow(title, name, icon, x, y, w, h);
    }

    /**
        Layer to get a windows size for the scripts to be able to call it without accessing the framework.
    **/
    void ObjectFactory::getWindowSize(std::string windowN, int &w, int &h){
        Framework::getFramework()->getWindowSize(windowN, w, h);
    }

    void ObjectFactory::update(double dt){

    }

    void ObjectFactory::handleMessage(Message* msg){
        switch(msg->getType()){
            case M_TYPES::ADDOBJ:
                break;
            case M_TYPES::QUIT:
                postMessage(new Message(M_TYPES::QUIT));
                break;
        }
    }
}
