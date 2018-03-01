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

#include "TimerSystem.hpp"

namespace WishEngine{
    TimerSystem::TimerSystem(){
        setSystemType("TIMER");
    }

    TimerSystem::~TimerSystem(){
        destroySystem();
        components = nullptr;
    }

    /**
        In this method the system updates each timer component subtracting or adding to the time counter
        depending if it's a count down or not.
    **/
    void TimerSystem::update(double dt){
        if(components != nullptr){
            std::vector<TimerComponent> *timers = nullptr;
            if(components->find("TIMER") != components->end()){
                timers = &dynamic_cast<Collection<TimerComponent>*>(components->at("TIMER"))->getCollection();
            }

            if(timers != nullptr){
                for(unsigned i=0; i<timers->size(); i++){
                    if((*timers)[i].getEnabled() && !(*timers)[i].getDeleted()){
                        TimerComponent *timer = &(*timers)[i];
                        if(!timer->getPaused()){
                            if(timer->getCountDown()){
                                timer->setCounter(timer->getCounter() - (1*dt));
                            }
                            else{
                                timer->setCounter(timer->getCounter() + (1*dt));
                            }
                        }
                        timer = nullptr;
                    }
                }
            }
        }
    }

    void TimerSystem::handleMessage(Message *msg){
        if(msg->getType() == "COMPONENTLIST"){
            ComponentListMessage* rmes = dynamic_cast<ComponentListMessage*>(msg);
            if(rmes != nullptr){
                components = rmes->getComponentList();
            }
            rmes = nullptr;
        }
    }
}
