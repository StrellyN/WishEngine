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

#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "Component.hpp"
#include "GameObject.hpp"

namespace WishEngine{
    class AudioComponent : public Component{
        private:
            std::string audioFile;
            bool isSong;

            bool isPlaying, fadingIn, fadingOut, stoped, crossFading, paused, resumed, isCurrentSong = false;
            int loops, fadeDuration;

            bool isDynamic;
            GameObject* emitter = nullptr; //Change this to string
            int maxVolume;
            double minRadius, maxRadius; //Min for when to start lowering volume, max for where the volume becomes 0
            DirectionVector offset = DirectionVector(0,0,0); //For dynamic volume depending on distance
        public:
            static GameObject *listener;
            AudioComponent(std::string audioFile, bool isS, bool isP, int lps);
            ~AudioComponent();
            void setDynamic(GameObject* emit, int maxV, double minRad, double maxRad, double x, double y, double z = 0);
            std::string &getAudioFile();
            void setAudioFile(std::string aFile);
            bool getIsSong();
            void setIsSong(bool isS);
            DirectionVector& getOffset();
            void setOffset(double x, double y, double z = 0);
            bool getIsDynamic();
            void setIsDynamic(bool isD);
            GameObject* getEmitter();
            void setEmitter(GameObject* emit);
            int getMaxVolume();
            void setMaxVolume(int mV);
            double getMinRadius();
            void setMinRadius(double minR);
            double getMaxRadius();
            void setMaxRadius(double maxR);
            bool getIsPlaying();
            void setIsPlaying(bool iP);
            int getLoops();
            bool getFadeIn();
            bool getFadeOut();
            bool getStop();
            bool getCrossFade();
            int getFadeDuration();
            bool getPaused();
            bool getResume();
            void play(int lps, bool fIn, unsigned dur, unsigned vol);
            void crossFade(int lps, bool fIn, unsigned dur, unsigned vol);
            void pause();
            void resume();
            void stop();
            void fadeOut(unsigned dur);
            bool getIsCurrentSong();
            void setIsCurrentSong(bool isCS);
    };
}
#endif // AUDIOCOMPONENT
