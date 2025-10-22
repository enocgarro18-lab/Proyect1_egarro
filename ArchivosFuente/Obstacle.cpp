#include "Obstacle.h"
#include <iostream>

Obstacle::Obstacle(ObstacleType obstacleType, const sf::Texture& texture, int row, int col)
    : sprite(texture)
    , type(obstacleType)
    , visible(true)
    , boardPosition(col, row)
{
    // Configurar salud segun el tipo
    switch (type) {
    case ObstacleType::ICE:
        health = 1;
        break;
    case ObstacleType::ROCK:
        health = 2;
        break;
    case ObstacleType::CHAIN:
        health = 1;
        break;
    case ObstacleType::LOCK:
        health = 3;
        break;
    }
}

void Obstacle::takeDamage(int damage) {
    if (!visible || !canBeDamaged()) return;

    health -= damage;
    std::cout << "Obstaculo recibió " << damage << " de danio. Salud: " << health << "\n";

    if (health <= 0) {
        hide();
        std::cout << "Obstaculo destruido!\n";
    }
}

bool Obstacle::isDestroyed() const {
    return health <= 0;
}

ObstacleType Obstacle::getType() const {
    return type;
}

sf::Vector2i Obstacle::getBoardPosition() const {
    return boardPosition;
}

int Obstacle::getHealth() const {
    return health;
}

bool Obstacle::blocksMovement() const {
    return type != ObstacleType::ICE;
}

bool Obstacle::canBeDamaged() const {
    return type != ObstacleType::LOCK || health > 0;
}

void Obstacle::setScale(float scaleX, float scaleY) {
    sprite.setScale({ scaleX, scaleY });
}

void Obstacle::setPosition(float x, float y) {
    sprite.setPosition({ x, y });
}

void Obstacle::draw(sf::RenderWindow& window) const {
    if (visible) {
        window.draw(sprite);
    }
}

void Obstacle::hide() {
    visible = false;
}

bool Obstacle::isVisible() const {
    return visible;
}

sf::Sprite& Obstacle::getSprite() {
    return sprite;
}


void Obstacle::reset() {
    visible = true;
    switch (type) {
    case ObstacleType::ICE:
        health = 1;
        break;
    case ObstacleType::ROCK:
        health = 2;
        break;
    case ObstacleType::CHAIN:
        health = 1;
        break;
    case ObstacleType::LOCK:
        health = 3;
        break;
    }
}

bool Obstacle::isAtPosition(int row, int col) const {
    return boardPosition.x == col && boardPosition.y == row;
}

void Obstacle::flashDamage() {
    sprite.setColor(sf::Color::Red);
}