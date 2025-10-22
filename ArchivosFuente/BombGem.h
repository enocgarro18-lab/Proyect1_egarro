#ifndef BOMB_GEM_H
#define BOMB_GEM_H

#include "Gem.h"

class BombGem : public Gem {
public:
    BombGem();

    void draw(sf::RenderWindow& window) const override;
    void onMatch() override;
    int getType() const override { return 5; } 
    void setType(int newType, sf::Texture** textures, int numTextures) override;
};

#endif