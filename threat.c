
#include "threat.h"
#include "gameobject.h"
#include "limits.h"
void AddThreat(GameObject* source, GameObject* target, int damage)
{
    Threat* next = &target->threatList;

    if (!next->obj)
    {
        next->obj = source;
        next->threat = damage;
        return;
    }

    bool found = false;

    int i = 0;
    while (1)
    {
        if (next->obj == source)
        {
            next->threat += damage;
            found = true;
            break;
        }
        if (next->next == NULL)
        {
            break;
        }
        next = next->next;
    }
    if (!found)
    {
        Threat* t = calloc(1,sizeof(Threat));
        t->obj = source;
        t->threat = damage;
        next->next = t;
    }
}

void DeleteThreatList(GameObject* g)
{
    int numElements = 0;
    Threat* next = g->threatList.next;
    if (!next)
        return; 
    while (1)
    {
        numElements++;
        next = next->next;
        if (next == NULL)
            break;
    }

    Threat** list = calloc(numElements,sizeof(Threat*));
    next = g->threatList.next;
    for (int i = 0; i < numElements; i++)
    {
        list[i] = next;
        next = next->next;
    }
    for (int i = 0; i < numElements; i++)
    {
        if (list[i])
            free(list[i]);
    }
    g->threatList.obj = NULL;
    g->threatList.threat = 0;
    g->threatList.next = NULL;


    free(list);
}
void KillObj(GameObject* g)
{
    g->properties &= ~OBJ_ACTIVE;
    g->spriteIndex = 0;
    DeleteThreatList(g);
}
Threat* GetHighestThreat(Threat* threatList)
{
    int highest = -INT_MIN;
    Threat* next = threatList;
    Threat* highestThreat;
    while (1)
    {
        if (next->threat > highest)
        {
            highest = next->threat;
            highestThreat = next;
        }
        next = next->next;
        if (next == NULL)
        {
            break;
        }
    }
    return highestThreat;
}
int GetNumThreats(Threat* threatList)
{
    int i = 0;
    Threat* next = threatList;
    if (!next->obj)
    {
        return 0;
    }
    while (1)
    {
        i++;
        next = next->next;
        if (next == NULL)
        {
            break;
        }
    }
    return i;

}   