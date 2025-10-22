#ifndef NORMAL_GEM_H
#define NORMAL_GEM_H

#include "Gem.h"

class NormalGem : public Gem {
private:
    int colorType;

public:
    NormalGem(int color = 0);
    void draw(sf::RenderWindow& window) const override;
    void onMatch() override;
    int getType() const override { return colorType; }
    void setType(int newType, sf::Texture** textures, int numTextures) override;
    

    void setColor(int color);
};

#endif