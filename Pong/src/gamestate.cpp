#include "gamestate.hpp"

GameState::GameState() 
    : currentScreen(GameScreen::LOGO), framesCounter(0), letterCount(0), state(0),
      pause(true), gameStarted(false), playerScore(0), computerScore(0) {}

void GameState::Update() {
    if (currentScreen == GameScreen::LOGO) {
        if (state == 0) {
            framesCounter++;
            if (framesCounter/12) {
                letterCount++;
                framesCounter = 0;
                if (letterCount > 8) {
                    state = 1;
                }
            }
        } else if(state == 1) {
            framesCounter++;
            if (framesCounter > 120) {
                currentScreen = GameScreen::TITLE;
            }
        }
    }
}

void GameState::ResetGame() {
    playerScore = 0;
    computerScore = 0;
    pause = true;
    gameStarted = false;
}

// Getters
GameScreen GameState::GetCurrentScreen() const { return currentScreen; }
bool GameState::IsPaused() const { return pause; }
bool GameState::IsGameStarted() const { return gameStarted; }
int GameState::GetPlayerScore() const { return playerScore; }
int GameState::GetComputerScore() const { return computerScore; }

// Setters
void GameState::SetCurrentScreen(GameScreen screen) { currentScreen = screen; }
void GameState::SetPause(bool paused) { pause = paused; }
void GameState::IncrementPlayerScore() { playerScore++; }
void GameState::IncrementComputerScore() { computerScore++; }