#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <climits>

// Game Save State
// Stores all persistent player data between sessions.
struct GameState {
    int currentLevel   = 1;
    int playerHealth   = 100;
    int totalKills     = 0;   // enemies defeated across all sessions
    int totalDeaths    = 0;   // times the player has died
    int totalWins      = 0;   // times the player has beaten the game
};

//High Score Record 
struct HighScore {
    int bestKills      = 0;   // most enemies defeated in a single win run
    int bestHealth     = 0;   // highest health remaining on victory
    int fewestDeaths   = INT_MAX; // least deaths in a winning run (INT_MAX so any first win beats it)
};

// SaveManager 
// Demonstrates File I/O (OOP requirement):
//   - saveGame()      writes progress to asta_adventure_save.txt
//   - loadGame()      reads and validates progress
//   - saveHighScore() writes best-run stats to asta_highscore.txt
//   - loadHighScore() reads best-run stats
//   - logEvent()      appends a timestamped event to asta_combat_log.txt
class SaveManager {
private:
    std::string saveFile;
    std::string highScoreFile;
    std::string logFile;

public:
    SaveManager();

    // Core save / load
    bool saveGame(const GameState& state);
    bool loadGame(GameState& state);

    // High score tracking
    bool saveHighScore(const HighScore& score);
    bool loadHighScore(HighScore& score);
    void updateHighScore(const GameState& state);

    // Combat log (append-only)
    void logEvent(const std::string& event);
    void logSessionStart();
    void logSessionEnd(const GameState& state);
};

#endif // SAVEMANAGER_H
