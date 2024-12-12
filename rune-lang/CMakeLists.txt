cmake_minimum_required(VERSION 3.10)
project(RuneLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

# Source files
set(SOURCES
    src/RuneParser.cpp
    src/RuneSystem.cpp
    src/GhostSystem.cpp
)

# Include directories
include_directories(include)

# Create the library
add_library(runelang SHARED ${SOURCES})

# Create test executable
add_executable(rune_test tests/rune_test.cpp)
target_link_libraries(rune_test runelang)
