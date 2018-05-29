COMPILER = g++
SOFLAGS = -shared -o
SOLFLAGS = -fPIC
BRFLAGS = -ldl
CRFLAGS = -O3 -std=c++11
LIBFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net

all: Wish Frame Object Scripts Animation Audio Collision Input Network Timer

Wish: WishEngine/main.cpp WishEngine/Engine.cpp WishEngine/State.cpp CommonIncludes/Message.cpp
	$(COMPILER) $? $(CRFLAGS) $(BRFLAGS) -o WishEngine/WishEngine

Frame: Framework/DLLInterface.cpp Framework/Framework.cpp CommonIncludes/AnimationComponent.cpp CommonIncludes/AudioMessage.cpp CommonIncludes/CameraComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/CreateWindowMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp CommonIncludes/Event.cpp CommonIncludes/GameObject.cpp CommonIncludes/GraphicComponent.cpp CommonIncludes/InputListMessage.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp CommonIncludes/NetworkMessage.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp CommonIncludes/TextComponent.cpp CommonIncludes/VectorMessage.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libFramework.so $(SOLFLAGS) $? $(CRFLAGS) $(LIBFLAGS) 

Object: CommonIncludes/AnimationComponent.cpp CommonIncludes/AnimationState.cpp CommonIncludes/AnimatorComponent.cpp CommonIncludes/AudioComponent.cpp CommonIncludes/AvailableObjectsMessage.cpp CommonIncludes/CameraComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/CreateScriptMessage.cpp CommonIncludes/CreateWindowMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp ObjectFactory/DLLInterface.cpp CommonIncludes/Event.cpp CommonIncludes/GameObject.cpp CommonIncludes/GraphicComponent.cpp CommonIncludes/HitboxComponent.cpp CommonIncludes/InputComponent.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp ObjectFactory/ObjectFactory.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/PhysicsComponent.cpp CommonIncludes/PropertiesComponent.cpp CommonIncludes/Rectangle.cpp CommonIncludes/TextComponent.cpp CommonIncludes/TimerComponent.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libObjectFactory.so $(SOLFLAGS) $? $(CRFLAGS) 

Scripts: CommonIncludes/AnimationComponent.cpp CommonIncludes/AnimationState.cpp CommonIncludes/AnimatorComponent.cpp CommonIncludes/AudioComponent.cpp CommonIncludes/AvailableObjectsMessage.cpp CommonIncludes/CameraComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/CreateScriptMessage.cpp CommonIncludes/CreateWindowMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp ScriptsInterface/DLLInterface.cpp CommonIncludes/Event.cpp CommonIncludes/GameObject.cpp CommonIncludes/GraphicComponent.cpp CommonIncludes/HitboxComponent.cpp CommonIncludes/InputComponent.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp CommonIncludes/NetworkMessage.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/PhysicsComponent.cpp CommonIncludes/PropertiesComponent.cpp CommonIncludes/Rectangle.cpp ScriptsInterface/ScriptsInterface.cpp CommonIncludes/TextComponent.cpp CommonIncludes/TimerComponent.cpp CommonIncludes/VectorMessage.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libScriptsInterface.so $(SOLFLAGS) $? $(CRFLAGS) 

Animation: CommonIncludes/AnimationComponent.cpp CommonIncludes/AnimationState.cpp AnimationSystem/AnimationSystem.cpp CommonIncludes/AnimatorComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp AnimationSystem/DLLInterface.cpp CommonIncludes/GameObject.cpp CommonIncludes/Message.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libAnimationSystem.so $(SOLFLAGS) $? $(CRFLAGS) 

Audio: CommonIncludes/AudioComponent.cpp CommonIncludes/AudioMessage.cpp AudioSystem/AudioSystem.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp AudioSystem/DLLInterface.cpp CommonIncludes/GameObject.cpp CommonIncludes/Message.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libAudioSystem.so $(SOLFLAGS) $? $(CRFLAGS) 

Collision: CollisionSystem/CollisionSystem.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp CollisionSystem/DLLInterface.cpp CommonIncludes/GameObject.cpp CommonIncludes/HitboxComponent.cpp CommonIncludes/Message.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libCollisionSystem.so $(SOLFLAGS) $? $(CRFLAGS) 

Input: CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp InputSystem/DLLInterface.cpp CommonIncludes/Event.cpp CommonIncludes/InputComponent.cpp CommonIncludes/InputListMessage.cpp CommonIncludes/InputMessage.cpp InputSystem/InputSystem.cpp CommonIncludes/Message.cpp CommonIncludes/Rectangle.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libInputSystem.so $(SOLFLAGS) $? $(CRFLAGS) 

Network: CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp NetworkSystem/DLLInterface.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp CommonIncludes/NetworkMessage.cpp NetworkSystem/NetworkSystem.cpp CommonIncludes/Rectangle.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libNetworkSystem.so $(SOLFLAGS) $? $(CRFLAGS) 

Timer: CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp TimerSystem/DLLInterface.cpp CommonIncludes/Message.cpp CommonIncludes/Rectangle.cpp CommonIncludes/TimerComponent.cpp TimerSystem/TimerSystem.cpp
	$(COMPILER) $(SOFLAGS) WishEngine/systems/libTimerSystem.so $(SOLFLAGS) $? $(CRFLAGS) 
