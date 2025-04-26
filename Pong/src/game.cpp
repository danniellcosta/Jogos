#include "game.hpp"
#include <cmath>

Game::Game(int width, int height) 
    : screenWidth(width), screenHeight(height),
      ball(width, height),
      player(50, height/2 - 55, 20, 110),
      computer(width - 70, height/2 - 55, 20, 110, true) {}

Game::~Game() {
    CloseWindow();
}

void Game::Run() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Pong Game - OOP");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        state.Update();
        HandleInput();
        
        if (state.GetCurrentScreen() == GameScreen::GAMEPLAY && !state.IsPaused()) {
            UpdateGameplay();
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch (state.GetCurrentScreen()) {
            case GameScreen::LOGO: DrawLogo(); break;
            case GameScreen::TITLE: DrawTitle(); break;
            case GameScreen::GAMEPLAY: DrawGameplay(); break;
            case GameScreen::ENDING: DrawEnding(); break;
        }
        
        EndDrawing();
    }
}

void Game::HandleInput() {
    if (IsKeyPressed(KEY_SPACE) && state.GetCurrentScreen() == GameScreen::GAMEPLAY) {
        state.SetPause(!state.IsPaused());
    }
    
    if (state.GetCurrentScreen() == GameScreen::TITLE && 
        (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        state.SetCurrentScreen(GameScreen::GAMEPLAY);
    }
    
    if (state.GetCurrentScreen() == GameScreen::ENDING && 
        (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        state.ResetGame();
        ball.Reset(screenWidth, screenHeight);
        state.SetCurrentScreen(GameScreen::GAMEPLAY);
    }
}

void Game::UpdateGameplay() {
    ball.Update();
    
    // Move player paddle with mouse
    player.Move(GetMouseY(), screenHeight);
    
    // Update computer paddle AI
    computer.UpdateAI(ball.GetPosition().y, ball.GetSpeed().y, screenHeight);
    
    // Check collisions
    CheckCollisions();
    
    // Check scoring
    if (ball.GetPosition().x < 0) {
        state.IncrementComputerScore();
        ball.Reset(screenWidth, screenHeight);
        soundManager.PlayScoreSound();
    } else if (ball.GetPosition().x > screenWidth) {
        state.IncrementPlayerScore();
        ball.Reset(screenWidth, screenHeight);
        ball.SetSpeed({-4.0f, 4.0f});
        soundManager.PlayScoreSound();
    }
    
    // Check game over
    if (state.GetPlayerScore() == 10 || state.GetComputerScore() == 10) {
        state.SetCurrentScreen(GameScreen::ENDING);
    }
}

void Game::CheckCollisions() {
    // Collision with top and bottom
    if ((ball.GetPosition().y >= (screenHeight - ball.GetRadius())) || 
        (ball.GetPosition().y <= ball.GetRadius())) {
        ball.ReverseY();
    }
    
    // Collision with player paddle
    if (ball.GetSpeed().x < 0 && 
        ball.GetPosition().x - ball.GetRadius() <= player.GetX() + player.GetWidth() && 
        ball.GetPosition().x - ball.GetRadius() > player.GetX() && 
        ball.GetPosition().y + ball.GetRadius() >= player.GetY() && 
        ball.GetPosition().y - ball.GetRadius() <= player.GetY() + player.GetHeight()) {
        
        soundManager.PlayBallSound();
        ball.ReverseX();
        ball.IncreaseSpeed();
        ball.SetPosition({static_cast<float>(player.GetX() + player.GetWidth() + ball.GetRadius()), ball.GetPosition().y});
    }
    
    // Collision with computer paddle
    if (ball.GetSpeed().x > 0 && 
        ball.GetPosition().x + ball.GetRadius() >= computer.GetX() && 
        ball.GetPosition().x - ball.GetRadius() < computer.GetX() &&  
        ball.GetPosition().y + ball.GetRadius() >= computer.GetY() && 
        ball.GetPosition().y - ball.GetRadius() <= computer.GetY() + computer.GetHeight()) {
        
        soundManager.PlayBallSound();
        ball.ReverseX();
        ball.IncreaseSpeed();
        ball.SetPosition({static_cast<float>(computer.GetX() - ball.GetRadius()), ball.GetPosition().y});
    }
}

void Game::DrawLogo() const {
    DrawText(TextSubtext("Pong Game", 0, state.GetPlayerScore()), 
             screenWidth/2 - 120, screenHeight/2 - 40, 50, WHITE);
}

void Game::DrawTitle() const {
    DrawText("PONG", 20, 20, 40, WHITE);
    DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA COMEÇAR A JOGAR", 
             150, 200, 15, WHITE);
}

void Game::DrawGameplay() const {
    // Draw score
    DrawText(TextFormat("%d", state.GetPlayerScore()), 280, 10, 50, WHITE);
    DrawText(" X ", screenWidth/2 - 40, 10, 50, WHITE);
    DrawText(TextFormat("%d", state.GetComputerScore()), 500, 10, 50, WHITE);
    
    // Draw game elements
    player.Draw();
    computer.Draw();
    ball.Draw();
    
    // Draw pause message
    if (state.IsPaused()) {
        if (((int)(GetTime()*2) % 2) == 0) { 
            DrawText("PAUSED", 350, 200, 30, GRAY);
            DrawText("APERTE ESPACO para DESPAUSAR", 320, 240, 10, GRAY);
        }
    }
}

void Game::DrawEnding() const {
    if (state.GetPlayerScore() == 10) {
        DrawText("Você Venceu!!", 20, 20, 40, WHITE);
        DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA RECOMEÇAR", 
                 150, 200, 15, WHITE);
    } else if (state.GetComputerScore() == 10) {
        DrawText("Você Perdeu!!", 20, 20, 40, WHITE);
        DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA RECOMEÇAR", 
                 150, 200, 15, WHITE);
    }
}