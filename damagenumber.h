#pragma once

#include "point.h"

typedef struct ALLEGRO_COLOR ALLEGRO_COLOR;
typedef struct GameObject GameObject;

typedef struct DamageNumber
{
    ALLEGRO_COLOR* color; 
    char* text;
    Point pos;
    float fade;
} DamageNumber;

#define MAX_DAMAGE_NUMBERS 16
DamageNumber damageNumbers[MAX_DAMAGE_NUMBERS];
int currDamageNumber;

void DrawDamageNumbers();
void InitDamageNumbers();
void AddDamageNumber(int damage, int x, int y, GameObject* source);
void UpdateDamageNumbers(float dt);


