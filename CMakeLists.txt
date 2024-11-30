cmake_minimum_required(VERSION 3.10)
# Project name and version
project(PricingLibrary VERSION 1.0)

# Enable C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(Threads REQUIRED)

# Add spdlog (using FetchContent)
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/client/include
)

# Gather source files (excluding main.cpp)
file(GLOB_RECURSE LIB_SOURCES 
    "src/models/*.cpp"
    "src/utils/*.cpp"
    "src/server/*.cpp"
    "client/src/*.cpp"
)

# Create a static library with all the source files
add_library(PricingLib STATIC ${LIB_SOURCES})
target_link_libraries(PricingLib PRIVATE spdlog::spdlog)

# Add the main executable
add_executable(PricingServer src/main.cpp)
target_link_libraries(PricingServer PRIVATE PricingLib spdlog::spdlog)

# Add test executable
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(PricingTests ${TEST_SOURCES})
target_link_libraries(PricingTests 
    PRIVATE 
    PricingLib
    gtest 
    gtest_main 
    Threads::Threads
    spdlog::spdlog
)

# Add testing
enable_testing()
add_test(NAME AllTests COMMAND PricingTests)

# Set compiler flags
if(MSVC)
    target_compile_options(PricingLib PRIVATE /W4)
    target_compile_options(PricingServer PRIVATE /W4)
    target_compile_options(PricingTests PRIVATE /W4)
else()
    target_compile_options(PricingLib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(PricingServer PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(PricingTests PRIVATE -Wall -Wextra -Wpedantic)
endif()
