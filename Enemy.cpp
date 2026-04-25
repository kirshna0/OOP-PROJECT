#include "Enemy.h"
#include <SFML/Graphics/Image.hpp>
#include "WindMagic.h"
#include "Game.h"
#include "Player.h"
#include <iostream>

Enemy::Enemy(Game* g, std::string name, float x, float y, int hp, float spd, int dmg)
    : Entity(name, x, y, hp, spd), game(g), attackTimer(0.f), attackCooldown(2.0f), damage(dmg),
      shootTimer(0.f), shootCooldown(3.0f) {

    // Give all enemies a Wind Blade magic to start with
    magicSpell = std::make_unique<WindMagic>();
    magicSpell->setDamage(dmg / 2); // Magic does half contact damage

    sf::Image image;
    if (image.loadFromFile("assets/enemy.png")) {
        sf::Vector2u size = image.getSize();
        for (unsigned int iy = 0; iy < size.y; ++iy) {
            for (unsigned int ix = 0; ix < size.x; ++ix) {
                sf::Color c = image.getPixel({ix, iy});
                bool isNeutral = std::abs((int)c.r - (int)c.g) < 40 &&
                                 std::abs((int)c.g - (int)c.b) < 40 &&
                                 std::abs((int)c.r - (int)c.b) < 40;
                if (isNeutral && (c.r > 120 || c.g > 120 || c.b > 120)) {
                    image.setPixel({ix, iy}, sf::Color::Transparent);
                }
            }
        }
        if (!texture.loadFromImage(image)) {
            std::cerr << "Failed to load enemy texture from image!\n";
        }
    }
    // Construct sprite only after texture is loaded.
    initSprite();
    sprite->setScale(sf::Vector2f(0.25f, 0.25f));
    sprite->setPosition(position);
}

void Enemy::update(float deltaTime) {
    if (!game || !game->getPlayer()) return;
    sf::Vector2f playerPos = game->getPlayer()->getPosition();

    moveTowards(playerPos, deltaTime);

    // Handle Magic Attack
    shootTimer += deltaTime;
    if (shootTimer >= shootCooldown) {
        shootTimer = 0.f;
        sf::Vector2f direction = playerPos - position;
        magicSpell->cast(position, direction);
    }

    if (magicSpell->isActive()) {
        magicSpell->update(deltaTime);
    }

    if (attackTimer > 0.f) {
        attackTimer -= deltaTime;
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (magicSpell->isActive()) {
        magicSpell->draw(window);
    }
    if (sprite) window.draw(*sprite);
}

void Enemy::moveTowards(sf::Vector2f targetPos, float deltaTime) {
    sf::Vector2f direction = targetPos - position;
    float distance = direction.length();

    if (distance > 5.0f) {
        position += direction.normalized() * speed * deltaTime;
        if (sprite) sprite->setPosition(position);
    }
}

bool Enemy::canAttack() {
    if (attackTimer <= 0.f) {
        attackTimer = attackCooldown;
        return true;
    }
    return false;
}

int Enemy::getDamage() const {
    return damage;
}
