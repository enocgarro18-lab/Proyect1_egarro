#include "BombGem.h"
#include <iostream>

BombGem::BombGem() {
    type = 5;
    isMatched = false;
}

void BombGem::draw(sf::RenderWindow& window) const {
    if (!isMatched) {
        window.draw(sprite);
    }
}

void BombGem::onMatch() {
    std::cout << "BOMBA explotando! Elimina área 3x3" << std::endl;
}

void BombGem::setType(int newType, sf::Texture** textures, int numTextures) {
    type = 5;

    std::cout << "BombGem::setType - Cargando textura de bomba" << std::endl;

    if (5 < numTextures && textures[5] != nullptr) {
        sprite.setTexture(*textures[5]);

        sf::Vector2u size = textures[5]->getSize();
        float scaleX = static_cast<float>(CELL_SIZE) / size.x;
        float scaleY = static_cast<float>(CELL_SIZE) / size.y;
        sprite.setScale(sf::Vector2f(scaleX, scaleY));

        std::cout << "Textura cargada - Escala: " << scaleX << "x" << scaleY << std::endl;
    }
    else {
        std::cout << "No se pudo cargar textura de bomba" << std::endl;
    }
}

