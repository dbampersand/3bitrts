#pragma once

#include "stdbool.h"
#include "point.h"

typedef struct Ability Ability; 
typedef struct GameObject GameObject;

typedef struct Player
{ 
    bool aicontrolled;
    bool selecting;
    Point selectionStart;
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

//The point (16, SCREEN_SIZE-16, 16, SCREEN_SIZE-16) at which the camera starts to pan
#define SCREEN_EDGE 16
float cameraSpeed;

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
void FocusCameraOnPos(float x, float y);
void MoveCamera(ALLEGRO_MOUSE_STATE mouseState,ALLEGRO_KEYBOARD_STATE* keyState, float dt);
void MoveCam(float x, float y);
void ClearSelection();
