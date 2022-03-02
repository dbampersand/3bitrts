#include "ability.h"
#include "helperfuncs.h"
#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "luafuncs.h"
#include "gameobject.h"
Ability CloneAbilityPrefab(Ability* prefab, lua_State* l)
{
    Ability a;
    a.path = prefab->path;
    a.luabuffer = prefab->luabuffer;
    if (a.path && a.luabuffer)
    {
        LoadAbility(a.path,l,&a);
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
        if (strcmp(path,abilities[i].path)==0)
        {
            return &abilities[i];
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

        a->path = calloc(strlen(path),sizeof(char));
        memcpy(a->path,path,strlen(path)*sizeof(char));
    }
     if (luaL_loadbuffer(l, a->luabuffer,strlen(a->luabuffer),NULL) || lua_pcall(l, 0, 0, 0))
     {
         printf("Can't load lua file %s",lua_tostring(l,-1));
     }
    else
     {
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
            lua_rawgeti(l,LUA_REGISTRYINDEX,funcIndex);
            lua_pcall(l,0,0,0);
            a->luafunc_casted = funcIndex;
        }
        else
            a->luafunc_casted = -1;


     }
}

//void CopyAbility(Ability* prefab)