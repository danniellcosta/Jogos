#include "raylib.h"
#include <cmath>

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Pong Game");
    InitAudioDevice();

    GameScreen currentScreen = LOGO;

    int playerY = screenHeight / 2 - 60; 
    int playerHeight = 110;             
    int playerWidth = 20;                

    int computerY = screenHeight / 2 - playerHeight / 2; 
    float computerBaseSpeed = 4.0f;
    float computerMaxSpeed = 8.0f; 

    Vector2 ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    Vector2 ballSpeed = { 4.0f, 4.0f };
    int ballRadius = 10;
    float speedIncreaseFactor = 1.1f;
    
    bool pause = true;
    bool gameStarted = false;
    
    int playerScore = 0; 
    int computerScore = 0;
    
    int letterCount = 0;
    int state = 0;

    int framesCounter = 0;
    
    Sound ballSom = LoadSound("ballHit.wav");
    Sound pontoSom = LoadSound("ponto.mp3");
    
    SetTargetFPS(60);
    
    auto ResetGame = [&]() {
        ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
        ballSpeed = { 4.0f, 4.0f };
        playerScore = 0;
        computerScore = 0;
        pause = true;
        gameStarted = false;
    };

    while (!WindowShouldClose())
    {
        
       
        
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause) {
  
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            if (fabs(ballSpeed.x) > 12.0f) ballSpeed.x = copysign(12.0f, ballSpeed.x);
            if (fabs(ballSpeed.y) > 12.0f) ballSpeed.y = copysign(12.0f, ballSpeed.y);

            if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) 
                ballSpeed.y *= -1.0f;

            playerY = GetMouseY() - playerHeight / 2;
            if (playerY < 0) playerY = 0;
            if (playerY > screenHeight - playerHeight) playerY = screenHeight - playerHeight;

            float distanceToBall = ballPosition.y - (computerY + playerHeight / 2);

            float adaptiveSpeed = computerBaseSpeed + (computerMaxSpeed - computerBaseSpeed) * (fabsf(distanceToBall) / screenHeight);

            float randomError = GetRandomValue(-10, 10) * 0.5f;
            adaptiveSpeed += randomError;
            
            if (distanceToBall > 0) {
                computerY += adaptiveSpeed;
            } 
            else if (distanceToBall < 0) {
                computerY -= adaptiveSpeed;
            }

            if (computerY < 0) computerY = 0;
            if (computerY > screenHeight - playerHeight) computerY = screenHeight - playerHeight;

            if (ballPosition.x - ballRadius <= 50 + playerWidth && 
                ballPosition.y + ballRadius >= playerY && ballPosition.y - ballRadius <= playerY + playerHeight) {
                PlaySound(ballSom);
                ballSpeed.x *= -1.0f;
                ballSpeed.x *= speedIncreaseFactor;
                ballSpeed.y *= speedIncreaseFactor;
                ballPosition.x = 50 + playerWidth + ballRadius;
            }

            if (ballPosition.x + ballRadius >= 725 && 
                ballPosition.y + ballRadius >= computerY && ballPosition.y - ballRadius <= computerY + playerHeight) {
                PlaySound(ballSom);
                ballSpeed.x *= -1.0f;
                ballSpeed.x *= speedIncreaseFactor; 
                ballSpeed.y *= speedIncreaseFactor; 
                ballPosition.x = 725 - ballRadius; 
            }

            if (ballPosition.x < 0) {
                computerScore++;
                ballPosition = { screenWidth/2.0f, screenHeight/2.0f };
                ballSpeed = { 4.0f, 4.0f };
                PlaySound(pontoSom);
            }
            else if (ballPosition.x > screenWidth) {
                playerScore++;
                ballPosition = { screenWidth/2.0f, screenHeight/2.0f };
                ballSpeed = { -4.0f, 4.0f };
                PlaySound(pontoSom);
            }
        }

        switch (currentScreen) {
            case LOGO: {
               

               
                 if (state == 0)                 
        {

           framesCounter++;

            if (framesCounter/12)       
            {
                letterCount++;
                framesCounter = 0;
                
                 if (letterCount > 8) {
                   state = 1;
                }
                             
                
            }
            
        }else if(state == 1){
                 
                 framesCounter++;
                if (framesCounter > 120) {
                    currentScreen = TITLE;
                }
         
        }

            } break;

            case TITLE: {
                if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = GAMEPLAY;
                }
            } break;

            case GAMEPLAY: {
                if (playerScore == 10 || computerScore == 10) {
                    currentScreen = ENDING;
                }
            } break;

            case ENDING: {
                if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    ResetGame(); 
                    currentScreen = GAMEPLAY;
                }

                if (playerScore == 10) {
                    DrawText("Você venceu!", screenWidth / 2 - 100, screenHeight / 2 - 20, 40, GREEN);
                } else {
                    DrawText("Você perdeu!", screenWidth / 2 - 100, screenHeight / 2 - 20, 40, RED);
                }
            } break;

            default: break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen) {
            case LOGO: {
                 DrawText(TextSubtext("Pong Game", 0, letterCount), GetScreenWidth()/2 -120, GetScreenHeight()/2 -40, 50, WHITE);
            } break;

            case TITLE: {
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("PONG", 20, 20, 40, WHITE);
                DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA COMEÇAR A JOGAR", 150, 200, 15, WHITE);
            } break;

            case GAMEPLAY: {
                DrawText(TextFormat("%d", playerScore), 280, 10, 50, WHITE);
                DrawText(" X ", screenWidth/2 - 40,10,50,WHITE);
                DrawText(TextFormat("%d", computerScore),500, 10, 50, WHITE);

                DrawRectangle(50, playerY, playerWidth, playerHeight, WHITE);
                DrawRectangle(725, computerY, playerWidth, playerHeight, WHITE);
                DrawCircleV(ballPosition, (float)ballRadius, WHITE); 

    if (pause) {
        if ((framesCounter / 30) % 2 == 0) {
            DrawText("PAUSED", 350, 200, 30, GRAY); 
            DrawText("APERTE ESPACO para DESPAUSAR",320,240,10,GRAY);
        }
    }
            } break;

            case ENDING: {
                gameStarted = false;
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                if (playerScore == 10) {
                    DrawText("Você Venceu!!", 20, 20, 40, WHITE);
                    DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA RECOMEÇAR", 150, 200, 15, WHITE);
                } else if (computerScore == 10) {
                    DrawText("Você Perdeu!!", 20, 20, 40, WHITE);
                    DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA RECOMEÇAR", 150, 200, 15, WHITE);
                }
            } break;

            default: break;
        }

        EndDrawing();
    }
    
    UnloadSound(ballSom);
    UnloadSound(pontoSom);
    
    CloseAudioDevice();

    CloseWindow();

    return 0;
}
