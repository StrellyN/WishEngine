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
    PhysicsSystem::PhysicsSystem(){
        setSystemType(S_TYPES::PHYSIC);
    }

    PhysicsSystem::~PhysicsSystem(){
        destroySystem();
    }

    /**
        Method that updates the logic.
        In this case it checks every object to see if it has the physics component and a hitbox, and if it has
        it starts checking with all the other objects to see if it has to do collision calculations, forces, or whatver.
    **/
    void PhysicsSystem::update(double dt){
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<objs.size(); i++){
            if(objs[i].getEnabled() && objs[i].hasComponent(C_TYPES::DIMENTION) && objs[i].getComponent(C_TYPES::DIMENTION)->getEnabled()){ //If they have a dimention of course
                if((objs[i].hasComponent(C_TYPES::PHYSICS) && objs[i].getComponent(C_TYPES::PHYSICS)->getEnabled())){
                    //Check gravity
                    VelocityComponent* vel1 = dynamic_cast<VelocityComponent*>(objs[i].getComponent(C_TYPES::VELOCITY));
                    DimentionComponent* dim1 = dynamic_cast<DimentionComponent*>(objs[i].getComponent(C_TYPES::DIMENTION));
                    if(objs[i].hasComponent(C_TYPES::GRAVITY) && objs[i].getComponent(C_TYPES::GRAVITY)->getEnabled()){
                        GravityComponent* gravComp = dynamic_cast<GravityComponent*>(objs[i].getComponent(C_TYPES::GRAVITY));
                        if(vel1 != nullptr && vel1->getEnabled()){
                            vel1->setX(vel1->getX() + (gravComp->getX() * (gravComp->getForce() * dt)));
                            vel1->setY(vel1->getY() + (gravComp->getY() * (gravComp->getForce() * dt)));
                        }
                        gravComp = nullptr;
                    }
                    if(objs[i].hasComponent(C_TYPES::HITBOX) && objs[i].getComponent(C_TYPES::HITBOX)->getEnabled()){ //If the Object has everything needed to
                        for(unsigned j=0; j<objs.size(); j++){
                            if(i != j && objs[j].getEnabled()){ //If they aren't the same Object and the second object is enabled
                                if(objs[j].hasComponent(C_TYPES::DIMENTION) && objs[j].getComponent(C_TYPES::DIMENTION)->getEnabled()){ //If j has a dimention of course
                                    if(objs[j].hasComponent(C_TYPES::HITBOX) && objs[j].getComponent(C_TYPES::HITBOX)->getEnabled()){ //And j has a hitbox
                                        //Check emited forces
                                        if((objs[j].hasComponent(C_TYPES::EMITEDFORCE) && objs[j].getComponent(C_TYPES::EMITEDFORCE)->getEnabled())){ //If j is emiting a force
                                            if(objs[i].hasComponent(C_TYPES::PUSHABLE) && objs[i].getComponent(C_TYPES::PUSHABLE)->getEnabled()){
                                                //Add to the position
                                                EmittedForceComponent* emitComp = dynamic_cast<EmittedForceComponent*>(objs[j].getComponent(C_TYPES::EMITEDFORCE));
                                                if(vel1 != nullptr && vel1->getEnabled()){
                                                    vel1->setX(vel1->getX() + (emitComp->getX() * (emitComp->getForce() * dt)));
                                                    vel1->setY(vel1->getY() + (emitComp->getY() * (emitComp->getForce() * dt)));
                                                }
                                                emitComp = nullptr;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    /**
                        With this code below you can now also apply everything yourself in case you don't want to use the velocity,
                        but if you don't use velocity you'll have to add the gravity and forces in your script.

                        Basically this lets you move the dimension component directly while having solid collisions, as
                        long as you don't have gravity or forces applied to the object through the physics system,
                        or you have the velocity component disabled/deleted.
                    **/
                    if((vel1->getX() != 0 || vel1->getY() != 0) && vel1 != nullptr && vel1->getEnabled()){
                        dim1->setX(dim1->getX() + (vel1->getX() * dt));
                        dim1->setY(dim1->getY() + (vel1->getY() * dt));
                    }
                    dim1 = nullptr;
                    vel1 = nullptr;
                }
            }
        }
    }

    /**
        Method to handle received messages.
    **/
    void PhysicsSystem::handleMessage(Message* msg){

    }
}
