#include "Tetromino.h"

const std::vector<std::vector<sf::Vector2i>> Tetromino::SHAPES = {
    {{0,0}, {1,0}, {0,1}, {1,1}}, // O
    {{0,0}, {1,0}, {2,0}, {3,0}}, // I
    {{0,1}, {1,1}, {2,1}, {1,0}}, // T
    {{0,0}, {0,1}, {1,1}, {2,1}}, // L
    {{2,0}, {0,1}, {1,1}, {2,1}}, // J
    {{1,0}, {2,0}, {0,1}, {1,1}}, // S
    {{0,0}, {1,0}, {1,1}, {2,1}}  // Z
};

const std::vector<sf::Color> Tetromino::COLORS = {
    sf::Color::Yellow,
    sf::Color::Cyan,
    sf::Color::Magenta,
    sf::Color(255, 165, 0), // Orange
    sf::Color::Blue,
    sf::Color::Green,
    sf::Color::Red
};

Tetromino::Tetromino(int type, int x, int y)
    : mType(type), mPosition(x, y)
{
    mShape = SHAPES[mType];
    mColor = COLORS[mType];
}

void Tetromino::move(int dx, int dy) {
    mPosition.x += dx;
    mPosition.y += dy;
}

// Changed from rotate()
std::vector<sf::Vector2i> Tetromino::getRotatedShape() const {
    sf::Vector2i pivot = mShape[0]; // Assuming first block as pivot

    std::vector<sf::Vector2i> rotatedShape;
    for (const auto& p : mShape) {
        int relativeX = p.x - pivot.x;
        int relativeY = p.y - pivot.y;

        int newRelativeX = -relativeY;
        int newRelativeY = relativeX;

        rotatedShape.push_back(sf::Vector2i(pivot.x + newRelativeX, pivot.y + newRelativeY));
    }
    return rotatedShape;
}

void Tetromino::setShape(const std::vector<sf::Vector2i>& newShape) {
    mShape = newShape;
}


void Tetromino::draw(sf::RenderWindow& window, int cellSize) const {
    sf::RectangleShape block(sf::Vector2f(cellSize, cellSize));
    block.setFillColor(mColor);

    for (const auto& p : mShape) {
        block.setPosition(sf::Vector2f((mPosition.x + p.x) * cellSize, (mPosition.y + p.y) * cellSize));
        window.draw(block);
    }
}

sf::Vector2i Tetromino::getPosition() const {
    return mPosition;
}

const std::vector<sf::Vector2i>& Tetromino::getShape() const {
    return mShape;
}
