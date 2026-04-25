#  Black Clover Adventure

> A 2D top-down action game built with **C++** and **SFML 3**, inspired by the *Black Clover* anime/manga series.

---

## 📋 Table of Contents
- [Project Description](#-project-description)
- [Developer Information](#-developer-information)
- [Use Cases](#-use-cases)
- [How to Compile](#-how-to-compile)
- [How to Run](#-how-to-run)
- [How to Use](#-how-to-use)
- [Assumptions & Limitations](#-assumptions--limitations)
- [Project File Structure](#-project-file-structure)

---

## 📖 Project Description

**Black Clover Adventure** is a 2D top-down action game developed in **C++** using the **SFML 3** graphics library. It was created as a semester project for the Object-Oriented Programming course at FAST-NUCES.

The player controls **Asta** — a magic-less warrior who wields an Anti-Magic Sword — and battles through three escalating levels, each themed after a major faction from the anime:

| Level | Theme | Boss |
|-------|-------|------|
| Level 1 | Clover Kingdom (Emerald & Stone) | — |
| Level 2 | Diamond Kingdom (Molten Red & Obsidian) | Mars |
| Level 3 | Midnight Sun (Void Purple & Galaxy Black) | Vetto the Despair |

### OOP Concepts Demonstrated

- **Inheritance & Polymorphism** — `Entity` base class extended by `Player` and `Enemy`; `Spell` base class extended by `AntiMagicSword`, `FireMagic`, and `WindMagic`; `Level` base class extended by `Level1`, `Level2`, `Level3`.
- **Abstraction** — Pure virtual `update()` and `draw()` methods enforced across all `Entity` and `Level` subclasses.
- **Encapsulation** — All game state, spell data, and save data managed through well-defined class interfaces.
- **File I/O** — `SaveManager` handles persistent save files, high scores, and a timestamped combat log.
- **Dynamic Memory** — `std::unique_ptr` used throughout for safe heap management of enemies and spells.

---

##  Developer Information

| Name | Student ID | Role |
|------|------------|------|
| Kirshna Kumari | 25K-0519 | Sole Developer (Individual Project) |

**Course:** Object-Oriented Programming — FAST-NUCES | Spring 2026

---

##  Use Cases

| ID | Use Case | Description |
|----|----------|-------------|
| UC-01 | Start a New Game | Player launches the game, selects Start from the main menu, and begins on Level 1 of the Clover Kingdom. |
| UC-02 | Move the Player Character | Player uses W/A/S/D keys to navigate Asta around the screen, avoiding enemies and repositioning for attacks. |
| UC-03 | Cast a Spell | Player left-clicks to fire the active spell toward the cursor direction, dealing damage to enemies. |
| UC-04 | Switch Active Spell | Player presses Q to cycle between equipped spells, allowing strategic choice between high-speed or high-damage attacks. |
| UC-05 | Progress Through Levels | Upon defeating all enemies in a level, the game automatically transitions to the next level with increasing difficulty. |
| UC-06 | Save Game Progress | Player clicks the Save button during gameplay; current level, health, kills, deaths, and wins are written to `asta_adventure_save.txt`. |
| UC-07 | Load a Saved Game | On startup, if a save file exists, the game loads the last saved state, restoring the player's level and cumulative statistics. |
| UC-08 | Win the Game | Player defeats all enemies across all 3 levels. A Victory screen is shown, the high score is updated, and the win is logged. |
| UC-09 | View High Score | High score data (best kills, best health remaining, fewest deaths) is persisted in `asta_highscore.txt` and displayed on the Victory screen. |
| UC-10 | Review Combat Log | After sessions, the player can open `asta_combat_log.txt` to review timestamped events: deaths, wins, session starts/ends, and high score achievements. |

---

## 🔧 How to Compile

### Prerequisites

- C++17-compatible compiler (GCC 9+, Clang 10+, or MSVC 2019+)
- CMake 3.10 or higher
- SFML 3.0 (Graphics, Window, System, Audio)

### Install SFML

```bash
# macOS (Homebrew)
brew install sfml

# Ubuntu / Debian
sudo apt install libsfml-dev
```

### Build Steps

1. Clone or extract the project so all `.cpp`/`.h` files and the `assets/` folder are in the same directory.

2. Open a terminal in the project root and run:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

> The `CMakeLists.txt` automatically copies the `assets/` folder into the build output directory after compilation.

---

## How to Run

After a successful build, run the executable from inside the `build/` directory:

```bash
# macOS / Linux
./BlackCloverGame

# Windows
BlackCloverGame.exe
```

> The executable **must** be run from the directory that contains the `assets/` folder, otherwise textures will not load.

### Save Files (created automatically in the working directory)

| File | Purpose |
|------|---------|
| `asta_adventure_save.txt` | Persistent save data (level, health, kills, deaths, wins) |
| `asta_highscore.txt` | Best run records |
| `asta_combat_log.txt` | Timestamped session events |

---

##  How to Use

### Main Menu

- Press **Enter** or click **Start** to begin a new game.
- Press **Escape** or click **Exit** to quit.
- If a save file exists, your last saved level and stats are loaded automatically on startup.

### In-Game Controls

| Key / Input | Action |
|-------------|--------|
| `W` `A` `S` `D` | Move Asta up / left / down / right |
| `Left Mouse Click` | Cast the active spell toward the cursor |
| `Q` | Switch between equipped spells (Anti-Magic Sword / Fire Roar) |
| `Save Button (HUD)` | Save current game progress |
| `Menu Button (HUD)` | Return to the main menu |
| `Escape` | Quit the game |

### Gameplay Loop

1. Defeat all enemies in the current level to advance to the next.
2. Avoid enemy contact and their **Wind Blade** projectiles — both deal damage.
3. Your health bar is shown in the HUD. If it reaches **0**, it's Game Over.
4. Completing all 3 levels triggers the **Victory screen** and updates your high score.

### Spells

| Spell | Damage | Speed | Notes |
|-------|--------|-------|-------|
| Anti-Magic Sword | 50 | 700 px/s | Short range, expires after 0.5s |
| Fire Roar | 40 | 800 px/s | Fast projectile, orange tint |
| Wind Blade *(enemy)* | 20 | 1000 px/s | Used by all enemies |

---

## Assumptions & Limitations

### Assumptions

- The `assets/` folder must be present alongside the executable and contain `player.png`, `enemy.png`, and `spell.png`. Textures will not render without these files.
- **SFML 3.0 is required specifically.** SFML 2.x is not compatible due to API changes (`sf::Sprite` construction, `sf::Vector2f::normalized()`, `direction.angle()`, `event.getIf<>()`).
- Save files are stored as plain text in the working directory. No encryption or anti-tamper protection is applied.
- The combat log (`asta_combat_log.txt`) grows indefinitely across sessions and is never automatically cleared.

### Limitations

- **Single-player only** — no multiplayer or network support.
- **3 levels only** — the level system supports extension but no Level 4+ content is implemented.
- All enemies share the same `WindMagic` spell and `enemy.png` sprite, differentiated only by colour tint and stats.
- **No audio** — no sound effects or music, despite `SFML::Audio` being linked.
- The game window is **fixed at 800 × 600 pixels** and is not resizable.
- There is **no pause menu** — the game can only be exited to the main menu or closed entirely.
- Player health **does not regenerate** between levels within a single session.

---

## 📁 Project File Structure

```
BlackCloverGame/
├── main.cpp
├── Game.h / Game.cpp              — Main game loop and screen states
├── Entity.h / Entity.cpp          — Abstract base for all game entities
├── Player.h / Player.cpp          — Player character (Asta)
├── Enemy.h / Enemy.cpp            — Enemy AI and combat
├── Spell.h / Spell.cpp            — Abstract base for all spells
├── AntiMagicSword.h / .cpp        — Player primary attack
├── FireMagic.h / .cpp             — Player secondary spell
├── WindMagic.h / .cpp             — Enemy projectile
├── Level.h / Level.cpp            — Abstract base for levels
├── Level1.h / .cpp                — Clover Kingdom
├── Level2.h / .cpp                — Diamond Kingdom
├── Level3.h / .cpp                — Midnight Sun
├── SaveManager.h / SaveManager.cpp — File I/O (save, load, log)
├── CMakeLists.txt
└── assets/
    ├── player.png
    ├── enemy.png
    └── spell.png
```

---

*FAST-NUCES | Object-Oriented Programming | Spring 2026*
