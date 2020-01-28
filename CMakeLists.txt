cmake_minimum_required(VERSION 3.10)

project(cnake)

add_executable(cnake cnake.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)

target_link_libraries(cnake ${CURSES_LIBRARIES})

target_link_libraries(cnake pthread)