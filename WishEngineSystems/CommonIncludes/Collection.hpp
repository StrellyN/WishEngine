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

#ifndef COLLECTION_H
#define COLLECTION_H

#include "Component.hpp"

namespace WishEngine{
    class BaseCollection{
        private:
            int type;

        public:
            virtual ~BaseCollection(){}
            void setType(int ty){
                type = ty;
            }
            int getType(){
                return type;
            }
    };

    template <class T>
    class Collection : public BaseCollection{
        private:
            std::vector<T> collection;
            std::vector<unsigned> availablePositions;

        public:
            Collection(int ty);
            virtual ~Collection();
            unsigned addItem(T item, unsigned ownrId, std::string name = "");
            T &getItem(unsigned ownrId, std::string name);
            unsigned deleteItem(unsigned ownrId, std::string name);
            bool hasItem(unsigned ownrId, std::string name);
            bool hasItem(unsigned ownrId);
            std::vector<T> &getCollection();
            unsigned deleteByPos(unsigned pos);
    };

    template <class T>
    Collection<T>::Collection(int ty){
        setType(ty);
    }

    template <class T>
    Collection<T>::~Collection(){
        collection.clear();
        availablePositions.clear();
    }

    template <class T>
    unsigned Collection<T>::addItem(T item, unsigned ownrId, std::string name){
        if(item.getType() != COMPONENTTYPES::AUDIO && hasItem(ownrId)){
            return 0;
        }
        int i=1;
        std::string originalName = item.getName();
        while(name == "" || hasItem(ownrId, name)){
            if(name == ""){
                name = "Component";
                originalName = name;
                continue;
            }
            name = originalName;
            name += Utils::intToString(i);
            i++;
        }
        if(!hasItem(ownrId, name)){
            if(availablePositions.empty()){
                item.setName(name);
                collection.push_back(item);
                return collection.size();
            }
            else{
                item.setName(name);
                unsigned newPos = availablePositions[availablePositions.size() - 1];
                collection[newPos] = item;
                availablePositions.pop_back();
                return newPos + 1;
            }
        }
        return 0;
    }

    template <class T>
    T &Collection<T>::getItem(unsigned ownrId, std::string name){
        for(unsigned i=0; i<collection.size(); i++){
            if(!collection[i].getDeleted() && collection[i].getName() == name && collection[i].getOwnerId() == ownrId){
                return collection[i];
            }
        }
        return NULL;
    }

    template<class T>
    unsigned Collection<T>::deleteByPos(unsigned pos){
        if(pos > 0 && pos < collection.size() && !collection[pos].getDeleted()){
            availablePositions.push_back(pos);
            collection[pos].setDeleted(true);
            return 1;
        }
        return 0;
    }

    template <class T>
    unsigned Collection<T>::deleteItem(unsigned ownrId, std::string name){
        for(unsigned i=0; i<collection.size(); i++){
            if(!collection[i].getDeleted() && collection[i].getName() == name && collection[i].getOwnerId() == ownrId){
                availablePositions.push_back(i);
                collection[i].setDeleted(true);
                return 1;
            }
        }
        return 0;
    }

    template <class T>
    bool Collection<T>::hasItem(unsigned ownrId, std::string name){
        for(unsigned i=0; i<collection.size(); i++){
            if(!collection[i].getDeleted() && collection[i].getName() == name && collection[i].getOwnerId() == ownrId){
                return true;
            }
        }
        return false;
    }

    template <class T>
    bool Collection<T>::hasItem(unsigned ownrId){
        for(unsigned i=0; i<collection.size(); i++){
            if(!collection[i].getDeleted() && collection[i].getOwnerId() == ownrId){
                return true;
            }
        }
        return false;
    }

    template <class T>
    std::vector<T> &Collection<T>::getCollection(){
        return collection;
    }
}
#endif // COLLECTION_H

