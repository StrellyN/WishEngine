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
    TimerSystem::TimerSystem(){
        setSystemType(S_TYPES::TIMER);
    }

    TimerSystem::~TimerSystem(){
        destroySystem();
    }

    /**
        In this method the system updates each timer component subtracting or adding to the time counter
        depending if it's a count down or not.
    **/
    void TimerSystem::update(double dt){
        std::vector<GameObject> &obj = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<obj.size(); i++){
            if(obj[i].getEnabled() && obj[i].hasComponent(C_TYPES::TIMERC)){
                TimerComponent *timer = dynamic_cast<TimerComponent*>(obj[i].getComponent(C_TYPES::TIMERC));
                if(timer != nullptr && timer->getEnabled() && !timer->getPaused()){
                    if(timer->getCountDown()){
                        timer->setCounter(timer->getCounter() - (1*dt));
                    }
                    else{
                        timer->setCounter(timer->getCounter() + (1*dt));
                    }
                }
            }
        }
        std::vector<Camera> &cam = ObjectFactory::getObjectFactory()->getCameras();
        for(unsigned i=0; i<cam.size(); i++){
            if(cam[i].getEnabled() && cam[i].hasComponent(C_TYPES::TIMERC)){
                TimerComponent *timer = dynamic_cast<TimerComponent*>(cam[i].getComponent(C_TYPES::TIMERC));
                if(timer != nullptr && timer->getEnabled() && !timer->getPaused()){
                    if(timer->getCountDown()){
                        timer->setCounter(timer->getCounter() - (1*dt));
                    }
                    else{
                        timer->setCounter(timer->getCounter() + (1*dt));
                    }
                }
            }
        }
    }

    void TimerSystem::handleMessage(Message *msg){

    }
}
