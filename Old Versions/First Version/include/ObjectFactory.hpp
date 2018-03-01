#ifndef OBJECTFACTORY_H
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

#define OBJECTFACTORY_H

#include "GameObject.hpp"
#include "Message.hpp"
#include "Camera.hpp"

namespace WishEngine{
    class ObjectFactory{ //This class is also the interface scripts use to communicate with the engine.
        private:
            static ObjectFactory *objFactory;
            ObjectFactory();

            std::vector<GameObject> objects;
            std::vector<Camera> cameras;

            std::vector<Message*> postedMessages;
        public:
            static ObjectFactory* getObjectFactory();
            ~ObjectFactory();
            void clearData();
            void destroyObjectFactory();
            void createObjects(std::string file);
            void createObject(std::string data);
            void insertObject(GameObject obj);
            void deleteObject(int pos);
            GameObject* getObject(int pos);
            void deleteObject(std::string name);
            GameObject* getObject(std::string name);
            int getObjectPos(std::string name);
            std::vector<GameObject>& getObjects();
            void setObjects(std::vector<GameObject>& obj);

            void addCamera(Camera cam);
            Camera* getCamera(std::string name);
            Camera* getCamera(int pos);
            void deleteCamera(std::string name);
            void sortCameras();
            std::vector<Camera>& getCameras();

            void deleteWindow(std::string name);
            void createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h);
            void getWindowSize(std::string windowN, int &w, int &h);

            std::vector<Message*>& getMessages();
            void goToState(std::string stateFile);
            void update(double dt);
            void handleMessage(Message* msg);
            void postMessage(Message *toPost);

            void createSaveState(std::string file);
            void loadSaveState(std::string file);
            void setMaxFPS(int mFPS);
            void setFrameCapFlag(bool frameCap);
            bool getFrameCapFlag();
            int getMaxFPS();
    };
}
#endif // OBJECTFACTORY
