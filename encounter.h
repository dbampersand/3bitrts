#pragma once

#include "ability.h"

typedef struct Sprite Sprite;
typedef struct lua_State lua_State;
typedef struct Encounter
{
    Ability abilities[8];
    char* name;

    int spriteIndex;
    char* description;
    char* lua_buffer;
    int luafunc_setup;
} Encounter;

void LoadEncounter(char* dirPath,lua_State* l);
void LoadEncounters(char* dirPath,lua_State* l);

Encounter** encounters;
int numEncounters;
int numEncountersAlloced;
Encounter* currEncounterRunning;

int selectedEncounterIndex;