#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "raylib.h"

class SoundManager {
private:
    Sound ballSound;
    Sound scoreSound;

public:
    SoundManager();
    ~SoundManager();
    
    void PlayBallSound();
    void PlayScoreSound();
};

#endif