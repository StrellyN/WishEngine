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
    AnimationSystem::AnimationSystem(){
        setSystemType(S_TYPES::ANIMATIONS); //Set the system type
    }

    AnimationSystem::~AnimationSystem(){
        destroySystem();
    }

    /**
        Method to update the logic present in every system.
        In this case it checks for the objects that have the components necesary for animating them and then
        proceeds to process the information and animate the object.
    **/
    void AnimationSystem::update(double dt){
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<objs.size(); i++){
            if(objs[i].getEnabled() && objs[i].hasComponent(C_TYPES::ANIMATION) && objs[i].hasComponent(C_TYPES::ANIMATOR)){
                AnimationComponent* animComp = dynamic_cast<AnimationComponent*>(objs[i].getComponent(C_TYPES::ANIMATION));
                AnimatorComponent* atorComp = dynamic_cast<AnimatorComponent*>(objs[i].getComponent(C_TYPES::ANIMATOR));
                if(animComp != nullptr && animComp->getEnabled() && atorComp != nullptr && atorComp->getEnabled()){
                    AnimationState* animState = atorComp->getAnimationState(atorComp->getCurrentState());
                    if(animState != nullptr){
                        animComp->setW(animState->getFrameW()); //Set the correct size of the frame
                        animComp->setH(animState->getFrameH());
                        animState->setAnimationCounter(animState->getAnimationCounter() - (1*animState->getAnimationSpeed())); //Add to the animator counter
                        if(animState->getAnimationCounter() <= 0){ //Go to next frame if counter reached goal
                            animState->setAnimationCounter(1-animState->getAnimationSpeed()); //Set the counter back to 0
                            animState->setFrameCounter(animState->getFrameCounter()+1); //Go to next frame
                            if(animState->getFrameCounter() >= animState->getAnimationFrames()){ //If next frame is greater than the length of the anim
                                animState->setFrameCounter(0); //Go back to the beginning
                                animComp->setX(animState->getInitialX());
                                animComp->setY(animState->getInitialY());
                            }
                            else{
                                if(animState->getReverseAnimation()){ //If the animation is reversed
                                    animComp->setX(animComp->getX() - animState->getFrameW()); //Change the frame position to the next
                                    if(animComp->getX() <= 0){ //If the x position is less than 0
                                        animComp->setX(animState->getTextureW() - animState->getFrameW()); //Reset the x to Max texture pos
                                        animComp->setY(animComp->getY() - animState->getFrameH()); //add to the y
                                        if(animComp->getY() <= 0){ //If the y is less than 0
                                            animComp->setY(animState->getTextureH() - animState->getFrameH()); //Reset y to Max texture pos;
                                        }
                                    }
                                }
                                else{
                                    animComp->setX(animComp->getX() + animState->getFrameW()); //Change the frame position to the next
                                    if(animComp->getX() >= animState->getTextureW()){ //If the x position is greater than the texture width
                                        animComp->setX(0); //Reset the x to 0
                                        animComp->setY(animComp->getY() + animState->getFrameH()); //add to the y
                                        if(animComp->getY() >= animState->getTextureH()){ //If the y is greater than the texture height
                                            animComp->setY(0); //Reset y to 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    animState = nullptr;
                }
                animComp = nullptr;
                atorComp = nullptr;
            }
        }
    }

    /**
        Method to handle received messages present in every system.
    **/
    void AnimationSystem::handleMessage(Message* msg){

    }
}
