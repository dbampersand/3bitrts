#include <dirent.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "item.h"

#include "luafuncs.h"
#include "helperfuncs.h"
#include "ui.h"
#include "gameobject.h"
#include "effect.h"
#include "helperfuncs.h"

Item* items = NULL;
int numItems = 0;
int numItemsAllocated = 0; 

ItemPool itemPools[NUM_ITEM_POOLS] = {0};

bool propagateItemEffects = false;

Item* currItemRunning = NULL;

void InitItems()
{
    propagateItemEffects = true;
}
Item* LoadItemFuncs(Item* i, lua_State* l)
{
    int funcIndex;
    if (luaL_loadbuffer(l, i->luaBuffer,strlen(i->luaBuffer),NULL) || lua_pcall(l, 0, 0, 0))
    {
        printf("%s\n\n---\nCan't load lua file:\n %s\n---\n\n\n",COL_ERR,lua_tostring(l,-1));
        fflush(stdout);
        return NULL;
    }

    if (CheckFuncExists("update",i->luaBuffer))
    {
        lua_getglobal(l, "update");
        funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
        i->luafunc_update = funcIndex;
    }
    else
        i->luafunc_update = -1;

    if (CheckFuncExists("attached",i->luaBuffer))
    {
        lua_getglobal(l, "attached");
        funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
        i->luafunc_attached = funcIndex;
    }
    else
        i->luafunc_attached = -1;

    if (CheckFuncExists("setup",i->luaBuffer))
    {
        lua_getglobal(l, "setup");
        funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
        lua_rawgeti(l,LUA_REGISTRYINDEX,funcIndex);
        lua_pushnumber(l,i-items);
        lua_pcall(l,1,0,0);
        i->luafunc_setup = funcIndex;
    }
    else
        i->luafunc_setup = -1;


    if (CheckFuncExists("OnEffect",i->luaBuffer))
    {
        lua_getglobal(l, "OnEffect");
        funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
        i->luafunc_oneffect = funcIndex;
    }
    else
        i->luafunc_oneffect = -1;

    if (CheckFuncExists("OnAttack",i->luaBuffer))
    {
        lua_getglobal(l, "OnAttack");
        funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
        i->luafunc_onattack = funcIndex;
    }
    else
        i->luafunc_onattack = -1;

    if (CheckFuncExists("OnDamaged",i->luaBuffer))
    {
        lua_getglobal(l, "OnDamaged");
        funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
        i->luafunc_ondamaged = funcIndex;
    }
    else
        i->luafunc_ondamaged = -1;

    return i;
}

Item* LoadItem(const char* path, lua_State* l)
{
    for (int i = 0; i < numItems; i++)
    {
        Item* it = &items[i];
        
        if (it->path && strcasecmp(path,it->path) == 0)
        {
            return it;
        }
    }
    Item iNew = {0};
    iNew.goldCost = DEFAULT_COST;

    items[numItems] = iNew;

    Item* i = &items[numItems];
    numItems++;


    currItemRunning = i;
    
    i->luaBuffer = readFile(path);

    char* cpy;
    cpy = calloc(strlen(path)+1,sizeof(char));
    strcpy(cpy,path);


    if (luaL_loadbuffer(l, i->luaBuffer,strlen(i->luaBuffer),NULL) || lua_pcall(l, 0, 0, 0))
     {
         printf("%s\n\n---\nCan't load lua file:\n %s path: %s\n---\n\n\n",COL_ERR,lua_tostring(l,-1),path);
         fflush(stdout);
     }
     else
     {

        LoadItemFuncs(i,l);


        char* strSplit;
        char* svPtr;
        #ifdef __WIN32__
            strSplit = strtok_r(cpy,"/",&svPtr);
        #else
            strSplit = strtok_r(cpy,"/",&svPtr);
        #endif
        char* two = strSplit;

        while (strSplit)
        {
        #ifdef __WIN32__
            strSplit = strtok_r(NULL,"/",&svPtr);
        #else
            strSplit = strtok_r(NULL,"/",&svPtr);
        #endif

           if (strSplit == NULL)
           {    
                break;
            }
            two = strSplit;
        }
        svPtr = NULL;
        #ifdef __WIN32__
            two = strtok_r(two,".",&svPtr);

        #else
            two = strtok_r(two,".",&svPtr);

        #endif
        //if name hasnt been set in setup routine
        if (!i->name)
        {
            i->name = calloc(strlen(two)+1,sizeof(char));
            strcpy(i->name,two);
        }
        else if (strlen(i->name) == 0)
        {
            i->name = realloc(i->name,(strlen(two)+1)*sizeof(char));
            strcpy(i->name,two);

        }
        i->path = calloc(strlen(path)+1,sizeof(char));
        strcpy(i->path,path);
        //SET_BIT(&g->properties,0);

     }
     free(cpy);

    itemPools[i->itemTier].itemIndices[itemPools[i->itemTier].numItems] = i - items;
    itemPools[i->itemTier].numItems++;

    if (numItems >= numItemsAllocated)
    {
        items = realloc(items,(numItemsAllocated+32)*sizeof(Item));
        numItemsAllocated += 32;

        for (int i = 0; i < NUM_ITEM_POOLS; i++)
        {
            itemPools[i].itemIndices = realloc(itemPools[i].itemIndices,numItemsAllocated*sizeof(int));
        }
    }
    return i;
}
int GetNumSubDirs(char* path)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    int numDirs = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0)
            {
                numDirs++;
            }
        }
        closedir(d);
    }

    return numDirs;
}
//recursively load all items in path
void LoadItemFolder(char* path)
{
    if (!items)
    {
        numItems = 0;
        numItemsAllocated = GetNumSubDirs(path);
        items = calloc(numItemsAllocated,sizeof(Item));

        for (int i = 0; i < NUM_ITEM_POOLS; i++)
        {
            itemPools[i].itemIndices = calloc(numItemsAllocated,sizeof(int));
        }
    }
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (dir->d_type == DT_REG &&  strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
                {
                    char* f = strstr(dir->d_name,".lua");
                    if (f > 0)
                    {
                        char* dirConcat = calloc(strlen(path)+strlen("/")+strlen(".lua")+strlen(dir->d_name)+1,sizeof(char));
                        strcpy(dirConcat,path);
                        strcat(dirConcat,"/");
                        strcat(dirConcat,dir->d_name);

                        LoadItem(dirConcat,luaState);

                        free(dirConcat);
                    }
                }
                if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0)
                {
                        char* dirConcat = calloc(strlen(path)+strlen("/")+strlen(dir->d_name)+1,sizeof(char));
                        strcpy(dirConcat,path);
                        strcat(dirConcat,"/");
                        strcat(dirConcat,dir->d_name);

                        LoadItemFolder(dirConcat);

                        free(dirConcat);

                }
            }
            closedir(d);

        }

}
Item CopyItem(Item* i, lua_State* l)
{
    Item new = *i;
    LoadItemFuncs(&new,l);
    return new;
}
void AttachItem(GameObject* g, Item* i)
{
    Item new = CopyItem(i,luaState);
    for (int i = 0; i < INVENTORY_SLOTS; i++)
    {
        if (g->inventory[i].enabled == false)
        {
            g->inventory[i] = new;
            g->inventory[i].enabled = true;
            
            new.attachedTo = g;

            lua_rawgeti(luaState, LUA_REGISTRYINDEX, new.luafunc_attached);
            lua_pushinteger(luaState,(int)(g-objects));    
            lua_pcall(luaState,1,0,0);
        

            return;
        }
    }
}
void UpdateItem(Item* i, GameObject* g, float dt)
{
    currItemRunning = i;
    currGameObjRunning = g;
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, i->luafunc_update);
    lua_pushinteger(luaState, i - g->inventory);
    lua_pushinteger(luaState, g - objects);
    lua_pcall(luaState,2,0,0);

}
void ItemOnAttack(Item* i, GameObject* g, float dt, float* value)
{
    if (!i->enabled)
        return;
    currItemRunning = i;
    currGameObjRunning = g;
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, i->luafunc_onattack);
    lua_pushinteger(luaState, i - g->inventory);
    lua_pushinteger(luaState, g - objects);
    lua_pushinteger(luaState, g->targObj - objects);
    lua_pushnumber(luaState, dt);
    lua_pushnumber(luaState, *value);

    lua_pcall(luaState,5,1,0);

    //if script returns something
    if (lua_isnumber(luaState,-1))
    {
        *value = lua_tonumber(luaState, -1);
    }
}
void ProcessItemsOnAttack(GameObject* g, float dt, float* value)
{
    for (int i = 0; i < INVENTORY_SLOTS; i++)
    {
        Item* it = &g->inventory[i];
        ItemOnAttack(it,g,dt,value);
    }
}
void ItemOnDamaged(Item* i, GameObject* src, GameObject* target, float* value)
{
    if (!i->enabled)
        return;

    Item* beforeItem = currItemRunning;
    GameObject* beforeObj = currGameObjRunning;

    currItemRunning = i;
    currGameObjRunning = src;

    lua_rawgeti(luaState, LUA_REGISTRYINDEX, i->luafunc_ondamaged);

    lua_pushinteger(luaState, i - src->inventory);
    lua_pushinteger(luaState, src - objects);
    lua_pushinteger(luaState, target - objects);
    
    lua_pushnumber(luaState, *value);


    lua_pcall(luaState,4,1,0);
    //if script returns something
    if (lua_isnumber(luaState,-1))
    {
        *value = lua_tonumber(luaState, -1);
    }
    currItemRunning = beforeItem;
    currGameObjRunning = beforeObj;


}
void ProcessItemsOnDamaged(GameObject* source, GameObject* target, float* value)
{
    propagateItemEffects = false;
    for (int i = 0; i < INVENTORY_SLOTS; i++)
    {
        Item* it = &target->inventory[i];
        ItemOnDamaged(it,source,target,value);
    }
    propagateItemEffects = true;
    
}

void ItemOnEffect(Item* i, GameObject* g, Effect* e, float* value)
{
    if (!i->enabled)
        return;
    currItemRunning = i;
    currGameObjRunning = g;

    lua_rawgeti(luaState, LUA_REGISTRYINDEX, i->luafunc_oneffect);
    lua_pushinteger(luaState, i - g->inventory);
    lua_pushinteger(luaState, g - objects);
    lua_pushinteger(luaState, g->targObj - objects);
    
    lua_pushinteger(luaState, e->trigger);
    lua_pushinteger(luaState, e->effectType);
    lua_pushinteger(luaState, e->stacks);
    lua_pushinteger(luaState, e->value);
    lua_pushinteger(luaState, e->duration);
    
    lua_pushnumber(luaState, *value);


    lua_pcall(luaState,9,1,0);
    //if script returns something
    if (lua_isnumber(luaState,-1))
    {
        *value = lua_tonumber(luaState, -1);
    }

}
void ProcessItemsOnEffect(GameObject* g, Effect* e, float* value)
{
    for (int i = 0; i < INVENTORY_SLOTS; i++)
    {
        Item* it = &g->inventory[i];
        ItemOnEffect(it,g,e,value);
    }
}


void UpdateItems(float dt)
{
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g))
        {
            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                UpdateItem(&g->inventory[j],g,dt);
            }
        }
    }
}
Item* GetRandomItem(ItemLevel i)
{
    ItemPool* ip = &itemPools[i];
    Item* it = &items[ip->itemIndices[RandRangeI(0,ip->numItems)]];

    return it;  
}
int NumAttachedItems(GameObject* g)
{
    int numItems = 0;
    for (int i = 0; i < INVENTORY_SLOTS; i++)
    {
        if (g->inventory[i].enabled)
            numItems++;
    }
    return numItems;
}
