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

#include "HitboxComponent.hpp"

namespace WishEngine{
    HitboxComponent::HitboxComponent(double offX, double offY, double w, double h, bool solid, bool checkCollisions){
        setType(COMPONENTTYPES::HITBOX);
        offset = DirectionVector(offX, offY);
        W = w;
        H = h;
        L = 0;
        checkForCollisions = checkCollisions;
        isSolid = solid;
    }

    HitboxComponent::HitboxComponent(double offX, double offY, double offZ, double w, double h, double l, bool solid, bool checkCollisions){
        setType(COMPONENTTYPES::HITBOX);
        offset = DirectionVector(offX, offY, offZ);
        W = w;
        H = h;
        L = l;
        checkForCollisions = checkCollisions;
        isSolid = solid;
    }

    HitboxComponent::~HitboxComponent(){
    }

    void HitboxComponent::setOffset(double offX, double offY, double offZ = 0){
        offset = DirectionVector(offX, offY, offZ);
    }

    double HitboxComponent::getOffsetX(){
        return offset.getX();
    }

    double HitboxComponent::getOffsetY(){
        return offset.getY();
    }

    double HitboxComponent::getOffsetZ(){
        return offset.getZ();
    }

    void HitboxComponent::setW(double w){
        W = w;
    }

    void HitboxComponent::setH(double h){
        H = h;
    }

    void HitboxComponent::setL(double l){
        L = l;
    }

    double HitboxComponent::getW(){
        return W;
    }

    double HitboxComponent::getH(){
        return H;
    }

    double HitboxComponent::getL(){
        return L;
    }

    bool HitboxComponent::getIsColliding(){
        return isColliding;
    }

    void HitboxComponent::setIsColliding(bool isColl){
        isColliding = isColl;
    }

    bool HitboxComponent::getCheckForCollisions(){
        return checkForCollisions;
    }

    void HitboxComponent::setCheckForCollisions(bool check){
        checkForCollisions = check;
    }

    bool HitboxComponent::getIsSolid(){
        return isSolid;
    }

    void HitboxComponent::setIsSolid(bool solid){
        isSolid = solid;
    }

    std::vector<CollisionStruct>& HitboxComponent::getCollisionList(){
        return collisionList;
    }

    void HitboxComponent::addCollisionElement(unsigned ind, std::string n){
        CollisionStruct newElement(ind, n);
        collisionList.push_back(newElement);
    }
}
