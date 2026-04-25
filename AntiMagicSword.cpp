#include "AntiMagicSword.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

AntiMagicSword::AntiMagicSword() : Spell("Anti-Magic Sword", 50, 700.0f), lifetime(0.f), maxLifetime(0.5f) {
    // sprite is constructed by Spell base via initSprite() after texture load
    if (sprite) {
        sprite->setColor(sf::Color(100, 100, 100, 255));
        sprite->setScale(sf::Vector2f(0.3f, 0.3f));
    }
}

void AntiMagicSword::cast(sf::Vector2f startPos, sf::Vector2f direction) {
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
    lifetime = 0.f;
}

void AntiMagicSword::update(float deltaTime) {
    if (active) {
        Spell::update(deltaTime);
        lifetime += deltaTime;
        if (lifetime >= maxLifetime) {
            active = false;
        }
    }
}
