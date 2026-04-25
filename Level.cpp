#include "Level.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>
#include <optional>
#include <cmath>

Level::Level(Game* g) : hasSpawnedEnemies(false), game(g), completed(false), levelTimer(0.f) {}

bool Level::isCompleted() const {
    return completed;
}

// FIX: Centralised enemy update logic — previously copy-pasted identically
//      into Level1, Level2, and Level3. Now lives here once.
void Level::updateEnemies(float deltaTime) {
    levelTimer += deltaTime;

    if (!enemies.empty()) {
        hasSpawnedEnemies = true;
    }

    // Only allow completion if we've been running for at least 1 second
    // This prevents the level from skipping during the first frame of init
    if (levelTimer > 1.0f && hasSpawnedEnemies && enemies.empty()) {
        completed = true;
        return;
    }

    Player* p = game->getPlayer();
    if (!p) return;

    for (auto it = enemies.begin(); it != enemies.end();) {
        (*it)->update(deltaTime);

        // Enemy contact hits player
        if ((*it)->getBounds().findIntersection(p->getBounds()).has_value()) {
            if ((*it)->canAttack()) {
                p->takeDamage((*it)->getDamage());
                std::cout << (*it)->getName() << " hit player! Health: " << p->getHealth() << "\n";
            }
        }

        // Enemy spell hits player
        Spell* enemySpell = (*it)->getSpell();
        if (enemySpell && enemySpell->isActive()) {
            if (enemySpell->getBounds().findIntersection(p->getBounds()).has_value()) {
                p->takeDamage(enemySpell->getDamage());
                enemySpell->setActive(false);
                std::cout << (*it)->getName() << "'s magic hit player! Health: " << p->getHealth() << "\n";
            }
        }

        // Player spells hit enemy
        for (auto& spell : p->getActiveSpells()) {
            if (spell->isActive() && spell->getBounds().findIntersection((*it)->getBounds()).has_value()) {
                (*it)->takeDamage(spell->getDamage());
                spell->deactivate();
                break;
            }
        }

        if ((*it)->isDead()) {
            std::cout << (*it)->getName() << " defeated!\n";
            if (game) game->addKill();
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}
void Level::drawGrid(sf::RenderWindow& window, sf::Color color) {
    sf::RectangleShape line;
    line.setFillColor(color);
    for (int x = 0; x <= (int)Game::WINDOW_WIDTH; x += 40) {
        line.setSize(sf::Vector2f(1.f, Game::WINDOW_HEIGHT));
        line.setPosition(sf::Vector2f((float)x, 0.f));
        window.draw(line);
    }
    for (int y = 0; y <= (int)Game::WINDOW_HEIGHT; y += 40) {
        line.setSize(sf::Vector2f(Game::WINDOW_WIDTH, 1.f));
        line.setPosition(sf::Vector2f(0.f, (float)y));
        window.draw(line);
    }
}

void Level::drawTiles(sf::RenderWindow& window, sf::Color color1, sf::Color color2) {
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(80.f, 80.f));
    for (int y = 0; y < (int)Game::WINDOW_HEIGHT; y += 80) {
        for (int x = 0; x < (int)Game::WINDOW_WIDTH; x += 80) {
            tile.setFillColor(((x + y) / 80) % 2 == 0 ? color1 : color2);
            tile.setPosition(sf::Vector2f((float)x, (float)y));
            window.draw(tile);
        }
    }
}

void Level::drawParticles(sf::RenderWindow& window, sf::Color color) {
    static float timer = 0;
    timer += 0.01f;
    sf::CircleShape p(2.f);
    p.setFillColor(color);
    for (int i = 0; i < 15; ++i) {
        float x = (float)((i * 137) % (int)Game::WINDOW_WIDTH);
        float y = (float)((i * 251) % (int)Game::WINDOW_HEIGHT) + std::sin(timer + i) * 20.f;
        p.setPosition(sf::Vector2f(x, y));
        window.draw(p);
    }
}
void Level::drawEnemies(sf::RenderWindow& window) {
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }
}
