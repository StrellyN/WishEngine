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

#include "TimerComponent.hpp"

namespace WishEngine{
    TimerComponent::TimerComponent(double counterTime, double maxT, bool countingDown, bool pause){
        setType(COMPONENTTYPES::TIMER);
        counter = counterTime;
        maxTime = maxT;
        countDown = countingDown;
        paused = pause;
    }

    TimerComponent::~TimerComponent(){

    }

    double TimerComponent::getCounter(){
        return counter;
    }

    double TimerComponent::getMaxTime(){
        return maxTime;
    }

    bool TimerComponent::getCountDown(){
        return countDown;
    }

    bool TimerComponent::getPaused(){
        return paused;
    }

    void TimerComponent::setCounter(double c){
        counter = c;
        if(counter>maxTime){
            counter = maxTime;
        }
        if(counter<0){
            counter = 0;
        }
    }

    void TimerComponent::setMaxTime(double mT){
        maxTime = mT;
    }

    void TimerComponent::setCountDown(bool cD){
        countDown = cD;
    }

    void TimerComponent::setPaused(bool p){
        paused = p;
    }

    void TimerComponent::reset(){
        if(countDown){
            counter = maxTime;
        }
        else{
            counter = 0;
        }
    }
}
