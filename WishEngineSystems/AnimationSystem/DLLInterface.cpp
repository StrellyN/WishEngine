#include "AnimationSystem.hpp"

#ifdef _WIN32
	#define EXPORT __declspec(dllexport)
#elif defined(__unix__) || defined(__linux__)
	#define EXPORT
#endif

extern "C"{
    EXPORT WishEngine::GameSystem *getSystem(){
        return new WishEngine::AnimationSystem();
    }
}
