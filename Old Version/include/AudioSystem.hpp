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

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "System.hpp"
#include "GameObject.hpp"
#include "AudioComponent.hpp"

namespace WishEngine{
    class AudioSystem : public GameSystem{
        private:
            GameObject* listener = nullptr; //Change this to strings so I don't have the dependency
            AudioComponent* currentSong = nullptr; //To be able to modify the volume dynamically each frame if its 3d sound
            double songTimer;
        public:
            AudioSystem();
            ~AudioSystem();
            void update(double dt);
            void handleMessage(Message* msg);
            void play(AudioComponent* comp, int volume, int loops, bool fadeIn, int duration);
            void crossFadeInto(AudioComponent* comp, int volume, int loops, bool fadeIn, int duration);
            void setVolume(AudioComponent* comp, int volume);
            void stopMusic();
            void fadeOutMusic(int duration);
            void deleteAudio(AudioComponent* comp);
            void pauseMusic();
            void resumeMusic();
            bool isMusicPlaying();
            bool isMusicPaused();
            void setListener(GameObject* listen);
            GameObject* getListener();
    };
}
#endif // AUDIOSYSTEM
