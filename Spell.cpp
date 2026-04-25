#include "Spell.h"
#include "Game.h"
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <cmath>

Spell::Spell(std::string name, int dmg, float spd)
    : name(name), damage(dmg), speed(spd),
      position(0.f, 0.f), velocity(0.f, 0.f), active(false)
{
    sf::Image image;
    if (image.loadFromFile("assets/spell.png")) {
        sf::Vector2u size = image.getSize();
        for (unsigned int sy = 0; sy < size.y; ++sy) {
            for (unsigned int sx = 0; sx < size.x; ++sx) {
                sf::Color c = image.getPixel(sf::Vector2u(sx, sy));
                bool isNeutral = std::abs((int)c.r - (int)c.g) < 40 &&
                                 std::abs((int)c.g - (int)c.b) < 40 &&
                                 std::abs((int)c.r - (int)c.b) < 40;
                if (isNeutral && (c.r > 120 || c.g > 120 || c.b > 120)) {
                    image.setPixel(sf::Vector2u(sx, sy), sf::Color::Transparent);
                }
            }
        }
        if (!texture.loadFromImage(image)) {
            std::cerr << "Failed to load spell texture from image!\n";
        }
    }

    // FIX (SFML 3): Construct sprite only after texture is loaded.
    initSprite();
    sprite->setTextureRect(sf::IntRect(sf::Vector2i(10, 10), sf::Vector2i(380, 380)));
    sprite->setOrigin({190.f, 190.f});
    sprite->setScale(sf::Vector2f(0.15f, 0.15f));
}

// Called after texture is loaded to properly initialise the optional sprite.
void Spell::initSprite() {
    sprite.emplace(texture);
}

void Spell::update(float deltaTime) {
    if (active) {
        position += velocity * deltaTime;
        if (sprite) sprite->setPosition(position);

        if (position.x < -100.f || position.x > Game::WINDOW_WIDTH + 100.f ||
            position.y < -100.f || position.y > Game::WINDOW_HEIGHT + 100.f) {
            active = false;
        }
    }
}

void Spell::draw(sf::RenderWindow& window) {
    if (active && sprite) {
        window.draw(*sprite);
    }
}

bool Spell::isActive() const {
    return active;
}

void Spell::deactivate() {
    active = false;
}

int Spell::getDamage() const {
    return damage;
}

sf::FloatRect Spell::getBounds() const {
    if (sprite) return sprite->getGlobalBounds();
    return sf::FloatRect(position, {1.f, 1.f});
}
