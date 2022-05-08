#pragma once

#include "ability.h"
#include "limits.h"

typedef struct Sprite Sprite;
typedef struct lua_State lua_State;

typedef enum Encounter_Difficulty
{
    DIFFICULTY_UNRATED = 0,
    DIFFICULTY_EASY = 1,
    DIFFICULTY_MEDIUM = 256,
    DIFFICULTY_HARD = 512,
    DIFFICULTY_IMPOSSIBLE = 1024,

    forceenumtoint = INT_MAX
} Encounter_Difficulty;
typedef struct Encounter
{
    Ability abilities[8];
    char* name;

    int spriteIndex;
    char* description;
    char* lua_buffer;
    int luafunc_setup;

    char* mapPath;
    int numUnitsToSelect;
    Encounter_Difficulty difficulty;
} Encounter;

void LoadEncounter(char* dirPath,lua_State* l);
void LoadEncounters(char* dirPath,lua_State* l);

Encounter** encounters;
int numEncounters;
int numEncountersAlloced;
Encounter* currEncounterRunning;

int selectedEncounterIndex;