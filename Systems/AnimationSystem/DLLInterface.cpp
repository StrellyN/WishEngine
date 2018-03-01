#include "AnimationSystem.hpp"

extern "C"{
    __declspec(dllexport) WishEngine::GameSystem *getSystem(){
        return new WishEngine::AnimationSystem();
    }
}
