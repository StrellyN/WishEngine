#ifndef CAMERASCRIPTFOLLOWING_H
#define CAMERASCRIPTFOLLOWING_H

/**
	You can add attributes both in private(recommended) and public,
	but remember to create getters and setters if you do it in private.
**/

#include "PlayerScript.cpp"

using namespace WishEngine;

class CameraScriptFollowing : public ScriptComponent{
    private:
        std::string objectToFollow;
        unsigned horizontalState;

    public:
		/**
			Initialization code here. This method will execute when the
			script is created in an object.
		**/
        CameraScriptFollowing(){
            objectToFollow = "Player";
            horizontalState = 3;
        }

		/**
			Destruction code here, remember to delete your pointers if this
			component has the ownership, example, a component created in
			this script. If you got a system, an object or a component via
			the game state, you probably don't want to delete it.
			This method will execute when the object that this is attached to
			is deleted, or if the component is deleted manualy.
		**/
        ~CameraScriptFollowing(){

        }

		/**
			Execution code here. This method will execute every logic update.
		**/
        void execute(GameObject &thisObj, double deltaT){
            GameObject *toFollow = ScriptsInterface::getObject(objectToFollow);
            DimensionComponent *camDimention = ScriptsInterface::getComponent<DimensionComponent>(&thisObj, "DIMENSION"),
                               *followDimention = nullptr;
            PlayerScript *playerScript = ScriptsInterface::getComponent<PlayerScript>(toFollow, "SCRIPT", "PlayerScript");
            if(toFollow != nullptr){
                followDimention = ScriptsInterface::getComponent<DimensionComponent>(toFollow, "DIMENSION");
            }


            if(camDimention != nullptr && followDimention != nullptr && playerScript != nullptr){
                //Y axis
                if(playerScript->getVerticalSpeed() < 0){ //Goes up if the character surpass the cameras middle point
                    if(followDimention->getY() < (camDimention->getY() + (camDimention->getH()/2))){
                        camDimention->setY(camDimention->getY() + (playerScript->getVerticalSpeed() * deltaT));
                        if((camDimention->getY() + ((camDimention->getH())/2)) < followDimention->getY()){
                            camDimention->setY(followDimention->getY() - (camDimention->getH()/2));
                        }
                    }
                }
                else if(playerScript->getVerticalSpeed() > 0){ //Goes down and sets the character in the middle of the screen
                    if(followDimention->getY() > (camDimention->getY() + ((camDimention->getH())/2))){
                        camDimention->setY(camDimention->getY() + (playerScript->getVerticalSpeed() * deltaT));
                        if((camDimention->getY() + ((camDimention->getH())/2)) > followDimention->getY()){
                            camDimention->setY(followDimention->getY() - ((camDimention->getH())/2));
                        }
                    }
                }
                else{ //Character comes 4/7 of the way of the camera
                    if(playerScript->getPlayerState() != 5){
                        if(followDimention->getY() > (camDimention->getY() + ((camDimention->getH()*4)/7)) + 1){
                            camDimention->setY(camDimention->getY() + (500 * deltaT));
                            if((camDimention->getY() + ((camDimention->getH()*4)/7)) + 1 > followDimention->getY()){
                                camDimention->setY(followDimention->getY() - ((camDimention->getH()*4)/7));
                            }
                        }
                        else if(followDimention->getY() < (camDimention->getY() + ((camDimention->getH()*4)/7)) - 1){
                            camDimention->setY(camDimention->getY() + (-500 * deltaT));
                            if((camDimention->getY() + ((camDimention->getH()*4)/7)) - 1 < followDimention->getY()){
                                camDimention->setY(followDimention->getY() - ((camDimention->getH()*4)/7));
                            }
                        }
                        else{
                            camDimention->setY(camDimention->getY() + 0);
                        }
                    }
                    else{
                        camDimention->setY(camDimention->getY() + 0);
                    }
                }

                //X axis
                if(horizontalState == 0){ //Rushing left
                    //check for orientation
                    if(!playerScript->getHDirection()){ //left
                        if(camDimention->getX() > followDimention->getX() - ((camDimention->getW()*3)/5)){
                            camDimention->setX(camDimention->getX() - (((-playerScript->getHorizontalSpeed()) + 300) * deltaT));
                        }
                        else{
                            horizontalState = 1;
                        }
                    }
                    else{ //right
                        if(camDimention->getX() < (followDimention->getX() + followDimention->getW()) - ((camDimention->getW()*4)/5)){
                            horizontalState = 2;
                        }
                        else{
                            camDimention->setX(camDimention->getX());
                        }
                    }
                }
                else if(horizontalState == 1){ //left
                    if(!playerScript->getHDirection()){ //left
                        if(camDimention->getX() > followDimention->getX() - ((camDimention->getW()*3)/5)){
                            camDimention->setX(camDimention->getX() + (playerScript->getHorizontalSpeed() * deltaT));
                        }
                        else{
                            camDimention->setX(camDimention->getX());
                        }
                    }
                    else{ //right
                        if(camDimention->getX() < (followDimention->getX() + followDimention->getW()) - ((camDimention->getW()*4)/5)){
                            horizontalState = 2;
                        }
                        else{
                            camDimention->setX(camDimention->getX());
                        }
                    }
                }
                else if(horizontalState == 2){ //rushing right
                    if(playerScript->getHDirection()){ //right
                        if(camDimention->getX() < (followDimention->getX() + followDimention->getW())  - ((camDimention->getW()*2)/5)){
                            camDimention->setX(camDimention->getX() + ((playerScript->getHorizontalSpeed() + 300) * deltaT));
                        }
                        else{
                            horizontalState = 3;
                        }
                    }
                    else{ //left
                        if(camDimention->getX() > (followDimention->getX()) - ((camDimention->getW()*1)/5)){
                            horizontalState = 0;
                        }
                        else{
                            camDimention->setX(camDimention->getX());
                        }
                    }
                }
                else if(horizontalState == 3){ //right
                    if(playerScript->getHDirection()){ //right
                        if(camDimention->getX() < (followDimention->getX() + followDimention->getW())  - ((camDimention->getW()*2)/5)){
                            camDimention->setX(camDimention->getX() + (playerScript->getHorizontalSpeed() * deltaT));
                        }
                        else{
                            camDimention->setX(camDimention->getX());
                        }
                    }
                    else{ //left
                        if(camDimention->getX() > (followDimention->getX()) - ((camDimention->getW()*1)/5)){
                            horizontalState = 0;
                        }
                        else{
                            camDimention->setX(camDimention->getX());
                        }
                    }
                }
            }

            toFollow = nullptr;
            camDimention = nullptr;
            followDimention = nullptr;
            playerScript = nullptr;
        }

        void setObjectToFollow(std::string newObject){
            objectToFollow = newObject;
        }
};

#endif // CAMERASCRIPTFOLLOWING_H
