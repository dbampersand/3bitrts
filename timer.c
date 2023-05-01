#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include "luafuncs.h"
void AddTimer(Timer t)
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
void UpdateTimer(int index, float dt)
{
    Timer* t = timers[index];
    t->timer -= dt;
    if (t->timer <= 0)
    {
        currGameObjRunning = t->obj;
        currAbilityRunning = t->ability;
        currAttackRunning = t->attack;

        void (*func)(lua_State*) = (void*)(t->func);
        lua_settop(luaState,0);
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,t->registryIndex);
        
        int len = lua_rawlen(luaState,1);
        for (int i = 1; i <= len; i++)
        {
            lua_pushnumber(luaState,i);
            lua_gettable(luaState,1);
        }
        lua_remove (luaState,1);
        func(luaState);
        DeleteTimer(index);
        return;
    }
}
void UpdateTimers(float dt)
{
    for (int i = 0; i < numTimers; i++)
    {
        UpdateTimer(i,dt);
    }
}