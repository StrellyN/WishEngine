#ifndef NPCSCRIPT_H
#define NPCSCRIPT_H

namespace WishEngine{
    class NPCScript : public ScriptComponent{
        private:
            std::string dialogueFile;

        public:
            NPCScript(std::string dFile){
                dialogueFile = dFile;
            }

            ~NPCScript(){

            }

            std::string getDialogueFile(){
                return dialogueFile;
            }

            void execute(GameObject &thisObj, double deltaT){
                //Here goes the AI if there's ever one.
            }
    };
}

#endif // NPCSCRIPT_H
