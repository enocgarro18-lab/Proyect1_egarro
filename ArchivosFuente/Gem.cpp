#include "Gem.h"

namespace {
    sf::Texture& getDummyTexture() {
        static sf::Texture texture;
        static bool initialized = false;
        if (!initialized) {
            texture.loadFromFile("../assets/gem0.png");
            initialized = true;
        }
        return texture;
    }
}

//Constructor
Gem::Gem() : type(-1), sprite(getDummyTexture()), isMatched(false) {
}

//Destructor
Gem::~Gem() {

}

void Gem::draw(sf::RenderWindow& window) const {
    if (type != -1 && !isMatched) {
        window.draw(sprite);
    }
}

void Gem::onMatch() {
    std::cout << "Gema eliminada - Tipo: " << type << std::endl;
}

int Gem::getType() const {
    return type;
}

void Gem::setType(int newType, sf::Texture** textures, int numTextures) {
    type = newType;
    isMatched = false;

    if (type >= 0 && type < numTextures && textures[type] != nullptr) {
        sprite.setTexture(*textures[type]);

        sf::Vector2u size = textures[type]->getSize();
        float scaleX = static_cast<float>(CELL_SIZE) / size.x;
        float scaleY = static_cast<float>(CELL_SIZE) / size.y;
        sprite.setScale(sf::Vector2f(scaleX, scaleY));
    }
}
void Gem::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}
void Gem::setTexture(const sf::Texture& texture) {
    sprite.setTexture(texture);

    sf::Vector2u size = texture.getSize();
    float scaleX = static_cast<float>(CELL_SIZE) / size.x;
    float scaleY = static_cast<float>(CELL_SIZE) / size.y;
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
}