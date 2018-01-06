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
    AnimatorComponent::AnimatorComponent(){
        setType(C_TYPES::ANIMATOR);
    }

    AnimatorComponent::~AnimatorComponent(){
        states.clear();
    }

    AnimationState* AnimatorComponent::getAnimationState(std::string name){
        if(states.count(name) != 0){
            return &states[name];
        }
        return nullptr;
    }

    void AnimatorComponent::addAnimationState(std::string name, AnimationState state){ //check name
        std::string originalName = name;
        int i=1;
        while(states.count(name) != 0 || name == ""){
            if(name == ""){
                name = "AnimationState";
                originalName = name;
                continue;
            }
            name = originalName;
            name += Utils::intToString(i);
            i++;
        }
        states[name] = state;
    }

    void AnimatorComponent::deleteAnimationState(std::string name){
        if(states.count(name) != 0){
            states.erase(name);
        }
    }

    std::string &AnimatorComponent::getCurrentState(){
        return currentState;
    }

    void AnimatorComponent::setCurrentState(std::string state){
        currentState = state;
    }

    std::map<std::string, AnimationState>& AnimatorComponent::getStates(){
        return states;
    }
}
