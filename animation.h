#pragma once

#include "stdbool.h"
#include "colors.h"

typedef struct Sprite Sprite;

typedef struct Animation
{
    int currentIndice;
    
    int frameW;
    int frameH;

    int startIndice;

    float timer;
    float cooldown;

    int spriteIndex_Animation;
    bool hasLooped;
    int numFrames;

} Animation;

Sprite* GetCurrentFrame(Animation* a);
void ProcessAnimations(Animation* a, float dt);
void DrawAnimation(Animation* a, int x, int y, Color c, bool invert);
Animation LoadAnimation(char* path, int w, int h, float cooldown,int startIndice, int numFrames);
