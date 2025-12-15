#ifndef TETROMINO_H
#define TETROMINO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> // For std::min and std::max (not directly used here, but good practice for vector manipulation)

class Tetromino {
public:
    Tetromino(int type, int x, int y);

    void move(int dx, int dy);
    std::vector<sf::Vector2i> getRotatedShape() const; // Changed from rotate()

    void draw(sf::RenderWindow& window, int cellSize, const sf::Vector2i& offset = sf::Vector2i(0, 0)) const;

    sf::Vector2i getPosition() const;
    const std::vector<sf::Vector2i>& getShape() const;
    void setShape(const std::vector<sf::Vector2i>& newShape); // New setter for shape
    int getType() const { return mType; }

public: // Made public for direct access from Game::spawnTetromino
    static const std::vector<std::vector<sf::Vector2i>> SHAPES;
    static const std::vector<sf::Color> COLORS;

private:
    int mType;
    sf::Vector2i mPosition;
    std::vector<sf::Vector2i> mShape;
    sf::Color mColor;
};

#endif // TETROMINO_H
