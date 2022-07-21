#pragma once

#include "stdbool.h"
#include "sound.h"

typedef struct GameObject GameObject;

typedef enum ABILITY_CAST_TYPE
{
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
    char* luabuffer;
    int luafunc_casted;
    int luafunc_setup;
    int luafunc_onhit;
    int luafunc_abilitytick;
    int luafunc_untoggle;
    int luafunc_ontimeout;

    float range;
    //float length;
    char* path;
    int spriteIndex_Portrait;
    float cooldown;
    float cdTimer;
    
    float manaCost;

    ABILITY_CAST_TYPE castType;

    bool toggled;

    char* description;

    HINT targetingHint;
    float hintRadius;


} Ability;


Ability* abilities;
int numAbilities;
int numAbilitiesAllocated; 


SoundIndex ability_UI_click_sound;
SoundIndex ability_UI_click_up_sound;



typedef struct lua_State lua_State;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;

void SetManaCost(Ability* a, float mana);

void LoadAbility(const char* path, lua_State* l, Ability* a);
Ability CloneAbilityPrefab(Ability* prefab, lua_State* l);
Ability* AddAbility(const char* path);
void CastAbility(GameObject* g, Ability* a, float x, float y, float headingx, float headingy, GameObject* target);
float GetManaCost(Ability* a);
bool AbilityIsInitialised(Ability* a);
void ToggleAbility(Ability* a, GameObject* ownedBy, bool toggled);
bool AbilityCanBeCast(Ability* a, GameObject* g, GameObject* target, float x, float y);
bool AbilityIsCastImmediately(Ability* a);
bool AbilityShouldBeCastOnTarget(Ability* a);
int GetAbilityIndexClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void CastAbilityOnMouse(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyState);
void UpdateAbilityInteractions(ALLEGRO_KEYBOARD_STATE* keyState,ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void DrawHeldAbility(ALLEGRO_MOUSE_STATE* mouseState);
void InitAbilities();

bool ObjectHasManaToCast(GameObject* g, Ability* a); 
bool AbilityIsOnCooldown(Ability* a);
