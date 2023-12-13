#include <time.h>
#include <math.h>
#include <float.h>

#include "luafuncs.h"

#include "gameobject.h"
#include "helperfuncs.h"
#include "map.h"
#include "effect.h"
#include "attack.h"
#include "animationeffect.h"
#include "colors.h"
#include "encounter.h"
#include "sprite.h"
#include "sound.h"
#include "gamestate.h"
#include "augment.h"
#include "ui.h"
#include "player.h"
#include "shield.h"
#include "settings.h"
#include "video.h" 
#include "pointspace.h"
#include "item.h"
#include "shop.h"
#include "vectorshape.h"
#include "timer.h"
#include "easings.h"
#include "particle.h"
#include "pathfind.h"
#include "console.h"

#include "allegro5/allegro_font.h"



 lua_State* luaState = NULL;
 GameObject* currGameObjRunning = NULL; 
 Ability* currAbilityRunning = NULL;
 Attack* currAttackRunning = NULL;
 ALLEGRO_KEYBOARD_STATE* keyStateLua = NULL;
 bool _LOADING_PREFAB = false;

void dumpstack (lua_State* l) 
{
  int top=lua_gettop(l);
  for (int i=1; i <= top; i++) {
    ConsolePrintf("%d\t%s\t", i, luaL_typename(l,i));
    switch (lua_type(l, i)) {
      case LUA_TNUMBER:
        ConsolePrintf("%g\n",lua_tonumber(l,i));
        break;
      case LUA_TSTRING:
        ConsolePrintf("%s\n",lua_tostring(l,i));
        break;
      case LUA_TBOOLEAN:
        ConsolePrintf("%s\n", (lua_toboolean(l, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        ConsolePrintf("%s\n", "nil");
        break;
      default:
        ConsolePrintf("%p\n",lua_topointer(l,i));
        break;
    }
  }
  fflush(stdout);
}
void PrintVal(lua_State* l, int i)
{
    ConsolePrintf("%s: ",luaL_typename(l,i));
    int typeVal = lua_type(l,i);
    if (typeVal == LUA_TNUMBER)
    {
        ConsolePrintf("%f\n",lua_tonumber(l,i));
    }
    if (typeVal == LUA_TBOOLEAN)
    {
        ConsolePrintf("%s\n",lua_toboolean(l,i) == true ? "True" : "False");
    }
    if (typeVal == LUA_TSTRING)
    {
        ConsolePrintf("%s\n",lua_tostring(l,i));
    }
    ConsolePrintf("\n");



}
int L_After(lua_State* l)
{
    lua_CFunction func = lua_tocfunction(l,1);
    float time = lua_tonumber(l,2);
    bool persistsAfterDeath = lua_toboolean(l,3);
    int top = lua_gettop(l);
    Timer t = {0};

    lua_newtable(l);
    lua_rotate(l,1,1);

    for (int i = 5; i <= top+1; i++)
    {
        lua_pushnumber(l,i-4);
        lua_pushvalue(l,i);
       // PrintVal(l,lua_gettop(l));

        lua_settable(l,1);
    }
    lua_pop(l,top);
    
    int j = luaL_ref(l,LUA_REGISTRYINDEX);

    t.func = func;
    t.registryIndex = j;
    t.timer = time;
    t.obj = currGameObjRunning;
    t.ability = currAbilityRunning;
    t.attack = currAttackRunning;
    t.persistsAfterDeath = persistsAfterDeath;


    Timer* ref = AddTimer(t);
    lua_pushnumber(l,(size_t)ref);
    return 1;
}
int L_UpdateTimerArgument(lua_State* l)
{
    size_t pointer = lua_tonumber(l,1);
    Timer* t = (Timer*)pointer;

    int argument = lua_tonumber(l,2);
    float valueF;
    const char* str = NULL;
    bool b;



    if (lua_isnumber(l,3))
    {
        valueF = lua_tonumber(l,3);
        UpdateTimerArgument(t,argument,&valueF,NULL,NULL);
    }
    if (lua_isstring(l,3))
    {
        str = lua_tostring(l,3);
        UpdateTimerArgument(t,argument,NULL,str,NULL);
    }
    if (lua_isboolean(l,3))
    {
        b = lua_toboolean(l,3);
        UpdateTimerArgument(t,argument,NULL,NULL,&b);
    }
    return 0;
}
int L_GetHeadingVector(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    float x2 = lua_tonumber(l,3);
    float y2 = lua_tonumber(l,4);

    float dx = x2 - x;
    float dy = y2 - y;

    float angle = atan2f(dy,dx);

    float xHeading = cosf(angle);
    float yHeading = sinf(angle);

    lua_pushnumber(l,1);
    lua_newtable(l);

    lua_pushnumber(l,xHeading);
    lua_setfield(l,-2,"headingx");

    lua_pushnumber(l,yHeading);
    lua_setfield(l,-2,"headingy");

    return 1;

}
int L_SetHPIgnoreAugment(lua_State* l)
{
    currGameObjRunning->health = lua_tonumber(l,1);
    if (currGameObjRunning->health > currGameObjRunning->maxHP)
    {
        currGameObjRunning->health = currGameObjRunning->maxHP;
    } 
    return 0;

}
int L_SetHP(lua_State* l)
{
    currGameObjRunning->health = lua_tonumber(l,1) + GetAugmentHealthBonus(lua_tonumber(l,1),currEncounterRunning->augment);
    if (currGameObjRunning->health > currGameObjRunning->maxHP)
    {
        currGameObjRunning->health = currGameObjRunning->maxHP;
    } 
    return 0;
}
int L_SetObjName(lua_State* l)
{
    const char* name = lua_tostring(l,1);
    currGameObjRunning->name = calloc(strlen(name)+1,sizeof(char));
    strcpy(currGameObjRunning->name,name);
    return 0;
}

int L_SetObjDescription(lua_State* l)
{
    const char* description = lua_tostring(l,1);
    currGameObjRunning->description = calloc(strlen(description)+1,sizeof(char));
    strcpy(currGameObjRunning->description,description);
    return 0;
}
int L_SetObjPurchaseScreenSprite(lua_State* l)
{
    if (!currGameObjRunning)
        return 0;
    const char* path = lua_tostring(l,1);
    currGameObjRunning->spriteIndex_PurchaseScreenSprite = LoadSprite(path,false);
    return 0;
}

int L_SetAttackSounds(lua_State* l)
{
    if (!lua_istable(l,1))
    {
        ConsolePrintf("Could not load attack sound, argument needs to be inside a table, for instance: {\"assets/audio...\",\"assets/audio/...\"}\n");
        return 0;
    }
    int len = lua_rawlen(l,1);

    if (currGameObjRunning->attackSoundIndices)
        free(currGameObjRunning->attackSoundIndices);

    currGameObjRunning->attackSoundIndices = calloc(len,sizeof(int));
    currGameObjRunning->numAttackSounds = len;

    for (int i = 0; i < len; i++)
    {
        lua_pushnumber(l,i+1);
        lua_gettable(l,1);


        const char* path = lua_tostring(l,-1);
        lua_pop(l,1);

        currGameObjRunning->attackSoundIndices[i] = LoadSound(path);
    }
    lua_pop(l,1);

    return 0;

}
int L_PlayMusic(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    const float gain = lua_tonumber(l,2);

    const double loopPoint = lua_tonumber(l,3);
    if (path)
    {
        PlayMusic(path,gain,loopPoint);
    }
    return 0;
}
int L_SetEncounterName(lua_State* l)
{
    const char* name = lua_tostring(l,1);
    currEncounterRunning->name = calloc(strlen(name)+1,sizeof(char));
    strcpy(currEncounterRunning->name,name);
    return 0;
}
int L_SetEncounterMusic(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    if (path)
    {
        if (currEncounterRunning->musicPath)
            free(currEncounterRunning->musicPath);
        currEncounterRunning->musicPath = calloc(strlen(path)+1,sizeof(char));
        strcpy(currEncounterRunning->musicPath,path);
    }
    return 0;
}
int L_GetTime(lua_State* l)
{
   time_t seconds;
   seconds = time(NULL);
   lua_pushnumber(l,(int)seconds);
    return 1;
}
int L_SetEncounterSprite(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    
    currEncounterRunning->spriteIndex = LoadSprite(path,true);
    return 0;
}
int L_SetEncounterDescription(lua_State* l)
{
    const char* str = lua_tostring(l,1);
    currEncounterRunning->description = calloc(strlen(str)+1,sizeof(char));
    strcpy(currEncounterRunning->description,str);
    return 0;
}
int L_AddEncounterAbility(lua_State* l)
{
    const char* str = lua_tostring(l,1);
    int index = lua_tonumber(l,2);
    int ascendLevel = lua_tonumber(l,3);

    if (index >= 0 && index < 8)
    {
        LoadAbility(str,l,&currEncounterRunning->abilities[index]);
    }
    return 0;
}


int L_SetRange(lua_State* l)
{
    SetRange(currGameObjRunning,lua_tonumber(l,1));
    //currGameObjRunning->range = lua_tonumber(l,1);
    //currGameObjRunning->range = _MAX(currGameObjRunning->range,MINIMUM_RANGE);
    return 0;
}
int L_SetDescription(lua_State* l)
{
    const char* str = lua_tostring(l,1);
    char* buff = calloc(strlen(str)+1,sizeof(char));
    memcpy(buff,str,(strlen(str)+1)*sizeof(char));
    currAbilityRunning->description = buff;
    return 0;
}
int L_SetAbilityName(lua_State* l)
{
    const char* str = lua_tostring(l,1);
    if (currAbilityRunning->name && strcmp(currAbilityRunning->name,str)==0)
    {
        return 0;
    }
    currAbilityRunning->name = calloc(strlen(str)+1,sizeof(char));
    strcpy(currAbilityRunning->name,str);
    return 0;

}
int L_SetAttackPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    
    Attack* a = &attacks[index];
    a->x = lua_tonumber(l,2);
    a->y = lua_tonumber(l,3);

    return 0;
}
int L_GetAttackRef(lua_State* l)
{
    lua_pushnumber(l,currAttackRunning-attacks);
    return 1;
}   
int L_GetThisObj(lua_State* l)
{
    lua_pushnumber(l,currGameObjRunning-objects);
    return 1;
}
int L_TimeSinceLastCast(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ABILITIES)
    {
        ConsolePrintf("L_TimeSinceLastCast: index invalid: %i\n",index);
        return 0;
    }
    Ability* a = &currGameObjRunning->abilities[index];
    lua_pushnumber(l,a->timeSinceLastCast);
    return 1;
}
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
int L_SetObjType(lua_State* l)
{
    currGameObjRunning->objType = lua_tonumber(l,1);
    return 0; 
}
int L_AddThreat(lua_State* l)
{
    int fromIndex = lua_tonumber(l,1);
    int toIndex = lua_tonumber(l,2);

    float value = lua_tonumber(l,3);

    if (fromIndex < 0 || fromIndex >= MAX_OBJS || toIndex < 0 || toIndex >= MAX_OBJS)
        return 0;
    
    AddThreat(&objects[fromIndex],&objects[toIndex],value);
    return 0;
}
int L_GetRandomUnit(lua_State* l)
{
    OBJ_FRIENDLINESS friend = lua_tonumber(l,1);
    GAMEOBJ_TYPE_HINT typeHint = lua_tonumber(l,2);
    float range = lua_tonumber(l,3);
    if (!lua_isnumber(l,3))
        range = FLT_MAX;
    int toGet = 1;
    if (lua_isnumber(l,4))
        toGet = lua_tonumber(l,4);


    GameObject** list = calloc(MAX_OBJS,sizeof(GameObject*));
    int numObjs=0;
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        if ((g->health <= 0))
            continue;
        if (ObjIsDecoration(g) && friend != TYPE_DECORATION)
            continue;
        if (GetDist(g,currGameObjRunning) < range)
        {
            if (ObjHasType(g,typeHint))
            {
                if (friend == TYPE_ENEMY)
                {
                    if (GetPlayerOwnedBy_IncludeDecor(g) != GetPlayerOwnedBy_IncludeDecor(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }
                if (friend == TYPE_FRIENDLY)
                {
                    if (GetPlayerOwnedBy_IncludeDecor(g) == GetPlayerOwnedBy_IncludeDecor(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }
            }
        }
        
    }
    if (numObjs > 0)
    {
        int numPushed = 0;
        lua_newtable(l);
        for (int i = 0; i < toGet; i++)
        {

            int pickedObj = rand()%numObjs;
            GameObject* randObj = list[pickedObj];
            lua_pushnumber(l,i+1);
            lua_pushnumber(l,randObj-objects);
            for (int j = pickedObj; j < numObjs-1; j++)
            {
                list[j] = list[j+1];
            }
            numObjs--;
            numPushed++;
            lua_settable(l,-3);
            if (numObjs <= 0)
                break;
        }
        free(list);
        return 1;


    }
    //There are no objects tagged - pick a random object that follows the friendliness
    else 
    {
        for (int i = 0; i < numActiveObjects; i++)
        {
            GameObject* g = activeObjects[i];
            if (ObjIsDecoration(g) && friend != TYPE_DECORATION)
                continue;

          
            if (GetDist(g,currGameObjRunning) < range)
            {
                if (friend == TYPE_ENEMY)
                {
                    if (GetPlayerOwnedBy_IncludeDecor(g) != GetPlayerOwnedBy_IncludeDecor(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }
                if (friend == TYPE_FRIENDLY)
                {
                    if (GetPlayerOwnedBy_IncludeDecor(g) == GetPlayerOwnedBy_IncludeDecor(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }

            }
        
        }
        if (numObjs > 0)
        {
            int numPushed = 0;
            lua_newtable(l);
            for (int i = 0; i < toGet; i++)
            {

                int pickedObj = rand()%numObjs;
                GameObject* randObj = list[pickedObj];
                lua_pushnumber(l,i+1);
                lua_pushnumber(l,randObj-objects);
                for (int j = pickedObj; j < numObjs-1; j++)
                {
                    list[j] = list[j+1];
                }
                numObjs--;
                numPushed++;
                lua_settable(l,-3);
                if (numObjs <= 0)
                    break;
            }
            free(list);
            return 1;
        }
    }
    //there are no objects with the friendliness specified so let's just pick a completely random object
    /*if (numObjs <= 0)
    {
        for (int i = 0; i < MAX_OBJS; i++)
        {
            GameObject* g = &objects[i];
            if (IsActive(g))
            {
                list[numObjs] = g;
                numObjs++;
            }
        }
        GameObject* randObj = list[rand()%numObjs];
        lua_pushnumber(l,randObj-objects);
    }*/
    //if we still have no objects
    if (numObjs <= 0)
    {
        lua_newtable(l);
    }
    free(list);
    return 1;
}
void LoadLuaFile(const char* filename, GameObject* g)
{
    
    if (!g->lua_buffer.buffer)
    {
        g->lua_buffer.buffer = readFile(filename,NULL);
     }
}
int L_SetObjectCompletionPercent(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[index];
    g->completionPercent = lua_tonumber(l,2);

    return 0;
}
int L_GetNumberOfUnitsSelected(lua_State* l)
{
    lua_pushnumber(l,players[0].numUnitsSelected);
    return 1;
}
int L_EncounterSetNumUnitsToSelect(lua_State* l)
{
    currEncounterRunning->numUnitsToSelect = lua_tonumber(l,1);
    return 0;
}
int L_ObjectUsesMana(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    bool usesMana = lua_toboolean(l,2);

    objects[index].usesMana = usesMana;
    return 0;
}
int L_Print(lua_State* l)
{
    if (currGameObjRunning)
    {
        if (currGameObjRunning->name)
        {
             ConsolePrintf("%s: %s\n",currGameObjRunning->name,lua_tostring(l,-1));
        }
        else
        {
             ConsolePrintf("%s\n",lua_tostring(l,-1));
        }
    }
    else
    {
            ConsolePrintf("%s\n",lua_tostring(l,-1));
    }
    fflush(stdout);
    return 0;
}
int L_GetOwnedBy(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0; 
    lua_pushnumber(l,GetPlayerOwnedBy(&objects[index]));
    return 1;
}
int L_GetX(lua_State* l)
{
    float x; float y; 
    if (lua_isnumber(l,1))
    {
        int index = lua_tonumber(l,1);
        if (index >= 0 && index < MAX_OBJS)
        {
            GetCentre(&objects[index],&x,&y);
            lua_pushnumber(l,x);
            
        }
        else
        {
            ConsolePrintf("GetX: Index out of range: %i\n",index);
            GetCentre(currGameObjRunning,&x,&y);
            lua_pushnumber(l,x);
        }

    }
    else
    {
        GetCentre(currGameObjRunning,&x,&y);
        lua_pushnumber(l,x);
    }
    return 1;
}
int L_GamestateIsInTransition(lua_State* l)
{
    bool b = GameStateIsTransition(&gameState);
    lua_pushboolean(l,b);
    return 1;
}
int L_GetY(lua_State* l)
{
    float x; float y; 
    if (lua_isnumber(l,1))
    {
        int index = lua_tonumber(l,1);
        if (index >= 0 && index < MAX_OBJS)
        {
            GetCentre(&objects[index],&x,&y);
            lua_pushnumber(l,y);
        }
        else
        {
            ConsolePrintf("GetY: Index out of range: %i\n",index);
            GetCentre(currGameObjRunning,&x,&y);
            lua_pushnumber(l,y);
        }
    }
    else
    {
        GetCentre(currGameObjRunning,&x,&y);
        lua_pushnumber(l,y);
    }
    return 1;
}
int L_GetGamestate(lua_State* l)
{
    lua_pushnumber(l,gameState);
    return 1;
}
int L_GetHighestThreat(lua_State* l)
{
    Threat* t = GetHighestThreat(&currGameObjRunning->threatList);
    if (t && t->obj)
        lua_pushnumber(l,t->obj-objects);
    else
        lua_pushnumber(l,-1);
    return 1;
}
int L_GetRotation(lua_State* l)
{
    lua_pushnumber(l,currGameObjRunning->angle);
    return 1;
}
int L_SetRotation(lua_State* l)
{
    float angle = lua_tonumber(l,1);
    currGameObjRunning->angle = angle;
    return 0;
}
int L_SetThreatMultiplier(lua_State* l)
{
    currGameObjRunning->threatMultiplier = lua_tonumber(l,1);
    return 0;
}
int L_GetCentre(lua_State* l)
{
    GameObject* g;
    if (!lua_isnumber(l,1))
    {
        g = currGameObjRunning;
    }
    else
    {
        int index = lua_tonumber(l,1);
        if (index >= 0 && index < MAX_OBJS)
        {
            g = &objects[index];
        }
        else
        {
            g = currGameObjRunning;
        }
    }
    lua_newtable(l);
    float x; float y; 
    GetCentre(g,&x,&y);
    lua_pushstring(l,"x");
    lua_pushnumber(l,x);
    lua_settable(l,-3);

    lua_pushstring(l,"y");
    lua_pushnumber(l,y);
    lua_settable(l,-3);

    return 1;
}
int L_GetBetween(lua_State* l)
{
    int objFrom = lua_tonumber(l,1);
    if (objFrom < 0 || objFrom >= MAX_OBJS)
        return 0; 
    GameObject* g = &objects[objFrom];
    int x2 = lua_tonumber(l,2);
    int y2 = lua_tonumber(l,3);

    float x1; float y1; 
    GetCentre(g,&x1,&y1);
    int numObjs = 0;
    lua_newtable(l);
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g2 = activeObjects[i];
        if (LineIntersectsObj(g2,x1,y1,x2,y2))
        {
            lua_pushnumber(l,numObjs);
            lua_pushnumber(l,g2-objects);
            lua_settable(l, -3);

            numObjs++;
        }   
    }
    return 1;
}
int L_GetMouseScreenX(lua_State* l)
{
    MouseState mouseState = GetMouseClamped();
    lua_pushnumber(l,mouseState.screenX);
    return 1;
}
int L_GetMouseScreenY(lua_State* l)
{
    MouseState mouseState = GetMouseClamped();
    lua_pushnumber(l,mouseState.screenY);
    return 1;
}
int L_GetMouseWorldX(lua_State* l)
{
    MouseState mouseState = GetMouseClamped();
    lua_pushnumber(l,mouseState.worldX);
    return 1;
}
int L_GetMouseWorldY(lua_State* l)
{
    MouseState mouseState = GetMouseClamped();
    lua_pushnumber(l,mouseState.worldY);
    return 1;
}
float GetTableField(lua_State* l, int tableIndex, const char* name, bool* isAField)
{
    lua_getfield(l,tableIndex,name);
    if (!lua_isnil(l,-1))
    {
        int isNum;
        float j = lua_tonumberx(l,-1, &isNum);
        if (!isNum)
        {
            *isAField = false;
            return -FLT_MAX;
        }
        lua_remove(l,-1);
        *isAField = true;
        return j;
    }
    *isAField = false; 
    lua_remove(l,-1);
    return -FLT_MAX;

}
bool GetTableField_Bool(lua_State* l, int tableIndex, const char* name, bool* isAField)
{
    lua_getfield(l,tableIndex,name);
    if (!lua_isnil(l,-1))
    {
        bool j = lua_toboolean(l,-1);
        lua_remove(l,-1);
        *isAField = true; 
        return j;
    }
    *isAField = false; 
    lua_remove(l,-1);
    return false;

}

const char* GetTableField_Str(lua_State* l, int tableIndex, const char* name, bool* isAField)
{
    lua_getfield(l,tableIndex,name);
    if (!lua_isnil(l,-1))
    {
        const char* str = lua_tostring(l,-1);
        if (!str)
        {
            *isAField = false;
            return 0;
        }
        lua_remove(l,-1);
        *isAField = true;
        return str;
    }
    *isAField = false; 
    lua_remove(l,-1);
    return 0;

}

int L_GetObjRef(lua_State* l)
{
    lua_pushnumber(l,(int)(currGameObjRunning-objects));
    return 1;
}
int L_GetAbilityRef(lua_State* l)
{
    lua_pushnumber(l,(int)(currAbilityRunning-currGameObjRunning->abilities));
    return 1;
}

int L_AbilityIsOnCooldown(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);

    if (objIndex < 0 || objIndex >= MAX_OBJS || abilityIndex < 0 || abilityIndex >= MAX_ABILITIES)
    {
        lua_pushboolean(l,true);
        return 1;
    }
    GameObject* g = &objects[objIndex];
    Ability* a = &g->abilities[abilityIndex];
    if (AbilityIsOnCooldown(a))
    {
        lua_pushboolean(l,true);
        return 1;
    }
    lua_pushboolean(l,false);
    return 1;
}
int SortThreat(const void* a, const void* b)
{
    Threat* t1 = (Threat*)a;
    Threat* t2 = (Threat*)b;

    return t2->threat - t1->threat;
}

int L_GetThreatRank(lua_State* l)
{
    int j = GetNumThreats(&currGameObjRunning->threatList);
    Threat* next = &currGameObjRunning->threatList;
    lua_newtable(l);
    
    Threat* threats = calloc(j,sizeof(Threat));
    for (int i = 0; i < j; i++)
    {
        if (next->obj && IsActive(next->obj))
            threats[i] = *next;
        next = next->next;

        if (!next)
            break;
    }
    qsort(threats,j,sizeof(threats[0]),SortThreat);
    for (int i = 0; i < j; i++)
    {
        lua_pushnumber(l,i+1);
        lua_pushnumber(l,threats[i].obj-objects);
        //ConsolePrintf("%s\n",threats[i].obj->name ? );


        lua_settable(l,-3);
    }
    free(threats);
    return 1; 
}
int L_GetAttackTarget(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
    {
        ConsolePrintf("L_GetAttackTarget: invalid index: %i",index);
        lua_pushnumber(l,-1);
        return 1;
    }
    Threat* t = GetHighestThreat(&objects[index].threatList);

    lua_pushnumber(l, t->obj - objects);
    return 1;
}
Effect GetEffectFromTable(lua_State* l, int tableStackPos, int index, GameObject* from)
{
    lua_pushnumber(l,index);
    lua_gettable(l,tableStackPos);

    Effect e = {0};
    e.maxStacks = 9999;
    bool isField = false;
    e.trigger = GetTableField(l,-1,"trigger", &isField);
    e.effectType = GetTableField(l,-1,"type",&isField);
    float triggersPerSecond = GetTableField(l,-1,"triggersPerSecond",&isField);
    //max is 30 - one eery frame at 30 fps
    if (isField)
        e.triggersPerSecond = _MIN(triggersPerSecond,30);
    else
        e.triggersPerSecond = 1;
    e.duration = GetTableField(l,-1,"duration",&isField);
    const char* portrait = GetTableField_Str(l,-1,"portrait",&isField);
    if (isField && portrait)
    {
        e.spriteIndex_Portrait = LoadSprite(portrait,false);
    }
    else
    {
        e.spriteIndex_Portrait = 0;
    }
    const char* name = GetTableField_Str(l,-1,"name",&isField);
    if (name)
    {
        e.name = calloc(strlen(name)+1,sizeof(char));
        strcpy(e.name,name);
    }
    else
        e.name = NULL;
    const char* description = GetTableField_Str(l,-1,"description",&isField);
    if (description)
    {
        e.description = calloc(strlen(description)+1,sizeof(char));
        strcpy(e.description,description);
    }
    else
        e.description = NULL;

    e.canStack = (bool)GetTableField_Bool(l,-1,"canStack",&isField);
    e.overwrites = (bool)GetTableField_Bool(l,-1,"overwrites",&isField);
    e.stacks = 1;
    e.timer = 0;
    e.numTriggers = e.duration * e.triggersPerSecond;
    if (e.numTriggers == 0) 
        e.numTriggers = 1;
    int maxStacks = GetTableField(l,-1,"maxStacks",&isField);
    if (isField)
    {
        e.maxStacks = maxStacks;
    }

    e.tickTime = e.duration / e.numTriggers;
    e.value = GetTableField(l,-1,"value",&isField);
    if (!isField)
        e.value = 0;

    if (from)
        e.value += e.value * from->abilityPotency;


    if (e.trigger == TRIGGER_PERMANENT)
        e.duration = 1;

    return e;
}
int L_SetAbilityMaxStacks(lua_State* l)
{
    currAbilityRunning->maxStacks = lua_tonumber(l,1);
    currAbilityRunning->stacks = lua_tonumber(l,1); 
    return 0;
}
int L_SetAbilityStacks(lua_State* l)
{
    currAbilityRunning->stacks = lua_tonumber(l,1); 
    if (currAbilityRunning->stacks > currAbilityRunning->maxStacks)
        currAbilityRunning->stacks = currAbilityRunning->maxStacks;
    if (currAbilityRunning->stacks < 0)
        currAbilityRunning->stacks = 0;
    return 0;
}
int L_GetWidth(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* g = &objects[index];
        lua_pushnumber(l,al_get_bitmap_width(sprites[g->spriteIndex].sprite));
    }
    else 
    {
        lua_pushnumber(l,0);
    }
    return 1;
}
int L_GetHeight(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* g = &objects[index];
        lua_pushnumber(l,al_get_bitmap_height(sprites[g->spriteIndex].sprite));
    }
    else 
    {
        lua_pushnumber(l,0);
    }
    return 1;
}
int L_SetAbilityRange(lua_State* l)
{
    if (currAbilityRunning)
        currAbilityRunning->range = lua_tonumber(l,1);
    return 0;
}
int L_GetAbilityRange(lua_State* l)
{
    int obj = lua_tonumber(l,1);
    int ability = lua_tonumber(l,2);

    if (obj < 0 || obj >= MAX_OBJS || ability < 0 || ability >= MAX_ABILITIES)
        return 0;

    GameObject* o = &objects[obj];
    Ability* a = &o->abilities[ability];

    lua_pushnumber(l,a->range);

    return 1;
}
int L_SetDamage(lua_State* l)
{
    float damage = lua_tonumber(l,1);
    if (currGameObjRunning)
    {
        float add = 0;
        if (currEncounterRunning && !IsOwnedByPlayer(currGameObjRunning)) 
            add = GetAugmentDamageBonus(damage,currEncounterRunning->augment);
        currGameObjRunning->baseDamage = damage + add;
    }
    return 0;
}
int L_SetAttackSpeed(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[index];
    float as = lua_tonumber(l,2);
    if (as < 0.05f)
        as = 0.05f;
    g->attackSpeed = as;
    return 0;
}
int L_SetAttackMoveAngle(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ATTACKS)
        return 0;
    Attack* a = &attacks[index];
    float angleX = lua_tonumber(l,2);
    float angleY = lua_tonumber(l,3);

    //if angle isn't normalised
    if (angleX > 1 || angleX < -1 || angleY > 1 || angleY < -1)
        Normalize(&angleX,&angleY);
    a->targx = angleX;
    a->targy = angleY;

    return 0;
}
int L_GetAttackMoveAngle(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ATTACKS)
        return 0;
    Attack* a = &attacks[index];
    lua_pushnumber(l,1);
    lua_newtable(l);
    
    lua_pushnumber(l,a->targx);
    lua_setfield(l,-2,"x");
    lua_pushnumber(l,a->targy);
    lua_setfield(l,-2,"y");
    return 1;

}
int L_SetAttackVelocity(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ATTACKS)
        return 0;
    Attack* a = &attacks[index];
    float v = lua_tonumber(l,2);

    a->speed = v;

    return 0;
}
int L_GetAttackVelocity(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ATTACKS)
        return 0;
    Attack* a = &attacks[index];
    lua_pushnumber(l,a->speed);

    return 1;
}
int L_AddAttackVelocity(lua_State* l)
{
    int atk = lua_tonumber(l,1);
    if (atk < 0 || atk >= MAX_ATTACKS)
    return 0;
    Attack* a = &attacks[atk];
    a->speed += lua_tonumber(l,2);
    return 0;
}
int L_AddAttackSprite(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    int w = lua_tonumber(l,2);
    int h = lua_tonumber(l,3);
    float cd = lua_tonumber(l,4);


    int before = numAnimationEffectsPrefabs;
    int index = AddAnimationEffectPrefab(path, w, h, cd);

    //if (before != index)
    {
        //free(path);
    }
    currGameObjRunning->numAttackEffectIndices++;
    if (!currGameObjRunning->onAttackEffectsIndices)
        currGameObjRunning->onAttackEffectsIndices = calloc(1,sizeof(int));
    currGameObjRunning->onAttackEffectsIndices = realloc(currGameObjRunning->onAttackEffectsIndices,currGameObjRunning->numAttackEffectIndices*sizeof(int));
    currGameObjRunning->onAttackEffectsIndices[currGameObjRunning->numAttackEffectIndices-1] = index;
    return 0;
}
int L_ShakeScreen(lua_State* l)
{
    float intensity = lua_tonumber(l,1);
    float time = lua_tonumber(l,2);
    AddScreenshake(intensity,time);
    return 0;
}
int CreateProjectile(lua_State* l, float cx, float cy, float x, float y, const char* portrait, int attackType, int speed, int duration, bool shouldCallback, int properties, GameObject* targ, uint32_t color,float radius, Effect* effects, size_t len)
{

        
    Attack a = {0};
    //a.x = currGameObjRunning->position.worldX + w/2;
    //a.y = currGameObjRunning->position.worldY + h/2;
    a.x = cx;
    a.y = cy;
    a.radius = radius;
    a.easing=0;
    a.targetRadius = a.radius;
    a.target = targ;
    float x2 = x; float y2 = y;

    if (attackType == ATTACK_PROJECTILE_ANGLE)
    {
        x2 = x - cx ;  y2 = y - cy;
        if (x2 != 0 && y2 != 0)
            Normalize(&x2,&y2);
    }
    a.targx = x2;
    a.targy = y2;

    a.effects = calloc(len,sizeof(Effect));
    for (int i = 0; i < len; i++)
    {
        Effect e2 = effects[i];
        if (effects[i].name)
        {
            e2.name = calloc(strlen(effects[i].name)+1,sizeof(char));
            strcpy(e2.name,effects[i].name);
        }
        if (effects[i].description)
        {
            e2.description = calloc(strlen(effects[i].description)+1,sizeof(char));
            strcpy(e2.description,effects[i].description);
        }
        a.effects[i] = e2;
        
    }
    
    // memcpy(a.effects,effects,sizeof(Effect)*len);
    a.numEffects = len;
    a.attackType = attackType;
    a.properties = properties;

    a.speed = speed;
    a.cameFrom = currAbilityRunning;
    a.ownedBy = currGameObjRunning;

    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.color = color;
    Attack* ref = AddAttack(&a);
    return ref - attacks;
}   
int L_CreateConeProjectiles(lua_State* l)
{
    const float x = lua_tonumber(l,1);
    const float y = lua_tonumber(l,2);
    const float toX = lua_tonumber(l,3); 
    const float toY = lua_tonumber(l,4);
    const char* portrait = lua_tostring(l,5);
    const int attackType = lua_tonumber(l,6);
    const int speed = lua_tonumber(l,7);
    const int duration = lua_tonumber(l,8);
    const bool shouldCallback = lua_toboolean(l, 9);
    const int properties = lua_tonumber(l,10);
    const int numProjectiles = lua_tonumber(l,11);
    const int color = lua_tonumber(l,12);
    const float radius = DegToRad(lua_tonumber(l,13));
    float projectileRadius = 1;


    size_t len =  lua_rawlen(l,14);

    float anglePointedAt = Normalise(PointsToAngleRad(toX,toY,x,y),0,2*M_PI);




    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 14, i,currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       


    float startAngle = anglePointedAt - radius/2.0f;

    for (int i = 0; i < numProjectiles; i++)
    {
        float angle = (i) *  (radius*2) / (float)numProjectiles;
        CreateProjectile(l,x,y, x-cosf(angle+startAngle), y-sinf(angle+startAngle), portrait, attackType, speed, duration, shouldCallback, properties, NULL, color, projectileRadius,effects, len);
    }
    return 0;
}


int L_CreateCircularProjectiles(lua_State* l)
{
    const float x = lua_tonumber(l,1);
    const float y = lua_tonumber(l,2);
    const char* portrait = lua_tostring(l,3);
    const int attackType = lua_tonumber(l,4);
    const int speed = lua_tonumber(l,5);
    const int duration = lua_tonumber(l,6);   
    const bool shouldCallback = lua_toboolean(l, 7);
    const int properties = lua_tonumber(l,8);
    const int numProjectiles = lua_tonumber(l,9);
    const int color = lua_tonumber(l,10);
    const float angleOffset = DegToRad(lua_tonumber(l,11));
    float radius = 1;

    size_t len =  lua_rawlen(l,12);

    MouseState mouseState = GetMouseClamped();
    GameObject* targ = NULL;
    if (attackType == ATTACK_PROJECTILE_TARGETED)
    {   
        for (int i = 0; i < numActiveObjects; i++)
        {
            Rect r = GetObjRect(activeObjects[i]);

            if (PointInRect(x,y,r))
            {
                targ = activeObjects[i];
                break;
            }
        }

    }


    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 12, i,currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       
    for (int i = 0; i < numProjectiles; i++)
    {
        float angle = M_PI / (float)numProjectiles*(i+1)*2; 
        CreateProjectile(l,x,y, x+cosf(angle+angleOffset), y+sinf(angle+angleOffset), portrait, attackType, speed, duration, shouldCallback, properties, targ,color,radius, effects, len);

    }
    for (int i = 0; i < len; i++)
    {
        Effect* e = &effects[i];
        if (e->name)
            free(e->name);
        if (e->description)
            free(e->description);
    }
    return 0;
}
int L_SetMoveSpeed(lua_State* l)
{
    SetMoveSpeed(currGameObjRunning,lua_tonumber(l,1));
    return 0;
}
int L_NumObjectsOwnedByPlayer(lua_State* l)
{
    int player = lua_tonumber(l,1);
    int numObjs = 0;
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        if (GetPlayerOwnedBy_IncludeDecor(activeObjects[i]) == player)
        {
            numObjs++;
        }
    }
    lua_pushnumber(l,numObjs);
    return 1;
}
int L_CreateProjectile(lua_State* l)
{
    const float fromX = lua_tonumber(l,1);
    const float fromY = lua_tonumber(l,2);
    const float targX = lua_tonumber(l,3);
    const float targY = lua_tonumber(l,4);
    const char* portrait = lua_tostring(l,5);
    const int attackType = lua_tonumber(l,6);
    const int speed = lua_tonumber(l,7);
    const int duration = lua_tonumber(l,8);
    const bool shouldCallback = lua_toboolean(l, 9);
    const int properties = lua_tonumber(l,10);
    const uint32_t color = lua_tonumber(l,11);
    float radius = lua_tonumber(l,12);
    size_t len =  lua_rawlen(l,13);

    MouseState mouseState = GetMouseClamped();
    GameObject* targ = NULL;
    if (attackType == ATTACK_PROJECTILE_TARGETED)
    {   
        for (int i = 0; i < numActiveObjects; i++)
        {
            Rect r = GetObjRect(activeObjects[i]);

            if (PointInRect(targX,targY,r))
            {
                targ = activeObjects[i];
                break;
            }
        }   

    }
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 13, i,currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       
    int ref = CreateProjectile(l, fromX, fromY, targX, targY, portrait, attackType, speed, duration, shouldCallback, properties, targ, color,radius, effects, len);
    
    for (int i = 0; i < len; i++)
    {
        Effect* e = &effects[i];
        if (e->name)
            free(e->name);
        if (e->description)
            free(e->description);
    }
    lua_pushnumber(l,ref);
    return 1;
}
//used when called from *by the ability itself*
int L_SetCooldown(lua_State* l)
{
    const float cd = lua_tonumber(l,1);
    currAbilityRunning->cooldown = cd;
    
    return 0;
}
int L_SetCooldownTimer(lua_State* l)
{
    const float cd = lua_tonumber(l,1);
    currAbilityRunning->cdTimer = cd;
    return 0;
}
//used when called outside the ability
int L_SetAbilityCooldown(lua_State* l)
{
    if (!lua_isnumber(l,1) || !lua_isnumber(l,2) || !lua_isnumber(l,3))
    {
        ConsolePrintf("L_SetAbilityCooldown: Invalid number or types of arguments. Possibly you are meaning to use SetCooldown when inside an ability.\n");
        return 0;
    }
    int gameObjIndex = lua_tonumber(l,1);
    if (gameObjIndex < 0 || gameObjIndex >= MAX_OBJS)
        return 0;
    int abilityObjIndex = lua_tonumber(l,2);
    if (abilityObjIndex < 0 || abilityObjIndex >= MAX_ABILITIES)
        return 0;

    int cd = lua_tonumber(l,3);

    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
    a->cooldown = cd;

    return 0;
}
int L_SetAbilityCooldownTimer(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    int abilityObjIndex = lua_tonumber(l,2);
    int cd = lua_tonumber(l,3);

    if (!GameObjectIndexInRange(gameObjIndex) || abilityObjIndex < 0 || abilityObjIndex >= MAX_ABILITIES)
    {
        ConsolePrintf("L_SetAbilityCooldownTimer: index out of range: %i, %i\n",gameObjIndex,abilityObjIndex);
        return 0;
    }


    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
    a->cdTimer = cd;

    if (a->cdTimer > a->cooldown)
        a->cdTimer = a->cooldown;
    if (cd != 0)
    {
        a->stacks--;
        if (a->stacks < 0)
            a->stacks = 0;
    }
    return 0;
}
int L_ReduceCooldown(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    int abilityObjIndex = lua_tonumber(l,2);
    int cd = lua_tonumber(l,3);

    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
    LowerAbilityCooldown(a,cd);
    return 0;
}
int L_SetAbilityOnCooldown(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    if (gameObjIndex < 0 || gameObjIndex >= MAX_OBJS)
        return 0;
    int abilityObjIndex = lua_tonumber(l,2);
    if (abilityObjIndex < 0 || abilityObjIndex >= MAX_ABILITIES)
        return 0;
    GameObject* g = &objects[gameObjIndex];
    return 0;
}
int L_GetAbilityCooldownTimer(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    int abilityObjIndex = lua_tonumber(l,2);

    if (!GameObjectIndexInRange(gameObjIndex) || abilityObjIndex < 0 || abilityObjIndex >= MAX_ABILITIES)
    {
        ConsolePrintf("L_GetAbilityCooldownTimer: index out of range: %i, %i\n",gameObjIndex,abilityObjIndex);
        return 0;
    }

    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
    
    if (a->stacks == a->maxStacks)
    {
        lua_pushnumber(l,0);
        return 1;
    }
    lua_pushnumber(l,a->cdTimer);
    return 1;
}
int L_SetObjIsBoss(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    bool value = lua_toboolean(l,2);

    if (gameObjIndex < 0 || gameObjIndex >= MAX_OBJS)
        return 0;

    SetObjIsBoss(&objects[gameObjIndex],value);
    return 0;
}

int L_GetAbilityCooldown(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    int abilityObjIndex = lua_tonumber(l,2);

    if (!GameObjectIndexInRange(gameObjIndex) || abilityObjIndex < 0 || abilityObjIndex >= MAX_ABILITIES)
    {
        ConsolePrintf("L_GetAbilityCooldown: index out of range: %i, %i\n",gameObjIndex,abilityObjIndex);
        return 0;
    }


    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
    lua_pushnumber(l,a->cooldown);
    return 1;
}



int L_UntoggleOthers(lua_State* l)
{
    int index = currAbilityRunning-currGameObjRunning->abilities;
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        if (index == i) continue;
        ToggleAbility(&currGameObjRunning->abilities[i],currGameObjRunning,false);
    }
    return 0;
}
int L_UntoggleAll(lua_State* l)
{
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        ToggleAbility(&currGameObjRunning->abilities[i],currGameObjRunning,false);
    }
    return 0;
}
int L_GetAttackLifetime(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ATTACKS)
        return 0;
    Attack* a = &attacks[index];

    lua_pushnumber(l,a->duration);
    return 1;
}
int L_SetAttackLifetime(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_ATTACKS)
        return 0;
    Attack* a = &attacks[index];
    a->duration = lua_tonumber(l,2);
    return 0;
}
int L_ToggleAbility(lua_State* l)
{
    Ability* a;
    bool toggle = false; 
    if (lua_isnumber(l,1))
    {
        int index = lua_tonumber(l,1);
        if (index < 0 || index >= 4)
            return 0; 
        a = &currGameObjRunning->abilities[index];
        if (lua_isboolean(l,2))
        {
            toggle = lua_toboolean(l,2);
        }
        else
        {
            toggle = !a->toggled;
        }
    }
    else
    {
        a = currAbilityRunning;
        if (lua_isboolean(l,1))
        {
            toggle = lua_toboolean(l,1);
        }
        else
        {
            toggle = !a->toggled;
        }

    }
    ToggleAbility(a,currGameObjRunning,toggle);
    return 0;
}
int L_GetHP(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;

    lua_pushnumber(l,objects[index].health);
    return 1;
}
int L_GetMaxHP(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;

    lua_pushnumber(l,objects[index].maxHP);
    return 1;
}

int L_GetHPPercent(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;

    lua_pushnumber(l,objects[index].health / objects[index].maxHP * 100);
    return 1;

}
int L_SetObjAggroRadius(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    int radius = lua_tonumber(l,2);
    objects[index].aggroRadius = radius;
    return 0;
}
int L_SetMovePoint(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* obj = &objects[index];
    const float x = lua_tonumber(l,2);
    const float y = lua_tonumber(l,3);
    bool shouldAttack = lua_toboolean(l,4);
    bool shouldPathfind = lua_toboolean(l,5);

    GameObject* target = GetClicked(x,y);
    ClearCommandQueue(obj);

    //always pop the 0th element after processing the cast command
    //so add a null move
    //MoveCommand(obj,0,0);
    if (target && shouldAttack)
    {
        currGameObjRunning->targObj = target;
        AttackCommand(obj,target,false);
        
    }
    else
    {
        obj->targObj = NULL;
        int w = al_get_bitmap_width(sprites[obj->spriteIndex].sprite);
        int h = al_get_bitmap_height(sprites[obj->spriteIndex].sprite);

        SetTargetPosition(obj,x-w/2,y-h/2);
        if (!shouldPathfind)
        {
            obj->pathfindNeedsRefresh = false;
            obj->currentPathingNode = 0;
            obj->pathNodes[0].p.x = x; 
            obj->pathNodes[0].p.y = y;
            obj->pathNodes[1].p.x = x; 
            obj->pathNodes[1].p.y = y; 
        }
       // obj->targObj = NULL;
        
        MoveCommand(obj,x-w/2,y-w/2,false);
    }
    return 0;
}
int L_StopCommand(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* obj = &objects[index];

    StopCommand(&objects[index],false);
    return 0;
}
int L_HoldCommand(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* obj = &objects[index];

    HoldCommand(&objects[index],false);
    return 0;
}

int L_EnableAI(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;

    GameObject* g = &objects[index];
    bool set = lua_toboolean(l,2);

    g->shouldProcessAI = set;
    return 0;
}
int L_SetAttackTarget(lua_State* l)
{
    int indexAtk = lua_tonumber(l,1);
    int indexTarget = lua_tonumber(l,2);

    if (indexAtk >= 0 && indexAtk < MAX_ATTACKS && indexTarget >= 0 && indexTarget < MAX_OBJS)
    {
        attacks[indexAtk].target = &objects[indexTarget];
    }
    return 0;
}
int L_SetAttackTargetPosition(lua_State* l)
{   
    int atk = lua_tonumber(l,1);
    if (atk < 0 && atk >= MAX_ATTACKS)
    {
        ConsolePrintf("L_SetAttackTargetPosition: index out of range: %i\n",atk);
        return 0;
    }

    attacks[atk].targx = lua_tonumber(l,2);
    attacks[atk].targy = lua_tonumber(l,3);

    return 0;
}
int L_BlockCommands(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index))
    {
        ConsolePrintf("L_BlockCommands: index out of range: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];
    g->commandsBlocked = lua_toboolean(l,2);
    return 0;
}
int L_CreateCone(lua_State* l)
{
    const float x = lua_tonumber(l,1);
    const float y = lua_tonumber(l,2);
    const float x2 = lua_tonumber(l,3);
    const float y2 = lua_tonumber(l,4);

    const char* effectPortrait = lua_tostring(l,5);
    const float radius = lua_tonumber(l,6);
    const float tickrate = lua_tonumber(l,7);
    const float duration = lua_tonumber(l, 8);
    const bool shouldCallback = lua_toboolean(l, 9);
    const int properties = lua_tonumber(l,10);
    const int color = lua_tonumber(l,11);
    const int dither = lua_tonumber(l,12);
    const float length = lua_tonumber(l,13);
    size_t len =  lua_rawlen(l,14);

    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 14, i,currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }  

    Attack a = {0};
    a.x = x;
    a.y = y;
    a.targx = x2;
    a.targy = y2;
    a.easing=1.0;
    a.radius = radius;
    a.targetRadius = radius;
    a.effects = calloc(len,sizeof(Effect));
    memcpy(a.effects,effects,sizeof(Effect)*len);
    a.numEffects = len;
    a.ownedBy = currGameObjRunning;
    a.properties = properties;
    a.cameFrom = currAbilityRunning;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.attackType = ATTACK_CONE;
    a.tickrate = tickrate;
    a.color = color;
    a.dither = dither;
    a.range = length;
    Attack* ref = AddAttack(&a);

    lua_pushnumber(l,ref - attacks);

    return 1;
}
int L_SetAttackCircle(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_ATTACKS)
    {
        Attack* a = &attacks[index];
        bool circle = lua_toboolean(l,2);
        if (circle)
        {
            a->properties |= ATTACK_DRAW_CIRCLE;
        }
        else
            a->properties &= ~(ATTACK_PROPERTIES)ATTACK_DRAW_CIRCLE;
    }
    return 0;
}
int L_SetAggroGroup(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int group = lua_tonumber(l,2);

    if (objIndex < 0 || objIndex >= MAX_OBJS)
    {
        ConsolePrintf("L_SetAggroGroup: ObjIndex not valid: %i\n", objIndex);
        return 0;
    }
    GameObject* g = &objects[objIndex];

    if (group > 0)
    {
        g->aggroGroup = group;
        g->aggroGroupSet = true;
    }
    else
    {
        g->aggroGroup = 0;
        g->aggroGroupSet = false;
    }
    lua_pushnumber(l,g - objects);
    return 1;
}
int L_GetAggroGroup(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0 || objIndex >= MAX_OBJS)
    {
        ConsolePrintf("L_RemoveAggroGroup: ObjIndex not valid: %i\n", objIndex);
        return 0;
    }
    GameObject* g = &objects[objIndex];
    if (!g->aggroGroupSet)
    {
        lua_pushboolean(l,false);
        return 1;
    }
    
    lua_pushnumber(l,g->aggroGroup);
    return 1;

}
int L_RemoveAggroGroup(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0 || objIndex >= MAX_OBJS)
    {
        ConsolePrintf("L_RemoveAggroGroup: ObjIndex not valid: %i\n", objIndex);
        return 0;
    }
    GameObject* g = &objects[objIndex];
    g->aggroGroup = 0;
    g->aggroGroupSet = false;
    return 0;
}
int L_SetObjectPushable(lua_State* l)
{
    int obj = lua_tonumber(l,1);
    bool b = lua_toboolean(l,2);
        if (obj < 0 || obj >= MAX_OBJS)
    {
        ConsolePrintf("L_SetObjectPushable: Index invalid: %i\n",obj);
        return 0;
    }
    objects[obj].objIsPushable = b;
    return 0;

}
int L_PushObj(lua_State* l)
{
    float fromX = lua_tonumber(l,1);
    float fromY = lua_tonumber(l,2);

    int objPushing = lua_tonumber(l,3);
    
    float velocity = lua_tonumber(l,4);
    float timeToPush = lua_tonumber(l,5);

    if (objPushing < 0 || objPushing >= MAX_OBJS)
    {
        ConsolePrintf("L_PushObj: Index invalid: %i\n",objPushing);
        return 0;
    }
    GameObject* obj = &objects[objPushing];
    PushObj(obj,velocity,timeToPush,(Point){fromX,fromY});

    return 0;
}
int L_RotateAttackArea(lua_State* l)
{
    int atk = lua_tonumber(l,1);
    float angle = DegToRad(lua_tonumber(l,2));
    if (atk < 0 || atk >= MAX_ATTACKS)
    {
        ConsolePrintf("L_RotateAttackArea: invalid index: %i\n",atk);
        return 0;
    }

    Attack* a = &attacks[atk];
    a->shape.angle += angle;
    return 0;
}
int L_HasDoneTutorial(lua_State* l)
{
    lua_pushboolean(l,currSettings.hasDoneTutorial);
    return 1;
}
int L_DoneTutorial(lua_State* l)
{
    bool b = lua_toboolean(l,1);
    currSettings.hasDoneTutorial = b;
    WriteSettingsFile("config.cfg");
    return 0;
}

int L_CreateAttackArea(lua_State* l)
{
    int lenArea = lua_rawlen(l,1);
    Point* points = calloc(lenArea,sizeof(Point));


    for (int i = 0; i < lenArea; i++)
    {
        lua_pushnumber(l,i+1);
        lua_gettable(l,1);

        lua_pushnumber(l,1);
        lua_gettable(l,-2);
        lua_pushnumber(l,2);
        lua_gettable(l,-3);

        points[i].x = lua_tonumber(l,-2);
        points[i].y = lua_tonumber(l,-1);


        lua_pop(l,1);
        lua_pop(l,1);
        lua_pop(l,1);
    }   

    float x = lua_tonumber(l,2);
    float y = lua_tonumber(l,3);

    VectorShape v = CreateVectorShape(points,lenArea,x,y);
    free(points);

    const char* effectPortrait = lua_tostring(l,4);
    const float tickrate = lua_tonumber(l,5);
    const float duration = lua_tonumber(l, 6);
    const bool shouldCallback = lua_toboolean(l, 7);
    const int properties = lua_tonumber(l,8);
    const int color = lua_tonumber(l,9);
    const int dither = lua_tonumber(l,10);
    const bool isSoak = lua_toboolean(l,11);


    size_t len =  lua_rawlen(l,12);
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 12, i, currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       

    Attack* ref = CreateAttackShape(v,x,y, (char*)effectPortrait, tickrate, duration, shouldCallback,  properties,  color,  dither,  len, effects, NULL, currGameObjRunning);

    if (ref)
        lua_pushnumber(l,ref - attacks);
    else
        lua_pushnumber(l,-1);
    return 1;


}
int L_CutoutArea(lua_State* l)
{
    int attackIndex = lua_tonumber(l,1);

    int lenArea = lua_rawlen(l,2);
    Point* points = calloc(lenArea,sizeof(Point));


    for (int i = 0; i < lenArea; i++)
    {
        lua_pushnumber(l,i+1);
        lua_gettable(l,2);

        lua_pushnumber(l,1);
        lua_gettable(l,-2);
        //if the table is instead the format of 
        // {x = 1, y = 2}
        if (lua_isnil(l,-1))
        {
            lua_pop(l,1);
            lua_pushstring(l,"x");
            lua_gettable(l,-2);
        }

        lua_pushnumber(l,2);
        lua_gettable(l,-3);
        if (lua_isnil(l,-1))
        {
            lua_pop(l,1);
            lua_pushstring(l,"y");
            lua_gettable(l,-3);
        }


        points[i].x = lua_tonumber(l,-2);
        points[i].y = lua_tonumber(l,-1);

        lua_pop(l,1);
        lua_pop(l,1);
        lua_pop(l,1);

    }   

    NOTArea(&attacks[attackIndex].shape,points,lenArea);
    free(points);
    return 0;
}
int L_CreateAOE(lua_State* l)
{
    //read from table of effects
    //eg [Effect: DoT of 10dps, Effect: Slows]
    const float x = lua_tonumber(l,1);
    const float y = lua_tonumber(l,2);
    const char* effectPortrait = lua_tostring(l,3);
    const float radius = lua_tonumber(l,4);
    float tickrate = lua_tonumber(l,5);
    float duration = lua_tonumber(l, 6);
    const bool shouldCallback = lua_toboolean(l, 7);
    const int properties = lua_tonumber(l,8);
    const int color = lua_tonumber(l,9);
    const int dither = lua_tonumber(l,10);
    const bool isSoak = lua_toboolean(l,11);
    const int target = lua_tonumber(l,12);
    float initialAngleX = lua_tonumber(l,13);
    float initialAngleY = lua_tonumber(l,14);
    float initialSpeed = lua_tonumber(l,15);

    //30fps is minimum
    tickrate = _MAX(1/30.0f,tickrate);
    duration = _MAX(1/30.0f,duration);


    size_t len =  lua_rawlen(l,16);
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
        for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 16, i, currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       

    GameObject* targ = NULL;
    if (target >= 0 && target < MAX_OBJS)
    {   
        targ = &objects[target];
    }

    Attack* ref = CreateAoE(x,y, (char*)effectPortrait, radius, tickrate, duration, shouldCallback,  properties,  color,  dither,  len, effects, targ, currGameObjRunning);
    ref->cameFrom = currAbilityRunning;
    ref->targx = initialAngleX;
    ref->targy = initialAngleY;
    ref->speed = initialSpeed;

    
    if (isSoak && ref)
    {
        ref->properties |= ATTACK_SOAK;
    }
    //Attack* ref = AddAttack(&a);
    if (ref)
        lua_pushnumber(l,ref - attacks);
    else
        lua_pushnumber(l,-1);
    return 1;
}
int L_SetAutoWin(lua_State* l)
{
    bool b = lua_toboolean(l,1);
    currMap->automaticWinCheck = b;
    return 0;
}
int L_EnableNextLevelButton(lua_State* l)
{
    currMap->forceNextLevelButtonEnabled = lua_toboolean(l,1);
    return 0;
}
int L_Win(lua_State* l)
{
    WinGame();
    return 0;
}
int L_Lose(lua_State* l)
{
    LoseGame();
    return 0;
}

int L_GetCurrentMessage(lua_State* l)
{
    if (chatboxes)
    {
        if (chatboxShowing)
        {
            lua_pushstring(l,chatboxShowing->text);
        }
        else
        {
            lua_pushstring(l,"");
        }
    }
    else
    {
        lua_pushstring(l,"");
    }
    return 1;
}
int L_PopMessage(lua_State* l)
{
    Chatbox_NextLine();
    return 0;
}
int L_ClearMessages(lua_State* l)
{
    EndChatbox();
    return 0;
}
int L_GetShield(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS) 
    {
        int shield = GetTotalShield(&objects[index]);
        lua_pushnumber(l,shield);
    }
    else
    {
        lua_pushnumber(l,0);
    }
    return 1;
}
int L_PushMessage(lua_State* l)
{
    if (gameState == GAMESTATE_IN_EDITOR || transitioningTo == GAMESTATE_IN_EDITOR)
        return 0;
    gameState = GAMESTATE_IN_CHATBOX;
    transitioningTo = GAMESTATE_IN_CHATBOX;
    const char* msg = lua_tostring(l,1);
    if (msg)
    {
        numChatboxes++;
        if (!chatboxes)
        {
            chatboxes = calloc(1,sizeof(Chatbox));
            numChatboxes = 1;
        }
        chatboxes = realloc(chatboxes,numChatboxes*sizeof(Chatbox));
        chatboxes[numChatboxes-1].text  = calloc(strlen(msg)+1,sizeof(char));
        chatboxes[numChatboxes-1].displayingUpTo  = calloc(strlen(msg)+1,sizeof(char));
        chatboxes[numChatboxes-1].charTimer = 0;
        strcpy(chatboxes[numChatboxes-1].text,msg);

        chatboxes[numChatboxes-1].x = lua_tonumber(l,2);
        chatboxes[numChatboxes-1].y = lua_tonumber(l,3);
        chatboxes[numChatboxes-1].w = lua_tonumber(l,4);
        chatboxes[numChatboxes-1].h = lua_tonumber(l,5);
        chatboxes[numChatboxes-1].allowsInteraction = lua_toboolean(l,6);
        if (chatboxes[numChatboxes-1].allowsInteraction)
        {
            strcpy(chatboxes[numChatboxes-1].displayingUpTo,msg);
        }
        chatboxShowing = &chatboxes[0];

        if (chatboxShowing->allowsInteraction)
        {
            gameState = GAMESTATE_INGAME;
            transitioningTo = GAMESTATE_INGAME;
        }
        else
        {
            gameState = GAMESTATE_IN_CHATBOX;
            transitioningTo = GAMESTATE_IN_CHATBOX;
        }
    }

    else
    {
        if (chatboxShowing->allowsInteraction)
        {
            gameState = GAMESTATE_INGAME;
            transitioningTo = GAMESTATE_INGAME;
        }
    }
    return 0;
}
int L_RemoveAttack(lua_State* l)
{
    int atk = lua_tonumber(l,1);
    RemoveAttack(atk);
    return 0;
}
int L_SetMaxMana(lua_State* l)
{
    float mana = lua_tonumber(l,1);
    bool shouldAdd = lua_toboolean(l,2);

    currGameObjRunning->maxMana = mana;
    if (shouldAdd)
    {
        AddMana(currGameObjRunning,mana);
    }
    return 0;

}
int L_SetMaxHP(lua_State* l)
{
    float hp = lua_tonumber(l,1);
    bool heal = lua_toboolean(l,2);


    currGameObjRunning->maxHP = hp;

    //commenting this out for now as generating a heal event probably isn't what we want
        //if (heal)
            //  Heal(currGameObjRunning,hp);
    //instead just do it manually
    if (heal)
    {
        currGameObjRunning->health += hp;
        if (currGameObjRunning->health > currGameObjRunning->maxHP)
        {
            currGameObjRunning->health = currGameObjRunning->maxHP;
        }
    }
    return 0;
}
int L_SetObjTargetPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        SetTargetPosition(obj,lua_tonumber(l,2),lua_tonumber(l,3));
        
        obj->targObj = NULL;
    }
    return 0;
}
int L_GetObjTargetPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index))
    {
        ConsolePrintf("L_GetObjTargetPosition: index out of range: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];

    lua_newtable(l);

    lua_pushstring(l,"x");
    lua_pushnumber(l,g->targetPosition.x);
    lua_settable(l,-3);

    lua_pushstring(l,"y");
    lua_pushnumber(l,g->targetPosition.y);
    lua_settable(l,-3);
    return 1;
}

int L_SetObjPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        //obj->position.worldX = lua_tonumber(l,2);
        //obj->position.worldY = lua_tonumber(l,3);
        UpdateObjPosition(obj,lua_tonumber(l,2),lua_tonumber(l,3));
    }
    return 1;
}
int L_Cerp(lua_State* l)
{
    float f = Normalise(lua_tonumber(l,1),0,2*M_PI);
    float to = Normalise(lua_tonumber(l,2),0,2*M_PI);
    float maxMove = lua_tonumber(l,3);


    float result = TowardsCircular(f,to,maxMove);
    lua_pushnumber(l,result);
    return 1;

}
int L_Lerp(lua_State* l)
{
    float f = lua_tonumber(l,1);
    float to = lua_tonumber(l,2);
    float maxMove = lua_tonumber(l,3);

    f = Towards(f,to,maxMove);
    lua_pushnumber(l,f);
    return 1;
}
int L_Teleport(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        ScatterEffect(obj);
        for (int i = 0; i < RandRange(20,60); i++)
            AddParticleWithRandomProperties(obj->position.worldX+RandRange(0,GetWidth(obj)),obj->position.worldY+RandRange(0,GetHeight(obj)),GameObjToColor(obj),1,3,6,12,2*-M_PI,2*M_PI);
        Teleport(obj,lua_tonumber(l,2),lua_tonumber(l,3),true);
        for (int i = 0; i < RandRange(20,60); i++)
            AddParticleWithRandomProperties(obj->position.worldX+RandRange(0,GetWidth(obj)),obj->position.worldY+RandRange(0,GetHeight(obj)),GameObjToColor(obj),1,3,30,40  ,2*-M_PI,2*M_PI);

    }
    return 0;
}
int L_SetAttacking(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS) 
        return 0; 
    currGameObjRunning->targObj = &objects[index];
    return 0;
}
int L_SetInvincible(lua_State* l)
{
    int obj = lua_tonumber(l,1);
    bool set = lua_toboolean(l,2);

    if (obj >= 0 && obj < MAX_OBJS)
    {
        SetInvincible(&objects[obj],set);
    }
    return 0;
}

int L_SetDecoration(lua_State* l)
{
    int obj = lua_tonumber(l,1);
    bool set = lua_toboolean(l,2);

    if (obj >= 0 && obj < MAX_OBJS)
    {
        SetDecoration(&objects[obj],set);
    }
    GameObject* g = &objects[obj];
    lua_pushnumber(l,objects - g);
    return 1;
}
int L_IsDecoration(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
    {
        ConsolePrintf("L_IsDecoration: invalid index: %i.\n",index);
        return 0;
    }
    lua_pushboolean(l,ObjIsDecoration(&objects[index]));
    return 1;
}
int L_CreateObject(lua_State* l)
{
    GameObject* g;
    const char* l_path = lua_tostring(l,1);
    if (!l_path)
    {
        ConsolePrintf("CreateObject: Path null\n");
        return 0;    
    }
    const int x = lua_tonumber(l,2);
    const int y = lua_tonumber(l,3);
    int PLAYER = lua_tonumber(l,4);
    float summonTime = lua_tonumber(l,5);
    float completionPercent = DEFAULT_COMPLETION_PERCENT; 
    bool applyCompletion = false;
    if (gameState == GAMESTATE_IN_EDITOR)
        summonTime = 0; 
    GameObject* before = currGameObjRunning;

    int ownedBy = PLAYER;
    //when called from a gameobject or an ability used by a gameobject,
    //set the player appropriately (if TYPE_FRIENDLY, make it a friend of that object)
    if (currGameObjRunning)
    {
        if (PLAYER == TYPE_FRIENDLY)
        {
            ownedBy = GetPlayerOwnedBy(currGameObjRunning);
        }
        if (PLAYER == TYPE_ENEMY)
        {
            //if owned by player
            if (GetPlayerOwnedBy(currGameObjRunning) == 0)
            {
                ownedBy = 1;
            }
            else
            {
                ownedBy = 0;
            }
        }
    }
    if (lua_isnumber(l,6))
    {
        completionPercent = lua_tonumber(l,6);
        applyCompletion = true;
    }

    GAMEOBJ_SOURCE source = SOURCE_SPAWNED_FROM_OBJ;
    if (!currGameObjRunning)
        source = SOURCE_SPAWNED_FROM_MAP;
    
        


    bool prefabFound = false;
    for (int i = 0; i < numPrefabs; i++)
    {
        
        if (prefabs[i]->path && strcasecmp(prefabs[i]->path,l_path) == 0)
        {
            g = prefabs[i];
            prefabFound = true;
            break;
        }
    } 
    if (!prefabFound)
    {
        GameObject* prefab = LoadPrefab(l_path);
        g = AddGameobject(prefab,x,y,source,ownedBy);
        SetOwnedBy(g, ownedBy);

    }
    else
    {
        g = AddGameobject(g,x,y,source,ownedBy);
        SetOwnedBy(g, ownedBy);
    }
    if (source == SOURCE_SPAWNED_FROM_OBJ)  
    {
        g->objectIsSummoned = true;
    }
    if (PLAYER == TYPE_DECORATION)
    {
        //SetDecoration(currGameObjRunning,true);
    }

    currGameObjRunning = before;
    if (!g)
        return 0;
    g->summonTime = 0;
    g->summonMax = summonTime;

    if (applyCompletion)
        g->completionPercent = completionPercent;


    if (!ObjIsDecoration(g))
    {
        if (source == SOURCE_SPAWNED_FROM_OBJ && currGameObjRunning)
        {
            if (IsInCombat(currGameObjRunning))
            {
                for (int i = 0; i < numActiveObjects; i++)
                {
                    if (GetPlayerOwnedBy(activeObjects[i]) != GetPlayerOwnedBy(g) && !ObjIsDecoration(activeObjects[i]))
                    {
                        GameObject* gac = activeObjects[i];
                        float range = (255) / GetDist(g,activeObjects[i]);
                        AddThreat(activeObjects[i],g,range);
                    }
                }
                AttackHighestThreatCommand(g);
            }
        }    
    }

    lua_pushnumber(l,g-objects);
    return 1;
}
int L_NumIsInRange(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    float dist = lua_tonumber(l,3);
    lua_pushboolean(l,x > y - dist && x < y + dist);
    return 1;
}
int L_GetClosestObjectInRange(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    float range = lua_tonumber(l,3);

    float closestDist = range;
    GameObject* closestObj = NULL;

    int friendliness = lua_tonumber(l,4);
    int thisObjFriendliness = GetPlayerOwnedBy(currGameObjRunning);


    for (int i = 0; i < numActiveObjects; i++)
    {
        if (friendliness == TYPE_FRIENDLY)
        {
            if (thisObjFriendliness != GetPlayerOwnedBy(activeObjects[i]))
            {
                continue;
            }
        }
        if (friendliness == TYPE_ENEMY)
        {
            if (thisObjFriendliness == GetPlayerOwnedBy(activeObjects[i]))
            {
                continue;
            }
        }

            
        float cx; float cy;
        GetCentre(activeObjects[i],&cx,&cy);
        if (dist(cx,cy,x,y) < closestDist)
        {
            closestObj = activeObjects[i];
            closestDist = dist(cx,cy,x,y);
        }
    }
    if (closestObj != NULL)
        lua_pushnumber(l,closestObj-objects);
    else
        lua_pushnumber(l,-1);
    return 1;

}
int L_GetFurthestObjectInRange(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    float range = lua_tonumber(l,3);
    int friendliness = lua_tonumber(l,4);
    
    int thisObjFriendliness = GetPlayerOwnedBy(currGameObjRunning);


    float furthestDist = 0;
    GameObject* furthestObj = NULL;

    for (int i = 0; i < numActiveObjects; i++)
    {
        if (friendliness == TYPE_FRIENDLY)
        {
            if (thisObjFriendliness != GetPlayerOwnedBy(activeObjects[i]))
            {
                continue;
            }
        }
        if (friendliness == TYPE_ENEMY)
        {
            if (thisObjFriendliness == GetPlayerOwnedBy(activeObjects[i]))
            {
                continue;
            }
        }

        float cx; float cy;
        GetCentre(activeObjects[i],&cx,&cy);
        if (dist(cx,cy,x,y) > furthestDist)
        {
            furthestObj = activeObjects[i];
            furthestDist = dist(cx,cy,x,y);
        }
    }
    if (furthestObj != NULL)
        lua_pushnumber(l,furthestObj-objects);
    else
        lua_pushnumber(l,-1);
    return 1;

}
int L_GetObjectsInRange(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    float range = lua_tonumber(l,3);
    int ownedBy = lua_tonumber(l,4);

    int index = 1;
    lua_newtable(l);
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        float cx; float cy;
        GetCentre(g,&cx,&cy);
        if (GetPlayerOwnedBy(g) == ownedBy)
        {
            if (dist(cx,cy,x,y) < range)
            {

                lua_pushnumber(l,index);
                lua_newtable(l);


                lua_pushnumber(l, i);
                lua_setfield(l, -2, "obj");

                lua_pushnumber(l, dist(cx,cy,x,y));
                lua_setfield(l, -2, "dist");
                
                lua_settable(l,-3);

                index++;
            }
        }
    }


    return 1;
}
int L_GetUnitCommandList(lua_State* l)
{
    int index = lua_tonumber(l,1);
    GameObject* g = &objects[index];
    lua_newtable(l);
    for (int i = 0; i < MAX_QUEUED_CMD; i++)
    {
        lua_pushnumber(l,i+1);
        lua_pushnumber(l,g->queue[i].commandType);
        lua_settable(l,-3);
    }
    return 1;    
}

int L_GetUnitCurrentCommand(lua_State* l)
{
    int index = lua_tonumber(l,1);
    GameObject* g = &objects[index];
    lua_pushnumber(l,g->queue[0].commandType);
    return 1;
}
int L_AbilitySetCastType(lua_State* l)
{
    if (!currAbilityRunning)
    {
        printf("L_AbilitySetCastType: no ability running.");
        return 0;   
    }
    currAbilityRunning->castType = lua_tonumber(l,1);
    return 0;   
}
int L_SetSprite(lua_State* l)
{
    if (currGameObjRunning)
    {
        currGameObjRunning->spriteIndex = LoadSprite(lua_tostring(l,-1),true);
         
        if (currGameObjRunning->channelingSpriteIndex == 0)
            currGameObjRunning->channelingSpriteIndex = currGameObjRunning->spriteIndex;
    }
    return 0;
}
int L_ClearChanneledAbility(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index))
    {
        printf("L_ClearChanneledAbility: index out of range: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];
    g->properties &= ~OBJ_IS_CHANNELLING;
    g->channellingTime = 0;
    g->channellingTotal = 0;
    g->channelled_target = NULL;
    g->channelledAbility = NULL;
    return 0;

}
int L_SetLightColour(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[index];
    
    g->lightR = lua_tonumber(l,2)/255.0f;
    g->lightG = lua_tonumber(l,3)/255.0f;
    g->lightB = lua_tonumber(l,4)/255.0f;

    return 0;
}

int L_SetLightSize(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[index];

    int size = lua_tonumber(l,2);
    SetLightSize(g,size);
    return 0;
}
int L_SetLightIntensity(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[index];

    float intensity = lua_tonumber(l,2);
    g->lightSize = intensity;
    return 0;
}
int L_SetMapName(lua_State* l)
{
    if (currMap)
    {
        if (currMap->name)
        {
            free(currMap->name);
        }
        currMap->name = calloc(strlen(lua_tostring(l,1))+1,sizeof(char));
        strcpy(currMap->name,lua_tostring(l,1));
    }   
    return 0;
}

int L_SetMapSprite(lua_State* l)
{
    currMap->spriteIndex = LoadSprite(lua_tostring(l,-1),true);
    PreprocessMap(currMap,RandRangeI(3,15));
    return 0;
}
int L_SetAbilityCastTypeHint(lua_State* l)
{
    currAbilityRunning->castType = lua_tonumber(l,1);
    return 0;
}
int L_SetSpeed(lua_State* l)
{
    currGameObjRunning->speed = lua_tonumber(l,1);
    return 0;
}
int L_IsPlayerChoosable(lua_State* l)
{
    currGameObjRunning->playerChoosable = lua_toboolean(l,1);
    return 0;
}
int L_GetTransitioningTo(lua_State* l)
{
    lua_pushnumber(l,transitioningTo);
    return 1;
}
int L_SetEncounterLoadScreen(lua_State* l)
{
    currEncounterRunning->loadScreen_spriteIndex = LoadSprite(lua_tostring(l,1),false);
    return 0;
}
int L_SetEncounterSelectScreenSprite(lua_State* l)
{
    currEncounterRunning->selectScreenSprite = LoadSprite(lua_tostring(l,1),false);
    return 0;
}

int L_KillObj(lua_State* l)
{
    int index = lua_tonumber(l,1);
    bool triggerEffects = lua_toboolean(l,2);
    if (index >= 0 && index < MAX_OBJS)
    {
        KillObj(&objects[index],triggerEffects,true);
    }
    return 0;
}
int L_GetObjectName(lua_State* l)
{
    int index = lua_tointeger(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* g = &objects[index];
        if (g->name)
        {
            lua_pushstring(l,g->name);
            return 1;
        }
    }
    lua_pushstring(l,"");
    return 1;
}
int L_SetObjCost(lua_State* l)
{
    int cost = lua_tonumber(l,1);
    currGameObjRunning->cost = cost;
    return 0;
}
int L_GetObjFriendliness(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    
    lua_pushnumber(l,GetPlayerOwnedBy(&objects[index]));
    return 1;
}
int L_GetOppositeFriendliness(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
    {
        ConsolePrintf("GetOppositeFriendliness: invalid index: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];
    int friendliness = GetPlayerOwnedBy(g);
    if (friendliness == TYPE_ENEMY)
    {
        lua_pushnumber(l,TYPE_FRIENDLY);
    }
    else if (friendliness == TYPE_FRIENDLY)
    {
        lua_pushnumber(l,TYPE_ENEMY);
    }
    else if (friendliness == TYPE_DECORATION)
    {
        lua_pushnumber(l,TYPE_DECORATION);
    }
    else
    {
        ConsolePrintf("GetOppositeFriendliness: PANIC UNKNOWN FRIENDLINESS\n");
    }
    return 1;

}
int L_GetAllObjsByFriendliness(lua_State* l)
{
    int friendliness = lua_tonumber(l,1);
    lua_newtable(l);
    int index = 0;
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];

        if (GetPlayerOwnedBy(g) == friendliness)
        {
            index++;
            lua_pushnumber(l,index);
            lua_pushnumber(l,g-objects);
            lua_settable(l,-3);
        }
    }
    return 1;

}
int L_SetChannelingSprite(lua_State* l)
{
    currGameObjRunning->channelingSpriteIndex = LoadSprite(lua_tostring(l,-1),true);
    return 0;
}
int L_GetControlGroup(lua_State* l)
{
    int index = lua_tonumber(l,1);
    index = clamp(index,0,10);
    lua_newtable(l);
    for (int i = 0; i < MAXUNITSSELECTED; i++)
    {
        GameObject* g = players[0].controlGroups[index][i];
        if (g)
        {
            lua_pushnumber(l,i+1);
            lua_pushnumber(l,g-objects);
            lua_settable(l,-3);
        }
    }
    return 1;
}
int L_SetCategory(lua_State* l)
{
    GAMEOBJ_TYPE_HINT category = lua_tointeger(l,1);
    currGameObjRunning->category =  category; 
    return 0;
}
int L_SetManaCost(lua_State* l)
{
    if (currAbilityRunning)
    {
        SetManaCost(currAbilityRunning, lua_tonumber(l,1));
    }
    return 0;
}
int L_SetManaRegen(lua_State* l)
{
    if (currGameObjRunning)
    {
        SetManaRegen(currGameObjRunning, lua_tonumber(l,1));
    }
    return 0;
}
int L_SetHPRegen(lua_State* l)
{
    if (currGameObjRunning)
    {
        SetHPRegen(currGameObjRunning, lua_tonumber(l,1));
    }
    return 0;

}
int L_HurtObj(lua_State* l)
{
     int index = lua_tonumber(l,1);
    float damage = lua_tonumber(l,2);

    if (index < 0 || index >= MAX_OBJS)
        return 0;
    
    if (Damage(currGameObjRunning,&objects[index],damage,false,0, NULL))
    {
        //PrintDiedFrom(&objects[index],currGameObjRunning,NULL,damage);
    }
    return 0;
}
int L_ShowString(lua_State* l)
{

    const char* str = lua_tostring(l,1);
    if (numStringsToDraw >= NUM_TEXT_DISPLAYS)
    {
        ConsolePrintf("Too many strings (%i) to draw '%s'\n",numStringsToDraw,str);
        return 0;
    }
    strncpy(textDisplays[numStringsToDraw].str,str,TEXT_DISPLAY_MAX_SIZE);
    textDisplays[numStringsToDraw].x = lua_tonumber(l,2);
    textDisplays[numStringsToDraw].y = lua_tonumber(l,3);
    textDisplays[numStringsToDraw].align = lua_tonumber(l,4);
    numStringsToDraw++;

    return 0;
}
int L_PlaySound(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    int ind = LoadSound(path);
    Sound* s = &sounds[ind];


    float x = lua_tonumber(l,3);
    float y = lua_tonumber(l,4);


    if (!lua_isnumber(l,3))
    {
        if (currGameObjRunning)
        {
            GetCentre(currGameObjRunning,&x,&y);
        }        
        else if (currAttackRunning)
        {
            x = currAttackRunning->x;
            y = currAttackRunning->y;
        }
        else
        {
            x = GetCameraMiddleX();
            y = GetCameraMiddleY();

        }
    }

    PlaySoundAtPosition(s,lua_tonumber(l,2), x,y,true);
    return 0;
}
int L_AddStack(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);
    int numStacksToAdd = lua_tonumber(l,3);

    if (objIndex < 0 || objIndex >= MAX_OBJS || abilityIndex < 0 || abilityIndex >= MAX_ABILITIES)  
        return 0;
    GameObject* obj = &objects[objIndex];
    Ability* a = &obj->abilities[abilityIndex];
    a->stacks += numStacksToAdd;
    if (a->stacks > a->maxStacks)
        a->stacks = a->maxStacks;
    if (a->stacks < 0)
        a->stacks = 0;
    return 0;
}
int L_GetStacks(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);

    GameObject* obj = &objects[objIndex];
    Ability* a = &obj->abilities[abilityIndex];
    lua_pushnumber(l,a->stacks);
    return 1;
}
int L_GetMaxStacks(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);

    GameObject* obj = &objects[objIndex];
    Ability* a = &obj->abilities[abilityIndex];
    lua_pushnumber(l,a->maxStacks);
    return 1;

}
int L_Normalize(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    Normalize(&x,&y);

    lua_newtable(l);

    lua_pushstring(l,"x");
    lua_pushnumber(l,x);
    lua_settable(l,-3);

    lua_pushstring(l,"y");
    lua_pushnumber(l,y);
    lua_settable(l,-3);

    return 1;

}
int L_GetUIHeight(lua_State* l)
{
    lua_pushnumber(l,GetUIStartHeight());
    return 1;
}
int L_SetBounty(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int bounty = lua_tonumber(l,2);
    GameObject* g = &objects[objIndex];
    if (objIndex < 0 || objIndex >= MAX_OBJS)
        return 0;
    g->bounty = bounty;
    lua_pushnumber(l,objIndex);
    return 1;
}
int L_CreateTicker(lua_State* l)
{
    float time = lua_tonumber(l,1);
    Attack* a = CreateAoE(0,0,"",0,0,time,true,0,1,0,0,NULL,NULL,currGameObjRunning);

    lua_pushnumber(l,a-attacks);
    return 1;
}
int L_ObjIsStunnable(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    bool stunnable = lua_toboolean(l,2);
    objects[index].objectIsStunnable = stunnable;
    return 0;
}
int L_SetStunTimer(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    objects[index].stunTimer = lua_tonumber(l,2);
    return 0; 
    
}
int L_GetNumBadEffects(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    lua_pushnumber(l,GetNumberOfBadEffects(&objects[index]));
    return 1;

}
int L_GetNumEffects(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    lua_pushnumber(l,GetNumberOfActiveEffects(&objects[index]));
    return 1;
}
int L_GetItemRef(lua_State* l)
{
    lua_pushnumber(l,currItemRunning-items);
    return 1;
}
int L_SetItemName(lua_State* l)
{
    int index = lua_tonumber(l,1);
    const char* str = lua_tostring(l,2);
    if (index < 0 || index >= numItems)
        return 0;
    Item* i = &items[index];
    i->name = calloc(strlen(str)+1,sizeof(char));
    strcpy(i->name,str);
    return 0;
}
int L_SetItemDescription(lua_State* l)
{
    int index = lua_tonumber(l,1);
    const char* str = lua_tostring(l,2);
    if (index < 0 || index >= numItems)
        return 0;
    Item* i = &items[index];
    i->description = calloc(strlen(str)+1,sizeof(char));
    strcpy(i->description,str);
    return 0;
}

int L_AddArmor(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[index];
    int armor = lua_tonumber(l,2);
    g->armor += armor;
    return 0;
}

//Debug function to be used in the console
//Creates object with default properties on the mouse position
int L_MObj(lua_State* l)
{

    GameObject* g;
    const char* l_path = lua_tostring(l,1);
    if (!l_path)
    {
        ConsolePrintf("CreateObject: Path null\n");
        return 0;    
    }
    MouseState m = GetMouseClamped();
    const int x = m.worldX;
    const int y = m.worldY;
    int PLAYER = lua_tonumber(l,2);
    GameObject* before = currGameObjRunning;
    bool prefabFound = false;

    GAMEOBJ_SOURCE source = SOURCE_SPAWNED_FROM_OBJ;

    for (int i = 0; i < numPrefabs; i++)
    {
        
        if (prefabs[i]->path && strcasecmp(prefabs[i]->path,l_path) == 0)
        {
            g = prefabs[i];
            prefabFound = true;
            break;
        }
    } 
    if (!prefabFound)
    {
        GameObject* prefab = LoadPrefab(l_path);
        g = AddGameobject(prefab,x,y,SOURCE_SPAWNED_FROM_MAP,PLAYER);
        SetOwnedBy(g, PLAYER);

    }
    else
    {
        g = AddGameobject(g,x,y,source,PLAYER);
        SetOwnedBy(g, PLAYER);
    }
    currGameObjRunning = before;
    return 0;
}
int L_Clamp(lua_State* l)
{
    float toClamp = lua_tonumber(l,1);
    float min = lua_tonumber(l,2);
    float max = lua_tonumber(l,3);

    lua_pushnumber(l,clamp(toClamp,min,max));
    return 1;
}
int L_IsInAttackRange(lua_State* l)
{
    int g1 = lua_tonumber(l,1);
    int g2 = lua_tonumber(l,2);

    if (GameObjectIndexInRange(g1) && GameObjectIndexInRange(g2))
    {
        GameObject* g = &objects[g1];
        GameObject* targ = &objects[g2];

        float range = GetAttackRange(g);

        lua_pushboolean(l,RectDist(g, targ) < GetAttackRange(g));
        return 1;
    }   
    else
    {
        ConsolePrintf("L_IsInAttackRange: index out of range: %i, %i\n",g1,g2);
        lua_pushboolean(l,false);
        return 1;
    }

}
int L_GetObjsByName(lua_State* l)
{
    const char* name = lua_tostring(l,1);
    if (!name)
    {
        ConsolePrintf("L_GetObjsByName: no string parameter supplied.\n");
        return 0;
    }
    lua_newtable(l);

    int numSelectedObjs = 0;
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        if (g->name && strcmp(g->name,name) == 0)
        {
            lua_pushnumber(l,numSelectedObjs+1);
            lua_pushnumber(l,g-objects);
            lua_settable(l,-3);

            numSelectedObjs++;
        }
    }
    return 1;
}
int L_Heal(lua_State* l)
{
    int index = lua_tonumber(l,1);
    double toHeal = lua_tonumber(l,2);
    if (!GameObjectIndexInRange(index))
    {
        ConsolePrintf("L_Heal: index out of range: %i",index);
        return 0;
    }
    Heal(&objects[index],toHeal);
    return 0;
}
int L_SetCanHitParent(lua_State* l)
{
    int atkIndex = lua_tonumber(l,1);
    bool canHit = lua_toboolean(l,2);

    if (atkIndex < 0 || atkIndex >= MAX_ATTACKS)
    {
        ConsolePrintf("L_SetCanHitParent: index out of range: %i\n",atkIndex);
        return 0;
    }
    attacks[atkIndex].canHitParent = canHit;
    return 0;
}
void SetGlobals(lua_State* l)
{
    //-- Enums -- 

    SetLuaKeyEnums(l);

    lua_pushinteger(l,0);
    lua_setglobal(l,"PLAYER");

    lua_pushinteger(l,1);
    lua_setglobal(l,"ENEMY");

    lua_pushinteger(l,_SCREEN_SIZE);
    lua_setglobal(l,"SCREEN_SIZE");

    lua_pushinteger(l,MAX_OBJS);
    lua_setglobal(l,"MAX_OBJS");


    lua_pushinteger(l,TRIGGER_TIMER);
    lua_setglobal(l,"TRIGGER_TIMER");

    lua_pushinteger(l,TRIGGER_ONATTACK);
    lua_setglobal(l,"TRIGGER_ONATTACK");

    lua_pushinteger(l,TRIGGER_ONHIT);
    lua_setglobal(l,"TRIGGER_ONHIT");

    lua_pushinteger(l,TRIGGER_INSTANT);
    lua_setglobal(l,"TRIGGER_INSTANT");

    lua_pushinteger(l,TRIGGER_CONST);
    lua_setglobal(l,"TRIGGER_CONST");

    lua_pushinteger(l,TRIGGER_PERMANENT);
    lua_setglobal(l,"TRIGGER_PERMANENT");



    lua_pushinteger(l,EFFECT_MAXHP);
    lua_setglobal(l,"EFFECT_MAXHP");

    lua_pushinteger(l,EFFECT_HURT);
    lua_setglobal(l,"EFFECT_HURT");

    lua_pushinteger(l,EFFECT_HEAL);
    lua_setglobal(l,"EFFECT_HEAL");

    lua_pushinteger(l,EFFECT_POSITION);
    lua_setglobal(l,"EFFECT_POSITION");

    lua_pushinteger(l,EFFECT_THREAT);
    lua_setglobal(l,"EFFECT_THREAT");

    lua_pushinteger(l,EFFECT_SPEED);
    lua_setglobal(l,"EFFECT_SPEED");

    lua_pushinteger(l,EFFECT_MISS_CHANCE);
    lua_setglobal(l,"EFFECT_MISS_CHANCE");

    lua_pushinteger(l,EFFECT_SHIELD);
    lua_setglobal(l,"EFFECT_SHIELD");

    lua_pushinteger(l,EFFECT_ATTACKDAMAGE);
    lua_setglobal(l,"EFFECT_ATTACKDAMAGE");

    lua_pushinteger(l,EFFECT_ATTACKSPEED);
    lua_setglobal(l,"EFFECT_ATTACKSPEED");

    lua_pushinteger(l,EFFECT_ATTACKRANGE);
    lua_setglobal(l,"EFFECT_ATTACKRANGE");

    lua_pushinteger(l,EFFECT_ADD_MANA);
    lua_setglobal(l,"EFFECT_ADD_MANA");

    lua_pushinteger(l,EFFECT_ARMOR);
    lua_setglobal(l,"EFFECT_ARMOR");

    lua_pushinteger(l,EFFECT_LIFESTEAL);
    lua_setglobal(l,"EFFECT_LIFESTEAL");

    lua_pushinteger(l,EFFECT_INVULN);
    lua_setglobal(l,"EFFECT_INVULN");

    lua_pushinteger(l,EFFECT_CURE);
    lua_setglobal(l,"EFFECT_CURE");

    lua_pushinteger(l,EFFECT_STUN);
    lua_setglobal(l,"EFFECT_STUN");

    lua_pushinteger(l,EFFECT_HURT_PERCENT);
    lua_setglobal(l,"EFFECT_HURT_PERCENT");

    lua_pushinteger(l,EFFECT_COOLDOWN_RATE);
    lua_setglobal(l,"EFFECT_COOLDOWN_RATE");

    lua_pushinteger(l,EFFECT_MAXMANA);
    lua_setglobal(l,"EFFECT_MAXMANA");

    lua_pushinteger(l,EFFECT_MANAREGEN);
    lua_setglobal(l,"EFFECT_MANAREGEN");

    lua_pushinteger(l,EFFECT_ABILITY_POTENCY);
    lua_setglobal(l,"EFFECT_ABILITY_POTENCY");

    lua_pushinteger(l,EFFECT_CLEAVE);
    lua_setglobal(l,"EFFECT_CLEAVE");

    lua_pushinteger(l,EFFECT_DODGE_CHANCE);
    lua_setglobal(l,"EFFECT_DODGE_CHANCE");

    lua_pushinteger(l,EFFECT_HEAL_PERCENT);
    lua_setglobal(l,"EFFECT_HEAL_PERCENT");


    lua_pushinteger(l,ABILITY_INSTANT);
    lua_setglobal(l,"ABILITY_INSTANT");
    lua_pushinteger(l,ABILITY_POINT);
    lua_setglobal(l,"ABILITY_POINT");
    lua_pushinteger(l,ABILITY_TARGET_FRIENDLY);
    lua_setglobal(l,"ABILITY_TARGET_FRIENDLY");
    lua_pushinteger(l,ABILITY_TARGET_ENEMY);
    lua_setglobal(l,"ABILITY_TARGET_ENEMY");
    lua_pushinteger(l,ABILITY_TARGET_ALL);
    lua_setglobal(l,"ABILITY_TARGET_ALL");
    lua_pushinteger(l,ABILITY_TOGGLE);
    lua_setglobal(l,"ABILITY_TOGGLE");

    lua_pushinteger(l,ABILITY_ANGLE);
    lua_setglobal(l,"ABILITY_ANGLE");


    lua_pushinteger(l,ATTACK_AOE);
    lua_setglobal(l,"ATTACK_AOE");
    lua_pushinteger(l,ATTACK_PROJECTILE_TARGETED);
    lua_setglobal(l,"ATTACK_PROJECTILE_TARGETED");
    lua_pushinteger(l,ATTACK_PROJECTILE_POINT);
    lua_setglobal(l,"ATTACK_PROJECTILE_POINT");
    lua_pushinteger(l,ATTACK_PROJECTILE_ANGLE);
    lua_setglobal(l,"ATTACK_PROJECTILE_ANGLE");
    lua_pushinteger(l,ATTACK_MELEE);
    lua_setglobal(l,"ATTACK_MELEE");

    lua_pushinteger(l,ATTACK_ACTIVE);
    lua_setglobal(l,"ATTACK_ACTIVE");
    lua_pushinteger(l,ATTACK_HITS_ENEMIES);
    lua_setglobal(l,"ATTACK_HITS_ENEMIES");
    lua_pushinteger(l,ATTACK_HITS_FRIENDLIES);
    lua_setglobal(l,"ATTACK_HITS_FRIENDLIES");

    lua_pushinteger(l,HINT_NONE);
    lua_setglobal(l,"HINT_NONE");
    lua_pushinteger(l,HINT_LINE);
    lua_setglobal(l,"HINT_LINE");
    lua_pushinteger(l,HINT_CIRCLE);
    lua_setglobal(l,"HINT_CIRCLE");
    lua_pushinteger(l,HINT_CONE);
    lua_setglobal(l,"HINT_CONE");


    lua_pushinteger(l,COLOR_DEFAULT);
    lua_setglobal(l,"COLOR_DEFAULT");

    lua_pushinteger(l,COLOR_BG);
    lua_setglobal(l,"COLOR_BG");

    lua_pushinteger(l,COLOR_FRIENDLY);
    lua_setglobal(l,"COLOR_FRIENDLY");

    lua_pushinteger(l,COLOR_ENEMY);
    lua_setglobal(l,"COLOR_ENEMY");

    lua_pushinteger(l,COLOR_GROUND);
    lua_setglobal(l,"COLOR_GROUND");
    
    lua_pushinteger(l,COLOR_POISON);
    lua_setglobal(l,"COLOR_POISON");

    lua_pushinteger(l,COLOR_HEAL);
    lua_setglobal(l,"COLOR_HEAL");

    lua_pushinteger(l,COLOR_DAMAGE);
    lua_setglobal(l,"COLOR_DAMAGE");

    lua_pushinteger(l,COLOR_SPEED);
    lua_setglobal(l,"COLOR_SPEED");
    
    lua_pushinteger(l,COLOR_SHIELD);
    lua_setglobal(l,"COLOR_SHIELD");

    lua_pushinteger(l,COLOR_FRIENDLY_POISON);
    lua_setglobal(l,"COLOR_FRIENDLY_POISON");

    lua_pushinteger(l,COLOR_FRIENDLY_HEAL);
    lua_setglobal(l,"COLOR_FRIENDLY_HEAL");

    lua_pushinteger(l,COLOR_FRIENDLY_DAMAGE);
    lua_setglobal(l,"COLOR_FRIENDLY_DAMAGE");

    lua_pushinteger(l,COLOR_FRIENDLY_SPEED);
    lua_setglobal(l,"COLOR_FRIENDLY_SPEED");

    lua_pushinteger(l,COLOR_FRIENDLY_SHIELD);
    lua_setglobal(l,"COLOR_FRIENDLY_SHIELD");


    lua_pushinteger(l,DITHER_NONE);
    lua_setglobal(l,"DITHER_NONE");
    lua_pushinteger(l,DITHER_FILL);
    lua_setglobal(l,"DITHER_FILL");
    lua_pushinteger(l,DITHER_HALF);
    lua_setglobal(l,"DITHER_HALF");
    lua_pushinteger(l,DITHER_QUARTER);
    lua_setglobal(l,"DITHER_QUARTER");
    lua_pushinteger(l,DITHER_EIGTH);
    lua_setglobal(l,"DITHER_EIGTH");
    
    lua_pushinteger(l,DITHER_VERTICAL_HALF);
    lua_setglobal(l,"DITHER_VERTICAL_HALF");

    lua_pushinteger(l,DITHER_VERTICAL_QUARTER);
    lua_setglobal(l,"DITHER_VERTICAL_QUARTER");
    lua_pushinteger(l,DITHER_VERTICAL_EIGTH);
    lua_setglobal(l,"DITHER_VERTICAL_EIGTH");

    lua_pushinteger(l,DITHER_HORIZONTAL_HALF);
    lua_setglobal(l,"DITHER_HORIZONTAL_HALF");
    lua_pushinteger(l,DITHER_HORIZONTAL_QUARTER);
    lua_setglobal(l,"DITHER_HORIZONTAL_QUARTER");
    lua_pushinteger(l,DITHER_HORIZONTAL_EIGTH);
    lua_setglobal(l,"DITHER_HORIZONTAL_EIGTH");

    lua_pushinteger(l,DITHER_STAR_HALF);
    lua_setglobal(l,"DITHER_STAR_HALF");
    lua_pushinteger(l,DITHER_STAR_QUARTER);
    lua_setglobal(l,"DITHER_STAR_QUARTER");
    lua_pushinteger(l,DITHER_STAR_EIGTH);
    lua_setglobal(l,"DITHER_STAR_EIGTH");
    
    lua_pushinteger(l,DITHER_HEAL_HALF);
    lua_setglobal(l,"DITHER_HEAL_HALF");
    lua_pushinteger(l,DITHER_HEAL_QUARTER);
    lua_setglobal(l,"DITHER_HEAL_QUARTER");
    lua_pushinteger(l,DITHER_HEAL_EIGTH);
    lua_setglobal(l,"DITHER_HEAL_EIGTH");

    lua_pushinteger(l,DITHER_POISON_HALF);
    lua_setglobal(l,"DITHER_POISON_HALF");
    lua_pushinteger(l,DITHER_POISON_QUARTER);
    lua_setglobal(l,"DITHER_POISON_QUARTER");
    lua_pushinteger(l,DITHER_POISON_EIGTH);
    lua_setglobal(l,"DITHER_POISON_EIGTH");

    lua_pushinteger(l,DITHER_SPEED_HALF);
    lua_setglobal(l,"DITHER_SPEED_HALF");
    lua_pushinteger(l,DITHER_SPEED_QUARTER);
    lua_setglobal(l,"DITHER_SPEED_QUARTER");
    lua_pushinteger(l,DITHER_SPEED_EIGTH);
    lua_setglobal(l,"DITHER_SPEED_EIGTH");

    lua_pushinteger(l,DITHER_SHIELD_HALF);
    lua_setglobal(l,"DITHER_SHIELD_HALF");
    lua_pushinteger(l,DITHER_SHIELD_QUARTER);
    lua_setglobal(l,"DITHER_SHIELD_QUARTER");
    lua_pushinteger(l,DITHER_SHIELD_EIGTH);
    lua_setglobal(l,"DITHER_SHIELD_EIGTH");


    lua_pushinteger(l,DITHER_DAMAGE_HALF);
    lua_setglobal(l,"DITHER_DAMAGE_HALF");
    lua_pushinteger(l,DITHER_DAMAGE_QUARTER);
    lua_setglobal(l,"DITHER_DAMAGE_QUARTER");
    lua_pushinteger(l,DITHER_DAMAGE_EIGTH);
    lua_setglobal(l,"DITHER_DAMAGE_EIGTH");

    lua_pushinteger(l,DITHER_ATTACK_DAMAGE_HALF);
    lua_setglobal(l,"DITHER_ATTACK_DAMAGE_HALF");
    lua_pushinteger(l,DITHER_ATTACK_DAMAGE_QUARTER);
    lua_setglobal(l,"DITHER_ATTACK_DAMAGE_QUARTER");
    lua_pushinteger(l,DITHER_ATTACK_DAMAGE_EIGTH);
    lua_setglobal(l,"DITHER_ATTACK_DAMAGE_EIGTH");


    lua_pushinteger(l,TYPE_ALL);
    lua_setglobal(l,"TYPE_ALL");
    lua_pushinteger(l,TYPE_TANK);
    lua_setglobal(l,"TYPE_TANK");
    lua_pushinteger(l,TYPE_HEALER);
    lua_setglobal(l,"TYPE_HEALER");
    lua_pushinteger(l,TYPE_MELEEDPS);
    lua_setglobal(l,"TYPE_MELEEDPS");
    lua_pushinteger(l,TYPE_RANGEDDPS);
    lua_setglobal(l,"TYPE_RANGEDDPS");
    lua_pushinteger(l,TYPE_UTILITY);
    lua_setglobal(l,"TYPE_UTILITY");
    
    lua_pushinteger(l,TYPE_DECORATION);
    lua_setglobal(l,"TYPE_DECORATION");
    lua_pushinteger(l,TYPE_ENEMY);
    lua_setglobal(l,"TYPE_ENEMY");
    lua_pushinteger(l,TYPE_FRIENDLY);
    lua_setglobal(l,"TYPE_FRIENDLY");

    lua_pushinteger(l,DIFFICULTY_EASY);
    lua_setglobal(l,"DIFFICULTY_EASY");
    lua_pushinteger(l,DIFFICULTY_MEDIUM);
    lua_setglobal(l,"DIFFICULTY_MEDIUM");
    lua_pushinteger(l,DIFFICULTY_HARD);
    lua_setglobal(l,"DIFFICULTY_HARD");
    lua_pushinteger(l,DIFFICULTY_IMPOSSIBLE);
    lua_setglobal(l,"DIFFICULTY_IMPOSSIBLE");

   lua_pushinteger(l,GAMESTATE_MAIN_MENU);
    lua_setglobal(l,"GAMESTATE_MAIN_MENU");
    lua_pushinteger(l,GAMESTATE_CHOOSING_ENCOUNTER);
    lua_setglobal(l,"GAMESTATE_CHOOSING_ENCOUNTER");
    lua_pushinteger(l,GAMESTATE_CHOOSING_UNITS);
    lua_setglobal(l,"GAMESTATE_CHOOSING_UNITS");
    lua_pushinteger(l,GAMESTATE_INGAME);
    lua_setglobal(l,"GAMESTATE_INGAME");
    lua_pushinteger(l,GAMESTATE_EXIT);
    lua_setglobal(l,"GAMESTATE_EXIT");

    lua_pushinteger(l,DIALOGUE_H);
    lua_setglobal(l,"DIALOGUE_H");
    lua_pushinteger(l,DIALOGUE_W);
    lua_setglobal(l,"DIALOGUE_W");
    lua_pushinteger(l,DIALOGUE_X);
    lua_setglobal(l,"DIALOGUE_X");
    lua_pushinteger(l,DIALOGUE_Y);
    lua_setglobal(l,"DIALOGUE_Y");

    lua_pushinteger(l,COMMAND_NONE);
    lua_setglobal(l,"COMMAND_NONE");
    lua_pushinteger(l,COMMAND_MOVE);
    lua_setglobal(l,"COMMAND_MOVE");
    lua_pushinteger(l,COMMAND_ATTACK);
    lua_setglobal(l,"COMMAND_ATTACK");
    lua_pushinteger(l,COMMAND_CAST);
    lua_setglobal(l,"COMMAND_CAST");
    lua_pushinteger(l,COMMAND_STOP);
    lua_setglobal(l,"COMMAND_STOP");
    lua_pushinteger(l,COMMAND_ATTACKMOVE);
    lua_setglobal(l,"COMMAND_ATTACKMOVE");

    lua_pushinteger(l,DIALOGUE_X);
    lua_setglobal(l,"DIALOGUE_X");
    lua_pushinteger(l,DIALOGUE_Y);
    lua_setglobal(l,"DIALOGUE_Y");
    lua_pushinteger(l,DIALOGUE_W);
    lua_setglobal(l,"DIALOGUE_W");
    lua_pushinteger(l,DIALOGUE_H);
    lua_setglobal(l,"DIALOGUE_H");

    lua_pushboolean(l,_DEBUG);
    lua_setglobal(l,"_DEBUG");

    lua_pushinteger(l,LEVEL_POOR);
    lua_setglobal(l,"LEVEL_POOR");
    lua_pushinteger(l,LEVEL_MID);
    lua_setglobal(l,"LEVEL_MID");
    lua_pushinteger(l,LEVEL_HIGH);
    lua_setglobal(l,"LEVEL_HIGH");

    lua_pushinteger(l,ALLEGRO_ALIGN_LEFT);
    lua_setglobal(l,"ALIGN_LEFT");
    lua_pushinteger(l,ALLEGRO_ALIGN_CENTER);
    lua_setglobal(l,"ALIGN_CENTER");
    lua_pushinteger(l,ALLEGRO_ALIGN_RIGHT);
    lua_setglobal(l,"ALIGN_RIGHT");


}
int L_SetItemIcon(lua_State* l)
{
    const int item = lua_tonumber(l,1);
    const char* path = lua_tostring(l,2);
    if (item < 0 || item >= numItems)
        return 0;
    items[item].spriteIndex_Icon = LoadSprite(path,true);
    return 0;
}
int L_SetItemGoldCost(lua_State* l)
{
    const int item = lua_tonumber(l,1);
    const int cost = lua_tonumber(l,2);
    if (item < 0 || item >= numItems)
        return 0;
    items[item].goldCost = cost;
    return 0;

}
int L_SetGamestateToMainMenu(lua_State* l)
{
    SetGameStateToInMenu();
    return 0;
}

int L_CleaveEffect(lua_State* l)
{
    int primary = lua_tonumber(l,1);
    int numCleaves = lua_tonumber(l,3);
    int range = lua_tonumber(l,4);
    float cleavePercent = lua_tonumber(l,5);
    float percentToMainTarget = lua_tonumber(l,6);

    //lua_pop(l,-1);
    //lua_pop(l,-1);



    GameObject** targets = calloc(numCleaves+1,sizeof(GameObject*));
    int numTargets = 1;

    GameObject* mainObj = &objects[primary];


    targets[0] = mainObj;
    
    for (int i = 0; i < numCleaves; i++)
    {
        for (int j = 0; j < numActiveObjects; j++)
        {
            bool ignore = false;
            //ignore targets already on the list or of a different friendliness
            for (int z = 0; z < numTargets; z++)
            {
                if (targets[z] == activeObjects[j] )
                {
                    ignore = true;
                    break;
                }
                if (GetPlayerOwnedBy(activeObjects[j]) != GetPlayerOwnedBy(mainObj))
                {
                    ignore = true;
                    break;
                }

            }
            if (!ignore)
            {
                if (GetDist(activeObjects[j],mainObj) <= range)
                {
                    targets[numTargets] = activeObjects[j];
                    numTargets++;
                    break;
                }
            }
        }
    }

    size_t len =  lua_rawlen(l,2);

    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 2, i,currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        for (int j = 0; j  < numTargets; j++)
        {
            Effect eCopy = e;
            //targets other than the main
            if (j != 0)
            {
                eCopy.value *= cleavePercent;
            }
            if (j == 0)
            {
                eCopy.value *= percentToMainTarget;
            }
            ApplyEffect(&eCopy,currGameObjRunning,targets[j],currAbilityRunning);
        }
        lua_pop(l,1);
    }       


    free(targets);
    return 0;

}
int L_ApplyEffect(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0) 
    {
        ConsolePrintf("Could not apply effect from %s: 'obj' index out of range. Obj index is %i.",currAbilityRunning->path ? currAbilityRunning->path : "[No path]",objIndex);
        return 0; 
    }
    if (objIndex >= MAX_OBJS) 
    {
        ConsolePrintf("Could not apply effect from %s: 'obj' index out of range. Obj index is %i.",currAbilityRunning->path ? currAbilityRunning->path : "[No path]",objIndex);
        return 0;
    }
    GameObject* g = &objects[objIndex];

    size_t len =  lua_rawlen(l,2);

    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 2, i, currGameObjRunning);
        e.from = currGameObjRunning;
        e.abilityFrom = currAbilityRunning;
        //lua_remove(l,-1);
        ProcessItemsOnApplyEffect(currGameObjRunning,&objects[objIndex],&e);
        ApplyEffect(&e,currGameObjRunning,&objects[objIndex],currAbilityRunning);
        lua_pop(l,1);
    }       
    
    return 0;
}
int L_DealDamage(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0) return -1; 
    if (objIndex >= MAX_OBJS) return -1;

    int dmg = lua_tonumber(l,2);
    
    if (Damage(currGameObjRunning,&objects[objIndex],dmg,false,0, NULL))
    {
        //PrintDiedFrom(&objects[objIndex],currGameObjRunning,NULL,dmg);
    }
    return 1;
}
int L_ClearCommandQueue(lua_State* l)
{
    GameObject* g = &objects[(int)lua_tonumber(l,1)];
    ClearCommandQueue(g);
    return 0;
}
int L_SetObjectPush(lua_State* l)
{   
    bool b = lua_toboolean(l,1);
    if (b)
        currGameObjRunning->properties |= OBJ_CAN_PUSH;
    else
        currGameObjRunning->properties &= ~OBJ_CAN_PUSH;
    return 0;
}
int L_IsInCombat(lua_State* l)
{
    lua_pushboolean(l,IsInCombat(currGameObjRunning));
    return 1;
}
int L_ObjIsValidIndex(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
        lua_pushboolean(l,true);
    else
        lua_pushboolean(l,false);
    return 1;

}
int L_IsAlive(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        lua_pushboolean(l,IsActive(&objects[index]));
    }
    else
    {
        lua_pushboolean(l,false);
    }
    return 1;
}
int L_SetObjChannelling(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* obj = &objects[index];
    float time = lua_tonumber(l,2);

    float x; float y;
    GetCentre(currGameObjRunning,&x,&y);
    SetObjChannelling(currGameObjRunning,NULL,time,x,y,currGameObjRunning,0,0);
    return 0;
}
int L_ObjIsChannelling(lua_State* l)
{

    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    GameObject* obj = &objects[index];
    lua_pushboolean(l,ObjIsChannelling(obj));
    return 1;
}
int L_SetAttackInactive(lua_State* l)
{
    int atk = lua_tonumber(l,1);
    float length = lua_tonumber(l,2);

    if (atk < 0 || atk >= MAX_ATTACKS)
    {
        ConsolePrintf("L_SetAttackInactive: invalid index: %i\n",atk);
        return 0;
    }
    attacks[atk].inactiveFor = length;

    return 0;

}
int L_GetAttackInactive(lua_State* l)
{
    int atk = lua_tonumber(l,1);

    if (atk < 0 || atk >= MAX_ATTACKS)
    {
        ConsolePrintf("L_GetAttackInactive: invalid index: %i\n",atk);
        return 0;
    }
    lua_pushnumber(l,attacks[atk].inactiveFor);

    return 1;

}

int L_CastAbility(lua_State* l)
{

    if (ObjIsChannelling(currGameObjRunning))
    {
        lua_pushboolean(l,false);
        return 1;
    }
    int index = lua_tonumber(l,1);
        
    if (index < 0 || index >= MAX_ABILITIES)
    {
        lua_pushboolean(l,false);
        ConsolePrintf("L_CastAbility: Ability index out of range: %i\n");
        return 1;
    }

    Ability* ability = &currGameObjRunning->abilities[index];
    if (ability->stacks == 0 && AbilityIsOnCooldown(ability))//.cdTimer > 0.001f)
    {
        lua_pushboolean(l,false);
        return 1;
    }
    if (!AbilityIsInitialised(ability))
    {
        lua_pushboolean(l,false);
        return 1;
    }
    float channelTime = lua_tonumber(l,2);

    size_t len =  lua_rawlen(l,3);

    float x = -FLT_MAX; float y=-FLT_MAX; int obj = -1; float headingx=-1; float headingy=-1;
    GetCentre(currGameObjRunning,&x,&y);
    bool isAField;
    bool xIsPresent; bool yIsPresent;
    for (int i = 0; i < len; i++)
    {
        lua_pushnumber(l,i+1);
        lua_gettable(l,3);

        x = GetTableField(l,-1,"x",&xIsPresent);
        y = GetTableField(l,-1,"y",&yIsPresent);
        obj = GetTableField(l,-1,"target",&isAField);
        headingx = GetTableField(l,-1,"headingx",&isAField);
        headingy = GetTableField(l,-1,"headingy",&isAField);
    }
    GameObject* target = obj > -1 ? &objects[obj] : NULL;
    if (target)
    {
        if (GetDist(currGameObjRunning,target) > ability->range)
        {
            ConsolePrintf("L_CastAbility: out of range: %s\n",ability->path);
            lua_pushboolean(l,false);
            return 0;
        }
    }
    //on the ground
    else if (xIsPresent && yIsPresent)
    {
        float x1; float y1;
        GetCentre(currGameObjRunning,&x1,&y1);
        if (dist(x1,y1,x,y) > ability->range)
        {
            lua_pushboolean(l,false);
            return 0;
        }
    }
    //otherwise: may be xheading/yheading which does not need to be checked
    if (!xIsPresent)
        GetCentre(currGameObjRunning,&x,NULL);
    if (!yIsPresent)
        GetCentre(currGameObjRunning,NULL,&y);

    if (channelTime > 0)
    {
        //GameObject* target;
        if (obj >= 0 && obj < MAX_OBJS)
        {
            target = &objects[obj];
        }
        else
            target = NULL;
        Ability* a = &currGameObjRunning->abilities[index];
        if (AbilityShouldBeCastOnTarget(a) && !target)
        {
            lua_pushboolean(l,false);
            return 1;
        }
        SetObjChannelling(currGameObjRunning,a,channelTime,x,y,target,headingx,headingy);
        lua_pushboolean(l,true);
        return 1;

    }
    else
    {
        GameObject* g = NULL;
        if (obj >= 0 && obj < MAX_OBJS)
            g = &objects[obj];
        
        lua_pushboolean(l,CastAbility(currGameObjRunning,&currGameObjRunning->abilities[index],x,y,headingx,headingy,g));
        
    }
    return 1;
}
int L_GoShop(lua_State* l)
{
    currEncounterRunning->goingToShop = lua_toboolean(l,1);
    return 0;
}
int L_SetEnraged(lua_State* l)
{
    int obj = lua_tonumber(l,1);
    if (obj < 0 || obj >= MAX_OBJS)
        return 0;
    bool b = lua_toboolean(l,2);

    objects[obj].enraged = b;
    return 0;
}
int L_RandRange(lua_State* l)
{
    double min = lua_tonumber(l,1);
    double max = lua_tonumber(l,2);
    lua_pushnumber(l,RandRange(min,max));
    return 1;
}
int L_RandRangeI(lua_State* l)
{
    double min = lua_tonumber(l,1);
    double max = lua_tonumber(l,2);
    lua_pushinteger(l,RandRangeI(min,max));
    return 1;
}
int L_AbilitySetHintColor(lua_State* l)
{
    if (currGameObjRunning && currAbilityRunning)
    {
        int col = lua_tointeger(l,1);
        currAbilityRunning->hintColor = EffectColors[col];
        currAbilityRunning->hintColorSet = true;
    }
    return 0;
}
int L_SetAbilityHint(lua_State* l)
{
    if (!currAbilityRunning)
    {
        ConsolePrintf("SetAbilityHint needs to be called from an ability.\n");
        return 0;
    }
    currAbilityRunning->targetingHint = lua_tonumber(l,1);
    if (lua_isnumber(l,2))
    {
        currAbilityRunning->hintRadius = lua_tonumber(l,2);
    }
    else
    {
        currAbilityRunning->hintRadius = 30;
    }

    if (lua_isboolean(l,3))
    {
        currAbilityRunning->hintSoak = lua_toboolean(l,3);
    }
    else
    {
        currAbilityRunning->hintSoak = false;
    }
    if (lua_isnumber(l,4))
    {
        currAbilityRunning->hintLength = lua_tonumber(l,4);
    }
    else
    {
        currAbilityRunning->hintLength = 0;
    }

    return 0;
}
int L_ChangeMap(lua_State* l)
{
    //if we're already in the shop

    const char* path = lua_tostring(l,1);
    ChangeMap(path);

    return 0;
}
int L_Round(lua_State* l)
{
    float num = lua_tonumber(l,1);
    int rounded = round(num);
    lua_pushinteger(l,rounded);

    return 1;
}
int L_AddAbility(lua_State* l)
{
    GameObject* g;
    if (_LOADING_PREFAB)
        g = currGameObjRunning;
    else
    {
        int objIndex = lua_tonumber(l,1);
        if (objIndex < 0 || objIndex >= MAX_OBJS)
            return 0;
        g = &objects[objIndex];
    }
    const char* path = lua_tostring(l,2);
    int index = lua_tonumber(l,3);
    if (index < 0) index = 0; 
    if (index >= MAX_ABILITIES) index = MAX_ABILITIES-1;

    //if we're adding the same ability twice in a row don't add it
    bool shouldAdd = true;
    if (g->abilities[index].path)
    {
        if (strcasecmp(g->abilities[index].path,path) == 0)
            shouldAdd = false;
    }
    if (shouldAdd)
    {
        if (g->abilities[index].path)
        {
            RemoveAbility(&g->abilities[index],g);
        }
        Ability* prefab = AddAbility(path); 
        if (!g->abilities[index].path)
        {
        }
        g->numAbilities = _MAX(g->numAbilities,index+1);

        //g->abilities[index] = CloneAbilityPrefab(prefab,l);
        CloneAbilityPrefab(prefab,l,&g->abilities[index],g);
        
    }
    
    lua_pushnumber(l,index);

    return 1;
}
int L_MoveAttack(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_ATTACKS)
    {
        Attack* a = &attacks[index];
        float x = lua_tonumber(l,2);
        float y = lua_tonumber(l,3);

        if (a)
        {

            a->x = x;
            a->y = y;
        }
        if (a->attackType == ATTACK_SHAPE)
        {
            a->shape.x = x;
            a->shape.y = y;
            for (int i = 0; i < a->shape.numCutOutAreas; i++)
            {
                a->shape.cutoutAreas[i].x = x;
                a->shape.cutoutAreas[i].y = y;
            }

        }
    }
    return 0;
}
int L_Bor(lua_State* l)
{
    int i = lua_tointeger(l,1);
    int j = lua_tointeger(l,2);
    int result = i | j;
    lua_pushnumber(l,result);
    return 1;
}
int L_Band(lua_State* l)
{
    int i = lua_tointeger(l,1);
    int j = lua_tointeger(l,2);
    int result = i & j;
    lua_pushnumber(l,result);
    return 1;
}
int L_Bxor(lua_State* l)
{
    int i = lua_tointeger(l,1);
    int j = lua_tointeger(l,2);
    int result = i ^ j;
    lua_pushnumber(l,result);
    return 1;
}
int L_Bnot(lua_State* l)
{
    int i = lua_tointeger(l,1);
    int result = ~i;
    lua_pushnumber(l,result);
    return 1;
}


int L_AbilitySetPortrait(lua_State* l)
{
    if (!currAbilityRunning)
    {   
        ConsolePrintf("L_AbilitySetPortrait: not being called from an ability.");
        return 0;
    }
    currAbilityRunning->spriteIndex_Portrait = LoadSprite(lua_tostring(l,-1),true);
    return 1;   
}
int L_RemoveFromEncountersList(lua_State* l)
{
    if (!currEncounterRunning)
    {   
        ConsolePrintf("L_AbilitySetPortrait: not being called from an ability.");
        return 0;
    }

    currEncounterRunning->encounterShouldBeSkipped = true;
    return 0;
}
int L_SetEncounterMapPath(lua_State* l)
{
    const char* str = lua_tostring(l,1);
    currEncounterRunning->mapPath = calloc(strlen(str)+1,sizeof(char));
    strcpy(currEncounterRunning->mapPath,str);
    return 0;
}
int L_SetEncounterDifficulty(lua_State* l)
{
    currEncounterRunning->difficulty = lua_tonumber(l,1);
    return 0;
}
int L_GetW(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        lua_pushnumber(l,GetWidth(&objects[index]));
    }
    else    
    {
        lua_pushnumber(l,0);
    }
    return 1;
}
int L_GetH(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        lua_pushnumber(l,GetHeight(&objects[index]));
    }
    else    
    {
        lua_pushnumber(l,0);
    }
    return 1;
}
int L_GetKey(lua_State* l)
{
    int index = lua_tonumber(l,1);
    
    if (KeyDown(keyStateLua,index))
    {
        lua_pushboolean(l,true);
    }
    else
    {
        lua_pushboolean(l,false);
    }
    return 1;
}
int L_SetLifetime(lua_State* l)
{
    int index = lua_tonumber(l,1);
    float time = lua_tonumber(l,2);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* g = &objects[index];
        g->deathTimerActivated = time >= 0;
        g->deathTimer = time;
    }
    return 0;
}
int L_GetLifetime(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index))
    {
        ConsolePrintf("L_GetLifetime: index out of range: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];
    lua_pushnumber(l,g->deathTimer) ;
    return 1;
}
int L_DeathTimerIsSet(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index))
    {
        ConsolePrintf("L_DeathTimerSet: index out of range: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];
    lua_pushboolean(l,g->deathTimerActivated) ;
    return 1;
}


int L_CopyObject(lua_State* l)
{
    int index = lua_tonumber(l,1);
    int x = lua_tonumber(l,2);
    int y = lua_tonumber(l,3);

    GameObject* g = NULL;

    if (index >= 0 && index < MAX_OBJS)
    {
        g = &objects[index]; 
    }
    if (g)
    {
        GameObject* g2 = AddGameobject(g,x,y,SOURCE_SPAWNED_FROM_OBJ,GetPlayerOwnedBy(g));
        for (int i = 0; i < MAX_ABILITIES; i++)
        {
            g2->abilities[i].cdTimer = g->abilities[i].cdTimer;
        }
        lua_pushnumber(l,g2-objects);
        return 1;
    }
    return 0;
}
int L_SetSpawnPoint(lua_State* l)
{
    int x = lua_tonumber(l,1);
    int y = lua_tonumber(l,2);

    currMap->spawnPoint.x = x;
    currMap->spawnPoint.y = y;

    return 0;
}
int L_RotatePoint(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);

    float cx = lua_tonumber(l,3);
    float cy = lua_tonumber(l,4);

    float angle = lua_tonumber(l,5);

    RotatePointF(&x,&y,cx,cy,DegToRad(angle));
    lua_newtable(l);
    lua_pushstring(l,"x");
    lua_pushnumber(l,x);
    lua_settable(l,-3);

    lua_pushstring(l,"y");
    lua_pushnumber(l,y);
    lua_settable(l,-3);

    return 1;
}
int L_CureNamedEffect(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    const char* name = lua_tostring(l,2);
    if (!name) 
        return 0;
    int numberToRemove = lua_tonumber(l,3);

    GameObject* g;
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    g = &objects[index];
    int numCured = CureNamedEffect(g, name, numberToRemove);
    lua_pushnumber(l,numCured);
    return 1;
}
int L_GetAbilityStacks(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);

    if (objIndex < 0 || objIndex >= MAX_OBJS || abilityIndex < 0 || abilityIndex >= MAX_ABILITIES)
        return 0;
    lua_pushnumber(l,objects[objIndex].abilities[abilityIndex].stacks);
    return 1;
    

}
int L_AddItem(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    const char* path = lua_tostring(l,2);

    if (objIndex < 0 || objIndex >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[objIndex];

        Item* i = LoadItem(path,l);
    AttachItem(g,i);
    return 0;
}
int L_RemoveItem(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int itemIndex = lua_tonumber(l,2);

    if (!GameObjectIndexInRange(objIndex) || itemIndex < 0 || itemIndex >= INVENTORY_SLOTS)
    {
        ConsolePrintf("L_RemoveItem: index out of range: %i,%i\n",objIndex,itemIndex);
        return 0;
    }
    RemoveItem(&objects[objIndex].inventory[itemIndex],&objects[objIndex]);
    return 0;

}

int L_SetGoldCost(lua_State* l)
{
    int itemIndex = lua_tonumber(l,1);
    if (itemIndex < 0 || itemIndex >= numItems)
        return 0;
    int cost = lua_tonumber(l,2);
    items[itemIndex].goldCost = cost;
    return 0;
}
int L_SetItemTier(lua_State* l)
{
    int itemIndex = lua_tonumber(l,1);
    if (itemIndex < 0 || itemIndex >= numItems)
        return 0;
    int tier = lua_tonumber(l,2);
    tier = clamp(tier,0,NUM_ITEM_POOLS-1);
    items[itemIndex].itemTier = tier;

    return 0;
}
int L_AttackIsActive(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index > MAX_ATTACKS)
    {
        ConsolePrintf("L_AttackIsActive: index out of range: %i\n",index);
        lua_pushboolean(l,false);
        return 1;
    }
    lua_pushboolean(l,AttackIsActive(&attacks[index]));
    return 1;
}
int L_ModifyAbilityCooldownTimer(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);

    if (objIndex < 0 || objIndex >= MAX_OBJS || abilityIndex < 0 || abilityIndex >= MAX_ABILITIES)
        return 0;
    Ability* a = &objects[objIndex].abilities[abilityIndex];
    ModifyAbilityCooldownTimer(a,lua_tonumber(l,3));
    
    return 1;

}
int L_SetCooldownRate(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(objIndex))
    {
        ConsolePrintf("L_SetCooldownRate: index out of range: %i\n",objIndex);
        return 0;
    }
    objects[objIndex].cooldownRate = lua_tonumber(l,2);
    return 0;

}
int L_GetAttackPosition(lua_State* l)
{
    int atk = lua_tonumber(l,1);
    if (atk < 0 || atk >= MAX_ATTACKS)
        return 0;
    lua_newtable(l);

    Attack* a = &attacks[atk];
    lua_pushstring(l,"x");
    lua_pushnumber(l,a->x);
    lua_settable(l,-3);

    lua_pushstring(l,"y");
    lua_pushnumber(l,a->y);
    lua_settable(l,-3);

    return 1;

}
int L_GetDist(lua_State* l)
{
    int objOne = lua_tonumber(l,1);
    int objTwo = lua_tonumber(l,2);

    if (objOne < 0 || objOne >= MAX_OBJS || objTwo < 0 || objTwo >= MAX_OBJS)
    {
        ConsolePrintf("L_GetDist: index out of range: %i, %i\n",objOne,objTwo);
        lua_pushnumber(l,-1);
        return 1;
    }
    float dist = GetDist(&objects[objOne],&objects[objTwo]);
    lua_pushnumber(l,dist);
    return 1;

}
int L_DistXY(lua_State* l)
{
    float x1 = lua_tonumber(l,1);
    float y1 = lua_tonumber(l,2);
    float x2 = lua_tonumber(l,3);
    float y2 = lua_tonumber(l,4);

    lua_pushnumber(l,dist(x1,y1,x2,y2));
    return 1;
}
int L_HasEffect(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    const char* effectName = lua_tostring(l,2);
    if (!GameObjectIndexInRange(objIndex))
    {
        ConsolePrintf("L_HasEffect: index out of range: %i\n",objIndex);
        return 0;
    }
    GameObject* g = &objects[objIndex];
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (g->effects[i].name && strcmp(effectName,g->effects[i].name) == 0)
        {
            lua_pushboolean(l,true);
            return 1;
        }
    }
    lua_pushboolean(l,false);
    return 1;
}
int L_GetEffects(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0 || objIndex >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[objIndex];
    lua_newtable(l);
    int index = 1;
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];
        if (!e->enabled)
            continue;

        lua_pushnumber(l,index);
        lua_newtable(l);

        lua_pushnumber(l, e->trigger);
        lua_setfield(l, -2, "trigger");

        lua_pushnumber(l, e->effectType);
        lua_setfield(l, -2, "effectType");

        if (e->from)
        {
            lua_pushnumber(l, e->from-objects);
            lua_setfield(l, -2, "from");
        }
        if (e->name)
        {
            lua_pushstring(l, e->name);
            lua_setfield(l, -2, "name");
        }
        if (e->description)
        {
            lua_pushstring(l, e->description);
            lua_setfield(l, -2, "description");
        }
        lua_pushboolean(l, e->canStack);
        lua_setfield(l, -2, "canStack");

        lua_pushnumber(l, e->stacks);
        lua_setfield(l, -2, "stacks");

        lua_pushnumber(l, e->maxStacks);
        lua_setfield(l, -2, "maxStacks");

        lua_pushnumber(l, e->value);
        lua_setfield(l, -2, "value");

        lua_pushnumber(l, e->triggersPerSecond);
        lua_setfield(l, -2, "triggersPerSecond");

        lua_pushnumber(l, e->numTriggers);
        lua_setfield(l, -2, "numTriggers");

        lua_pushnumber(l, e->timer);
        lua_setfield(l, -2, "timer");

        lua_pushnumber(l, e->duration);
        lua_setfield(l, -2, "duration");

        lua_pushnumber(l, e->duration - e->timer);
        lua_setfield(l, -2, "durationLeft");


        lua_pushnumber(l, e->tickTime);
        lua_setfield(l, -2, "tickTime");

        lua_pushnumber(l, e->spriteIndex_Portrait);
        lua_setfield(l, -2, "spriteIndex_Portrait");

        lua_pushnumber(l, e->overwrites);
        lua_setfield(l, -2, "overwrites");

        lua_settable(l,-3);

        index++;

    }
    return 1;
}
int L_GetMapWidth(lua_State* l)
{
    lua_pushnumber(l,GetMapWidth());
    return 1;
}
int L_GetMapHeight(lua_State* l)
{
    lua_pushnumber(l,GetMapHeight());
    return 1;
}
int L_StartsUnlocked(lua_State* l)
{
    currEncounterRunning->unlocked = lua_toboolean(l,1);
    return 0;
}
int L_UnlockEncounter(lua_State* l)
{
    const char* path = (const char*)lua_tostring(l,1);
    UnlockEncounter(path,0,0,0,0,false);

    return 0;
}
int L_SetUnlockLevelPrice(lua_State* l)
{
    currEncounterRunning->unlockBasePrice = lua_tonumber(l,1);
    return 0;
}
int L_AddGold(lua_State* l)
{
    int gold = lua_tonumber(l,1);
    if (gameState == GAMESTATE_INGAME)
    {
        AddGold(gold);
    }
    else
    {
        players[0].bankedGold += gold;
        AddGold(gold);

    }
    return 0;
}
int L_GetCompletionPercent(lua_State* l)
{
    lua_pushnumber(l,currMap->percentComplete);
    return 1;
}
int L_SetCompletionPercent(lua_State* l)
{
    int amtToAdd = lua_tonumber(l,1) - currMap->percentComplete; 
    AddCompletionPercent(amtToAdd);
    return 0;
}
int L_AnyObjInCombat(lua_State* l)
{
    for (int i = 0; i < numActiveObjects; i++)
    {
        if (IsInCombat(activeObjects[i]))
        {
            lua_pushboolean(l,true);
            return 1;
        }
    }
    lua_pushboolean(l,false);
    return 1;
}
int L_RemoveFromCount(lua_State* l)
{
    int index = lua_tonumber(l,1);
    bool b = lua_toboolean(l,2);

    if (index < 0 || index >= MAX_OBJS)
    {
        ConsolePrintf("L_RemoveFromCount: invalid index: %i\n",index);
        return 0;
    }
    objects[index].isRemovedFromCount = b;
    return 0;
}
int L_GetTargetingHint(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);   

    if (!GameObjectIndexInRange(objIndex) || !AbilityIndexInRange(abilityIndex))
    {
        ConsolePrintf("L_GetTargetingHint: index not in range: %i, %i\n",objIndex,abilityIndex);
        lua_pushnumber(l,HINT_NONE);
        return 1;
    }
    GameObject* g = &objects[objIndex];
    Ability* a = &g->abilities[abilityIndex];
    lua_pushnumber(l,a->targetingHint);
    return 1;
}
int L_GetHintRadius(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);   
    if (!GameObjectIndexInRange(objIndex) || !AbilityIndexInRange(abilityIndex))
    {
        ConsolePrintf("L_GetHintRadius: index not in range: %i, %i\n",objIndex,abilityIndex);
        lua_pushnumber(l,0);
        return 1;
    }
    GameObject* g = &objects[objIndex];
    Ability* a = &g->abilities[abilityIndex];
    lua_pushnumber(l,a->hintRadius);
    return 1;
}
int L_GetHintSoak(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);   

    if (!GameObjectIndexInRange(objIndex) || !AbilityIndexInRange(abilityIndex))
    {
        ConsolePrintf("L_GetHintSoak: index not in range: %i, %i\n",objIndex,abilityIndex);
        lua_pushboolean(l,false);
        return 1;
    }
    GameObject* g = &objects[objIndex];
    Ability* a = &g->abilities[abilityIndex];
    lua_pushboolean(l,a->hintSoak);
    return 1;
}
int L_HintLength(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int abilityIndex = lua_tonumber(l,2);   

    if (GameObjectIndexInRange(objIndex) && AbilityIndexInRange(abilityIndex))
    {
        ConsolePrintf("L_HintLength: index not in range: %i, %i\n",objIndex,abilityIndex);
        lua_pushnumber(l,0);
        return 1;
    }
    GameObject* g = &objects[objIndex];
    Ability* a = &g->abilities[abilityIndex];
    lua_pushnumber(l,a->hintLength);
    return 1;
}
int L_DegToRad(lua_State* l)
{
    float f = lua_tonumber(l,1);
    lua_pushnumber(l,DegToRad(f));
    return 1;
}   
int L_RadToDeg(lua_State* l)
{
    float f = lua_tonumber(l,1);
    lua_pushnumber(l,RadToDeg(f));
    return 1;
}   
int L_DegToHeadingVector(lua_State* l)
{
    float angle = lua_tonumber(l,1);
    angle = DegToRad(angle);
    float x = cosf(angle);
    float y = sinf(angle);

    lua_pushnumber(l,1);
    lua_newtable(l);

    lua_pushnumber(l,x);
    lua_setfield(l,-2,"headingx");

    lua_pushnumber(l,y);
    lua_setfield(l,-2,"headingy");

    return 1;
}
int L_GetSampleIndices(lua_State* l)
{
    //returns n random unique indices in a range
    int numIndices = lua_tonumber(l,1);
    int min = lua_tonumber(l,2);
    int max = lua_tonumber(l,3);

    if ((max - min)+1 < numIndices)
    {
        ConsolePrintf("L_GetSampleIndices: range too small. NumIndices: %i, min: %i, max: %i\n",numIndices,min,max);
        return 0;
    }
    
    int* selected = calloc(numIndices,sizeof(int));
    int numSelected = 0;
    
    while (numSelected < numIndices)
    {
        int r = RandRangeI(min,max);
        bool select = true;
        for (int i = 0; i < numSelected; i++)
        {
            if (selected[i] == r)
                select = false;
        }

        if (select)
        {
            selected[numSelected] = r;
            numSelected++;
        }
    }
    lua_newtable(l);
    for (int i = 0; i < numSelected; i++)
    {
        lua_pushnumber(l,i+1);
        lua_pushnumber(l,selected[i]);
        lua_settable(l,-3);
    }
    free(selected);
    return 1;

}
int L_AddMana(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(objIndex))
    {
        ConsolePrintf("L_AddMana: index out of range: %i\n",objIndex);
        return 0;
    }
    float manaToAdd = lua_tonumber(l,2);
    AddMana(&objects[objIndex],manaToAdd);
    return 0;
}
int L_GetObjectsInCone(lua_State* l)
{
    int objFriendliness = lua_tonumber(l,1);
    int fromX = lua_tonumber(l,2);
    int fromY = lua_tonumber(l,3);
    int toX = lua_tonumber(l,4);
    int toY = lua_tonumber(l,5);
    float radius = lua_tonumber(l,6);
    float length = lua_tonumber(l,7);

    int thisFriendliness = GetPlayerOwnedBy(currGameObjRunning);

    int* selected = calloc(numActiveObjects,sizeof(int));

    int numSelected = 0;

    
    float angle = RadToDeg(atan2f(toY - fromY, toX - fromX));

    for (int i = 0; i < numActiveObjects; i++)
    {
        Rect r = GetObjRect(activeObjects[i]);
        if (!RectInCone(r,fromX,fromY,angle,radius,length))
        {
            continue;
        }

        if (objFriendliness & TYPE_FRIENDLY)
        {
            if (GetPlayerOwnedBy(activeObjects[i]) == thisFriendliness)
            {
                selected[numSelected] = activeObjects[i] - objects;
                numSelected++;
            }
        }
        if (objFriendliness & TYPE_ENEMY)
        {
            if (GetPlayerOwnedBy(activeObjects[i]) != thisFriendliness)
            {
                selected[numSelected] = activeObjects[i] - objects;
                numSelected++;
            }
        }

    }
    lua_newtable(l);
    for (int i = 0; i < numSelected; i++)
    {
        lua_pushnumber(l,i+1);
        lua_pushnumber(l,selected[i]);
        lua_settable(l,-3);
    }
    free(selected);
    return 1;
}
int L_Stun(lua_State* l)
{   
    int target = lua_tonumber(l,1);

    if (!GameObjectIndexInRange(target))
    {
        ConsolePrintf("L_Stun: index out of range: %i\n",target);
    }
    float time = lua_tonumber(l,2);
    Stun(currGameObjRunning,&objects[target],time);
    return 0;
}
int L_SetObjSummoned(lua_State* l)
{
    int index = lua_tonumber(l,1);
    bool b = lua_toboolean(l,2);
    if (!GameObjectIndexInRange(index))
    {
        ConsolePrintf("L_SetObjSummoned: index out of range: %i\n",index);
        return 0;
    }
    objects[index].objectIsSummoned = b;
    return 0;
}
int L_IsWalkable(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    bool caresAboutUnits = lua_toboolean(l,3);

    x /= _GRAIN;
    y /= _GRAIN;

    lua_pushboolean(l,PointIsFree(x,y,caresAboutUnits));
    return 1;
}
int L_GetMana(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index)) 
    {
        ConsolePrintf("L_GetMana: index out of range: %i\n",index);
        return 0;
    }
    lua_pushnumber(l,objects[index].mana);
    return 1;
}
int L_GetMaxMana(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index)) 
    {
        ConsolePrintf("L_GetMaxMana: index out of range: %i\n",index);
        return 0;
    }
    lua_pushnumber(l,objects[index].maxMana);
    return 1;
}
int L_GetManaPercent(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (!GameObjectIndexInRange(index)) 
    {
        ConsolePrintf("L_GetManaPercent: index out of range: %i\n",index);
        return 0;
    }
    lua_pushnumber(l,objects[index].mana/objects[index].maxMana * 100);
    return 1;
}
int L_AddAbilityPotency(lua_State* l)
{
    int index = lua_tonumber(l,1);
        if (!GameObjectIndexInRange(index)) 
    {
        ConsolePrintf("L_AddAbilityPotency: index out of range: %i\n",index);
        return 0;
    }
    double amountToAdd = lua_tonumber(l,2);
    objects[index].abilityPotency += amountToAdd;
    return 0;
}
int L_GetSpeed(lua_State* l)
{
    int index = lua_tonumber(l,1);
        if (!GameObjectIndexInRange(index)) 
    {
        ConsolePrintf("L_AddAbilityPotency: index out of range: %i\n",index);
        return 0;
    }
    float speed = objects[index].speed;
    lua_pushnumber(l,speed);
    return 1;

}
int L_SetItemHighlight(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS || !currGameObjRunning)
    {
        ConsolePrintf("L_SetItemHighlight: index out of range: %i\n",index);
        return 0;
    }
    currGameObjRunning->inventory[index].highlighted = lua_toboolean(l,2);
    return 0; 
}
int L_SetItemStackString(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS || !currGameObjRunning)
    {
        ConsolePrintf("L_SetItemStackString: index out of range: %i\n",index);
        return 0;
    }
    const char* str = lua_tostring(l,2);
    if (currGameObjRunning->inventory[index].stacksString)
        free(currGameObjRunning->inventory[index].stacksString);
    currGameObjRunning->inventory[index].stacksString = calloc(strlen(str)+1,sizeof(char));
    strcpy(currGameObjRunning->inventory[index].stacksString,str);
    return 0; 
}
int L_SetEncounterHardLoss(lua_State* l)
{
    bool b = lua_toboolean(l,1);
    if (currEncounterRunning)
    {   
        currEncounterRunning->hardLoss = b;
    }
    return 0;
}
int L_SetEncounterBreakPoints(lua_State* l)
{
    if (currEncounterRunning)
    {
        for (int i = 0; i < MAX_CHESTS; i++)
        {   
            float time = lua_tonumber(l,i+1);
            currEncounterRunning->timeBreakpoints[i] = time;
        }
    }   
    return 0;
}
int L_IsNear(lua_State* l)
{
    int index = lua_tonumber(l,1);
    float x = lua_tonumber(l,2);
    float y = lua_tonumber(l,3);

    if (!GameObjectIndexInRange(index))
    {
        printf("L_IsNear: index out of range: %i\n",index);
        return 0;
    }
    GameObject* g = &objects[index];
    bool b = IsNear(g->position.worldX,x,1) && IsNear(g->position.worldY,y,1);
    lua_pushboolean(l,b);
    return 1;
}
int L_SetTime(lua_State* l)
{
    float hours = lua_tonumber(l,1)*60*60;
    float minutes = lua_tonumber(l,2)*60;
    float seconds = lua_tonumber(l,3);

    gameStats.timeTaken = hours+minutes+seconds;
    return 0;
}
int L_SetMapGoldMultiplier(lua_State* l)
{
    if (!currMap)
    {
        ConsolePrintf("L_SetMapGoldMultiplier: no map currently set.");
        return 0;
    }
    currMap->goldMultiplier = lua_tonumber(l,1);
    return 0;
}
void SetLuaKeyEnums(lua_State* l)
{
    //TODO: Update these when a key is changed in settings
    lua_pushinteger(l,ALLEGRO_KEY_LSHIFT);
    lua_setglobal(l,"KEY_LSHIFT");

    lua_pushinteger(l,ALLEGRO_KEY_LCTRL);
    lua_setglobal(l,"KEY_LCTRL");

    lua_pushinteger(l,ALLEGRO_KEY_Q);
    lua_setglobal(l,"KEY_Q");
    lua_pushinteger(l,ALLEGRO_KEY_W);
    lua_setglobal(l,"KEY_W");
    lua_pushinteger(l,ALLEGRO_KEY_E);
    lua_setglobal(l,"KEY_E");
    lua_pushinteger(l,ALLEGRO_KEY_R);
    lua_setglobal(l,"KEY_R");
    lua_pushinteger(l,ALLEGRO_KEY_T);
    lua_setglobal(l,"KEY_T");
    lua_pushinteger(l,ALLEGRO_KEY_Y);
    lua_setglobal(l,"KEY_Y");
    lua_pushinteger(l,ALLEGRO_KEY_U);
    lua_setglobal(l,"KEY_U");
    lua_pushinteger(l,ALLEGRO_KEY_I);
    lua_setglobal(l,"KEY_I");
    lua_pushinteger(l,ALLEGRO_KEY_O);
    lua_setglobal(l,"KEY_O");
    lua_pushinteger(l,ALLEGRO_KEY_P);
    lua_setglobal(l,"KEY_P");
    lua_pushinteger(l,ALLEGRO_KEY_A);
    lua_setglobal(l,"KEY_A");
    lua_pushinteger(l,ALLEGRO_KEY_S);
    lua_setglobal(l,"KEY_S");
    lua_pushinteger(l,ALLEGRO_KEY_D);
    lua_setglobal(l,"KEY_D");
    lua_pushinteger(l,ALLEGRO_KEY_F);
    lua_setglobal(l,"KEY_F");
    lua_pushinteger(l,ALLEGRO_KEY_G);
    lua_setglobal(l,"KEY_G");
    lua_pushinteger(l,ALLEGRO_KEY_H);
    lua_setglobal(l,"KEY_H");
    lua_pushinteger(l,ALLEGRO_KEY_J);
    lua_setglobal(l,"KEY_J");
    lua_pushinteger(l,ALLEGRO_KEY_K);
    lua_setglobal(l,"KEY_K");
    lua_pushinteger(l,ALLEGRO_KEY_L);
    lua_setglobal(l,"KEY_L");
    lua_pushinteger(l,ALLEGRO_KEY_Z);
    lua_setglobal(l,"KEY_Z");
    lua_pushinteger(l,ALLEGRO_KEY_X);
    lua_setglobal(l,"KEY_X");
    lua_pushinteger(l,ALLEGRO_KEY_C);
    lua_setglobal(l,"KEY_C");
    lua_pushinteger(l,ALLEGRO_KEY_V);
    lua_setglobal(l,"KEY_V");
    lua_pushinteger(l,ALLEGRO_KEY_B);
    lua_setglobal(l,"KEY_B");
    lua_pushinteger(l,ALLEGRO_KEY_N);
    lua_setglobal(l,"KEY_N");
    lua_pushinteger(l,ALLEGRO_KEY_M);
    lua_setglobal(l,"KEY_M");

    lua_pushinteger(l,ALLEGRO_KEY_1);
    lua_setglobal(l,"KEY_1");
    lua_pushinteger(l,ALLEGRO_KEY_2);
    lua_setglobal(l,"KEY_2");
    lua_pushinteger(l,ALLEGRO_KEY_3);
    lua_setglobal(l,"KEY_3");
    lua_pushinteger(l,ALLEGRO_KEY_4);
    lua_setglobal(l,"KEY_4");
    lua_pushinteger(l,ALLEGRO_KEY_5);
    lua_setglobal(l,"KEY_5");
    lua_pushinteger(l,ALLEGRO_KEY_6);
    lua_setglobal(l,"KEY_6");
    lua_pushinteger(l,ALLEGRO_KEY_7);
    lua_setglobal(l,"KEY_7");
    lua_pushinteger(l,ALLEGRO_KEY_8);
    lua_setglobal(l,"KEY_8");
    lua_pushinteger(l,ALLEGRO_KEY_9);
    lua_setglobal(l,"KEY_9");
    lua_pushinteger(l,ALLEGRO_KEY_0);
    lua_setglobal(l,"KEY_0");


}
void SetLuaFuncs()
{
    SetGlobals(luaState);


    //Functions for the scripting API

    lua_pushcfunction(luaState, L_Print);
    lua_setglobal(luaState, "Print");

    lua_pushcfunction(luaState, L_CreateObject);
    lua_setglobal(luaState, "CreateObject");

    lua_pushcfunction(luaState, L_SetSprite);
    lua_setglobal(luaState, "SetSprite");

    lua_pushcfunction(luaState, L_SetMapSprite);
    lua_setglobal(luaState, "SetMapSprite");

    lua_pushcfunction(luaState, L_AddAbility);
    lua_setglobal(luaState, "AddAbility");

    lua_pushcfunction(luaState, L_AbilitySetPortrait);
    lua_setglobal(luaState, "AbilitySetPortrait");

    lua_pushcfunction(luaState, L_GetX);
    lua_setglobal(luaState, "GetX");
    lua_pushcfunction(luaState, L_GetY);
    lua_setglobal(luaState, "GetY");

    lua_pushcfunction(luaState, L_GetCentre);
    lua_setglobal(luaState, "GetCentre");



    lua_pushcfunction(luaState, L_GetMouseScreenX);
    lua_setglobal(luaState, "GetMouseScreenX");
    lua_pushcfunction(luaState, L_GetMouseScreenY);
    lua_setglobal(luaState, "GetMouseScreenY");
    lua_pushcfunction(luaState, L_GetMouseWorldX);
    lua_setglobal(luaState, "GetMouseWorldX");
    lua_pushcfunction(luaState, L_GetMouseWorldY);
    lua_setglobal(luaState, "GetMouseWorldY");




    lua_pushcfunction(luaState, L_CreateProjectile);
    lua_setglobal(luaState, "CreateProjectile");

    lua_pushcfunction(luaState, L_CreateAOE);
    lua_setglobal(luaState, "CreateAOE");

    lua_pushcfunction(luaState, L_GetObjRef);
    lua_setglobal(luaState, "GetObjRef");

    lua_pushcfunction(luaState, L_SetObjPosition);
    lua_setglobal(luaState, "SetObjPosition");

    lua_pushcfunction(luaState, L_GetWidth);
    lua_setglobal(luaState, "GetWidth");

    lua_pushcfunction(luaState, L_GetHeight);
    lua_setglobal(luaState, "GetHeight");

    lua_pushcfunction(luaState, L_SetObjTargetPosition);
    lua_setglobal(luaState, "SetObjTargetPosition");

    lua_pushcfunction(luaState, L_Teleport);
    lua_setglobal(luaState, "Teleport");

    lua_pushcfunction(luaState, L_AddAttackSprite);
    lua_setglobal(luaState, "AddAttackSprite");

    lua_pushcfunction(luaState, L_CastAbility);
    lua_setglobal(luaState, "CastAbility");

    lua_pushcfunction(luaState, L_ApplyEffect);
    lua_setglobal(luaState, "ApplyEffect");

    lua_pushcfunction(luaState, L_DealDamage);
    lua_setglobal(luaState, "DealDamage");

    lua_pushcfunction(luaState, L_GetThreatRank);
    lua_setglobal(luaState, "GetThreatRank");

    lua_pushcfunction(luaState, L_GetAttackTarget);
    lua_setglobal(luaState, "GetAttackTarget");

    lua_pushcfunction(luaState, L_GetThisObj);
    lua_setglobal(luaState, "GetThisObj");

    lua_pushcfunction(luaState, L_SetAbilityRange);
    lua_setglobal(luaState, "SetAbilityRange");

    lua_pushcfunction(luaState, L_GetAbilityRange);
    lua_setglobal(luaState, "GetAbilityRange");


    lua_pushcfunction(luaState, L_SetCooldown);
    lua_setglobal(luaState, "SetCooldown");

    lua_pushcfunction(luaState, L_AbilitySetCastType);
    lua_setglobal(luaState, "AbilitySetCastType");

    lua_pushcfunction(luaState, L_MoveAttack);
    lua_setglobal(luaState, "MoveAttack");

    lua_pushcfunction(luaState, L_RemoveAttack);
    lua_setglobal(luaState, "RemoveAttack");

    lua_pushcfunction(luaState, L_SetAttacking);
    lua_setglobal(luaState, "SetAttacking");

    lua_pushcfunction(luaState, L_SetDamage);
    lua_setglobal(luaState, "SetDamage");

    lua_pushcfunction(luaState, L_CreateCircularProjectiles);
    lua_setglobal(luaState, "CreateCircularProjectiles");

    lua_pushcfunction(luaState, L_SetMovePoint);
    lua_setglobal(luaState, "SetMovePoint");

    lua_pushcfunction(luaState, L_ToggleAbility);
    lua_setglobal(luaState, "ToggleAbility");

    lua_pushcfunction(luaState, L_UntoggleOthers);
    lua_setglobal(luaState, "UntoggleOthers");

    lua_pushcfunction(luaState, L_Bor);
    lua_setglobal(luaState, "Bor");
    lua_pushcfunction(luaState, L_Band);
    lua_setglobal(luaState, "Band");
    lua_pushcfunction(luaState, L_Bxor);
    lua_setglobal(luaState, "Bxor");
    lua_pushcfunction(luaState, L_Bnot);
    lua_setglobal(luaState, "Bnot");

    lua_pushcfunction(luaState, L_SetDescription);
    lua_setglobal(luaState, "SetDescription");
    
    lua_pushcfunction(luaState, L_SetRange);
    lua_setglobal(luaState, "SetRange");

    lua_pushcfunction(luaState, L_GetBetween);
    lua_setglobal(luaState, "GetBetween");

    lua_pushcfunction(luaState, L_GetOwnedBy);
    lua_setglobal(luaState, "GetOwnedBy");

    lua_pushcfunction(luaState, L_SetAbilityCastTypeHint);
    lua_setglobal(luaState, "SetAbilityCastTypeHint");

    lua_pushcfunction(luaState, L_SetMaxHP);
    lua_setglobal(luaState, "SetMaxHP");

    lua_pushcfunction(luaState, L_SetSpeed);
    lua_setglobal(luaState, "SetSpeed");

    lua_pushcfunction(luaState, L_SetObjectPush);
    lua_setglobal(luaState, "SetObjectPush");

    lua_pushcfunction(luaState, L_IsInCombat);
    lua_setglobal(luaState, "IsInCombat");

    lua_pushcfunction(luaState, L_RandRange);
    lua_setglobal(luaState, "RandRange");

    lua_pushcfunction(luaState, L_AbilityIsOnCooldown);
    lua_setglobal(luaState, "AbilityIsOnCooldown");

    lua_pushcfunction(luaState, L_SetObjType);
    lua_setglobal(luaState, "SetObjType");

    lua_pushcfunction(luaState, L_GetRandomUnit);
    lua_setglobal(luaState, "GetRandomUnit");

    lua_pushcfunction(luaState, L_SetAbilityHint);
    lua_setglobal(luaState, "SetAbilityHint");

    lua_pushcfunction(luaState, L_CreateCone);
    lua_setglobal(luaState, "CreateCone");

    lua_pushcfunction(luaState, L_SetAttackPosition);
    lua_setglobal(luaState, "SetAttackPosition");

    lua_pushcfunction(luaState, L_SetAttackTarget);
    lua_setglobal(luaState, "SetAttackTarget");

    lua_pushcfunction(luaState, L_SetAttackTargetPosition);
    lua_setglobal(luaState, "SetAttackTargetPosition");


    lua_pushcfunction(luaState, L_SetThreatMultiplier);
    lua_setglobal(luaState, "SetThreatMultiplier");

    lua_pushcfunction(luaState, L_SetEncounterSprite);
    lua_setglobal(luaState, "SetEncounterSprite");

    lua_pushcfunction(luaState, L_SetEncounterDescription);
    lua_setglobal(luaState, "SetEncounterDescription");

    lua_pushcfunction(luaState, L_AddEncounterAbility);
    lua_setglobal(luaState, "AddEncounterAbility");


    lua_pushcfunction(luaState, L_SetEncounterMapPath);
    lua_setglobal(luaState, "SetEncounterMapPath");

    lua_pushcfunction(luaState, L_IsPlayerChoosable);
    lua_setglobal(luaState, "IsPlayerChoosable");

    lua_pushcfunction(luaState, L_SetEncounterDifficulty);
    lua_setglobal(luaState, "SetEncounterDifficulty");

    lua_pushcfunction(luaState, L_GetTime);
    lua_setglobal(luaState, "GetTime");

    lua_pushcfunction(luaState, L_SetHP);
    lua_setglobal(luaState, "SetHP");

    lua_pushcfunction(luaState, L_EncounterSetNumUnitsToSelect);
    lua_setglobal(luaState, "EncounterSetNumUnitsToSelect");

    lua_pushcfunction(luaState, L_GetHighestThreat);
    lua_setglobal(luaState, "GetHighestThreat");

    lua_pushcfunction(luaState, L_GetHP);
    lua_setglobal(luaState, "GetHP");

    lua_pushcfunction(luaState, L_GetHPPercent);
    lua_setglobal(luaState, "GetHPPercent");

    lua_pushcfunction(luaState, L_GetMaxHP);
    lua_setglobal(luaState, "GetMaxHP");

    lua_pushcfunction(luaState, L_SetEncounterMusic);
    lua_setglobal(luaState, "SetEncounterMusic");

    lua_pushcfunction(luaState, L_PlayMusic);
    lua_setglobal(luaState, "PlayMusic");

    lua_pushcfunction(luaState, L_SetDecoration);
    lua_setglobal(luaState, "SetDecoration");

    lua_pushcfunction(luaState, L_SetInvincible);
    lua_setglobal(luaState, "SetInvincible");

    lua_pushcfunction(luaState, L_SetRotation);
    lua_setglobal(luaState, "SetRotation");

    lua_pushcfunction(luaState, L_GetRotation);
    lua_setglobal(luaState, "GetRotation");

    lua_pushcfunction(luaState, L_GetGamestate);
    lua_setglobal(luaState, "GetGamestate");


    lua_pushcfunction(luaState, L_GamestateIsInTransition);
    lua_setglobal(luaState, "GamestateIsInTransition");

    lua_pushcfunction(luaState, L_GetTransitioningTo);
    lua_setglobal(luaState, "GetTransitioningTo");

    lua_pushcfunction(luaState, L_ClearCommandQueue);
    lua_setglobal(luaState, "ClearCommandQueue");

    lua_pushcfunction(luaState, L_SetEncounterLoadScreen);
    lua_setglobal(luaState, "SetEncounterLoadScreen");

    lua_pushcfunction(luaState, L_SetEncounterName);
    lua_setglobal(luaState, "SetEncounterName");

    lua_pushcfunction(luaState, L_SetMoveSpeed);
    lua_setglobal(luaState, "SetMoveSpeed");

    lua_pushcfunction(luaState, L_PushMessage);
    lua_setglobal(luaState, "PushMessage");

    lua_pushcfunction(luaState, L_RemoveFromEncountersList);
    lua_setglobal(luaState, "RemoveFromEncountersList");

    lua_pushcfunction(luaState, L_GetCurrentMessage);
    lua_setglobal(luaState, "GetCurrentMessage");

    lua_pushcfunction(luaState, L_PopMessage);
    lua_setglobal(luaState, "PopMessage");

    lua_pushcfunction(luaState, L_ClearMessages);
    lua_setglobal(luaState, "ClearMessages");

    lua_pushcfunction(luaState, L_GetNumberOfUnitsSelected);
    lua_setglobal(luaState, "GetNumberOfUnitsSelected");

    lua_pushcfunction(luaState, L_GetUnitCommandList);
    lua_setglobal(luaState, "GetUnitCommandList");
    
    lua_pushcfunction(luaState, L_GetUnitCurrentCommand);
    lua_setglobal(luaState, "GetUnitCurrentCommand");

    lua_pushcfunction(luaState, L_SetAutoWin);
    lua_setglobal(luaState, "SetAutoWin");

    lua_pushcfunction(luaState, L_Win);
    lua_setglobal(luaState, "Win");

    lua_pushcfunction(luaState, L_Lose);
    lua_setglobal(luaState, "Lose");

    lua_pushcfunction(luaState, L_SetAbilityCooldownTimer);
    lua_setglobal(luaState, "SetAbilityCooldownTimer");

    lua_pushcfunction(luaState, L_SetAbilityCooldown);
    lua_setglobal(luaState, "SetAbilityCooldown");

    lua_pushcfunction(luaState, L_SetAbilityOnCooldown);
    lua_setglobal(luaState, "SetAbilityOnCooldown");

    lua_pushcfunction(luaState, L_GetShield);
    lua_setglobal(luaState, "GetShield");

    lua_pushcfunction(luaState, L_GetW);
    lua_setglobal(luaState, "GetW");

    lua_pushcfunction(luaState, L_GetH);
    lua_setglobal(luaState, "GetH");

    lua_pushcfunction(luaState, L_KillObj);
    lua_setglobal(luaState, "KillObj");

    lua_pushcfunction(luaState, L_IsAlive);
    lua_setglobal(luaState, "IsAlive");

    lua_pushcfunction(luaState, L_GetControlGroup);
    lua_setglobal(luaState, "GetControlGroup");

    lua_pushcfunction(luaState, L_GetKey);
    lua_setglobal(luaState, "GetKey");

    lua_pushcfunction(luaState, L_GetNumEffects);
    lua_setglobal(luaState, "GetNumEffects");

    lua_pushcfunction(luaState, L_SetManaCost);
    lua_setglobal(luaState, "SetManaCost");

    lua_pushcfunction(luaState, L_SetManaRegen);
    lua_setglobal(luaState, "SetManaRegen");

    lua_pushcfunction(luaState, L_SetMaxMana);
    lua_setglobal(luaState, "SetMaxMana");

    lua_pushcfunction(luaState, L_NumObjectsOwnedByPlayer);
    lua_setglobal(luaState, "NumObjectsOwnedByPlayer");

    lua_pushcfunction(luaState, L_ChangeMap);
    lua_setglobal(luaState, "ChangeMap");


    lua_pushcfunction(luaState, L_CopyObject);
    lua_setglobal(luaState, "CopyObject");


    lua_pushcfunction(luaState, L_SetLifetime);
    lua_setglobal(luaState, "SetLifetime");

    lua_pushcfunction(luaState, L_GetAttackRef);
    lua_setglobal(luaState, "GetAttackRef");

    lua_pushcfunction(luaState, L_SetCategory);
    lua_setglobal(luaState, "SetCategory");

    lua_pushcfunction(luaState, L_SetAttackCircle);
    lua_setglobal(luaState, "SetAttackCircle");

    lua_pushcfunction(luaState, L_GetObjectName);
    lua_setglobal(luaState, "GetObjectName");

    lua_pushcfunction(luaState, L_SetChannelingSprite);
    lua_setglobal(luaState, "SetChannelingSprite");

    lua_pushcfunction(luaState, L_SetSpawnPoint);
    lua_setglobal(luaState, "SetSpawnPoint");

    lua_pushcfunction(luaState, L_RotatePoint);
    lua_setglobal(luaState, "RotatePoint");

    lua_pushcfunction(luaState, L_HurtObj);
    lua_setglobal(luaState, "HurtObj");
    
    lua_pushcfunction(luaState, L_SetAbilityMaxStacks);
    lua_setglobal(luaState, "SetAbilityMaxStacks");

    lua_pushcfunction(luaState, L_CureNamedEffect);
    lua_setglobal(luaState, "CureNamedEffect");


    lua_pushcfunction(luaState, L_GetAbilityCooldownTimer);
    lua_setglobal(luaState, "GetAbilityCooldownTimer");

    lua_pushcfunction(luaState, L_GetAbilityCooldown);
    lua_setglobal(luaState, "GetAbilityCooldown");

    lua_pushcfunction(luaState, L_SetAttackSpeed);
    lua_setglobal(luaState, "SetAttackSpeed");

    lua_pushcfunction(luaState, L_ShowString);
    lua_setglobal(luaState, "ShowString");

    lua_pushcfunction(luaState, L_GetObjFriendliness);
    lua_setglobal(luaState, "GetObjFriendliness");

    lua_pushcfunction(luaState, L_ObjectUsesMana);
    lua_setglobal(luaState, "ObjectUsesMana");

    lua_pushcfunction(luaState, L_SetHPIgnoreAugment);
    lua_setglobal(luaState, "SetHPIgnoreAugment");

    lua_pushcfunction(luaState, L_PlaySound);
    lua_setglobal(luaState, "PlaySound");

    lua_pushcfunction(luaState, L_SetObjAggroRadius);
    lua_setglobal(luaState, "SetObjAggroRadius");

    lua_pushcfunction(luaState, L_GetAllObjsByFriendliness);
    lua_setglobal(luaState, "GetAllObjsByFriendliness");

    lua_pushcfunction(luaState, L_EnableAI);
    lua_setglobal(luaState, "EnableAI");

    lua_pushcfunction(luaState, L_SetObjChannelling);
    lua_setglobal(luaState, "SetObjChannelling");

    lua_pushcfunction(luaState, L_SetAttackLifetime);
    lua_setglobal(luaState, "SetAttackLifetime");

    lua_pushcfunction(luaState, L_GetAttackLifetime);
    lua_setglobal(luaState, "GetAttackLifetime");

    lua_pushcfunction(luaState, L_AddStack);
    lua_setglobal(luaState, "AddStack");

    lua_pushcfunction(luaState, L_GetStacks);
    lua_setglobal(luaState, "GetStacks");

    lua_pushcfunction(luaState, L_GetMaxStacks);
    lua_setglobal(luaState, "GetMaxStacks");


    lua_pushcfunction(luaState, L_GetAbilityRef);
    lua_setglobal(luaState, "GetAbilityRef");

    lua_pushcfunction(luaState, L_SetAbilityStacks);
    lua_setglobal(luaState, "SetAbilityStacks");

    lua_pushcfunction(luaState, L_SetCooldownTimer);
    lua_setglobal(luaState, "SetCooldownTimer");

    lua_pushcfunction(luaState, L_GetAbilityStacks);
    lua_setglobal(luaState, "GetAbilityStacks");

    lua_pushcfunction(luaState, L_ModifyAbilityCooldownTimer);
    lua_setglobal(luaState, "ModifyAbilityCooldownTimer");
    
    lua_pushcfunction(luaState, L_Normalize);
    lua_setglobal(luaState, "Normalize");

    lua_pushcfunction(luaState, L_GetAttackPosition);
    lua_setglobal(luaState, "GetAttackPosition");

    lua_pushcfunction(luaState, L_ObjIsStunnable);
    lua_setglobal(luaState, "ObjIsStunnable");

    lua_pushcfunction(luaState, L_CreateTicker);
    lua_setglobal(luaState, "CreateTicker");

    lua_pushcfunction(luaState, L_UntoggleAll);
    lua_setglobal(luaState, "UntoggleAll");

    lua_pushcfunction(luaState, L_GetObjectsInRange);
    lua_setglobal(luaState, "GetObjectsInRange");

    lua_pushcfunction(luaState, L_GetClosestObjectInRange);
    lua_setglobal(luaState, "GetClosestObjectInRange");

    lua_pushcfunction(luaState, L_GetNumEffects);
    lua_setglobal(luaState, "GetNumEffects");

    lua_pushcfunction(luaState, L_GetEffects);
    lua_setglobal(luaState, "GetEffects");

    lua_pushcfunction(luaState, L_GetItemRef);
    lua_setglobal(luaState, "GetItemRef");

    lua_pushcfunction(luaState, L_SetItemName);
    lua_setglobal(luaState, "SetItemName");

    lua_pushcfunction(luaState, L_SetItemDescription);
    lua_setglobal(luaState, "SetItemDescription");

    lua_pushcfunction(luaState, L_AddItem);
    lua_setglobal(luaState, "AddItem");

    lua_pushcfunction(luaState, L_SetItemGoldCost);
    lua_setglobal(luaState, "SetItemGoldCost");

    lua_pushcfunction(luaState, L_SetItemTier);
    lua_setglobal(luaState, "SetItemTier");

    lua_pushcfunction(luaState, L_SetGoldCost);
    lua_setglobal(luaState, "SetGoldCost");

    lua_pushcfunction(luaState, L_SetItemIcon);
    lua_setglobal(luaState, "SetItemIcon");

    lua_pushcfunction(luaState, L_SetBounty);
    lua_setglobal(luaState, "SetBounty");

    lua_pushcfunction(luaState, L_AddArmor);
    lua_setglobal(luaState, "AddArmor");

    lua_pushcfunction(luaState, L_SetObjIsBoss);
    lua_setglobal(luaState, "SetObjIsBoss");

    lua_pushcfunction(luaState, L_SetLightSize);
    lua_setglobal(luaState, "SetLightSize");
    lua_pushcfunction(luaState, L_SetLightIntensity);
    lua_setglobal(luaState, "SetLightIntensity");
    lua_pushcfunction(luaState, L_SetLightColour);
    lua_setglobal(luaState, "SetLightColour");

    lua_pushcfunction(luaState, L_SetObjectCompletionPercent);
    lua_setglobal(luaState, "SetObjectCompletionPercent");

    lua_pushcfunction(luaState, L_EnableNextLevelButton);
    lua_setglobal(luaState, "EnableNextLevelButton");

    lua_pushcfunction(luaState, L_SetObjPurchaseScreenSprite);
    lua_setglobal(luaState, "SetObjPurchaseScreenSprite");


    lua_pushcfunction(luaState, L_SetObjDescription);
    lua_setglobal(luaState, "SetObjDescription");

    lua_pushcfunction(luaState, L_SetObjName);
    lua_setglobal(luaState, "SetObjName");

    lua_pushcfunction(luaState, L_SetObjCost);
    lua_setglobal(luaState, "SetObjCost");

    lua_pushcfunction(luaState, L_NumIsInRange);
    lua_setglobal(luaState, "NumIsInRange");

    lua_pushcfunction(luaState, L_ObjIsChannelling);
    lua_setglobal(luaState, "ObjIsChannelling");

    lua_pushcfunction(luaState, L_AddThreat);
    lua_setglobal(luaState, "AddThreat");

    lua_pushcfunction(luaState, L_GetHeadingVector);
    lua_setglobal(luaState, "GetHeadingVector");

    lua_pushcfunction(luaState, L_SetEnraged);
    lua_setglobal(luaState, "SetEnraged");

    lua_pushcfunction(luaState, L_StartsUnlocked);
    lua_setglobal(luaState, "StartsUnlocked");

    lua_pushcfunction(luaState, L_UnlockEncounter);
    lua_setglobal(luaState, "UnlockEncounter");


    lua_pushcfunction(luaState, L_SetUnlockLevelPrice);
    lua_setglobal(luaState, "SetUnlockLevelPrice");

    lua_pushcfunction(luaState, L_MObj);
    lua_setglobal(luaState, "MObj");

    lua_pushcfunction(luaState, L_AddGold);
    lua_setglobal(luaState, "AddGold");


    lua_pushcfunction(luaState, L_SetAbilityName);
    lua_setglobal(luaState, "SetAbilityName");

    lua_pushcfunction(luaState, L_SetAttackSounds);
    lua_setglobal(luaState, "SetAttackSounds");

    lua_pushcfunction(luaState, L_CreateConeProjectiles);
    lua_setglobal(luaState, "CreateConeProjectiles");

    lua_pushcfunction(luaState, L_SetAggroGroup);
    lua_setglobal(luaState, "SetAggroGroup");

    lua_pushcfunction(luaState, L_RemoveAggroGroup);
    lua_setglobal(luaState, "RemoveAggroGroup");

    lua_pushcfunction(luaState, L_GoShop);
    lua_setglobal(luaState, "GoShop");

    lua_pushcfunction(luaState, L_IsDecoration);
    lua_setglobal(luaState, "IsDecoration");

    lua_pushcfunction(luaState, L_GetOppositeFriendliness);
    lua_setglobal(luaState, "GetOppositeFriendliness");

    lua_pushcfunction(luaState, L_GetMapWidth);
    lua_setglobal(luaState, "GetMapWidth");

    lua_pushcfunction(luaState, L_GetMapHeight);
    lua_setglobal(luaState, "GetMapHeight");

    lua_pushcfunction(luaState, L_GetAttackTarget);
    lua_setglobal(luaState, "GetAttackTarget");

    lua_pushcfunction(luaState, L_TimeSinceLastCast);
    lua_setglobal(luaState, "TimeSinceLastCast");

    lua_pushcfunction(luaState, L_SetCompletionPercent);
    lua_setglobal(luaState, "SetCompletionPercent");

    lua_pushcfunction(luaState, L_GetCompletionPercent);
    lua_setglobal(luaState, "GetCompletionPercent");

    lua_pushcfunction(luaState, L_AnyObjInCombat);
    lua_setglobal(luaState, "AnyObjInCombat");

    lua_pushcfunction(luaState, L_RemoveFromCount);
    lua_setglobal(luaState, "RemoveFromCount");

    lua_pushcfunction(luaState, L_PushObj);
    lua_setglobal(luaState, "PushObj");

    lua_pushcfunction(luaState, L_SetObjectPushable);
    lua_setglobal(luaState, "SetObjectPushable");

    lua_pushcfunction(luaState, L_ShakeScreen);
    lua_setglobal(luaState, "ShakeScreen");

    lua_pushcfunction(luaState, L_CreateAttackArea);
    lua_setglobal(luaState, "CreateAttackArea");

    lua_pushcfunction(luaState, L_RotateAttackArea);
    lua_setglobal(luaState, "RotateAttackArea");

    lua_pushcfunction(luaState, L_CutoutArea);
    lua_setglobal(luaState, "CutoutArea");

    lua_pushcfunction(luaState, L_SetAttackMoveAngle);
    lua_setglobal(luaState, "SetAttackMoveAngle");

    lua_pushcfunction(luaState, L_SetAttackVelocity);
    lua_setglobal(luaState, "SetAttackVelocity");

    lua_pushcfunction(luaState, L_GetAttackVelocity);
    lua_setglobal(luaState, "GetAttackVelocity");


    lua_pushcfunction(luaState, L_CleaveEffect);
    lua_setglobal(luaState, "CleaveEffect");

    lua_pushcfunction(luaState, L_GetUIHeight);
    lua_setglobal(luaState, "GetUIHeight");

    lua_pushcfunction(luaState, L_SetStunTimer);
    lua_setglobal(luaState, "SetStunTimer");

    lua_pushcfunction(luaState, L_SetAttackInactive);
    lua_setglobal(luaState, "SetAttackInactive");

    lua_pushcfunction(luaState, L_GetAttackInactive);
    lua_setglobal(luaState, "GetAttackInactive");

    lua_pushcfunction(luaState, L_GetAggroGroup);
    lua_setglobal(luaState, "GetAggroGroup");

    lua_pushcfunction(luaState, L_Round);
    lua_setglobal(luaState, "Round");

    lua_pushcfunction(luaState, L_StopCommand);
    lua_setglobal(luaState, "StopCommand");
    lua_pushcfunction(luaState, L_After);
    lua_setglobal(luaState, "After");

    lua_pushcfunction(luaState, L_ReduceCooldown);
    lua_setglobal(luaState, "ReduceCooldown");

    lua_pushcfunction(luaState, L_GetFurthestObjectInRange);
    lua_setglobal(luaState, "GetFurthestObjectInRange");

    lua_pushcfunction(luaState, L_Clamp);
    lua_setglobal(luaState, "Clamp");

    lua_pushcfunction(luaState, L_GetDist);
    lua_setglobal(luaState, "GetDist");

    lua_pushcfunction(luaState, L_ObjIsValidIndex);
    lua_setglobal(luaState, "ObjIsValidIndex");

    lua_pushcfunction(luaState, L_IsInAttackRange);
    lua_setglobal(luaState, "IsInAttackRange");

    lua_pushcfunction(luaState, L_GetTargetingHint);
    lua_setglobal(luaState, "GetTargetingHint");
    lua_pushcfunction(luaState, L_GetHintRadius);
    lua_setglobal(luaState, "GetHintRadius");
    lua_pushcfunction(luaState, L_GetHintSoak);
    lua_setglobal(luaState, "GetHintSoak");
    lua_pushcfunction(luaState, L_HintLength);
    lua_setglobal(luaState, "HintLength");

    lua_pushcfunction(luaState, L_DegToHeadingVector);
    lua_setglobal(luaState, "DegToHeadingVector");

    lua_pushcfunction(luaState, L_GetObjsByName);
    lua_setglobal(luaState, "GetObjsByName");

    lua_pushcfunction(luaState, L_GetSampleIndices);
    lua_setglobal(luaState, "GetSampleIndices");

    lua_pushcfunction(luaState, L_GetObjectsInCone);
    lua_setglobal(luaState, "GetObjectsInCone");

    lua_pushcfunction(luaState, L_AddMana);
    lua_setglobal(luaState, "AddMana");

    lua_pushcfunction(luaState, L_UpdateTimerArgument);
    lua_setglobal(luaState, "UpdateTimerArgument");

    lua_pushcfunction(luaState, L_DistXY);
    lua_setglobal(luaState, "DistXY");

    lua_pushcfunction(luaState, L_Heal);
    lua_setglobal(luaState, "Heal");

    lua_pushcfunction(luaState, L_Stun);
    lua_setglobal(luaState, "Stun");

    lua_pushcfunction(luaState, L_AttackIsActive);
    lua_setglobal(luaState, "AttackIsActive");

    lua_pushcfunction(luaState, L_RandRangeI);
    lua_setglobal(luaState, "RandRangeI");

    lua_pushcfunction(luaState, L_SetObjSummoned);
    lua_setglobal(luaState, "SetObjSummoned");

    lua_pushcfunction(luaState, L_RemoveItem);
    lua_setglobal(luaState, "RemoveItem");

    lua_pushcfunction(luaState, L_SetCanHitParent);
    lua_setglobal(luaState, "SetCanHitParent");

    lua_pushcfunction(luaState, L_IsWalkable);
    lua_setglobal(luaState, "IsWalkable");

    lua_pushcfunction(luaState, L_GetMana);
    lua_setglobal(luaState, "GetMana");

    lua_pushcfunction(luaState, L_GetMaxMana);
    lua_setglobal(luaState, "GetMaxMana");

    lua_pushcfunction(luaState, L_GetManaPercent);
    lua_setglobal(luaState, "GetManaPercent");

    lua_pushcfunction(luaState, L_AddAbilityPotency);
    lua_setglobal(luaState, "AddAbilityPotency");

    lua_pushcfunction(luaState, L_GetSpeed);
    lua_setglobal(luaState, "GetSpeed");

    lua_pushcfunction(luaState, L_SetItemHighlight);
    lua_setglobal(luaState, "SetItemHighlight");

    lua_pushcfunction(luaState, L_SetItemStackString);
    lua_setglobal(luaState, "SetItemStackString");

    lua_pushcfunction(luaState, L_GetLifetime);
    lua_setglobal(luaState, "GetLifetime");

    lua_pushcfunction(luaState, L_DeathTimerIsSet);
    lua_setglobal(luaState, "DeathTimerIsSet");

    lua_pushcfunction(luaState, L_GetObjTargetPosition);
    lua_setglobal(luaState, "GetObjTargetPosition");

    lua_pushcfunction(luaState, L_BlockCommands);
    lua_setglobal(luaState, "BlockCommands");

    lua_pushcfunction(luaState, L_SetEncounterHardLoss);
    lua_setglobal(luaState, "SetEncounterHardLoss");

    lua_pushcfunction(luaState, L_SetEncounterBreakPoints);
    lua_setglobal(luaState, "SetEncounterBreakPoints");

    lua_pushcfunction(luaState, L_SetTime);
    lua_setglobal(luaState, "SetTime");

    lua_pushcfunction(luaState, L_SetMapGoldMultiplier);
    lua_setglobal(luaState, "SetMapGoldMultiplier");

    lua_pushcfunction(luaState, L_HasEffect);
    lua_setglobal(luaState, "HasEffect");

    lua_pushcfunction(luaState, L_IsNear);
    lua_setglobal(luaState, "IsNear");

    lua_pushcfunction(luaState, L_HoldCommand);
    lua_setglobal(luaState, "HoldCommand");

    lua_pushcfunction(luaState, L_SetCooldownRate);
    lua_setglobal(luaState, "SetCooldownRate");

    lua_pushcfunction(luaState, L_SetGamestateToMainMenu);
    lua_setglobal(luaState, "SetGamestateToMainMenu");

    lua_pushcfunction(luaState, L_ClearChanneledAbility);
    lua_setglobal(luaState, "ClearChanneledAbility");

    lua_pushcfunction(luaState, L_AddAttackVelocity);
    lua_setglobal(luaState, "AddAttackVelocity");
    lua_pushcfunction(luaState, L_GetAttackMoveAngle);
    lua_setglobal(luaState, "GetAttackMoveAngle");

    lua_pushcfunction(luaState, L_DegToRad);
    lua_setglobal(luaState, "DegToRad");
    lua_pushcfunction(luaState, L_RadToDeg);
    lua_setglobal(luaState, "RadToDeg");

    lua_pushcfunction(luaState, L_Lerp);
    lua_setglobal(luaState, "Lerp");

    lua_pushcfunction(luaState, L_Cerp);
    lua_setglobal(luaState, "Cerp");


    lua_pushcfunction(luaState, L_HasDoneTutorial);
    lua_setglobal(luaState, "HasDoneTutorial");

    lua_pushcfunction(luaState, L_DoneTutorial);
    lua_setglobal(luaState, "DoneTutorial");

    lua_pushcfunction(luaState, L_SetMapName);
    lua_setglobal(luaState, "SetMapName");

    lua_pushcfunction(luaState, L_GetNumBadEffects);
    lua_setglobal(luaState, "GetNumBadEffects");

    lua_pushcfunction(luaState, L_AbilitySetHintColor);
    lua_setglobal(luaState, "AbilitySetHintColor");

    lua_pushcfunction(luaState, L_SetEncounterSelectScreenSprite);
    lua_setglobal(luaState, "SetEncounterSelectScreenSprite");


}


