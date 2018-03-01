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

#ifndef HITBOXCOMPONENT_H
#define HITBOXCOMPONENT_H

#include "Component.hpp"

namespace WishEngine{
    class HitboxComponent : public Component{
        private:
            DirectionVector offset = DirectionVector(0, 0);
            double W, H, L;
            bool checkForCollisions, isSolid, isColliding;
            std::vector<CollisionStruct> collisionList;
        public:
            HitboxComponent(double offX, double offY, double W, double H, bool solid, bool checkCollisions);
            HitboxComponent(double offX, double offY, double offZ, double W, double H, double L, bool solid, bool checkCollisions);
            ~HitboxComponent();
            void setOffset(double offX, double offY, double offZ);
            double getOffsetX();
            double getOffsetY();
            double getOffsetZ();
            void setW(double w);
            void setH(double h);
            void setL(double l);
            double getW();
            double getH();
            double getL();
            bool getIsColliding();
            void setIsColliding(bool isColl);
            bool getCheckForCollisions();
            void setCheckForCollisions(bool check);
            bool getIsSolid();
            void setIsSolid(bool solid);
            std::vector<CollisionStruct>& getCollisionList();
            void addCollisionElement(unsigned ind, std::string n);
    };
}
#endif // HITBOXCOMPONENT
