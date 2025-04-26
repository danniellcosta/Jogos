#include "paddle.hpp"
#include "raylib.h"
#include <cmath>

Paddle::Paddle(int startX, int startY, int paddleWidth, int paddleHeight, bool computer) 
    : x(startX), y(startY), width(paddleWidth), height(paddleHeight), isComputer(computer) {
    speed = 5;
    baseSpeed = 4.0f;
    maxSpeed = 8.0f;
}

void Paddle::Update(int ballY) {
    if (isComputer) {
        UpdateAI(ballY, 0, GetScreenHeight());
    }
}

void Paddle::Draw() const {
    DrawRectangle(x, y, width, height, WHITE);
}

void Paddle::Move(int newY, int screenHeight) {
    y = newY - height / 2;
    if (y < 0) y = 0;
    if (y > screenHeight - height) y = screenHeight - height;
}

void Paddle::UpdateAI(int ballY, int ballSpeedY, int screenHeight) {
    float distanceToBall = ballY - (y + height / 2);
    float adaptiveSpeed = baseSpeed + (maxSpeed - baseSpeed) * (fabsf(distanceToBall) / screenHeight);
    float randomError = GetRandomValue(-10, 10) * 0.5f;
    adaptiveSpeed += randomError;
    
    if (distanceToBall > 0) {
        y += adaptiveSpeed;
    } 
    else if (distanceToBall < 0) {
        y -= adaptiveSpeed;
    }

    if (y < 0) y = 0;
    if (y > screenHeight - height) y = screenHeight - height;
}

// Getters
int Paddle::GetX() const { return x; }
int Paddle::GetY() const { return y; }
int Paddle::GetWidth() const { return width; }
int Paddle::GetHeight() const { return height; }