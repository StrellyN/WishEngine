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
    DimentionComponent::DimentionComponent(double x, double y, int w, int h){
        setType(C_TYPES::DIMENTION);
        setX(x);
        setY(y);
        setZ(0);
        pX = x;
        pY = y;
        pZ = 0;
        setW(w);
        setH(h);
        setL(0);
        setScale(1);
    }

    DimentionComponent::DimentionComponent(double x, double y, double z, int w, int h, int l){
        setX(x);
        setY(y);
        setZ(z);
        pX = x;
        pY = y;
        pZ = z;
        setW(w);
        setH(h);
        setL(l);
        setScale(1);
    }

    DimentionComponent::~DimentionComponent(){
    }

    void DimentionComponent::setX(double x){
        pX = rect.getX();
        rect.setX(x);
    }

    void DimentionComponent::setY(double y){
        pY = rect.getY();
        rect.setY(y);
    }

    void DimentionComponent::setZ(double z){
        pZ = rect.getZ();
        rect.setZ(z);
    }

    double DimentionComponent::getX(){
        return rect.getX();;
    }

    double DimentionComponent::getY(){
        return rect.getY();
    }

    double DimentionComponent::getZ(){
        return rect.getZ();
    }

    double DimentionComponent::getpX(){
        return pX;
    }

    double DimentionComponent::getpY(){
        return pY;
    }

    double DimentionComponent::getpZ(){
        return pZ;
    }

    void DimentionComponent::setW(int w){
        rect.setW(w);
    }

    void DimentionComponent::setH(int h){
        rect.setH(h);
    }

    void DimentionComponent::setL(int l){
        rect.setL(l);
    }

    int DimentionComponent::getW(){
        return rect.getW();
    }

    int DimentionComponent::getH(){
        return rect.getH();
    }

    int DimentionComponent::getL(){
        return rect.getL();
    }

    double DimentionComponent::getScale(){
        return scale;
    }

    void DimentionComponent::setScale(double s){
        scale = s;
    }
}
