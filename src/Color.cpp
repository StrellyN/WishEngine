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

#include "Color.hpp"

namespace WishEngine{
    Color::Color(){
        R = 255;
        G = 255;
        B = 255;
        A = 255;
    }

    Color::Color(int r, int g, int b, int a){
        if(r<0) r=0;
        if(r>255) r=255;
        if(g<0) g=0;
        if(g>255) g=255;
        if(b<0) b=0;
        if(b>255) b=255;
        if(a<0) a=0;
        if(a>255) a=255;
        R = r;
        G = g;
        B = b;
        A = a;
    }

    Color::~Color(){}

    int Color::getR(){
        return R;
    }

    int Color::getG(){
        return G;
    }

    int Color::getB(){
        return B;
    }

    int Color::getA(){
        return A;
    }

    void Color::setR(int r){
        if(r<0) r=0;
        if(r>255) r=255;
        R = r;
    }

    void Color::setG(int g){
        if(g<0) g=0;
        if(g>255) g=255;
        G = g;
    }

    void Color::setB(int b){
        if(b<0) b=0;
        if(b>255) b=255;
        B = b;
    }

    void Color::setA(int a){
        if(a<0) a=0;
        if(a>255) a=255;
        A = a;
    }
}
