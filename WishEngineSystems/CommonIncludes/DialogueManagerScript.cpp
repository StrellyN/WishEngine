#ifndef DIALOGUEMANAGERSCRIPT_H
#define DIALOGUEMANAGERSCRIPT_H

/**
    How will dialogues work:

    - First you'll create the Dialogue Manager instance, and give the file it needs to read.
    - After that, the game script will head into cutscene mode so you can't control the character.
    - The Dialogue Manager will load the dialogue file getting the initial dialogue npc image and all
      the text. The text is going to be stored into a vector of vector of strings, each vector is going to
      be a path on the dialogue, in case there is options and stuff, so choosing an option will increase or
      decrease the index of the vector that's gonna be used.
    - When reading the text, the Dialogue Manager will check for "commands" before showing the text. This
      commands will be things like, change the animation of the face, change the sprite of the face, have
      colorful text, etc... If a command is detected, it won't be shown in the dialogue box, of course.
    - When the current dialogue vector is empty, the dialogue will end and in the last message, the arrow
      indicating you can continue dialoguing, won't appear.


      IDEA FOR MULTIPLE CHOICES IN ONE CONVERSATION:
      Once you've chosen your path in the current options, the branches corresponding the other options are
      deleted from the tree.
**/

namespace WishEngine{
    class DialogueManagerScript : public ScriptComponent{
        private:
            bool dialogueOver = false, loaded = false;
            bool accept = false; //Input used to advance dialogue or choose an option.
            bool down = false, up = false; //Inputs used to select the option you want.
            bool skip = false; //Input to skip the one character at a time dialogue.
            std::string file = "";
            std::vector<std::vector<std::string>> dialogueTree;
            unsigned currentDialogueBranch = 0, currentLine = 0, optionChosen = 0;
            //currentDialogueBranch += optionChosen; to go to the next branch
            //Or not, probably gonna be problems with options that have more options inside
            //Unless I create each branch when reading the options.

        public:
            DialogueManagerScript(){
                //Create the portrait object and the text object
                ScriptsInterface::insertObject("DialoguePortrait", false);
                ScriptsInterface::insertObject("DialogueText", false);
            }

            ~DialogueManagerScript(){

            }

            void clearData(){
                for(unsigned i=0; i<dialogueTree.size(); i++){
                    dialogueTree[i].clear();
                }
                dialogueTree.clear();
            }

            void execute(GameObject &thisObj, double deltaT){
                if(!loaded && file != ""){
                    //Load the file
                    std::ifstream toLoad(file);
                    std::string currentInstruction;
                    dialogueTree.emplace_back();
                    while(std::getline(toLoad, currentInstruction)){
                        if(currentInstruction == ""){
                            continue;
                        }
                        if(currentInstruction == "SET_TALK_CHAR"){
                            //Set the portrait file
                            std::getline(toLoad, currentInstruction);
                            ScriptsInterface::getComponent<GraphicComponent>(&thisObj, "GRAPHIC")->setTextureFile(currentInstruction);
                        }
                        else if(currentInstruction == "SET_TALK_ANIM"){
                            //Set the animation state
                            std::getline(toLoad, currentInstruction);
                            ScriptsInterface::getComponent<AnimatorComponent>(&thisObj, "ANIMATOR")->setCurrentState(currentInstruction);
                        }
                        else if(currentInstruction == "DIAG_OPT"){
                            //There are options comming.
                            dialogueTree[currentDialogueBranch].push_back(currentInstruction);
                            unsigned amount;
                            toLoad >> amount;
                            std::getline(toLoad, currentInstruction);
                            for(unsigned i=0; i<amount; i++){
                                std::getline(toLoad, currentInstruction);
                                dialogueTree[currentDialogueBranch].push_back(currentInstruction);
                                dialogueTree.emplace_back();
                            }
                        }
                        /**else if(currentInstruction == "DIAG_END"){
                            //End of a branch
                        }**/
                        else if(currentInstruction == "DIAG_OPT_START"){
                            //Start of a new branch+
                            currentDialogueBranch++;
                        }
                        else{
                            //It's a normal line, insert it in the current branch
                            dialogueTree[currentDialogueBranch].push_back(currentInstruction);
                        }
                    }
                    toLoad.close();

                    loaded = true;
                    currentDialogueBranch = 0;
                }

                if(loaded){
                    //Do everything here;
                }
            }

            bool isDialogueOver(){
                return dialogueOver;
            }

            void setAccept(bool acc){
                accept = acc;
            }

            void setDown(bool d){
                down = d;
            }

            void setUp(bool u){
                up = u;
            }

            void setSkip(bool s){
                skip = s;
            }

            void setFile(std::string f){
                file = f;
            }
    };
}

#endif // DIALOGUEMANAGERSCRIPT_H
