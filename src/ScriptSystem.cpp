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
    ScriptSystem::ScriptSystem(){
        setSystemType(S_TYPES::SCRIPTS);
    }

    ScriptSystem::~ScriptSystem(){
        destroySystem();
    }

    /**
        Method that updates the logic.
        In this case, for every object and camera that has at least one script component,
        it calls the execute function of the scripts to update them.
    **/
    void ScriptSystem::update(double dt){
        std::vector<GameObject> &obj = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<obj.size(); i++){
            if(obj[i].getEnabled() && obj[i].hasComponent(C_TYPES::SCRIPT)){
                std::vector<Component*> comps = obj[i].getComponents();
                for(unsigned j=0; j<comps.size(); j++){
                    if(comps[j] != nullptr && comps[j]->getType() == C_TYPES::SCRIPT && comps[j]->getEnabled()){
                        ScriptComponent* scr = dynamic_cast<ScriptComponent*>(comps[j]);
                        if(scr != nullptr)
                            scr->execute(obj[i], dt);
                    }
                }
            }
        }

        std::vector<Camera> &cameras = ObjectFactory::getObjectFactory()->getCameras();
        for(unsigned i=0; i<cameras.size(); i++){
            if(cameras[i].getEnabled() && cameras[i].hasComponent(C_TYPES::SCRIPT)){
                std::vector<Component*> comps = cameras[i].getComponents();
                for(unsigned j=0; j<comps.size(); j++){
                    if(comps[j] != nullptr && comps[j]->getType() == C_TYPES::SCRIPT && comps[j]->getEnabled()){
                        ScriptComponent* scr = dynamic_cast<ScriptComponent*>(comps[j]);
                        if(scr != nullptr)
                            scr->execute(cameras[i], dt);
                    }
                }
            }
        }
    }

    /**
        Method to handle received messages.
    **/
    void ScriptSystem::handleMessage(Message* msg){

    }
}
