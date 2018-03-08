#include "AnimationSystem.hpp"

WishEngine::AnimationSystem *sys = nullptr;

extern "C"{
    EXPORT WishEngine::GameSystem *getSystem(){
        if(sys == nullptr){
            sys = new WishEngine::AnimationSystem();
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
