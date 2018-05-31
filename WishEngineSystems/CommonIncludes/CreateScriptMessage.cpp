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

#include "CreateScriptMessage.hpp"

namespace WishEngine{
    CreateScriptMessage::CreateScriptMessage(std::unordered_map<int, BaseCollection*> *col, GameObject *own, unsigned ownPos, bool isEn, std::string n, std::string ar){
        setType(MESSAGETYPES::CREATESCRIPT);
        collection = col;
        owner = own;
        ownerPos = ownPos;
        isScriptEnabled = isEn;
        name = n;
        args = ar;
    }

    CreateScriptMessage::~CreateScriptMessage(){
        collection = nullptr;
        owner = nullptr;
    }

    std::unordered_map<int, BaseCollection*> *CreateScriptMessage::getCollection(){
        return collection;
    }

    bool CreateScriptMessage::getIsScriptEnabled(){
        return isScriptEnabled;
    }

    std::string CreateScriptMessage::getName(){
        return name;
    }

    std::string CreateScriptMessage::getArgs(){
        return args;
    }

    GameObject *CreateScriptMessage::getOwner(){
        return owner;
    }

    unsigned CreateScriptMessage::getOwnerPos(){
        return ownerPos;
    }
}
