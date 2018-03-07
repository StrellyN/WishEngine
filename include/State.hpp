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

#ifndef STATE_H
#define STATE_H

#include "System.hpp"

#ifdef _WIN32
    #include "windows.h"
#elif defined(__unix__) || defined(__linux__)
    #include <dlfcn.h>
    #include <dirent.h>
#endif

namespace WishEngine{
    class State{
        private:
            #ifdef _WIN32
                std::vector<HINSTANCE> systemsDLL;
            #elif defined(__unix__) || defined(__linux__)
                std::vector<void*> systemsDLL;
            #endif
            std::vector<GameSystem*> systems;
            bool quit = false;
        public:
            State();
            ~State();
            void loadSystems();
            void update();
            void render(double interpolation);
            void handleMessage(Message *msg);
            void handleMessages();
            void sendMessage(Message *mes);
            void addSystem(GameSystem *nSystem);
            GameSystem *getSystem(std::string type);
            std::vector<GameSystem*>& getSystems();
            void setSystems(std::vector<GameSystem*>& nSystems);
            bool hasSystemType(std::string type);
            bool getQuit();
            void setQuit(bool q);
    };
}
#endif // STATE
