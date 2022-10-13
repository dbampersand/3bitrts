#pragma once

#include "ability.h"
#include "limits.h"
#include "augment.h"
#include "ui.h"
#include "point.h"

typedef struct Sprite Sprite;
typedef struct lua_State lua_State;

typedef enum Encounter_Difficulty
{
    DIFFICULTY_UNRATED = 0,
    DIFFICULTY_EASY = 1,
    DIFFICULTY_MEDIUM = 256,
    DIFFICULTY_HARD = 512,
    DIFFICULTY_IMPOSSIBLE = 1024,

    DIFFICULTY_FORCEENUM = INT_MAX
} Encounter_Difficulty;

typedef struct Encounter
{
    Ability abilities[8];
    char* name;

    int spriteIndex;
    char* description;
    char* lua_buffer;

    int luafunc_setup;
    int luafunc_update;
    int luafunc_updateloadscreen;


    char* mapPath;
    int numUnitsToSelect;
    char* musicPath;
    Encounter_Difficulty difficulty;
    
    int loadScreen_spriteIndex; 

    int augment;
    int maxaugment;

    Augment augments[MAX_AUGMENTS];

    bool encounterShouldBeSkipped;

    bool automaticWinCheck;

    UIElement encounter_ButtonLeft;
    UIElement encounter_ButtonConfirm;
    UIElement encounter_ButtonRight;
    UIElement encounter_RerollAugments;


} Encounter;

void LoadEncounter(char* dirPath,lua_State* l);
void LoadEncounters(char* dirPath,lua_State* l);
void NextEncounter();
void PreviousEncounter();
Encounter* GetEncounterByName(char* name);
void UpdateEncounter(float dt);

Encounter** encounters;
int numEncounters;
int numEncountersAlloced;

float encounterMoveTimer;
float encounterOffset;
float encounterMoveTo;

Encounter* currEncounterRunning;

int selectedEncounterIndex;
