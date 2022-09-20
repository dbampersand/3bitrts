#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "allegro5/allegro_primitives.h"

#include "attack.h"

#include "rect.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "colors.h"
#include "luafuncs.h"
#include "ability.h"
#include "video.h"
#include "particle.h"
#include "player.h"
#include "map.h"

int attack_top = 0;

void InitAttacks()
{
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        freeAttacks[i] = i;
    }
    memset(attacks,0,sizeof(Attack)*MAX_ATTACKS);
}
Attack* CreateAoE(float x, float y, char* effectPortrait, float radius, float tickrate, float duration, bool shouldCallback, int properties, int color, int dither, int numEffects, Effect* effects, GameObject* target, GameObject* from)
{
    Attack a = {0};
    a.ownedBy = from;
    a.x = x;
    a.y = y;    
    a.targx = x;
    a.targy = y;
    a.radius = 0;
    a.easing = 0.85f;
    a.targetRadius = radius;
    a.effects = calloc(numEffects,sizeof(Effect));
    memcpy(a.effects,effects,sizeof(Effect)*numEffects);
    a.numEffects = numEffects;
    //a.ownedBy = currGameObjRunning;
    a.properties = properties;
    a.cameFrom = currAbilityRunning;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.attackType = ATTACK_AOE;
    a.tickrate = tickrate;
    a.color = color;
    a.dither = dither;
    a.target = target;

    Attack* ref = AddAttack(&a);
    return ref;

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
    if (attackindex < 0 || attackindex >= MAX_ATTACKS)
        return;
    
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

    if (attack_top <= 0)
        return;
    attack_top--;
    freeAttacks[attack_top] = attackindex;
    if (a->effects)
    {
        for (int i = 0; i < a->numEffects; i++)
        {
            RemoveEffect(&a->effects[i],NULL,false);
        }
        free(a->effects);
        a->effects = NULL;
    }


}
bool AttackIsSoak(Attack* a)
{
    return a->properties & ATTACK_SOAK;
}
int NumUnitsInsideAttack(Attack* a)
{
    int numObjects = 0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g))
        {
            Rect r = GetObjRect(&objects[i]);
            if (CircleInRect(a->x,a->y,a->targetRadius,r))
            {   
                int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);

                if (a->properties & ATTACK_HITS_ENEMIES)
                {
                    int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);
                    int objOwnedBy = GetPlayerOwnedBy(&objects[i]);

                    if (a->ownedBy)
                    {
                        if (objOwnedBy != abilityOwnedBy)
                        {
                            numObjects++;
                            continue;
                        }
                    }
                    else
                    {
                        if (objOwnedBy == TYPE_FRIENDLY)
                        {
                            numObjects++;
                            continue;
                        }
                    }
                }
                else if (a->properties & ATTACK_HITS_FRIENDLIES)
                {
                    int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);
                    int objOwnedBy = GetPlayerOwnedBy(&objects[i]);
                    if (a->ownedBy)
                    {
                        if (objOwnedBy == abilityOwnedBy)
                        {
                            numObjects++;
                            continue;
                        }
                    }
                    else
                    {
                        if (objOwnedBy == TYPE_ENEMY)
                        {
                            numObjects++;
                            continue;
                        }
                    }

                }
            }
        }

    }
    return numObjects;

}
void RemoveAllAttacks()
{
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        RemoveAttack(i);
    }
    players[0].abilityHeld = NULL;
}
void ApplyAttack(Attack* a, GameObject* target)
{
    bool apply = true;
    if (a->attackType == ATTACK_AOE || a->attackType == ATTACK_CONE)
    {
        if (a->timer > a->tickrate)
        {
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
        }
    }
}
void draw_circle_dithered(float cX, float cY, float radius, ALLEGRO_COLOR color, DITHER_PATTERN dither)
{
    if (dither == DITHER_NONE)
    {
        return;
    }
    if (dither == DITHER_FILL)
    {
        al_draw_filled_circle(cX,cY,radius,color);
    }
    if (dither == DITHER_HALF || dither == DITHER_QUARTER || dither == DITHER_EIGTH)
    {
        for (int x = cX-radius; x < cX+radius; x++)
        {
            for (int y = cY-radius; y < cY+radius; y++)
            {
                if (PointInCircle(x,y,cX,cY,radius))
                {
                    if (y%dither==0 && x%dither == 0)
                    {
                        al_draw_pixel(x,y,color);
                    }
                }
            }

        }
    }
    //todo: could make this one more efficient
    if (dither == DITHER_VERTICAL_HALF || dither == DITHER_VERTICAL_QUARTER || dither == DITHER_VERTICAL_EIGTH)
    {
        int pattern;
        if (dither == DITHER_VERTICAL_HALF)
            pattern=2;
        if (dither == DITHER_VERTICAL_QUARTER)
            pattern=4;
        if (dither == DITHER_VERTICAL_EIGTH)
            pattern=8;
        for (int x = cX-radius; x < cX+radius; x+=pattern)
        {
            for (int y = cY-radius; y < cY+radius; y++)
            {
                if (PointInCircle(x,y,cX,cY,radius))
                {
                    al_draw_pixel(x,y,color);
                }
            }
        }
    }
    if (dither == DITHER_HORIZONTAL_HALF || dither == DITHER_HORIZONTAL_QUARTER || dither == DITHER_HORIZONTAL_EIGTH)
    {
        int pattern;
        if (dither == DITHER_HORIZONTAL_HALF)
            pattern=2;
        if (dither == DITHER_HORIZONTAL_QUARTER)
            pattern=4;
        if (dither == DITHER_HORIZONTAL_EIGTH)
            pattern=8;
        for (int y = cY-radius; y < cY+radius; y+=pattern)
        {
            for (int x = cX-radius; x < cX+radius; x++)
            {
                if (PointInCircle(x,y,cX,cY,radius))
                {   
                    al_draw_pixel(x,y,color);
                }
            }
        }
    }
    if (dither == DITHER_STAR_HALF || dither == DITHER_STAR_QUARTER || dither == DITHER_STAR_EIGTH)
    {
        int pattern;
        if (dither == DITHER_STAR_HALF)
            pattern=2;
        if (dither == DITHER_STAR_QUARTER)
            pattern=4;
        if (dither == DITHER_STAR_EIGTH)
            pattern=8;
        for (int x = cX-radius-pattern*2; x < cX+radius; x+=pattern*2)
        {
            for (int y = cY-radius-pattern*2; y < cY+radius; y+=pattern*2)
            {
                if (PointInCircle(x,y,cX,cY,radius))
                    al_draw_pixel(x,y,color);
                if (PointInCircle(x-1,y+1,cX,cY,radius))
                    al_draw_pixel(x-1,y+1,color);
                if (PointInCircle(x+1,y+1,cX,cY,radius))
                    al_draw_pixel(x+1,y+1,color);
                if (PointInCircle(x,y+2,cX,cY,radius))
                    al_draw_pixel(x,y+2,color);
            }
        }
    }
}
void DrawAttack(Attack* a, float dt)
{
    if (a->attackType == ATTACK_AOE)
    {
        Color c = a->color;
        float timeleft = a->duration;
        if (timeleft < 4.0f && timeleft >= 0.1f)
        {
            int coef = (_TARGET_FPS*2) - (1/timeleft * (_TARGET_FPS/1.5f));
            coef = coef > (_TARGET_FPS/14) ? coef : _TARGET_FPS/14;
            if (_FRAMES % (coef) >= 0 && _FRAMES % (coef) < _TARGET_FPS/10)
            {
                c = COLOR_GROUND_DARK;
            }
        }

        if (a->dither == DITHER_NONE || a->properties &  ATTACK_DRAW_CIRCLE)
            al_draw_circle(ToScreenSpace_X(a->x),ToScreenSpace_Y(a->y),a->radius,GetColor(c,GetPlayerOwnedBy(a->ownedBy)),1);
        draw_circle_dithered(ToScreenSpace_X(a->x),ToScreenSpace_Y(a->y),a->radius,GetColor(c,GetPlayerOwnedBy(a->ownedBy)),a->dither);
        if (AttackIsSoak(a))
        {
            float move = ((_FRAMES)%10)/4.0f;
            float x = a->x;
            float y = a->y - a->targetRadius - 10 - move;

            float endX = a->x;
            float endY = a->y - a->targetRadius - 5 - move;
            RotatePointF(&x,&y,a->x,a->y,DegToRad(45));
            RotatePointF(&endX,&endY,a->x,a->y,DegToRad(45));

            for (int i = 0; i < 4; i++)
            {
                RotatePointF(&x,&y,a->x,a->y,DegToRad(90));
                RotatePointF(&endX,&endY,a->x,a->y,DegToRad(90));
                
                DrawArrow(ToScreenSpace_X(endX),ToScreenSpace_Y(endY),ToScreenSpace_X(x),ToScreenSpace_Y(y),GetColor(c,GetPlayerOwnedBy(a->ownedBy)));
            }
        }
    }
    else if (a->attackType == ATTACK_CONE)
    {
        float x2 = a->targx; float y2 = a->targy;
        if (a->target)
        {
            GetCentre(a->target,&x2,&y2);
            Normalize(&x2,&y2);
            x2 *= a->x + a->range;
            y2 *= a->y + a->range;
            //al_draw_pixel(x2,y2,POISON);
        }
        float angle = RadToDeg(atan2(y2-a->y,x2-a->x));
        DrawCone(ToScreenSpace_X(a->x),ToScreenSpace_Y(a->y),angle,a->radius,a->range,FRIENDLY);
    }
    else
    {
        al_draw_filled_circle(ToScreenSpace_X(a->x),ToScreenSpace_Y(a->y),a->radius,GetColor(a->color,GetPlayerOwnedBy(a->ownedBy)));
        al_draw_filled_circle(ToScreenSpace_X(a->x+2),ToScreenSpace_Y(a->y+2),a->radius,BG);

    }
}
bool AttackIsAOE(Attack* a)
{
    return (a->attackType == ATTACK_AOE);
}
bool AttackIsProjectile(Attack* a)
{
    return (a->attackType == ATTACK_PROJECTILE_TARGETED || a->attackType == ATTACK_PROJECTILE_POINT || a->attackType == ATTACK_PROJECTILE_ANGLE);
}
void UpdateAttack(Attack* a, float dt)
{
    if (!(a->properties & ATTACK_ACTIVE))
        return;
    if (AttackIsAOE(a))
    {
        if (_FRAMES%(int)(_TARGET_FPS/AOE_PARTICLES_PER_SECOND) == 0)
        {
            RandParticleAroundEdgeOfCircle(a->x,a->y,a->radius,3,1,a->color);
        }
    }

    if (a->cameFrom)
    {
        if (a->cameFrom->castType != ABILITY_TOGGLE)
        {
            a->duration -= dt;
        }
    }
    else
    {
        a->duration -= dt;
    }
    float amtToIncreaseBy = (a->timer*a->timer*a->timer*a->timer);

    if (a->radius != a->targetRadius)
    {
        if (a->radius < a->targetRadius && a->radius + amtToIncreaseBy < a->targetRadius)
        {
            a->radius+=amtToIncreaseBy;
        }
        else if (a->radius > a->targetRadius && a->radius + amtToIncreaseBy > a->targetRadius)
        {
            a->radius-=amtToIncreaseBy;
        }
        else
        {
            a->radius = a->targetRadius;
        }
    }

    a->radius = a->easing*a->easing*a->easing * (a->targetRadius);
    if (a->easing >= 1)
        a->radius = a->targetRadius;
    a->easing += dt*2.5f;
    a->timer += dt;

    Attack* copied = NULL;
    Attack copy = *a;

    bool isSoak = false;

    currAttackRunning = a;
    if (a->target ) 
    {
    }
    //else
    {
        if (a->attackType == ATTACK_PROJECTILE_ANGLE)
        {
            MoveAngle(&a->x,&a->y,a->targx,a->targy,a->speed,dt);
        }
        else if (a->attackType == ATTACK_AOE || a->attackType == ATTACK_CONE)
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

        if (AttackIsSoak(a))
        {
            isSoak = true;
            int numUnits = NumUnitsInsideAttack(a);
            copy.effects = calloc(a->numEffects,sizeof(Effect));
            for (int i = 0; i < a->numEffects; i++)
            {
                copy.effects[i] = a->effects[i];
                if (numUnits > 0)
                    copy.effects[i].value /= numUnits;
            }
            copied = &copy;

        }


        for (int i = 0; i < numObjects; i++)
        {
            if (!IsActive(&objects[i]))
                continue;
            int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);

            //if (&objects[i] == &objects[abilityOwnedBy] && (AttackIsProjectile(a)))
              //  continue;


            if (a->properties & ATTACK_HITS_ENEMIES)
            {
                int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);
                int objOwnedBy = GetPlayerOwnedBy(&objects[i]);

                if (a->ownedBy && (!ObjIsDecoration(a->ownedBy)))
                {
                    if (objOwnedBy == abilityOwnedBy)
                    {
                        continue;
                    }
                }
                else
                {
                    if (objOwnedBy == TYPE_FRIENDLY)
                        continue;
                }
            }
            else if (a->properties & ATTACK_HITS_FRIENDLIES)
            {
                int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);
                int objOwnedBy = GetPlayerOwnedBy(&objects[i]);
                if (a->ownedBy && (!ObjIsDecoration(a->ownedBy)))
                {
                    if (objOwnedBy != abilityOwnedBy)
                    {
                        continue;
                    }
                }
                else
                {
                    if (objOwnedBy == TYPE_ENEMY)
                        continue;
                }

            }
           // if (a->ownedBy == &objects[i])
             //   continue;
            Rect r = GetObjRect(&objects[i]);
            if (a->attackType == ATTACK_AOE || a->attackType == ATTACK_PROJECTILE_TARGETED || a->attackType ==   ATTACK_PROJECTILE_POINT ||a->attackType == ATTACK_PROJECTILE_ANGLE)
            {
                if (CircleInRect(a->x,a->y,a->targetRadius,r))
                {   
                    if (isSoak)
                        ApplyAttack(copied,&objects[i]);
                    else
                        ApplyAttack(a,&objects[i]);

                    if (a->attackType != ATTACK_AOE && a->attackType != ATTACK_CONE)
                    {  
                        RemoveAttack(a-attacks);

                        //We had to copy to a new attack 
                        if (isSoak)
                        {
                            free(copied->effects);
                        }
                        return;
                    }
                }
            }
            if (a->attackType == ATTACK_CONE)
            {
                float x2; float y2;
                GetCentre(&objects[i],&x2,&y2);

                float angle = RadToDeg(atan2(y2-a->y,x2-a->x));
                Rect r = GetObjRect(&objects[i]);
                if (RectInCone(r,a->x,a->y,angle,a->radius,a->range))
                {
                    ApplyAttack(a,&objects[i]);
                }
            }
        }
        if (a->attackType == ATTACK_AOE || a->attackType == ATTACK_CONE)
        {
            if (a->timer > a->tickrate)
            {
                a->timer = 0;
            }
        }
    }
    if (a->cameFrom)
    {
        currAbilityRunning = a->cameFrom;
        currGameObjRunning = a->ownedBy;
        //currAttackRunning = a;
        
        if (currAbilityRunning)
        {
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,currAbilityRunning->luafunc_abilitytick);

            lua_pushnumber(luaState,a->x + a->radius/2.0f);
            lua_pushnumber(luaState,a->y + a->radius/2.0f);
            lua_pushnumber(luaState,a->timer);    
            lua_pushinteger(luaState,currGameObjRunning-objects);
            lua_pushinteger(luaState,a->target - objects);
            lua_pushnumber(luaState,dt);
            lua_pushinteger(luaState,a-attacks);
            lua_pushnumber(luaState,a->duration);


            lua_pcall(luaState,8,0,0);
        }

    }
    if (isSoak)
    {
        free(copied->effects);
    }
    if (a->x < 0 || a->y < 0 || a->x > GetMapWidth() || a->y > GetMapHeight() || a->duration < 0)
    {
        if (a->shouldCallback)
        {
            
            currAbilityRunning = a->cameFrom;
            currGameObjRunning = a->ownedBy;
            currAttackRunning = a;

            lua_rawgeti(luaState,LUA_REGISTRYINDEX,currAbilityRunning->luafunc_ontimeout);

            lua_pushnumber(luaState,a->x);
            lua_pushnumber(luaState,a->y);
            lua_pushinteger(luaState,a->ownedBy-objects);
            lua_pushnumber(luaState,dt);    
            lua_pushinteger(luaState,a->target - objects);
            lua_pushinteger(luaState,a-attacks);
            lua_pcall(luaState,6,0,0);

        }
        RemoveAttack(a-attacks);
        return;
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