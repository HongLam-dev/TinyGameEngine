# CMakeList.txt : CMake project for TinyGameEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TinyGameEngine")

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

add_library(TinyGameEngine
    "Engine/Source/TinyGameEngine.cpp"
    "Engine/Source/Input.cpp"
    "Engine/Source/Window.cpp"
    "Engine/Source/Component.cpp"
    "Engine/Source/SpriteRenderer.cpp"
    "Engine/Source/GameObject.cpp"
    "Engine/Source/Transform.cpp"
    "Engine/Source/CollisionManager.cpp"
    "Engine/Source/BoxCollider2D.cpp"
    "Engine/Source/Rigidbody2D.cpp"
    "Engine/Source/Animation.cpp"
    "Engine/Source/Animator.cpp"
    "Engine/Source/Camera.cpp"
    "Engine/Source/UI/UITransform.cpp"
    "Engine/Source/UI/UIObject.cpp"
    "Engine/Source/UI/Image.cpp"
    "Engine/Source/RenderManager.cpp"
    "Engine/Source/Scene.cpp"
    "Engine/Source/TextureManager.cpp"
    "Engine/Header/Vector3.h"
    "Engine/Header/Vector2.h"
)
target_include_directories(TinyGameEngine PUBLIC
    Engine/Header
    Engine/Header/UI
)
target_link_libraries(TinyGameEngine PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
)

# Add source to this project's executable.
add_executable (TinyGame
"Game/main.cpp"
"Game/System/SceneManager.h"
"Game/System/SceneManager.cpp"
"Game/System/SceneBuilder.h"
"Game/System/SceneBuilder.cpp"
"Game/Header/PlayerController.h"
"Game/Source/PlayerController.cpp"
"Game/Header/PingPong.h" 
"Game/Source/PingPong.cpp" 
"Game/Header/CameraFollow.h"
"Game/Source/CameraFollow.cpp")

target_include_directories(TinyGame PRIVATE
    Game/System
    Game/Header
)

target_link_libraries(TinyGame PRIVATE
    TinyGameEngine
)

add_executable(TinyGameEditor
    "Editor/main.cpp"
)
find_package(ImGui-SFML CONFIG REQUIRED)

target_include_directories(TinyGameEditor PRIVATE
    Editor/Header
)

target_link_libraries(TinyGameEditor PRIVATE
    TinyGameEngine
    ImGui-SFML::ImGui-SFML
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
set_property(TARGET TinyGameEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET TinyGame PROPERTY CXX_STANDARD 20)
set_property(TARGET TinyGameEditor PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
