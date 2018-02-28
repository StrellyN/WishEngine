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

#include "HppHeaders.hpp"

namespace WishEngine{
    NetworkSystem::NetworkSystem(){
        setSystemType(S_TYPES::NETWORK);
    }

    NetworkSystem::~NetworkSystem(){
        destroySystem();
    }

    void NetworkSystem::update(double dt){
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<objs.size(); i++){
            if(objs[i].getEnabled()){
                NetworkComponent *netComp = dynamic_cast<NetworkComponent*>(objs[i].getComponent(C_TYPES::NETWORKC));
                if(netComp != nullptr && netComp->getEnabled()){
                    //Do all the stuff here :D
                    if(netComp->getIsConnected()){ //Do the checks for packet sending and receiving
                        if(netComp->getDisconnect()){ //Disconnect the component and delete all its stuffs in the framework
                            Framework::getFramework()->deleteNet(netComp);
                        }
                        else{ //The checks continue :)
                            Framework::getFramework()->updateNet(netComp);
                        }
                    }
                    else{ //Check if you need to connect.
                        if(netComp->getAttemptConnection()){ //Try to connect
                            Framework::getFramework()->connectNet(netComp);
                        }
                    }
                }
                netComp = nullptr;
            }
        }
    }

    void NetworkSystem::handleMessage(Message *msg){

    }
}
