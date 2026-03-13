cmake_minimum_required(VERSION 3.31.6)
project(JamGame VERSION 1.0.0)

find_package(OpenGL REQUIRED)

add_executable(JamGame
    src/main.cpp

    src/core/util.cpp
    src/core/util.hpp

    src/core/game.cpp
    src/core/game.hpp
    src/core/gameobject.cpp
    src/core/gameobject.hpp

    src/core/resourcemanager.cpp
    src/core/resourcemanager.hpp
    
    src/core/window.cpp
    src/core/window.hpp
    src/core/spriterenderer.cpp
    src/core/spriterenderer.hpp
    src/core/shader.cpp
    src/core/shader.hpp
    src/core/sprite.cpp
    src/core/sprite.hpp

    src/game/player.cpp
    src/game/player.hpp
    src/game/world.cpp
    src/game/world.hpp
    
    src/native/glglfw.h
    src/native/gl.c
    src/native/stb_image.h
    src/native/stb.cpp
)

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
)
FetchContent_MakeAvailable(yaml-cpp)

target_include_directories(JamGame PRIVATE dependencies)
target_link_directories(JamGame PRIVATE lib)

target_link_libraries(JamGame
    glfw3
    OpenGL::GL
    yaml-cpp::yaml-cpp
)