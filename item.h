#pragma once

#include <stdbool.h>
#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"
#include "lua-5.4.3/src/luaconf.h"
#include "luafuncs.h"

typedef struct GameObject GameObject;
typedef struct Effect Effect;

#define DEFAULT_COST 1
#define NUM_ITEM_POOLS 3

typedef enum ItemLevel
{
    LEVEL_POOR = 0,
    LEVEL_MID = 1,
    LEVEL_HIGH = 2
} ItemLevel;


typedef struct Item
{
    int luafunc_setup;    
    int luafunc_update;
    int luafunc_onattack;
    int luafunc_oneffect;
    int luafunc_attached;
    int luafunc_ondamaged;



    char* description;
    char* name;

    char* path;
    bool enabled;

    LuaBuffer luaBuffer;

    GameObject* attachedTo;

    int goldCost;
    int itemTier;

    int spriteIndex_Icon;
} Item;
typedef struct ItemPool
{
    int* itemIndices;
    int numItems;
} ItemPool;

#define NUM_ITEM_FUNCTIONS 5

extern Item* items;
extern int numItems;
extern int numItemsAllocated;

extern ItemPool itemPools[NUM_ITEM_POOLS];

extern bool propagateItemEffects;

extern Item* currItemRunning;

typedef struct lua_State lua_State;

Item* LoadItem(const char* path, lua_State* l);
void LoadItemFolder(char* path);
Item* LoadItemFuncs(Item* i, lua_State* l);
void UpdateItems(float dt);

void ProcessItemsOnAttack(GameObject* g, float dt, float* value);
void ProcessItemsOnEffect(GameObject* g, Effect* e, float* value);
void ProcessItemsOnDamaged(GameObject* source, GameObject* target, float* value);

void AttachItem(GameObject* g, Item* i);
void InitItems();

void RemoveItem(Item* i);

Item* GetRandomItem(ItemLevel i);
int NumAttachedItems(GameObject* g);
bool ItemIsPurchasable(Item* i);
