

#pragma once

#include "allegro5/allegro.h"

unsigned int _RENDERSIZE;
#define _SCREEN_SIZE 256

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct GameObject GameObject;
typedef struct HashTable HashTable;
typedef struct Sprite
{
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_BITMAP* inverseSprite;

    char* path;
} Sprite;

int numSprites; 
int maxSprites; //will dynamically change 
Sprite* sprites;

HashTable* spritesHashTable;

unsigned int LoadSprite(const char* path, bool needsInverted);
void DrawSprite(Sprite* sprite, int x, int y, float originX, float originY, float angle, ALLEGRO_COLOR tint, bool invert);
void GenerateInvertedSprite(Sprite* s);
void DrawSpriteRegion(Sprite* s, float sx, float sy, float sw, float sh, float dx, float dy, ALLEGRO_COLOR tint, bool invert);
void InitSprites();
int GetHeightSprite(Sprite* s);
int GetWidthSprite(Sprite* s);
Sprite* NewSprite(int w, int h);
