#include "Player.h"
#include "Spell.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

Player::Player(float x, float y) : Entity("Asta", x, y, 100, 300.0f), activeSpellIndex(0) {
    sf::Image image;
    if (image.loadFromFile("assets/player.png")) {
        // Remove neutral grayish/whitish background pixels
        sf::Vector2u size = image.getSize();
        for (unsigned int py = 0; py < size.y; ++py) {
            for (unsigned int px = 0; px < size.x; ++px) {
                sf::Color c = image.getPixel({px, py});
                bool isNeutral = std::abs((int)c.r - (int)c.g) < 40 &&
                                 std::abs((int)c.g - (int)c.b) < 40 &&
                                 std::abs((int)c.r - (int)c.b) < 40;
                if (isNeutral && (c.r > 120 || c.g > 120 || c.b > 120)) {
                    image.setPixel({px, py}, sf::Color::Transparent);
                }
            }
        }
        if (!texture.loadFromImage(image)) {
            std::cerr << "Failed to load player texture from image!\n";
        }
    }
    // FIX (SFML 3): Construct sprite only after texture is loaded.
    initSprite();
    sprite->setScale(sf::Vector2f(0.25f, 0.25f));
    sprite->setPosition(position);
}

void Player::update(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);

    if (movingUp) movement.y -= 1.f;
    if (movingDown) movement.y += 1.f;
    if (movingLeft) movement.x -= 1.f;
    if (movingRight) movement.x += 1.f;

    if (movement != sf::Vector2f{0.f, 0.f}) {
        movement = movement.normalized();
    }

    position += movement * speed * deltaTime;

    // Clamp player within window bounds
    const float spriteSize = 50.f;
    position.x = std::clamp(position.x, 0.f, Game::WINDOW_WIDTH - spriteSize);
    position.y = std::clamp(position.y, 0.f, Game::WINDOW_HEIGHT - spriteSize);

    if (sprite) sprite->setPosition(position);

    for (auto& spell : spells) {
        if (spell->isActive()) {
            spell->update(deltaTime);
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (sprite) window.draw(*sprite);
    for (auto& spell : spells) {
        if (spell->isActive()) {
            spell->draw(window);
        }
    }
}

void Player::addSpell(std::unique_ptr<Spell> spell) {
    spells.push_back(std::move(spell));
}

void Player::castSpell(sf::Vector2f targetDir) {
    if (spells.empty()) return;
    if (!spells[activeSpellIndex]->isActive()) {
        spells[activeSpellIndex]->cast(position + sf::Vector2f(25.f, 25.f), targetDir);
    }
}

void Player::switchSpell() {
    if (!spells.empty()) {
        activeSpellIndex = (activeSpellIndex + 1) % static_cast<int>(spells.size());
        std::cout << "Switched to spell: " << spells[activeSpellIndex]->getName() << "\n";
    }
}

std::vector<std::unique_ptr<Spell>>& Player::getActiveSpells() {
    return spells;
}

std::string Player::getCurrentSpellName() const {
    if (spells.empty()) return "None";
    return spells[activeSpellIndex]->getName();
}

void Player::handleEvent(const sf::Event& event) {
    if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::W) movingUp = true;
        if (keyPress->code == sf::Keyboard::Key::S) movingDown = true;
        if (keyPress->code == sf::Keyboard::Key::A) movingLeft = true;
        if (keyPress->code == sf::Keyboard::Key::D) movingRight = true;
    } else if (const auto* keyRelease = event.getIf<sf::Event::KeyReleased>()) {
        if (keyRelease->code == sf::Keyboard::Key::W) movingUp = false;
        if (keyRelease->code == sf::Keyboard::Key::S) movingDown = false;
        if (keyRelease->code == sf::Keyboard::Key::A) movingLeft = false;
        if (keyRelease->code == sf::Keyboard::Key::D) movingRight = false;
    }
}
