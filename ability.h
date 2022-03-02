#pragma once

typedef struct Ability
{
    //void (*cast)(struct Ability* self);
    char* luabuffer;
    int luafunc_casted;
    int luafunc_setup;
    char* path;
    int spriteIndex_Portrait;
} Ability;

Ability* abilities;
int numAbilities;
int numAbilitiesAllocated;
typedef struct lua_State lua_State;
void LoadAbility(const char* path, lua_State* l, Ability* a);
Ability CloneAbilityPrefab(Ability* prefab, lua_State* l);
Ability* AddAbility(const char* path);

