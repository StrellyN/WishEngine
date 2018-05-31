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

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

//Component that defines an object as a camera, providing the priority of rendering and
//the offset for the camera to render on screen, basically the position of the camera.

#include "Component.hpp"

namespace WishEngine{
    class CameraComponent : public Component{
        private:
            int priority;
            DirectionVector rect = DirectionVector(0, 0);
        public:
            CameraComponent(int pri);
            CameraComponent(int pri, double x, double y);
            CameraComponent(int pri, double x, double y, double z);
            virtual ~CameraComponent();
            int getPriority();
            double getX();
            double getY();
            double getZ();
            void setPriority(int pri);
            void setX(double x);
            void setY(double y);
            void setZ(double z);
    };
}
#endif // CAMERACOMPONENT_H
