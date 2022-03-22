#pragma once
#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "lua-5.4.3/src/luaconf.h"
#include "stdbool.h"


typedef struct GameObject GameObject;
typedef struct Ability Ability;
typedef struct Attack Attack;
lua_State* luaState;
GameObject* currGameObjRunning;
Ability* currAbilityRunning;
Attack* currAttackRunning;
void CallLuaFunc(int funcID);
void init_lua();
void LoadLuaFile(const char* filename, GameObject* g);
void SetLuaFuncs();
float GetTableField(lua_State* l, int tableIndex, const char* name, bool* isAField);
