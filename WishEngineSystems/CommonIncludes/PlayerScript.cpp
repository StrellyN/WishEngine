#ifndef PLAYERSCRIPT_H
#define PLAYERSCRIPT_H

/**
	You can add attributes both in private(recommended) and public,
	but remember to create getters and setters if you do it in private.
**/

using namespace WishEngine;

class PlayerScript : public ScriptComponent{
    private:
        DimensionComponent *playerDim = nullptr;
        double verticalSpeed, gravity, terminalVelocity, horizontalSpeed, walkingSpeed = 400, dashTimer, poundTimer, recoverTimer, jumpForce;
        bool left, right, crouching, jump, jumpHolding, wallSlidable, wallJumpDirection, dash, dashHolding, action, actionHolding;
        bool hDirection; //0 = left, 1 = right
        bool vDirection; //0 = up, 1 = down
        bool highJump = false;
        std::vector<double> heightValues;

        //States
        unsigned playerState, jumpsLeft, maxJumps = 2, dashLeft, maxDash = 1;
        /**
            1 = grounded
            2 = walking
            3 = jumping
            4 = falling
            5 = dashing
            6 = pounding /When you hit the ground from pounding theres a small period of time where you can jump higher
            7 = wallsliding
            8 = bonking
            9 = walljump
            10 = pound recover
            11 = getting up

            grounded -> crouching, walking, jumping, falling.
            walking -> grounded, crouching, jumping, falling, dashing.
            jumping -> falling, dashing, pounding.
            falling -> jumping, grounded, pounding, dashing, wallsliding.
            dashing -> falling, jumping, pounding, bonking.
            pounding -> pound recover.
            wallsliding -> falling, jumping, grounded.
            bonking -> getting up.
            walljump -> falling, dashing, pounding.
            pound recover -> jumping, grounded. /depends on animation ending (timer for now)
            getting up -> grounded. /depends on animation ending (timer for now)
        **/

    public:
		/**
			Initialization code here. This method will execute when the
			script is created in an object.
		**/
		bool getHDirection(){
            return hDirection;
		}

		double getVerticalSpeed(){
            return verticalSpeed;
		}

		double getHorizontalSpeed(){
            return horizontalSpeed;
		}

        unsigned getPlayerState(){
            return playerState;
        }

        PlayerScript(){
            verticalSpeed = 0;
            gravity = 9.8;
            terminalVelocity = 500;
            horizontalSpeed = 0;
            left = false;
            right = false;
            hDirection = 1;
            vDirection = 1;
            playerState = 4;
            crouching = false;
            jump = false;
            jumpHolding = false;
            jumpsLeft = maxJumps;
            dashLeft = maxDash;
            wallSlidable = false;
            wallJumpDirection = false;
            dash = false;
            dashHolding = false;
            action = false;
            actionHolding = false;
            dashTimer = 0;
            poundTimer = 0;
            recoverTimer = 0;
            jumpForce = -510;
        }

		/**
			Destruction code here, remember to delete your pointers if this
			component has the ownership, example, a component created in
			this script. If you got a system, an object or a component via
			the game state, you probably don't want to delete it.
			This method will execute when the object that this is attached to
			is deleted, or if the component is deleted manualy.
		**/
        ~PlayerScript(){
            playerDim = nullptr;
            heightValues.clear();
        }

		/**
			Execution code here. This method will execute every logic update.
		**/
        void execute(GameObject &thisObj, double deltaT){
            if(playerDim == nullptr){
                playerDim = ScriptsInterface::getComponent<DimensionComponent>(&thisObj, COMPONENTTYPES::DIMENSION);

                //Side hitbox
                ScriptsInterface::insertObject("PlayerSideHitbox", true);
                ScriptsInterface::insertComponent<DimensionComponent>("PlayerSideHitbox", DimensionComponent(playerDim->getX() + playerDim->getW() - 1, playerDim->getY() + 30, 5, playerDim->getH() - 40));
                ScriptsInterface::insertComponent<GraphicComponent>("PlayerSideHitbox", GraphicComponent(false));
                ScriptsInterface::insertComponent<HitboxComponent>("PlayerSideHitbox", HitboxComponent(0, 0, 5, playerDim->getH() - 40, false, true));

                //Bottom hitbox
                ScriptsInterface::insertObject("PlayerBottomHitbox", true);
                ScriptsInterface::insertComponent<DimensionComponent>("PlayerBottomHitbox", DimensionComponent(playerDim->getX() + 20, playerDim->getY() + playerDim->getH() - 1, playerDim->getW() - 35, 5));
                ScriptsInterface::insertComponent<GraphicComponent>("PlayerBottomHitbox", GraphicComponent(false));
                ScriptsInterface::insertComponent<HitboxComponent>("PlayerBottomHitbox", HitboxComponent(0, 0, playerDim->getW() - 35, 5, false, true));

                //Bottom hitbox 2
                /**
                    This second hitbox is for checking if the character is still grounded when the normal bottom hitbox isn't
                    colliding with anything, so, each frame, the height value of the character is saved and then, if the
                    normal bottom hitbox isn't colliding and all the 10 height values are the same, it'll check if this
                    hitbox is colliding with anything solid except the player, and if it is, then it'll set the player
                    as grounded.
                **/
                ScriptsInterface::insertObject("PlayerBottomHitbox2", true);
                ScriptsInterface::insertComponent<DimensionComponent>("PlayerBottomHitbox2", DimensionComponent(playerDim->getX(), playerDim->getY() + playerDim->getH() - 1, playerDim->getW(), 5));
                ScriptsInterface::insertComponent<HitboxComponent>("PlayerBottomHitbox2", HitboxComponent(0, 0, playerDim->getW(), 5, false, true));
            }

            if(playerDim != nullptr){
                //Check some inputs to change states
                if(playerState == 1 && (right || left)){
                    playerState = 2;
                }
                if(playerState == 2 && !(right || left)){
                    playerState = 1;
                }
                if(jump){
                    if(playerState == 7){
                        verticalSpeed = jumpForce;
                        if(wallJumpDirection){
                            horizontalSpeed -= 600;
                        }
                        else{
                            horizontalSpeed += 600;
                        }
                        hDirection = !wallJumpDirection;
                        dashLeft = maxDash;
                        playerState = 9;
                    }
                    else if(playerState != 8 && playerState != 11){
                        if(jumpsLeft > 0){ //check for states that don't let jump happen
                            //change animation for jump here
                            if(playerState == 1 && crouching){ //This is like doing the pounding jump but crouching still
                                verticalSpeed = jumpForce-110;
                                highJump = true;
                            }
                            else if(playerState == 10){
                                poundTimer = 0;
                                verticalSpeed = jumpForce-110;
                                highJump = true;
                            }
                            else{
                                highJump = false;
                                verticalSpeed = jumpForce;
                            }
                            playerState = 3;
                            jumpsLeft -= 1;
                            dashLeft = maxDash;
                        }
                        /**else if(playerState == 5){ Deleted to try the "you can only cancel the dash when you have jumps
                            playerState = 4;          or with the ground pound theory.
                        }**/
                    }
                    jump = false;
                }
                if(wallSlidable){
                    if(hDirection && right){
                        wallJumpDirection = true;
                        playerState = 7;
                    }
                    if(!hDirection && left){
                        wallJumpDirection = false;
                        playerState = 7;
                    }
                }
                //check for states where you can't change direction
                if(playerState != 5 && playerState != 6 && playerState != 7 && playerState != 8 && playerState != 10 && playerState != 11){
                    if(right){
                        hDirection = true;
                    }
                    if(left){
                        hDirection = false;
                    }
                }

                //Dash
                if(dash){
                    if(dashTimer <= 0){
                        if(playerState == 2 || playerState == 3 || playerState == 4 || playerState == 9){
                            if(/**right || left){**/!crouching){ //the commented stuff in this line and the one bellow is for
                                if(dashLeft > 0){                //having the groundpound not need crouching just not pressing l || r
                                    playerState = 5;
                                    dashLeft -= 1;
                                }
                            }
                            else{
                                if(playerState == 1 || playerState == 2){
                                    if(dashLeft > 0){
                                        playerState = 5;
                                        dashLeft -= 1;
                                    }
                                }
                                else{
                                    playerState = 6;
                                }
                            }
                        }
                    }
                    if((playerState == 5 || playerState == 4 || playerState == 3 || playerState == 9) /**&& !(right || left)){**/&& crouching){
                        playerState = 6;
                    }
                    if(playerState == 7){
                        playerState = 4;
                    }
                    dash = false;
                }
                if(playerState != 5 && dashTimer > 0){
                    dashTimer -= 1 * deltaT;
                    if(dashTimer < 0){
                        dashTimer = 0;
                    }
                }

                //State dependent things (remember to change animator state)
                if(playerState == 1){ //grounded
                    jumpsLeft = maxJumps;
                    dashLeft = maxDash;
                    verticalSpeed = 0;
                    horizontalSpeed = 0;
                    if(crouching){
                        //do crouching stuff in animation and character size
                    }
                }
                if(playerState == 2){ //walking /remember to change directions for animation thing
                    jumpsLeft = maxJumps;
                    dashLeft = maxDash;
                    if(crouching){
                       verticalSpeed = 0;
                        if(right){
                            horizontalSpeed = walkingSpeed*0.5;
                        }
                        if(left){
                            horizontalSpeed = -walkingSpeed*0.5;
                        }
                        if(!right && !left){
                            horizontalSpeed = 0;
                        }
                    }
                    else{
                        verticalSpeed = 0;
                        if(right){
                            horizontalSpeed = walkingSpeed;
                        }
                        if(left){
                            horizontalSpeed = -walkingSpeed;
                        }
                        if(!right && !left){
                            horizontalSpeed = 0;
                        }
                    }
                }
                if(playerState == 3){ //jumping /remember to change directions for animation thing
                    verticalSpeed += gravity;//*deltaT;
                    if(verticalSpeed > terminalVelocity){
                        verticalSpeed = terminalVelocity;
                    }

                    if(right){
                        horizontalSpeed = walkingSpeed;
                    }
                    if(left){
                        horizontalSpeed = -walkingSpeed;
                    }
                    if(!right && !left){
                        horizontalSpeed = 0;
                    }
                    if(verticalSpeed > 0){
                        //change animation for falling here
                        playerState = 4;
                    }
                    if(!jumpHolding){ //Now it plays better
                        if(verticalSpeed < -400)
                            verticalSpeed = -400; //If I forget what this is, it's the jump "cancel" when you release the jump key.
                        playerState = 4;
                    }
                }
                if(playerState == 4){ //falling /remember to change directions for animation thing
                    verticalSpeed += (gravity*2);//*deltaT;
                    if(verticalSpeed > terminalVelocity){
                        verticalSpeed = terminalVelocity;
                    }

                    if(right){
                        horizontalSpeed = walkingSpeed;
                    }
                    if(left){
                        horizontalSpeed = -walkingSpeed;
                    }
                    if(!right && !left){
                        horizontalSpeed = 0;
                    }
                    if(jumpsLeft > 1){
                        jumpsLeft = 1;
                    }
                }
                if(playerState == 5){ //dashing
                    verticalSpeed = 0;
                    if(hDirection){
                        horizontalSpeed = 700;
                    }
                    else{
                        horizontalSpeed = -700;
                    }
                    dashTimer += 1 * deltaT;
                    if(dashTimer >= 0.3){
                        dashTimer = 0.3;
                        playerState = 4;
                    }
                }
                if(playerState == 6){ //pounding
                    verticalSpeed = 700;
                    horizontalSpeed = 0;
                    if(jumpsLeft > 1){
                        jumpsLeft = 1;
                    }
                }
                if(playerState == 7){ //wallsliding /remember to change directions for animation thing
                    verticalSpeed = 200;
                    horizontalSpeed = 0;
                }
                if(playerState == 8){ //bonking
                    verticalSpeed += gravity;//*deltaT;
                    if(verticalSpeed > terminalVelocity){
                        verticalSpeed = terminalVelocity;
                    }

                    horizontalSpeed -= horizontalSpeed * 0.01;
                }
                if(playerState == 9){ //walljump
                    verticalSpeed += gravity;//*deltaT;
                    if(verticalSpeed > terminalVelocity){
                        verticalSpeed = terminalVelocity;
                    }

                    if(right){
                        if(!wallJumpDirection){
                            //horizontalSpeed = walkingSpeed; //Removed because it caused to be able to wall climb
                        }
                        else{
                            horizontalSpeed += walkingSpeed*0.03;
                        }
                    }
                    if(left){
                        if(wallJumpDirection){
                            //horizontalSpeed = -walkingSpeed; //Removed because it caused to be able to wall climb
                        }
                        else{
                            horizontalSpeed += -walkingSpeed*0.03;
                        }
                    }
                    if(!right && !left){
                        horizontalSpeed -= horizontalSpeed * 0.02;
                    }
                    if(verticalSpeed > 0){
                        //change animation for falling here
                        playerState = 4;
                    }
                }
                if(playerState == 10){ //pound recovery
                    poundTimer += 1 * deltaT;
                    verticalSpeed = 0;
                    horizontalSpeed = 0;
                    jumpsLeft = maxJumps;
                    if(poundTimer > 0.2){
                        poundTimer = 0;
                        playerState = 1;
                    }
                }
                if(playerState == 11){ //getting up
                    recoverTimer += 1 * deltaT;
                    horizontalSpeed = 0;
                    verticalSpeed = 0;
                    if(recoverTimer > 0.5){
                        recoverTimer = 0;
                        playerState = 1;
                    }
                }

                //Add current height to heightValues for falling stuck check
                if(playerState != 5){
                    heightValues.push_back(playerDim->getY());
                }
                if(heightValues.size() > 3){
                    heightValues.erase(heightValues.begin());
                }

                //State independent things
                playerDim->setX(playerDim->getX()+(horizontalSpeed*deltaT));
                playerDim->setY(playerDim->getY()+(verticalSpeed*deltaT));

                DimensionComponent *sideHitBox = ScriptsInterface::getComponent<DimensionComponent>(ScriptsInterface::getObject("PlayerSideHitbox"), COMPONENTTYPES::DIMENSION),
                                   *botHitBox = ScriptsInterface::getComponent<DimensionComponent>(ScriptsInterface::getObject("PlayerBottomHitbox"), COMPONENTTYPES::DIMENSION),
                                   *botHitBox2 = ScriptsInterface::getComponent<DimensionComponent>(ScriptsInterface::getObject("PlayerBottomHitbox2"), COMPONENTTYPES::DIMENSION);
                if(sideHitBox != nullptr && botHitBox != nullptr && botHitBox2 != nullptr){
                    //Check directions to change position and size
                    if(hDirection == false){
                        sideHitBox->setX(playerDim->getX() - 4);
                    }
                    else{
                        sideHitBox->setX(playerDim->getX() + playerDim->getW() - 1);
                    }
                    sideHitBox->setY(playerDim->getY() + 30);
                    botHitBox->setX(playerDim->getX() + 20);
                    botHitBox->setY(playerDim->getY() + playerDim->getH() - 1);
                    botHitBox2->setX(playerDim->getX());
                    botHitBox2->setY(playerDim->getY() + playerDim->getH() - 1);
                }

                sideHitBox = nullptr;
                botHitBox = nullptr;
                botHitBox2 = nullptr;

                //Hit boxes checking for state changing
                HitboxComponent *botHit = ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject("PlayerBottomHitbox"), COMPONENTTYPES::HITBOX),
                                *botHit2 = ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject("PlayerBottomHitbox2"), COMPONENTTYPES::HITBOX),
                                *sideHit = ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject("PlayerSideHitbox"), COMPONENTTYPES::HITBOX);

                if(sideHit != nullptr){
                    wallSlidable = false;
                    bool solidWall = false;
                    for(unsigned i=0; i<sideHit->getCollisionList().size(); i++){
                        if(sideHit->getCollisionList()[i].name != "Player" && ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject(sideHit->getCollisionList()[i].name), COMPONENTTYPES::HITBOX)->getIsSolid()){
                            if(playerState == 4){
                                //change animation
                                wallSlidable = true;
                            }
                            if(playerState == 5){
                                verticalSpeed = 0;
                                if(hDirection){
                                    horizontalSpeed = -100;
                                }
                                else{
                                    horizontalSpeed = 100;
                                }
                                playerState = 8;
                            }
                            solidWall = true;
                        }
                    }
                    if(!solidWall && playerState == 7){
                        playerState = 4;
                    }
                }
                if(botHit != nullptr){
                    bool fall = true;
                    for(unsigned i=0; i<botHit->getCollisionList().size(); i++){
                        if(botHit->getCollisionList()[i].name != "Player" && ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject(botHit->getCollisionList()[i].name), COMPONENTTYPES::HITBOX)->getIsSolid()){
                            if(playerState == 4 || playerState == 7){
                                //change animation
                                playerState = 1;
                            }
                            else if(playerState == 8){
                                playerState = 11;
                            }
                            else if(playerState == 6){
                                playerState = 10;
                            }
                            fall = false;
                        }
                    }
                    if(fall && (playerState == 1 || playerState == 2)){
                        playerState = 4;
                        jumpsLeft = 1;
                    }
                    //check for all equal heights and then if they are all equal but the state is 4 and hit2 is colliding ground
                    bool allEqual = true;
                    for(unsigned j=1; j<heightValues.size(); j++){
                        if(heightValues[0] != heightValues[j]){
                            allEqual = false;
                            break;
                        }
                    }
                    for(unsigned i=0; i<botHit2->getCollisionList().size(); i++){
                        if(botHit2->getCollisionList()[i].name != "Player" && ScriptsInterface::getComponent<HitboxComponent>(ScriptsInterface::getObject(botHit2->getCollisionList()[i].name), COMPONENTTYPES::HITBOX)->getIsSolid()){
                            if(allEqual && (playerState == 4 || playerState == 7)){
                                playerState = 1;
                                jumpsLeft = maxJumps;
                            }
                            if(allEqual && playerState == 6){
                                playerState = 10;
                            }
                            if(allEqual && playerState == 8){
                                playerState = 11;
                            }
                        }
                    }
                }
                botHit = nullptr;
                sideHit = nullptr;
                botHit2 = nullptr;
            }
        }

        void setLeft(bool l){
            left = l;
        }

        void setRight(bool r){
            right = r;
        }

        void setCrouch(bool c){
            crouching = c;
        }

        void setJumpHold(bool j){
            jumpHolding = j;
        }

        void setJump(bool j){
            jump = j;
        }

        void setDash(bool d){
            dash = d;
        }

        void setAction(bool a){
            action = a;
        }
};

#endif // PLAYERSCRIPT_H
