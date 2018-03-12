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

#include "AudioMessage.hpp"

namespace WishEngine{
    AudioMessage::AudioMessage(MESSAGETYPES ty, std::string file, int vol, int loop, int duration, double timer){
        setType(ty);
        audioFile = file;
        volume = vol;
        loops = loop;
        fadeDuration = duration;
        songTimer = timer;
    }

    AudioMessage::~AudioMessage(){

    }

    std::string &AudioMessage::getAudioFile(){
        return audioFile;
    }

    int AudioMessage::getVolume(){
        return volume;
    }

    int AudioMessage::getLoops(){
        return loops;
    }

    int AudioMessage::getFadeDuration(){
        return fadeDuration;
    }

    double AudioMessage::getSongTimer(){
        return songTimer;
    }
}
