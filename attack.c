#include "attack.h"
#include "string.h"
#include "stdlib.h"
#include "rect.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "colors.h"
#include <allegro5/allegro_primitives.h>
#include "luafuncs.h"
#include "ability.h"
int attack_top = 0;

void InitAttacks()
{
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        freeAttacks[i] = i;
    }
    memset(attacks,0,sizeof(Attack)*MAX_ATTACKS);
}

Attack* AddAttack(Attack* a)
{
    if (attack_top >= MAX_ATTACKS)
        return NULL;
    int index = freeAttacks[attack_top];
    attacks[freeAttacks[attack_top]] = *a;
    attacks[freeAttacks[attack_top]].properties |= ATTACK_ACTIVE;
    attack_top++;   
    return &attacks[index];
}
void RemoveAttack(int attackindex)
{
    Attack* a = &attacks[attackindex];
    if (a->cameFrom)
    {
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,a->cameFrom->luafunc_onhit);

        lua_pushinteger(luaState,a->x);
        lua_pushinteger(luaState,a->y);    
        lua_pushinteger(luaState,(-1));    
        lua_pcall(luaState,3,0,0);
    }
    a->properties = '\0';

    if (attack_top < 0)
        return;
    attack_top--;
    freeAttacks[attack_top] = attackindex;
    if (a->effects)
    {
        free(a->effects);
        a->effects = NULL;
    }

}

void ApplyAttack(Attack* a, GameObject* target)
{
    bool apply = true;
    if (a->attackType == ATTACK_AOE)
    {
        if (a->timer > a->tickrate)
        {
            a->timer = 0;
        }
        else
        {
            apply = false;
        }
    }
    if (apply)
    {
        for (int i = 0; i < a->numEffects; i++)
        {
            ApplyEffect(&a->effects[i],currGameObjRunning,target);
        }
        if (a->shouldCallback)
        {
            currAbilityRunning = a->cameFrom; 
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,a->cameFrom->luafunc_onhit);

            lua_pushinteger(luaState,a->x);
            lua_pushinteger(luaState,a->y);    
            lua_pushinteger(luaState,(int)(target-objects));    

            lua_pcall(luaState,3,0,0);


            //CallLuaFunc(a->callback_onhit);
        }
    }
}
void DrawAttack(Attack* a, float dt)
{
    if (a->attackType == ATTACK_AOE)
    {
        al_draw_circle(a->x,a->y,a->radius,FRIENDLY,1);
    }
    else
    {
        al_draw_filled_circle(a->x,a->y,a->radius,FRIENDLY);
    }
}
void UpdateAttack(Attack* a, float dt)
{
    if (a->cameFrom->castType != ABILITY_TOGGLE)
    {
        a->duration -= dt;
    }
    a->timer += dt;

    currAttackRunning = a;
    if (a->x < 0 || a->y < 0 || a->x > 255 || a->y > 255 || a->duration < 0)
    {
        RemoveAttack(a-attacks);
        return;
    }
    if (a->target)
    {

        Rect r = GetObjRect(a->target);
        MoveTo(&a->x,&a->y,r.x+r.w/2.,r.y+r.h/2,a->speed,dt);
    }
    else
    {
        if (a->attackType == ATTACK_PROJECTILE_ANGLE)
        {
            MoveAngle(&a->x,&a->y,a->targx,a->targy,a->speed,dt);
        }
        else if (a->attackType == ATTACK_AOE)
        {

        }
        else
        {
            if (MoveTo(&a->x,&a->y,a->targx,a->targy,a->speed,dt))
            {
                RemoveAttack(a-attacks);
                return;
            }
        }


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
           // if (a->ownedBy == &objects[i])
             //   continue;
            Rect r = GetObjRect(&objects[i]);
            if (CircleInRect(a->x,a->y,a->radius,r))
            {   
                ApplyAttack(a,&objects[i]);
                if (a->attackType != ATTACK_AOE)
                {  
                    RemoveAttack(a-attacks);
                }
            }
        }
    }
    if (a->cameFrom)
    {
        currAbilityRunning = a->cameFrom;
        currGameObjRunning = a->ownedBy;

        lua_rawgeti(luaState,LUA_REGISTRYINDEX,currAbilityRunning->luafunc_tick);

        lua_pushnumber(luaState,a->x + a->radius/2.0f);
        lua_pushnumber(luaState,a->y + a->radius/2.0f);
        lua_pushnumber(luaState,a->timer);    
        lua_pushinteger(luaState,currGameObjRunning-objects);
        lua_pushinteger(luaState,a->target - objects);


        lua_pcall(luaState,5,0,0);

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