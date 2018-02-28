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

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "ComponentListMessage.hpp"
#include "ComponentHeaders.hpp"
#include "ObjectListMessage.hpp"
#include "CreateScriptMessage.hpp"
#include "CreateWindowMessage.hpp"
#include "Collection.hpp"
#include "GameObject.hpp"
#include "System.hpp"

namespace WishEngine{
    class ObjectFactory : public GameSystem{ //This class is also the interface scripts use to communicate with the engine.
        private:
            std::vector<GameObject> objects;
            std::map<std::string, BaseCollection*> componentCollections;

        public:
            ObjectFactory();
            virtual ~ObjectFactory();
            void clearData(); //Send a message for each object telling it's been destroyed
            void loadObjects(std::string file);
            std::string checkObjectName(std::string &name);
            std::vector<GameObject> &getObjects();
            std::map<std::string, BaseCollection*> &getComponentCollections();
            void deleteObject(unsigned objPos);

            void update(double dt);
            void handleMessage(Message* msg);

            /**void insertObject(GameObject obj);
            void deleteObject(std::string name);
            void deleteObject(unsigned id);
            GameObject *getObject(std::string name);
            GameObject *getObject(unsigned id);**/
    };
}
#endif // OBJECTFACTORY
