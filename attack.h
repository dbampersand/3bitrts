#pragma once

#include <stdbool.h>

#include "colors.h"
#include "video.h" 
#include "pointspace.h"
#include "gameobject.h"
#include "vectorshape.h"


typedef struct Effect Effect;
typedef struct Ability Ability;

#define ATTACK_EASING 0.85f

typedef enum ATTACK_TYPE
{
    ATTACK_AOE = 1,
    ATTACK_PROJECTILE_TARGETED = 2,
    ATTACK_PROJECTILE_POINT = 4,
    ATTACK_PROJECTILE_ANGLE = 8,
    ATTACK_MELEE = 16,
    ATTACK_CONE = 32,
    ATTACK_SHAPE = 64
} ATTACK_TYPE;

typedef enum ATTACK_PROPERTIES
{
    ATTACK_ACTIVE = 1,
    ATTACK_HITS_ENEMIES = 2,
    ATTACK_HITS_FRIENDLIES = 4,
    ATTACK_SOAK = 8,
    ATTACK_DRAW_CIRCLE = 16
} ATTACK_PROPERTIES;

typedef struct Attack
{
    Ability* cameFrom;
    GameObject* target;
    GameObject* ownedBy;


    Effect* effects;    

    float x; float y; 
    float screenX; float screenY;
    
    float radius; float targetRadius; 
    float easing;

    float targx; 
    float targy; 
    
    float speed;    
    
    float duration;
    float tickrate;
    float timer;
    float range;

    Color color;

    bool shouldCallback;

    char numEffects;

    DITHER_PATTERN dither;
    ATTACK_TYPE attackType; 
    ATTACK_PROPERTIES properties;
    OBJ_FRIENDLINESS playerOwnedBy;

    VectorShape shape;

    float inactiveFor;

} Attack;

#define MAX_ATTACKS 1024
#define MAX_AOE_SIZE 255
#define MAX_AOE_CIRCUMFERENCE_SIZE MAX_AOE_SIZE * 2


extern Attack attacks[MAX_ATTACKS];
extern unsigned int freeAttacks[MAX_ATTACKS]; //indices for attacks
extern int attack_top;

//~500kb on 64bit systems
extern ALLEGRO_BITMAP* cachedAttackSprites[MAX_AOE_CIRCUMFERENCE_SIZE+1][DITHER_ALL];


#define AOE_PARTICLES_PER_SECOND 1.0f

void DrawHintSoak(float cx, float cy, float radius, ALLEGRO_COLOR col);
void DrawOutlineBrokenCircle(float cx, float cy, float radius, ALLEGRO_COLOR color, float rotspeed, float numSegments);

Attack* AddAttack(Attack* a);
void InitAttacks();
void RemoveAttack(int attackindex);
void RemoveAllAttacks();
void DrawAttack(Attack* a, float dt);
void DrawAttacks(float dt, int type);
void UpdateAttacks(float dt);
void UpdateAttack(Attack* a, float dt);
void ApplyAttack(Attack* a, GameObject* target);
Attack* CreateAoE(float x, float y, char* effectPortrait, float radius, float tickrate, float duration, bool shouldCallback, int properties, int color, int dither, int numEffects, Effect* effects, GameObject* target, GameObject* from);
bool AttackIsSoak(Attack* a);
bool AttackIsProjectile(Attack* a);
void AttackCanHitTarget(Attack* a, GameObject* g);
int GetNumActiveAttacks();

int NumUnitsInsideAttack(Attack* a);
bool AttackIsActive(Attack* a);
void UpdateScreenPositionsAttack(Attack* a);

bool AttackIsAOE(Attack* a);
Attack* CreateAttackShape(VectorShape v, float cx, float cy, char* effectPortrait, float tickrate, float duration, bool shouldCallback, int properties, int color, int dither, int numEffects, Effect* effects, GameObject* target, GameObject* from);
