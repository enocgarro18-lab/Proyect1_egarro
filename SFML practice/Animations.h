#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <SFML/Graphics.hpp>
#include "Gem.h"

enum class AnimationType {
    GEM_SWAP,
    GEM_MATCH,
    GEM_FALL,
    ICE_BREAK,
    SCORE_POPUP,
};

struct Animation {
    AnimationType type;
    float duration;
    float elapsed;
    bool completed;
    int targetID;
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    float startScale;
    float endScale;
    const sf::Sprite* targetSprite;
    Animation* next;
};

class Animations {
private:
    Animation* firstAnimation;
    Animation* lastAnimation;
    int animationCount;
    bool animationsEnabled;

public:
    Animations();
    ~Animations();

    void addAnimation(AnimationType type, int targetID, float duration,
        sf::Vector2f startPos, sf::Vector2f endPos,
        float startScale, float endScale,
        const sf::Sprite* sprite);

    void addGemSwapAnimation(int gemID, sf::Vector2f from, sf::Vector2f to, const sf::Sprite* sprite);
    void addGemMatchAnimation(int gemID, sf::Vector2f position, const sf::Sprite* sprite);
    void addGemFallAnimation(int gemID, sf::Vector2f from, sf::Vector2f to, const sf::Sprite* sprite);
    void addIceBreakAnimation(int gemID, sf::Vector2f position, const sf::Sprite* sprite);
    void addScorePopup(int points, sf::Vector2f position, const sf::Sprite* sprite);

    void update(float deltaTime);
    void clearAll();
    void clearCompleted();
    bool isAnimating() const;
    void setEnabled(bool enabled) { animationsEnabled = enabled; }
    int getAnimationCount() const { return animationCount; }

    void draw(sf::RenderWindow& window);
    bool isSpriteAnimating(const sf::Sprite* sprite) const;
};

#endif