#include <limits.h>

#include "threat.h"

#include "gameobject.h"
#include "gamestate.h"

void AddThreat(GameObject* source, GameObject* target, int damage)
{
    if (GetPlayerOwnedBy(source) == GetPlayerOwnedBy(target) || !IsActive(source) || !IsActive(target) || ObjIsDecoration(target) || ObjIsDecoration(source))
        return;

    if (damage > 0)
        StartCombat();
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
            next->threat += damage * source->threatMultiplier;
            found = true;
            //break;
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

void RemoveObjFromAllThreatlists(GameObject* g)
{
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g2 = activeObjects[i];
        if (IsActive(g2))
        {
            Threat* next = &g2->threatList;
            Threat* previous = &g2->threatList;
            while (1)
            {
                if (next)
                {
                    if (next->obj == g)
                    {
                        Threat* patched = next->next;
                        previous->next = patched;

                        next->obj = NULL;
                        next->threat = 0;
                        //if this isn't the first element of the list (not malloced)
                        if (next != &g2->threatList)
                            free(next);
                        break;
                    }
                    previous = next;
                    next = next->next;
                }
                else
                    break;
            }      
        }
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
void DeleteThreat(Threat* t)
{
    if (!t) return;
    free(t);
}
Threat* GetHighestThreat(Threat* threatList)
{
    int highest = INT_MIN;
    Threat* next = threatList;
    Threat* highestThreat = NULL;
    while (1)
    {
        if (next->obj && !(next->obj->properties & OBJ_ACTIVE))
        {
            next = next->next;
            if (next == NULL) 
                break;
        }

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
        if (next->obj)
            i++;
        next = next->next;
        if (next == NULL)
        {
            break;
        }
    }
    return i;

}   