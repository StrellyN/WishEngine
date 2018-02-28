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

#include "ScriptsInterface.hpp"

//Here I include all the scripts.cpps
#include "TitleScript.cpp"

namespace WishEngine{
    std::map<std::string, BaseCollection*> *ScriptsInterface::components = nullptr;

    std::vector<GameObject> *ScriptsInterface::objects = nullptr;

    std::vector<Message*> *ScriptsInterface::postedM = nullptr;

    ScriptsInterface::ScriptsInterface(){
        setSystemType("SCRIPTINTERFACE");
        postedM = &getMessages();
    }

    ScriptsInterface::~ScriptsInterface(){
        components = nullptr;
        objects = nullptr;
        postedM = nullptr;
        destroySystem();
    }

    void ScriptsInterface::insertObject(std::string name, bool isEnabled){
        std::string finalName = name;
        if(finalName == ""){
            finalName = "Object";
        }
        int modifier = 1;
        for(int i=0; i<objects->size(); i++){
            if((*objects)[i].getName() == finalName){
                finalName = name;
                finalName += Utils::intToString(modifier);
                i = -1;
                modifier++;
            }
        }

        objects->emplace_back(finalName, isEnabled);
    }

    unsigned ScriptsInterface::deleteObject(std::string name){
        for(unsigned i=0; i<objects->size(); i++){
            if((*objects)[i].getName() == name && !(*objects)[i].getDeleted()){
                (*objects)[i].setDeleted(true);
                postedM->push_back(new Message("OBJECTDELETED", Utils::intToString(i)));
                return 1;
            }
        }
        return 0;
    }

    unsigned ScriptsInterface::deleteObject(unsigned id){
        for(unsigned i=0; i<objects->size(); i++){
            if((*objects)[i].getId() == id && !(*objects)[i].getDeleted()){
                (*objects)[i].setDeleted(true);
                postedM->push_back(new Message("OBJECTDELETED", Utils::intToString(i)));
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

    template <class T>
    unsigned ScriptsInterface::insertComponent(unsigned oId, T component, std::string name){
        if(objects != nullptr && components != nullptr){
            for(unsigned i=0; i<objects->size(); i++){
                if((*objects)[i].getId() == oId && !(*objects)[i].getDeleted() && !(*objects)[i].hasComponent(component.getType(), name)){
                    if(components->count(component.getType()) == 0){
                        (*components)[component.getType()] = Collection<T>(component.getType());
                    }
                    Collection<T> *col = dynamic_cast<Collection<T>*>((*components)[component.getType()]);
                    unsigned result = col->addItem(component, oId, name);
                    if(result){
                        unsigned compPos = col->getCollection().size() - 1;
                        component.setOwnerId(oId);
                        component.setOwnerPos(i);
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
    T *ScriptsInterface::getComponent(GameObject *obj, std::string type, std::string name){
        if(components != nullptr && components->count(type) > 0 && !obj->getDeleted()){
            if(obj->hasComponent(type, name)){
                T *aux = &dynamic_cast<Collection<T>*>((*components)[type])->getCollection()[obj->getComponentPosition(type, name)];
                if(!aux->getDeleted()){
                    return aux;
                }
            }
        }
        return nullptr;
    }

    template <class T>
    unsigned ScriptsInterface::deleteComponent(GameObject *obj, std::string type, std::string name){
        if(components != nullptr){
            if(!obj->getDeleted() && components->count(type) > 0){
                return dynamic_cast<Collection<T>*>((*components)[type])->deleteItem(obj->getId(), name);
            }
        }
        else{
            return 0;
        }
        return 0;
    }

    void ScriptsInterface::deleteWindow(std::string name){
        postedM->push_back(new Message("DELETEWINDOW", name));
    }

    void ScriptsInterface::createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h){
        postedM->push_back(new CreateWindowMessage(title, name, icon, x, y, w, h));
    }

    void ScriptsInterface::goToState(std::string stateFile){
        postedM->push_back(new Message("GOTOSTATE", stateFile));
    }

    void ScriptsInterface::createSaveState(){

    }

    void ScriptsInterface::loadSaveState(){

    }

    void ScriptsInterface::setMaxFPS(int mFPS){
        postedM->push_back(new Message("SETMAXFPS", Utils::intToString(mFPS)));
    }

    void ScriptsInterface::setFrameCapFlag(bool frameCap){
        postedM->push_back(new Message("SETFRAMECAPFLAG", Utils::intToString(frameCap)));
    }

    void ScriptsInterface::createScript(Collection<BaseCollection*> *col, GameObject *own, unsigned ownPos, bool isEn, std::string n, std::string ar){
        bool exists = false;
        for(unsigned i=0; i<col->getCollection().size(); i++){
            if(col->getCollection()[i]->getType() == n){
                if(n == "TitleScript"){
                    Collection<TitleScript> *coli = dynamic_cast<Collection<TitleScript>*>(col->getCollection()[i]);
                    if(coli != nullptr){
                        unsigned result = coli->addItem(TitleScript(), own->getId());
                        if(result){
                            unsigned compPos = coli->getCollection().size()-1;
                            std::string compN = coli->getCollection()[compPos].getName();
                            own->getComponents().emplace_back(coli->getCollection()[compPos].getType(), compN, compPos);
                            coli->getCollection()[compPos].setEnabled(isEn);
                            coli->getCollection()[compPos].setOwnerId(own->getId());
                            coli->getCollection()[compPos].setOwnerPos(ownPos);
                        }
                    }
                    coli = nullptr;
                }
                exists = true;
                break;
            }
        }
        if(!exists){
            if(n == "TitleScript"){
                col->getCollection().push_back(new Collection<TitleScript>("TitleScript"));
                Collection<TitleScript> *coli = dynamic_cast<Collection<TitleScript>*>(col->getCollection()[col->getCollection().size() - 1]);
                if(coli != nullptr){
                    unsigned result = coli->addItem(TitleScript(), own->getId());
                    if(result){
                        unsigned compPos = coli->getCollection().size()-1;
                        std::string compN = coli->getCollection()[compPos].getName();
                        own->getComponents().emplace_back(coli->getCollection()[compPos].getType(), compN, compPos);
                        coli->getCollection()[compPos].setEnabled(isEn);
                        coli->getCollection()[compPos].setOwnerId(own->getId());
                        coli->getCollection()[compPos].setOwnerPos(ownPos);
                    }
                }
                coli = nullptr;
            }
        }

        /**
            INSTRUCTIONS TO CHECK AND ADD SCRIPTS:
            1) Check the name of the script.
            2) Cycle through all the BaseCollections vector to check the type.
            3) If the type match, dynamic cast it into the type, add the script there and do all the pos, id, etc...
            4) If the type doesn't exist, we'll emplace_back a new Collection of the type and create the script there

            Reference stuff:
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
        **/
    }

    void ScriptsInterface::update(double dt){
        std::vector<BaseCollection*> *scripts = nullptr; //Gets the objects
        if(components != nullptr && components->find("SCRIPT") != components->end()){
            scripts = &dynamic_cast<Collection<BaseCollection*>*>(components->at("SCRIPT"))->getCollection();
        }

        if(scripts != nullptr){
            for(unsigned i=0; i<scripts->size(); i++){
                //if getType() == "..." -> dynamic_cast<...> -> for unsigned j -> execute;
                if((*scripts)[i]->getType() == "TitleScript"){
                    Collection<TitleScript> *col = dynamic_cast<Collection<TitleScript>*>((*scripts)[i]);
                    if(col != nullptr){
                        for(unsigned j=0; j<col->getCollection().size(); j++){
                            col->getCollection()[j].execute((*objects)[col->getCollection()[j].getOwnerId()], dt);
                        }
                    }
                    col = nullptr;
                }
            }
        }

        scripts = nullptr;
    }

    void ScriptsInterface::handleMessage(Message* mes){
        if(mes->getType() == "CREATESCRIPT"){
            CreateScriptMessage *scrmes = dynamic_cast<CreateScriptMessage*>(mes);
            if(scrmes != nullptr){
                createScript(scrmes->getCollection(), scrmes->getOwner(), scrmes->getOwnerPos(), scrmes->getIsScriptEnabled(), scrmes->getName(), scrmes->getArgs());
            }
            scrmes = nullptr;
        }
        else if(mes->getType() == "OBJECTLIST"){
            ObjectListMessage* rmes = dynamic_cast<ObjectListMessage*>(mes);
            if(rmes != nullptr){
                objects = rmes->getObjectList();
            }
            rmes = nullptr;
        }
        else if(mes->getType() == "COMPONENTLIST"){
            ComponentListMessage* rmes = dynamic_cast<ComponentListMessage*>(mes);
            if(rmes != nullptr){
                components = rmes->getComponentList();
            }
            rmes = nullptr;
        }
    }
}
