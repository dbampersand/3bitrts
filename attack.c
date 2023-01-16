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
#include "pointspace.h"
#include "particle.h"
#include "player.h"
#include "map.h"

Attack attacks[MAX_ATTACKS] = {0};
unsigned char freeAttacks[MAX_ATTACKS] = {0}; 
int attack_top = 0;
int numActiveAttacks = 0;
ALLEGRO_BITMAP* cachedAttackSprites[MAX_AOE_CIRCUMFERENCE_SIZE+1][DITHER_ALL] = {0};


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
    a->playerOwnedBy = GetPlayerOwnedBy(a->ownedBy);
    int index = freeAttacks[attack_top];
    attacks[freeAttacks[attack_top]] = *a;
    attacks[freeAttacks[attack_top]].properties |= ATTACK_ACTIVE;
    attack_top++;   
    numActiveAttacks++;
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
    numActiveAttacks--;
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
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        if (IsActive(g))
        {
            Rect r = GetObjRect(activeObjects[i]);
            if (CircleInRect(a->x,a->y,a->targetRadius,r))
            {   
                int abilityOwnedBy = a->playerOwnedBy;//GetPlayerOwnedBy(a->ownedBy);

                if (a->properties & ATTACK_HITS_ENEMIES)
                {
                    int abilityOwnedBy = a->playerOwnedBy;//GetPlayerOwnedBy(a->ownedBy);
                    int objOwnedBy = GetPlayerOwnedBy(activeObjects[i]);

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
                    int objOwnedBy = GetPlayerOwnedBy(activeObjects[i]);
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
            ApplyEffect(&a->effects[i],a->ownedBy,target);
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
//TODO: replace the long strings of put_pixel with a for loop iterating over bitmap
void draw_circle_dithered(float cX, float cY, float radius, ALLEGRO_COLOR color, DITHER_PATTERN dither)
{
    if (radius > MAX_AOE_CIRCUMFERENCE_SIZE/2)
        radius = MAX_AOE_CIRCUMFERENCE_SIZE/2;
    int circum = radius*2;
    if (circum > MAX_AOE_CIRCUMFERENCE_SIZE)
        circum = MAX_AOE_CIRCUMFERENCE_SIZE;

    if (cachedAttackSprites[(int)circum][dither])
    {
        al_draw_tinted_bitmap(cachedAttackSprites[(int)circum][dither],color,cX-radius,cY-radius,0);
        return;
    }

    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    
    cachedAttackSprites[(int)circum][dither] = al_create_bitmap(circum,circum);
    al_set_target_bitmap(cachedAttackSprites[(int)circum][dither]);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_lock_bitmap(al_get_target_bitmap(),ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);


    if (dither == DITHER_NONE)
    {
        al_unlock_bitmap(al_get_target_bitmap());
        al_set_target_bitmap(before);
        return;
    }
    if (dither == DITHER_FILL)
    {
        al_draw_filled_circle(cX,cY,radius,color);
    }
    if (dither == DITHER_HALF || dither == DITHER_QUARTER || dither == DITHER_EIGTH)
    {

        al_lock_bitmap(al_get_target_bitmap(),ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);


        for (int x = 0; x < circum+dither; x++)
        {
            for (int y = 0; y < circum+dither; y++)
            {
                if (PointInCircle(x,y,radius,radius,radius))
                {
                    if (y%dither==0 && x%dither == 0)
                    {
                        al_put_pixel(x,y,WHITE);
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
        
        for (int x = 0; x < circum+pattern; x+=pattern)
        {
            for (int y = 0; y < circum+pattern; y++)
            {
                if (PointInCircle(x,y,radius,radius,radius))
                {
                    al_put_pixel(x,y,WHITE);
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
            
        for (int y = 0; y < circum+pattern; y+=pattern)
        {
            for (int x = 0; x < circum+pattern; x++)
            {
                if (PointInCircle(x,y,radius,radius,radius))
                {   
                    al_put_pixel(x,y,WHITE);
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

        for (int x = 0; x < circum+pattern; x+=pattern*2)
        {
            for (int y = 0; y < circum+pattern; y+=pattern*2)
            {
                if (PointInCircle(x,y,radius,radius,radius))
                    al_put_pixel(x,y,WHITE);
                if (PointInCircle(x-1,y+1,radius,radius,radius))
                    al_put_pixel(x-1,y+1,WHITE);
                if (PointInCircle(x+1,y+1,radius,radius,radius))
                    al_put_pixel(x+1,y+1,WHITE);
                if (PointInCircle(x,y+2,radius,radius,radius))
                    al_put_pixel(x,y+2,WHITE);
            }
        }
    }
    if (dither == DITHER_HEAL_HALF || dither == DITHER_HEAL_QUARTER || dither == DITHER_HEAL_EIGTH)
    {
        int pattern;
        if (dither == DITHER_HEAL_HALF)
            pattern=2;
        if (dither == DITHER_HEAL_QUARTER)
            pattern=4;
        if (dither == DITHER_HEAL_EIGTH)
            pattern=8;

        for (int x = 0; x < circum+pattern; x+=pattern*2)
        {
            for (int y = 0; y < circum+pattern; y+=pattern*2)
            {
                //middle
                if (PointInCircle(x,y,radius,radius,radius))
                    al_put_pixel(x,y,WHITE);
                
                //horizontal
                if (PointInCircle(x-1,y,radius,radius,radius))
                    al_put_pixel(x-1,y,WHITE);
                if (PointInCircle(x+1,y,radius,radius,radius))
                    al_put_pixel(x+1,y,WHITE);

                //vertical
                if (PointInCircle(x,y-1,radius,radius,radius))
                    al_put_pixel(x,y-1,WHITE);
                if (PointInCircle(x,y+1,radius,radius,radius))
                    al_put_pixel(x,y+1,WHITE);
            }
        }
    }
    if (dither == DITHER_POISON_HALF || dither == DITHER_POISON_QUARTER || dither == DITHER_POISON_EIGTH)
    {
        al_unlock_bitmap(al_get_target_bitmap());
        int pattern;
        if (dither == DITHER_POISON_HALF)
            pattern=2;
        if (dither == DITHER_POISON_QUARTER)
            pattern=4;
        if (dither == DITHER_POISON_EIGTH)
            pattern=8;

        float rad = 3;

        float midX = radius;
        float midY = radius;

        int index = 0;
        for (int x = 0; x < circum; x+=pattern*2)
        {
            index++;
            for (int y = 0; y < circum; y+=pattern*2)
            {
                float add = 0;
                if (index % 2 == 0)
                    add = rad;

                float distance = dist(x,y+add,midX,midY) + rad/2.0f;
                if (distance < radius-1.25f)
                {
                    al_draw_circle(x,y+add,rad,WHITE,0);
                }
            }
        }
    }
    if (dither == DITHER_SPEED_HALF || dither == DITHER_SPEED_QUARTER || dither == DITHER_SPEED_EIGTH)
    {
        int pattern;
        if (dither == DITHER_SPEED_HALF)
            pattern=3;
        if (dither == DITHER_SPEED_QUARTER)
            pattern=4;
        if (dither == DITHER_SPEED_EIGTH)
            pattern=5;

        int index = 0;
        for (int x = 0; x < circum+pattern; x+=pattern*2)
        {
            index++;
            for (int y = 0; y < circum+pattern; y+=pattern*2)
            {
                float add = 0;
                if (index % 2 == 0)
                    add = pattern;

                //tail
                if (PointInCircle(x,y+add,radius,radius,radius))
                    al_put_pixel(x,y+add,WHITE);
                if (PointInCircle(x+1,y-1+add,radius,radius,radius))
                    al_put_pixel(x+1,y-1+add,WHITE);
                if (PointInCircle(x+2,y-2+add,radius,radius,radius))
                    al_put_pixel(x+2,y-2+add,WHITE);
                    
                //head
                if (PointInCircle(x+3,y-3+add,radius,radius,radius))
                    al_put_pixel(x+3,y-3+add,WHITE);
                if (PointInCircle(x+4,y-4+add,radius,radius,radius))
                    al_put_pixel(x+4,y-4+add,WHITE);

                //head left
                if (PointInCircle(x+3,y-4+add,radius,radius,radius))
                    al_put_pixel(x+3,y-4+add,WHITE);
                if (PointInCircle(x+2,y-4+add,radius,radius,radius))
                    al_put_pixel(x+2,y-4+add,WHITE);

                //head right
                if (PointInCircle(x+4,y-3+add,radius,radius,radius))
                    al_put_pixel(x+4,y-3+add,WHITE);
                if (PointInCircle(x+4,y-2+add,radius,radius,radius))
                    al_put_pixel(x+4,y-2+add,WHITE);


            }
        }
    }
    if (dither == DITHER_DAMAGE_HALF || dither == DITHER_DAMAGE_QUARTER || dither == DITHER_DAMAGE_EIGTH)
    {
        int pattern;
        if (dither == DITHER_DAMAGE_HALF)
            pattern=3;
        if (dither == DITHER_DAMAGE_QUARTER)
            pattern=5;
        if (dither == DITHER_DAMAGE_EIGTH)
            pattern=7;

        int index = 0;
        for (int x = 0; x < circum+pattern; x+=pattern*2)
        {
            index++;
            for (int y = 0; y < circum+pattern; y+=pattern*2)
            {
                float add = 0;
                if (index % 2 == 0)
                    add = pattern;

                if (PointInCircle(x+2,y+add,radius,radius,radius))
                    al_put_pixel(x+2,y+add,WHITE);
                if (PointInCircle(x+1,y+1+add,radius,radius,radius))
                    al_put_pixel(x+1,y+1+add,WHITE);
                if (PointInCircle(x,y+2+add,radius,radius,radius))
                    al_put_pixel(x,y+2+add,WHITE);
                if (PointInCircle(x+3,y+1+add,radius,radius,radius))
                    al_put_pixel(x+3,y+1+add,WHITE);
                if (PointInCircle(x+4,y+2+add,radius,radius,radius))
                    al_put_pixel(x+4,y+2+add,WHITE);


                if (PointInCircle(x+2,y+2+add,radius,radius,radius))
                    al_put_pixel(x+2,y+2+add,WHITE);
                if (PointInCircle(x+2,y+3+add,radius,radius,radius))
                    al_put_pixel(x+2,y+3+add,WHITE);
                if (PointInCircle(x+2,y+4+add,radius,radius,radius))
                    al_put_pixel(x+2,y+4+add,WHITE);


            }
        }
    }
        if (dither == DITHER_ATTACK_DAMAGE_HALF || dither == DITHER_ATTACK_DAMAGE_QUARTER || dither == DITHER_ATTACK_DAMAGE_EIGTH)
    {
        int pattern;
        if (dither == DITHER_ATTACK_DAMAGE_HALF)
            pattern=3;
        if (dither == DITHER_ATTACK_DAMAGE_QUARTER)
            pattern=5;
        if (dither == DITHER_ATTACK_DAMAGE_EIGTH)
            pattern=7;

        int index = 0;
        for (int x = 0; x < circum+pattern; x+=pattern*2)
        {
            index++;
            for (int y = 0; y < circum+pattern; y+=pattern*2)
            {
                float add = 0;
                if (index % 2 == 0)
                    add = pattern;

                if (PointInCircle(x+1,y+add,radius,radius,radius))
                    al_put_pixel(x+1,y+add,WHITE);
                if (PointInCircle(x+1,y+1+add,radius,radius,radius))
                    al_put_pixel(x+1,y+1+add,WHITE);
                if (PointInCircle(x+1,y+2+add,radius,radius,radius))
                    al_put_pixel(x+1,y+2+add,WHITE);
                if (PointInCircle(x+1,y+3+add,radius,radius,radius))
                    al_put_pixel(x+1,y+3+add,WHITE);
                if (PointInCircle(x+1,y+4+add,radius,radius,radius))
                    al_put_pixel(x+1,y+4+add,WHITE);

                //hilt
                if (PointInCircle(x+1,y+5+add,radius,radius,radius))
                    al_put_pixel(x+1,y+5+add,WHITE);
                if (PointInCircle(x+1,y+6+add,radius,radius,radius))
                    al_put_pixel(x+1,y+6+add,WHITE);
                if (PointInCircle(x,y+5+add,radius,radius,radius))
                    al_put_pixel(x,y+5+add,WHITE);
                if (PointInCircle(x+2,y+5+add,radius,radius,radius))
                    al_put_pixel(x+2,y+5+add,WHITE);


            }
        }
    }
    al_unlock_bitmap(al_get_target_bitmap());

    al_set_target_bitmap(before);
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
        {
            al_draw_circle((a->screenX),(a->screenY),a->radius,GetColor(c,a->playerOwnedBy),1.5f);
        }
        draw_circle_dithered(a->screenX,a->screenY,a->radius,GetColor(c,a->playerOwnedBy),a->dither);

        if (AttackIsSoak(a))
        {
            float move = ((_FRAMES)%10)/4.0f;
            float x = a->screenX;
            float y = a->screenY - a->targetRadius - 10 - move;

            float endX = a->screenX;
            float endY = a->screenY - a->targetRadius - 5 - move;
            RotatePointF(&x,&y,a->screenX,a->screenY,DegToRad(45));
            RotatePointF(&endX,&endY,a->screenX,a->screenY,DegToRad(45));

            for (int i = 0; i < 4; i++)
            {
                RotatePointF(&x,&y,a->screenX,a->screenY,DegToRad(90));
                RotatePointF(&endX,&endY,a->screenX,a->screenY,DegToRad(90));
                
                DrawArrow((endX),(endY),(x),(y),GetColor(c,GetPlayerOwnedBy(a->ownedBy)));
            }
        }
    }
    else if (a->attackType == ATTACK_CONE)
    {
        float x2 = ToScreenSpace_X(a->targx); float y2 = ToScreenSpace_Y(a->targy);
        if (a->target)
        {
            GetCentre(a->target,&x2,&y2);
            Normalize(&x2,&y2);
            x2 *= a->screenX + a->range;
            y2 *= a->screenY + a->range;
            //al_put_pixel(x2,y2,POISON);
        }
        float angle = RadToDeg(atan2(y2-a->y,x2-a->x));
        DrawCone((a->x),(a->y),angle,a->radius,a->range,FRIENDLY);
    }
    else
    {
        al_draw_filled_circle((a->screenX),(a->screenY),a->radius,GetColor(a->color,a->playerOwnedBy));
        al_draw_filled_circle((a->screenX+2),(a->screenY+2),a->radius,BG);

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


        for (int i = 0; i < numActiveObjects; i++)
        {
            if (!IsActive(activeObjects[i]))
                continue;
           // int abilityOwnedBy = GetPlayerOwnedBy(a->ownedBy);

            //if (&objects[i] == &objects[abilityOwnedBy] && (AttackIsProjectile(a)))
              //  continue;


            if (a->properties & ATTACK_HITS_ENEMIES)
            {
                int abilityOwnedBy = a->playerOwnedBy;
                int objOwnedBy = GetPlayerOwnedBy(activeObjects[i]);

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
                int abilityOwnedBy = a->playerOwnedBy;
                int objOwnedBy = GetPlayerOwnedBy(activeObjects[i]);
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
            
            Rect r = GetObjRect(activeObjects[i]);
            
            if (a->attackType == ATTACK_AOE || a->attackType == ATTACK_PROJECTILE_TARGETED || a->attackType ==   ATTACK_PROJECTILE_POINT ||a->attackType == ATTACK_PROJECTILE_ANGLE)
            {
                if (CircleInRect(a->x,a->y,a->targetRadius,r))
                {   
                    if (isSoak)
                        ApplyAttack(copied,activeObjects[i]);
                    else
                        ApplyAttack(a,activeObjects[i]);

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
                GameObject* g = activeObjects[i];
                float x = a->x; float y = a->y; 
                float x2 = a->targx; float y2 = a->targy;
                if (a->target)
                {
                    x2 = a->target->position.worldX;
                    y2 = a->target->position.worldY;
                }

                float angle;
                angle = atan2(y2 - y, x2 - x);
                float length = a->range;//dist(x,y,x2,y2);
                
                Rect r = GetObjRect(activeObjects[i]);
                if (RectInCone(r,x,y,angle,a->targetRadius,length))
                {
                    ApplyAttack(a,activeObjects[i]);
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
    //al_lock_bitmap(al_get_target_bitmap(),ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        Attack* a = &attacks[i];
        if (a->properties & ATTACK_ACTIVE)
        {
            DrawAttack(a,dt);
        }
    }   
    //al_unlock_bitmap(al_get_target_bitmap());

}
bool AttackIsActive(Attack* a)
{
    return (a->properties & ATTACK_ACTIVE);
}
void UpdateScreenPositionsAttack(Attack* a)
{
    a->screenX = ToScreenSpace_X(a->x);
    a->screenY = ToScreenSpace_Y(a->y);
}
int GetNumActiveAttacks()
{
    return numActiveAttacks;
}