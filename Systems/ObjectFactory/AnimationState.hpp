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

#ifndef ANIMATIONSTATE_H_
#define ANIMATIONSTATE_H_

namespace WishEngine{
    class AnimationState{
        private:
            bool reverseAnimation;
            double initialX, initialY;
            double frameW, frameH;
            double textureW, textureH;
            double animationSpeed, animationCounter;
            double animationFrames, frameCounter;
        public:
            AnimationState();
            AnimationState(bool revAnim, double iniX, double iniY, double frW, double frH,
                           double texW, double texH, double animSpeed, double animFrames);
            ~AnimationState();
            void setReverseAnimation(bool revAnim);
            void setInitialX(double iX);
            void setInitialY(double iY);
            void setFrameW(double fW);
            void setFrameH(double fH);
            void setTextureW(double tW);
            void setTextureH(double tH);
            void setAnimationSpeed(double aS);
            void setAnimationCounter(double aC);
            void setAnimationFrames(double aF);
            void setFrameCounter(double fC);
            bool getReverseAnimation();
            double getInitialX();
            double getInitialY();
            double getFrameW();
            double getFrameH();
            double getTextureW();
            double getTextureH();
            double getAnimationSpeed();
            double getAnimationCounter();
            double getAnimationFrames();
            double getFrameCounter();
    };
}
#endif // ANIMATIONSTATE_H_
