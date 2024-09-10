#include "raylib.h"
#include <cmath>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Pong Game");
    InitAudioDevice();

    GameScreen currentScreen = LOGO;

    // Player retângulos
    int playerY = screenHeight / 2 - 60; // Iniciar no meio da tela (jogador esquerdo)
    int playerHeight = 110;              // Altura dos retângulos
    int playerWidth = 20;                // Largura dos retângulos

    // Computador retângulo
    int computerY = screenHeight / 2 - playerHeight / 2; // Iniciar no meio da tela (jogador direito)
    float computerBaseSpeed = 4.0f; // Velocidade base do retângulo do computador
    float computerMaxSpeed = 8.0f; // Velocidade máxima do computador

    // Bola
    Vector2 ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    Vector2 ballSpeed = { 4.0f, 4.0f };
    int ballRadius = 10;
    float speedIncreaseFactor = 1.1f; // Fator de aumento de velocidade a cada colisão
    
    bool pause = true;  // Inicia com o jogo pausado
    bool gameStarted = false;
    
    int playerScore = 0; // Initialize player score to 0
    int computerScore = 0; // Initialize computer score to 0

    int framesCounter = 0; // Contador de frames
    
    Sound ballSom = LoadSound("ballHit.wav");
    Sound pontoSom = LoadSound("ponto.mp3");
    
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Função para resetar o jogo
    auto ResetGame = [&]() {
        ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
        ballSpeed = { 4.0f, 4.0f };
        playerScore = 0;
        computerScore = 0;
        pause = true;
        gameStarted = false;
    };

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button ou ESC key
    {
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause) {
            // Movimentação da bola
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            // Limitar a velocidade máxima da bola
            if (fabs(ballSpeed.x) > 12.0f) ballSpeed.x = copysign(12.0f, ballSpeed.x);
            if (fabs(ballSpeed.y) > 12.0f) ballSpeed.y = copysign(12.0f, ballSpeed.y);

            // Verifica colisão com as bordas superiores e inferiores
            if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) 
                ballSpeed.y *= -1.0f;

            // Movimento do jogador esquerdo (usando o mouse)
            playerY = GetMouseY() - playerHeight / 2;
            if (playerY < 0) playerY = 0;
            if (playerY > screenHeight - playerHeight) playerY = screenHeight - playerHeight;

            // Movimento do computador (segue a bola adaptando a velocidade)
            float distanceToBall = ballPosition.y - (computerY + playerHeight / 2); // Distância da bola ao centro do retângulo do computador

            // Velocidade adaptada
            float adaptiveSpeed = computerBaseSpeed + (computerMaxSpeed - computerBaseSpeed) * (fabsf(distanceToBall) / screenHeight);

            // Adicionar variação aleatória na movimentação do computador para simular erros
            float randomError = GetRandomValue(-10, 10) * 0.5f; // Variação aleatória para simular erro
            adaptiveSpeed += randomError;

            // Movimento do computador com a velocidade adaptada
            if (distanceToBall > 0) {
                computerY += adaptiveSpeed; // Move para baixo
            } 
            else if (distanceToBall < 0) {
                computerY -= adaptiveSpeed; // Move para cima
            }

            // Impede que o computador saia dos limites da tela
            if (computerY < 0) computerY = 0;
            if (computerY > screenHeight - playerHeight) computerY = screenHeight - playerHeight;

            // Verificar colisão da bola com o retângulo esquerdo (player)
            if (ballPosition.x - ballRadius <= 50 + playerWidth && 
                ballPosition.y + ballRadius >= playerY && ballPosition.y - ballRadius <= playerY + playerHeight) {
                PlaySound(ballSom);
                ballSpeed.x *= -1.0f; // Inverter a direção horizontal da bola
                ballSpeed.x *= speedIncreaseFactor; // Aumentar a velocidade no eixo X
                ballSpeed.y *= speedIncreaseFactor; // Aumentar a velocidade no eixo Y
                ballPosition.x = 50 + playerWidth + ballRadius; // Evitar "grudar" na borda
            }

            // Verificar colisão da bola com o retângulo direito (computador)
            if (ballPosition.x + ballRadius >= 725 && 
                ballPosition.y + ballRadius >= computerY && ballPosition.y - ballRadius <= computerY + playerHeight) {
                PlaySound(ballSom);
                ballSpeed.x *= -1.0f; // Inverter a direção horizontal da bola
                ballSpeed.x *= speedIncreaseFactor; // Aumentar a velocidade no eixo X
                ballSpeed.y *= speedIncreaseFactor; // Aumentar a velocidade no eixo Y
                ballPosition.x = 725 - ballRadius; // Evitar "grudar" na borda
            }

            // Atualizar pontuação
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

        // Update
        switch (currentScreen) {
            case LOGO: {
                framesCounter++;
                if (framesCounter > 120) {
                    currentScreen = TITLE;
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
                    ResetGame(); // Resetar o jogo ao final da tela de ENDING
                    currentScreen = GAMEPLAY; // Voltar para a tela de gameplay
                }

                // Desenhar o texto de fim de jogo
                if (playerScore == 10) {
                    DrawText("Você venceu!", screenWidth / 2 - 100, screenHeight / 2 - 20, 40, GREEN);
                } else {
                    DrawText("Você perdeu!", screenWidth / 2 - 100, screenHeight / 2 - 20, 40, RED);
                }
            } break;

            default: break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen) {
            case LOGO: {
                DrawText("Bem Vindo!", 20, 20, 40, LIGHTGRAY);
                DrawText("ESPERE por 2 SEGUNDOS...", 290, 220, 20, GRAY);
            } break;

            case TITLE: {
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("PONG", 20, 20, 40, WHITE);
                DrawText("PRESSIONE A TECLA ENTER OU CLIQUE NA TELA PARA COMEÇAR A JOGAR", 150, 200, 15, WHITE);
            } break;

            case GAMEPLAY: {
                DrawText(TextFormat("Player Score: %d", playerScore), 10, 10, 20, WHITE);
                DrawText(TextFormat("Computer Score: %d", computerScore), 570, 10, 24, WHITE);

                // Desenha os retângulos dos jogadores e a bola
                DrawRectangle(50, playerY, playerWidth, playerHeight, WHITE);
                DrawRectangle(725, computerY, playerWidth, playerHeight, WHITE);
                DrawCircleV(ballPosition, (float)ballRadius, WHITE); // Bola desenhada

                 // Exibe o texto PAUSED se o jogo estiver pausado
    if (pause) {
        if ((framesCounter / 30) % 2 == 0) {
            DrawText("PAUSED", 350, 200, 30, GRAY); // Desenha o texto "PAUSED"
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
