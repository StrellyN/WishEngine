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

#include "NetworkComponent.hpp"

namespace WishEngine{
    NetworkComponent::NetworkComponent(){
        setType("Network");
        ip = "";
        maxPacketSize = 0;
        maxConnections = 0;
        isServer = false;
        isTcp = false;
        isConnected = false;
        disconnected = false;
        attemptConnection = false;
        port = 0;
        netSocketSetIndex = -1;
        connectionFailed = false;
        elapsedTimeBetweenChecks = 1000;
    }

    NetworkComponent::NetworkComponent(bool isS, bool isT, unsigned maxSize, unsigned elapsedTime, unsigned maxC){
        setType("Network");
        ip = "";
        maxPacketSize = maxSize;
        maxConnections = maxC;
        isServer = isS;
        isTcp = isT;
        isConnected = false;
        disconnected = false;
        attemptConnection = false;
        port = 0;
        netSocketSetIndex = -1;
        connectionFailed = false;
        elapsedTimeBetweenChecks = elapsedTime;
    }

    NetworkComponent::~NetworkComponent(){
        clearData();
    }

    void NetworkComponent::clearData(){
        //Call system method to destroy the sockets in the index vector
        netSocketSetIndex = -1;
        socketsIndex.clear();
        toSend.clear();
        received.clear();
        ip = "";
        port = 0;
    }

    void NetworkComponent::connect(std::string ip_, uint16_t port_){
        ip = ip_;
        port = port_;
        attemptConnection = true;
    }

    void NetworkComponent::setIsConnected(bool con){
        isConnected = con;
    }

    void NetworkComponent::disconnect(){
        disconnected = true;
    }

    void NetworkComponent::send(Packet p){
        toSend.push_back(p);
    }

    std::vector<Packet> &NetworkComponent::getReceived(){
        return received;
    }

    std::vector<Packet> &NetworkComponent::getSent(){
        return toSend;
    }

    std::vector<unsigned> &NetworkComponent::getSocketsIndex(){
        return socketsIndex;
    }

    std::string &NetworkComponent::getIp(){
        return ip;
    }

    unsigned NetworkComponent::getMaxPacketSize(){
        return maxPacketSize;
    }

    void NetworkComponent::setMaxPacketSize(unsigned maxSize){
        maxPacketSize = maxSize;
    }

    unsigned NetworkComponent::getMaxConnections(){
        return maxConnections;
    }

    void NetworkComponent::setMaxConnections(unsigned mC){
        maxConnections = mC;
    }

    uint16_t NetworkComponent::getPort(){
        return port;
    }

    bool NetworkComponent::getIsServer(){
        return isServer;
    }

    void NetworkComponent::setIsServer(bool isS){
        isServer = isS;
    }

    bool NetworkComponent::getIsTcp(){
        return isTcp;
    }

    bool NetworkComponent::getIsConnected(){
        return isConnected;
    }

    bool NetworkComponent::getDisconnect(){
        return disconnected;
    }

    void NetworkComponent::setDisconnect(bool dis){
        disconnected = dis;
    }

    bool NetworkComponent::getAttemptConnection(){
        return attemptConnection;
    }

    void NetworkComponent::setAttemptConnection(bool attC){
        attemptConnection = attC;
    }

    unsigned NetworkComponent::getNetSocketSetIndex(){
        return netSocketSetIndex;
    }

    void NetworkComponent::setNetSocketSetIndex(unsigned ind){
        netSocketSetIndex = ind;
    }

    void NetworkComponent::setConnectionFailed(bool con){
        connectionFailed = con;
    }

    bool NetworkComponent::getConnectionFailed(){
        return connectionFailed;
    }

    unsigned NetworkComponent::getElapsedTimeBetweenChecks(){
        return elapsedTimeBetweenChecks;
    }

    void NetworkComponent::setElapsedTimeBetweenChecks(unsigned time){
        elapsedTimeBetweenChecks = time;
    }
}
