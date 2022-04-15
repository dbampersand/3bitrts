#include "ability.h"
#include "helperfuncs.h"
#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "luafuncs.h"
#include "gameobject.h"
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
            if (strcmp(path,abilities[i].path) == 0)
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
    }


    if (a->castType == ABILITY_TOGGLE)
    {
        a->toggled = !a->toggled;
    }

}
bool AbilityIsInitialised(Ability* a)
{
    return (a->path != 0);
}
//void CopyAbility(Ability* prefab)

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
        if (dist(g->x,g->y,x,y) <= a->range)
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
bool AbilityIsCastImmediately(Ability* a)
{
    if (!a) return false;
    return !!((a->castType & ABILITY_TOGGLE) | (a->castType & ABILITY_INSTANT));
}