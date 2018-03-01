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

#include "PhysicsSystem.hpp"

namespace WishEngine{
    PhysicsSystem::PhysicsSystem(){
        setSystemType("PHYSICS");
    }

    PhysicsSystem::~PhysicsSystem(){
        destroySystem();
        components = nullptr;
        objects = nullptr;
    }

    /**
        Method that updates the logic.
        In this case it checks every object to see if it has the physics component and a hitbox, and if it has
        it starts checking with all the other objects to see if it has to do collision calculations, forces, or whatver.
    **/
    void PhysicsSystem::update(double dt){
        if(components != nullptr && objects != nullptr){
            std::vector<HitboxComponent> *hitboxes = nullptr;
            if(components->find("HITBOX") != components->end()){
                hitboxes = &dynamic_cast<Collection<HitboxComponent>*>(components->at("HITBOX"))->getCollection();
            }
            std::vector<DimensionComponent> *dimensions = nullptr;
            if(components->find("DIMENSION") != components->end()){
                dimensions = &dynamic_cast<Collection<DimensionComponent>*>(components->at("DIMENSION"))->getCollection();
            }
            std::vector<PhysicsComponent> *physics = nullptr;
            if(components->find("PHYSICS") != components->end()){
                physics = &dynamic_cast<Collection<PhysicsComponent>*>(components->at("PHYSICS"))->getCollection();
            }

            if(dimensions != nullptr && physics != nullptr){
                for(unsigned i=0; i<physics->size(); i++){
                    //Check it's own flags applying gravity to its own dimension

                    //For the other physics
                    //Check if they are pushable
                    //If they are, check the distance with the dimensions
                    //If in range, apply the forces.
                }
            }

            hitboxes = nullptr;
            dimensions = nullptr;
            physics = nullptr;
        }
    }

    /**
        Method to handle received messages.
    **/
    void PhysicsSystem::handleMessage(Message* msg){
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
