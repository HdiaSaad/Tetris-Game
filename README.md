# C++ Tetris Game

This is a Tetris game developed in C++ using the SFML (Simple and Fast Multimedia Library) for graphics and multimedia. The project is built with CMake, targeting a full-featured Tetris experience with progressive levels, scoring, and a responsive UI.

## Features

*   **Classic Tetris Gameplay:** Core mechanics of falling blocks, movement, and rotation.
*   **Collision Detection:** Accurate detection for block placement and rotations.
*   **Line Clearing:** Automatically clears full lines and updates score.
*   **Dynamic Scoring:** 20 points per line cleared.
*   **Progressive Leveling:** Level-ups occur every 100 points, increasing game speed.
*   **Visual Level Transitions:** Smooth background color transitions upon level changes to indicate progression.
*   **Responsive UI:** Adjusted grid size and UI element positioning for a better visual experience.
*   **Main Menu:** "Start" and "Close" options.
*   **Game Over Screen:** Displays "Game Over!" message, score, and level.

## Building and Running

### Prerequisites

To build and run this project, you will need:

1.  **A C++ Compiler:** (e.g., MSVC on Windows, GCC/Clang on Linux/macOS).
2.  **CMake:** Version 3.10 or higher.
3.  **SFML Library:** Version 3.0.2. Ensure it's installed and accessible to CMake. For Windows, this typically means having the SFML SDK unzipped and providing the path to its `lib/cmake/SFML` directory during CMake configuration.

### Build Instructions

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/HdiaSaad/Tetris-Game.git
    cd Tetris-Game
    ```
2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```
3.  **Configure CMake:**
    Replace `C:\Path\To\SFML-3.0.2\lib\cmake\SFML` with the actual path to your SFML CMake files.
    ```bash
    cmake -DSFML_DIR="C:\Path\To\SFML-3.0.2\lib\cmake\SFML" ..
    ```
    *   **Note for Linux/macOS:** If SFML is installed system-wide (e.g., via package manager), you might not need `-DSFML_DIR`. `cmake ..` might suffice.
4.  **Compile the project:**
    ```bash
    cmake --build .
    ```

### Running the Game

After a successful build, the executable will be located in the `build/Debug` or `build/Release` directory (depending on your build configuration).

```bash
# From within the build directory
./Debug/Tetris.exe
# or
./Release/Tetris.exe
```

## Contributing

Feel free to fork the repository, make improvements, and submit pull requests.

## License

(No explicit license specified yet.)
