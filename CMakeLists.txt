cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PricingLibrary VERSION 1.0)

# Enable C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(include client/include)

# Gather source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

# Add executable for the main application
add_executable(PricingServer ${SOURCES})

# Link libraries for gtest
find_package(Threads REQUIRED)

# Add gtest-based tests
add_executable(PricingTests ${TEST_SOURCES})
target_link_libraries(PricingTests gtest gtest_main Threads::Threads)

# Add testing
enable_testing()
add_test(NAME AllTests COMMAND PricingTests)

