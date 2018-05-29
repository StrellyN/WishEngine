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

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "FrameHeader.hpp"
#include "../CommonIncludes/Event.hpp"
#include "../CommonIncludes/Collection.hpp"
#include "../CommonIncludes/System.hpp"
#include "../CommonIncludes/GameObject.hpp"

#include "../CommonIncludes/InputListMessage.hpp"
#include "../CommonIncludes/ObjectListMessage.hpp"
#include "../CommonIncludes/ComponentListMessage.hpp"
#include "../CommonIncludes/CreateWindowMessage.hpp"
#include "../CommonIncludes/AudioMessage.hpp"
#include "../CommonIncludes/NetworkMessage.hpp"
#include "../CommonIncludes/VectorMessage.hpp"

#include "../CommonIncludes/NetworkComponent.hpp"
#include "../CommonIncludes/GraphicComponent.hpp"
#include "../CommonIncludes/DimensionComponent.hpp"
#include "../CommonIncludes/AnimationComponent.hpp"
#include "../CommonIncludes/CameraComponent.hpp"

namespace WishEngine{
    typedef struct JoystickStruct{
        SDL_Joystick *pad;
        int id, joystickDeadzone = 8000;
    };

    class Framework : public GameSystem{
        private:
            //General
            SDL_Event fEvent;
            bool frameCapFlag;
            int maxFPS, frameTicks;
            std::vector<GameObject> *objects = nullptr;
            std::unordered_map<int, BaseCollection*> *components = nullptr;

            //Inputs
            std::vector<Event> frameEvents;

            //Joysticks
            std::vector<JoystickStruct> joysticks;

            //Window
            std::unordered_map<std::string, SDL_Window*> windowPool;
            std::unordered_map<std::string, SDL_Renderer*> renderPool;

            //Textures
            std::unordered_map<std::string, SDL_Texture*> texturePool;

            //Audio
            std::unordered_map<std::string, Mix_Music*> musicPool;
            std::unordered_map<std::string, Mix_Chunk*> chunkPool;

            //Text
            std::unordered_map<std::string, TTF_Font*> fontPool;

            //Networking
            std::vector<TCPsocket> tcpSockets;
            std::vector<UDPsocket> udpSockets;
            std::vector<SDLNet_SocketSet> socketSets;
            std::vector<IPaddress> ipAdresses, udpIpAdresses;
        public:
            Framework();
            virtual ~Framework();
            void init();
            void update(double dt);
            void handleMessage(Message *msg);
            void quit();

            double getTicks();
            void getEvents();
            void clearData();
            void fullScreen(std::string windowName);
            void setMaxFPS(int mFPS);
            void setFrameCapFlag(bool frameCap);
            bool getFrameCapFlag();
            int getMaxFPS();
            int getJoystickDeadzone(int joyId);
            void setJoystickDeadzone(int joyId, int joyDeadzone);

            void deleteNet(NetworkComponent *netComp); //Search a way to avoid having network components here
            void updateNet(NetworkComponent *netComp);
            void connectNet(NetworkComponent *netComp);

            void deleteWindow(std::string name);
            void createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h);
            void getWindowSize(std::string windowN, int &w, int &h);
            void getWindowPosition(std::string windowN, int &x, int &y);
            std::string getWindowTitle(std::string windowN);
            unsigned getWindowCount();
            std::string getWindowName(unsigned pos);

            void playSound(std::string soundFile, int volume, int loops);
            void playMusic(std::string musicFile, int volume, int loops);
            void setMusicPos(double position);
            void stopMusic();
            void fadeInMusic(std::string musicFile, int volume, int loops, int duration);
            void fadeInMusicPos(std::string musicFile, int volume, int loops, int duration, double position);
            void fadeOutMusic(int duration);
            void setMusicVolume(int volume);
            void deleteMusic(std::string musicFile);
            void deleteSound(std::string soundFile);
            bool isMusicPlaying();
            bool isMusicPaused();
            void pauseMusic();
            void resumeMusic();

            void startFrame();
            void endFrame();
            void renderObjects(double interpolation);
            void render(GraphicComponent *graphComp, DimensionComponent *dimComp, AnimationComponent *animComp, double interpolation, DimensionComponent *camDim, CameraComponent *camComp, std::string window = "");
            void renderText(GraphicComponent *graphComp, DimensionComponent *dimComp, AnimationComponent *animComp, double interpolation, DimensionComponent *camDim, CameraComponent *camComp, std::string window = "");
            void renderPlainText(GraphicComponent *graphComp, DimensionComponent *dimComp, AnimationComponent *animComp, double interpolation, DimensionComponent *camDim, CameraComponent *camComp, std::string window = "");
            void getSizeOfText(std::string text, std::string font, int &w, int &h);
    };
}
#endif // FRAMEWORK
