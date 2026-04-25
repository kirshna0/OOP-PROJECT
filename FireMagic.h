#ifndef FIREMAGIC_H
#define FIREMAGIC_H

#include "Spell.h"

// Concrete Spell: Fire Magic (High damage, slow)
class FireMagic : public Spell {
public:
    FireMagic();
    
    void cast(sf::Vector2f startPos, sf::Vector2f direction) override;
};

#endif // FIREMAGIC_H
