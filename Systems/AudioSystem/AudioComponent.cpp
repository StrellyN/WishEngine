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

#include "AudioComponent.hpp"

namespace WishEngine{
    unsigned AudioComponent::listener = -1;

    AudioComponent::AudioComponent(std::string audioF, bool isS, bool isP, int lps){
        setType("AUDIO");
        if(lps < -1){
            lps = -1;
        }
        loops = lps;
        isPlaying = isP;
        setName(audioF);
        audioFile = audioF;
        isSong = isS;
        isDynamic = false;
        maxVolume = 128;
        emitterId = -1;

        fadingIn = false;
        fadingOut = false;
        stoped = false;
        crossFading = false;
        fadeDuration = 0;
        paused = false;
        resumed = false;
    }

    AudioComponent::~AudioComponent(){

    }

    void AudioComponent::setDynamic(unsigned emitter, int maxV, double minRad, double maxRad, double x, double y, double z){
        isDynamic = true;
        emitterId = emitter;
        maxVolume = maxV;
        minRadius = minRad;
        maxRadius = maxRad;
        offset.setX(x);
        offset.setY(y);
        offset.setZ(z);
    }

    std::string &AudioComponent::getAudioFile(){
        return audioFile;
    }

    void AudioComponent::setAudioFile(std::string aFile){
        audioFile = aFile;
    }

    bool AudioComponent::getIsSong(){
        return isSong;
    }

    void AudioComponent::setIsSong(bool isS){
        isSong = isS;
    }

    DirectionVector& AudioComponent::getOffset(){
        return offset;
    }

    void AudioComponent::setOffset(double x, double y, double z){
        offset.setX(x);
        offset.setY(y);
        offset.setZ(z);
    }

    bool AudioComponent::getIsDynamic(){
        return isDynamic;
    }

    void AudioComponent::setIsDynamic(bool isD){
        isDynamic = isD;
    }

    unsigned AudioComponent::getEmitterId(){
        return emitterId;
    }

    void AudioComponent::setEmitterId(unsigned emit){
        emitterId = emit;
    }

    int AudioComponent::getMaxVolume(){
        return maxVolume;
    }

    void AudioComponent::setMaxVolume(int mV){
        maxVolume = mV;
    }

    double AudioComponent::getMinRadius(){
        return minRadius;
    }

    void AudioComponent::setMinRadius(double minR){
        minRadius = minR;
    }

    double AudioComponent::getMaxRadius(){
        return maxRadius;
    }

    void AudioComponent::setMaxRadius(double maxR){
        maxRadius = maxR;
    }

    bool AudioComponent::getIsPlaying(){
        return isPlaying;
    }

    void AudioComponent::setIsPlaying(bool iP){
        isPlaying = iP;
    }

    int AudioComponent::getLoops(){
        return loops;
    }

    bool AudioComponent::getFadeIn(){
        return fadingIn;
    }

    bool AudioComponent::getFadeOut(){
        return fadingOut;
    }

    bool AudioComponent::getStop(){
        return stoped;
    }

    bool AudioComponent::getCrossFade(){
        return crossFading;
    }

    int AudioComponent::getFadeDuration(){
        return fadeDuration;
    }

    bool AudioComponent::getPaused(){
        return paused;
    }

    bool AudioComponent::getResume(){
        return resumed;
    }

    void AudioComponent::play(int lps, bool fIn, unsigned dur, unsigned vol){
        isPlaying = true;
        loops = lps;
        fadingIn = fIn;
        fadeDuration = dur;
        maxVolume = vol;
        stoped = false;
        paused = false;
        resumed = false;
        crossFading = false;
        fadingOut = false;
    }

    void AudioComponent::crossFade(int lps, bool fIn, unsigned dur, unsigned vol){
        isPlaying = true;
        loops = lps;
        fadingIn = fIn;
        fadeDuration = dur;
        maxVolume = vol;
        stoped = false;
        paused = false;
        resumed = false;
        crossFading = true;
        fadingOut = false;
    }

    void AudioComponent::pause(){
        fadingIn = false;
        stoped = false;
        paused = true;
        resumed = false;
        crossFading = false;
        fadingOut = false;
    }

    void AudioComponent::resume(){
        fadingIn = false;
        stoped = false;
        paused = false;
        resumed = true;
        crossFading = false;
        fadingOut = false;
    }

    void AudioComponent::stop(){
        fadingIn = false;
        stoped = true;
        paused = false;
        resumed = false;
        crossFading = false;
        fadingOut = false;
    }

    void AudioComponent::fadeOut(unsigned dur){
        fadingIn = false;
        stoped = false;
        paused = false;
        resumed = false;
        crossFading = false;
        fadingOut = true;
        fadeDuration = dur;
    }

    bool AudioComponent::getIsCurrentSong(){
        return isCurrentSong;
    }

    void AudioComponent::setIsCurrentSong(bool isCS){
        isCurrentSong = isCS;
    }
}
