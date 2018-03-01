#include "ObjectFactory.hpp"

extern "C"{
    __declspec(dllexport) WishEngine::GameSystem *getSystem(){
        return new WishEngine::ObjectFactory();
    }
}
