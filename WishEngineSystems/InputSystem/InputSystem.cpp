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

#include "InputSystem.hpp"

namespace WishEngine{
    InputSystem::InputSystem(){
        setSystemType("INPUT");
    }

    InputSystem::~InputSystem(){
        destroySystem();
        components = nullptr;
    }

    /**
        Method that updates the logic.
        Just sets every Input Components events stack to the one in the system, a.k.a. Passes the events.
    **/
    void InputSystem::update(double dt){

    }

    /**
        Method that handles the input that receives from the framework.
    **/
    void InputSystem::handleInput(std::vector<Event> *inputList){
        std::vector<InputComponent> *inputs = nullptr;
        if(components != nullptr && components->find(COMPONENTTYPES::INPUT) != components->end()){
            inputs = &dynamic_cast<Collection<InputComponent>*>(components->at(COMPONENTTYPES::INPUT))->getCollection();
        }
        if(inputs != nullptr){
            for(unsigned i=0; i<inputs->size(); i++){
                if(!(*inputs)[i].getDeleted()){
                    (*inputs)[i].getInputs() = *inputList;
                }
            }
        }
        inputs = nullptr;

        for(unsigned i=0; i<inputList->size(); i++){
            if((*inputList)[i].getType() == EVENTTYPES::EQUIT){ //Checks if it quit, and if it is sends the quit message
                postMessage(new Message(MESSAGETYPES::QUIT));
                break;
            }
        }
    }

    /**
        Method that handles received messages.
    **/
    void InputSystem::handleMessage(Message* mes){
        if(mes->getType() == MESSAGETYPES::INPUTLIST){
            InputListMessage *aux = dynamic_cast<InputListMessage*>(mes);
            if(aux != nullptr){
                handleInput(aux->getInputList());
            }
            aux = nullptr;
        }
        else if(mes->getType() == MESSAGETYPES::COMPONENTLIST){
            ComponentListMessage *aux = dynamic_cast<ComponentListMessage*>(mes);
            if(aux != nullptr){
                components = aux->getComponentList();
            }
            aux = nullptr;
        }
        else if(mes->getType() == MESSAGETYPES::DELETEEVERYTHING){
            destroySystem();
        }
    }
}
