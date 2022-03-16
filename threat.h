#pragma once

typedef struct GameObject GameObject;

typedef struct Threat
{
    GameObject* obj;
    int threat; 

    struct Threat* next; 
} Threat;

void AddThreat(GameObject* source, GameObject* target, int damage);
Threat* GetHighestThreat(Threat* threatList);
int GetNumThreats(Threat* threatList);
