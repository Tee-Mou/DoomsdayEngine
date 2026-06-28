cmake_minimum_required(VERSION 3.10)
project(doomsdayEngine CXX)

set(LIB_SOURCES
    src/Utils.cpp
    src/Eval.cpp
    src/Game.cpp
    src/MoveTables.cpp
)
set(LIB_HEADERS
    inc/Utils.h
    inc/Eval.h
    inc/Game.h
    inc/Move.h
    inc/MoveTables.h
)
set(TEST_SOURCES
    tests/runTests.cpp
    tests/src/Tests.cpp
    tests/src/MoveTableTests.cpp
    tests/src/GameTests.cpp
    tests/src/EvalTests.cpp
)

add_library(doomsdayLib ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(doomsdayEngine main.cpp)
target_link_libraries(doomsdayEngine doomsdayLib)

add_executable(doomsdayEngineTests ${TEST_SOURCES})
target_link_libraries(doomsdayEngineTests doomsdayLib)