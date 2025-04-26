#include "ball.hpp"
#include <cmath>

Ball::Ball(int screenWidth, int screenHeight) {
    position = {static_cast<float>(screenWidth)/2.0f, static_cast<float>(screenHeight)/2.0f};
    speed = {4.0f, 4.0f};
    radius = 10;
    speedIncreaseFactor = 1.1f;
}

void Ball::Update() {
    position.x += speed.x;
    position.y += speed.y;
    
    // Limit speed
    if (fabs(speed.x) > 12.0f) speed.x = copysign(12.0f, speed.x);
    if (fabs(speed.y) > 12.0f) speed.y = copysign(12.0f, speed.y);
}

void Ball::Draw() const {
    DrawCircleV(position, static_cast<float>(radius), WHITE);
}

void Ball::Reset(int screenWidth, int screenHeight) {
    position = {static_cast<float>(screenWidth)/2.0f, static_cast<float>(screenHeight)/2.0f};
    speed = {4.0f, 4.0f};
}

void Ball::IncreaseSpeed() {
    speed.x *= speedIncreaseFactor;
    speed.y *= speedIncreaseFactor;
}

void Ball::ReverseX() {
    speed.x *= -1.0f;
}

void Ball::ReverseY() {
    speed.y *= -1.0f;
}

// Getters
Vector2 Ball::GetPosition() const { return position; }
int Ball::GetRadius() const { return radius; }
Vector2 Ball::GetSpeed() const { return speed; }

// Setters
void Ball::SetPosition(Vector2 newPosition) { position = newPosition; }
void Ball::SetSpeed(Vector2 newSpeed) { speed = newSpeed; }