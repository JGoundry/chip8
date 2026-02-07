cmake_minimum_required(VERSION 3.31)
project(chip8 LANGUAGES CXX)

# Global settings (want to configure fetched content with these)
set(CMAKE_CXX_STANDARD            23)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

# External libs
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.0
)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(SDL3)
include(GoogleTest)

# Targets
add_executable(emu src/main.cpp)
target_link_libraries(emu PRIVATE SDL3::SDL3)
