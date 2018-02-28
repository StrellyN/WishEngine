/**
    Copyright 2017 Strelly

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

#include "System.hpp"
#include "Collection.hpp"
#include "GameObject.hpp"
#include "CreateScriptMessage.hpp"
#include "ObjectListMessage.hpp"
#include "ComponentListMessage.hpp"
#include "CreateWindowMessage.hpp"
#include "ScriptComponent.hpp"

//Include all components
#include "GraphicComponent.hpp"

namespace WishEngine{
    class ScriptsInterface : public GameSystem{
        private:
            static std::map<std::string, BaseCollection*> *components;
            static std::vector<GameObject> *objects;
            static std::vector<Message*> *postedM;

        public:
            ScriptsInterface();
            virtual ~ScriptsInterface();
            void update(double dt);
            void handleMessage(Message* mes);
            void createScript(Collection<BaseCollection*> *col, GameObject *own, unsigned ownPos, bool isEn, std::string n, std::string ar);

            static void insertObject(std::string name, bool isEnabled);
            static unsigned deleteObject(std::string name);
            static unsigned deleteObject(unsigned id);
            static GameObject *getObject(std::string name);
            static GameObject *getObject(unsigned id);

            template <class T>
            static unsigned insertComponent(unsigned oId, T component, std::string name = "");
            template <class T>
            static T *getComponent(GameObject *obj, std::string type, std::string name = "");
            template <class T>
            static unsigned deleteComponent(GameObject *obj, std::string type, std::string name = "");

            static void deleteWindow(std::string name);
            static void createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h);
            //static void getWindowSize(std::string windowN, int &w, int &h);
            static void goToState(std::string stateFile);
            static void createSaveState();
            static void loadSaveState();
            static void setMaxFPS(int mFPS);
            static void setFrameCapFlag(bool frameCap);
            //static bool getFrameCapFlag();
            //static int getMaxFPS();
    };
}
#endif // SCRIPTSINTERFACE
