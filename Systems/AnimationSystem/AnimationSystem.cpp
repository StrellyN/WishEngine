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

#include "AnimationSystem.hpp"

namespace WishEngine{
    AnimationSystem::AnimationSystem(){
        setSystemType("ANIMATION"); //Set the system type
    }

    AnimationSystem::~AnimationSystem(){
        destroySystem();
        components = nullptr;
        objects = nullptr;
    }

    /**
        Method to update the logic present in every system.
        In this case it checks for the objects that have the components necesary for animating them and then
        proceeds to process the information and animate the object.
    **/
    void AnimationSystem::update(double dt){
        std::vector<AnimationComponent> *animations = nullptr;
        std::vector<AnimatorComponent> *animators = nullptr;
        if(components != nullptr && components->find("ANIMATION") != components->end()){
            animations = &dynamic_cast<Collection<AnimationComponent>*>(components->at("ANIMATION"))->getCollection();
        }
        if(components != nullptr && components->find("ANIMATOR") != components->end()){
            animators = &dynamic_cast<Collection<AnimatorComponent>*>(components->at("ANIMATOR"))->getCollection();
        }

        if(objects != nullptr && animations != nullptr && animators != nullptr){
            for(unsigned i=0; i<animators->size(); i++){
                unsigned animatorObjPos = (*animators)[i].getOwnerPos();
                if(!(*objects)[animatorObjPos].getDeleted() && (*objects)[animatorObjPos].getEnabled() && (*objects)[animatorObjPos].hasComponent("ANIMATION")){
                    unsigned animationPos = (*objects)[animatorObjPos].getComponentPosition("ANIMATION");
                    if((*animators)[i].getEnabled() && !(*animators)[i].getDeleted() && (*animations)[animationPos].getEnabled() && !(*animations)[animationPos].getDeleted()){
                        AnimationState* animState = (*animators)[i].getAnimationState((*animators)[i].getCurrentState());
                        if(animState != nullptr){
                            (*animations)[animationPos].setW(animState->getFrameW()); //Set the correct size of the frame
                            (*animations)[animationPos].setH(animState->getFrameH());
                            animState->setAnimationCounter(animState->getAnimationCounter() - (1*animState->getAnimationSpeed())); //Add to the animator counter
                            if(animState->getAnimationCounter() <= 0){ //Go to next frame if counter reached goal
                                animState->setAnimationCounter(1-animState->getAnimationSpeed()); //Set the counter back to 0
                                animState->setFrameCounter(animState->getFrameCounter()+1); //Go to next frame
                                if(animState->getFrameCounter() >= animState->getAnimationFrames()){ //If next frame is greater than the length of the anim
                                    animState->setFrameCounter(0); //Go back to the beginning
                                    (*animations)[animationPos].setX(animState->getInitialX());
                                    (*animations)[animationPos].setY(animState->getInitialY());
                                }
                                else{
                                    if(animState->getReverseAnimation()){ //If the animation is reversed
                                        (*animations)[animationPos].setX((*animations)[animationPos].getX() - animState->getFrameW()); //Change the frame position to the next
                                        if((*animations)[animationPos].getX() < 0){ //If the x position is less than 0
                                            (*animations)[animationPos].setX(animState->getTextureW() - animState->getFrameW()); //Reset the x to Max texture pos
                                            (*animations)[animationPos].setY((*animations)[animationPos].getY() - animState->getFrameH()); //add to the y
                                            if((*animations)[animationPos].getY() < 0){ //If the y is less than 0
                                                (*animations)[animationPos].setY(animState->getTextureH() - animState->getFrameH()); //Reset y to Max texture pos;
                                            }
                                        }
                                    }
                                    else{
                                        (*animations)[animationPos].setX((*animations)[animationPos].getX() + animState->getFrameW()); //Change the frame position to the next
                                        if((*animations)[animationPos].getX() >= animState->getTextureW()){ //If the x position is greater than the texture width
                                            (*animations)[animationPos].setX(0); //Reset the x to 0
                                            (*animations)[animationPos].setY((*animations)[animationPos].getY() + animState->getFrameH()); //add to the y
                                            if((*animations)[animationPos].getY() >= animState->getTextureH()){ //If the y is greater than the texture height
                                                (*animations)[animationPos].setY(0); //Reset y to 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        animState = nullptr;
                    }
                }
            }
        }
        animations = nullptr;
        animators = nullptr;
    }

    /**
        Method to handle received messages present in every system.
    **/
    void AnimationSystem::handleMessage(Message* msg){
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
