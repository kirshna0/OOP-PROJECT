#include "FireMagic.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

FireMagic::FireMagic() : Spell("Fire Roar", 40, 800.0f) {
    if (sprite) sprite->setColor(sf::Color(255, 165, 0, 230)); // Orange tint
}

void FireMagic::cast(sf::Vector2f startPos, sf::Vector2f direction) {
    active = true;
    position = startPos;

    if (direction != sf::Vector2f{0.f, 0.f}) {
        velocity = direction.normalized() * speed;
        if (sprite) sprite->setRotation(direction.angle());
    } else {
        velocity = sf::Vector2f(speed, 0.f);
        if (sprite) sprite->setRotation(sf::degrees(0.f));
    }

    if (sprite) sprite->setPosition(position);
}
