cmake_minimum_required(VERSION 3.10)
project(doomsdayEngine)

set(CMAKE_CXX_STANDARD 17)
SET(engineName doomsdayEngine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(doomsdayEngine
    src/main.cpp
    )