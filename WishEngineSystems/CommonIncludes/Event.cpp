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

#include "Event.hpp"

namespace WishEngine{
    Event::Event(){
        setType(EVENTTYPES::NULLEVENT);
        setValue("NULL");
        xPos = 0;
        xRel = 0;
        yPos = 0;
        yRel = 0;
        deviceID = -1;
        timeStamp = 0;
        windowID = -1;
    }

    Event::Event(EVENTTYPES t, std::string val){
        setType(t);
        setValue(val);
        xPos = 0;
        xRel = 0;
        yPos = 0;
        yRel = 0;
        deviceID = -1;
        timeStamp = 0;
        windowID = -1;
    }

    Event::~Event(){

    }

    /**
        All the getters and setters for the event attributes, not all of them may be use for every
        type of event so here is a list of what is used for each type:
        Do the list
    **/

    EVENTTYPES Event::getType(){
        return type;
    }

    void Event::setType(EVENTTYPES t){
        type = t;
    }

    std::string &Event::getValue(){
        return value;
    }

    void Event::setValue(std::string val){
        value = val;
    }

    int Event::getDeviceID(){
        return deviceID;
    }

    int Event::getXPos(){
        return xPos;
    }

    int Event::getYPos(){
        return yPos;
    }

    int Event::getXRel(){
        return xRel;
    }

    int Event::getYRel(){
        return yRel;
    }

    unsigned Event::getTimeStamp(){
        return timeStamp;
    }

    unsigned Event::getWindowID(){
        return windowID;
    }

    void Event::setDeviceID(int id){
        deviceID = id;
    }

    void Event::setXPos(int x){
        xPos = x;
    }

    void Event::setYPos(int y){
        yPos = y;
    }

    void Event::setXRel(int x){
        xRel = x;
    }

    void Event::setYRel(int y){
        yRel = y;
    }

    void Event::setTimeStamp(unsigned time){
        timeStamp = time;
    }

    void Event::setWindowID(unsigned window){
        windowID = window;
    }
}
