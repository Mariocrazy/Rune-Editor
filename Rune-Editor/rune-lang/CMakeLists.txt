cmake_minimum_required(VERSION 3.10)
project(RuneLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/RuneParser.cpp
    src/main.cpp
)

# Add header files
set(HEADERS
    include/RuneParser.hpp
)

# Create executable
add_executable(rune_lang ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(rune_lang PRIVATE include)
