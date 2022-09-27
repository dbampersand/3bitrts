#pragma once

#include <stdbool.h>
#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "lua-5.4.3/src/luaconf.h"

typedef struct GameObject GameObject;
typedef struct Effect Effect;


typedef struct Item
{
    int luafunc_setup;    
    int luafunc_update;
    int luafunc_onattack;
    int luafunc_oneffect;
    int luafunc_attached;

    char* description;
    char* name;

    char* path;
    bool enabled;

    char* luaBuffer;

    GameObject* attachedTo;
} Item;

Item* items;
int numItems;
int numItemsAllocated;


Item* currItemRunning;

typedef struct lua_State lua_State;

Item* LoadItem(const char* path, lua_State* l);
void LoadItemFolder(char* path);
Item* LoadItemFuncs(Item* i, lua_State* l);
void UpdateItems(float dt);

void ProcessItemsOnAttack(GameObject* g, float dt, float* value);
void ProcessItemsOnEffect(GameObject* g, Effect* e, float* value);

void AttachItem(GameObject* g, Item* i);
