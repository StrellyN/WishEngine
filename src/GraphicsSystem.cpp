/**
    Copyright 2017 Strelly

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
**/

#include "HppHeaders.hpp"

namespace WishEngine{
    GraphicsSystem::GraphicsSystem(){
        setSystemType(S_TYPES::GRAPHICS);
    }

    /**
        Going to clarify this here because it's seen through out the engine some times.
        I have a separate destruction function for some objects to avoid the destruction
        of the pointers objects they are holding when making a copy of the object and then
        deleting it, in case there needs to be a copy.
        This way, only the true owner of the objects will destroy everything it holds.
    **/
    GraphicsSystem::~GraphicsSystem(){
        destroySystem();
    }

    /**
        Method that updates the logic.
        In this case it just calls the cameras update, which call their behaviour scripts.
    **/
    void GraphicsSystem::update(double dt){

    }

    /**
        Method to determine if an object is greater than other in terms of render priority.
        A lower priority will make the graphic system render that camera first, being behind of the next ones.
    **/
    bool objectGraphicSorting(GameObject &a, GameObject &b){
        GraphicComponent* grA = dynamic_cast<GraphicComponent*>(a.getComponent(C_TYPES::GRAPHIC));
        GraphicComponent* grB = dynamic_cast<GraphicComponent*>(b.getComponent(C_TYPES::GRAPHIC));
        bool result;
        if(grA == nullptr && grB == nullptr){
            return false;
        }
        if(grA == nullptr && grB != nullptr){
            result = false;
        }
        if(grB == nullptr && grA != nullptr){
            result = true;
        }
        if(grA != nullptr && grB != nullptr){
            result = grA->getPriority() < grB->getPriority();
        }
        return result;
    }

    /**
        Method that renders all the objects after sorting them by render priority.
    **/
    void GraphicsSystem::renderObjects(double interpolation){
        std::vector<GameObject> &objs = ObjectFactory::getObjectFactory()->getObjects(); //Gets the objects
        if(!std::is_sorted(objs.begin(), objs.end(), objectGraphicSorting)){ //If they are not sorted, sort them.
            std::sort(objs.begin(), objs.end(), objectGraphicSorting);
        }
        std::vector<Camera> &cameras = ObjectFactory::getObjectFactory()->getCameras();
        for(unsigned j=0; j<cameras.size(); j++){ //Renders every object in every camera.
            if(cameras[j].getEnabled()){
                for(unsigned i=0; i<objs.size(); i++){
                    if(objs[i].getEnabled() && objs[i].hasComponent(C_TYPES::GRAPHIC) && objs[i].hasComponent(C_TYPES::DIMENTION)){
                        GraphicComponent *graph = dynamic_cast<GraphicComponent*>(objs[i].getComponent(C_TYPES::GRAPHIC));
                        if(graph != nullptr){
                            if(graph->getIsText()){
                                if(graph->getText().getIsPlain()){
                                    Framework::getFramework()->renderPlainText(objs[i], interpolation, &cameras[j], "mainWindow");
                                }
                                else{
                                    Framework::getFramework()->renderText(objs[i], interpolation, &cameras[j], "mainWindow");
                                }
                            }
                            else{
                                Framework::getFramework()->render(objs[i], interpolation, &cameras[j], "mainWindow");
                            }
                        }
                        graph = nullptr;
                    }
                }
                /**
                    This part down here is for not having to set a new position to the cameras each frame for them to not do
                    the shake if you pause or whatever, it's been done with objects in the render function too.

                    It just updates the last position to the new one calculated with the interpolation, that way, even when
                    the object/camera is not receiving position updates, the last position is going to catch up with the current
                    one, avoiding the "shake" effect of rendering the camera/objects in the last position and current one, when
                    they are too far apart.
                **/

                DimentionComponent* camDim = dynamic_cast<DimentionComponent*>(cameras[j].getComponent(C_TYPES::DIMENTION));
                camDim->setX((nearbyint(camDim->getX()*(interpolation) + camDim->getpX()*(1-interpolation))));
                camDim->setX(camDim->getpX());
                camDim->setY((nearbyint(camDim->getY()*(interpolation) + camDim->getpY()*(1-interpolation))));
                camDim->setY(camDim->getpY());
                camDim = nullptr;
            }
        }
    }

    /**
        Method to start the frame.
    **/
    void GraphicsSystem::startFrame(){
        Framework::getFramework()->startFrame();
    }

    /**
        Method to finish the frame.
    **/
    void GraphicsSystem::finishFrame(){
        Framework::getFramework()->endFrame();
    }

    /**
        Method to handle received messages.
    **/
    void GraphicsSystem::handleMessage(Message *msg){
        switch(msg->getType()){
            case M_TYPES::SFRAME:
                startFrame();
                break;
            case M_TYPES::FFRAME:
                finishFrame();
                break;
            case M_TYPES::RENDER:
                RenderMessage* rmes = dynamic_cast<RenderMessage*>(msg);
                if(rmes != nullptr){
                    renderObjects(rmes->getInterpolation());
                }
                break;
        }
    }
}
