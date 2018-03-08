#include "InputSystem.hpp"

WishEngine::InputSystem *sys = nullptr;

extern "C"{
    EXPORT WishEngine::GameSystem *getSystem(){
        if(sys == nullptr){
            sys = new WishEngine::InputSystem();
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
