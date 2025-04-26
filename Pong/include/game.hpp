#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "GameState.hpp"
#include "SoundManager.hpp"

class Game {
private:
    const int screenWidth;
    const int screenHeight;
    
    Ball ball;
    Paddle player;
    Paddle computer;
    GameState state;
    SoundManager soundManager;
    
    void HandleInput();
    void UpdateGameplay();
    void CheckCollisions();
    void DrawLogo() const;
    void DrawTitle() const;
    void DrawGameplay() const;
    void DrawEnding() const;

public:
    Game(int width, int height);
    ~Game();
    
    void Run();
};

#endif