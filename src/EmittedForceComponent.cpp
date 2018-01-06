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
    EmittedForceComponent::EmittedForceComponent(double x, double y, double f){
        setType(C_TYPES::EMITEDFORCE);
        forceDirection = DirectionVector(x, y);
        force = f;
    }

    EmittedForceComponent::~EmittedForceComponent(){

    }

    void EmittedForceComponent::setX(double x){
        forceDirection.setX(x);
    }

    double EmittedForceComponent::getX(){
        return forceDirection.getX();
    }

    void EmittedForceComponent::setY(double y){
        forceDirection.setY(y);
    }

    double EmittedForceComponent::getY(){
        return forceDirection.getY();
    }

    void EmittedForceComponent::setForce(double f){
        force = f;
    }

    double EmittedForceComponent::getForce(){
        return force;
    }
}
