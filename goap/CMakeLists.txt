
project(goap CXX)

cmake_minimum_required(VERSION 3.1)

set(src 
	Action.cpp
	Action.h
	Node.cpp
	Node.h
	Planner.cpp
	Planner.h
	WorldState.cpp
	WorldState.h
    )
    
add_library(goap ${src})
target_compile_options(goap PUBLIC -g3 -O0 -Wall)
target_include_directories(goap PUBLIC .)
