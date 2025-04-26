#include "soundmanager.hpp"

SoundManager::SoundManager() {
    InitAudioDevice();
    ballSound = LoadSound("resources/ballHit.wav");
    scoreSound = LoadSound("resources/ponto.mp3");
}

SoundManager::~SoundManager() {
    UnloadSound(ballSound);
    UnloadSound(scoreSound);
    CloseAudioDevice();
}

void SoundManager::PlayBallSound() {
    PlaySound(ballSound);
}

void SoundManager::PlayScoreSound() {
    PlaySound(scoreSound);
}