#ifndef TITLESCRIPT_H
#define TITLESCRIPT_H

namespace WishEngine{
    class TitleScript : public ScriptComponent{
        private:
            double titleTimer;
        public:
            TitleScript(){
                titleTimer = 0;
            }

            ~TitleScript(){}

            void execute(GameObject &thisObj, double deltaT){ //Done
                titleTimer += 1 * deltaT;
                if(titleTimer > 3){
                    ScriptsInterface::goToState("data/states/MenuState.config");
                }
                GraphicComponent *titleImage = ScriptsInterface::getComponent<GraphicComponent>(&thisObj, COMPONENTTYPES::GRAPHIC);
                if(titleImage != nullptr && titleTimer > 2){
                    titleImage->setAlpha(255 * (3 - titleTimer));
                }
                titleImage = nullptr;
            }
    };
}

#endif // TITLESCRIPT_H
