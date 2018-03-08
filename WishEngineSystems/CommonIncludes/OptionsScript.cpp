#ifndef OPTIONSSCRIPT_H
#define OPTIONSSCRIPT_H

using namespace WishEngine;

class OptionsScript : public ScriptComponent{
    private:
        PlayerScript *player = nullptr;
        bool testing = false, exitHold = false, jumpHolding = false, dashHolding = false, actionHolding = false, editing = false, canEdit = true;
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
        bool up, down, left, right, optionsLoaded = false, fullHold = false;

    public:
        OptionsScript(){
            selectedOption = 0;
            up = false;
            down = false;
            left = false;
            right = false;
            menuOptions.clear();
        }

        ~OptionsScript(){
            menuOptions.clear();
            controlsAssigned.clear();
            player = nullptr;
        }

        void reWrite(){
            std::string rep = "UP: ";
            rep = rep + controlsAssigned[0];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[0], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "LEFT: " + controlsAssigned[1];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[1], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "RIGHT: " + controlsAssigned[2];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[2], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "DOWN: " + controlsAssigned[3];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[3], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "SELECT/JUMP: " + controlsAssigned[4];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[4], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "DASH: " + controlsAssigned[5];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[5], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "ACTION: " + controlsAssigned[6];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[6], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "BACK/QUIT: " + controlsAssigned[7];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[7], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            rep = "FULLSCREEN: " + controlsAssigned[8];
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[8], "GRAPHIC")->getText().setText(rep, 255, 255, 255, 128, 42);
            //rep = "FPS: << " + Utils::intToString(ScriptsInterface::getMaxFPS()) + " >>";
            //dynamic_cast<GraphicComponent*>(menuOptions[9]->getComponent(C_TYPES::GRAPHIC))->getText().setText(rep, 255, 255, 255, 128, 42);
            ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], "GRAPHIC")->getText().getIndividualCharacterColor()[0].setA(255);
        }

        void save(){
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

        void execute(GameObject &thisObj, double deltaT){
            if(player == nullptr){
                player = ScriptsInterface::getComponent<PlayerScript>(ScriptsInterface::getObject("Player"), "SCRIPT", "PlayerScript");
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

            if(!menuOptions.empty() && menuOptions[0]->getName() != "UpEdit"){
                menuOptions.clear();
            }
            if(menuOptions.empty()){
                menuOptions.push_back(ScriptsInterface::getObject("UpEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("LeftEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("RightEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("DownEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("JumpEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("DashEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("ActionEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("QuitEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("FullscreenEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("FPSEdit"));
                menuOptions.push_back(ScriptsInterface::getObject("TestControls"));
                menuOptions.push_back(ScriptsInterface::getObject("GoBack"));
                reWrite();
            }

            handleInput(thisObj);

            if(editing){
                ScriptsInterface::getObject("ChangeText")->setEnabled(true);
            }
            else{
                ScriptsInterface::getObject("ChangeText")->setEnabled(false);
            }
        }

        void handleInput(GameObject &thisObj){
            InputComponent *inputs = ScriptsInterface::getComponent<InputComponent>(&thisObj, "INPUT");
            if(inputs != nullptr){
                if(!testing && !menuOptions.empty()){
                    if(!editing){
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
                            if(press && inputs->getInputs()[i].getValue() == controlsAssigned[0]){
                                if(!up){
                                    ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], "GRAPHIC")->getText().getIndividualCharacterColor()[0].setA(128);
                                    selectedOption--;
                                    if(selectedOption < 0) selectedOption = menuOptions.size()-1;
                                    ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], "GRAPHIC")->getText().getIndividualCharacterColor()[0].setA(255);
                                }
                                up = true;
                            }
                            if(press && inputs->getInputs()[i].getValue() == controlsAssigned[3]){
                                if(!down){
                                    ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], "GRAPHIC")->getText().getIndividualCharacterColor()[0].setA(128);
                                    selectedOption++;
                                    if(selectedOption > menuOptions.size()-1) selectedOption = 0;
                                    ScriptsInterface::getComponent<GraphicComponent>(menuOptions[selectedOption], "GRAPHIC")->getText().getIndividualCharacterColor()[0].setA(255);
                                }
                                down = true;
                            }
                            if(press && inputs->getInputs()[i].getValue() == controlsAssigned[1]){
                                if(!left){
                                    if(selectedOption == 9){ //Change FPS
                                        int currentFPS = 60/**ObjectFactory::getObjectFactory()->getMaxFPS()**/;
                                        if(currentFPS == 30){
                                            ScriptsInterface::setMaxFPS(60);
                                        }
                                        if(currentFPS == 60){
                                            ScriptsInterface::setMaxFPS(60);
                                        }
                                        if(currentFPS == 120){
                                            ScriptsInterface::setMaxFPS(60);
                                        }
                                        reWrite();
                                    }
                                }
                                left = true;
                            }
                            if(press && inputs->getInputs()[i].getValue() == controlsAssigned[2]){
                                if(!right){
                                    if(selectedOption == 9){ //Change FPS
                                        int currentFPS = 60/**ObjectFactory::getObjectFactory()->getMaxFPS()**/;
                                        if(currentFPS == 30){
                                            ScriptsInterface::setMaxFPS(60);
                                        }
                                        if(currentFPS == 60){
                                            ScriptsInterface::setMaxFPS(60);
                                        }
                                        if(currentFPS == 120){
                                            ScriptsInterface::setMaxFPS(60);
                                        }
                                        reWrite();
                                    }
                                }
                                right = true;
                            }
                            if(release && inputs->getInputs()[i].getValue() == controlsAssigned[0]){
                                up = false;
                            }
                            if(release && inputs->getInputs()[i].getValue() == controlsAssigned[3]){
                                down = false;
                            }
                            if(release && inputs->getInputs()[i].getValue() == controlsAssigned[1]){
                                left = false;
                            }
                            if(release && inputs->getInputs()[i].getValue() == controlsAssigned[2]){
                                right = false;
                            }
                            if(press && inputs->getInputs()[i].getValue() == controlsAssigned[4]){
                                if(canEdit && selectedOption >= 0 && selectedOption < 9){
                                    editing = true;
                                    canEdit = false;
                                }
                                if(selectedOption == 10){
                                    ScriptsInterface::getComponent<GraphicComponent>(ScriptsInterface::getObject("StateText"), "GRAPHIC")->getText().setText("Right now: Testing", 255, 255, 255, 255, 42);
                                    testing = true;
                                }
                                if(selectedOption == 11){
                                    save();
                                    ScriptsInterface::goToState("data/states/MenuState.config");
                                }
                            }
                            if(!canEdit && release && (inputs->getInputs()[i].getValue() == controlsAssigned[selectedOption] || inputs->getInputs()[i].getValue() == controlsAssigned[4])){
                                canEdit = true;
                            }
                            if(press && (inputs->getInputs()[i].getValue() == controlsAssigned[7])){
                                if(!exitHold){
                                    save();
                                    ScriptsInterface::goToState("data/states/MenuState.config");
                                    exitHold = true;
                                }
                            }
                            if(release && (inputs->getInputs()[i].getValue() == controlsAssigned[7])){
                                exitHold = false;
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
                    else{
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
                            if(!canEdit && release && inputs->getInputs()[i].getValue() == controlsAssigned[4]){
                                canEdit = true;
                            }
                            if(canEdit && press){
                                std::string newKey = inputs->getInputs()[i].getValue();
                                bool used = false;
                                for(unsigned i=0; i<9; i++){
                                    if(i != selectedOption && controlsAssigned[i] == newKey){
                                        used = true;
                                    }
                                }
                                if(!used){
                                    controlsAssigned[selectedOption] = newKey;
                                    reWrite();
                                    editing = false;
                                    canEdit = false;
                                }
                            }
                        }
                        if(selectedOption == 0){
                            up = true;
                        }
                        if(selectedOption == 1){
                            left = true;
                        }
                        if(selectedOption == 2){
                            right = true;
                        }
                        if(selectedOption == 3){
                            down = true;
                        }
                        if(selectedOption == 7){
                            exitHold = true;
                        }
                        if(selectedOption == 8){
                            fullHold = true;
                        }
                    }
                }
                else if(testing && player != nullptr){
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
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[6]){
                            actionHolding = false;
                        }
                        if(press && inputs->getInputs()[i].getValue() == controlsAssigned[7]){
                            if(!exitHold){
                                ScriptsInterface::getComponent<GraphicComponent>(ScriptsInterface::getObject("StateText"), "GRAPHIC")->getText().setText("Right now: Editing", 255, 255, 255, 255, 42);
                                testing = false;
                                exitHold = true;
                            }
                        }
                        if(release && inputs->getInputs()[i].getValue() == controlsAssigned[7]){
                            exitHold = false;
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
            }
            inputs = nullptr;
        }
};

#endif // OPTIONSSCRIPT_H
