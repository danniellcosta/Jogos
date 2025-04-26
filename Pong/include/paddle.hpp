#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "raylib.h"

class Paddle {
private:
    int x;
    int y;
    int width;
    int height;
    int speed;
    bool isComputer;
    float baseSpeed;
    float maxSpeed;

public:
    Paddle(int startX, int startY, int width, int height, bool isComputer = false);
    
    void Update(int ballY);
    void Draw() const;
    void Move(int newY, int screenHeight);
    
    // Getters
    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;
    
    // For computer paddle
    void UpdateAI(int ballY, int ballSpeedY, int screenHeight);
};

#endif