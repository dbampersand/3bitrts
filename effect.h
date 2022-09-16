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
    EFFECT_ATTACKDAMAGE = 8,
    EFFECT_INVULN = 9,
    EFFECT_CURE = 10,
    EFFECT_STUN = 11,
    EFFECT_ALL
} EFFECT_TYPE;

typedef struct Effect
{
    EFFECT_TRIGGER trigger;
    EFFECT_TYPE effectType;
    GameObject* from;
    char* name;
    char* description;
    bool canStack;
    int stacks;
    int maxStacks;
    float value;

    float numTriggers;
    float timer;
    
    float duration; 
    float tickTime;
    bool enabled;

    int spriteIndex_Portrait;

    bool overwrites;
    
} Effect;

void ApplyEffect(Effect* e, GameObject* from, GameObject* target);
void ProcessEffects(GameObject* g, float dt);
bool ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove);
int CureNamedEffect(GameObject* g, const char* name, int numStacksToRemove);
void CureEffect(GameObject* g, int numEffects, bool removeAllStacks);
void CureAll(GameObject* g);
bool RemoveEffect(Effect* e, GameObject* from,bool removeAllStacks);
bool EffectIsEnabled(Effect* e);
