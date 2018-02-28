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

#include "CameraComponent.hpp"

namespace WishEngine{
    CameraComponent::CameraComponent(int pri){
        setType("CAMERA");
        priority = pri;
        rect.setX(0);
        rect.setY(0);
        rect.setZ(0);
    }

    CameraComponent::CameraComponent(int pri, double x, double y){
        setType("CAMERA");
        priority = pri;
        rect.setX(x);
        rect.setY(y);
        rect.setZ(0);
    }

    CameraComponent::CameraComponent(int pri, double x, double y, double z){
        setType("CAMERA");
        priority = pri;
        rect.setX(x);
        rect.setY(y);
        rect.setZ(z);
    }

    CameraComponent::~CameraComponent(){

    }

    double CameraComponent::getX(){
        rect.getX();
    }

    double CameraComponent::getY(){
        rect.getY();
    }

    double CameraComponent::getZ(){
        rect.getZ();
    }

    int CameraComponent::getPriority(){
        return priority;
    }

    void CameraComponent::setX(double x){
        rect.setX(x);
    }

    void CameraComponent::setY(double y){
        rect.setY(y);
    }

    void CameraComponent::setZ(double z){
        rect.setZ(z);
    }

    void CameraComponent::setPriority(int pri){
        priority = pri;
    }
}
