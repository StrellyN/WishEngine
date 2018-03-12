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

#ifndef EVENT_H
#define EVENT_H

#include "Headers.hpp"

namespace WishEngine{
    enum EVENTTYPES{KEYBOARD_PRESS, KEYBOARD_RELEASE, MOUSE_PRESS, MOUSE_RELEASE, GAMEPAD_ADDED, GAMEPAD_REMOVED, GAMEPAD_PRESS,
                    GAMEPAD_RELEASE, GAMEPAD_AXIS, MOUSE_WHEEL, MOUSE_MOTION, EQUIT, NULLEVENT};

    class Event{
        private:
            EVENTTYPES type;
            std::string value;
            int deviceID, xPos, yPos, xRel, yRel;
            unsigned timeStamp, windowID;
        public:
            Event();
            Event(EVENTTYPES t, std::string val);
            ~Event();
            EVENTTYPES getType();
            void setType(EVENTTYPES t);
            std::string &getValue();
            void setValue(std::string val);
            int getDeviceID();
            int getXPos();
            int getYPos();
            int getXRel();
            int getYRel();
            unsigned getTimeStamp();
            unsigned getWindowID();
            void setDeviceID(int id);
            void setXPos(int x);
            void setYPos(int y);
            void setXRel(int x);
            void setYRel(int y);
            void setTimeStamp(unsigned time);
            void setWindowID(unsigned window);
    };
}
#endif // EVENT
