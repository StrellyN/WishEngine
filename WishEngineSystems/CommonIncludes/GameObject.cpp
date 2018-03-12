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

#include "GameObject.hpp"

namespace WishEngine{
    GameObject::GameObject(std::string n, bool isEnabled){
        setEnabled(isEnabled);
        setName(n);
    }

    GameObject::~GameObject(){

    }

    unsigned GameObject::getId(){
        return id;
    }

    void GameObject::setId(unsigned i){
        id = i;
    }

    bool GameObject::hasComponent(int type, std::string name){
        if(!getDeleted()){
            for(unsigned i=0; i<components.size(); i++){
                if(components[i].componentType == type){
                    if(name == "" || components[i].componentName == name){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    unsigned GameObject::getComponentPosition(int type, std::string name){
        if(!getDeleted()){
            for(unsigned i=0; i<components.size(); i++){
                if(components[i].componentType == type){
                    if(name == "" || components[i].componentName == name){
                        return components[i].componentPosition;
                    }
                }
            }
        }
        return -1;
    }

    void GameObject::setComponentPosition(unsigned newPos, int type, std::string name){
        for(unsigned i=0; i<components.size(); i++){
            if(components[i].componentType == type){
                if(name == "" || components[i].componentName == name){
                    components[i].componentPosition = newPos;
                }
            }
        }
    }

    std::vector<ComponentInformation> &GameObject::getComponents(){
        return components;
    }
}
