#include "NormalGem.h"
#include <iostream>

NormalGem::NormalGem(int color) : colorType(color) {
    type = color;
    isMatched = false;
}

void NormalGem::draw(sf::RenderWindow& window) const {
    if (!isMatched) {
        window.draw(sprite);
    }
}

void NormalGem::onMatch() {
    // Comportamiento especifico para gemas normales
    std::cout << "Gema normal eliminada - Tipo: " << colorType << std::endl;
}

void NormalGem::setType(int newType, sf::Texture** textures, int numTextures) {
    colorType = newType;
    type = newType;

    if (colorType >= 0 && colorType < numTextures && textures[colorType] != nullptr) {
        sprite.setTexture(*textures[colorType]);

        sf::Vector2u size = textures[colorType]->getSize();
        float scaleX = static_cast<float>(CELL_SIZE) / size.x;
        float scaleY = static_cast<float>(CELL_SIZE) / size.y;
        sprite.setScale(sf::Vector2f(scaleX, scaleY));
    }
}

void NormalGem::setColor(int color) {
    colorType = color;
    type = color;
}