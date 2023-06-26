#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include "luafuncs.h"
#include "gameobject.h"
#include "stdbool.h"

Timer* AddTimer(Timer t)
{
    if (!timers)
    {
        timers = calloc(1,sizeof(Timer*));
        numTimers = 0;
        timerMax = 1;
    }

    if (numTimers >= timerMax)
    {
        timerMax += 32;
        timers = realloc(timers,timerMax*sizeof(Timer*));
    }

    Timer* tNew = calloc(1,sizeof(Timer));
    *tNew = t;

    timers[numTimers] = tNew;

    numTimers++;

    return tNew;
}
void DeleteTimer(int index)
{
    Timer* t = timers[index];
    luaL_unref(luaState,LUA_REGISTRYINDEX,t->registryIndex);
    free(timers[index]);
    for (int i = index; i < numTimers-1; i++)
    {
        timers[i] = timers[i+1];
    }

    numTimers--;
}
bool UpdateTimer(int index, float dt)
{
    Timer* t = timers[index];
    t->timer -= dt;
    if (!IsActive(t->obj) && !t->persistsAfterDeath)
    {
        DeleteTimer(index); 
        return true;
    }
    if (t->timer <= 0)
    {
        currGameObjRunning = t->obj;
        currAbilityRunning = t->ability;
        currAttackRunning = t->attack;

        if (currAbilityRunning && currAbilityRunning->luafunc_timerbeforetick)
        {
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,currAbilityRunning->luafunc_timerbeforetick);
            lua_pushnumber(luaState,(size_t)t);
            lua_pushnumber(luaState,(size_t)t->func);
            lua_pcall(luaState,2,0,0);
        }

        void (*func)(lua_State*) = (void*)(t->func);
        lua_settop(luaState,0);
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,t->registryIndex);
        
        int len = lua_rawlen(luaState,1);
        for (int i = 1; i <= len; i++)
        {
            lua_pushnumber(luaState,i);
            lua_gettable(luaState,1);
        }
        lua_remove(luaState,1);
        func(luaState);
        DeleteTimer(index);
        return true;
    }
    return false;
}
void UpdateTimers(float dt)
{
    for (int i = 0; i < numTimers; i++)
    {
        //returns true on deletion
        if (UpdateTimer(i,dt))
            i--;
    }
}
void UpdateTimerArgument(Timer* t, int argumentPosition, float* f, const char* str, bool* b)
{
    lua_settop(luaState,0);
    lua_rawgeti(luaState,LUA_REGISTRYINDEX,t->registryIndex);
    lua_pushnumber(luaState,argumentPosition);
    if (f)
    {
        lua_pushnumber(luaState,*f);
    }
    if (str)
    {
        lua_pushstring(luaState,str);
    }
    if (b)
    {
        lua_pushboolean(luaState,*b);
    }
    lua_settable(luaState,-3);

    luaL_unref(luaState,LUA_REGISTRYINDEX,t->registryIndex);
    t->registryIndex = luaL_ref(luaState,LUA_REGISTRYINDEX);
}   
