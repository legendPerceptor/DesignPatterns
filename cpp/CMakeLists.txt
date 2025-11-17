cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 17)
project(DesignPatternDemo VERSION 1.0)

add_library(DesignPatternExample designPatternExamples.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} DesignPatternExample)