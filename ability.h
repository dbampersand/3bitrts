#pragma once

typedef enum ABILITY_CAST_TYPE
{
    ABILITY_INSTANT = 0,
    ABILITY_POINT = 1,
    ABILITY_TARGET_FRIENDLY = 2,
    ABILITY_TARGET_ENEMY = 3,
    ABILITY_TARGET_ALL = 4
} ABILITY_CAST_TYPE;

typedef struct Ability
{
    char* luabuffer;
    int luafunc_casted;
    int luafunc_setup;
    char* path;
    int spriteIndex_Portrait;
    float cooldown;
    float cdTimer;
    ABILITY_CAST_TYPE castType;
} Ability;

Ability* abilities;
int numAbilities;
int numAbilitiesAllocated;
typedef struct lua_State lua_State;
void LoadAbility(const char* path, lua_State* l, Ability* a);
Ability CloneAbilityPrefab(Ability* prefab, lua_State* l);
Ability* AddAbility(const char* path);
void CastAbility(Ability* a);
