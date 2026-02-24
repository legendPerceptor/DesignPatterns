add_library(DesignPatternExample designPatternExamples.cpp C9State.cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} DesignPatternExample)