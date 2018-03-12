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

#include "DimensionComponent.hpp"

namespace WishEngine{
    DimensionComponent::DimensionComponent(double x, double y, int w, int h){
        setType(COMPONENTTYPES::DIMENSION);
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

    DimensionComponent::DimensionComponent(double x, double y, double z, int w, int h, int l){
        setType(COMPONENTTYPES::DIMENSION);
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

    DimensionComponent::~DimensionComponent(){
    }

    void DimensionComponent::setX(double x){
        pX = rect.getX();
        rect.setX(x);
    }

    void DimensionComponent::setY(double y){
        pY = rect.getY();
        rect.setY(y);
    }

    void DimensionComponent::setZ(double z){
        pZ = rect.getZ();
        rect.setZ(z);
    }

    double DimensionComponent::getX(){
        return rect.getX();;
    }

    double DimensionComponent::getY(){
        return rect.getY();
    }

    double DimensionComponent::getZ(){
        return rect.getZ();
    }

    double DimensionComponent::getpX(){
        return pX;
    }

    double DimensionComponent::getpY(){
        return pY;
    }

    double DimensionComponent::getpZ(){
        return pZ;
    }

    void DimensionComponent::setW(int w){
        rect.setW(w);
    }

    void DimensionComponent::setH(int h){
        rect.setH(h);
    }

    void DimensionComponent::setL(int l){
        rect.setL(l);
    }

    int DimensionComponent::getW(){
        return rect.getW();
    }

    int DimensionComponent::getH(){
        return rect.getH();
    }

    int DimensionComponent::getL(){
        return rect.getL();
    }

    double DimensionComponent::getScale(){
        return scale;
    }

    void DimensionComponent::setScale(double s){
        scale = s;
    }
}
