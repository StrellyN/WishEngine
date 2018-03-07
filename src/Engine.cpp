/**
    Copyright 2018 Strelly

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

#include "Engine.hpp"

namespace WishEngine{
    Engine::Engine(){
        srand(time(NULL)); //Seed for the random number generator, done only once
    }

    Engine::~Engine(){}

    /**
        Main engine loop, it just adds and deletes the states and updates the current one.
    **/
    void Engine::update(){
        getState().update(); //Update the state
    }

    /**
        Method that returns the states vector.
    **/
    State &Engine::getState(){
        return currentState;
    }

    /**
        Method that deletes and sets the states with the new ones passed.
    **/
    void Engine::setState(State &nState){
        currentState = nState;
    }
}
