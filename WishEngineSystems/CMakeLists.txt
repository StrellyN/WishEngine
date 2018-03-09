cmake_minimum_required(VERSION 3.5)
project(WishEngine CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY WishEngine/)
add_executable(WishEngine WishEngine/main.cpp WishEngine/Engine.cpp WishEngine/State.cpp CommonIncludes/Message.cpp)
target_compile_options(WishEngine PRIVATE -O3 -std=c++11)
target_link_libraries(WishEngine ${CMAKE_DL_LIBS})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(Framework SHARED Framework/DLLInterface.cpp Framework/Framework.cpp CommonIncludes/AnimationComponent.cpp CommonIncludes/AudioMessage.cpp CommonIncludes/CameraComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/CreateWindowMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp CommonIncludes/Event.cpp CommonIncludes/GameObject.cpp CommonIncludes/GraphicComponent.cpp CommonIncludes/InputListMessage.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp CommonIncludes/NetworkMessage.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp CommonIncludes/TextComponent.cpp)
target_compile_options(Framework PRIVATE -fPIC -O3 -std=c++11)
target_link_libraries(Framework "-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(ObjectFactory SHARED CommonIncludes/AnimationComponent.cpp CommonIncludes/AnimationState.cpp CommonIncludes/AnimatorComponent.cpp CommonIncludes/AudioComponent.cpp CommonIncludes/AvailableObjectsMessage.cpp CommonIncludes/CameraComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/CreateScriptMessage.cpp CommonIncludes/CreateWindowMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp ObjectFactory/DLLInterface.cpp CommonIncludes/Event.cpp CommonIncludes/GameObject.cpp CommonIncludes/GraphicComponent.cpp CommonIncludes/HitboxComponent.cpp CommonIncludes/InputComponent.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp ObjectFactory/ObjectFactory.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/PhysicsComponent.cpp CommonIncludes/PropertiesComponent.cpp CommonIncludes/Rectangle.cpp CommonIncludes/TextComponent.cpp CommonIncludes/TimerComponent.cpp)
target_compile_options(ObjectFactory PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(ScriptsInterface SHARED CommonIncludes/AnimationComponent.cpp CommonIncludes/AnimationState.cpp CommonIncludes/AnimatorComponent.cpp CommonIncludes/AudioComponent.cpp CommonIncludes/AvailableObjectsMessage.cpp CommonIncludes/CameraComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/CreateScriptMessage.cpp CommonIncludes/CreateWindowMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp ScriptsInterface/DLLInterface.cpp CommonIncludes/Event.cpp CommonIncludes/GameObject.cpp CommonIncludes/GraphicComponent.cpp CommonIncludes/HitboxComponent.cpp CommonIncludes/InputComponent.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp CommonIncludes/NetworkMessage.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/PhysicsComponent.cpp CommonIncludes/PropertiesComponent.cpp CommonIncludes/Rectangle.cpp ScriptsInterface/ScriptsInterface.cpp CommonIncludes/TextComponent.cpp CommonIncludes/TimerComponent.cpp)
target_compile_options(ScriptsInterface PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(InputSystem SHARED CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp InputSystem/DLLInterface.cpp CommonIncludes/Event.cpp CommonIncludes/InputComponent.cpp CommonIncludes/InputListMessage.cpp CommonIncludes/InputMessage.cpp InputSystem/InputSystem.cpp CommonIncludes/Message.cpp CommonIncludes/Rectangle.cpp)
target_compile_options(InputSystem PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(AnimationSystem SHARED CommonIncludes/AnimationComponent.cpp CommonIncludes/AnimationState.cpp AnimationSystem/AnimationSystem.cpp CommonIncludes/AnimatorComponent.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp AnimationSystem/DLLInterface.cpp CommonIncludes/GameObject.cpp CommonIncludes/Message.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp)
target_compile_options(AnimationSystem PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(AudioSystem SHARED CommonIncludes/AudioComponent.cpp CommonIncludes/AudioMessage.cpp AudioSystem/AudioSystem.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp AudioSystem/DLLInterface.cpp CommonIncludes/GameObject.cpp CommonIncludes/Message.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp)
target_compile_options(AudioSystem PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(CollisionSystem SHARED CollisionSystem/CollisionSystem.cpp CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DimensionComponent.cpp CommonIncludes/DirectionVector.cpp CollisionSystem/DLLInterface.cpp CommonIncludes/GameObject.cpp CommonIncludes/HitboxComponent.cpp CommonIncludes/Message.cpp CommonIncludes/ObjectListMessage.cpp CommonIncludes/Rectangle.cpp)
target_compile_options(CollisionSystem PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(NetworkSystem SHARED CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp NetworkSystem/DLLInterface.cpp CommonIncludes/Message.cpp CommonIncludes/NetworkComponent.cpp CommonIncludes/NetworkMessage.cpp NetworkSystem/NetworkSystem.cpp CommonIncludes/Rectangle.cpp)
target_compile_options(NetworkSystem PRIVATE -fPIC -O3 -std=c++11)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY WishEngine/systems)
add_library(TimerSystem SHARED CommonIncludes/Color.cpp CommonIncludes/ComponentListMessage.cpp CommonIncludes/DirectionVector.cpp TimerSystem/DLLInterface.cpp CommonIncludes/Message.cpp CommonIncludes/Rectangle.cpp CommonIncludes/TimerComponent.cpp TimerSystem/TimerSystem.cpp)
target_compile_options(TimerSystem PRIVATE -fPIC -O3 -std=c++11)
