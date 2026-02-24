cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 17)
project(DesignPatternDemo VERSION 1.0)

# Enable testing BEFORE adding subdirectories
enable_testing()
set(BUILD_TESTING ON CACHE BOOL "Build tests" FORCE)

add_subdirectory(cpp)
