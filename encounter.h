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

    Augment augments[MAX_AUGMENTS];

    bool encounterShouldBeSkipped;


    UIElement encounter_ButtonLeft;
    UIElement encounter_ButtonConfirm;
    UIElement encounter_ButtonRight;
    UIElement encounter_RerollAugments;
    UIElement encounter_PurchaseAugment;


    bool unlocked;
    int32_t difficultyUnlocked;
    int32_t unlockBasePrice;

    int32_t bestProfited;
    int32_t totalProfit;


} Encounter;

#define NUM_ENCOUNTER_FUNCTIONS 3


void LoadEncounter(char* dirPath,lua_State* l);
void LoadEncounters(char* dirPath,lua_State* l);
void NextEncounter();
void PreviousEncounter();
Encounter* GetEncounterByName(char* name);
void UpdateEncounter(float dt);
void UnlockEncounter(const char* path, int difficultyUnlocked, int bestProfit, int totalProfit, bool overwrite);
void ResetEncounterPosition();
bool PathCmp(char* path1, char* path2);

void SetEncounterProfited(Encounter* e, int amountProfited);
int GetAugmentCost(Encounter* e, int level);



extern Encounter** encounters;
extern int numEncounters;
extern int numEncountersAlloced;

extern float encounterMoveTimer;
extern float encounterOffset;
extern float encounterMoveTo;

extern Encounter* currEncounterRunning;

extern int selectedEncounterIndex;
extern float goldLossRate;

extern int maxAugments;