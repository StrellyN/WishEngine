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

#include "Message.hpp"

namespace WishEngine{
    Message::Message(MESSAGETYPES t){
        setType(t);
        value = "";
        numericValue = 0;
    }

    Message::Message(MESSAGETYPES mT, std::string val){
        setType(mT);
        value = val;
        numericValue = 0;
    }

    Message::Message(MESSAGETYPES mT, double numVal){
        setType(mT);
        value = "";
        numericValue = numVal;
    }

    Message::Message(MESSAGETYPES mT, std::string val, double numVal){
        setType(mT);
        setValue(val);
        setNumericValue(numVal);
    }

    Message::Message(){

    }

    MESSAGETYPES Message::getType(){
        return type;
    }

    void Message::setType(MESSAGETYPES t){
        type = t;
    }

    void Message::setValue(std::string val){
        value = val;
    }

    std::string &Message::getValue(){
        return value;
    }

    double Message::getNumericValue(){
        return numericValue;
    }

    void Message::setNumericValue(double numVal){
        numericValue = numVal;
    }
}
