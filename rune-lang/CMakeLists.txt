cmake_minimum_required(VERSION 3.18)
project(RuneLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pthread
find_package(Threads REQUIRED)

# Add library
add_library(runelang SHARED
    src/RuneParser.cpp
    src/RuneSystem.cpp
    src/GhostSystem.cpp
    src/GhostTerminal.cpp
)

target_include_directories(runelang PUBLIC include)
target_link_libraries(runelang PRIVATE Threads::Threads)

# Add test executable
add_executable(rune_test tests/rune_test.cpp)
target_link_libraries(rune_test runelang Threads::Threads)

# Add terminal executable
add_executable(ghost_terminal src/ghost_terminal_main.cpp)
target_link_libraries(ghost_terminal runelang Threads::Threads)

# Set compile options
target_compile_options(runelang PRIVATE -Wall -Wextra)
target_compile_options(rune_test PRIVATE -Wall -Wextra)
target_compile_options(ghost_terminal PRIVATE -Wall -Wextra)
