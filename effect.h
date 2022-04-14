#pragma once
#include "stdbool.h"
typedef struct GameObject GameObject;

typedef enum EFFECT_TRIGGER
{
   TRIGGER_TIMER = 0,
   TRIGGER_ONATTACK = 1,
   TRIGGER_ONHIT = 2,
   TRIGGER_INSTANT = 3,
   TRIGGER_CONST
} EFFECT_TRIGGER;
typedef enum EFFECT_TYPE
{
    EFFECT_NONE = 0,
    EFFECT_MAXHP = 1,
    EFFECT_HURT = 2,
    EFFECT_HEAL = 3,
    EFFECT_POSITION = 4,
    EFFECT_THREAT = 5,
    EFFECT_SPEED = 6,
    EFFECT_SHIELD = 7,
    EFFECT_DAMAGE = 8,
    EFFECT_INVULN = 9
} EFFECT_TYPE;
typedef struct Effect
{
    EFFECT_TRIGGER trigger;
    EFFECT_TYPE effectType;
    GameObject* from;
    float value;

    float numTriggers;
    float timer;
    
    float duration; 
    float tickTime;
    bool enabled;

    
} Effect;
void ApplyEffect(Effect* e, GameObject* from, GameObject* target);
void ProcessEffects(GameObject* g, float dt);
void ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove);
