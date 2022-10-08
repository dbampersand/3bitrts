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
#include "settings.h"
#include "hashtable.h"
void InitAbilities()
{
    ability_UI_click_sound = LoadSound("assets/audio/ability_down.wav");
    ability_UI_click_up_sound = LoadSound("assets/audio/ability_up.wav");
    
    abilityPathHashTable = InitHashTable();
}
void DrawHeldAbility(MouseState* mouseState)
{
    float cx; float cy;
    GameObject* g = players[0].selection[players[0].indexSelectedUnit];
    GetCentre_Screen(g, &cx, &cy);
    int w; int h;
    float radius = players[0].abilityHeld->range;

    al_draw_circle((cx),(cy),radius,FRIENDLY,0);
    
    if (players[0].abilityHeld->targetingHint == HINT_LINE)
    {
        //normalise then project to min(radius,mousepos)
        float distX = mouseState->screenX - cx; 
        float distY = mouseState->screenY - cy;
        float dist = sqrt(distX*distX+distY*distY);
        
        float x = mouseState->worldX - cx;
        float y = mouseState->worldY - cy;
        Normalize(&x,&y); 
        x = cx + x*_MIN(radius,dist);
        y = cy + y*_MIN(radius,dist);
        ToScreenSpace(&x,&y);
        al_draw_line(ToScreenSpace_X(cx),ToScreenSpace_Y(cy),x,y,FRIENDLY,1);
    }
    GameObject* heldSelected = players[0].selection[players[0].indexSelectedUnit];
    float cxHeld; float cyHeld; 
    GetCentre(heldSelected,&cxHeld,&cyHeld);
    al_draw_line(mouseState->screenX+2,(mouseState->screenY+2),ToScreenSpace_X(cxHeld),ToScreenSpace_Y(cyHeld),FRIENDLY,1);

}
void UpdateAbilityInteractions(ALLEGRO_KEYBOARD_STATE* keyState,ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState mouseState,MouseState mouseStateLastFrame)
{
    if (transitioningTo != gameState)
        return;
    CheckAbilityClicked(keyState,keyStateLastFrame, &mouseState);
    if (mouseState.mouse.buttons & 1) 
    {
        currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
        currAbilityRunning = players[0].abilityHeld;
        if (IsInsideUI(mouseState.worldX,mouseState.worldY))
        {
            GetAbilityClickedInsideUI(mouseState,mouseStateLastFrame);
        }
        else
        {
            ProcessAttackMoveMouseCommand(&mouseState, keyState);
            CastAbilityOnMouse(&mouseState, keyState);
            players[0].abilityHeld = NULL;

        }
        currAbilityRunning = NULL;
    }

    if (!(mouseState.mouse.buttons & 2) && (mouseStateLastFrame.mouse.buttons & 2))
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
void CastAbilityOnMouse(MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (players[0].abilityHeld)
    {
        GameObject* target = NULL;  
        for (int i = 0; i < MAX_OBJS; i++)
        {
            if (IsActive(&objects[i]))
            {
                if (PointInRect(mouseState->worldX,mouseState->worldY,GetObjRect(&objects[i])))
                {
                    target = &objects[i];
                    break;
                }
            }
        }
            float midX=0; float midY=0;
        GetOffsetCenter(currGameObjRunning,&midX,&midY);
    
        //if (!IsBindDown(keyState,currSettings.keymap.key_Shift))
          //  ClearCommandQueue(currGameObjRunning);
        if (target)
        {
            if (!ObjIsInvincible(target))
            {
                CastCommand(currGameObjRunning,target,currAbilityRunning,mouseState->worldX,mouseState->worldY,IsBindDown(keyState,currSettings.keymap.key_Shift));
            }
            else
                MoveCommand(currGameObjRunning,mouseState->worldX,mouseState->worldY, IsBindDown(keyState,currSettings.keymap.key_Shift));
        }
        else
            CastCommand(currGameObjRunning,target,currAbilityRunning,mouseState->worldX,mouseState->worldY,IsBindDown(keyState,currSettings.keymap.key_Shift));
        //CastAbility(currGameObjRunning,currAbilityRunning,mouseState->x,mouseState->y,mouseState->x-midX,mouseState->y-midY,target);
        players[0].clickedThisFrame = target;

        //Attack/move command with all other objs
        if (GetPlayerOwnedBy(target) == TYPE_ENEMY)
            for (int i = 0; i < MAXUNITSSELECTED; i++)
            {
                GameObject* g = players[0].selection[i];
                if (target)
                    AttackCommand(g,target,IsBindDown(keyState,currSettings.keymap.key_Shift));
                else
                    MoveCommand(g,mouseState->worldX,mouseState->worldY,IsBindDown(keyState,currSettings.keymap.key_Shift));
            }

    }

}
int GetAbilityIndexClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{

    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_Q))
    {
        return 0;
    }
    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_W))
    {
        return 1;
    }
    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_E))
    {
        return 2;
    }
    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_R))
    {
        return 3;
    }
    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_F))
    {
        return 4;
    }

    return -1;
}
void CloneAbilityPrefab(Ability* prefab, lua_State* l, Ability* cloneTo)
{
    Ability* a = cloneTo;
    a->path = prefab->path;
    a->luabuffer = prefab->luabuffer;
    if (a->path && a->luabuffer)
    {
        LoadAbility(a->path, l, a);
    }
}

Ability* AddAbility(const char* path)
{
    if (!abilities)
    {
        abilities = calloc(32,sizeof(Ability));
        numAbilities = 0;
        numAbilitiesAllocated = 32;
    }

    HashItem* i = SearchHashTable(abilityPathHashTable,path);
    if (i->data)
    {
        int index = *((int*)i->data);
        Ability* a = &abilities[index];
    }
    /*
    for (int i = 0; i < numAbilities; i++)
    {
        if (abilities[i].path)
        {
            const char* c = path;
            const char* c2 = abilities[i].path;
            HashItem* i = SearchHashTable(abilityPathHashTable,path);
            Ability* a = (Ability*)data;
            if (a)
                return a;
            //if (strcasecmp(path,abilities[i].path) == 0)
            //{
            //    return &abilities[i];
            //}
        }
    }*/
    if (numAbilities + 1 >= numAbilitiesAllocated)
    {
        numAbilitiesAllocated += BUFFER_PREALLOC_AMT;
        abilities = realloc(abilities,numAbilitiesAllocated*sizeof(Ability));
    }
    memset(&abilities[numAbilities],0,sizeof(Ability));

    abilities[numAbilities].hintRadius = 1;

    LoadAbility(path,luaState,&abilities[numAbilities]);
    i->data = calloc(1,sizeof(int));
    *(int*)(i->data) = numAbilities;

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
        currAbilityRunning = a;
        a->castType = ABILITY_NONE;
        a->cooldown = 1;
        a->stacks = 1;
        a->maxStacks = 1;
        a->manaCost = 0;
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
            a->luafunc_abilitytick = funcIndex;

        }
        else
            a->luafunc_abilitytick = -1;

        if (CheckFuncExists("untoggle",a->luabuffer))
        {
            lua_getglobal(l, "untoggle");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_untoggle = funcIndex;
        }
        else
            a->luafunc_untoggle = -1;

        if (CheckFuncExists("onchanneled",a->luabuffer))
        {
            lua_getglobal(l, "onchanneled");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            a->luafunc_onchanneled = funcIndex;
        }
        else
            a->luafunc_onchanneled = -1;



        currAbilityRunning = before;

     }
}
bool AbilityShouldBeCastOnTarget(Ability* a)
{
    return ((a->castType & ABILITY_TARGET_FRIENDLY) | (a->castType & ABILITY_TARGET_ENEMY) | (a->castType & ABILITY_TARGET_ALL));
}
bool AbilityCanBeCastOnGround(Ability* a)
{
    return (a->castType & (ABILITY_POINT|ABILITY_ANGLE));
}
//returns if the ability has been cast or not
bool CastAbility(GameObject* g, Ability* a, float x, float y, float headingx, float headingy, GameObject* target)
{
    if (g->stunTimer > 0)
        return false;
    if (AbilityIsOnCooldown(a))//->cdTimer > 0)
        return false;
    if (g)
    {
        if (!ObjectHasManaToCast(g,a))
        {
            return false;
        }
    }
    if (target && (a->castType & ABILITY_TARGET_ALL || a->castType & ABILITY_TARGET_ENEMY || a->castType & ABILITY_TARGET_FRIENDLY) || a->castType & ABILITY_NONE)
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

        if (GetDist(g,target) > a->range)
            return false;
    }
    if (g && (!(a->castType & ABILITY_INSTANT) &&  !(a->castType & ABILITY_TOGGLE)))
    {
        float tx = x; float ty = y;
        if (target)
        {
            GetCentre(target,&tx,&ty);
        }
        float cx; float cy;
        GetCentre(g,&cx,&cy); 
        float distance = dist(cx,cy,tx,ty);
        if (target)
            distance = RectDist(g,target);
        if (distance > a->range)
            return false;

    }
    if (target == NULL && AbilityShouldBeCastOnTarget(a) && !AbilityCanBeCastOnGround(a))
        return false;

    if (target != g)
    {
    }
    
    cast:

    currAbilityRunning = a; 
    currGameObjRunning = g; 

    //This is so that, if we copy an object, it can't copy again by casting the same ability
    float cooldownBefore = a->cdTimer;
    a->cdTimer = a->cooldown;

    if ((a->castType != ABILITY_TOGGLE) || (a->castType == ABILITY_TOGGLE && !a->toggled) || AbilityCanBeCastOnGround(a))
    {
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, a->luafunc_casted);
        
        float cx; float cy;
        GetCentre(g, &cx,&cy);
        if (a->castType != ABILITY_INSTANT)
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
            a->stacks--;
            if (a->stacks < 0)
                a->stacks = 0;
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
    return true;

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
    if (g->stunTimer > 0)
        return false;
    if (a->stacks <= 0)
        return false;
    if (a->castType & ABILITY_INSTANT || a->castType & ABILITY_TOGGLE)
    {
        return true;
    }
    if (a->castType & ABILITY_ANGLE)
    {
        return true;        
    }
    if (a->castType & ABILITY_POINT)
    {
        if (dist(g->position.worldX,g->position.worldY,x,y) <= a->range)
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
    if (a)
        return a->stacks <= 0;//a->cdTimer > 0;
    else  return true;
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
void ModifyAbilityCooldownTimer(Ability* a, float value)
{
    a->cdTimer += value;
    if (a->cdTimer < 0)
        a->cdTimer = 0;
    if (a->cdTimer > a->cooldown)
        a->cdTimer = a->cooldown;
    
}