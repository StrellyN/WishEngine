# Wish Engine

## About

Here you can find all the systems I've personaly developed for the engine.

First of all, sorry for not separating all the hpps from the cpps in systems folders, but I couldn't be asked. Okay, moving on...

For each system there's a "DLLInterface.cpp" which defines and implements a function and exports it so it can be accessed from outside the DLL. The only 
thing it does is return a pointer to a new system for the main engine to save and access.

## Current Systems

### Framework

It does everything multimedia related from playing sound and sending each frame inputs through message, to rendering and networking. It's basically a 
layer between the engine and the multimedia library SDL2. It requieres SDL2, but don't worry, it's the only system that requieres an external library.

### ObjectFactory

Stores the objects and components and sends messages each frame with pointer to both containers so other systems may have direct access to them.

### Scripts Interface

This system is the one in charge of creating the scripts and inserting them into the components collections of the Object Factory and of calling the 
execute method of each script each frame. It also provides the methods that let custom scripts access functionality from the engine like creating objects, 
components, setting the FPS, etc...

### Animation System

Using the Animator Component, it sets the position of the Animation Component so the Framework knows where to cut the texture each frame and create the 
animation from a sprite sheet.

### Audio System

It manages all the Audio Components and the dynamic volume checking the position of objects to apply the correct volume to the sound or music. It also keeps 
track of the song timer to be able to crossfade between songs.

### Collision System

Checks if two objects are colliding with the Dimension Component and the Hitbox Component, if they are, it checks some flags in the Hitbox Component and 
add the corresponging objects to the others Collision List for others to be able to check if they are colliding.

### Input System

It gets the inputs from the Framework via message, and gives them to all the Input Components, it also checks for general inputs that may affect the engine 
like the QUIT input, and if it detects it, it sends a message with information regarding what happened. Right now it only needs to send a message for QUIT 
though and that makes the engine shut down.

### Network System 

Through the Network Components, it makes it easier for the user to create and delete TCP/UDP connections and to send and receive data through those connections. 
It communicates with the Framework via messages to access the SDL_Net functionality.

### Physics System

It does nothing right now.

### Timer System

It's a simple timer that multiplies 1 * deltaTime and adds it to the counter of every Timer Component. Nothing special.