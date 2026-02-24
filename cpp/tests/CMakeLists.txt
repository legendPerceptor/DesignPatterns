# Download Google Test if not available
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable Google Test
include(GoogleTest)

# Main test executable with ALL tests
file(GLOB TEST_SOURCES "*Test.cpp")
add_executable(AllTests ${TEST_SOURCES})
target_link_libraries(AllTests GTest::gtest_main DesignPatternExample)
gtest_discover_tests(AllTests)

# Optional: Separate executables by pattern type for targeted testing
file(GLOB CREATIONAL_SOURCES "A*Test.cpp")
add_executable(CreationalTests ${CREATIONAL_SOURCES})
target_link_libraries(CreationalTests GTest::gtest_main DesignPatternExample)
gtest_discover_tests(CreationalTests)

file(GLOB STRUCTURAL_SOURCES "B*Test.cpp")
add_executable(StructuralTests ${STRUCTURAL_SOURCES})
target_link_libraries(StructuralTests GTest::gtest_main DesignPatternExample)
gtest_discover_tests(StructuralTests)

file(GLOB BEHAVIORAL_SOURCES "C*Test.cpp")
add_executable(BehavioralTests ${BEHAVIORAL_SOURCES})
target_link_libraries(BehavioralTests GTest::gtest_main DesignPatternExample)
gtest_discover_tests(BehavioralTests)
