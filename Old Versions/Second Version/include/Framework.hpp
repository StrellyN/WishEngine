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

#define JOYSTICK_DEADZONE 8000

#include "FrameHeader.hpp"
#include "Event.hpp"
#include "Collection.hpp"
#include "System.hpp"
#include "GameObject.hpp"

#include "RenderMessage.hpp"
#include "InputListMessage.hpp"
#include "ObjectListMessage.hpp"
#include "ComponentListMessage.hpp"
#include "CreateWindowMessage.hpp"
#include "AudioMessage.hpp"
#include "NetworkMessage.hpp"

#include "NetworkComponent.hpp"
#include "GraphicComponent.hpp"
#include "DimensionComponent.hpp"
#include "AnimationComponent.hpp"
#include "CameraComponent.hpp"

namespace WishEngine{
    typedef struct JoystickStruct{
        SDL_Joystick *pad;
        int id;
    };

    class Framework : public GameSystem{
        private:
            //General
            SDL_Event fEvent;
            bool frameCapFlag;
            int maxFPS, frameTicks;
            std::vector<GameObject> *objects = nullptr;
            std::map<std::string, BaseCollection*> *components = nullptr;

            //Inputs
            std::vector<Event> frameEvents;

            //Joysticks
            std::vector<JoystickStruct> joysticks;

            //Window
            std::map<std::string, SDL_Window*> windowPool;
            std::map<std::string, SDL_Renderer*> renderPool;

            //Textures
            std::map<std::string, SDL_Texture*> texturePool;

            //Audio
            std::map<std::string, Mix_Music*> musicPool;
            std::map<std::string, Mix_Chunk*> chunkPool;

            //Text
            std::map<std::string, TTF_Font*> fontPool;

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
