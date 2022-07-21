#include <math.h>

#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include "ability.h"
#include "helperfuncs.h"
#include "luafuncs.h"
#include "gameobject.h"
#include "player.h"
#include "video.h"
#include "ui.h"
#include "colors.h"
#include "sound.h"

void InitAbilities()
{
    ability_UI_click_sound = LoadSound("assets/audio/ability_down.wav");
    ability_UI_click_up_sound = LoadSound("assets/audio/ability_up.wav");
    
}
void DrawHeldAbility(ALLEGRO_MOUSE_STATE* mouseState)
{
    float cx; float cy;
    GameObject* g = players[0].selection[players[0].indexSelectedUnit];
    GetCentre(g, &cx, &cy);
    int w; int h;
    float radius = players[0].abilityHeld->range;

    al_draw_circle(cx,cy,radius,FRIENDLY,0);
    
    if (players[0].abilityHeld->targetingHint == HINT_LINE)
    {
        //normalise then project to min(radius,mousepos)
        float distX = mouseState->x - cx; 
        float distY = mouseState->y - cy;
        float dist = sqrt(distX*distX+distY*distY);
        
        float x = mouseState->x - cx;
        float y = mouseState->y - cy;
        Normalize(&x,&y); 
        x = cx + x*_MIN(radius,dist);
        y = cy + y*_MIN(radius,dist);

        al_draw_line(cx,cy,x,y,FRIENDLY,1);
    }
    GameObject* heldSelected = players[0].selection[players[0].indexSelectedUnit];
    float cxHeld; float cyHeld; 
    GetCentre(heldSelected,&cxHeld,&cyHeld);
    al_draw_line(mouseState->x+2,mouseState->y+2,cxHeld,cyHeld,FRIENDLY,1);

}
void UpdateAbilityInteractions(ALLEGRO_KEYBOARD_STATE* keyState,ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    CheckAbilityClicked(keyState,keyStateLastFrame, mouseState);
    if (mouseState->buttons & 1) 
    {
        currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
        currAbilityRunning = players[0].abilityHeld;
        if (IsInsideUI(mouseState->x,mouseState->y))
        {
            GetAbilityClickedInsideUI(mouseState,mouseStateLastFrame);
        }
        else
        {
            ProcessAttackMoveMouseCommand(mouseState, keyState);
            CastAbilityOnMouse(mouseState, keyState);
        }
        currAbilityRunning = NULL;
        players[0].abilityHeld = NULL;
    }

    if (!(mouseState->buttons & 2) && (mouseStateLastFrame->buttons & 2))
    {
        currAbilityRunning = NULL;
        players[0].abilityHeld = NULL;

    }
    if (players[0].abilityHeld)
    {
        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
    }
    else
    {
        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW);

    }
}
void CastAbilityOnMouse(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (players[0].abilityHeld)
    {
        GameObject* target = NULL;
        for (int i = 0; i < MAX_OBJS; i++)
        {
            if (objects[i].properties & OBJ_ACTIVE)
            {
                if (PointInRect(mouseState->x,mouseState->y,GetObjRect(&objects[i])))
                {
                    target = &objects[i];
                    break;
                }
            }
        }
        float midX=0; float midY=0;
        GetOffsetCenter(currGameObjRunning,&midX,&midY);
    
        if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
            ClearCommandQueue(currGameObjRunning);
        if (target)
        {
            if (!ObjIsInvincible(target))
            {
                CastCommand(currGameObjRunning,target,currAbilityRunning,mouseState->x,mouseState->y);
            }
            else
                MoveCommand(currGameObjRunning,mouseState->x,mouseState->y);
        }
        else
            CastCommand(currGameObjRunning,target,currAbilityRunning,mouseState->x,mouseState->y);
        //CastAbility(currGameObjRunning,currAbilityRunning,mouseState->x,mouseState->y,mouseState->x-midX,mouseState->y-midY,target);
        players[0].clickedThisFrame = target;

    }

}
int GetAbilityIndexClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    if (!al_key_down(keyState,ALLEGRO_KEY_Q) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_Q))
    {
        return 0;
    }
    if (!al_key_down(keyState,ALLEGRO_KEY_W) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_W))
    {
        return 1;
    }
    if (!al_key_down(keyState,ALLEGRO_KEY_E) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_E))
    {
        return 2;
    }
    if (!al_key_down(keyState,ALLEGRO_KEY_R) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_R))
    {
        return 3;
    }
    return -1;
}
Ability CloneAbilityPrefab(Ability* prefab, lua_State* l)
{
    Ability a = {0};
    a.path = prefab->path;
    a.luabuffer = prefab->luabuffer;
    if (a.path && a.luabuffer)
    {
        LoadAbility(a.path, l, &a);
    }
    return a;
}

Ability* AddAbility(const char* path)
{
    if (!abilities)
    {
        abilities = calloc(32,sizeof(Ability));
        numAbilities = 0;
        numAbilitiesAllocated = 32;
    }

    for (int i = 0; i < numAbilities; i++)
    {
        if (abilities[i].path)
        {
            const char* c = path;
            const char* c2 = abilities[i].path;
            if (strcasecmp(path,abilities[i].path) == 0)
            {
                return &abilities[i];
            }
        }
    }
    if (numAbilities + 1 >= numAbilitiesAllocated)
    {
        numAbilitiesAllocated += BUFFER_PREALLOC_AMT;
        abilities = realloc(abilities,numAbilitiesAllocated*sizeof(Ability));
    }
    abilities[numAbilities].hintRadius = 1;
    LoadAbility(path,luaState,&abilities[numAbilities]);
    numAbilities++;

    return &abilities[numAbilities-1];
}
void LoadAbility(const char* path, lua_State* l, Ability* a)
{
    if (a == NULL) 
        return;

    if (!a->luabuffer)
    {
        memset(a,0,sizeof(Ability));
        char* file = readFile(path);
        a->luabuffer = file;

        a->path = calloc(strlen(path)+1,sizeof(char));
        memcpy(a->path,path,strlen(path)*sizeof(char));
    }
    if (luaL_loadbuffer(l, a->luabuffer,strlen(a->luabuffer),NULL) || lua_pcall(l, 0, 0, 0))
    {
        printf("Can't load lua file %s",lua_tostring(l,-1));
    }
    else
    {
        Ability* before = currAbilityRunning;
        a->cooldown = 1;
        int funcIndex;
        if (CheckFuncExists("setup",a->luabuffer))
        {
            lua_getglobal(l, "setup");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_setup = funcIndex;
            currAbilityRunning = a;
            CallLuaFunc(funcIndex);

        }
        else
            a->luafunc_setup = -1;

        if (CheckFuncExists("casted",a->luabuffer))
        {
            lua_getglobal(l, "casted");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_casted = funcIndex;
        }
        else
            a->luafunc_casted = -1;
        if (CheckFuncExists("onhit",a->luabuffer))
        {
            lua_getglobal(l, "onhit");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_onhit = funcIndex;

        }
        else
            a->luafunc_onhit = -1;

        if (CheckFuncExists("ontimeout",a->luabuffer))
        {
            lua_getglobal(l, "ontimeout");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_ontimeout = funcIndex;

        }
        else
            a->luafunc_ontimeout = -1;

        if (CheckFuncExists("abilitytick",a->luabuffer))
        {
            lua_getglobal(l, "abilitytick");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_tick = funcIndex;

        }
        else
            a->luafunc_tick = -1;

        if (CheckFuncExists("untoggle",a->luabuffer))
        {
            lua_getglobal(l, "untoggle");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_untoggle = funcIndex;
        }
        else
            a->luafunc_untoggle = -1;

        currAbilityRunning = before;

     }
}
bool AbilityShouldBeCastOnTarget(Ability* a)
{
    return ((a->castType & ABILITY_TARGET_FRIENDLY) || (a->castType & ABILITY_TARGET_ENEMY) || (a->castType & ABILITY_TARGET_ALL));
}
void CastAbility(GameObject* g, Ability* a, float x, float y, float headingx, float headingy, GameObject* target)
{
    if (a->cdTimer > 0)
        return;
    if (g)
    {
        if (!ObjectHasManaToCast(g,a))
        {
            return;
        }
    }
    if (target && (a->castType & ABILITY_TARGET_ALL || a->castType & ABILITY_TARGET_ENEMY || a->castType & ABILITY_TARGET_FRIENDLY))
    {
        bool ownedByG = IsOwnedByPlayer(g);
        bool ownedByTarget = IsOwnedByPlayer(target);

        if (a->castType & ABILITY_TARGET_ENEMY)
        {
            if (ownedByG == ownedByTarget)
            {
                return;
            }
        }
        if (a->castType & ABILITY_TARGET_FRIENDLY)
        {
            if (ownedByG != ownedByTarget)
            {
                return;
            }
        }

        if (GetDist(g,target) > a->range)
            return;
    }
    if (target == NULL && AbilityShouldBeCastOnTarget(a))
        return;

    if (target != g)
    {
    }
    currAbilityRunning = a; 
    currGameObjRunning = g; 

    //This is so that, if we copy an object, it can't copy again by casting the same ability
    float cooldownBefore = a->cdTimer;
    a->cdTimer = a->cooldown;

    if ((a->castType != ABILITY_TOGGLE) || (a->castType == ABILITY_TOGGLE && !a->toggled))
    {
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, a->luafunc_casted);
        
        float cx; float cy;
        GetCentre(g, &cx,&cy);
        ClampToRadius(&x,&y,cx,cy,a->range);

        lua_pushinteger(luaState,x);
        lua_pushinteger(luaState,y);    
        lua_pushinteger(luaState,(int)(target-objects));    
        lua_pushnumber(luaState,headingx);
        lua_pushnumber(luaState,headingy);    

        lua_pcall(luaState,5,1,0);
        bool b = lua_toboolean(luaState,-1);
        if (b)
        {
            a->cdTimer = a->cooldown;
        }
        else
        {
            a->cdTimer = cooldownBefore;
        }
    }
    else if (a->castType == ABILITY_TOGGLE && a->toggled)
    {
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, a->luafunc_untoggle);

        lua_pushinteger(luaState,x);
        lua_pushinteger(luaState,y);    
        lua_pushinteger(luaState,(int)(target-objects));    
        lua_pushnumber(luaState,headingx);
        lua_pushnumber(luaState,headingy);    


        lua_pcall(luaState,5,1,0);
        bool b = lua_toboolean(luaState,-1);
        if (b)
        {
            a->cdTimer = a->cooldown;
        }
        else
        {
            a->cdTimer = cooldownBefore;
        }

    }


    if (a->castType == ABILITY_TOGGLE)
    {
        a->toggled = !a->toggled;
    }
    AddMana(g,-GetManaCost(a));

}
//TODO: things that affect mana cost change this value
float GetManaCost(Ability* a)
{
    return a->manaCost;
}
bool AbilityIsInitialised(Ability* a)
{
    return (a->path != 0);
}

void ToggleAbility(Ability* a, GameObject* ownedBy, bool toggled)
{
    if (toggled)
    {
        float x; float y; 
        GetCentre(ownedBy,&x,&y);
        CastAbility(ownedBy,a,x,y,x,y,NULL);
        return;
    }
    a->toggled = toggled;

    if (!toggled)
    {
        if (a->luafunc_untoggle)
        {
            lua_rawgeti(luaState, LUA_REGISTRYINDEX, a->luafunc_untoggle);
            float x; float y; 
            GetCentre(ownedBy,&x,&y);
            lua_pushinteger(luaState,x);
            lua_pushinteger(luaState,y);    
            lua_pushinteger(luaState,(int)(ownedBy-objects));    
            lua_pushnumber(luaState,x);
            lua_pushnumber(luaState,y);    


            lua_pcall(luaState,5,1,0);
            bool b = lua_toboolean(luaState,-1);
            if (b)
            {
                a->cdTimer = a->cooldown;
            }
        }
    }
}
bool AbilityCanBeCast(Ability* a, GameObject* g, GameObject* target, float x, float y)
{
    if (a->cdTimer > 0)
        return false;
    if (a->castType == ABILITY_INSTANT || a->castType == ABILITY_TOGGLE)
    {
        return true;
    }
    if (a->castType == ABILITY_ANGLE)
    {
        return true;        
    }
    if (a->castType == ABILITY_POINT)
    {
        if (dist(g->position.x,g->position.y,x,y) <= a->range)
            return true;
        else
            return false;
    }
    if (GetDist(g,target) > a->range)
        return false;
    if (target == NULL && AbilityShouldBeCastOnTarget(a))
        return false;

    if (target && (a->castType & ABILITY_TARGET_ALL || a->castType & ABILITY_TARGET_ENEMY || a->castType & ABILITY_TARGET_FRIENDLY))
    {
        bool ownedByG = IsOwnedByPlayer(g);
        bool ownedByTarget = IsOwnedByPlayer(target);

        if (a->castType & ABILITY_TARGET_ENEMY)
        {
            if (ownedByG == ownedByTarget)
            {
                return false;
            }
        }
        if (a->castType & ABILITY_TARGET_FRIENDLY)
        {
            if (ownedByG != ownedByTarget)
            {
                return false;   
            }
        }
    }
    return true;
}
void SetManaCost(Ability* a, float mana)
{
    a->manaCost = mana;
}
bool AbilityIsOnCooldown(Ability* a)
{
    return a->cdTimer > 0;
}
bool ObjectHasManaToCast(GameObject* g, Ability* a)
{
    return (g->mana >= GetManaCost(a));
}
bool AbilityIsCastImmediately(Ability* a)
{
    if (!a) return false;
    return !!((a->castType & ABILITY_TOGGLE) | (a->castType & ABILITY_INSTANT));
}