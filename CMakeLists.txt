cmake_minimum_required(VERSION 3.26)
project(1stDA_Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}")
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_TMP})

# Add the include directories
include_directories(${CMAKE_SOURCE_DIR})

# Define the source files
set(SOURCES
        main.cpp
        Menu.h
        Menu.cpp
        dijkstra.cpp
        dijkstra.h
)

# Define the executable target
add_executable(1stDA_Project ${SOURCES})