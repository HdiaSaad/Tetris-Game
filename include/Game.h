#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <chrono> // For fall timer
#include "Tetromino.h"

class Game {
public:
    Game();
    void run();

private:
    enum GameState { MENU, PLAYING, GAME_OVER };
    void clearFullLines();
    sf::Font loadFont(const std::string& fontPath); // Helper function
    void spawnTetromino(); // New method to spawn a tetromino

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void drawGrid();
    void restartGame(); // New method

    bool checkCollision(const Tetromino& tetromino, int offsetX, int offsetY, const std::vector<sf::Vector2i>* newShape = nullptr) const;

    sf::RenderWindow mWindow;
    GameState mState;
    sf::Font mFont;
    sf::Text mMenuText_Start;
    sf::Text mMenuText_Close;

    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 20; // Corrected: removed duplicate 'int'
    static const int CELL_SIZE = 25;

    std::vector<std::vector<int>> mGrid;
    std::unique_ptr<Tetromino> mCurrentTetromino; // New member
    std::unique_ptr<Tetromino> mNextTetromino;    // New member for next Tetromino preview
    // Fall timer
    sf::Time mFallTime;

    sf::Time mTimeSinceLastFall;

    // Game stats
    int mScore;
    int mLevel;
    int mPreviousLevel; // To detect level changes for background transition
    int mLinesCleared;
    int mPointsToNextLevel; // New member for tracking level progression
    int mLives; // New member for player lives
    sf::Text mScoreText;
    sf::Text mLevelText;
    int mBestScore; // New member for best score
    sf::Text mBestScoreText; // New member for best score display
    void saveScores(); // New method for saving scores
    void loadScores(); // New method for loading scores

    // Background color transition members
    sf::Color mCurrentBgColor;
    sf::Color mTargetBgColor;
    sf::Clock mLevelTransitionClock;
    sf::Time mLevelTransitionDuration;
};

#endif // GAME_H