#ifndef GEM_H
#define GEM_H

#include <SFML/Graphics.hpp>
#include <iostream>

const int CELL_SIZE = 64;

class Gem {
private:
    int type;
    sf::Sprite sprite;

public:
    static sf::Texture emptyTexture;

    Gem();
    Gem(int t, sf::Texture* textures, int numTextures);

    void setType(int t, sf::Texture* textures, int numTextures);
    int getType() const;
    void setPosition(const sf::Vector2f& pos);
    void draw(sf::RenderWindow& window);
};

#endif
