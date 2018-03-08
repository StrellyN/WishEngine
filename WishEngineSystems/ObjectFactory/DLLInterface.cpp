#include "ObjectFactory.hpp"

WishEngine::ObjectFactory *sys = nullptr;

extern "C"{
    EXPORT WishEngine::GameSystem *getSystem(){
        if(sys == nullptr){
            sys = new WishEngine::ObjectFactory();
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
