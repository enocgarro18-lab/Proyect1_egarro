#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>

enum class ObstacleType {
    ICE,
    ROCK,
    CHAIN,
    LOCK
};

class Obstacle {
private:
    sf::Sprite sprite;
    ObstacleType type;
    int health;
    bool visible;
    sf::Vector2i boardPosition;

public:
    Obstacle(ObstacleType obstacleType, const sf::Texture& texture, int row, int col);

    void takeDamage(int damage);
    bool isDestroyed() const;
    ObstacleType getType() const;
    sf::Vector2i getBoardPosition() const;
    int getHealth() const;
    bool blocksMovement() const;
    bool canBeDamaged() const;

    void setScale(float scaleX, float scaleY);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window) const;
    void hide();
    bool isVisible() const;
    sf::Sprite& getSprite();
    void reset();
    bool isAtPosition(int row, int col) const;
    void flashDamage();
};

#endif