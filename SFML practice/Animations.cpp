#include "Animations.h"
#include <iostream>

Animations::Animations()
    : firstAnimation(nullptr), lastAnimation(nullptr),
    animationCount(0), animationsEnabled(true) {
}

Animations::~Animations() {
    clearAll();
}

void Animations::addAnimation(AnimationType type, int targetID, float duration,
    sf::Vector2f startPos, sf::Vector2f endPos,
    float startScale, float endScale,
    const sf::Sprite* sprite) {

    if (!animationsEnabled) return;

    Animation* newAnim = new Animation();
    newAnim->type = type;
    newAnim->targetID = targetID;
    newAnim->duration = duration;
    newAnim->elapsed = 0.0f;
    newAnim->completed = false;
    newAnim->startPos = startPos;
    newAnim->endPos = endPos;
    newAnim->startScale = startScale;
    newAnim->endScale = endScale;
    newAnim->targetSprite = sprite;
    newAnim->next = nullptr;

    if (lastAnimation) {
        lastAnimation->next = newAnim;
        lastAnimation = newAnim;
    }
    else {
        firstAnimation = newAnim;
        lastAnimation = newAnim;
    }

    animationCount++;
    std::cout << "Animacion añadida. Total: " << animationCount << std::endl;
}

void Animations::addGemSwapAnimation(int gemID, sf::Vector2f from, sf::Vector2f to, const sf::Sprite* sprite) {
    addAnimation(AnimationType::GEM_SWAP, gemID, 0.4f, from, to, 1.0f, 1.0f, sprite);
}

void Animations::addGemMatchAnimation(int gemID, sf::Vector2f position, const sf::Sprite* sprite) {
    addAnimation(AnimationType::GEM_MATCH, gemID, 0.5f, position, position, 1.0f, 1.5f, sprite);
}

void Animations::addGemFallAnimation(int gemID, sf::Vector2f from, sf::Vector2f to, const sf::Sprite* sprite) {
    addAnimation(AnimationType::GEM_FALL, gemID, 0.6f, from, to, 1.0f, 1.0f, sprite);
}

void Animations::addIceBreakAnimation(int gemID, sf::Vector2f position, const sf::Sprite* sprite) {
    addAnimation(AnimationType::ICE_BREAK, gemID, 0.6f, position, position, 1.0f, 0.0f, sprite);
}

void Animations::addScorePopup(int points, sf::Vector2f position, const sf::Sprite* sprite) {
    addAnimation(AnimationType::SCORE_POPUP, points, 1.0f, position, position + sf::Vector2f(0, -50), 1.0f, 1.0f, sprite);
}

void Animations::update(float deltaTime) {
    if (!animationsEnabled) return;

    if (deltaTime < 0.001f) {
        deltaTime = 0.016f;
    }

    Animation* current = firstAnimation;
    Animation* prev = nullptr;

    while (current != nullptr) {
        if (!current->completed) {
            current->elapsed += deltaTime;

            float progress = std::min(current->elapsed / current->duration, 1.0f);

            if (current->targetSprite) {
                sf::Sprite* sprite = const_cast<sf::Sprite*>(current->targetSprite);

                switch (current->type) {
                case AnimationType::GEM_SWAP:
                case AnimationType::GEM_FALL: {
                    sf::Vector2f newPos;
                    newPos.x = current->startPos.x + (current->endPos.x - current->startPos.x) * progress;
                    newPos.y = current->startPos.y + (current->endPos.y - current->startPos.y) * progress;
                    sprite->setPosition(newPos);
                    break;
                }
                case AnimationType::GEM_MATCH: {
                    float scale = current->startScale + (current->endScale - current->startScale) * progress;
                    sprite->setScale(sf::Vector2f(scale, scale));

                    sf::Color color = sprite->getColor();
                    color.a = static_cast<uint8_t>(255 * (1.0f - progress));
                    sprite->setColor(color);
                    break;
                }
                case AnimationType::ICE_BREAK: {
                    float scale = 1.0f - progress;
                    sprite->setScale(sf::Vector2f(scale, scale));
                    sprite->setRotation(sf::degrees(progress * 90.0f));
                    break;
                }
                case AnimationType::SCORE_POPUP: {
                    float scale = 1.0f + progress * 0.3f;
                    sf::Vector2f newPos;
                    newPos.x = current->startPos.x;
                    newPos.y = current->startPos.y - (40.0f * progress);

                    sprite->setPosition(newPos);
                    sprite->setScale(sf::Vector2f(scale, scale));

                    sf::Color color = sprite->getColor();
                    color.a = static_cast<uint8_t>(255 * (1.0f - progress * 0.7f));
                    sprite->setColor(color);
                    break;
                }
                }
            }

            if (current->elapsed >= current->duration) {
                current->completed = true;

                if (current->targetSprite) {
                    sf::Sprite* sprite = const_cast<sf::Sprite*>(current->targetSprite);

                    switch (current->type) {
                    case AnimationType::GEM_SWAP:
                    case AnimationType::GEM_FALL:
                        break;
                    case AnimationType::GEM_MATCH:
                        break;
                    case AnimationType::ICE_BREAK:
                        break;
                    case AnimationType::SCORE_POPUP:
                        sprite->setScale(sf::Vector2f(0, 0));
                        break;
                    }
                }
            }
        }

        prev = current;
        current = current->next;
    }

    clearCompleted();
}

void Animations::clearAll() {
    Animation* current = firstAnimation;
    while (current) {
        Animation* next = current->next;
        delete current;
        current = next;
    }
    firstAnimation = nullptr;
    lastAnimation = nullptr;
    animationCount = 0;
}

void Animations::clearCompleted() {
    Animation* current = firstAnimation;
    Animation* prev = nullptr;

    while (current) {
        if (current->completed) {
            Animation* toDelete = current;
            current = current->next;

            if (prev) {
                prev->next = current;
            }
            else {
                firstAnimation = current;
            }

            if (toDelete == lastAnimation) {
                lastAnimation = prev;
            }

            delete toDelete;
            animationCount--;
        }
        else {
            prev = current;
            current = current->next;
        }
    }
}

bool Animations::isAnimating() const {
    return firstAnimation != nullptr;
}

void Animations::draw(sf::RenderWindow& window) {
    Animation* current = firstAnimation;
    int bombasDibujadas = 0;

    while (current != nullptr) {
        if (!current->completed && current->targetSprite) {
            // ✅ DEBUG: Verificar si es una bomba
            if (current->type == AnimationType::GEM_SWAP ||
                current->type == AnimationType::GEM_FALL) {
                // Podríamos agregar más información aquí si fuera necesario
            }
            window.draw(*current->targetSprite);
        }
        current = current->next;
    }

    if (bombasDibujadas > 0) {
        std::cout << "Bombas dibujadas en animaciones: " << bombasDibujadas << std::endl;
    }
}

bool Animations::isSpriteAnimating(const sf::Sprite* sprite) const {
    Animation* current = firstAnimation;
    while (current != nullptr) {
        if (!current->completed && current->targetSprite == sprite) {
            return true;
        }
        current = current->next;
    }
    return false;
}