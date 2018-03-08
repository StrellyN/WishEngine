#ifndef MAPMANAGERSCRIPT_H
#define MAPMANAGERSCRIPT_H

/**
	You can add attributes both in private(recommended) and public,
	but remember to create getters and setters if you do it in private.
**/

using namespace WishEngine;

class MapManagerScript : public ScriptComponent{
    private:
        std::vector<std::string> objectNames;
        bool loaded = false;
        unsigned tileSize = 32;

    public:
		/**
			Initialization code here. This method will execute when the
			script is created in an object.
		**/
        MapManagerScript(){

        }

		/**
			Destruction code here, remember to delete your pointers if this
			component has the ownership, example, a component created in
			this script. If you got a system, an object or a component via
			the game state, you probably don't want to delete it.
			This method will execute when the object that this is attached to
			is deleted, or if the component is deleted manualy.
		**/
        ~MapManagerScript(){
            objectNames.clear();
        }

		/**
			Execution code here. This method will execute every logic update.
		**/
        void execute(GameObject &thisObj, double deltaT){
            if(!loaded){
                loadMap("data/maps/test.map");
                loaded = true;
            }
        }

        void unloadCurrentMap(){
            for(unsigned i=0; i<objectNames.size(); i++){
                ScriptsInterface::deleteObject(objectNames[i]);
            }
            objectNames.clear();
        }

        void loadMap(std::string configFile){
            unloadCurrentMap();
            std::fstream stream(configFile, std::ios::in);
            //Terrain
            std::string tileSet, objectsRoute;
            unsigned nObjects, objectId;
            unsigned currentObjectsInFactory = ScriptsInterface::getObjectQuantity(), endingObjectsInFactory;
            int currentX = 0, currentY = 0;
            bool tiled;
            stream >> tileSet;
            stream >> tiled;
            stream >> nObjects;
            if(tiled){
                stream >> currentX;
                stream >> currentY;
            }
            for(unsigned i=0; i<nObjects; i++){
                //load all the terrain objects
                if(tiled){
                    //Don't load position
                    stream >> objectId;
                    if(objectId != 0 ){
                        if(objectId != 2){
                            ScriptsInterface::insertObject("Tile", true);
                            GameObject *aux = ScriptsInterface::getObjectByPos(ScriptsInterface::getObjectQuantity() - 1);
                            if(aux != nullptr){
                                std::string objName = aux->getName();
                                ScriptsInterface::insertComponent<GraphicComponent>(objName, GraphicComponent(false, tileSet, 255, 25));
                                ScriptsInterface::insertComponent<AnimationComponent>(objName, AnimationComponent(32, 0, 32, 32));
                                ScriptsInterface::insertComponent<DimensionComponent>(objName, DimensionComponent(currentX, currentY, tileSize, tileSize));
                                ScriptsInterface::insertComponent<HitboxComponent>(objName, HitboxComponent(0, 0, tileSize, tileSize, true, false));
                            }
                            aux = nullptr;
                        }
                    }
                    currentX += tileSize;
                    if(currentX >= 80*tileSize){
                        currentX = 0;
                        currentY += tileSize;
                    }

                }
                else{
                    //Load position
                }
            }
            //stream >> objectsRoute;
            //ObjectFactory::getObjectFactory()->createObjects(objectsRoute);
            stream.close();
            endingObjectsInFactory = ScriptsInterface::getObjectQuantity();
            //Save the names of the objects between currentobjects and endingobjects
        }
};

#endif // MAPMANAGERSCRIPT_H
