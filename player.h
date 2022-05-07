#pragma once
#include "vector2.h"
#include "stdbool.h"
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

} Player;

Player* players;


void RemoveFromSelection(Player* p, int index);
void RemoveGameObjectFromSelection(Player* p, GameObject* g);
