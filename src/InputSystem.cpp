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
    InputSystem::InputSystem(){
        setSystemType(S_TYPES::INPUT);
    }

    InputSystem::~InputSystem(){
        destroySystem();
        eventList.clear();
    }

    /**
        Method that updates the logic.
        Just sets every Input Components events stack to the one in the system, a.k.a. Passes the events.
    **/
    void InputSystem::update(double dt){
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<objs.size(); i++){
            if(eventList.size() > 0 && objs[i].getEnabled()){
                InputComponent *objInput = dynamic_cast<InputComponent*>(objs[i].getComponent(C_TYPES::INPUTC));
                if(objInput != nullptr && objInput->getEnabled()){
                    objInput->getInputs().clear();
                    objInput->getInputs() = eventList;
                }
                objInput = nullptr;
            }
        }
    }

    /**
        Method that adds an event to the event stack, and deletes
        the bottom one if the size is over the max size.
    **/
    void InputSystem::addEvent(Event &e){
        eventList.push_back(e);
    }

    /**
        Method that returns the last event added. Not that useful.
    **/
    Event& InputSystem::getInput(){
        return eventList[eventList.size()-1];
    }

    /**
        Method that returns the event stack.
    **/
    std::vector<Event>& InputSystem::getEvents(){
        return eventList;
    }

    /**
        Method that handles the input that receives from the framework.
    **/
    void InputSystem::handleInput(){
        eventList.clear();
        std::vector<Event> inputEvents = Framework::getFramework()->getEvents(); //Gets the input
        for(unsigned i=0; i<inputEvents.size(); i++){
            if(inputEvents[i].getType() == E_TYPES::EQUIT){ //Checks if it quit, and if it is sends the quit message
                postMessage(new Message(M_TYPES::QUIT));
            }
            else{ //If its not quit, sends an input message with the info needed and adds the event in the stack
                postMessage(new InputMessage(M_TYPES::EVENTS, inputEvents[i].getType(), inputEvents[i].getValue()));
                if(inputEvents[i].getType() != E_TYPES::ENULL){
                    addEvent(inputEvents[i]);
                }
            }
        }
    }

    /**
        Method that handles received messages.
    **/
    void InputSystem::handleMessage(Message* mes){
        switch(mes->getType()){
            case M_TYPES::HANDLEINPUT:
                handleInput();
                break;
        }
    }
}
