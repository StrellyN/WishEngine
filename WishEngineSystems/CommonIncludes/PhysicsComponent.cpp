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

#include "PhysicsComponent.hpp"

namespace WishEngine{
    PhysicsComponent::PhysicsComponent(){
        setType(COMPONENTTYPES::PHYSICS);
        isPushable = false;
        hasGravity = false;
        emitsForce = false;
        gravityForce = 0;
        gravityXDir = 0;
        gravityYDir = 0;
        emittedForce = 0;
        effectDistance = 0;
        emittedXDir = 0;
        emittedYDir = 0;
    }

    PhysicsComponent::PhysicsComponent(bool pushable, bool gravity, bool emits, double gravForce, double gravX, double gravY, double emittForce, double effDist, double emittX, double emittY){
        setType(COMPONENTTYPES::PHYSICS);
        isPushable = pushable;
        hasGravity = gravity;
        emitsForce = emits;
        gravityForce = gravForce;
        gravityXDir = gravX;
        gravityYDir = gravY;
        emittedForce = emittForce;
        effectDistance = effDist;
        emittedXDir = emittX;
        emittedYDir = emittY;
    }

    PhysicsComponent::~PhysicsComponent(){

    }
}
