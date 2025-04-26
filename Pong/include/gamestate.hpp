#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "raylib.h"

enum class GameScreen { LOGO, TITLE, GAMEPLAY, ENDING };

class GameState {
private:
    GameScreen currentScreen;
    int framesCounter;
    int letterCount;
    int state;
    bool pause;
    bool gameStarted;
    int playerScore;
    int computerScore;

public:
    GameState();
    
    void Update();
    void ResetGame();
    
    // Getters
    GameScreen GetCurrentScreen() const;
    bool IsPaused() const;
    bool IsGameStarted() const;
    int GetPlayerScore() const;
    int GetComputerScore() const;
    
    // Setters
    void SetCurrentScreen(GameScreen screen);
    void SetPause(bool paused);
    void IncrementPlayerScore();
    void IncrementComputerScore();
};

#endif