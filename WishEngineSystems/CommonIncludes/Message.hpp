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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Headers.hpp"

namespace WishEngine{
    enum MESSAGETYPES{QUIT, SFRAME, RFRAME, FFRAME, HANDLEINPUT
                      //New message types bellow
                      ,INPUTLIST, COMPONENTLIST, OBJECTLIST, DELETEEVERYTHING, CREATEWINDOW,
                      FULLSCREEN, CONNECTNET, UPDATENET, DISCONNECTNET, FADEINMUSIC, FADEOUTMUSIC, FADEINMUSICPOS, PLAYMUSIC,
                      PLAYSOUND, SETMUSICPOS, SETMUSICVOLUME, DELETEMUSIC, DELETESOUND, PAUSEMUSIC, RESUMEMUSIC, STOPMUSIC,
                      DELETEWINDOW, SETMAXFPS, SETFRAMECAPFLAG, CREATESCRIPT, AVAILABLEOBJECTS, LOADOBJECTS, GOTOSTATE,
                      CHECKCOLLISIONS, SETJOYSTICKDEADZONE};

    class Message{
        private:
            MESSAGETYPES type;
            std::string value;
            double numericValue;
        public:
            Message(MESSAGETYPES t);
            Message(MESSAGETYPES mT, std::string val);
            Message(MESSAGETYPES mT, double numVal);
            Message(MESSAGETYPES mT, std::string val, double numVal);
            Message();
            virtual ~Message(){}
            MESSAGETYPES getType();
            void setType(MESSAGETYPES t);
            std::string &getValue();
            void setValue(std::string val);
            double getNumericValue();
            void setNumericValue(double numVal);
    };
}
#endif // MESSAGE
