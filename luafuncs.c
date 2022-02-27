#include "luafuncs.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "map.h"
void CallLuaFunc(int funcID)
{
    lua_rawgeti(luaState,LUA_REGISTRYINDEX,funcID);
    lua_pcall(luaState,0,0,0);

}
void init_lua()
{
    luaState = luaL_newstate();
    luaL_openlibs(luaState);

    SetLuaFuncs();

}
void LoadLuaFile(const char* filename, GameObject* g)
{
    
    if (!g->lua_buffer)
    {
        g->lua_buffer = readFile(filename);
     }
}
int L_Print(lua_State* l)
{
    if (currGameObjRunning)
    {
        if (currGameObjRunning->name)
        {
             printf("%s: %s\n",currGameObjRunning->name,lua_tostring(l,-1));
        }
        else
        {
             printf("%s\n",lua_tostring(l,-1));
        }
    }
    else
    {
            printf("%s\n",lua_tostring(l,-1));
    }
    fflush(stdout);
    return 1;
}
int L_CreateObject(lua_State* l)
{
    GameObject* g;
    const char* l_path = lua_tostring(l,1);
    const int x = lua_tonumber(l,2);
    const int y = lua_tonumber(l,3);
    int PLAYER = lua_tonumber(l,4);
    if (!lua_isnumber(l,PLAYER)) 
        PLAYER = 1;


    bool prefabFound = false;
    /*for (int i = 0; i < numPrefabs; i++)
    {
        if (strcmp(prefabs[i].path,l_path) == 0)
        {
            g = &prefabs[i];
            prefabFound = true;
            break;
        }
    } */
    if (!prefabFound)
    {
        GameObject* prefab = LoadPrefab(l_path);
        g = AddGameobject(prefab);
        g->x = x;
        g->y = y;
        g->xtarg = x;
        g->ytarg = y;
        g->speed = 5;
        g->health = 100;
        g->maxHP = 100;
        SetOwnedBy(g, PLAYER);

    }
    else
    {
        g = AddGameobject(g);
        g->x = x;
        g->y = y;
        g->xtarg = x;
        g->ytarg = y;
        g->speed = 5;
        g->health = 100;
        g->maxHP = 100;
        SetOwnedBy(g, PLAYER);
    }

    return 1;
}
int L_SetSprite(lua_State* l)
{
    currGameObjRunning->spriteIndex = LoadSprite(lua_tostring(l,-1),true);
    return 1;
}
int L_SetMapSprite(lua_State* l)
{
    currMap->spriteIndex = LoadSprite(lua_tostring(l,-1),false);
    return 1;
}
void SetGlobals(lua_State* l)
{
    lua_pushinteger(l,0);
    lua_setglobal(l,"PLAYER");

    lua_pushinteger(l,1);
    lua_setglobal(l,"ENEMY");
}
void SetLuaFuncs()
{
    SetGlobals(luaState);

    lua_pushcfunction(luaState, L_Print);
    lua_setglobal(luaState, "Print");

    lua_pushcfunction(luaState, L_CreateObject);
    lua_setglobal(luaState, "CreateObject");

    lua_pushcfunction(luaState, L_SetSprite);
    lua_setglobal(luaState, "SetSprite");

    lua_pushcfunction(luaState, L_SetMapSprite);
    lua_setglobal(luaState, "SetMapSprite");


}