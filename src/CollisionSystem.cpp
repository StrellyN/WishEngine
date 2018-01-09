#include "HppHeaders.hpp"

namespace WishEngine{
    CollisionSystem::CollisionSystem(){
        setSystemType(S_TYPES::COLLISION);
    }

    CollisionSystem::~CollisionSystem(){
        destroySystem();
    }

    /**
        Method to check if there's a collision happening between two objects.
        This basically checks if the objects are inside each other, the moment it
        detects they can't be inside each other it returns false.
    **/
    bool CollisionSystem::checkCollision(GameObject &obj1, GameObject &obj2){
        DimentionComponent* dim1 = dynamic_cast<DimentionComponent*>(obj1.getComponent(C_TYPES::DIMENTION)); //Gets everything it needs to check
        DimentionComponent* dim2 = dynamic_cast<DimentionComponent*>(obj2.getComponent(C_TYPES::DIMENTION));
        HitboxComponent* hit1 = dynamic_cast<HitboxComponent*>(obj1.getComponent(C_TYPES::HITBOX));
        HitboxComponent* hit2 = dynamic_cast<HitboxComponent*>(obj2.getComponent(C_TYPES::HITBOX)); //And checks if everything is enabled
        if((obj1.getEnabled() && obj2.getEnabled()) && (dim1 != nullptr && dim1->getEnabled()) && (dim2 != nullptr && dim2->getEnabled())){
            if((hit1 != nullptr && hit2 != nullptr) && (hit1->getEnabled() && hit2->getEnabled())){ //Sets everything needed to do the check
                double left1 = dim1->getX() + hit1->getOffsetX(), right1 = dim1->getX() + hit1->getOffsetX() + (hit1->getW()*dim1->getScale());
                double top1 = dim1->getY() + hit1->getOffsetY(), bottom1 = dim1->getY() + hit1->getOffsetY() + (hit1->getH()*dim1->getScale());
                double left2 = dim2->getX() + hit2->getOffsetX(), right2 = dim2->getX() + hit2->getOffsetX() + (hit2->getW()*dim2->getScale());
                double top2 = dim2->getY() + hit2->getOffsetY(), bottom2 = dim2->getY() + hit2->getOffsetY() + (hit2->getH()*dim2->getScale());

                if( bottom1 < top2 ){ //Checks
                    dim1 = dim2 = nullptr; //Sets gathered things to null.
                    hit1 = hit2 = nullptr;
                    return false;
                }
                if( top1 > bottom2 ){
                    dim1 = dim2 = nullptr;
                    hit1 = hit2 = nullptr;
                    return false;
                }
                if( right1 < left2 ){
                    dim1 = dim2 = nullptr;
                    hit1 = hit2 = nullptr;
                    return false;
                }
                if( left1 > right2 ){
                    dim1 = dim2 = nullptr;
                    hit1 = hit2 = nullptr;
                    return false;
                }
                dim1 = dim2 = nullptr;
                hit1 = hit2 = nullptr;
                return true;
            }
            else{
                dim1 = dim2 = nullptr;
                hit1 = hit2 = nullptr;
                return false;
            }
        }
        else{
            dim1 = dim2 = nullptr;
            hit1 = hit2 = nullptr;
            return false;
        }
    }

    /**
        THIS METHOD IS VERY EXPENSIVE AND WILL LOCK YOUR GAME UNTIL IT'S DONE
        DO NOT USE IT EVERYFRAME OR YOUR GAME WON'T RUN PROPERLY
        USE THIS FOR STUFF LIKE SINGLE MOUSE CLICK TELEPORTATION WHERE
        YOU CAN'T GO THROUGH WALLS

        A function that will move an object directly to a new position while checking if a collision
        happens every position interval, if it does happen, it will stop.
        The collision checking will only apply with the game objects you pass,
        this way there's room for optimization in your game if you don't want
        to pass all the game objects everytime you do the check.
    **/
    void CollisionSystem::moveToPosition(GameObject &obj, std::vector<GameObject> &objs, double x, double y, double intervals){
        if(obj.getEnabled()){
            DimentionComponent *dim1 = dynamic_cast<DimentionComponent*>(obj.getComponent(C_TYPES::DIMENTION));
            if(dim1 != nullptr && dim1->getEnabled()){
                double xVec = (x-dim1->getX())/intervals;
                double yVec = (y-dim1->getY())/intervals;
                bool colision = false;
                while(dim1->getX() != x || dim1->getY() != y){
                    dim1->setX(dim1->getX() + xVec);
                    dim1->setY(dim1->getY() + yVec);
                    for(unsigned j=0; j<objs.size(); j++){
                        if((obj.getName() != objs[j].getName()) && objs[j].getEnabled()){ //If they aren't the same Object and the second object is enabled
                            if(objs[j].hasComponent(C_TYPES::DIMENTION) && objs[j].getComponent(C_TYPES::DIMENTION)->getEnabled()){ //If j has a dimention of course
                                if(objs[j].hasComponent(C_TYPES::HITBOX) && objs[j].getComponent(C_TYPES::HITBOX)->getEnabled()){ //And j has a hitbox
                                    if((objs[j].hasComponent(C_TYPES::SOLID) && objs[j].getComponent(C_TYPES::SOLID)->getEnabled()) && //If they are both solid
                                       (obj.hasComponent(C_TYPES::SOLID) && obj.getComponent(C_TYPES::SOLID)->getEnabled())){
                                        if(checkCollision(obj, objs[j])){ //If a collision happens
                                            double auxX = dim1->getX(); //Gets some things needed in case of having to set the object back
                                            double auxY = dim1->getY(); //and checking for sliding
                                            dim1->setX(dim1->getpX()); //Sets the object to its previous position
                                            dim1->setY(dim1->getpY());
                                            double sumX = (auxX - dim1->getX())/10; //Sets the intervals for checking the collision
                                            double sumY = (auxY - dim1->getY())/10;
                                            while(!checkCollision(obj, objs[j])){ //And while a collision hasn't happened
                                                dim1->setX(dim1->getX()+sumX); //Sets the object to the next interval
                                                dim1->setY(dim1->getY()+sumY);
                                            }
                                            dim1->setX(dim1->getpX()); //When the collision has happened, send the object back
                                            dim1->setY(dim1->getpY());
                                            //And try to advance in both directions for sliding
                                            //Checks to advance in the direction paralel
                                            //to the wall, aka sliding.
                                            dim1->setX(auxX);
                                            if(checkCollision(obj, objs[j])){ //If it can't slide in the x direction
                                                dim1->setX(dim1->getpX()); //Set the object back to the safe position
                                                dim1->setX(dim1->getX()); //And lock it there for animation.
                                            }
                                            dim1->setY(auxY);
                                            if(checkCollision(obj, objs[j])){ //The same for the y direction
                                                dim1->setY(dim1->getpY());
                                                dim1->setY(dim1->getY());
                                            }
                                            colision = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(colision){
                        break;
                    }
                }
            }
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
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects();
        DimentionComponent *dim1 = nullptr, *dim2 = nullptr;
        HitboxComponent *hitI = nullptr, *hitJ = nullptr;
        for(unsigned i=0; i<objs.size(); i++){
            if(objs[i].getEnabled() && objs[i].hasComponent(C_TYPES::DIMENTION) && objs[i].getComponent(C_TYPES::DIMENTION)->getEnabled()){ //If they have a dimention of course
                if(objs[i].hasComponent(C_TYPES::HITBOX) && objs[i].getComponent(C_TYPES::HITBOX)->getEnabled()){ //If the Object has everything needed to
                    dim1 = dynamic_cast<DimentionComponent*>(objs[i].getComponent(C_TYPES::DIMENTION));
                    hitI = dynamic_cast<HitboxComponent*>(objs[i].getComponent(C_TYPES::HITBOX));
                    hitI->getCollisionList().clear();
                    if(hitI->getCheckForCollisions()){
                        double auxX = dim1->getX(); //Gets some things needed in case of having to set the object back
                        double auxY = dim1->getY(); //and checking for sliding
                        double auxpX = dim1->getpX();
                        double auxpY = dim1->getpY();
                        std::vector<double> newXs;
                        std::vector<double> newYs;
                        for(unsigned j=0; j<objs.size(); j++){
                            if(i != j && objs[j].getEnabled()){ //If they aren't the same Object and the second object is enabled
                                dim2 = dynamic_cast<DimentionComponent*>(objs[j].getComponent(C_TYPES::DIMENTION)); //seems like this is the reason for the slow
                                hitJ = dynamic_cast<HitboxComponent*>(objs[j].getComponent(C_TYPES::HITBOX));
                                if(dim2 != nullptr && dim2->getEnabled()){ //If j has a dimention of course
                                    if(hitJ != nullptr && hitJ->getEnabled()){ //And j has a hitbox
                                        if(checkCollision(objs[i], objs[j])){
                                            //Check solid collisions
                                            if((objs[j].hasComponent(C_TYPES::SOLID) && objs[j].getComponent(C_TYPES::SOLID)->getEnabled()) &&
                                               (objs[i].hasComponent(C_TYPES::SOLID) && objs[i].getComponent(C_TYPES::SOLID)->getEnabled())){
                                                 //If a collision happens
                                                dim1->setX(dim1->getpX()); //Sets the object to its previous position
                                                dim1->setY(dim1->getpY());
                                                double sumX = (auxX - dim1->getX())/10; //Sets the intervals for checking the collision
                                                double sumY = (auxY - dim1->getY())/10;
                                                while(!checkCollision(objs[i], objs[j])){ //And while a collision hasn't happened
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
                                                if(checkCollision(objs[i], objs[j])){ //If it can't slide in the x direction
                                                    dim1->setX(safeX); //Set the object back to the safe position
                                                    dim1->setX(safeX); //And lock it there for animation.
                                                }
                                                dim1->setY(dim1->getpY());
                                                dim1->setY(auxY);
                                                dim1->setX(auxpX);
                                                if(checkCollision(objs[i], objs[j])){ //The same for the y direction
                                                    dim1->setY(safeY);
                                                    dim1->setY(safeY);
                                                }
                                                dim1->setX(dim1->getpX());
                                                newXs.push_back(dim1->getX());
                                                newYs.push_back(dim1->getY());
                                                //Add j object to i's collision component queue and viceversa
                                                hitI->addCollisionElement(j, objs[j].getName());
                                                hitJ->addCollisionElement(i, objs[i].getName());
                                            }
                                            else{ //Normal collisions (add to the collision queue)
                                                //Add j object to i's collision component queue
                                                //Take collision component and add the stuff hell ye
                                                hitI->addCollisionElement(j, objs[j].getName());
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
                        double newX = auxX, newY = auxY;
                        for(unsigned s=0; s<newXs.size(); s++){
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
                    dim1 = nullptr;
                    hitI = nullptr;
                }
            }
        }
    }

    /**
        Method to handle received messages.
    **/
    void CollisionSystem::handleMessage(Message* msg){

    }
}
