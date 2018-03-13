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

#include "ScriptsInterface.hpp"

//Here I include all the scripts source files

namespace WishEngine{
    std::unordered_map<int, BaseCollection*> *ScriptsInterface::components = nullptr;

    std::vector<GameObject> *ScriptsInterface::objects = nullptr;

    std::vector<Message*> *ScriptsInterface::postedM = nullptr;

    std::vector<unsigned> *ScriptsInterface::availableObjects = nullptr;

    ScriptsInterface::ScriptsInterface(){
        setSystemType("SCRIPTINTERFACE");
        postedM = &getMessages();
    }

    ScriptsInterface::~ScriptsInterface(){
        components = nullptr;
        objects = nullptr;
        postedM = nullptr;
        availableObjects = nullptr;
        destroySystem();
    }

    void ScriptsInterface::insertObject(std::string name, bool isEnabled){
        if(name == ""){
            name = "Object";
        }
        std::string finalName = name;
        int modifier = 1;
        for(int i=0; i<objects->size(); i++){
            if(!(*objects)[i].getDeleted() && (*objects)[i].getName() == finalName){
                finalName = name;
                finalName += Utils::intToString(modifier);
                i = -1;
                modifier++;
            }
        }
        if(availableObjects == nullptr || availableObjects->empty()){
            objects->emplace_back(finalName, isEnabled);
            if(objects->size()-1 <= 0){
                (*objects)[objects->size() - 1].setId(0);
            }
            else{
                (*objects)[objects->size()-1].setId((*objects)[objects->size()-2].getId() + 1);
            }
        }
        else{
            unsigned objPos = (*availableObjects)[availableObjects->size() - 1];
            unsigned oldId = (*objects)[objPos].getId();
            (*objects)[objPos] = GameObject(finalName, isEnabled);
            (*objects)[objPos].setId(oldId);
            availableObjects->pop_back();
        }
    }

    unsigned ScriptsInterface::getObjectQuantity(){
        if(objects != nullptr){
            return objects->size();
        }
        return 0;
    }

    unsigned ScriptsInterface::deleteObject(std::string name){
        for(unsigned i=0; i<objects->size(); i++){
            if((*objects)[i].getName() == name && !(*objects)[i].getDeleted()){
                (*objects)[i].setDeleted(true);
                for(unsigned j=0; j<(*objects)[i].getComponents().size(); j++){
                    if(components != nullptr && (*components).count((*objects)[i].getComponents()[j].componentType) > 0){
                        //Check for each script
                        /** TEMPLATE:
                        if((*objects)[i].getComponents()[j].componentType == SCRIPTTYPES::YOURSCRIPT){
                            dynamic_cast<Collection<YourScript>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        **/
                        //End of custom scripts
                        if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::ANIMATION){
                            dynamic_cast<Collection<AnimationComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::CAMERA){
                            dynamic_cast<Collection<CameraComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::DIMENSION){
                            dynamic_cast<Collection<DimensionComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::GRAPHIC){
                            dynamic_cast<Collection<GraphicComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::INPUT){
                            dynamic_cast<Collection<InputComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::NETWORK){
                            dynamic_cast<Collection<NetworkComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::AUDIO){
                            dynamic_cast<Collection<AudioComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::HITBOX){
                            dynamic_cast<Collection<HitboxComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::ANIMATOR){
                            dynamic_cast<Collection<AnimatorComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::TIMER){
                            dynamic_cast<Collection<TimerComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::PROPERTIES){
                            dynamic_cast<Collection<PropertiesComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::PHYSICS){
                            dynamic_cast<Collection<PhysicsComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                    }
                }
                availableObjects->push_back(i);
                return 1;
            }
        }
        return 0;
    }

    unsigned ScriptsInterface::deleteObject(unsigned id){
        for(unsigned i=0; i<objects->size(); i++){
            if((*objects)[i].getId() == id && !(*objects)[i].getDeleted()){
                (*objects)[i].setDeleted(true);
                for(unsigned j=0; j<(*objects)[i].getComponents().size(); j++){
                    if(components != nullptr && (*components).count((*objects)[i].getComponents()[j].componentType) > 0){
                        //Check for each script
                        /** TEMPLATE:
                        if((*objects)[i].getComponents()[j].componentType == SCRIPTTYPES::YOURSCRIPT){
                            dynamic_cast<Collection<YourScript>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        **/
                        //End of custom scripts
                        if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::ANIMATION){
                            dynamic_cast<Collection<AnimationComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::CAMERA){
                            dynamic_cast<Collection<CameraComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::DIMENSION){
                            dynamic_cast<Collection<DimensionComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::GRAPHIC){
                            dynamic_cast<Collection<GraphicComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::INPUT){
                            dynamic_cast<Collection<InputComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::NETWORK){
                            dynamic_cast<Collection<NetworkComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::AUDIO){
                            dynamic_cast<Collection<AudioComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::HITBOX){
                            dynamic_cast<Collection<HitboxComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::ANIMATOR){
                            dynamic_cast<Collection<AnimatorComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::TIMER){
                            dynamic_cast<Collection<TimerComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::PROPERTIES){
                            dynamic_cast<Collection<PropertiesComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                        else if((*objects)[i].getComponents()[j].componentType == COMPONENTTYPES::PHYSICS){
                            dynamic_cast<Collection<PhysicsComponent>*>((*components)[(*objects)[i].getComponents()[j].componentType])->deleteByPos((*objects)[i].getComponents()[j].componentPosition);
                        }
                    }
                    //(*objects)[i].getComponents().erase((*objects)[i].getComponents().begin() + j);
                }
                availableObjects->push_back(i);
                return 1;
            }
        }
        return 0;
    }

    GameObject *ScriptsInterface::getObject(std::string name){
        for(unsigned i=0; i<objects->size(); i++){
            if((*objects)[i].getName() == name && !(*objects)[i].getDeleted()){
                return &(*objects)[i];
            }
        }
        return nullptr;
    }

    GameObject *ScriptsInterface::getObject(unsigned id){
        for(unsigned i=0; i<objects->size(); i++){
            if((*objects)[i].getId() == id && !(*objects)[i].getDeleted()){
                return &(*objects)[i];
            }
        }
        return nullptr;
    }

    GameObject *ScriptsInterface::getObjectByPos(unsigned pos){
        if(pos > 0 && pos < objects->size() && !(*objects)[pos].getDeleted()){
            return &(*objects)[pos];
        }
        return nullptr;
    }

    template <class T>
    unsigned ScriptsInterface::insertComponent(std::string oName, T component, std::string name){
        if(objects != nullptr && components != nullptr){
            for(unsigned i=0; i<objects->size(); i++){
                if((*objects)[i].getName() == oName && !(*objects)[i].getDeleted() && !(*objects)[i].hasComponent(component.getType(), name)){
                    if(components->count(component.getType()) == 0){
                        (*components)[component.getType()] = new Collection<T>(component.getType());
                    }
                    Collection<T> *col = dynamic_cast<Collection<T>*>((*components)[component.getType()]);
                    unsigned result = col->addItem(component, (*objects)[i].getId(), name);
                    if(result > 0){
                        unsigned compPos = result - 1;
                        col->getCollection()[compPos].setOwnerId((*objects)[i].getId());
                        col->getCollection()[compPos].setOwnerPos(i);
                        (*objects)[i].getComponents().emplace_back(col->getCollection()[compPos].getType(), col->getCollection()[compPos].getName(), compPos);
                        col = nullptr;
                        return 1;
                    }
                    col = nullptr;
                    return 0;
                }
            }
        }
        return 0;
    }

    template <class T>
    unsigned ScriptsInterface::insertComponent(unsigned oId, T component, std::string name){
        if(objects != nullptr && components != nullptr){
            for(unsigned i=0; i<objects->size(); i++){
                if((*objects)[i].getId() == oId && !(*objects)[i].getDeleted() && !(*objects)[i].hasComponent(component.getType(), name)){
                    if(components->count(component.getType()) == 0){
                        (*components)[component.getType()] = new Collection<T>(component.getType());
                    }
                    Collection<T> *col = dynamic_cast<Collection<T>*>((*components)[component.getType()]);
                    unsigned result = col->addItem(component, oId, name);
                    if(result > 0){
                        unsigned compPos = result - 1;
                        col->getCollection()[compPos].setOwnerId(oId);
                        col->getCollection()[compPos].setOwnerPos(i);
                        (*objects)[i].getComponents().emplace_back(col->getCollection()[compPos].getType(), col->getCollection()[compPos].getName(), compPos);
                        col = nullptr;
                        return 1;
                    }
                    col = nullptr;
                    return 0;
                }
            }
        }
        return 0;
    }

    template <class T>
    T *ScriptsInterface::getComponent(GameObject *obj, int type, std::string name){
        if(components != nullptr && components->count(type) > 0 && !obj->getDeleted()){
            if(!obj->getDeleted() && obj->hasComponent(type, name)){
                T *aux = &dynamic_cast<Collection<T>*>((*components)[type])->getCollection()[obj->getComponentPosition(type, name)];
                if(!aux->getDeleted()){
                    return aux;
                }
            }
        }
        return nullptr;
    }

    template <class T>
    unsigned ScriptsInterface::deleteComponent(GameObject *obj, int type, std::string name){
        if(components != nullptr){
            if(!obj->getDeleted() && components->count(type) > 0){
                unsigned result = dynamic_cast<Collection<T>*>((*components)[type])->deleteItem(obj->getId(), name);
                if(result){
                    for(unsigned i=0; i<obj->getComponents().size(); i++){
                        if(obj->getComponents()[i].componentType == type && obj->getComponents()[i].componentName == name){
                            obj->getComponents().erase(obj->getComponents().begin() + i);
                        }
                    }
                }
            }
        }
        else{
            return 0;
        }
        return 0;
    }

    void ScriptsInterface::deleteWindow(std::string name){
        postedM->push_back(new Message(MESSAGETYPES::DELETEWINDOW, name));
    }

    void ScriptsInterface::createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h){
        postedM->push_back(new CreateWindowMessage(title, name, icon, x, y, w, h));
    }

    void ScriptsInterface::goToState(std::string stateFile){
        postedM->push_back(new Message(MESSAGETYPES::GOTOSTATE, stateFile));
        postedM->push_back(new Message(MESSAGETYPES::DELETEEVERYTHING));
    }

    void ScriptsInterface::quit(){
        postedM->push_back(new Message(MESSAGETYPES::QUIT));
    }

    void ScriptsInterface::fullScreen(std::string window){
        postedM->push_back(new Message(MESSAGETYPES::FULLSCREEN, window));
    }

    void ScriptsInterface::createSaveState(){

    }

    void ScriptsInterface::loadSaveState(){

    }

    void ScriptsInterface::setMaxFPS(int mFPS){
        postedM->push_back(new Message(MESSAGETYPES::SETMAXFPS, Utils::intToString(mFPS)));
    }

    void ScriptsInterface::setFrameCapFlag(bool frameCap){
        postedM->push_back(new Message(MESSAGETYPES::SETFRAMECAPFLAG, Utils::intToString(frameCap)));
    }

    void ScriptsInterface::createScript(std::unordered_map<int, BaseCollection*> *col, GameObject *own, unsigned ownPos, bool isEn, std::string n, std::string ar){
        /** TEMPLATE:
            if(n == "YourScriptName"){
                if(col->count(SCRIPTTYPES::YOURSCRIPT) == 0){
                    (*col)[SCRIPTTYPES::YOURSCRIPT] = new Collection<YourScript>(SCRIPTTYPES::YOURSCRIPT);
                }
                Collection<YourScript> *coli = dynamic_cast<Collection<YourScript>*>((*col)[SCRIPTTYPES::YOURSCRIPT]);
                if(coli != nullptr){
                    unsigned result = coli->addItem(YourScript(), own->getId(), n);
                    if(result){
                        std::string compN = coli->getCollection()[result-1].getName();
                        own->getComponents().emplace_back(SCRIPTTYPES::YOURSCRIPT, compN, result-1);
                        coli->getCollection()[result-1].setEnabled(isEn);
                        coli->getCollection()[result-1].setOwnerId(own->getId());
                        coli->getCollection()[result-1].setOwnerPos(ownPos);
                    }
                }
                coli = nullptr;
            }**/
    }

    void ScriptsInterface::update(double dt){
        if(components != nullptr){
            /** TEMPLATE:
            if((*components).count(SCRIPTTYPES::YOURSCRIPT) > 0){
                Collection<YourScript> *col = dynamic_cast<Collection<YourScript>*>((*components)[SCRIPTTYPES::YOURSCRIPT]);
                if(col != nullptr){
                    for(unsigned j=0; j<col->getCollection().size(); j++){
                        col->getCollection()[j].execute((*objects)[col->getCollection()[j].getOwnerPos()], dt);
                    }
                }
                col = nullptr;
            }**/
        }
    }

    void ScriptsInterface::handleMessage(Message* mes){
        if(mes->getType() == MESSAGETYPES::CREATESCRIPT){
            CreateScriptMessage *scrmes = dynamic_cast<CreateScriptMessage*>(mes);
            if(scrmes != nullptr){
                createScript(scrmes->getCollection(), scrmes->getOwner(), scrmes->getOwnerPos(), scrmes->getIsScriptEnabled(), scrmes->getName(), scrmes->getArgs());
            }
            scrmes = nullptr;
        }
        else if(mes->getType() == MESSAGETYPES::OBJECTLIST){
            ObjectListMessage* rmes = dynamic_cast<ObjectListMessage*>(mes);
            if(rmes != nullptr){
                objects = rmes->getObjectList();
            }
            rmes = nullptr;
        }
        else if(mes->getType() == MESSAGETYPES::COMPONENTLIST){
            ComponentListMessage* rmes = dynamic_cast<ComponentListMessage*>(mes);
            if(rmes != nullptr){
                components = rmes->getComponentList();
            }
            rmes = nullptr;
        }
        else if(mes->getType() == MESSAGETYPES::AVAILABLEOBJECTS){
            AvailableObjectsMessage* rmes = dynamic_cast<AvailableObjectsMessage*>(mes);
            if(rmes != nullptr){
                availableObjects = rmes->getAvailabilityList();
            }
            rmes = nullptr;
        }
        else if(mes->getType() == MESSAGETYPES::DELETEEVERYTHING){
            destroySystem();
        }
    }
}
