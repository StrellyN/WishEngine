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

#ifndef NETWORKCOMPONENT_H
#define NETWORKCOMPONENT_H

namespace WishEngine{
    typedef struct Packet{
        char *data;
    };

    class NetworkComponent : public Component{
        private:
            std::vector<Packet> toSend, received;
            std::vector<unsigned> socketsIndex;
            std::string ip;
            unsigned maxPacketSize, maxConnections, netSocketSetIndex, elapsedTimeBetweenChecks;
            bool isServer, isTcp, isConnected, disconnected, attemptConnection, connectionFailed;
            uint16_t port;
        public:
            NetworkComponent();
            NetworkComponent(bool isS, bool isT, unsigned maxSize, unsigned elapsedTime, unsigned maxC = 0);
            ~NetworkComponent();
            void clearData();
            void connect(std::string ip_, uint16_t port_);
            void disconnect();
            void send(Packet p);
            std::vector<Packet> &getReceived();
            std::vector<Packet> &getSent();
            std::vector<unsigned> &getSocketsIndex();
            std::string &getIp();
            unsigned getMaxPacketSize();
            void setMaxPacketSize(unsigned maxSize);
            unsigned getMaxConnections();
            void setMaxConnections(unsigned mC);
            uint16_t getPort();
            bool getIsServer();
            void setIsServer(bool isS);
            bool getIsTcp();
            bool getIsConnected();
            void setIsConnected(bool con);
            bool getDisconnect();
            void setDisconnect(bool dis);
            bool getAttemptConnection();
            void setAttemptConnection(bool attC);
            unsigned getNetSocketSetIndex();
            void setNetSocketSetIndex(unsigned ind);
            void setConnectionFailed(bool con);
            bool getConnectionFailed();
            unsigned getElapsedTimeBetweenChecks();
            void setElapsedTimeBetweenChecks(unsigned time);
    };
}

#endif // NETWORKCOMPONENT_H
