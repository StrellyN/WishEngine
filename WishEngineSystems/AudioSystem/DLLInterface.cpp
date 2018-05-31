#include "AudioSystem.hpp"

WishEngine::AudioSystem *sys = nullptr;

extern "C"{
    EXPORT WishEngine::GameSystem *getSystem(){
        if(sys == nullptr){
            sys = new WishEngine::AudioSystem();
        }
        return sys;
    }

    EXPORT void destroySystem(){
        if(sys != nullptr){
            delete sys;
            sys = nullptr;
        }
    }
}
