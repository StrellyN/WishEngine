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

#ifndef SCRIPTSINTERFACE_H
#define SCRIPTSINTERFACE_H

#include "../CommonIncludes/System.hpp"
#include "../CommonIncludes/Collection.hpp"
#include "../CommonIncludes/GameObject.hpp"
#include "../CommonIncludes/CreateScriptMessage.hpp"
#include "../CommonIncludes/ObjectListMessage.hpp"
#include "../CommonIncludes/AvailableObjectsMessage.hpp"
#include "../CommonIncludes/ComponentListMessage.hpp"
#include "../CommonIncludes/CreateWindowMessage.hpp"
#include "../CommonIncludes/VectorMessage.hpp"
#include "../CommonIncludes/ScriptComponent.hpp"

#include "../CommonIncludes/ComponentHeaders.hpp"

namespace WishEngine{
    enum SCRIPTTYPES{};

    class ScriptsInterface : public GameSystem{
        private:
            static std::unordered_map<int, BaseCollection*> *components;
            static std::vector<GameObject> *objects;
            static std::vector<unsigned> *availableObjects;
            static std::vector<Message*> *postedM;

        public:
            ScriptsInterface();
            virtual ~ScriptsInterface();
            void update(double dt);
            void handleMessage(Message* mes);
            void createScript(std::unordered_map<int, BaseCollection*> *col, GameObject *own, unsigned ownPos, bool isEn, std::string n, std::string ar);

            static unsigned getObjectQuantity();

            static void insertObject(std::string name, bool isEnabled);
            static unsigned deleteObject(std::string name);
            static unsigned deleteObject(unsigned id);
            static GameObject *getObject(std::string name);
            static GameObject *getObject(unsigned id);
            static GameObject *getObjectByPos(unsigned pos);

            template <class T>
            static unsigned insertComponent(std::string oName, T component, std::string name = "");
            template <class T>
            static unsigned insertComponent(unsigned oId, T component, std::string name = "");
            template <class T>
            static T *getComponent(GameObject *obj, int type, std::string name = "");
            template <class T>
            static unsigned deleteComponent(GameObject *obj, int type, std::string name = "");

            static void deleteWindow(std::string name);
            static void createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h);
            //static void getWindowSize(std::string windowN, int &w, int &h);
            static void goToState(std::string stateFile);
            static void createSaveState();
            static void loadSaveState();
            static void setMaxFPS(int mFPS);
            static void setFrameCapFlag(bool frameCap);
            static void quit();
            static void fullScreen(std::string window);
            static void setJoystickDeadzone(int joyId, int joyDeadzone);
            //static bool getFrameCapFlag();
            //static int getMaxFPS();
    };
}
#endif // SCRIPTSINTERFACE
