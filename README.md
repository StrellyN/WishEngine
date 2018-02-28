# Wish Engine

## About

Wish Engine is a (for now) 2D game engine coded in C++ using (for now as well since later on I want to make my own multimedia API) the SDL2 Library.
The engine itself is my last college project and its a modular entity component system, right now isn't possible but in the future I hope to implement a way 
to add systems and components through dlls since right now each time you want to add a system you have to compile the engine.

It works on Linux and Windows, I haven't tested it on Mac since I don't have access to one. It's also free software so feel free to do whatever you want with 
the code.

## Important Stuffs Log

### 28/08/2018

Well hello there, I'm back with big and important changes, as you can see, now there's an "Old Version" folder. It contains the source code that was here until 
this commit, in case I have to go back to it or someone is curious about it.

Now, you may be wondering what's this "new version" about. Well I've been working on an engine restructure, to make it modular and to have the components outside 
the game objects for them to be more cache friendly, don't have them allocated with "new", etc... I also separated all the systems, now they can only talk 
through messages, that means no more direct calls to ObjectFactory or Framework, but thanks to this I'll may be able to make systems as DLLs, so that's cool.

What else... Oh yeah, some systems are gone, like the ScriptSystem and GraphicSystem. GraphicSystem became useless after having everything talk through messages, 
it was going to send a message for each object to render, instead I just made the Framework receive the Render message and do it itself.

ScriptSystem didn't disappear per say, it just became ScriptsInterface, which, as the name implies, acts as the way for scripts to comunicate with the engine and 
as a way to update the scripts and create them for the ObjectFactory.

For now that's it, I still need to convert the rest of the systems to this new form, and then test the DLL thing. If it ends up failing, I'll think about bringing 
the old version back or continue with this new one. See you then!

### 17/02/2018

Networking has been completed, I coded pong with online functionality and it worked fine, so that's just amazing. I still need to fix some things, like keeping 
udp connections synchronized and making tcp a bit more reliable since it's stream based and I'd like to be able to receive more than one packet at a time. Let's 
see if I can do that easily so I can go back to other things like bettering the save state methods for the engine, diving deeper into being able to use more 
than one window, being able to zoom in and out and some other interesting stuff. Although, to be fair, I feel like with the completion of the networking system 
this engine is done, the only thing major I'd like to develop for it is the modularity part, but I've been researching and it looks really hard, if I'm not able 
to do it I'll add a scripting language like LUA so the gameplay can be done without recompiling the engine. Apart from that, there's nothing that major to be added, 
I already mentioned some things some lines before, but I guess I could also develop a particle system and make the physics system an actual physics system. Anyway, 
I'll be looking into all of that.

### 19/01/2018

Had to change the file I/O because I can't have it as template methods since C++ won't let me get a whole line spaces included similar to how ">>" gets a single
data. Now I have a method for each important type, int (you can store bools with this as well), double and string. I guess I could add char as well, but int can
be used for that right now, I'll think about it.
Also finished the input detection in the framework and now you can use game pads and stuff!

### 01/01/2018

This is the first day of the year as well as the first commit for this project, even though I've been working on it for around a month and a half, so 
progress is far, right now I have the basic graphics, inputs, sound, collision, animation, scripting and timer, having physics on the work and networking,
lets say... started (?). I'll keep updating more the repository since I've already created it I won't feel lazy to do it anymore, I'll be adding documentation,
probably making a github page with the classes and everything like unity has, example games when I develop them, configuration files structures since you need 
to make them yourself because there's no editor :(, and more. Anyway, thank you for reading, I hope you find this project useful and if you have any recomendation 
on how to make documentation better and those things, please let me know.