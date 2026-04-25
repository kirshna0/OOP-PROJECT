#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"

class Game;

class Level {
protected:
    bool hasSpawnedEnemies; // NEW: Ensures we don't win before spawning
    
    Game* game;
    bool completed;
    float levelTimer;
    sf::RectangleShape background;

    std::vector<std::unique_ptr<Enemy>> enemies;

    // FIX: Shared helper so Level1/2/3 don't duplicate identical update logic.
    // Handles enemy movement, player collision, spell collision, and death removal.
    void updateEnemies(float deltaTime);
    void drawGrid(sf::RenderWindow& window, sf::Color color);
    void drawTiles(sf::RenderWindow& window, sf::Color color1, sf::Color color2);
    void drawParticles(sf::RenderWindow& window, sf::Color color);
    void drawEnemies(sf::RenderWindow& window);

public:
    Level(Game* g);
    virtual ~Level() = default;

    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    bool isCompleted() const;
};

#endif // LEVEL_H
