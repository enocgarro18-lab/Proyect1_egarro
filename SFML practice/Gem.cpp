#include "Gem.h"

// Inicialización del emptyTexture
sf::Texture Gem::emptyTexture;

Gem::Gem() : type(-1), sprite(emptyTexture) {
    std::cout << "[Gem] Constructor default llamado\n";
    std::cout << "[Gem] setType asignado tipo " << type << (type == -1 ? " (vacío)" : "") << std::endl;
}

Gem::Gem(int t, sf::Texture* textures, int numTextures) : sprite(emptyTexture) {
    std::cout << "[Gem] Constructor con tipo " << t << " llamado\n";
    setType(t, textures, numTextures);
}

void Gem::setType(int t, sf::Texture* textures, int numTextures) {
    if (!textures || t < 0 || t >= numTextures) {
        std::cerr << "[Gem] tipo inválido o puntero a texturas nulo\n";
        type = -1;
        sprite.setTexture(emptyTexture);
        return;
    }

    sf::Vector2u size = textures[t].getSize();
    if (size.x == 0 || size.y == 0) {
        std::cerr << "[Gem] textura vacía, no asignada\n";
        type = -1;
        sprite.setTexture(emptyTexture);
        return;
    }

    type = t;
    sprite.setTexture(textures[type]);
    sprite.setScale(sf::Vector2f(CELL_SIZE / float(size.x), CELL_SIZE / float(size.y)));
}

int Gem::getType() const {
    return type;
}

void Gem::setPosition(const sf::Vector2f& pos) {
    std::cout << "[Gem] setPosition: (" << pos.x << ", " << pos.y << ")\n";
    sprite.setPosition(pos);
}

void Gem::draw(sf::RenderWindow& window) {
    if (type != -1) {
        window.draw(sprite);
    }
}
