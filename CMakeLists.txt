cmake_minimum_required(VERSION 3.31.6)
project(JamGame VERSION 1.0.0)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "src/*.h")
add_executable(JamGame ${SOURCES})

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
)
FetchContent_MakeAvailable(yaml-cpp)

target_include_directories(JamGame PRIVATE dependencies)
target_include_directories(JamGame PRIVATE src)

target_link_directories(JamGame PRIVATE lib)

target_link_libraries(JamGame
    glfw3
    OpenGL::GL
    yaml-cpp::yaml-cpp
)