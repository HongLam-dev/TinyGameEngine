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

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# Add source to this project's executable.
add_executable (TinyGameEngine "Game/main.cpp" 
"Engine/Source/TinyGameEngine.cpp"
"Engine/Source/Input.cpp" 
"Engine/Source/Window.cpp" "Engine/Source/Component.cpp" "Engine/Header/SpriteRenderer.h" "Engine/Source/SpriteRenderer.cpp" "Engine/Header/GameObject.h" "Engine/Source/GameObject.cpp" "Engine/Header/Transform.h" "Engine/Source/Transform.cpp" "Engine/Header/Vector3.h" "Game/Header/PlayerController.h" "Game/Source/PlayerController.cpp" "Engine/Header/CollisionManager.h" "Engine/Source/CollisionManager.cpp" "Engine/Header/BoxCollider2D.h" "Engine/Source/BoxCollider2D.cpp" "Engine/Header/Bounds.h" "Editor/Gizmo.h" "Engine/Header/Rigidbody2D.h" "Engine/Source/Rigidbody2D.cpp" "Engine/Header/EngineSettings.h" "Engine/Header/IComponentObserver.h")

target_include_directories(TinyGameEngine PRIVATE
    Engine/Header
    Game/Header
)

target_link_libraries(TinyGameEngine PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TinyGameEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
