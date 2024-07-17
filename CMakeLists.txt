cmake_minimum_required(VERSION 3.28)
project(spread_pricing)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(include)
add_library(spread_pricing STATIC ${SOURCES})

# Get Gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add the test files
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

# Create an executable for the tests
add_executable(runTests ${TEST_SOURCES})

target_include_directories(spread_pricing PUBLIC include)

# Link the GoogleTest and library to the test executable
target_link_libraries(runTests gtest gtest_main spread_pricing)

# Add tests
include(GoogleTest)
gtest_discover_tests(runTests)