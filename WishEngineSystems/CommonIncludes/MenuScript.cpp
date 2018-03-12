#ifndef MENUSCRIPT_H
#define MENUSCRIPT_H

/**
	You can add attributes both in private(recommended) and public,
	but remember to create getters and setters if you do it in private.
**/

using namespace WishEngine;

class MenuScript : public ScriptComponent{
    private:
        int selectedOption;
        std::vector<GameObject*> menuOptions;
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
        bool up, down, optionsLoaded = false, fullHold = false;

    public:
		/**
			Initialization code here. This method will execute when the
			script is created in an object.
		**/
        MenuScript(){
            selectedOption = 0;
            up = false;
            down = false;
            //Check if input configuration file exists, if exists, load it, if not, set standard controls.
        }

		/**
			Destruction code here, remember to delete your pointers if this
			component has the ownership, example, a component created in
			this script. If you got a system, an object or a component via
			the game state, you probably don't want to delete it.
			This method will execute when the object that this is attached to
			is deleted, or if the component is deleted manualy.
		**/
        ~MenuScript(){
            menuOptions.clear();
            controlsAssigned.clear();
        }

		/**
			Execution code here. This method will execute every logic update.
		**/
        void execute(GameObject &thisObj, double deltaT){
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

            if(menuOptions.empty()){
                menuOptions.push_back(ScriptsInterface::getObject("Continue"));
                menuOptions.push_back(ScriptsInterface::getObject("NewGame"));
                menuOptions.push_back(ScriptsInterface::getObject("Options"));
                menuOptions.push_back(ScriptsInterface::getObject("QuitGame"));
                ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], COMPONENTTYPES::GRAPHIC)->getText().getIndividualCharacterColor()[0].setA(255);
            }

            handleInput(thisObj);
        }

        void handleInput(GameObject &thisObj){
            InputComponent *inputs = ScriptsInterface::getComponent<InputComponent>(&thisObj, COMPONENTTYPES::INPUT);
            if(inputs != nullptr){
                for(unsigned i=0; i<inputs->getInputs().size(); i++){
                    bool press = false, release = false;
                    if(inputs->getInputs()[i].getType() == EVENTTYPES::KEYBOARD_PRESS ||
                       inputs->getInputs()[i].getType() == EVENTTYPES::GAMEPAD_PRESS ||
                       inputs->getInputs()[i].getType() == EVENTTYPES::MOUSE_PRESS ||
                       inputs->getInputs()[i].getType() == EVENTTYPES::GAMEPAD_AXIS){
                        press = true;
                    }
                    if(inputs->getInputs()[i].getType() == EVENTTYPES::KEYBOARD_RELEASE ||
                       inputs->getInputs()[i].getType() == EVENTTYPES::GAMEPAD_RELEASE ||
                       inputs->getInputs()[i].getType() == EVENTTYPES::MOUSE_RELEASE){
                        release = true;
                    }
                    if(press && inputs->getInputs()[i].getValue() == controlsAssigned[0]){
                        if(!up){
                            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], COMPONENTTYPES::GRAPHIC)->getText().getIndividualCharacterColor()[0].setA(128);
                            selectedOption--;
                            if(selectedOption < 0) selectedOption = menuOptions.size()-1;
                            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], COMPONENTTYPES::GRAPHIC)->getText().getIndividualCharacterColor()[0].setA(255);
                        }
                        up = true;
                    }
                    if(press && inputs->getInputs()[i].getValue() == controlsAssigned[3]){
                        if(!down){
                            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], COMPONENTTYPES::GRAPHIC)->getText().getIndividualCharacterColor()[0].setA(128);
                            selectedOption++;
                            if(selectedOption > menuOptions.size()-1) selectedOption = 0;
                            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], COMPONENTTYPES::GRAPHIC)->getText().getIndividualCharacterColor()[0].setA(255);
                        }
                        down = true;
                    }
                    if(release && inputs->getInputs()[i].getValue() == controlsAssigned[0]){
                        up = false;
                    }
                    if(release && inputs->getInputs()[i].getValue() == controlsAssigned[3]){
                        down = false;
                    }

                    if(press && inputs->getInputs()[i].getValue() == controlsAssigned[4]){
                        switch(selectedOption){
                            case 0:
                                ScriptsInterface::goToState("data/states/MainGame.config");
                                break;
                            case 1:
                                Utils::writeIntToFile("data/NewGame.FLAG", "NEWGAME", true, false);
                                ScriptsInterface::goToState("data/states/MainGame.config");
                                break;
                            case 2:
                                ScriptsInterface::goToState("data/states/OptionsState.config");
                                break;
                            case 3:
                                ScriptsInterface::quit();
                                break;
                        }
                    }
                    if(press && inputs->getInputs()[i].getValue() == controlsAssigned[7]){
                        ScriptsInterface::quit();
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
            }
            inputs = nullptr;
        }
};

#endif // MENUSCRIPT_H

