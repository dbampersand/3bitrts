#pragma once

#include <stdbool.h>

#include "colors.h"

typedef struct GameObject GameObject;
typedef struct Effect Effect;
typedef struct Ability Ability;

typedef enum ATTACK_TYPE
{
    ATTACK_AOE = 0,
    ATTACK_PROJECTILE_TARGETED,
    ATTACK_PROJECTILE_POINT,
    ATTACK_PROJECTILE_ANGLE,
    ATTACK_MELEE,
    ATTACK_CONE
} ATTACK_TYPE;

typedef enum ATTACK_PROPERTIES
{
    ATTACK_ACTIVE = 1,
    ATTACK_HITS_ENEMIES = 2,
    ATTACK_HITS_FRIENDLIES = 4,
} ATTACK_PROPERTIES;

typedef struct Attack
{
    float x; float y; float radius; float targetRadius; 
    float easing;

    float targx; float targy; 
    GameObject* target;
    Effect* effects;    
    int numEffects;
    ATTACK_TYPE attackType;
    char properties;
    GameObject* ownedBy;
    float speed;    

    bool shouldCallback;
    float duration;
    float tickrate;
    float timer;
    float range;

    Ability* cameFrom;

    Color color;
    DITHER_PATTERN dither;

} Attack;

#define MAX_ATTACKS 128
Attack attacks[MAX_ATTACKS];
unsigned char freeAttacks[MAX_ATTACKS]; //indices for attacks
int attack_top;

Attack* AddAttack(Attack* a);
void InitAttacks();
void RemoveAttack(int attackindex);
void RemoveAllAttacks();
void DrawAttack(Attack* a, float dt);
void DrawAttacks(float dt);
void UpdateAttacks(float dt);
void UpdateAttack(Attack* a, float dt);
void ApplyAttack(Attack* a, GameObject* target);
Attack* CreateAoE(float x, float y, char* effectPortrait, float radius, float tickrate, float duration, bool shouldCallback, int properties, int color, int dither, int numEffects, Effect* effects);
