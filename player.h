#include "vector2.h"
typedef struct Player
{ 
    bool aicontrolled;
    bool selecting;
    Vector2 selectionStart;
    #define MAXUNITSSELECTED 12 //hell yeaaa bw
    GameObject* selection[MAXUNITSSELECTED];
    int numUnitsSelected;
} Player;

Player* players;
