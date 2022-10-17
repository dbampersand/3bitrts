#pragma once
#include <stdbool.h>    


typedef enum AUGMENT_TYPES
{
    AUGMENT_NULL=0,                         //None
    
    AUGMENT_BAD_EFFECT_TIME,        //Augment_ChangeEffectTime
    AUGMENT_BAD_RANDOMDMGPOOLS,     //Augment_RandomDmgPool
    AUGMENT_BAD_DEATHINCDMG,        //Bad_AugmentDeathAddDamage
    AUGMENT_BAD_MOVESPEED,          //Bad_GetAugmentMoveSpeed
    AUGMENT_BAD_ENEMY_EXPLODES,     //Bad_EnemyExplodes

    AUGMENT_NEUTRAL_DIVIDER,                //None

    AUGMENT_NEUTRAL_TOTALHEAL,      //Neutral_GetAugmentAbilityHeal
    AUGMENT_NEUTRAL_TOTALDAMAGE,    //Neutral_GetAugmentAbilityDamage

    AUGMENT_GOOD_DIVIDER,                   //None

    AUGMENT_GOOD_HEALS,             //Good_GetAugmentAbilityHeal
    AUGMENT_GOOD_DAMAGE,            //Good_GetAugmentAbilityDamage
    AUGMENT_GOOD_MOVESPEED,         //Good_GetAugmentMoveSpeed
    AUGMENT_GOOD_SHIELD,         //Good_GetAugmentMoveSpeed


    AUGMENT_ALL                             //None
    
} AUGMENT_TYPES;

#define MAX_AUGMENTS 4
typedef enum AUGMENT_BUFF_TYPE
{
    AUGMENT_GOOD,
    AUGMENT_NEUTRAL,
    AUGMENT_BAD
}AUGMENT_BUFF_TYPE;

typedef struct Augment
{
    AUGMENT_TYPES augment;
    AUGMENT_BUFF_TYPE friendliness;
    float timer;
    int portrait_sprite_index;
}Augment;

typedef struct Effect Effect;
typedef struct Encounter Encounter; 
typedef struct GameObject GameObject;
typedef struct ALLEGRO_COLOR ALLEGRO_COLOR;
typedef struct MouseState MouseState;

extern Augment AllAugments[AUGMENT_ALL];

int GetNumBadAugments(int augmentLevel);
int GetNumGoodAugments(int augmentLevel);

Augment GetRandomAugment(AUGMENT_BUFF_TYPE augType, Encounter* e);
void SetEncounterRandAugments(Encounter* e);
bool HasAugment(Encounter* e, AUGMENT_TYPES augment);

void ProcessAugments(Augment* a, float dt);

void Augment_ChangeEffectTime(Effect* e, int augmentLevel);
void Augment_RandomDmgPool(Augment* a, int augmentLevel);
char* GetAugmentDescription(AUGMENT_TYPES aug);
void ClearAugments(Encounter* e);

float Neutral_GetAugmentAbilityDamage(int damage, int augmentlevel);
float Neutral_GetAugmentAbilityHeal(int damage, int augmentlevel);

float Good_GetAugmentAbilityHeal(int heal, int augmentlevel);
float Good_GetAugmentAbilityDamage(int damage, int augmentlevel);
float Good_GetAugmentMoveSpeed(float movespeed, int augmentlevel);

float Bad_GetAugmentMoveSpeed(float movespeed, int augmentlevel);
void Bad_AugmentDeathAddDamage(GameObject* g, int augmentLevel);
void Bad_EnemyExplodes(GameObject* g, int augmentLevel);


float GetAugmentDamageBonus(int damage,int augmentLevel);
float GetAugmentHealthBonus(int health, int augmentLevel);
float GetAugmentAbilityDamage(int damage, int augmentLevel);
float GetAugmentMultiplier(int augmentLevel);


ALLEGRO_COLOR* GetAugmentDescriptionColor(Augment* a);
void InitAugments();
bool DrawAugmentPortrait(Augment* a, int x, int y, MouseState* mouseState);