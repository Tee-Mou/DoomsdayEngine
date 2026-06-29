cmake_minimum_required(VERSION 3.10)
project(doomsdayEngine CXX)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.15.0
)
FetchContent_MakeAvailable(Catch2)

set(LIB_SOURCES
    src/Utils.cpp
    src/Eval.cpp
    src/Game.cpp
    src/MoveTables.cpp
    src/Interface.cpp
)
set(LIB_HEADERS
    inc/Utils.h
    inc/Eval.h
    inc/Game.h
    inc/Move.h
    inc/MoveTables.h
    inc/Interface.h
)

set(TEST_SOURCES
    tests/src/MoveTableTests.cpp
    tests/src/GameTests.cpp
    tests/src/EvalTests.cpp
    tests/src/UtilsTests.cpp
)

add_library(doomsdayLib ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(doomsdayEngine main.cpp)
target_link_libraries(doomsdayEngine doomsdayLib)

add_executable(doomsdayEngineTests ${TEST_SOURCES})
target_link_libraries(doomsdayEngineTests PRIVATE doomsdayLib Catch2::Catch2WithMain)