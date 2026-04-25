#ifndef ANTIMAGICSWORD_H
#define ANTIMAGICSWORD_H

#include "Spell.h"

// Concrete Spell: Anti-Magic Sword (Asta's primary attack)
class AntiMagicSword : public Spell {
private:
    float lifetime;
    float maxLifetime;

public:
    AntiMagicSword();
    
    void cast(sf::Vector2f startPos, sf::Vector2f direction) override;
    void update(float deltaTime) override;
};

#endif // ANTIMAGICSWORD_H
