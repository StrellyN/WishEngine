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

#include "CollisionSystem.hpp"

namespace WishEngine{
    CollisionSystem::CollisionSystem(){
        setSystemType("COLLISION");
    }

    CollisionSystem::~CollisionSystem(){
        destroySystem();
        objects = nullptr;
        components = nullptr;
    }

    /**
        Method to check if there's a collision happening between two objects.
        This basically checks if the objects are inside each other, the moment it
        detects they can't be inside each other it returns false.
    **/
    bool CollisionSystem::checkCollision(DimensionComponent *dim1, DimensionComponent *dim2, HitboxComponent *hit1, HitboxComponent *hit2){
        if((dim1 != nullptr && dim1->getEnabled()) && (dim2 != nullptr && dim2->getEnabled())){
            if((hit1 != nullptr && hit2 != nullptr) && (hit1->getEnabled() && hit2->getEnabled())){ //Sets everything needed to do the check
                double left1 = dim1->getX() + hit1->getOffsetX(), right1 = dim1->getX() + hit1->getOffsetX() + (hit1->getW()*dim1->getScale());
                double top1 = dim1->getY() + hit1->getOffsetY(), bottom1 = dim1->getY() + hit1->getOffsetY() + (hit1->getH()*dim1->getScale());
                double left2 = dim2->getX() + hit2->getOffsetX(), right2 = dim2->getX() + hit2->getOffsetX() + (hit2->getW()*dim2->getScale());
                double top2 = dim2->getY() + hit2->getOffsetY(), bottom2 = dim2->getY() + hit2->getOffsetY() + (hit2->getH()*dim2->getScale());

                if( bottom1 < top2 ){ //Checks
                    return false;
                }
                if( top1 > bottom2 ){
                    return false;
                }
                if( right1 < left2 ){
                    return false;
                }
                if( left1 > right2 ){
                    return false;
                }
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }

    /**
        Method that updates the logic.
        In this case it checks every object to see if it has the physics component and a hitbox, and if it has
        it starts checking with all the other objects to see if it has to do collision calculations, forces, or whatver.

        This is slow as fuck, find a way to fix. (Starts really slowing down at ~80 objects.
        Already tested with rendering only and ~3200 objects are still manageable, but collision checking is shit.
        It also takes a lot of time to load the level (since you are creating tons of objects), look into that.
        Maybe differentiate between objects and tiles? fml

        08/01/2018
        To fix the clipping problem, for each collision check, reset the objects x, y, px and py, and save the new positions
        to a vector and after all the checks, stick with the value thats further away from the original X or Y.
        The new positions, include the sliding, so if you can slide with a collision, the new position will be the original
        X or Y.

        09/01/2018
        Clipping has been solved but there's still a problem with getting stuck, I still haven't figured out why it happens tho.
        IT HAS BEEN FIXED :D
    **/
    void CollisionSystem::update(double dt){
        if(components != nullptr){
            std::vector<HitboxComponent> *hitboxes = nullptr;
            if(components->find("HITBOX") != components->end()){
                hitboxes = &dynamic_cast<Collection<HitboxComponent>*>(components->at("HITBOX"))->getCollection();
            }
            std::vector<DimensionComponent> *dimensions = nullptr;
            if(components->find("DIMENSION") != components->end()){
                dimensions = &dynamic_cast<Collection<DimensionComponent>*>(components->at("DIMENSION"))->getCollection();
            }

            if(hitboxes != nullptr && dimensions != nullptr){
                DimensionComponent *dim1 = nullptr, *dim2 = nullptr;
                HitboxComponent *hitI = nullptr, *hitJ = nullptr;
                for(unsigned i=0; i<hitboxes->size(); i++){
                    unsigned hitObjPos = (*hitboxes)[i].getOwnerPos();
                    if((*objects)[hitObjPos].getEnabled() && !(*objects)[hitObjPos].getDeleted() && (*objects)[hitObjPos].hasComponent("DIMENSION")){ //If they have a dimention of course
                        dim1 = &(*dimensions)[(*objects)[hitObjPos].getComponentPosition("DIMENSION")];
                        hitI = &(*hitboxes)[i];
                        if(hitI->getEnabled() && !hitI->getDeleted() && dim1->getEnabled() && !dim1->getDeleted()){ //If the Object has everything needed to
                            hitI->getCollisionList().clear();
                            if(hitI->getCheckForCollisions()){
                                double auxX = dim1->getX(); //Gets some things needed in case of having to set the object back
                                double auxY = dim1->getY(); //and checking for sliding
                                double auxpX = dim1->getpX();
                                double auxpY = dim1->getpY();
                                std::vector<double> newXs;
                                std::vector<double> newYs;
                                for(unsigned j=0; j<hitboxes->size(); j++){
                                    if(i != j){ //If they aren't the same Object and the second object is enabled
                                        unsigned hit2ObjPos = (*hitboxes)[j].getOwnerPos();
                                        dim2 = &(*dimensions)[(*objects)[hit2ObjPos].getComponentPosition("DIMENSION")]; //seems like this is the reason for the slow
                                        hitJ = &(*hitboxes)[j];
                                        if((*objects)[hit2ObjPos].getEnabled() && !(*objects)[hit2ObjPos].getDeleted() && dim2 != nullptr && dim2->getEnabled() && !dim2->getDeleted()){ //If j has a dimention of course
                                            if(hitJ != nullptr && hitJ->getEnabled() && !hitJ->getDeleted()){ //And j has a hitbox
                                                if(checkCollision(dim1, dim2, hitI, hitJ)){
                                                    //Check solid collisions
                                                    if(hitI->getIsSolid() && hitJ->getIsSolid()){
                                                         //If a collision happens
                                                        dim1->setX(dim1->getpX()); //Sets the object to its previous position
                                                        dim1->setY(dim1->getpY());
                                                        double sumX = (auxX - dim1->getX())/10; //Sets the intervals for checking the collision
                                                        double sumY = (auxY - dim1->getY())/10;
                                                        while(!checkCollision(dim1, dim2, hitI, hitJ)){ //And while a collision hasn't happened
                                                            dim1->setX(dim1->getX()+sumX); //Sets the object to the next interval
                                                            dim1->setY(dim1->getY()+sumY);
                                                        }
                                                        dim1->setX(dim1->getpX()); //When the collision has happened, send the object back
                                                        dim1->setY(dim1->getpY());
                                                        double safeX = dim1->getX();
                                                        double safeY = dim1->getY();
                                                        //And try to advance in both directions for sliding
                                                        //Checks to advance in the direction paralel
                                                        //to the wall, aka sliding.
                                                        dim1->setX(auxX);
                                                        dim1->setY(auxpY);
                                                        if(checkCollision(dim1, dim2, hitI, hitJ)){ //If it can't slide in the x direction
                                                            dim1->setX(safeX); //Set the object back to the safe position
                                                            dim1->setX(safeX); //And lock it there for animation.
                                                        }
                                                        dim1->setY(dim1->getpY());
                                                        dim1->setY(auxY);
                                                        dim1->setX(auxpX); //This is why the objects were getting stuck, because the X
                                                                           //wasn't being reset to the original position and Y couldn't slide.
                                                        if(checkCollision(dim1, dim2, hitI, hitJ)){ //The same for the y direction
                                                            dim1->setY(safeY);
                                                            dim1->setY(safeY);
                                                        }
                                                        dim1->setX(dim1->getpX());
                                                        newXs.push_back(dim1->getX());
                                                        newYs.push_back(dim1->getY());
                                                        //Add j object to i's collision component queue and viceversa
                                                        hitI->addCollisionElement(j, (*objects)[hit2ObjPos].getName());
                                                        hitJ->addCollisionElement(i, (*objects)[hitObjPos].getName());
                                                    }
                                                    else{ //Normal collisions (add to the collision queue)
                                                        //Add j object to i's collision component queue
                                                        //Take collision component and add the stuff hell ye
                                                        hitI->addCollisionElement(j, (*objects)[hit2ObjPos].getName());
                                                    }
                                                }
                                                //Resetting the original positions for the next collision check
                                                dim1->setX(auxpX);
                                                dim1->setX(auxX);
                                                dim1->setY(auxpY);
                                                dim1->setY(auxY);
                                            }
                                        }
                                        hitJ = nullptr;
                                        dim2 = nullptr;
                                    }
                                }
                                double newX = auxX, newY = auxY; //This checks which new position is the shortest one to select the
                                for(unsigned s=0; s<newXs.size(); s++){ //final position for the object.
                                    if(auxX - auxpX > 0){
                                        if(auxX - newXs[s] > auxX - newX){
                                            newX = newXs[s];
                                        }
                                    }
                                    else{
                                        if(auxX - newXs[s] < auxX - newX){
                                            newX = newXs[s];
                                        }
                                    }
                                    if(auxY - auxpY > 0){
                                        if(auxY - newYs[s] > auxY - newY){
                                            newY = newYs[s];
                                        }
                                    }
                                    else{
                                        if(auxY - newYs[s] < auxY - newY){
                                            newY = newYs[s];
                                        }
                                    }
                                }
                                dim1->setX(auxpX);
                                dim1->setY(auxpY);
                                dim1->setX(newX);
                                dim1->setY(newY);
                                newXs.clear();
                                newYs.clear();
                            }
                        }
                        dim1 = nullptr;
                        hitI = nullptr;
                    }
                }
            }
        }
    }

    /**
        Method to handle received messages.
    **/
    void CollisionSystem::handleMessage(Message* msg){
        if(msg->getType() == "COMPONENTLIST"){
            ComponentListMessage* rmes = dynamic_cast<ComponentListMessage*>(msg);
            if(rmes != nullptr){
                components = rmes->getComponentList();
            }
            rmes = nullptr;
        }
        else if(msg->getType() == "OBJECTLIST"){
            ObjectListMessage* rmes = dynamic_cast<ObjectListMessage*>(msg);
            if(rmes != nullptr){
                objects = rmes->getObjectList();
            }
            rmes = nullptr;
        }
    }
}
