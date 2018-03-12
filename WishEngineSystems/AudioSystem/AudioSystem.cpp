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

#include "AudioSystem.hpp"

namespace WishEngine{
    AudioSystem::AudioSystem(){
        setSystemType("AUDIO");
        songTimer = 0;
        listenerId = -1;
        isMusicPlaying = false;
        isMusicPaused = false;
    }

    AudioSystem::~AudioSystem(){
        destroySystem();
        stopMusic();
        currentSong = nullptr;
        components = nullptr;
        objects = nullptr;
    }

    /**
        Plays a song starting from the beginning, this resets the song timer back to 0.
    **/
    void AudioSystem::play(AudioComponent* comp, int volume, int loops, bool fadeIn, int duration){
        if(comp != nullptr && comp->getEnabled()){
            if(comp->getIsDynamic() && objects != nullptr){ //You'll see this code in other places in this system, its for dynamic volume music (changes volume depending on distance)
                GameObject *emitter = nullptr, *listener = nullptr;
                for(unsigned j=0; j<objects->size(); j++){
                    if((*objects)[j].getId() == comp->getEmitterId()){
                        emitter = &(*objects)[j];
                    }
                    if((*objects)[j].getId() == listenerId){
                        listener = &(*objects)[j];
                    }
                }
                std::vector<DimensionComponent> *dimensions = nullptr;
                if(components->find(COMPONENTTYPES::DIMENSION) != components->end()){
                    dimensions = &dynamic_cast<Collection<DimensionComponent>*>(components->at(COMPONENTTYPES::DIMENSION))->getCollection();
                }
                if(emitter != nullptr && emitter->getEnabled()){ //If the emitter of the song is enabled and the song is dynamic
                    if(listener != nullptr && listener->getEnabled()){ //If the listener is enabled too
                        DimensionComponent* listenerDim = &(*dimensions)[listener->getComponentPosition(COMPONENTTYPES::DIMENSION)];
                        DimensionComponent* emitterDim = &(*dimensions)[emitter->getComponentPosition(COMPONENTTYPES::DIMENSION)];
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
                dimensions = nullptr;
                emitter = nullptr;
                listener = nullptr;
            }
            if(comp->getIsSong()){ //If its a song
                songTimer = 0; //Reset the timer
                currentSong->setIsCurrentSong(false);
                if(fadeIn){ //And fadein is active
                    //Send a MusicMessage
                    //Framework::getFramework()->fadeInMusic(comp->getAudioFile(), volume, loops, duration); //Fade in the music
                    postMessage(new AudioMessage(MESSAGETYPES::FADEINMUSIC, comp->getAudioFile(), volume, loops, duration, 0));
                }
                else{
                    //Send a MusicMessage
                    //Framework::getFramework()->playMusic(comp->getAudioFile(), volume, loops); //If not, just play it normally
                    postMessage(new AudioMessage(MESSAGETYPES::PLAYMUSIC, comp->getAudioFile(), volume, loops, 0, 0));
                }
                currentSong = comp; //Set the current song to the new one for volume updates in case of dynamic song
                currentSong->setIsCurrentSong(false);
            }
            else{ //If its not a song
                //Send a MusicMessage
                //Framework::getFramework()->playSound(comp->getAudioFile(), volume, loops); //Just play the sound
                postMessage(new AudioMessage(MESSAGETYPES::PLAYSOUND, comp->getAudioFile(), volume, loops, 0, 0));
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
            if(comp->getIsDynamic() && objects != nullptr){ //You'll see this code in other places in this system, its for dynamic volume music (changes volume depending on distance)
                GameObject *emitter = nullptr, *listener = nullptr;
                for(unsigned j=0; j<objects->size(); j++){
                    if((*objects)[j].getId() == comp->getEmitterId()){
                        emitter = &(*objects)[j];
                    }
                    if((*objects)[j].getId() == listenerId){
                        listener = &(*objects)[j];
                    }
                }
                std::vector<DimensionComponent> *dimensions = nullptr;
                if(components->find(COMPONENTTYPES::DIMENSION) != components->end()){
                    dimensions = &dynamic_cast<Collection<DimensionComponent>*>(components->at(COMPONENTTYPES::DIMENSION))->getCollection();
                }
                if(emitter != nullptr && emitter->getEnabled()){ //If the emitter of the song is enabled and the song is dynamic
                    if(listener != nullptr && listener->getEnabled()){ //If the listener is enabled too
                        DimensionComponent* listenerDim = &(*dimensions)[listener->getComponentPosition(COMPONENTTYPES::DIMENSION)];
                        DimensionComponent* emitterDim = &(*dimensions)[emitter->getComponentPosition(COMPONENTTYPES::DIMENSION)];
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
                dimensions = nullptr;
                emitter = nullptr;
                listener = nullptr;
            }
            if(comp->getIsSong()){ //If its a song
                currentSong->setIsCurrentSong(false);
                if(fadeIn){ //And fadein is active
                    //Send a MusicMessage
                    //Framework::getFramework()->fadeInMusicPos(comp->getAudioFile(), volume, loops, duration, songTimer); //Fade in the music
                    postMessage(new AudioMessage(MESSAGETYPES::FADEINMUSICPOS, comp->getAudioFile(), volume, loops, duration, songTimer));
                }
                else{
                    //Send a MusicMessage
                    //Framework::getFramework()->playMusic(comp->getAudioFile(), volume, loops); //If not, just play it normally
                    postMessage(new AudioMessage(MESSAGETYPES::PLAYMUSIC, comp->getAudioFile(), volume, loops, 0, 0));
                    //Framework::getFramework()->setMusicPos(songTimer);
                    postMessage(new AudioMessage(MESSAGETYPES::SETMUSICPOS, "", 0, 0, 0, songTimer));
                }
                currentSong = comp; //Set the current song to the new one for volume updates in case of dynamic song
                currentSong->setIsCurrentSong(false);
            }
            else{ //If its not a song
                //Send a MusicMessage
                //Framework::getFramework()->playSound(comp->getAudioFile(), volume, loops); //Just play the sound
                postMessage(new AudioMessage(MESSAGETYPES::PLAYSOUND, comp->getAudioFile(), volume, loops, 0, 0));
                comp->setIsPlaying(false);
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
                //Send a MusicMessage
                //Framework::getFramework()->setMusicVolume(volume);
                postMessage(new AudioMessage(MESSAGETYPES::SETMUSICVOLUME, "", volume, 0, 0, 0));
            }
        }
    }

    /**
        Method that fades and halts the current song.
        Also resets the song timer.
    **/
    void AudioSystem::fadeOutMusic(int duration){
        //Send a MusicMessage
        //Framework::getFramework()->fadeOutMusic(duration);
        postMessage(new AudioMessage(MESSAGETYPES::FADEOUTMUSIC, "", 0, 0, duration, 0));
        currentSong->setIsCurrentSong(false);
        currentSong = nullptr;
        songTimer = 0;
    }

    /**
        Method that deletes an audio file from the frame, it being song or sound effect.
    **/
    void AudioSystem::deleteAudio(AudioComponent* comp){
        if(comp->getIsSong()){
            //Send a MusicMessage
            //Framework::getFramework()->deleteMusic(comp->getAudioFile());
            postMessage(new AudioMessage(MESSAGETYPES::DELETEMUSIC, comp->getAudioFile(), 0, 0, 0, 0));
        }
        else{
            //Send a MusicMessage
            //Framework::getFramework()->deleteSound(comp->getAudioFile());
            postMessage(new AudioMessage(MESSAGETYPES::DELETESOUND, comp->getAudioFile(), 0, 0, 0, 0));
        }
    }

    /**
        Method that pauses the music.
    **/
    void AudioSystem::pauseMusic(){
        //Send a MusicMessage
        //Framework::getFramework()->pauseMusic();
        postMessage(new Message(MESSAGETYPES::PAUSEMUSIC));
    }

    /**
        You guessed it, method that resumes the music.
    **/
    void AudioSystem::resumeMusic(){
        //Send a MusicMessage
        //Framework::getFramework()->resumeMusic();
        postMessage(new Message(MESSAGETYPES::RESUMEMUSIC));
    }

    /**
        Method to halt the music and reset the song timer.
    **/
    void AudioSystem::stopMusic(){
        //Send a MusicMessage
        //Framework::getFramework()->stopMusic();
        postMessage(new Message(MESSAGETYPES::STOPMUSIC));
        if(currentSong != nullptr){
            currentSong->setIsCurrentSong(false);
        }
        currentSong = nullptr;
        songTimer = 0;
    }

    /**
        Method that checks if there's music playing right now.
    **/
    bool &AudioSystem::getIsMusicPlaying(){
        return isMusicPlaying;
    }

    /**
        Method that checks if the music is paused right now.
    **/
    bool &AudioSystem::getIsMusicPaused(){
        return isMusicPaused;
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
            if(!done && getIsMusicPlaying() && currentSong->getPaused()){
                pauseMusic();
                done = true;
            }
            if(!done && getIsMusicPaused() && currentSong->getResume()){
                resumeMusic();
                done = true;
            }
        }

        if(components != nullptr && objects != nullptr){
            std::vector<AudioComponent> *audios = nullptr;
            if(components->find(COMPONENTTYPES::AUDIO) != components->end()){
                audios = &dynamic_cast<Collection<AudioComponent>*>(components->at(COMPONENTTYPES::AUDIO))->getCollection();
            }
            if(audios != nullptr){
                for(unsigned i=0; i<audios->size(); i++){
                    if((*audios)[i].getEnabled()){
                        if((*audios)[i].getIsPlaying()){
                            if((*audios)[i].getIsSong()){
                                if(currentSong == nullptr){ //In case there's no current song being played
                                    currentSong = &(*audios)[i];
                                }
                                bool justPlay = true;
                                if((*audios)[i].getCrossFade()){
                                    if((*audios)[i].getFadeIn()){
                                        currentSong->setIsPlaying(false);
                                        crossFadeInto(&(*audios)[i], (*audios)[i].getMaxVolume(), (*audios)[i].getLoops(), true, (*audios)[i].getFadeDuration());
                                    }
                                    else{
                                        currentSong->setIsPlaying(false);
                                        crossFadeInto(&(*audios)[i], (*audios)[i].getMaxVolume(), (*audios)[i].getLoops(), false, (*audios)[i].getFadeDuration());
                                    }
                                    justPlay = false;
                                }
                                if(justPlay){
                                    if((*audios)[i].getFadeIn()){
                                        currentSong->setIsPlaying(false);
                                        play(&(*audios)[i], (*audios)[i].getMaxVolume(), (*audios)[i].getLoops(), true, (*audios)[i].getFadeDuration());
                                    }
                                    else{
                                        currentSong->setIsPlaying(false);
                                        play(&(*audios)[i], (*audios)[i].getMaxVolume(), (*audios)[i].getLoops(), false, (*audios)[i].getFadeDuration());
                                    }
                                }
                            }
                            else{
                                play(&(*audios)[i], (*audios)[i].getMaxVolume(), (*audios)[i].getLoops(), (*audios)[i].getFadeIn(), (*audios)[i].getFadeDuration());
                            }
                        }
                    }
                }
            }
            audios = nullptr;

            if(!getIsMusicPaused() && getIsMusicPlaying() && currentSong != nullptr){
                listenerId = currentSong->listener;
                GameObject *listener = nullptr, *emitter = nullptr;
                for(unsigned j=0; j<objects->size(); j++){
                    if((*objects)[j].getId() == listenerId){
                        listener = &(*objects)[j];
                    }
                    if((*objects)[j].getId() == currentSong->getEmitterId()){
                        emitter = &(*objects)[j];
                    }
                }

                songTimer += 1*dt; //Multiplying delta time by 1 makes it so the song timer advances a second each real second that passes, making it reliable.
                std::vector<DimensionComponent> *dimensions = nullptr;
                if(components->find(COMPONENTTYPES::DIMENSION) != components->end()){
                    dimensions = &dynamic_cast<Collection<DimensionComponent>*>(components->at(COMPONENTTYPES::DIMENSION))->getCollection();
                }
                if(currentSong->getIsDynamic() && listener != nullptr && dimensions != nullptr){ //The usefulness of saving the current song! Calculate the volume depending on the distance of the emitter
                    int newVolume = 0;           //and the listener!
                    DimensionComponent* listenerDim = &(*dimensions)[listener->getComponentPosition(COMPONENTTYPES::DIMENSION)];
                    DimensionComponent* emitterDim = &(*dimensions)[emitter->getComponentPosition(COMPONENTTYPES::DIMENSION)];
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
                        //Send MusicMessage
                        //Framework::getFramework()->setMusicVolume(newVolume);
                        postMessage(new AudioMessage(MESSAGETYPES::SETMUSICVOLUME, "", newVolume, 0, 0, 0));
                    }
                    else{
                        //Send MusicMessage
                        //Framework::getFramework()->setMusicVolume(currentSong->getMaxVolume());
                        postMessage(new AudioMessage(MESSAGETYPES::SETMUSICVOLUME, "", currentSong->getMaxVolume(), 0, 0, 0));
                    }
                }
                else{
                    //Send MusicMessage
                    //Framework::getFramework()->setMusicVolume(currentSong->getMaxVolume());
                    postMessage(new AudioMessage(MESSAGETYPES::SETMUSICVOLUME, "", currentSong->getMaxVolume(), 0, 0, 0));
                }
                dimensions = nullptr;
                listener = nullptr;
                emitter = nullptr;
            }
        }
        if(!getIsMusicPlaying()){
            songTimer = 0;
        }
    }

    /**
        Method to handle received messages present in every system.
    **/
    void AudioSystem::handleMessage(Message* msg){
        if(msg->getType() == MESSAGETYPES::OBJECTLIST){
            ObjectListMessage* rmes = dynamic_cast<ObjectListMessage*>(msg);
            if(rmes != nullptr){
                objects = rmes->getObjectList();
            }
            rmes = nullptr;
        }
        else if(msg->getType() == MESSAGETYPES::COMPONENTLIST){
            ComponentListMessage* rmes = dynamic_cast<ComponentListMessage*>(msg);
            if(rmes != nullptr){
                components = rmes->getComponentList();
            }
            rmes = nullptr;
        }
        else if(msg->getType() == MESSAGETYPES::DELETEEVERYTHING){
            destroySystem();
        }
    }

    /**
        Method to set the listener as a new object.
    **/
    void AudioSystem::setListenerId(unsigned listen){
        listenerId = listen;
    }

    /**
        Method to get the pointer to the current listener.
    **/
    unsigned AudioSystem::getListenerId(){
        return listenerId;
    }
}
