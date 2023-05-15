#pragma once
#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "lua-5.4.3/src/luaconf.h"
#include <stdbool.h>

typedef struct GameObject GameObject;
typedef struct Ability Ability;
typedef struct Attack Attack;

typedef struct Timer 
{
    lua_CFunction func;
    int registryIndex;
    float timer;

    GameObject* obj;
    Ability* ability;
    Attack* attack;


} Timer;

Timer** timers;
int numTimers;
int timerMax;


Timer* AddTimer(Timer t);
void DeleteTimer(int index);
void UpdateTimers(float dt);
void UpdateTimerArgument(Timer* t, int argumentPosition, float* f, const char* str, bool* b);
