# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(TetrisGame)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SFML Dependency ---
# This project requires the SFML library.
# For CMake to find SFML, you need to either:
#   1. Install SFML system-wide in a standard location.
#   2. Or, set the SFML_DIR CMake variable to the directory containing SFMLConfig.cmake.
#      Example: cmake -DSFML_DIR=/path/to/your/sfml/build/lib/cmake/SFML ..
#
# You can download SFML from: https://www.sfml-dev.org/download.php
find_package(SFML 3.0.2 COMPONENTS Graphics Window System REQUIRED)

# Include the 'include' directory for header files
include_directories(include)

# Add the executable target, linking main.cpp from the src directory
add_executable(Tetris src/main.cpp src/Game.cpp src/Tetromino.cpp)

# Link the SFML libraries to the executable
target_link_libraries(Tetris SFML::Graphics SFML::Window SFML::System)
