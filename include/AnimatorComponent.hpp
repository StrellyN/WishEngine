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

#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include "Component.hpp"
#include "AnimationState.hpp"

namespace WishEngine{
    class AnimatorComponent : public Component{ //User modifyable State machine that changes the animation Component through the animation System
        private:
            std::map<std::string, AnimationState> states;
            std::string currentState;
        public:
            AnimatorComponent();
            ~AnimatorComponent();
            AnimationState* getAnimationState(std::string name);
            void addAnimationState(std::string name, AnimationState state); //check name
            void deleteAnimationState(std::string name);
            std::string &getCurrentState();
            void setCurrentState(std::string state);
            std::map<std::string, AnimationState>& getStates();
    };
}
#endif // ANIMATORCOMPONENT
