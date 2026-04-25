#include "SaveManager.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

//  Constructor
SaveManager::SaveManager()
    : saveFile("asta_adventure_save.txt"),
      highScoreFile("asta_highscore.txt"),
      logFile("asta_combat_log.txt") {}

// saveGame
// Writes all GameState fields to the save file (overwrites previous save).
bool SaveManager::saveGame(const GameState& state) {
    std::ofstream out(saveFile);
    if (!out.is_open()) {
        std::cerr << "[SaveManager] ERROR: Could not open save file for writing.\n";
        return false;
    }

    out << "level="        << state.currentLevel << "\n";
    out << "health="       << state.playerHealth  << "\n";
    out << "total_kills="  << state.totalKills    << "\n";
    out << "total_deaths=" << state.totalDeaths   << "\n";
    out << "total_wins="   << state.totalWins     << "\n";
    out.close();

    std::cout << "[Save] Level: " << state.currentLevel
              << "  HP: "         << state.playerHealth
              << "  Kills: "      << state.totalKills
              << "  Deaths: "     << state.totalDeaths
              << "  Wins: "       << state.totalWins << "\n";
    return true;
}

// loadGame 
// Reads key=value pairs from the save file. Unknown keys are ignored.
bool SaveManager::loadGame(GameState& state) {
    std::ifstream in(saveFile);
    if (!in.is_open()) {
        std::cout << "[SaveManager] No save file found — starting fresh.\n";
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        auto sep = line.find('=');
        if (sep == std::string::npos) continue;
        std::string key   = line.substr(0, sep);
        std::string value = line.substr(sep + 1);
        try {
            if      (key == "level")        state.currentLevel = std::stoi(value);
            else if (key == "health")       state.playerHealth = std::stoi(value);
            else if (key == "total_kills")  state.totalKills   = std::stoi(value);
            else if (key == "total_deaths") state.totalDeaths  = std::stoi(value);
            else if (key == "total_wins")   state.totalWins    = std::stoi(value);
        } catch (...) {
            std::cerr << "[SaveManager] Warning: could not parse line: " << line << "\n";
        }
    }
    in.close();

    // Validate critical fields
    if (state.currentLevel < 1 || state.currentLevel > 3) {
        std::cerr << "[SaveManager] Invalid level in save (" << state.currentLevel << "). Resetting to 1.\n";
        state.currentLevel = 1;
    }
    if (state.playerHealth < 1 || state.playerHealth > 100) {
        std::cerr << "[SaveManager] Invalid health in save (" << state.playerHealth << "). Resetting to 100.\n";
        state.playerHealth = 100;
    }

    std::cout << "[Load] Level: " << state.currentLevel
              << "  HP: "         << state.playerHealth
              << "  Kills: "      << state.totalKills
              << "  Deaths: "     << state.totalDeaths
              << "  Wins: "       << state.totalWins << "\n";
    return true;
}

// saveHighScore 
bool SaveManager::saveHighScore(const HighScore& score) {
    std::ofstream out(highScoreFile);
    if (!out.is_open()) {
        std::cerr << "[SaveManager] ERROR: Could not open high score file.\n";
        return false;
    }
    // Never persist INT_MAX — write 0 as a safe sentinel for "not yet set"
    int fewestToWrite = (score.fewestDeaths == INT_MAX) ? 0 : score.fewestDeaths;
    out << "best_kills="       << score.bestKills    << "\n";
    out << "best_health="      << score.bestHealth   << "\n";
    out << "fewest_deaths="    << fewestToWrite      << "\n";
    out.close();
    std::cout << "[HighScore] Saved — Kills: " << score.bestKills
              << "  HP Left: " << score.bestHealth
              << "  Deaths: "  << fewestToWrite << "\n";
    return true;
}

// loadHighScore 
bool SaveManager::loadHighScore(HighScore& score) {
    std::ifstream in(highScoreFile);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        auto sep = line.find('=');
        if (sep == std::string::npos) continue;
        std::string key   = line.substr(0, sep);
        std::string value = line.substr(sep + 1);
        try {
            if      (key == "best_kills")    score.bestKills    = std::stoi(value);
            else if (key == "best_health")   score.bestHealth   = std::stoi(value);
            else if (key == "fewest_deaths") {
                int v = std::stoi(value);
                // 0 on disk means "never set" — restore sentinel so comparisons work
                score.fewestDeaths = (v == 0 && score.bestKills == 0) ? INT_MAX : v;
            }
        } catch (...) {}
    }
    in.close();
    return true;
}

// updateHighScore 
// Called after a win. Updates the high score file if this run was better.
void SaveManager::updateHighScore(const GameState& state) {
    HighScore existing;
    loadHighScore(existing);

    bool improved = false;
    if (state.totalKills    > existing.bestKills)    { existing.bestKills    = state.totalKills;    improved = true; }
    if (state.playerHealth  > existing.bestHealth)   { existing.bestHealth   = state.playerHealth;  improved = true; }
    if (state.totalDeaths   < existing.fewestDeaths) { existing.fewestDeaths = state.totalDeaths;   improved = true; }

    if (improved) {
        std::cout << "[HighScore] New record!\n";
        saveHighScore(existing);
        logEvent("[HIGH SCORE] New record set!");
    }
}

// logEvent 
// Appends a single line to the combat log with a timestamp.
void SaveManager::logEvent(const std::string& event) {
    std::ofstream log(logFile, std::ios::app); // append mode
    if (!log.is_open()) return;

    // Get current time as a simple timestamp string
    auto now   = std::chrono::system_clock::now();
    std::time_t t   = std::chrono::system_clock::to_time_t(now);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));

    log << "[" << buf << "] " << event << "\n";
    log.close();
}

// logSessionStart 
void SaveManager::logSessionStart() {
    auto now  = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char buf[30];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    std::ofstream log(logFile, std::ios::app);
    if (!log.is_open()) return;
    log << "\n========== SESSION STARTED: " << buf << " ==========\n";
    log.close();
}

// logSessionEnd
void SaveManager::logSessionEnd(const GameState& state) {
    logEvent("Session ended — Level: " + std::to_string(state.currentLevel) +
             "  HP: "     + std::to_string(state.playerHealth) +
             "  Kills: "  + std::to_string(state.totalKills)   +
             "  Deaths: " + std::to_string(state.totalDeaths)  +
             "  Wins: "   + std::to_string(state.totalWins));
    std::ofstream log(logFile, std::ios::app);
    if (log.is_open()) log << "========== SESSION ENDED ==========\n";
}
