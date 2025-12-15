#include "Game.h"
#include <iostream>
#include <random> // For random number generation
#include <string> // For std::to_string
#include <algorithm> // For std::max

// Define a set of colors for level transitions
const std::vector<sf::Color> LevelColors = {
    sf::Color(0, 0, 0),       // Level 1: Black
    sf::Color(50, 50, 150),   // Level 2: Dark Blue
    sf::Color(150, 50, 50),   // Level 3: Dark Red
    sf::Color(50, 150, 50),   // Level 4: Dark Green
    sf::Color(150, 150, 50),  // Level 5: Dark Yellow
    sf::Color(50, 150, 150),  // Level 6: Dark Cyan
    sf::Color(150, 50, 150),  // Level 7: Dark Magenta
    sf::Color(100, 100, 100), // Level 8: Dark Gray
    sf::Color(20, 20, 70),    // Level 9: Deeper Blue
    sf::Color(70, 20, 20)     // Level 10: Deeper Red
    // Add more colors if needed for higher levels, they will cycle
};

sf::Font Game::loadFont(const std::string& fontPath) {
    sf::Font font;
    if (!font.openFromFile(fontPath)) {
        std::cerr << "Failed to load font from " << fontPath << std::endl;
        exit(1); // Or a more graceful exit
    }
    return font;
}

Game::Game() 
    : mWindow(sf::VideoMode({400, 500}), "Tetris"), // New window size
      mState(MENU),
      mFont(loadFont("C:/Windows/Fonts/arial.ttf")),
      mMenuText_Start(mFont, "Start", 50),
      mMenuText_Close(mFont, "Close", 50),
      mGrid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0)),
      mFallTime(sf::seconds(0.5f)), // Initial fall speed
      mTimeSinceLastFall(sf::Time::Zero),
      mScore(0),
      mLevel(1),
      mPreviousLevel(1), // Initialize mPreviousLevel to 1
      mLinesCleared(0),
      mPointsToNextLevel(100), // Initialize mPointsToNextLevel to 100 for the first level up
      mScoreText(mFont), // Initialize mScoreText
      mLevelText(mFont),  // Initialize mLevelText
      mCurrentBgColor(sf::Color::Black), // Initial background color
      mTargetBgColor(sf::Color::Black),   // Target background color (initially black)
      mLevelTransitionDuration(sf::seconds(2.0f)) // 2-second transition
{
    mMenuText_Start.setPosition(sf::Vector2f(160, 200)); // Adjusted position
    mMenuText_Start.setFillColor(sf::Color::White);

    mMenuText_Close.setPosition(sf::Vector2f(160, 270)); // Adjusted position
    mMenuText_Close.setFillColor(sf::Color::White);

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setPosition(sf::Vector2f(GRID_WIDTH * CELL_SIZE + 20, 50)); // Adjusted position

    mLevelText.setFont(mFont);
    mLevelText.setCharacterSize(24);
    mLevelText.setFillColor(sf::Color::White);
    mLevelText.setPosition(sf::Vector2f(GRID_WIDTH * CELL_SIZE + 20, 100)); // Adjusted position
}

void Game::spawnTetromino() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, Tetromino::SHAPES.size() - 1);

    int type = distrib(gen);
    int startX = GRID_WIDTH / 2 - 2; // Center the tetromino
    int startY = 0; // Start at the top

    mCurrentTetromino = std::make_unique<Tetromino>(type, startX, startY);
}

void Game::restartGame() {
    mGrid = std::vector<std::vector<int>>(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0)); // Clear grid
    mScore = 0;
    mLevel = 1;
    mLinesCleared = 0;
    mFallTime = sf::seconds(0.5f); // Reset fall speed
    mTimeSinceLastFall = sf::Time::Zero;
    spawnTetromino();
    mState = PLAYING;
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }

        if (mState == MENU) {
            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
                    if (mMenuText_Start.getGlobalBounds().contains(mousePos)) {
                        restartGame(); // Call restartGame
                    }
                    if (mMenuText_Close.getGlobalBounds().contains(mousePos)) {
                        mWindow.close();
                    }
                }
            }
        } else if (mState == PLAYING) {
            if (event->is<sf::Event::KeyPressed>()) {
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (mCurrentTetromino) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                        if (!checkCollision(*mCurrentTetromino, -1, 0)) {
                            mCurrentTetromino->move(-1, 0);
                        }
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                        if (!checkCollision(*mCurrentTetromino, 1, 0)) {
                            mCurrentTetromino->move(1, 0);
                        }
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                        if (!checkCollision(*mCurrentTetromino, 0, 1)) {
                            mCurrentTetromino->move(0, 1);
                        }
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                        std::vector<sf::Vector2i> rotatedShape = mCurrentTetromino->getRotatedShape();
                        if (!checkCollision(*mCurrentTetromino, 0, 0, &rotatedShape)) {
                            mCurrentTetromino->setShape(rotatedShape);
                        }
                    }
                }
            }
        } else if (mState == GAME_OVER) { // New: allow restarting from game over screen
            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    restartGame(); // Restart game on click
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (mState != PLAYING) {
        return;
    }

    mTimeSinceLastFall += deltaTime;

    // Update score/level texts
    mScoreText.setString("Score: " + std::to_string(mScore));
    mLevelText.setString("Level: " + std::to_string(mLevel));

    // Adjust fall time based on level
    mFallTime = sf::seconds(std::max(0.05f, 0.5f - mLevel * 0.05f));

    // Level up logic:
    if (mScore >= mPointsToNextLevel) {
        mLevel++;
        mPointsToNextLevel += 100; // Increase points needed for next level by 100 each time
    }

    // Background color transition logic
    if (mLevel != mPreviousLevel) {
        // Level changed, start new transition
        mLevelTransitionClock.restart();
        mTargetBgColor = LevelColors[(mLevel - 1) % LevelColors.size()]; // Get target color for new level
        mPreviousLevel = mLevel; // Update previous level
    }

    sf::Time elapsed = mLevelTransitionClock.getElapsedTime();
    if (elapsed < mLevelTransitionDuration) {
        float progress = elapsed.asSeconds() / mLevelTransitionDuration.asSeconds();
        
        // Linear interpolation for each color component
        mCurrentBgColor.r = static_cast<unsigned char>(mCurrentBgColor.r + (mTargetBgColor.r - mCurrentBgColor.r) * progress);
        mCurrentBgColor.g = static_cast<unsigned char>(mCurrentBgColor.g + (mTargetBgColor.g - mCurrentBgColor.g) * progress);
        mCurrentBgColor.b = static_cast<unsigned char>(mCurrentBgColor.b + (mTargetBgColor.b - mCurrentBgColor.b) * progress);
    } else {
        mCurrentBgColor = mTargetBgColor; // Ensure final color is set when transition completes
    }

    if (mTimeSinceLastFall >= mFallTime) {
        if (mCurrentTetromino && !checkCollision(*mCurrentTetromino, 0, 1)) {
            mCurrentTetromino->move(0, 1);
        } else if (mCurrentTetromino) {
            // Collision detected, lock the tetromino to the grid
            sf::Vector2i position = mCurrentTetromino->getPosition();
            const std::vector<sf::Vector2i>& shape = mCurrentTetromino->getShape();
            int color = mCurrentTetromino->getType() + 1; // Use type + 1 as color ID

            for (const auto& p : shape) {
                if (position.y + p.y >= 0) { // Only place if block is within grid
                    mGrid[position.y + p.y][position.x + p.x] = color;
                }
            }
            
            clearFullLines(); // Call new method here

            spawnTetromino();
            if (checkCollision(*mCurrentTetromino, 0, 0)) { // Check for game over
                mState = GAME_OVER;
                std::cout << "Game Over!" << std::endl;
            }
        }
        mTimeSinceLastFall = sf::Time::Zero;
    }
}

void Game::drawGrid() {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            cell.setPosition(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE));
            if (mGrid[y][x] == 0) {
                cell.setFillColor(sf::Color::Black);
                cell.setOutlineColor(sf::Color(128, 128, 128)); // Gray
                cell.setOutlineThickness(1);
            } else {
                // Get the color from Tetromino::COLORS based on the grid value
                // mGrid stores type + 1, so subtract 1 to get the actual type index
                cell.setFillColor(Tetromino::COLORS[mGrid[y][x] - 1]);
                cell.setOutlineThickness(0);
            }
            mWindow.draw(cell);
        }
    }
}

bool Game::checkCollision(const Tetromino& tetromino, int offsetX, int offsetY, const std::vector<sf::Vector2i>* newShape) const {
    const std::vector<sf::Vector2i>& shape = (newShape) ? *newShape : tetromino.getShape();
    sf::Vector2i position = tetromino.getPosition();

    for (const auto& p : shape) {
        int x = position.x + p.x + offsetX;
        int y = position.y + p.y + offsetY;

        // Check horizontal boundaries
        if (x < 0 || x >= GRID_WIDTH) {
            return true;
        }
        // Check bottom boundary
        if (y >= GRID_HEIGHT) {
            return true;
        }
        // Check collision with existing blocks on the grid (if y is within grid bounds)
        // Only check if y is not above the grid (i.e., y >= 0)
        if (y >= 0 && mGrid[y][x] != 0) {
            return true;
        }
    }
    return false;
}

void Game::clearFullLines() {
    int linesClearedThisTurn = 0;
    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {
        bool fullLine = true;
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (mGrid[y][x] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            linesClearedThisTurn++;
            // Shift all lines above down by one
            for (int rowToMove = y; rowToMove > 0; --rowToMove) {
                for (int x = 0; x < GRID_WIDTH; ++x) {
                    mGrid[rowToMove][x] = mGrid[rowToMove - 1][x];
                }
            }
            // Clear the top line
            for (int x = 0; x < GRID_WIDTH; ++x) {
                mGrid[0][x] = 0;
            }
            // Re-check the same line, as it's now filled with the line above
            y++; 
        }
    }

    // Update score and level based on lines cleared
    if (linesClearedThisTurn > 0) {
        mScore += 20 * linesClearedThisTurn; // 20 points per line
        // Level up logic will be handled in update() based on score
    }
}

void Game::render() {
    mWindow.clear(mCurrentBgColor); // Use dynamically changing background color

    if (mState == MENU) {
        mWindow.draw(mMenuText_Start);
        mWindow.draw(mMenuText_Close);
    } else if (mState == PLAYING) {
        drawGrid();
        if (mCurrentTetromino) {
            mCurrentTetromino->draw(mWindow, CELL_SIZE);
        }
        mWindow.draw(mScoreText);
        mWindow.draw(mLevelText);
    } else if (mState == GAME_OVER) {
        sf::Text gameOverText(mFont, "Game Over!", 40); // Smaller font size
        gameOverText.setFillColor(sf::Color::Red);
        // Manually approximate centering without relying on getLocalBounds() for origin
        // Assuming text origin is top-left, approximate center by half its calculated width/height
        // Approx text width: 9 chars * 40px/char = 360px
        // Approx text height: 40px
        // Centered X: (window_width / 2) - (text_width / 2) = (400 / 2) - (360 / 2) = 200 - 180 = 20
        // Centered Y: (window_height / 2) - (text_height / 2) = (500 / 2) - (40 / 2) = 250 - 20 = 230
        gameOverText.setPosition(sf::Vector2f(20.0f, 230.0f));
        mWindow.draw(gameOverText);
        mWindow.draw(mScoreText); // Display final score
        mWindow.draw(mLevelText); // Display final level
    }
    
    mWindow.display();
}