#include "Level1.h"
#include "Game.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

Level1::Level1(Game *g) : Level(g) {
  background.setSize(sf::Vector2f(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT));
  background.setFillColor(sf::Color(150, 150, 150));
}

void Level1::init() {
  enemies.clear();
  // FIX: Added explicit damage values per enemy (3rd-to-last arg)
  enemies.push_back(
      std::make_unique<Enemy>(game, "Thug 1", 600.f, 100.f, 50, 100.f, 8));
  enemies.push_back(
      std::make_unique<Enemy>(game, "Thug 2", 600.f, 400.f, 50, 100.f, 8));
  std::cout << "Level 1 initialized.\n";
}

void Level1::update(float deltaTime) {
  // FIX: Delegate to base class helper instead of duplicating logic
  updateEnemies(deltaTime);
}

void Level1::draw(sf::RenderWindow &window) {
  // Level 1: Clover Kingdom (Emerald & Stone)
  drawTiles(window, sf::Color(30, 80, 30), sf::Color(100, 100, 100));
  drawGrid(window, sf::Color(200, 255, 200, 40));
  drawParticles(window, sf::Color(100, 255, 100, 150)); // Green Mana
  drawEnemies(window);
}
