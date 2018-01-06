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

int main(int argc, char *argv[]){
    WishEngine::Engine e;
    e.update();
    return 0;
}

/**
    TO DO:
        General:
            - Multiple windows handling and events (SDL_GetWindowID for event window comparing)
            - Basic particle system.
            - Zoom: I can use SDL_RenderSetScale(renderer, xFactor, yFactor); to zoom
            - Be able to change scaling mode ("nearest", "smooth")
            - Fix get component crash?. Weird bug with inserting objects turning components count to 0?
              (Memory address for components changes)
            - Make objects attachable to other object so they follow the parent object
            - Make barriers for camera movement? (Maybe make the camera unable to leave a rectangle or something like that)
            - Implement space partition
            - Implement system dependencies
            - Add a properties component that just has a string vector with the properties of the object.

        Look into:
            - Being able to add custom c++ scripts components just by creating it and using the engine library,
              like unity does, dll linking chains.
            - To save a state just save the objects in the object factory in another vector or something, and then
              restore them, that simple (Need to create a copy constructor for the components or find a way to copy them).

        If things get too bad performance wise:
            - In the update part of the systems, instead of updating each object in each system, pass the objects that has the
            component for that system only, so make the object for loop in the engine and pass the object to the system.
            Changing game loop to contain a for loop that goes through all the objects and pass each object to the
            desired systems, something like: for each component needed in system{ if object has all those components { send } }

        When multiple windows becomes a real thing:
            - Multiple windows event handling check https://wiki.libsdl.org/SDL_WindowEvent
            - Attach the UI component to a window and a camera? Or multiple ones using std::maps?
**/
