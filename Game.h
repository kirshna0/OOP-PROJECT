#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "Player.h"
#include "Level.h"
#include "SaveManager.h"

class Game {
public:
    enum class ScreenState { Menu, Playing, Victory, GameOver };
    
    // Window Constants
    static constexpr float WINDOW_WIDTH = 800.f;
    static constexpr float WINDOW_HEIGHT = 600.f;

    Game();
    ~Game() = default;

    void run();
    void nextLevel();
    void addKill() { gameKills++; }
    
    Player* getPlayer();

private:
    // SFML Core
    sf::RenderWindow window;
    sf::Font font;

    // Game Objects
    std::unique_ptr<Player> player;
    std::unique_ptr<Level> currentLevel;
    SaveManager saveManager;

    // Game State
    int currentLevelIndex;
    ScreenState currentScreenState;
    float levelIntroTimer;
    
    // Statistics
    int gameKills;
    int gameDeaths;
    int gameWins;

    // Screen animation timers (member vars so they reset properly between states)
    float loseTimer;
    float winTimer;
    float menuTimer;

    // UI Elements
    sf::Text titleText;
    sf::Text startText;
    sf::Text exitText;
    sf::Text menuBarText;
    sf::Text killCountText;
    sf::RectangleShape menuBarBackground;
    sf::RectangleShape menuBtn;
    sf::Text menuBtnText;
    sf::RectangleShape saveBtn;
    sf::Text saveBtnText;

    // Internal Helpers
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void renderMenu();
    void renderMenuBar();
    void renderVictory();
    void renderGameOver();
    
    void loadLevel(int levelIndex);
    void spawnPlayer(float x, float y, int health = 100);
};

#endif // GAME_H
