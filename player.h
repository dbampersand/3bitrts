#pragma once

#include "stdbool.h"
#include "point.h"
#include "helperfuncs.h"

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

    float gold;
    char* goldText;
    float goldAnimationTimer;

    float goldDisplaying;

    int32_t bankedGold;
} Player;

//The point (8, SCREEN_SIZE-8, 8, SCREEN_SIZE-8) at which the camera starts to pan
#define SCREEN_EDGE 8
extern float cameraSpeed;


extern Player* players;

typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
typedef struct MouseState MouseState; 
typedef struct Item Item;


void RemoveFromSelection(Player* p, int index);
void RemoveGameObjectFromSelection(Player* p, GameObject* g);
void InitPlayers();
void CheckAbilityClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,MouseState* mouseState);
void ToScreenSpace(float* x,float* y);
void ToScreenSpaceI(int* x,int* y);

void ToWorldSpace(float* x,float* y);
void ToWorldSpaceI(int* x,int* y);
float ToWorldSpace_X(float x);
float ToWorldSpace_Y(float y);
float ToScreenSpace_X(float x);
float ToScreenSpace_Y(float y);
void FocusCameraOnPos(float x, float y);
void MoveCamera(MouseState mouseState,ALLEGRO_KEYBOARD_STATE* keyState, float dt);
void MoveCam(float x, float y);
void ClearSelection();
bool BuyItem(Item* i);
int GetGold();
void AddGold(float count);
void DrawGoldCount();
void ClearGold();
void UpdateGoldAnimationTimer(float dt);


