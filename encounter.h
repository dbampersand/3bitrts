#pragma once

#include "ability.h"
#include "limits.h"
#include "augment.h"
#include "ui.h"
#include "point.h"
#include "luafuncs.h"


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
    char* path;

    int spriteIndex;
    char* description;
    LuaBuffer lua_buffer;

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


    UIElement encounter_ButtonLeft;
    UIElement encounter_ButtonConfirm;
    UIElement encounter_ButtonRight;
    UIElement encounter_RerollAugments;

    bool unlocked;


} Encounter;

#define NUM_ENCOUNTER_FUNCTIONS 3

void LoadEncounter(char* dirPath,lua_State* l);
void LoadEncounters(char* dirPath,lua_State* l);
void NextEncounter();
void PreviousEncounter();
Encounter* GetEncounterByName(char* name);
void UpdateEncounter(float dt);
void UnlockEncounter(const char* path);
void ResetEncounterPosition();
bool PathCmp(char* path1, char* path2);

extern Encounter** encounters;
extern int numEncounters;
extern int numEncountersAlloced;

extern float encounterMoveTimer;
extern float encounterOffset;
extern float encounterMoveTo;

extern Encounter* currEncounterRunning;

extern int selectedEncounterIndex;
