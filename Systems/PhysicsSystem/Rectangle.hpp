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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Headers.hpp"

namespace WishEngine{
    class Rectangle{
        private:
            double X, Y, Z, W, H, L;
        public:
            Rectangle();
            Rectangle(double x, double y, double w, double h);
            Rectangle(double x, double y, double z, double w, double h, double l);
            ~Rectangle();
            void setX(double x);
            void setY(double y);
            void setZ(double z);
            void setW(double w);
            void setH(double h);
            void setL(double l);
            double getX();
            double getY();
            double getZ();
            double getW();
            double getH();
            double getL();
    };
}
#endif // RECTANGLE
