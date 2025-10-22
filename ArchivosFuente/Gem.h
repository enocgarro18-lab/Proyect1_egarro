#ifndef GEM_H
#define GEM_H

#include <SFML/Graphics.hpp>
#include <iostream>

const int CELL_SIZE = 70;

class Gem {
protected:
    int type;
    sf::Sprite sprite;
    bool isMatched;

public:
    Gem();
    virtual ~Gem();

    
    virtual void draw(sf::RenderWindow& window) const;
    virtual void onMatch();
    virtual int getType() const;
    virtual void setType(int newType, sf::Texture** textures, int numTextures);
    void setPosition(sf::Vector2f position);
    const sf::Sprite& getSprite() const { return sprite; }
    void setTexture(const sf::Texture& texture);
    bool getIsMatched() const { return isMatched; }
    void setMatched(bool matched);
};

#endif