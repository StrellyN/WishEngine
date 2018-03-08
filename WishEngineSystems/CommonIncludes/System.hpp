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

#ifndef SYSTEM_H
#define SYSTEM_H

#include "Message.hpp"

namespace WishEngine{
    class GameSystem{
        private:
            std::string systemType;
            std::vector<Message*> postedMessages;
            int updatePriority = 0;

        public:
            virtual ~GameSystem(){

            }
            virtual void update(double dt) = 0;
            virtual void handleMessage(Message *msg) = 0;
            void destroySystem(){
                for(unsigned i=0; i<postedMessages.size(); i++){
                    if(postedMessages[i] != nullptr){
                        delete postedMessages[i];
                        postedMessages[i] = nullptr;
                    }
                }
                postedMessages.clear();
            }
            std::string &getSystemType(){
                return systemType;
            }
            void setSystemType(std::string type){
                systemType = type;
            }
            void postMessage(Message *toPost){
                postedMessages.push_back(toPost);
            }
            std::vector<Message*> &getMessages(){
                return postedMessages;
            }
            unsigned getMessagesAmount(){
                return postedMessages.size();
            }
            Message *getMessage(unsigned pos){
                if(pos < postedMessages.size() && pos >= 0){
                    return postedMessages[pos];
                }
                return nullptr;
            }
            void deleteMessage(unsigned pos){
                if(pos < postedMessages.size() && pos >= 0){
                    delete postedMessages[pos];
                    postedMessages[pos] = nullptr;
                    postedMessages.erase(postedMessages.begin() + pos);
                }
            }
            int getUpdatePriority(){
                return updatePriority;
            }
            void setUpdatePriority(int pri){
                updatePriority = pri;
            }
    };
}
#endif // SYSTEM
