#pragma once

#include "vector2.h"
#include "stdbool.h"
#include "point.h"

typedef struct Ability Ability; 
typedef struct GameObject GameObject;

typedef struct Player
{ 
    bool aicontrolled;
    bool selecting;
    Vector2 selectionStart;
    #define MAXUNITSSELECTED 12 //hell yeaaa bw
    GameObject* selection[MAXUNITSSELECTED];
    int indexSelectedUnit;
    int numUnitsSelected;

    Ability* abilityHeld;

    GameObject* controlGroups[10][MAXUNITSSELECTED];
    GameObject* clickedThisFrame;

    bool amoveSelected;

    Point cameraPos;

} Player;

Player* players;

typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE; 


void RemoveFromSelection(Player* p, int index);
void RemoveGameObjectFromSelection(Player* p, GameObject* g);
void InitPlayers();
void CheckAbilityClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,ALLEGRO_MOUSE_STATE* mouseState);
void ToScreenSpace(float* x,float* y);
void ToScreenSpaceI(int* x,int* y);

void ToWorldSpace(float* x,float* y);
void ToWorldSpaceI(int* x,int* y);
float ToWorldSpace_X(float x);
float ToWorldSpace_Y(float y);
float ToScreenSpace_X(float x);
float ToScreenSpace_Y(float y);
