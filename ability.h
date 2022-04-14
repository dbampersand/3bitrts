#pragma once

#include "stdbool.h"
typedef struct GameObject GameObject;

typedef enum ABILITY_CAST_TYPE
{
    ABILITY_INSTANT = 1,
    ABILITY_POINT = 2,
    ABILITY_TARGET_FRIENDLY = 4,
    ABILITY_TARGET_ENEMY = 8,
    ABILITY_TARGET_ALL = 16,
    ABILITY_TOGGLE = 32
} ABILITY_CAST_TYPE;
typedef enum HINT
{
    HINT_NONE=0,
    HINT_LINE=1
} HINT;

typedef struct Ability
{
    char* luabuffer;
    int luafunc_casted;
    int luafunc_setup;
    int luafunc_onhit;
    int luafunc_tick;
    int luafunc_untoggle;

    float range;

    char* path;
    int spriteIndex_Portrait;
    float cooldown;
    float cdTimer;
    ABILITY_CAST_TYPE castType;

    bool toggled;

    char* description;
    HINT targetingHint;

} Ability;

Ability* abilities;
int numAbilities;
int numAbilitiesAllocated; 
typedef struct lua_State lua_State;
void LoadAbility(const char* path, lua_State* l, Ability* a);
Ability CloneAbilityPrefab(Ability* prefab, lua_State* l);
Ability* AddAbility(const char* path);
void CastAbility(GameObject* g, Ability* a, int x, int y, float headingx, float headingy, GameObject* target);
bool AbilityIsInitialised(Ability* a);
void ToggleAbility(Ability* a, GameObject* ownedBy, bool toggled);
bool AbilityCanBeCast(Ability* a, GameObject* g, GameObject* target, float x, float y);
bool AbilityIsCastImmediately(Ability* a);
