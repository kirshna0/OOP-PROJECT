#include "Entity.h"

// Don't construct sprite here — texture is empty at this point.
// Derived classes load their texture first, then call initSprite().
Entity::Entity(std::string name, float x, float y, int hp, float spd)
    : position(x, y), health(hp), maxHealth(hp), speed(spd), name(name) {}

// Called by derived classes AFTER texture is loaded so the sprite gets a valid texture.
void Entity::initSprite() {
    sprite.emplace(texture);
    sprite->setPosition(position);
}

sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    if (sprite) sprite->setPosition(position);
}

int Entity::getHealth() const {
    return health;
}

int Entity::getMaxHealth() const {
    return maxHealth;
}

void Entity::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Entity::isDead() const {
    return health <= 0;
}

std::string Entity::getName() const {
    return name;
}

sf::FloatRect Entity::getBounds() const {
    if (sprite) return sprite->getGlobalBounds();
    // Fallback: 1x1 rect at position (won't collide meaningfully)
    return sf::FloatRect(position, {1.f, 1.f});
}
