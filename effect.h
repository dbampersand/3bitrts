#pragma once
#include "stdbool.h"
typedef enum EFFECT_TRIGGER
{
   TRIGGER_TIMER = 0,
   TRIGGER_ONATTACK = 1,
   TRIGGER_ONHIT = 2,
   TRIGGER_INSTANT = 3
} EFFECT_TRIGGER;

typedef enum EFFECT_TYPE
{
    EFFECT_MAXHP = 0,
    EFFECT_DAMAGE = 1,
    EFFECT_HEAL = 2
} EFFECT_TYPE;
typedef struct Effect
{
    EFFECT_TRIGGER trigger;
    EFFECT_TYPE effectType;
    float value;

    float numTriggers;
    float timer;
    
    float duration; 
    float tickTime;
    bool enabled;
} Effect;
typedef struct GameObject GameObject;
void ApplyEffect(Effect* e, GameObject* target);
void ProcessEffects(GameObject* g, float dt);
