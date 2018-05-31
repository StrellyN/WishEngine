#include "ScriptsInterface.hpp"

WishEngine::ScriptsInterface *sys = nullptr;

extern "C"{
    EXPORT WishEngine::GameSystem *getSystem(){
        if(sys == nullptr){
            sys = new WishEngine::ScriptsInterface();
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
