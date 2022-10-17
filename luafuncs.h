#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "lua-5.4.3/src/luaconf.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


typedef struct GameObject GameObject;
typedef struct Ability Ability;
typedef struct Attack Attack;
typedef struct Effect Effect;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE; 

extern lua_State* luaState;
extern GameObject* currGameObjRunning;
extern Ability* currAbilityRunning;
extern Attack* currAttackRunning;
extern ALLEGRO_KEYBOARD_STATE* keyStateLua;

void CallLuaFunc(int funcID);
void init_lua();
void LoadLuaFile(const char* filename, GameObject* g);
void SetLuaFuncs();
float GetTableField(lua_State* l, int tableIndex, const char* name, bool* isAField);
void CreateProjectile(lua_State* l, float x, float y, const char* portrait, int attackType, int speed, int duration, bool shouldCallback, int properties, GameObject* targ, uint32_t color, Effect* effects, size_t len);
void SetLuaKeyEnums(lua_State* l);
