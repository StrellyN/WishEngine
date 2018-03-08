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
    AudioSystem::AudioSystem(){
        setSystemType(S_TYPES::AUDIO);
        songTimer = 0;
    }

    AudioSystem::~AudioSystem(){
        destroySystem();
        stopMusic();
        listener = nullptr;
        currentSong = nullptr;
    }

    /**
        Plays a song starting from the beginning, this resets the song timer back to 0.
    **/
    void AudioSystem::play(AudioComponent* comp, int volume, int loops, bool fadeIn, int duration){
        if(comp != nullptr && comp->getEnabled()){
            if(comp->getIsDynamic()){ //You'll see this code in other places in this system, its for dynamic volume music (changes volume depending on distance)
                if(comp->getEmitter() != nullptr && comp->getEmitter()->getEnabled()){ //If the emitter of the song is enabled and the song is dynamic
                    if(listener != nullptr && listener->getEnabled()){ //If the listener is enabled too
                        DimentionComponent* listenerDim = dynamic_cast<DimentionComponent*>(listener->getComponent(C_TYPES::DIMENTION));
                        DimentionComponent* emitterDim = dynamic_cast<DimentionComponent*>(comp->getEmitter()->getComponent(C_TYPES::DIMENTION));
                        DirectionVector listener(listenerDim->getX(), listenerDim->getY());
                        DirectionVector emitter(emitterDim->getX(), emitterDim->getY());
                        DirectionVector offset(comp->getOffset().getX(), comp->getOffset().getY());
                        double distance = Utils::getDistanceForAudio(listener, emitter, offset); //Gather needed things and calculate distance
                        if(fabs(distance) <= comp->getMinRadius()){ //Check for the radius of the song and set the volume accordingly
                            volume = comp->getMaxVolume();
                        }
                        else if(fabs(distance) >= comp->getMaxRadius()){
                            volume = 0;
                        }
                        else{ //If it's inside the min and max radius, calculate the correct volume using a simple interpolation between max volume and 0
                            volume = Utils::getVolumeFromDistance(comp->getMinRadius(), comp->getMaxRadius(), comp->getMaxVolume(), distance);
                        }
                        listenerDim = nullptr; //Set gathered things to null
                        emitterDim = nullptr;
                    }
                }
            }
            if(comp->getIsSong()){ //If its a song
                songTimer = 0; //Reset the timer
                currentSong->setIsCurrentSong(false);
                if(fadeIn){ //And fadein is active
                    Framework::getFramework()->fadeInMusic(comp->getAudioFile(), volume, loops, duration); //Fade in the music
                }
                else{
                    Framework::getFramework()->playMusic(comp->getAudioFile(), volume, loops); //If not, just play it normally
                }
                currentSong = comp; //Set the current song to the new one for volume updates in case of dynamic song
                currentSong->setIsCurrentSong(false);
            }
            else{ //If its not a song
                Framework::getFramework()->playSound(comp->getAudioFile(), volume, loops); //Just play the sound
                comp->setIsPlaying(false);
            }
        }
    }

    /**
        Method to simulate crossfading, I'm proud of this one.
        It uses the song timer that's running on the system and when you want to change to
        another song, it uses that timer to set the new song to that time so it looks like
        it was just a transition and not just starting this new song.
        It works the same as the play, but just using the position functions instead of the normal
        ones to set the position when playing the music.
        Right now it doesn't have a fadeout for the current song, that could be done easily
        by the user or here just doing a for loop to set the volume until its around half of
        the original volume and then transition into the new song making the fadein a bit
        faster.
    **/
    void AudioSystem::crossFadeInto(AudioComponent* comp, int volume, int loops, bool fadeIn, int duration){
        if(comp != nullptr && comp->getEnabled()){
            if(comp->getIsDynamic()){
                if(comp->getEmitter() != nullptr && comp->getEmitter()->getEnabled()){
                    if(listener != nullptr && listener->getEnabled()){
                        DimentionComponent* listenerDim = dynamic_cast<DimentionComponent*>(listener->getComponent(C_TYPES::DIMENTION));
                        DimentionComponent* emitterDim = dynamic_cast<DimentionComponent*>(comp->getEmitter()->getComponent(C_TYPES::DIMENTION));
                        DirectionVector listener(listenerDim->getX(), listenerDim->getY());
                        DirectionVector emitter(emitterDim->getX(), emitterDim->getY());
                        DirectionVector offset(comp->getOffset().getX(), comp->getOffset().getY());
                        double distance = Utils::getDistanceForAudio(listener, emitter, offset);
                        if(fabs(distance) <= comp->getMinRadius()){
                            volume = comp->getMaxVolume();
                        }
                        else if(fabs(distance) >= comp->getMaxRadius()){
                            volume = 0;
                        }
                        else{
                            volume = Utils::getVolumeFromDistance(comp->getMinRadius(), comp->getMaxRadius(), comp->getMaxVolume(), distance);
                        }
                        listenerDim = nullptr;
                        emitterDim = nullptr;
                    }
                }
            }
            if(comp->getIsSong()){
                currentSong->setIsCurrentSong(false);
                if(fadeIn){
                    Framework::getFramework()->fadeInMusicPos(comp->getAudioFile(), volume, loops, duration, songTimer);
                }
                else{
                    Framework::getFramework()->playMusic(comp->getAudioFile(), volume, loops);
                    Framework::getFramework()->setMusicPos(songTimer);
                }
                currentSong = comp;
                currentSong->setIsCurrentSong(true);
            }
            else{
                Framework::getFramework()->playSound(comp->getAudioFile(), volume, loops);
            }
        }
    }

    /**
        Method that sets the volume of the song you pass.
    **/
    void AudioSystem::setVolume(AudioComponent* comp, int volume){
        if(comp->getIsSong()){ //Only changes the volume if its a song.
            if(comp->getIsDynamic()){ //If its a dynamic song, set the volume as the max volume of the song.
                comp->setMaxVolume(volume);
            }
            else{ //If not, set the real volume
                Framework::getFramework()->setMusicVolume(volume);
            }
        }
    }

    /**
        Method that fades and halts the current song.
        Also resets the song timer.
    **/
    void AudioSystem::fadeOutMusic(int duration){
        Framework::getFramework()->fadeOutMusic(duration);
        currentSong->setIsCurrentSong(false);
        currentSong = nullptr;
        songTimer = 0;
    }

    /**
        Method that deletes an audio file from the frame, it being song or sound effect.
    **/
    void AudioSystem::deleteAudio(AudioComponent* comp){
        if(comp->getIsSong()){
            Framework::getFramework()->deleteMusic(comp->getAudioFile());
        }
        else{
            Framework::getFramework()->deleteSound(comp->getAudioFile());
        }
    }

    /**
        Method that pauses the music.
    **/
    void AudioSystem::pauseMusic(){
        Framework::getFramework()->pauseMusic();
    }

    /**
        You guessed it, method that resumes the music.
    **/
    void AudioSystem::resumeMusic(){
        Framework::getFramework()->resumeMusic();
    }

    /**
        Method to halt the music and reset the song timer.
    **/
    void AudioSystem::stopMusic(){
        Framework::getFramework()->stopMusic();
        if(currentSong != nullptr){
            currentSong->setIsCurrentSong(false);
        }
        currentSong = nullptr;
        songTimer = 0;
    }

    /**
        Method that checks if there's music playing right now.
    **/
    bool AudioSystem::isMusicPlaying(){
        return Framework::getFramework()->isMusicPlaying();
    }

    /**
        Method that checks if the music is paused right now.
    **/
    bool AudioSystem::isMusicPaused(){
        return Framework::getFramework()->isMusicPaused();
    }

    /**
        Method to update the logic present in every system.
        In this case it updates the song timer if the music is playing and it's not paused,
        resets it if nothing is playing and updates the volume in case of the song being
        dynamic.
        It also now controls everything depending on the settings of the audio components of the game.
    **/
    void AudioSystem::update(double dt){
        if(currentSong != nullptr){
            bool done = false;
            if(!done && currentSong->getStop()){
                currentSong->setIsPlaying(false);
                stopMusic();
                done = true;
            }
            if(!done && currentSong->getFadeOut()){
                currentSong->setIsPlaying(false);
                fadeOutMusic(currentSong->getFadeDuration());
                done = true;
            }
            if(!done && isMusicPlaying() && currentSong->getPaused()){
                pauseMusic();
                done = true;
            }
            if(!done && isMusicPaused() && currentSong->getResume()){
                resumeMusic();
                done = true;
            }
        }
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects();
        for(unsigned i=0; i<objs.size(); i++){
            if(objs[i].getEnabled()){
                for(unsigned j=0; j<objs[i].getComponents().size(); j++){
                    if(objs[i].getComponents()[j]->getType() == C_TYPES::AUDIOC){
                        AudioComponent *objAudio = dynamic_cast<AudioComponent*>(objs[i].getComponents()[j]);
                        if(objAudio != nullptr && objAudio->getEnabled()){
                            if(objAudio->getIsPlaying()){
                                if(objAudio->getIsSong()){
                                    if(currentSong == nullptr){ //In case there's no current song being played
                                        currentSong = objAudio;
                                    }
                                    bool justPlay = true;
                                    if(objAudio->getCrossFade()){
                                        if(objAudio->getFadeIn()){
                                            currentSong->setIsPlaying(false);
                                            crossFadeInto(objAudio, objAudio->getMaxVolume(), objAudio->getLoops(), true, objAudio->getFadeDuration());
                                        }
                                        else{
                                            currentSong->setIsPlaying(false);
                                            crossFadeInto(objAudio, objAudio->getMaxVolume(), objAudio->getLoops(), false, objAudio->getFadeDuration());
                                        }
                                        justPlay = false;
                                    }
                                    if(justPlay){
                                        if(objAudio->getFadeIn()){
                                            currentSong->setIsPlaying(false);
                                            play(objAudio, objAudio->getMaxVolume(), objAudio->getLoops(), true, objAudio->getFadeDuration());
                                        }
                                        else{
                                            currentSong->setIsPlaying(false);
                                            play(objAudio, objAudio->getMaxVolume(), objAudio->getLoops(), false, objAudio->getFadeDuration());
                                        }
                                    }
                                }
                                else{
                                    play(objAudio, objAudio->getMaxVolume(), objAudio->getLoops(), objAudio->getFadeIn(), objAudio->getFadeDuration());
                                }
                            }
                        }
                        objAudio = nullptr;
                    }
                }
            }
        }
        if(!isMusicPaused() && isMusicPlaying() && currentSong != nullptr){
            listener = currentSong->listener;
            songTimer += 1*dt; //Multiplying delta time by 1 makes it so the song timer advances a second each real second that passes, making it reliable.
            if(currentSong->getIsDynamic() && listener != nullptr){ //The usefulness of saving the current song! Calculate the volume depending on the distance of the emitter
                int newVolume = 0;           //and the listener!
                DimentionComponent* listenerDim = dynamic_cast<DimentionComponent*>(listener->getComponent(C_TYPES::DIMENTION));
                DimentionComponent* emitterDim = dynamic_cast<DimentionComponent*>(currentSong->getEmitter()->getComponent(C_TYPES::DIMENTION));
                if(listenerDim != nullptr && emitterDim != nullptr){
                    DirectionVector listener(listenerDim->getX(), listenerDim->getY());
                    DirectionVector emitter(emitterDim->getX(), emitterDim->getY());
                    DirectionVector offset(currentSong->getOffset().getX(), currentSong->getOffset().getY());
                    double distance = Utils::getDistanceForAudio(listener, emitter, offset);
                    if(fabs(distance) <= currentSong->getMinRadius()){
                        newVolume = currentSong->getMaxVolume();
                    }
                    else if(fabs(distance) >= currentSong->getMaxRadius()){
                        newVolume = 0;
                    }
                    else{
                        newVolume = Utils::getVolumeFromDistance(currentSong->getMinRadius(), currentSong->getMaxRadius(), currentSong->getMaxVolume(), distance);
                    }
                    listenerDim = nullptr;
                    emitterDim = nullptr;
                    Framework::getFramework()->setMusicVolume(newVolume);
                }
                else{
                    Framework::getFramework()->setMusicVolume(currentSong->getMaxVolume());
                }
            }
            else{
                Framework::getFramework()->setMusicVolume(currentSong->getMaxVolume());
            }
        }
        if(!isMusicPlaying()){
            songTimer = 0;
        }
    }

    /**
        Method to handle received messages present in every system.
    **/
    void AudioSystem::handleMessage(Message* msg){

    }

    /**
        Method to set the listener as a new object.
    **/
    void AudioSystem::setListener(GameObject* listen){
        listener = listen;
    }

    /**
        Method to get the pointer to the current listener.
    **/
    GameObject* AudioSystem::getListener(){
        return listener;
    }
}
