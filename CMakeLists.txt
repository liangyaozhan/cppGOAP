
project(goap CXX)

cmake_minimum_required(VERSION 3.1)

add_subdirectory(goap goap)


add_executable(wargame wargame_example/main.cpp)

target_link_libraries(wargame PUBLIC goap)





add_executable(weapon weapon_example/main.cpp)

target_link_libraries(weapon PUBLIC goap)


