#include "PhysicsSystem.hpp"

extern "C"{
    __declspec(dllexport) WishEngine::GameSystem *getSystem(){
        return new WishEngine::PhysicsSystem();
    }
}
