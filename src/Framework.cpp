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
    Framework* Framework::frame = nullptr;

    Framework::Framework(){
        init();
    }

    Framework::~Framework(){
        quit();
    }

    void Framework::destroyFrameWork(){
        delete frame;
        frame = nullptr;
    }

    Framework* Framework::getFramework(){
        if(frame == nullptr){
            frame = new Framework();
        }
        return frame;
    }

    void Framework::fullScreen(std::string windowName){
        if(windowPool.count(windowName) != 0){
            if((SDL_GetWindowFlags(windowPool[windowName]) & SDL_WINDOW_FULLSCREEN) != 0){
                SDL_SetWindowFullscreen(windowPool[windowName], 0);
            }
            else {
                SDL_SetWindowFullscreen(windowPool[windowName], SDL_WINDOW_FULLSCREEN);
            }
        }
    }

    void Framework::startFrame(){
        for(std::map<std::string, SDL_Renderer*>::iterator i=renderPool.begin(); i!=renderPool.end(); i++){
            SDL_RenderClear((*i).second);
        }
        if(frameCapFlag){
            frameTicks = SDL_GetTicks();
        }
    }

    void Framework::setMaxFPS(int mFPS){
        if(mFPS < 0){
            mFPS = 0;
        }
        maxFPS = mFPS;
    }

    void Framework::setFrameCapFlag(bool frameCap){
        frameCapFlag = frameCap;
    }

    bool Framework::getFrameCapFlag(){
        return frameCapFlag;
    }

    int Framework::getMaxFPS(){
        return maxFPS;
    }

    void Framework::playMusic(std::string musicFile, int volume, int loops){
        if(volume < 0) volume = 0;
        if(volume > 128) volume = 128;
        if(loops < -1) loops = -1;
        if(musicPool.count(musicFile) == 0){
            musicPool[musicFile] = Mix_LoadMUS(musicFile.c_str());
        }
        if(Mix_PlayingMusic()){
            Mix_HaltMusic();
        }
        Mix_VolumeMusic(volume);
        Mix_PlayMusic(musicPool[musicFile], loops);
    }

    void Framework::setMusicPos(double position){
        if(isMusicPlaying()){
            Mix_SetMusicPosition(position);
        }
    }

    void Framework::playSound(std::string soundFile, int volume, int loops){
        if(volume < 0) volume = 0;
        if(volume > 128) volume = 128;
        if(loops < -1) loops = -1;
        if(chunkPool.count(soundFile) == 0){
            chunkPool[soundFile] = Mix_LoadWAV(soundFile.c_str());
        }
        Mix_VolumeChunk(chunkPool[soundFile], volume);
        Mix_PlayChannel(-1, chunkPool[soundFile], loops);
    }

    void Framework::stopMusic(){
        if(isMusicPlaying()){
            Mix_HaltMusic();
        }
    }

    void Framework::fadeInMusic(std::string musicFile, int volume, int loops, int duration){
        if(volume < 0) volume = 0;
        if(volume > 128) volume = 128;
        if(loops < -1) loops = -1;
        if(duration < 0) duration = 0;
        if(musicPool.count(musicFile) == 0){
            musicPool[musicFile] = Mix_LoadMUS(musicFile.c_str());
        }
        if(isMusicPlaying()){
            Mix_HaltMusic();
        }
        Mix_VolumeMusic(volume);
        Mix_FadeInMusic(musicPool[musicFile], loops, duration);
    }

    void Framework::fadeInMusicPos(std::string musicFile, int volume, int loops, int duration, double position){
        if(volume < 0) volume = 0;
        if(volume > 128) volume = 128;
        if(loops < -1) loops = -1;
        if(duration < 0) duration = 0;
        if(position < 0) position = 0;
        if(musicPool.count(musicFile) == 0){
            musicPool[musicFile] = Mix_LoadMUS(musicFile.c_str());
        }
        if(isMusicPlaying()){
            Mix_HaltMusic();
        }
        Mix_VolumeMusic(volume);
        Mix_FadeInMusicPos(musicPool[musicFile], loops, duration, position);
    }

    void Framework::fadeOutMusic(int duration){
        if(duration < 0) duration = 0;
        if(isMusicPlaying()){
            Mix_FadeOutMusic(duration);
        }
    }

    void Framework::setMusicVolume(int volume){
        if(volume < 0) volume = 0;
        if(volume > 128) volume = 128;
        if(isMusicPlaying()){
            Mix_VolumeMusic(volume);
        }
    }

    void Framework::deleteMusic(std::string musicFile){
        if(musicPool.count(musicFile) != 0){
            Mix_FreeMusic(musicPool[musicFile]);
            musicPool.erase(musicFile);
        }
    }

    void Framework::deleteSound(std::string soundFile){
        if(chunkPool.count(soundFile) != 0){
            Mix_FreeChunk(chunkPool[soundFile]);
            chunkPool.erase(soundFile);
        }
    }

    bool Framework::isMusicPlaying(){
        if(Mix_PlayingMusic()){
            return true;
        }
        return false;
    }

    bool Framework::isMusicPaused(){
        if(Mix_PausedMusic()){
            return true;
        }
        return false;
    }

    void Framework::pauseMusic(){
        Mix_PauseMusic();
    }

    void Framework::resumeMusic(){
        Mix_ResumeMusic();
    }


    void Framework::render(GameObject &obj, double interpolation, Camera *cam, std::string window){
        GraphicComponent* graphComp = dynamic_cast<GraphicComponent*>(obj.getComponent(C_TYPES::GRAPHIC));
        DimentionComponent* dimComp = dynamic_cast<DimentionComponent*>(obj.getComponent(C_TYPES::DIMENTION));
        AnimationComponent* animComp = dynamic_cast<AnimationComponent*>(obj.getComponent(C_TYPES::ANIMATION));
        if(obj.getEnabled() && graphComp != nullptr && graphComp->getEnabled() && dimComp != nullptr && dimComp->getEnabled()){ //Object and components are enabled
            SDL_Rect rect;
            Rectangle camRect; //change any camDim->get() to the interpolation equivalent
            if(cam != nullptr && cam->getEnabled() && cam->hasComponent(C_TYPES::DIMENTION) && !obj.hasComponent(C_TYPES::UI)){
                DimentionComponent* camDim = dynamic_cast<DimentionComponent*>(cam->getComponent(C_TYPES::DIMENTION));
                rect = {(nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation)))-(nearbyint(camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))),
                                 (nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation)))-(nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))),
                                  dimComp->getW()*dimComp->getScale(),dimComp->getH()*dimComp->getScale()};
                camRect = Rectangle(((camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))), (nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))), camDim->getW(), camDim->getH());
                camDim = nullptr;
            }
            else{
                rect = {(nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation))),
                                 (nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation))),
                                  dimComp->getW()*dimComp->getScale(),dimComp->getH()*dimComp->getScale()};
                camRect = Rectangle(0, 0, 0, 0);
            }
            Rectangle objRect(dimComp->getX(), dimComp->getY(), dimComp->getW(), dimComp->getH());
            if(Utils::checkCollision(objRect, camRect) || obj.hasComponent(C_TYPES::UI)){ //Culling!
                SDL_Rect *cut = nullptr;
                if(animComp != nullptr && animComp->getEnabled()){ //Animation doesn't use scale
                    cut = new SDL_Rect();
                    *cut = {animComp->getX(), animComp->getY(), animComp->getW(), animComp->getH()};
                }
                std::string tFile = graphComp->getTextureFile();
                //Apply the camera component here (same for text rendering)
                if(cam != nullptr && cam->getEnabled()){
                    CameraComponent *camComp = dynamic_cast<CameraComponent*>(cam->getComponent(C_TYPES::CAMERA));
                    if(camComp != nullptr && camComp->getEnabled()){
                        rect.x += camComp->getX();
                        rect.y += camComp->getY();
                    }
                }
                if(graphComp->getIsTexture()){
                    if(renderPool.count(window)){
                        if(!texturePool.count(tFile)){
                            texturePool[tFile] = IMG_LoadTexture(renderPool[window], tFile.c_str());
                            SDL_SetTextureBlendMode(texturePool[tFile], SDL_BLENDMODE_BLEND);
                        }
                        SDL_SetTextureAlphaMod(texturePool[tFile], graphComp->getAlpha());
                        SDL_RenderCopy(renderPool[window], texturePool[tFile], cut, &rect);
                    }
                    else{
                        if(!texturePool.count(tFile)){
                            texturePool[tFile] = IMG_LoadTexture(renderPool.begin()->second, tFile.c_str());
                            SDL_SetTextureBlendMode(texturePool[tFile], SDL_BLENDMODE_BLEND);
                        }
                        SDL_SetTextureAlphaMod(texturePool[tFile], graphComp->getAlpha());
                        SDL_RenderCopy(renderPool.begin()->second, texturePool[tFile], cut, &rect);
                    }
                }
                else{

                    if(window == "" || renderPool.count(window) == 0){ //If windows wasn't passed or it doesn't exist, draw to the first one
                        SDL_SetRenderDrawColor(renderPool.begin()->second, graphComp->getR(), graphComp->getG(), graphComp->getB(), graphComp->getAlpha());
                        SDL_RenderFillRect(renderPool.begin()->second, &rect);
                        SDL_SetRenderDrawColor(renderPool.begin()->second, 0, 0, 0, 255);
                    }
                    else{
                        SDL_SetRenderDrawColor(renderPool[window], graphComp->getR(), graphComp->getG(), graphComp->getB(), graphComp->getAlpha());
                        SDL_RenderFillRect(renderPool[window], &rect);
                        SDL_SetRenderDrawColor(renderPool[window], 0, 0, 0, 255);
                    }
                }
                delete cut;
                cut = nullptr;
            }

            dimComp->setX((nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation))));
            dimComp->setX(dimComp->getpX());
            dimComp->setY((nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation))));
            dimComp->setY(dimComp->getpY());
        }
        graphComp = nullptr;
        dimComp = nullptr;
        animComp = nullptr;
        cam = nullptr;
    }

    void Framework::renderText(GameObject &obj, double interpolation, Camera *cam, std::string window){
        GraphicComponent* graphComp = dynamic_cast<GraphicComponent*>(obj.getComponent(C_TYPES::GRAPHIC));
        DimentionComponent* dimComp = dynamic_cast<DimentionComponent*>(obj.getComponent(C_TYPES::DIMENTION));
        if(obj.getEnabled() && graphComp != nullptr && graphComp->getEnabled() && dimComp != nullptr && dimComp->getEnabled()){
            SDL_Rect textBox; //The text box
            Rectangle camRect;
            if(cam != nullptr && cam->getEnabled() && cam->hasComponent(C_TYPES::DIMENTION) && !obj.hasComponent(C_TYPES::UI)){
                DimentionComponent* camDim = dynamic_cast<DimentionComponent*>(cam->getComponent(C_TYPES::DIMENTION));
                textBox = {(nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation)))-(nearbyint(camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))),
                                 (nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation)))-(nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))),
                                  dimComp->getW()*dimComp->getScale(),dimComp->getH()*dimComp->getScale()};
                camRect = Rectangle((nearbyint(camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))), (nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))), camDim->getW(), camDim->getH());
                camDim = nullptr;
            }
            else{
                textBox = {(nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation))),
                                 (nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation))),
                                  dimComp->getW()*dimComp->getScale(),dimComp->getH()*dimComp->getScale()};
                camRect = Rectangle(0, 0, 0, 0);
            }
            Rectangle textRect(dimComp->getX(), dimComp->getY(), dimComp->getW(), dimComp->getH());
            if(Utils::checkCollision(textRect, camRect) || obj.hasComponent(C_TYPES::UI)){ //Culling!
                if(cam != nullptr && cam->getEnabled()){
                    CameraComponent *camComp = dynamic_cast<CameraComponent*>(cam->getComponent(C_TYPES::CAMERA));
                    if(camComp != nullptr && camComp->getEnabled()){
                        textBox.x += camComp->getX();
                        textBox.y += camComp->getY();
                    }
                }
                Rectangle letterRect(textRect.getX(), textRect.getY(), 0, 0);
                SDL_Rect letter = {textBox.x, textBox.y, 0, 0}; //rect for rendering each letter
                TextComponent &text = graphComp->getText(); //We get the text, font and open the fonts in the pool
                std::string fontName = text.getFont(), textString = text.getText();
                Color color; //Letter colors
                int lineNumber = 0, startingLine = text.getStartingLine(), maxLines = text.getMaxLines();
                std::string character, textureName, fontPoolName;
                for(int i=0; i<textString.size(); i++){
                    if(lineNumber > startingLine+maxLines){
                        break;
                    }
                    character += {textString[i]};
                    if(character[0] == '\n'){
                        lineNumber++;
                        letter.x = textBox.x;
                        letter.y = textBox.y + ((lineNumber-startingLine)*text.getLineSpacing());
                        letterRect.setX(textRect.getX());
                        letterRect.setY(textRect.getY() + ((lineNumber-startingLine)*text.getLineSpacing()));
                        textureName.clear();
                        fontPoolName.clear();
                        character.clear();
                        continue;
                    }
                    textureName += character;
                    textureName += fontName;
                    textureName += text.getIndividualCharacterSizeString()[i];
                    fontPoolName += fontName;
                    fontPoolName += text.getIndividualCharacterSizeString()[i];
                    if(fontPool.count(fontPoolName) == 0){ //Check if font exists
                        fontPool[fontPoolName] = TTF_OpenFont(fontName.c_str(), text.getIndividualCharacterSize()[i]);
                    }
                    if(fontPool[fontPoolName] == NULL){
                        break;
                    }
                    if(texturePool.count(textureName) == 0){
                        SDL_Surface* textSurface = TTF_RenderText_Blended(fontPool[fontPoolName], character.c_str(), {255, 255, 255, 255});
                        if(renderPool.count(window) != 0 && window != ""){
                            texturePool[textureName] = SDL_CreateTextureFromSurface(renderPool[window], textSurface);
                        }
                        else{
                            texturePool[textureName] = SDL_CreateTextureFromSurface(renderPool.begin()->second, textSurface);
                        }
                        SDL_FreeSurface(textSurface);
                    }
                    SDL_QueryTexture(texturePool[textureName], NULL, NULL, &letter.w, &letter.h);
                    letterRect.setW(letter.w);
                    letterRect.setH(letter.h);
                    if(lineNumber >= startingLine){
                        if(Utils::checkCollision(letterRect, camRect) || obj.hasComponent(C_TYPES::UI)){ //Letters Culling!
                            color = text.getIndividualCharacterColor()[i];
                            SDL_SetTextureColorMod(texturePool[textureName], color.getR(), color.getG(), color.getB());
                            SDL_SetTextureAlphaMod(texturePool[textureName], color.getA());
                            if(renderPool.count(window) != 0 && window != ""){
                                SDL_RenderCopy(renderPool[window], texturePool[textureName], NULL, &letter);
                            }
                            else{
                                SDL_RenderCopy(renderPool.begin()->second, texturePool[textureName], NULL, &letter);
                            }
                        }
                    }
                    letter.x += letter.w;
                    letterRect.setX(letterRect.getX() + letterRect.getW());
                    if(letter.x + letter.w > textBox.x + textBox.w){
                        letter.x = textBox.x;
                        letterRect.setX(textRect.getX());
                        lineNumber++;
                    }
                    letter.y = textBox.y + ((lineNumber-startingLine)*text.getLineSpacing());
                    letterRect.setY(textRect.getY() + ((lineNumber-startingLine)*text.getLineSpacing()));
                    textureName.clear();
                    fontPoolName.clear();
                    character.clear();
                }
            }

            dimComp->setX((nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation))));
            dimComp->setX(dimComp->getpX());
            dimComp->setY((nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation))));
            dimComp->setY(dimComp->getpY());
        }
        graphComp = nullptr;
        dimComp = nullptr;
        cam = nullptr;
    }

    void Framework::renderPlainText(GameObject &obj, double interpolation, Camera *cam, std::string window){
        GraphicComponent* graphComp = dynamic_cast<GraphicComponent*>(obj.getComponent(C_TYPES::GRAPHIC));
        DimentionComponent* dimComp = dynamic_cast<DimentionComponent*>(obj.getComponent(C_TYPES::DIMENTION));
        if(obj.getEnabled() && graphComp != nullptr && graphComp->getEnabled() && dimComp != nullptr && dimComp->getEnabled()){
            SDL_Rect textBox; //The text box
            Rectangle camRect;
            if(cam != nullptr && cam->getEnabled() && cam->hasComponent(C_TYPES::DIMENTION) && !obj.hasComponent(C_TYPES::UI)){
                DimentionComponent* camDim = dynamic_cast<DimentionComponent*>(cam->getComponent(C_TYPES::DIMENTION));
                textBox = {(nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation)))-(nearbyint(camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))),
                                 (nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation)))-(nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))),
                                  dimComp->getW()*dimComp->getScale(),dimComp->getH()*dimComp->getScale()};
                camRect = Rectangle((nearbyint(camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))), (nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))), camDim->getW(), camDim->getH());
                camDim = nullptr;
            }
            else{
                textBox = {(nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation))),
                                 (nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation))),
                                  dimComp->getW()*dimComp->getScale(),dimComp->getH()*dimComp->getScale()};
                camRect = Rectangle(0, 0, 0, 0);
            }
            Rectangle letterRect(dimComp->getX(), dimComp->getY(), 0, 0);
            SDL_Rect letter = {textBox.x, textBox.y, 0, 0}; //rect for rendering each letter
            TextComponent &text = graphComp->getText(); //We get the text, font and open the fonts in the pool
            std::string fontName = text.getFont(), textString = text.getText();
            Color color; //Letter colors
            std::string textureName, fontPoolName;
            textureName += textString;
            textureName += fontName;
            textureName += text.getIndividualCharacterSizeString()[0];
            fontPoolName += fontName;
            fontPoolName += text.getIndividualCharacterSizeString()[0];
            if(fontPool.count(fontPoolName) == 0){ //Check if font exists
                fontPool[fontPoolName] = TTF_OpenFont(fontName.c_str(), text.getIndividualCharacterSize()[0]);
            }
            if(fontPool[fontPoolName] == NULL){
                graphComp = nullptr;
                dimComp = nullptr;
                cam = nullptr;
                return;
            }
            if(texturePool.count(textureName) == 0){
                SDL_Surface* textSurface = TTF_RenderText_Blended(fontPool[fontPoolName], textString.c_str(), {255, 255, 255, 255});
                if(renderPool.count(window) != 0 && window != ""){
                    texturePool[textureName] = SDL_CreateTextureFromSurface(renderPool[window], textSurface);
                }
                else{
                    texturePool[textureName] = SDL_CreateTextureFromSurface(renderPool.begin()->second, textSurface);
                }
                SDL_FreeSurface(textSurface);
            }
            SDL_QueryTexture(texturePool[textureName], NULL, NULL, &letter.w, &letter.h);
            letterRect.setW(letter.w);
            letterRect.setH(letter.h);
            if(Utils::checkCollision(letterRect, camRect) || obj.hasComponent(C_TYPES::UI)){ //Letters Culling!
                if(cam != nullptr && cam->getEnabled()){
                    CameraComponent *camComp = dynamic_cast<CameraComponent*>(cam->getComponent(C_TYPES::CAMERA));
                    if(camComp != nullptr && camComp->getEnabled()){
                        letter.x += camComp->getX();
                        letter.y += camComp->getY();
                    }
                }
                color = text.getIndividualCharacterColor()[0];
                SDL_SetTextureColorMod(texturePool[textureName], color.getR(), color.getG(), color.getB());
                SDL_SetTextureAlphaMod(texturePool[textureName], color.getA());
                if(renderPool.count(window) != 0 && window != ""){
                    SDL_RenderCopy(renderPool[window], texturePool[textureName], NULL, &letter);
                }
                else{
                    SDL_RenderCopy(renderPool.begin()->second, texturePool[textureName], NULL, &letter);
                }
            }
            textureName.clear();
            fontPoolName.clear();

            dimComp->setX((nearbyint(dimComp->getX()*(interpolation) + dimComp->getpX()*(1-interpolation))));
            dimComp->setX(dimComp->getpX());
            dimComp->setY((nearbyint(dimComp->getY()*(interpolation) + dimComp->getpY()*(1-interpolation))));
            dimComp->setY(dimComp->getpY());
        }
        graphComp = nullptr;
        dimComp = nullptr;
        cam = nullptr;
    }

    void Framework::getSizeOfText(std::string text, std::string font, int &w, int &h){
        if(fontPool.count(font) == 0){
            w = -1;
            h = -1;
        }
        else{
            TTF_SizeText(fontPool[font], text.c_str(), &w, &h);
        }
    }

    void Framework::endFrame(){
        for(std::map<std::string, SDL_Renderer*>::iterator i=renderPool.begin(); i!=renderPool.end(); i++){
            SDL_RenderPresent((*i).second);
        }
        if(frameCapFlag){
            int realFT = SDL_GetTicks() - frameTicks;
            if(realFT < 1000/maxFPS){
                SDL_Delay(1000/maxFPS - realFT);
            }
            frameTicks = SDL_GetTicks();
        }
        else{
            SDL_Delay(1);
        }
    }

    double Framework::getTicks(){
        return SDL_GetTicks();
    }

    std::vector<Event> Framework::getEvents(){
        std::vector<Event> result;
        while(SDL_PollEvent(&fEvent)){
            Event ev;
            E_TYPES type = E_TYPES::ENULL;
            int mX = 0, mY = 0;
            switch(fEvent.type){
                case SDL_KEYDOWN:
                    type = E_TYPES::KEYBOARD_PRESS;
                    ev.setTimeStamp(fEvent.key.timestamp);
                    ev.setValue(SDL_GetKeyName(fEvent.key.keysym.sym));
                    ev.setWindowID(fEvent.key.windowID);
                    break;
                case SDL_KEYUP:
                    type = E_TYPES::KEYBOARD_RELEASE;
                    ev.setTimeStamp(fEvent.key.timestamp);
                    ev.setValue(SDL_GetKeyName(fEvent.key.keysym.sym));
                    ev.setWindowID(fEvent.key.windowID);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mX, &mY);
                    type = E_TYPES::MOUSE_PRESS;
                    ev.setDeviceID(fEvent.button.which);
                    switch(fEvent.button.button){
                        case SDL_BUTTON_LEFT:
                            ev.setValue("Left Button");
                            ev.setXPos(mX);
                            ev.setYPos(mY);
                            break;
                        case SDL_BUTTON_RIGHT:
                            ev.setValue("Right Button");
                            ev.setXPos(mX);
                            ev.setYPos(mY);
                            break;
                        case SDL_BUTTON_MIDDLE:
                            ev.setValue("Middle Button");
                            ev.setXPos(mX);
                            ev.setYPos(mY);
                            break;
                    }
                    ev.setTimeStamp(fEvent.button.timestamp);
                    ev.setWindowID(fEvent.button.windowID);
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&mX, &mY);
                    type = E_TYPES::MOUSE_RELEASE;
                    ev.setDeviceID(fEvent.button.which);
                    switch(fEvent.button.button){
                        case SDL_BUTTON_LEFT:
                            ev.setValue("Left Button");
                            ev.setXPos(mX);
                            ev.setYPos(mY);
                            break;
                        case SDL_BUTTON_RIGHT:
                            ev.setValue("Right Button");
                            ev.setXPos(mX);
                            ev.setYPos(mY);
                            break;
                        case SDL_BUTTON_MIDDLE:
                            ev.setValue("Middle Button");
                            ev.setXPos(mX);
                            ev.setYPos(mY);
                            break;
                    }
                    ev.setTimeStamp(fEvent.button.timestamp);
                    ev.setWindowID(fEvent.button.windowID);
                    break;
                case SDL_JOYDEVICEADDED:
                    type = E_TYPES::GAMEPAD_ADDED;
                    ev.setTimeStamp(fEvent.jdevice.timestamp);
                    ev.setDeviceID(fEvent.jdevice.which);
                    break;
                case SDL_JOYDEVICEREMOVED:
                    type = E_TYPES::GAMEPAD_REMOVED;
                    ev.setTimeStamp(fEvent.jdevice.timestamp);
                    ev.setDeviceID(fEvent.jdevice.which);
                    break;
                case SDL_JOYBUTTONDOWN:
                    type = E_TYPES::GAMEPAD_PRESS;
                    ev.setValue(Utils::intToString(fEvent.jbutton.button));
                    ev.setTimeStamp(fEvent.jdevice.timestamp);
                    ev.setDeviceID(fEvent.jdevice.which);
                    break;
                case SDL_JOYBUTTONUP:
                    type = E_TYPES::GAMEPAD_RELEASE;
                    ev.setValue(Utils::intToString(fEvent.jbutton.button));
                    ev.setTimeStamp(fEvent.jdevice.timestamp);
                    ev.setDeviceID(fEvent.jdevice.which);
                    break;
                case SDL_JOYAXISMOTION:
                    type = E_TYPES::GAMEPAD_AXIS; //which axis, direction and value
                    ev.setTimeStamp(fEvent.jaxis.timestamp);
                    if(!fEvent.jaxis.axis){ //If axis == 0 it's the X axis
                        ev.setXPos(fEvent.jaxis.value);
                    }
                    else{ //If == 1 it's the Y axis
                        ev.setYPos(fEvent.jaxis.value);
                    }
                    ev.setDeviceID(fEvent.jaxis.which);
                    break;
                case SDL_MOUSEWHEEL:
                    type = E_TYPES::MOUSE_WHEEL;
                    ev.setTimeStamp(fEvent.wheel.timestamp);
                    ev.setXPos(fEvent.wheel.x);
                    ev.setYPos(fEvent.wheel.y);
                    ev.setDeviceID(fEvent.wheel.which);
                    ev.setWindowID(fEvent.wheel.windowID);
                    break;
                case SDL_MOUSEMOTION:
                    type = E_TYPES::MOUSE_MOTION;
                    ev.setXPos(fEvent.motion.x);
                    ev.setYPos(fEvent.motion.y);
                    ev.setXRel(fEvent.motion.xrel);
                    ev.setYRel(fEvent.motion.yrel);
                    ev.setTimeStamp(fEvent.motion.timestamp);
                    ev.setDeviceID(fEvent.motion.which);
                    ev.setWindowID(fEvent.motion.windowID);
                    break;
                case SDL_QUIT:
                    type = E_TYPES::EQUIT;
                    break;
                case SDL_WINDOWEVENT:
                    switch(fEvent.window.event){ //Expand for all the window events and maybe include the ids for closing single windows etc...
                        case SDL_WINDOWEVENT_CLOSE:
                            type = E_TYPES::EQUIT;
                            break;
                    }
            }
            ev.setType(type);
            if(ev.getType() != E_TYPES::ENULL){
                result.push_back(ev);
            }
        }
        return result;
    }

    void Framework::deleteWindow(std::string name){
        if(name != "mainWindow" && name != "" && windowPool.count(name)){ //You can't delete the main window
            SDL_DestroyRenderer(renderPool[name]);
            SDL_DestroyWindow(windowPool[name]);
            windowPool.erase(name);
            renderPool.erase(name);
        }
    }

    void Framework::createWindow(std::string title, std::string name, std::string icon, int x, int y, int w, int h){
        int i=1;
        std::string originalName = name;
        while(name == "" || windowPool.count(name) != 0){
            if(name == ""){
                name = "Window";
                originalName = name;
                continue;
            }
            name = originalName;
            name += Utils::intToString(i);
            i++;
        }
        if(windowPool.count(name) == 0 && name != ""){
            SDL_Window* window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN);
            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
            SDL_Surface* iconSurf = IMG_Load(icon.c_str());
            SDL_SetWindowIcon(window, iconSurf);
            SDL_FreeSurface(iconSurf);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            windowPool[name] = {window};
            renderPool[name] = {renderer};
            window = nullptr;
            renderer = nullptr;
        }
    }

    void Framework::getWindowSize(std::string windowN, int &w, int &h){
        if(windowPool.count(windowN) != 0){
            SDL_GetWindowSize(windowPool[windowN], &w, &h);
        }
        else{
            w = 0;
            h = 0;
        }
    }

    void Framework::getWindowPosition(std::string windowN, int &x, int &y){
        if(windowPool.count(windowN) != 0){
            SDL_GetWindowPosition(windowPool[windowN], &x, &y);
        }
        else{
            x = 0;
            y = 0;
        }
    }

    std::string Framework::getWindowTitle(std::string windowN){
        if(windowPool.count(windowN) != 0){
            return SDL_GetWindowTitle(windowPool[windowN]);
        }
        else{
            return "";
        }
    }

    unsigned Framework::getWindowCount(){
        return windowPool.size();
    }

    std::string Framework::getWindowName(unsigned pos){
        std::map<std::string, SDL_Window*>::iterator it = windowPool.begin();
        for(unsigned i=0; i<pos; i++){
            it++;
        }
        return it->first;
    }

    void Framework::init(){
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(IMG_INIT_PNG);
        Mix_Init(MIX_INIT_OGG);
        TTF_Init();
        SDLNet_Init();
        Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
        std::fstream frameworkConfig;
        std::string windowTitle, icon;
        int winX, winY, winW, winH;
        bool isFullScreen;
        frameworkConfig.open("data/FRAMEWORK.config", std::ios::in); //we open the config file
        if(frameworkConfig){ //Check if we could access the file
            std::string command;
            frameworkConfig >> command;
            if(command == "BEGIN"){
                frameworkConfig >> command;
                while(command != "END"){
                    windowTitle += command;
                    frameworkConfig >> command;
                    if(command != "END"){
                        windowTitle += " ";
                    }
                }
            }
            frameworkConfig >> icon;
            frameworkConfig >> winX;
            frameworkConfig >> winY;
            frameworkConfig >> winW;
            frameworkConfig >> winH;
            frameworkConfig >> frameCapFlag;
            frameworkConfig >> maxFPS;
            frameworkConfig >> isFullScreen;
        }
        else{
            windowTitle = "Couldn't access data/FRAMEWORK.config file.";
            icon = "";
            winX = 100;
            winY = 100;
            winW = 500;
            winH = 100;
        }
        frameworkConfig.close(); //close the file
        SDL_Window* window;
        if(isFullScreen){
            window = SDL_CreateWindow(windowTitle.c_str(), winX, winY, winW, winH, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
        }
        else{
            window = SDL_CreateWindow(windowTitle.c_str(), winX, winY, winW, winH, SDL_WINDOW_SHOWN);
        }
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_Surface* iconSurf = IMG_Load(icon.c_str());
        SDL_SetWindowIcon(window, iconSurf);
        SDL_FreeSurface(iconSurf);
        windowPool["mainWindow"] = {window};
        renderPool["mainWindow"] = {renderer};
        window = nullptr;
        renderer = nullptr;
        fEvent = SDL_Event();
    }

    void Framework::clearData(){
        stopMusic();
        for(std::map<std::string, SDL_Texture*>::iterator i=texturePool.begin(); i!=texturePool.end(); i++){
            SDL_DestroyTexture(i->second);
        }
        texturePool.clear();
        for(std::map<std::string, Mix_Music*>::iterator i=musicPool.begin(); i!=musicPool.end(); i++){
            Mix_FreeMusic(i->second);
        }
        musicPool.clear();
        for(std::map<std::string, Mix_Chunk*>::iterator i=chunkPool.begin(); i!=chunkPool.end(); i++){
            Mix_FreeChunk(i->second);
        }
        chunkPool.clear();
        for(std::map<std::string, TTF_Font*>::iterator i=fontPool.begin(); i!=fontPool.end(); i++){
            TTF_CloseFont(i->second);
        }
        fontPool.clear();
    }

    void Framework::quit(){
        for(std::map<std::string, SDL_Renderer*>::iterator i=renderPool.begin(); i!=renderPool.end(); i++){
            SDL_DestroyRenderer(i->second);
        }
        renderPool.clear();
        for(std::map<std::string, SDL_Window*>::iterator i=windowPool.begin(); i!=windowPool.end(); i++){
            SDL_DestroyWindow(i->second);
        }
        windowPool.clear();
        clearData();
        Mix_CloseAudio();
        SDLNet_Quit();
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
    }
}
