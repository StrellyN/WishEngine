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

#include "CreateWindowMessage.hpp"

namespace WishEngine{
    CreateWindowMessage::CreateWindowMessage(std::string ti, std::string na, std::string ic, int x_, int y_, int w_, int h_){
        setType("CREATEWINDOW");
        title = ti;
        name = na;
        icon = ic;
        x = x_;
        y = y_;
        w = w_;
        h = h_;
    }

    CreateWindowMessage::~CreateWindowMessage(){

    }

    std::string CreateWindowMessage::getTitle(){
        return title;
    }

    std::string CreateWindowMessage::getName(){
        return name;
    }

    std::string CreateWindowMessage::getIcon(){
        return icon;
    }

    int CreateWindowMessage::getX(){
        return x;
    }

    int CreateWindowMessage::getY(){
        return y;
    }

    int CreateWindowMessage::getW(){
        return w;
    }

    int CreateWindowMessage::getH(){
        return h;
    }
}
