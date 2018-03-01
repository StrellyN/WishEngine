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
    GameObject::GameObject(std::string n, bool isEnabled){
        setEnabled(isEnabled);
        setName(n);
        for(unsigned i=0; i<C_TYPES::CTYPESIZE; i++){
            componentPosition[i] = -1;
        }
    }

    GameObject::~GameObject(){
        components.clear();
    }

    /**
        Method for the true owner of the object to destroy its components.
    **/
    void GameObject::destroyComponents(){
        for(unsigned i=0; i<components.size(); i++){
            delete components[i];
            components[i] = nullptr;
        }
        for(unsigned i=0; i<C_TYPES::CTYPESIZE; i++){
            componentPosition[i] = -1;
        }
    }

    /**
        Adds a component and like the addObject and addCamera methods, it checks
        if the name is already being used and changes it if it is.
    **/
    void GameObject::addComponent(Component* comp, std::string n){
        int i=1;
        std::string originalName = n;
        while(n == "" || hasComponent(comp->getType(), n)){
            if(n == ""){
                n = "Component";
                originalName = n;
                continue;
            }
            if(comp->getType() != C_TYPES::AUDIOC && comp->getType() != C_TYPES::SCRIPT){
                break;
            }
            n = originalName;
            n += Utils::intToString(i);
            i++;
        }
        if(!hasComponent(comp->getType(), n)){
            comp->setName(n);
            components.push_back(comp);
            // New Stuff
            if(comp->getType() == C_TYPES::AUDIOC || comp->getType() == C_TYPES::SCRIPT){
                if(componentPosition[comp->getType()] == -1){
                    componentPosition[comp->getType()] = components.size() - 1;
                }
            }
            else{
                componentPosition[comp->getType()] = components.size() - 1;
            }
        }
    }

    /**
        Method that returns a component by its type, and its name if its needed, so in case
        it's a script or an audio component.
        If the component doesn't exist, it returns nullptr.
    **/
    Component* GameObject::getComponent(C_TYPES type, std::string n){
        if(type == C_TYPES::AUDIOC || type == C_TYPES::SCRIPT){
            if(n == ""){
                if(componentPosition[type] != -1){
                    return components[componentPosition[type]];
                }
            }
            else{
                if(componentPosition[type] != -1){
                    for(unsigned i=componentPosition[type]; i<components.size(); i++){
                        if(components[i]->getName() == n){
                            return components[i];
                        }
                    }
                }
            }
        }
        else{
            if(n == ""){
                if(componentPosition[type] != -1){
                    return components[componentPosition[type]];
                }
            }
            else{
                if(componentPosition[type] != -1 && components[componentPosition[type]]->getName() == n){
                    return components[componentPosition[type]];
                }
            }
        }
        return nullptr;
    }

    /**
        Deletes a component by its type and if needed, its name too.
    **/
    void GameObject::deleteComponent(C_TYPES type, std::string n){
        if(type == C_TYPES::AUDIOC || type == C_TYPES::SCRIPT){
            if(n == ""){
                if(componentPosition[type] != -1){
                    delete components[componentPosition[type]];
                    components[componentPosition[type]] = nullptr;
                    components.erase(getComponents().begin() + componentPosition[type]);
                    bool hasAnother = false;
                    for(unsigned i=componentPosition[type]; i<components.size(); i++){
                        if(components[i]->getType() == type){
                            bool hasAnother = true;
                            componentPosition[type] = i;
                        }
                    }
                    if(!hasAnother){
                        componentPosition[type] = -1;
                    }
                }
            }
            else{
                if(componentPosition[type] != -1){
                    for(unsigned i=componentPosition[type]; i<components.size(); i++){
                        if(components[i]->getName() == n){
                            if(i == componentPosition[type]){ //Change the componentPosition[type] value
                                delete components[i];
                                components[i] = nullptr;
                                components.erase(getComponents().begin() + i);
                                bool hasAnother = false;
                                for(unsigned j=componentPosition[type]; j<components.size(); j++){
                                    if(components[j]->getType() == type){
                                        bool hasAnother = true;
                                        componentPosition[type] = i;
                                    }
                                }
                                if(!hasAnother){
                                    componentPosition[type] = -1;
                                }
                            }
                            else{
                                delete components[i];
                                components[i] = nullptr;
                                components.erase(getComponents().begin() + i);
                            }
                        }
                    }
                }
            }
        }
        else{
            if(n == ""){
                if(componentPosition[type] != -1){
                    delete components[componentPosition[type]];
                    components[componentPosition[type]] = nullptr;
                    components.erase(getComponents().begin() + componentPosition[type]);
                    componentPosition[type] = -1;
                }
            }
            else{
                if(componentPosition[type] != -1 && components[componentPosition[type]]->getName() == n){
                    delete components[componentPosition[type]];
                    components[componentPosition[type]] = nullptr;
                    components.erase(getComponents().begin() + componentPosition[type]);
                    componentPosition[type] = -1;
                }
            }
        }
    }

    /**
        Returns if the object has that type of component, and uses the name if needed.
    **/
    bool GameObject::hasComponent(C_TYPES type, std::string n){
        if(type == C_TYPES::AUDIOC || type == C_TYPES::SCRIPT){
            if(n == ""){
                if(componentPosition[type] != -1){
                    return true;
                }
            }
            else{
                if(componentPosition[type] != -1){
                    for(unsigned i=componentPosition[type]; i<components.size(); i++){
                        if(components[i]->getName() == n){
                            return true;
                        }
                    }
                }
            }
        }
        else{
            if(n == ""){
                if(componentPosition[type] != -1){
                    return true;
                }
            }
            else{
                if(componentPosition[type] != -1 && components[componentPosition[type]]->getName() == n){
                    return true;
                }
            }
        }
        return false;
    }

    /**
        Returns the components vector.
    **/
    std::vector<Component*>& GameObject::getComponents(){
        return components;
    }

    /**
        Sets the components destroying the last ones in the process.
    **/
    void GameObject::setComponents(std::vector<Component*>& comp){
        destroyComponents();
        components.clear();
        components = comp;
    }
}
