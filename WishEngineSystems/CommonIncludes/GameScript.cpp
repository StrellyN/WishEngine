#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

/**
	You can add attributes both in private(recommended) and public,
	but remember to create getters and setters if you do it in private.
**/

#include "PlayerScript.cpp"

using namespace WishEngine;

class GameScript : public ScriptComponent{
    private:
        PlayerScript *player = nullptr;
        DialogueManagerScript *dialogueManager = nullptr;
        bool dialogue, cutscene, optionsLoaded = false;
        bool jumpHolding = false, dashHolding = false, actionHolding = false, fullHold = false;
        std::vector<std::string> controlsAssigned;
        /**
            0 - UP
            1 - LEFT
            2 - RIGHT
            3 - DOWN
            4 - JUMP
            5 - DASH
            6 - ACTION
            7 - QUIT
            8 - FULLSCREEN
        **/

    public:
		/**
			Initialization code here. This method will execute when the
			script is created in an object.
		**/
        GameScript(){
            cutscene = false;
            dialogue = false;
        }

		/**
			Destruction code here, remember to delete your pointers if this
			component has the ownership, example, a component created in
			this script. If you got a system, an object or a component via
			the game state, you probably don't want to delete it.
			This method will execute when the object that this is attached to
			is deleted, or if the component is deleted manualy.
		**/
        ~GameScript(){
            controlsAssigned.clear();
            player = nullptr;
            dialogueManager = nullptr;
        }

		/**
			Execution code here. This method will execute every logic update.
		**/
        void execute(GameObject &thisObj, double deltaT){
            if(player == nullptr){
                player = ScriptsInterface::getComponent<PlayerScript>(ScriptsInterface::getObject("Player"), "SCRIPT", "PlayerScript");
            }
            if(dialogueManager == nullptr){
                dialogueManager = ScriptsInterface::getComponent<DialogueManagerScript>(ScriptsInterface::getObject("DialogueBox"), "SCRIPT", "DialogueManagerScript");
            }

            //Load or create the config file with FPS and inputs
            if(!optionsLoaded){
                if(Utils::fileAccessible("data/Options.config")){ //Load
                    int FPSread = Utils::readDoubleFromFile("data/Options.config", "FPS");
                    ScriptsInterface::setMaxFPS(FPSread);
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "UP"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "LEFT"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "RIGHT"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "DOWN"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "JUMP"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "DASH"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "ACTION"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "QUIT"));
                    controlsAssigned.push_back(Utils::readStringFromFile("data/Options.config", "FULLSCREEN"));
                }
                else{ //Create
                    controlsAssigned.push_back("W");
                    controlsAssigned.push_back("A");
                    controlsAssigned.push_back("D");
                    controlsAssigned.push_back("S");
                    controlsAssigned.push_back("J");
                    controlsAssigned.push_back("K");
                    controlsAssigned.push_back("L");
                    controlsAssigned.push_back("Escape");
                    controlsAssigned.push_back("F4");
                    Utils::writeDoubleToFile("data/Options.config", "FPS", 60/**ObjectFactory::getObjectFactory()->getMaxFPS()**/, false);
                    Utils::writeStringToFile("data/Options.config", "UP", controlsAssigned[0], true);
                    Utils::writeStringToFile("data/Options.config", "LEFT", controlsAssigned[1], true);
                    Utils::writeStringToFile("data/Options.config", "RIGHT", controlsAssigned[2], true);
                    Utils::writeStringToFile("data/Options.config", "DOWN", controlsAssigned[3], true);
                    Utils::writeStringToFile("data/Options.config", "JUMP", controlsAssigned[4], true);
                    Utils::writeStringToFile("data/Options.config", "DASH", controlsAssigned[5], true);
                    Utils::writeStringToFile("data/Options.config", "ACTION", controlsAssigned[6], true);
                    Utils::writeStringToFile("data/Options.config", "QUIT", controlsAssigned[7], true);
                    Utils::writeStringToFile("data/Options.config", "FULLSCREEN", controlsAssigned[8], true);
                }
                optionsLoaded = true;
            }
            //Config file end

            handleInput(thisObj);
        }

        void handleInput(GameObject &thisObj){
            InputComponent *inputs = ScriptsInterface::getComponent<InputComponent>(&thisObj, "INPUT");
            if(inputs != nullptr){
                for(unsigned i=0; i<inputs->getInputs().size(); i++){
                    bool press = false, release = false;
                    if(inputs->getInputs()[i].getType() == "KEYBOARD_PRESS" ||
                       inputs->getInputs()[i].getType() == "GAMEPAD_PRESS" ||
                       inputs->getInputs()[i].getType() == "MOUSE_PRESS" ||
                       inputs->getInputs()[i].getType() == "GAMEPAD_AXIS"){
                        press = true;
                    }
                    if(inputs->getInputs()[i].getType() == "KEYBOARD_RELEASE" ||
                       inputs->getInputs()[i].getType() == "GAMEPAD_RELEASE" ||
                       inputs->getInputs()[i].getType() == "MOUSE_RELEASE"){
                        release = true;
                    }
                    if(!dialogue && !cutscene){
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[0]){

                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[0]){

                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[1]){
                            if(player != nullptr){
                                player->setLeft(true);
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[1]){
                            if(player != nullptr){
                                player->setLeft(false);
                            }
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[2]){
                            if(player != nullptr){
                                player->setRight(true);
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[2]){
                            if(player != nullptr){
                                player->setRight(false);
                            }
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[3]){
                            if(player != nullptr){
                                player->setCrouch(true);
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[3]){
                            if(player != nullptr){
                                player->setCrouch(false);
                            }
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[4]){
                            if(!jumpHolding){
                                if(player != nullptr){
                                    player->setJump(true);
                                    player->setJumpHold(true);
                                }
                                jumpHolding = true;
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[4]){
                            jumpHolding = false;
                            if(player != nullptr)
                                player->setJumpHold(false);
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[5]){
                            if(!dashHolding){
                                if(player != nullptr){
                                    player->setDash(true);
                                }
                                dashHolding = true;
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[5]){
                            dashHolding = false;
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[6]){
                            if(!actionHolding){
                                if(player != nullptr){
                                    player->setAction(true);
                                }
                                actionHolding = true;
                                //Check for the collision list of player for an interactive object
                                //if is npc, set the dialogue file and enable the object
                                //also set the dialogue flag so the game enters in the state.
                                HitboxComponent *playerHit = ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject("Player"), "HITBOX");
                                if(playerHit != nullptr){
                                    for(unsigned k=0; k<playerHit->getCollisionList().size(); k++){
                                        //Checking if its NPC
                                        NPCScript *npc = ScriptsInterface::getComponent<NPCScript>(ScriptsInterface::getObject(playerHit->getCollisionList()[k].name), "SCRIPT", "NPCScript");
                                        if(npc != nullptr){
                                            if(npc->getDialogueFile() != ""){
                                                dialogueManager->setFile(npc->getDialogueFile());
                                                ScriptsInterface::getObject("DialogueBox")->setEnabled(true);
                                                dialogue = true;
                                            }
                                            break;
                                        }
                                        npc = nullptr;
                                    }
                                }
                                playerHit = nullptr;
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[6]){
                            actionHolding = false;
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[7]){
                            //add alpha to the fade out object, when it reaches 255, exit to the main menu
                            ScriptsInterface::goToState("data/states/MenuState.config");
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[7]){
                            //set the fadout object alpha to 0
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[8]){
                            if(!fullHold){
                                ScriptsInterface::fullScreen("mainWindow");
                                fullHold = true;
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[8]){
                            fullHold = false;
                        }
                    }
                    if(dialogue){

                    }
                    if(cutscene){

                    }
                }
            }
            inputs = nullptr;
        }
};

#endif // GAMESCRIPT_H
