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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.hpp"

namespace WishEngine{
    class GameObject : public Object{
        private:
            std::vector<Component*> components;
            int componentPosition[C_TYPES::CTYPESIZE] = {-1};
        public:
            GameObject(std::string n = "Object", bool isEnabled = true);
            virtual ~GameObject();
            void destroyComponents();
            void addComponent(Component* comp, std::string n = "");
            Component* getComponent(C_TYPES type, std::string n = "");
            void deleteComponent(C_TYPES type, std::string n = "");
            bool hasComponent(C_TYPES type, std::string n = "");
            std::vector<Component*>& getComponents();
            void setComponents(std::vector<Component*>& comp);
    };
}
#endif // GAMEOBJECT
