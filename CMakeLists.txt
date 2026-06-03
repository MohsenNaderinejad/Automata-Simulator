cmake_minimum_required(VERSION 3.10)

project(Theory_of_Computation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(dfa_simulator
    src/DFA/main.cpp
    src/DFA/DFA.cpp
    src/GLOBAL_VARIABLES.cpp
)

add_executable(dpda_simulator
    src/DPDA/main.cpp
    src/DPDA/DPDA.cpp
    src/GLOBAL_VARIABLES.cpp
)

target_include_directories(dfa_simulator PRIVATE include)
target_include_directories(dpda_simulator PRIVATE include)