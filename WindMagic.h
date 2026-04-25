#ifndef WINDMAGIC_H
#define WINDMAGIC_H

#include "Spell.h"

// Concrete Spell: Wind Magic (Yuno's style)
class WindMagic : public Spell {
public:
    WindMagic();
    
    void cast(sf::Vector2f startPos, sf::Vector2f direction) override;
};

#endif // WINDMAGIC_H
