

#pragma once
#include "allegro5/allegro.h"

unsigned int _RENDERSIZE;
#define _SCREEN_SIZE 256

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct GameObject GameObject;
typedef struct Sprite
{
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_BITMAP* inverseSprite;

    char* path;
} Sprite;

int numSprites; 
int maxSprites; //will dynamically change 
Sprite* sprites;

unsigned int LoadSprite(const char* path, bool needsInverted);
void DrawSprite(Sprite* sprite, int x, int y, ALLEGRO_COLOR tint, bool invert);
void GenerateInvertedSprite(Sprite* s);
void DrawSpriteRegion(Sprite* s, float sx, float sy, float sw, float sh, float dx, float dy, ALLEGRO_COLOR tint, bool invert);