#include "attack.h"
#include "string.h"
#include "stdlib.h"
#include "rect.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "colors.h"
#include <allegro5/allegro_primitives.h>

int attack_top = 0;

void AddAttack(Attack* a)
{
    if (attack_top >= MAX_ATTACKS)
        return;
    attacks[freeAttacks[attack_top]] = *a;
    attacks[freeAttacks[attack_top]].properties |= ATTACK_ACTIVE;
    attack_top++;   

}
void InitAttacks()
{
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        freeAttacks[i] = i;
    }
    memset(attacks,0,sizeof(Attack)*MAX_ATTACKS);
}
void RemoveAttack(int attackindex)
{
    if (attack_top < 0)
        return;
    attack_top--;
    freeAttacks[attack_top] = attackindex;
    attacks[attackindex].properties = '\0';
    if (attacks[attackindex].effects)
    {
        free(attacks[attackindex].effects);
    }

}

void ApplyAttack(Attack* a, GameObject* target)
{
    for (int i = 0; i < a->numEffects; i++)
    {
        ApplyEffect(&a->effects[i],target);
    }
}
void DrawAttack(Attack* a, float dt)
{
    al_draw_filled_circle(a->x,a->y,a->radius,FRIENDLY);
}
void UpdateAttack(Attack* a, float dt)
{
    if (a->target)
    {
        Rect r = GetObjRect(a->target);
        MoveTo(&a->x,&a->y,r.x+r.w/2.,r.y+r.h/2,a->speed,dt);
    }
    else
    {
        MoveTo(&a->x,&a->y,a->targx,a->targy,a->speed,dt);

        for (int i = 0; i < numObjects; i++)
        {
            int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);

            if (a->properties & ATTACK_HITS_ENEMIES)
            {
                int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);
                int objOwnedBy = GetPlayerOwnedBy(&objects[i]);

                if (objOwnedBy == abilityOwnedBy)
                {
                    continue;
                }
            }
            else
            {
                int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);
                int objOwnedBy = GetPlayerOwnedBy(&objects[i]);

                if (objOwnedBy != abilityOwnedBy)
                {
                    continue;
                }

            }
            if (a->ownedBy == &objects[i])
                continue;
            Rect r = GetObjRect(&objects[i]);
            if (CircleInRect(a->x,a->y,a->radius,r))
            {   
                ApplyAttack(a,&objects[i]);
                RemoveAttack(a-attacks);
            }
        }
    }
}
void UpdateAttacks(float dt)
{
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        Attack* a = &attacks[i];
        if (a->properties & ATTACK_ACTIVE)
        {
            UpdateAttack(a,dt);
        }
    }   
}
void DrawAttacks(float dt)
{
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        Attack* a = &attacks[i];
        if (a->properties & ATTACK_ACTIVE)
        {
            DrawAttack(a,dt);
        }
    }   
}