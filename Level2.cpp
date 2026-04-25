#include "Level2.h"
#include "Game.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

Level2::Level2(Game* g) : Level(g) {
    background.setSize(sf::Vector2f(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT));
    background.setFillColor(sf::Color(80, 50, 50));
}

void Level2::init() {
    enemies.clear();
    enemies.push_back(std::make_unique<Enemy>(game, "Diamond Mage 1", 600.f, 100.f, 80, 120.f, 15));
    enemies.push_back(std::make_unique<Enemy>(game, "Diamond Mage 2", 600.f, 400.f, 80, 120.f, 15));
    enemies.push_back(std::make_unique<Enemy>(game, "Mars", 700.f, 250.f, 250, 80.f, 25)); // BOSS
    
    enemies[0]->setColor(sf::Color(255, 100, 100)); // Red tint
    enemies[1]->setColor(sf::Color(255, 100, 100));
    enemies[2]->setColor(sf::Color(255, 0, 0)); // Bright red for boss
    
    std::cout << "Level 2 (Diamond Kingdom) initialized.\n";
}

void Level2::update(float deltaTime) {
    // FIX: Delegate to base class helper instead of duplicating logic
    updateEnemies(deltaTime);
}

void Level2::draw(sf::RenderWindow& window) {
    // Level 2: Diamond Kingdom (Molten Red & Obsidian)
    drawTiles(window, sf::Color(150, 30, 30), sf::Color(20, 20, 20));
    drawGrid(window, sf::Color(255, 50, 50, 60));
    drawParticles(window, sf::Color(255, 200, 200, 180)); // Red Mana
    drawEnemies(window);
}
