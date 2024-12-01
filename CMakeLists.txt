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
find_package(GTest REQUIRED)

# Add spdlog (using FetchContent)
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# Define source files explicitly
set(MODEL_SOURCES
    src/models/BermudanOption.cpp
    src/models/BinomialTree.cpp
    src/models/BlackScholes.cpp
    src/models/FiniteDifference.cpp
    src/models/LeastSquaresMonteCarlo.cpp
    src/models/MonteCarlo.cpp
)

set(UTILS_SOURCES
    src/utils/Logger.cpp
    src/utils/MathUtils.cpp
)

set(SERVER_SOURCES
    src/server/Server.cpp
)

set(CLIENT_SOURCES
    client/src/Client.cpp
)

# Create the main library
add_library(PricingLib STATIC
    ${MODEL_SOURCES}
    ${UTILS_SOURCES}
    ${SERVER_SOURCES}
    ${CLIENT_SOURCES}
)

# Set include directories for the library
target_include_directories(PricingLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/client/include
    PRIVATE
        ${spdlog_SOURCE_DIR}/include
)

# Link the library with spdlog
target_link_libraries(PricingLib
    PUBLIC
        spdlog::spdlog
)

# Add the main executable
add_executable(PricingServer src/main.cpp)
target_link_libraries(PricingServer
    PRIVATE
        PricingLib
)

# Add the server executable
add_executable(ServerExecutable src/server/ServerMain.cpp)
target_link_libraries(ServerExecutable
    PRIVATE
        PricingLib
)

# Add test executable
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(PricingTests ${TEST_SOURCES})

# Set include directories for tests
target_include_directories(PricingTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/client/include
        ${GTEST_INCLUDE_DIRS}
)

# Link test executable
target_link_libraries(PricingTests
    PRIVATE
        PricingLib
        GTest::GTest
        GTest::Main
        Threads::Threads
)

# Add testing
enable_testing()
add_test(NAME AllTests COMMAND PricingTests)

# Set compiler flags
if(MSVC)
    target_compile_options(PricingLib PRIVATE /W4)
    target_compile_options(PricingServer PRIVATE /W4)
    target_compile_options(ServerExecutable PRIVATE /W4)
    target_compile_options(PricingTests PRIVATE /W4)
else()
    target_compile_options(PricingLib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(PricingServer PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(ServerExecutable PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(PricingTests PRIVATE -Wall -Wextra -Wpedantic)
endif()
