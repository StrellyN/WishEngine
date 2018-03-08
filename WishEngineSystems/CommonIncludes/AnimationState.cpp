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

#include "AnimationState.hpp"

namespace WishEngine{
    AnimationState::AnimationState(){
        reverseAnimation = false;
        initialX = 0;
        initialY = 0;
        frameW = 0;
        frameH = 0;
        textureW = 0;
        textureH = 0;
        animationSpeed = 0;
        animationCounter = 0;
        animationFrames = 0;
        frameCounter = 0;
    }

    AnimationState::AnimationState(bool revAnim, double iniX, double iniY, double frW, double frH,
                   double texW, double texH, double animSpeed, double animFrames){
        reverseAnimation = revAnim;
        initialX = iniX;
        initialY = iniY;
        frameW = frW;
        frameH = frH;
        textureW = texW;
        textureH = texH;
        animationSpeed = animSpeed;
        animationCounter = 0;
        animationFrames = animFrames;
        frameCounter = 0;
    }

    AnimationState::~AnimationState(){

    }

    void AnimationState::setReverseAnimation(bool revAnim){
        reverseAnimation = revAnim;
    }

    void AnimationState::setInitialX(double iX){
        initialX = iX;
    }

    void AnimationState::setInitialY(double iY){
        initialY = iY;
    }

    void AnimationState::setFrameW(double fW){
        frameW = fW;
    }

    void AnimationState::setFrameH(double fH){
        frameH = fH;
    }

    void AnimationState::setTextureW(double tW){
        textureW = tW;
    }

    void AnimationState::setTextureH(double tH){
        textureH = tH;
    }

    void AnimationState::setAnimationSpeed(double aS){
        animationSpeed = aS;
    }

    void AnimationState::setAnimationCounter(double aC){
        animationCounter = aC;
    }

    void AnimationState::setAnimationFrames(double aF){
        animationFrames = aF;
    }

    void AnimationState::setFrameCounter(double fC){
        frameCounter = fC;
    }

    bool AnimationState::getReverseAnimation(){
        return reverseAnimation;
    }

    double AnimationState::getInitialX(){
        return initialX;
    }

    double AnimationState::getInitialY(){
        return initialY;
    }

    double AnimationState::getFrameW(){
        return frameW;
    }

    double AnimationState::getFrameH(){
        return frameH;
    }

    double AnimationState::getTextureW(){
        return textureW;
    }

    double AnimationState::getTextureH(){
        return textureH;
    }

    double AnimationState::getAnimationSpeed(){
        return animationSpeed;
    }

    double AnimationState::getAnimationCounter(){
        return animationCounter;
    }

    double AnimationState::getAnimationFrames(){
        return animationFrames;
    }

    double AnimationState::getFrameCounter(){
        return frameCounter;
    }
}
