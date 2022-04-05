#include "vector2.h"
typedef struct Ability Ability; 
typedef struct Player
{ 
    bool aicontrolled;
    bool selecting;
    Vector2 selectionStart;
    #define MAXUNITSSELECTED 12 //hell yeaaa bw
    GameObject* selection[MAXUNITSSELECTED];
    int numUnitsSelected;

    Ability* abilityHeld;

    GameObject* controlGroups[10][MAXUNITSSELECTED];
} Player;

Player* players;
