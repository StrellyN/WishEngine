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

#ifndef HEADERS_H
#define HEADERS_H

#define TICKS_PER_SECOND 30

namespace WishEngine{
    enum S_TYPES {INPUT, GRAPHICS, NETWORK, SCRIPTS, PHYSIC, AUDIO, ANIMATIONS, TIMER, COLLISION, STYPESIZE};
    enum M_TYPES {LOADSTATE, NEWSTATE, QUIT, SFRAME, FFRAME, ADDOBJ, RENDER, EVENTS, HANDLEINPUT, FULLSCREEN, MTYPESIZE};
    enum E_TYPES {MOUSE_WHEEL, MOUSE_PRESS, KEYBOARD_PRESS, GAMEPAD_ADDED, GAMEPAD_REMOVED, GAMEPAD_PRESS, GAMEPAD_AXIS, MOUSE_MOTION, MOUSE_RELEASE, KEYBOARD_RELEASE, GAMEPAD_RELEASE, EQUIT, ENULL, ETYPESIZE};
    enum C_TYPES {TIMERC, CAMERA, INPUTC, PLAYER, GRAPHIC, ANIMATION, ANIMATOR, AUDIOC, DIMENTION, HITBOX, PHYSICS, VELOCITY, SCRIPT, GRAVITY, EMITEDFORCE, PUSHABLE, SOLID, UI, PROPERTIES, NETWORKC, CTYPESIZE};
}

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cmath>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <stdint.h>
#include <type_traits>

#endif // HEADERS
