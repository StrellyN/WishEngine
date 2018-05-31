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

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "../CommonIncludes/System.hpp"
#include "../CommonIncludes/AudioComponent.hpp"
#include "../CommonIncludes/DimensionComponent.hpp"
#include "../CommonIncludes/Collection.hpp"
#include "../CommonIncludes/GameObject.hpp"
#include "../CommonIncludes/ComponentListMessage.hpp"
#include "../CommonIncludes/ObjectListMessage.hpp"
#include "../CommonIncludes/AudioMessage.hpp"

namespace WishEngine{
    class AudioSystem : public GameSystem{
        private:
            unsigned listenerId;
            std::vector<AudioComponent> *audios = nullptr;
            int currentSong;
            double songTimer;
            bool isMusicPlaying, isMusicPaused;
            std::unordered_map<int, BaseCollection*> *components = nullptr;
            std::vector<GameObject> *objects = nullptr;

        public:
            AudioSystem();
            ~AudioSystem();
            void update(double dt);
            void handleMessage(Message* msg);
            void play(AudioComponent* comp, int songPos, int volume, int loops, bool fadeIn, int duration);
            void crossFadeInto(AudioComponent* comp, int songPos, int volume, int loops, bool fadeIn, int duration);
            void setVolume(AudioComponent* comp, int volume);
            void stopMusic();
            void fadeOutMusic(int duration);
            void deleteAudio(AudioComponent* comp);
            void pauseMusic();
            void resumeMusic();
            bool &getIsMusicPlaying();
            bool &getIsMusicPaused();
            void setListenerId(unsigned listen);
            unsigned getListenerId();
    };
}
#endif // AUDIOSYSTEM
