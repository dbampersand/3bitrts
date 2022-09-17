#include "shield.h"

#include "gameobject.h"


float GetTotalShield(GameObject* g)
{
    float amt=0;
    for (int i = 0; i < g->numShields; i++)
    {
        amt += g->shields[i].amtLeft;
    }
    return amt;
}
void AttachShield(GameObject* g, float duration, float value)
{
    g->numShields++;
    if (!g->shields)
    {
        g->numShields = 1;
        g->shields = calloc(g->numShields,sizeof(Shield));
    }
    else
    {   
        g->shields = realloc(g->shields,g->numShields*sizeof(Shield));
    }
    g->shields[g->numShields-1] = (Shield){.duration = duration, .amtLeft = value};

}
void RemoveShield(GameObject* g, Shield* s)
{
    int index = s - g->shields;
    for (int i = index; i < g->numShields-1; i++)
    {
        g->shields[i] = g->shields[i+1];
    }
    g->numShields--;
    g->shields = realloc(g->shields,g->numShields*sizeof(Shield));
}
void ProcessShields(GameObject* g, float dt)
{
    if (!g->shields)
    {
        return;
    }
    for (int i = 0; i < g->numShields; i++)
    {
        g->shields[i].duration -= dt;
        if (g->shields[i].duration <= 0) 
        {
            RemoveShield(g,&g->shields[i]);
            return;
        }
        if (g->shields[i].amtLeft <= 0) 
        {
            RemoveShield(g,&g->shields[i]);
        }

    }
}
float DamageShield(GameObject* g, Shield* s, float damage)
{
    s->amtLeft -= damage;
    if (s->amtLeft < 0)
    {
        RemoveShield(g,s);
    }
    return s->amtLeft;
}
float DamageShields(GameObject* g, float damage)
{
    for (int i = 0; i < g->numShields; i++)
    {
        float diff = damage - g->shields[i].amtLeft; 
        float amountToMove = damage; 

        g->shields[i].amtLeft -= amountToMove;
        if (g->shields[i].amtLeft < 0)
        {
            RemoveShield(g,&g->shields[i]);
            i--;
        }
        if (diff > 0)
        {
            damage = diff; 
        }
        if (diff <= 0)
        {
            return 0;
        }
    }
    return damage;
}
void UpdateShield(GameObject* g, float dt)
{
    if (GetTotalShield(g) > 0)
    {
        g->shieldSizeTimer += dt * (1/SHIELD_EFFECT_TIME);
        if (g->shieldSizeTimer > 1)
            g->shieldSizeTimer = 1;
    }
    else
    {
        g->shieldSizeTimer -= dt * (1/SHIELD_EFFECT_TIME);
        if (g->shieldSizeTimer < 0)
            g->shieldSizeTimer = 0;

    }
}