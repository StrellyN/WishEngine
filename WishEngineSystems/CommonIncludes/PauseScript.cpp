#ifndef PAUSESCRIPT_H
#define PAUSESCRIPT_H

/**
	You can add attributes both in private(recommended) and public,
	but remember to create getters and setters if you do it in private.
**/

#include "GameScript.cpp"

using namespace WishEngine;

class PauseScript : public ScriptComponent{
    private:
        bool alreadyPaused;

    public:
		/**
			Initialization code here. This method will execute when the
			script is created in an object.
		**/
        PauseScript(){
            alreadyPaused = false;
        }

		/**
			Destruction code here, remember to delete your pointers if this
			component has the ownership, example, a component created in
			this script. If you got a system, an object or a component via
			the game state, you probably don't want to delete it.
			This method will execute when the object that this is attached to
			is deleted, or if the component is deleted manualy.
		**/
        ~PauseScript(){
        }

		/**
			Execution code here. This method will execute every logic update.
		**/
        void execute(GameObject &thisObj, double deltaT){
            //Redo the script with functions accesible from the gamescript instead of
            //having an instance of the script here.
            /**if(gameController == nullptr){
                gameController = ObjectFactory::getObjectFactory()->getObject("GameController");
            }

            if(gameController != nullptr && controller == nullptr){
                controller = dynamic_cast<GameScript*>(gameController->getComponent(C_TYPES::SCRIPT, "GameScript"));
            }

            if(gameController != nullptr && controller != nullptr){
                if(controller->getPause() && !alreadyPaused){
                    for(unsigned i=0; i<thisObj.getComponents().size(); i++){
                        if(thisObj.getComponents()[i]->getType() == C_TYPES::ANIMATOR || (thisObj.getComponents()[i]->getType() == C_TYPES::SCRIPT && (thisObj.getComponents()[i]->getName() != "PauseScript" && thisObj.getComponents()[i]->getName() != "PlayerScript"))){
                            thisObj.getComponents()[i]->setEnabled(false);
                        }
                    }
                    alreadyPaused = true;
                }
                else if(!controller->getPause() && alreadyPaused){
                    for(unsigned i=0; i<thisObj.getComponents().size(); i++){
                        if(thisObj.getComponents()[i]->getType() == C_TYPES::ANIMATOR || (thisObj.getComponents()[i]->getType() == C_TYPES::SCRIPT && (thisObj.getComponents()[i]->getName() != "PauseScript" && thisObj.getComponents()[i]->getName() != "PlayerScript"))){
                            thisObj.getComponents()[i]->setEnabled(true);
                        }
                    }
                    alreadyPaused = false;
                }
            }**/
        }
};

#endif // PAUSESCRIPT_H
