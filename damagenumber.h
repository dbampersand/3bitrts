#pragma once
#include "point.h"

typedef struct DamageNumber
{
    char* text;
    Point pos;
} DamageNumber;
#define MAX_DAMAGE_NUMBERS 16
DamageNumber damageNumbers[MAX_DAMAGE_NUMBERS];
int currDamageNumber;

void DrawDamageNumbers();
void InitDamageNumbers();
void AddDamageNumber(int damage, int x, int y);
void UpdateDamageNumbers(float dt);


