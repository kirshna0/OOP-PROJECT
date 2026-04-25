#include "Game.h"
#include <SFML/Graphics.hpp>
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "AntiMagicSword.h"
#include "WindMagic.h"
#include "FireMagic.h"
#include <iostream>
#include <optional>
#include <algorithm>
#include <cstdint>
#include <cmath>

Game::Game()
    : window(sf::VideoMode({static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT)}), "Black Clover Game"),
      player(nullptr),
      currentLevel(nullptr),
      currentLevelIndex(1),
      currentScreenState(Game::ScreenState::Menu),
      levelIntroTimer(0.f),
      gameKills(0),
      gameDeaths(0),
      gameWins(0),
      loseTimer(0.f),
      winTimer(0.f),
      menuTimer(0.f),
      titleText(font),
      startText(font),
      exitText(font),
      menuBarText(font),
      killCountText(font),
      menuBtnText(font),
      saveBtnText(font)
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/font.ttf")) {
        std::cerr << "Failed to load font!\n";
    }

    titleText.setFont(font);
    startText.setFont(font);
    exitText.setFont(font);
    menuBarText.setFont(font);
    killCountText.setFont(font);
    menuBtnText.setFont(font);
    saveBtnText.setFont(font);
    titleText.setString("BLACK CLOVER");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(200.f, 150.f));

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::Green);
    startText.setPosition(sf::Vector2f(300.f, 300.f));

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::Red);
    exitText.setPosition(sf::Vector2f(360.f, 400.f));
    
    // Initialize Menu Bar UI
    menuBarBackground.setSize({WINDOW_WIDTH, 50.f});
    menuBarBackground.setFillColor(sf::Color(20, 20, 30, 200)); // Glassy dark
    menuBarBackground.setOutlineColor(sf::Color(100, 100, 255, 150));
    menuBarBackground.setOutlineThickness(1.f);

    menuBtn.setSize({100.f, 30.f});
    menuBtn.setFillColor(sf::Color(60, 60, 80));
    menuBtn.setOutlineColor(sf::Color::White);
    menuBtn.setOutlineThickness(1.f);
    menuBtn.setPosition({680.f, 10.f});

    menuBtnText.setString("MENU");
    menuBtnText.setCharacterSize(18);
    menuBtnText.setFillColor(sf::Color::White);

    saveBtn.setSize({100.f, 30.f});
    saveBtn.setFillColor(sf::Color(40, 100, 40));
    saveBtn.setOutlineColor(sf::Color::White);
    saveBtn.setOutlineThickness(1.f);
    saveBtn.setPosition({570.f, 10.f});

    saveBtnText.setString("SAVE");
    saveBtnText.setCharacterSize(18);
    saveBtnText.setFillColor(sf::Color::White);



    // Attempt to load save; validate values before using them.
    // If loadGame() returns false, `state` keeps its default-initialised values
    // (level=1, health=100, all counters=0) which is the correct fresh-start state.
    GameState state;
    if (saveManager.loadGame(state)) {
        if (state.currentLevel >= 1 && state.currentLevel <= 3)
            currentLevelIndex = state.currentLevel;
        else
            currentLevelIndex = 1;
    } else {
        currentLevelIndex = 1;
    }

    saveManager.logSessionStart();
    spawnPlayer(400.f, 300.f, state.playerHealth);
    // Store loaded stats
    gameKills  = state.totalKills;
    gameDeaths = state.totalDeaths;
    gameWins   = state.totalWins;
    loadLevel(currentLevelIndex);
}

// FIX: Helper that creates the player and restores their health properly
void Game::spawnPlayer(float x, float y, int health) {
    player = std::make_unique<Player>(x, y);
    player->addSpell(std::make_unique<AntiMagicSword>());
    player->addSpell(std::make_unique<WindMagic>());
    player->addSpell(std::make_unique<FireMagic>());
    if (health < 100) {
        int dmg = 100 - health;
        player->takeDamage(dmg);
    }
}

void Game::loadLevel(int levelIndex) {
    if (levelIndex > 3) {
        std::cout << "You Win!" << std::endl;
        gameWins++;
        GameState winState;
        winState.currentLevel = 1;  // reset for next playthrough
        winState.playerHealth = player->getHealth();
        winState.totalKills   = gameKills;
        winState.totalDeaths  = gameDeaths;
        winState.totalWins    = gameWins;
        saveManager.saveGame(winState);
        saveManager.updateHighScore(winState);
        saveManager.logEvent("YOU WIN! HP remaining: " + std::to_string(winState.playerHealth) +
                             "  Total kills: " + std::to_string(gameKills));
        winTimer = 0.f;
        currentScreenState = Game::ScreenState::Victory;
        levelIntroTimer = 0.f;
        return;
    }

    currentLevelIndex = levelIndex;
    levelIntroTimer = 2.0f; // Show "Level X" for 2 seconds

    if (levelIndex == 1) {
        currentLevel = std::make_unique<Level1>(this);
    } else if (levelIndex == 2) {
        currentLevel = std::make_unique<Level2>(this);
    } else if (levelIndex == 3) {
        currentLevel = std::make_unique<Level3>(this);
    }
    
    if (currentLevel) currentLevel->init();

    // FIX: If the player is dead when loading a level, respawn them with full health
    // to prevent an infinite death loop on level load/resume.
    if (!player || player->isDead()) {
        spawnPlayer(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, 100);
    }

    GameState state;
    state.currentLevel = currentLevelIndex;
    state.playerHealth = player->getHealth();
    state.totalKills   = gameKills;
    state.totalDeaths  = gameDeaths;
    state.totalWins    = gameWins;
    saveManager.saveGame(state);
}

void Game::nextLevel() {
    if (player) player->heal(player->getMaxHealth());
    loadLevel(currentLevelIndex + 1);
}

Player* Game::getPlayer() {
    return player.get();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            GameState finalState;
            finalState.currentLevel = currentLevelIndex;
            finalState.playerHealth = player ? player->getHealth() : 100;
            finalState.totalKills   = gameKills;
            finalState.totalDeaths  = gameDeaths;
            finalState.totalWins    = gameWins;
            saveManager.logSessionEnd(finalState);
            window.close();
        } else if (currentScreenState == Game::ScreenState::Menu || currentScreenState == Game::ScreenState::Victory || currentScreenState == Game::ScreenState::GameOver) {
            if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {
                    if (currentScreenState == Game::ScreenState::Victory || currentScreenState == Game::ScreenState::GameOver) {
                        if (currentScreenState == Game::ScreenState::Victory) currentLevelIndex = 1; 
                        currentScreenState = Game::ScreenState::Menu; 
                        return;
                    }
                    sf::Vector2f mousePos(mouseClick->position.x, mouseClick->position.y);
                    sf::FloatRect resumeBtnBounds({290.f, 220.f}, {220.f, 50.f});
                    sf::FloatRect newGameBtnBounds({290.f, 290.f}, {220.f, 50.f});
                    sf::FloatRect exitBtnBounds({290.f, 360.f}, {220.f, 50.f});

                    if (resumeBtnBounds.contains(mousePos)) {
                        currentScreenState = Game::ScreenState::Playing;
                        loadLevel(currentLevelIndex); 
                    } else if (newGameBtnBounds.contains(mousePos)) {
                        spawnPlayer(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, 100);
                        loadLevel(1);
                        currentScreenState = Game::ScreenState::Playing;
                    } else if (exitBtnBounds.contains(mousePos)) {
                        window.close();
                    }
                }
            }
        } else if (currentScreenState == Game::ScreenState::Playing) {
            if (player) {
                player->handleEvent(*event);
            }
            if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos(mouseClick->position.x, mouseClick->position.y);
                    if (menuBtn.getGlobalBounds().contains(mousePos)) {
                        currentScreenState = Game::ScreenState::Menu;
                        return;
                    } else if (saveBtn.getGlobalBounds().contains(mousePos)) {
                        GameState state;
                        state.currentLevel = currentLevelIndex;
                        state.playerHealth = player->getHealth();
                        state.totalKills   = gameKills;
                        state.totalDeaths  = gameDeaths;
                        state.totalWins    = gameWins;
                        saveManager.saveGame(state);
                        saveManager.logEvent("Manual Save Triggered");
                    }
                }
            }
            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->scancode == sf::Keyboard::Scancode::Space) {
                    // FIX: Spell now fires toward the mouse cursor instead of always going right
                    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);
                    sf::Vector2f direction = mousePos - player->getPosition();
                    player->castSpell(direction);
                } else if (keyPress->scancode == sf::Keyboard::Scancode::Q) {
                    player->switchSpell();
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (currentScreenState != Game::ScreenState::Playing) return;

    if (levelIntroTimer > 0) {
        levelIntroTimer -= deltaTime;
        return; // Pause gameplay during intro
    }

    if (player) {
        player->update(deltaTime);
        // DEBUG: Print player position
        // std::cout << "Asta Position: " << player->getPosition().x << ", " << player->getPosition().y << "\n";
        
        if (player->isDead()) {
            std::cout << "Player died!\n";
            gameDeaths++;
            saveManager.logEvent("Player died on Level " + std::to_string(currentLevelIndex));
            loseTimer = 0.f;
            currentScreenState = Game::ScreenState::GameOver;
            return;
        }
    }

    if (currentLevel) {
        currentLevel->update(deltaTime);
        if (currentLevel->isCompleted()) {
            nextLevel();
        }
    }
}

void Game::renderMenu() {
    window.clear(sf::Color(5, 5, 15)); // Deep space black-blue

    menuTimer += 0.015f;

    // 1. Draw "Galaxy" Starfield
    for (int i = 0; i < 100; ++i) {
        float x = (float)((i * 797) % (int)WINDOW_WIDTH);
        float y = (float)((i * 919) % (int)WINDOW_HEIGHT);
        float brightness = 150.f + std::sin(menuTimer * 2.f + i) * 100.f;
        sf::CircleShape star(1.5f);
        star.setPosition({x, y});
        star.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(std::clamp(brightness, 0.f, 255.f))));
        window.draw(star);
    }

    // 2. Draw "Nebula Clouds"
    for (int i = 0; i < 4; ++i) {
        sf::CircleShape nebula(150.f);
        nebula.setOrigin({150.f, 150.f});
        nebula.setPosition({200.f + i * 150.f, 300.f + std::cos(menuTimer + i) * 30.f});
        nebula.setFillColor(sf::Color(100 + i * 30, 50, 255, 20)); // Soft purple/blue
        window.draw(nebula);
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // 3. COLOR-CYCLING TITLE
    uint8_t r = static_cast<uint8_t>(127 + 127 * std::sin(menuTimer));
    uint8_t g = static_cast<uint8_t>(127 + 127 * std::sin(menuTimer + 2.0f));
    titleText.setString("BLACK CLOVER RPG");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color(r, g, 255));
    titleText.setOutlineColor(sf::Color::White);
    titleText.setOutlineThickness(2.f);
    titleText.setPosition({WINDOW_WIDTH / 2.f - titleText.getGlobalBounds().size.x / 2.f, 100.f});
    window.draw(titleText);

    // 4. BUTTONS
    sf::RectangleShape resumeBtn({220.f, 50.f});
    resumeBtn.setPosition({290.f, 220.f});
    bool resumeHover = resumeBtn.getGlobalBounds().contains(mousePos);
    resumeBtn.setFillColor(resumeHover ? sf::Color(40, 180, 40, 200) : sf::Color(20, 80, 20, 150));
    resumeBtn.setOutlineColor(sf::Color(100, 255, 100));
    resumeBtn.setOutlineThickness(1.f);
    window.draw(resumeBtn);

    sf::Text resumeText(font);
    resumeText.setString("RESUME");
    resumeText.setCharacterSize(25);
    resumeText.setPosition({WINDOW_WIDTH / 2.f - resumeText.getGlobalBounds().size.x / 2.f, 230.f});
    window.draw(resumeText);

    sf::RectangleShape newGameBtn({220.f, 50.f});
    newGameBtn.setPosition({290.f, 290.f});
    bool newGameHover = newGameBtn.getGlobalBounds().contains(mousePos);
    newGameBtn.setFillColor(newGameHover ? sf::Color(40, 40, 180, 200) : sf::Color(20, 20, 80, 150));
    newGameBtn.setOutlineColor(sf::Color(100, 100, 255));
    newGameBtn.setOutlineThickness(1.f);
    window.draw(newGameBtn);

    sf::Text newGameText(font);
    newGameText.setString("NEW GAME");
    newGameText.setCharacterSize(25);
    newGameText.setPosition({WINDOW_WIDTH / 2.f - newGameText.getGlobalBounds().size.x / 2.f, 300.f});
    window.draw(newGameText);

    // 5. EXIT BUTTON
    sf::RectangleShape exitBtn({220.f, 50.f});
    exitBtn.setPosition({290.f, 360.f});
    bool exitHover = exitBtn.getGlobalBounds().contains(mousePos);
    exitBtn.setFillColor(exitHover ? sf::Color(180, 40, 40, 200) : sf::Color(80, 20, 20, 150));
    exitBtn.setOutlineColor(sf::Color(255, 100, 100));
    exitBtn.setOutlineThickness(2.f); 
    window.draw(exitBtn);

    sf::Text exitTxt(font);
    exitTxt.setString("EXIT GAME");
    exitTxt.setCharacterSize(25);
    exitTxt.setPosition({WINDOW_WIDTH / 2.f - exitTxt.getGlobalBounds().size.x / 2.f, 370.f});
    window.draw(exitTxt);

    window.display();
}

void Game::renderMenuBar() {
    window.draw(menuBarBackground);

    // Level Text
    menuBarText.setString("LEVEL: " + std::to_string(currentLevelIndex));
    menuBarText.setCharacterSize(20);
    menuBarText.setFillColor(sf::Color::Yellow);
    menuBarText.setPosition({20.f, 12.f});
    window.draw(menuBarText);

    // Kill Count
    killCountText.setString("KILLS: " + std::to_string(gameKills));
    killCountText.setCharacterSize(20);
    killCountText.setFillColor(sf::Color::White);
    killCountText.setPosition({150.f, 12.f});
    window.draw(killCountText);

    // Menu Button
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool hover = menuBtn.getGlobalBounds().contains(mousePos);
    menuBtn.setFillColor(hover ? sf::Color(100, 100, 150) : sf::Color(40, 40, 60));
    window.draw(menuBtn);

    menuBtnText.setPosition({menuBtn.getPosition().x + (100.f - menuBtnText.getGlobalBounds().size.x) / 2.f, 15.f});
    window.draw(menuBtnText);

    // Save Button
    bool saveHover = saveBtn.getGlobalBounds().contains(mousePos);
    saveBtn.setFillColor(saveHover ? sf::Color(60, 150, 60) : sf::Color(30, 80, 30));
    window.draw(saveBtn);

    saveBtnText.setPosition({saveBtn.getPosition().x + (100.f - saveBtnText.getGlobalBounds().size.x) / 2.f, 15.f});
    window.draw(saveBtnText);
}

void Game::renderGameOver() {
    window.clear(sf::Color(10, 0, 0)); // Almost pitch black

    loseTimer += 0.02f;

    // Dramatic pulsing red vignette effect
    for (int i = 5; i > 0; --i) {
        sf::CircleShape vignette(150.f * i);
        vignette.setOrigin({150.f * i, 150.f * i});
        vignette.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
        vignette.setFillColor(sf::Color(40 - i * 5, 0, 0, 50));
        window.draw(vignette);
    }

    // Floating blood particles
    for (int i = 0; i < 30; ++i) {
        float x = (float)((i * 313) % 800);
        float y = std::fmod((i * 123) + loseTimer * 50.f, 600.f);
        sf::CircleShape drop(2.f + (i % 3));
        drop.setFillColor(sf::Color(150, 0, 0, 200));
        drop.setPosition({x, y});
        window.draw(drop);
    }

    sf::Text loseText(font);
    loseText.setString("Y O U   D I E D");
    loseText.setCharacterSize(90);
    // Pulse alpha and scale
    float scale = 1.0f + 0.05f * std::sin(loseTimer * 3.f);
    loseText.setScale({scale, scale});
    loseText.setFillColor(sf::Color(255, 0, 0, static_cast<uint8_t>(200 + 55 * std::sin(loseTimer * 5.f))));
    loseText.setOutlineColor(sf::Color(50, 0, 0));
    loseText.setOutlineThickness(4.f);
    
    sf::FloatRect bounds = loseText.getLocalBounds();
    loseText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    loseText.setPosition({WINDOW_WIDTH / 2.f, 200.f});
    window.draw(loseText);

    // Stats
    sf::Text statsText(font);
    statsText.setString("Kills This Run: " + std::to_string(gameKills));
    statsText.setCharacterSize(25);
    statsText.setFillColor(sf::Color(200, 100, 100));
    bounds = statsText.getLocalBounds();
    statsText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    statsText.setPosition({WINDOW_WIDTH / 2.f, 350.f});
    window.draw(statsText);

    sf::Text subText(font);
    subText.setString("- Click anywhere to resurrect -");
    subText.setCharacterSize(22);
    subText.setFillColor(sf::Color(150, 150, 150, static_cast<uint8_t>(150 + 100 * std::sin(loseTimer * 2.f))));
    bounds = subText.getLocalBounds();
    subText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    subText.setPosition({WINDOW_WIDTH / 2.f, 450.f});
    window.draw(subText);

    window.display();
}

void Game::renderVictory() {
    window.clear(sf::Color(10, 20, 40)); // Deep magic blue

    winTimer += 0.03f;

    // Draw magical sunburst / rotating aura
    for (int i = 0; i < 12; ++i) {
        sf::ConvexShape beam(3);
        beam.setPoint(0, {0.f, -50.f});
        beam.setPoint(1, {20.f, 600.f});
        beam.setPoint(2, {-20.f, 600.f});
        beam.setFillColor(sf::Color(255, 215, 0, 20));
        beam.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
        beam.setRotation(sf::degrees(i * 30.f + winTimer * 20.f));
        window.draw(beam);
    }

    // Dynamic Rising Magic Sparks
    for (int i = 0; i < 60; ++i) {
        float x = (float)((i * 123) % 800);
        float startY = (float)((i * 456) % 600);
        float y = std::fmod(startY - winTimer * (30.f + (i % 50)), 600.f);
        if (y < 0) y += 600.f;
        sf::CircleShape p(2.f + (i % 4));
        if (i % 2 == 0) p.setFillColor(sf::Color(255, 215, 0, 200)); // Gold
        else            p.setFillColor(sf::Color(0, 255, 255, 200)); // Cyan
        p.setPosition({x, y});
        window.draw(p);
    }

    sf::Text winText(font);
    winText.setString("V I C T O R Y");
    winText.setCharacterSize(90);
    float bounce = std::abs(std::sin(winTimer * 3.f)) * 10.f;
    winText.setPosition({WINDOW_WIDTH / 2.f - winText.getLocalBounds().size.x / 2.f, 120.f - bounce});
    
    // Rainbow text color effect
    uint8_t r = static_cast<uint8_t>(127 + 127 * std::sin(winTimer));
    uint8_t g = static_cast<uint8_t>(127 + 127 * std::sin(winTimer + 2.f));
    uint8_t b = static_cast<uint8_t>(127 + 127 * std::sin(winTimer + 4.f));
    winText.setFillColor(sf::Color(r, g, b));
    winText.setOutlineColor(sf::Color::White);
    winText.setOutlineThickness(3.f);
    window.draw(winText);

    sf::Text subTextWin(font);
    subTextWin.setString("WIZARD KING SECURED");
    subTextWin.setCharacterSize(35);
    subTextWin.setFillColor(sf::Color::Yellow);
    subTextWin.setPosition({WINDOW_WIDTH / 2.f - subTextWin.getLocalBounds().size.x / 2.f, 250.f});
    window.draw(subTextWin);

    // Stats Box
    sf::RectangleShape statsBox({300.f, 100.f});
    statsBox.setPosition({WINDOW_WIDTH / 2.f - 150.f, 330.f});
    statsBox.setFillColor(sf::Color(0, 0, 0, 150));
    statsBox.setOutlineColor(sf::Color::Yellow);
    statsBox.setOutlineThickness(2.f);
    window.draw(statsBox);

    sf::Text statsText(font);
    int finalHp = player ? player->getHealth() : 0;
    statsText.setString("Kills: " + std::to_string(gameKills) + "\nRemaining HP: " + std::to_string(finalHp));
    statsText.setCharacterSize(25);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition({WINDOW_WIDTH / 2.f - statsText.getLocalBounds().size.x / 2.f, 350.f});
    window.draw(statsText);

    sf::Text hint(font);
    hint.setString("- Click anywhere to continue your legend -");
    hint.setCharacterSize(22);
    hint.setFillColor(sf::Color(200, 255, 200, static_cast<uint8_t>(150 + 100 * std::sin(winTimer * 4.f))));
    hint.setPosition({WINDOW_WIDTH / 2.f - hint.getLocalBounds().size.x / 2.f, 500.f});
    window.draw(hint);

    window.display();
}

void Game::render() {
    if (currentScreenState == Game::ScreenState::Menu) {
        renderMenu();
    } else if (currentScreenState == Game::ScreenState::Victory) {
        renderVictory();
    } else if (currentScreenState == Game::ScreenState::GameOver) {
        renderGameOver();
    } else {
        window.clear();
        if (currentLevel) {
            currentLevel->draw(window);
        }
        if (player) {
            player->draw(window);
        }

        renderMenuBar();
        
        // UI: Health Bar (Moved below menu bar)
        sf::RectangleShape healthBarBack({200.f, 15.f});
        healthBarBack.setPosition({20.f, 60.f});
        healthBarBack.setFillColor(sf::Color(50, 50, 50, 180));
        window.draw(healthBarBack);

        float healthPercent = (float)player->getHealth() / 100.f;
        sf::RectangleShape healthBarFront({200.f * healthPercent, 15.f});
        healthBarFront.setPosition({20.f, 60.f});
        healthBarFront.setFillColor(sf::Color(255, 50, 50));
        window.draw(healthBarFront);

        // UI: Current Spell
        sf::Text spellText(font);
        spellText.setString("SPELL: " + player->getCurrentSpellName());
        spellText.setCharacterSize(18);
        spellText.setPosition({240.f, 58.f});
        spellText.setFillColor(sf::Color::Cyan);
        window.draw(spellText);

        if (levelIntroTimer > 0) {
            sf::RectangleShape overlay({WINDOW_WIDTH, WINDOW_HEIGHT});
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            sf::Text levelText(font);
            levelText.setString("KINGDOM LEVEL " + std::to_string(currentLevelIndex));
            levelText.setCharacterSize(60);
            levelText.setFillColor(sf::Color::Yellow);
            levelText.setOutlineColor(sf::Color::Red);
            levelText.setOutlineThickness(3.f);
            levelText.setPosition({WINDOW_WIDTH / 2.f - levelText.getGlobalBounds().size.x / 2.f, 250.f});
            window.draw(levelText);
        }

        window.display();
    }
}
