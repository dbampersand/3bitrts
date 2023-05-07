#pragma once

#include "stdbool.h"
#include "pointspace.h"
#include "colors.h"
typedef struct GameObject GameObject;
typedef struct Ability Ability;

typedef enum EFFECT_TRIGGER
{
   TRIGGER_TIMER = 0,
   TRIGGER_ONATTACK = 1,
   TRIGGER_ONHIT = 2,
   TRIGGER_INSTANT = 3,
   TRIGGER_CONST = 4,
   TRIGGER_PERMANENT
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
    EFFECT_MISS_CHANCE = 7,
    EFFECT_SHIELD = 8,
    EFFECT_ATTACKDAMAGE = 9,
    EFFECT_INVULN = 10,
    EFFECT_CURE = 11,
    EFFECT_STUN = 12,
    EFFECT_ATTACKSPEED = 13,
    EFFECT_ATTACKRANGE = 14,
    EFFECT_ADD_MANA = 15,
    EFFECT_ARMOR = 16,
    EFFECT_LIFESTEAL = 17,
    EFFECT_HURT_PERCENT = 18,
    EFFECT_COOLDOWN_RATE = 19,
    EFFECT_ALL
} EFFECT_TYPE;


typedef struct EffectVisual {
    PointSpace position;
    float timer;
    float maxTimer;
    bool isActive;
    Color* color;
} EffectVisual;

typedef struct Effect
{
    EFFECT_TRIGGER trigger;
    EFFECT_TYPE effectType;
    GameObject* from;
    int playerOwnedBy;
    
    char* name;
    char* description;
    bool canStack;
    int stacks;
    int maxStacks;
    float value;

    float triggersPerSecond;
    float numTriggers;
    float timer;
    
    float duration; 
    float tickTime;
    bool enabled;

    int spriteIndex_Portrait;

    bool overwrites;

    EffectVisual visual;

    Ability* abilityFrom;

} Effect;

void ApplyEffect(Effect* e, GameObject* from, GameObject* target);
void ProcessEffects(GameObject* g, float dt);
bool ProcessEffect(Effect* e, GameObject* from, GameObject* target, bool remove);
int CureNamedEffect(GameObject* g, const char* name, int numStacksToRemove);
void CureEffect(GameObject* g, Effect* e, int numEffects, bool removeAllStacks);
void CureAll(GameObject* g);
bool RemoveEffect(Effect* e, GameObject* from,bool removeAllStacks);
bool EffectIsEnabled(Effect* e);

void DrawEffectVisuals(GameObject* g);
void UpdateEffectVisuals(GameObject* g, float dt);
void InitializeEffectVisual(Effect* e, int x, int y);
