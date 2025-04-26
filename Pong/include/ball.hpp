#ifndef BALL_HPP
#define BALL_HPP

#include "raylib.h"

class Ball {
private:
    Vector2 position;
    Vector2 speed;
    int radius;
    float speedIncreaseFactor;

public:
    Ball(int screenWidth, int screenHeight);
    
    void Update();
    void Draw() const;
    void Reset(int screenWidth, int screenHeight);
    void IncreaseSpeed();
    void ReverseX();
    void ReverseY();
    
    // Getters
    Vector2 GetPosition() const;
    int GetRadius() const;
    Vector2 GetSpeed() const;
    
    // Setters
    void SetPosition(Vector2 newPosition);
    void SetSpeed(Vector2 newSpeed);
};

#endif