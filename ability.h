#pragma once

#include "stdbool.h"
#include "sound.h"
#include "luafuncs.h"

typedef struct GameObject GameObject;


typedef enum ABILITY_CAST_TYPE
{
    ABILITY_NONE = 0,
    ABILITY_INSTANT = 1,
    ABILITY_POINT = 2,
    ABILITY_TARGET_FRIENDLY = 4,
    ABILITY_TARGET_ENEMY = 8,
    ABILITY_TARGET_ALL = 16,
    ABILITY_TOGGLE = 32,
    ABILITY_ANGLE = 64,
} ABILITY_CAST_TYPE;


typedef enum HINT
{
    HINT_NONE=0,
    HINT_LINE=1,
    HINT_CIRCLE,
    HINT_CONE
} HINT;


typedef struct Ability
{

    LuaBuffer luabuffer;
    int luafunc_casted;
    int luafunc_setup;
    int luafunc_onhit;
    int luafunc_abilitytick;
    int luafunc_untoggle;
    int luafunc_ontimeout;
    int luafunc_onchanneled;
    int luafunc_parentdeath;


    float range;
    //float length;
    char* path;
    char* name;
    int spriteIndex_Portrait;
    float cooldown;
    float cdTimer;
    
    float manaCost;

    ABILITY_CAST_TYPE castType;

    bool toggled;

    char* description;

    HINT targetingHint;
    float hintRadius;
    bool hintSoak;
    float hintLength;
    
    int stacks;
    int maxStacks;

    float timeSinceLastCast;




} Ability;

#define NUM_ABILITY_FUNCTIONS 7


extern Ability* abilities;
extern int numAbilities;
extern int numAbilitiesAllocated; 


extern SoundIndex ability_UI_click_sound;
extern SoundIndex ability_UI_click_up_sound;   



typedef struct lua_State lua_State;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
typedef struct MouseState MouseState;

void SetManaCost(Ability* a, float mana);

void LoadAbility(const char* path, lua_State* l, Ability* a);
void CloneAbilityPrefab(Ability* prefab, lua_State* l, Ability* cloneTo);
Ability* AddAbility(const char* path);
bool CastAbility(GameObject* g, Ability* a, float x, float y, float headingx, float headingy, GameObject* target);
float GetManaCost(Ability* a);
bool AbilityIsInitialised(Ability* a);
void ToggleAbility(Ability* a, GameObject* ownedBy, bool toggled);
bool AbilityCanBeCast(Ability* a, GameObject* g, GameObject* target, float x, float y);
bool AbilityIsCastImmediately(Ability* a);
bool AbilityShouldBeCastOnTarget(Ability* a);
int GetAbilityIndexClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void CastAbilityOnMouse(MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyState);
void UpdateAbilityInteractions(ALLEGRO_KEYBOARD_STATE* keyState,ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState mouseState,MouseState mouseStateLastFrame);
void DrawHeldAbility(MouseState* mouseState);
void InitAbilities();

bool ObjectHasManaToCast(GameObject* g, Ability* a); 
bool AbilityIsOnCooldown(Ability* a);
bool AbilityCanBeCastOnGround(Ability* a);

void ModifyAbilityCooldownTimer(Ability* a, float value);

void RemoveAbility(Ability* a, GameObject* from);
void LowerAbilityCooldown(Ability* a, float amt);