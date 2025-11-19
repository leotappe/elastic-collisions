cmake_minimum_required(VERSION 3.24) # CMP0135

project(demo)

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
)

FetchContent_MakeAvailable(raylib)

add_subdirectory(demo)