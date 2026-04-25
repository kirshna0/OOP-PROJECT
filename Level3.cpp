#include "Level3.h"
#include "Game.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

Level3::Level3(Game* g) : Level(g) {
    background.setSize(sf::Vector2f(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT));
    background.setFillColor(sf::Color(80, 20, 80));
}

void Level3::init() {
    enemies.clear();
    auto e1 = std::make_unique<Enemy>(game, "Midnight Sun Mage 1", 700.f, 150.f, 150, 150.f, 15);
    auto e2 = std::make_unique<Enemy>(game, "Midnight Sun Mage 2", 700.f, 450.f, 150, 150.f, 15);
    auto boss = std::make_unique<Enemy>(game, "Vetto the Despair",   750.f, 300.f, 400, 120.f, 40);
    
    e1->setColor(sf::Color(200, 100, 255)); // Purple tint
    e2->setColor(sf::Color(200, 100, 255));
    boss->setColor(sf::Color(150, 0, 255)); // Deep purple for Vetto
    
    enemies.push_back(std::move(e1));
    enemies.push_back(std::move(e2));
    enemies.push_back(std::move(boss));
    std::cout << "Level 3 (Midnight Sun) initialized.\n";
}

void Level3::update(float deltaTime) {
    // FIX: Delegate to base class helper instead of duplicating logic
    updateEnemies(deltaTime);
}

void Level3::draw(sf::RenderWindow& window) {
    // Level 3: Midnight Sun (Void Purple & Galaxy Black)
    drawTiles(window, sf::Color(60, 0, 120), sf::Color(5, 5, 10));
    drawGrid(window, sf::Color(150, 50, 255, 70));
    drawParticles(window, sf::Color(180, 100, 255, 200)); // Purple Mana
    drawEnemies(window);
}
