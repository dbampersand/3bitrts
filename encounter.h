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
    int luafunc_win;



    char* mapPath;
    int numUnitsToSelect;
    char* musicPath;
    Encounter_Difficulty difficulty;
    
    int loadScreen_spriteIndex; 

    int augment;

    #ifdef _AUGMENTS_ENABLED
    Augment augments[MAX_AUGMENTS];
    #endif
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

    bool goingToShop;

    bool hardLoss;

    float timeBreakpoints[MAX_CHESTS];

    float baseReward;
    int bestChest;
} Encounter;

#define NUM_ENCOUNTER_FUNCTIONS 3
#define MAX_DIFFICULTY_LEVELS 11
bool HasChest(Encounter* e, int chestIndex, float time);
float GetReward(Encounter* e, int augmentLevel, int chestIndex);
void LoadEncounter(char* dirPath,lua_State* l);
void LoadEncounters(char* dirPath,lua_State* l);
void NextEncounter();
void PreviousEncounter();
Encounter* GetEncounterByName(char* name);
void UpdateEncounter(float dt);
void UnlockEncounter(const char* path, int difficultyUnlocked, int bestChest, int bestProfit, int totalProfit, bool overwrite);
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
