#include <time.h>
#include <math.h>


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
#include "item.h"
#include "shop.h"


static void dumpstack (lua_State* l) {
  int top=lua_gettop(l);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(l,i));
    switch (lua_type(l, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(l,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(l,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(l, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(l,i));
        break;
    }
  }
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
int L_PlayMusic(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    if (path)
    {
        PlayMusic(path);
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
int L_SetAttackPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    currAttackRunning->x = lua_tonumber(l,2);
    currAttackRunning->y = lua_tonumber(l,3);

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

    GameObject** list = calloc(MAX_OBJS,sizeof(GameObject*));
    int numObjs=0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g))
        {
            if (ObjHasType(g,typeHint))
            {
                if (friend == TYPE_ENEMY)
                {
                    if (GetPlayerOwnedBy(g) != GetPlayerOwnedBy(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }
                if (friend == TYPE_FRIENDLY)
                {
                    if (GetPlayerOwnedBy(g) == GetPlayerOwnedBy(currGameObjRunning))
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
        GameObject* randObj = list[rand()%numObjs];
         lua_pushnumber(l,randObj-objects);

    }
    //There are no objects tagged - pick a random object that follows the friendliness
    else 
    {
        for (int i = 0; i < MAX_OBJS; i++)
        {
            GameObject* g = &objects[i];
            if (IsActive(g))
            {
                if (friend == TYPE_ENEMY)
                {
                    if (GetPlayerOwnedBy(g) != GetPlayerOwnedBy(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }
                if (friend == TYPE_FRIENDLY)
                {
                    if (GetPlayerOwnedBy(g) == GetPlayerOwnedBy(currGameObjRunning))
                    {
                        list[numObjs] = g;
                        numObjs++;
                    }
                }

            }
        }
        if (numObjs > 0)
        {
            GameObject* randObj = list[rand()%numObjs];
            lua_pushnumber(l,randObj-objects);
        }
    }
    //there are no objects with the friendliness specified so let's just pick a completely random object
    if (numObjs <= 0)
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

    }
    //if we still have no objects
    if (numObjs <= 0)
    {
         lua_pushnumber(l,-1);
    }
    free(list);
    return 1;
}
void LoadLuaFile(const char* filename, GameObject* g)
{
    
    if (!g->lua_buffer)
    {
        g->lua_buffer = readFile(filename);
     }
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
    if (index < 0 || index > MAX_OBJS)
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
    if (t)
        lua_pushnumber(l,t->obj-objects);
    else
        return 0;
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
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g2 = &objects[i];
        if (IsActive(g2))
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
int L_GetMouseX(lua_State* l)
{
    ALLEGRO_MOUSE_STATE mouseState = GetMouseClamped();
    lua_pushnumber(l,mouseState.x);
    return 1;
}
int L_GetMouseY(lua_State* l)
{
    ALLEGRO_MOUSE_STATE mouseState = GetMouseClamped();
    lua_pushnumber(l,mouseState.y);
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
            return 0;
        }
        lua_remove(l,-1);
        *isAField = true;
        return j;
    }
    *isAField = false; 
    lua_remove(l,-1);
    return 0;

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
    GameObject* g = &objects[(int)lua_tonumber(l,1)];
    Ability* a = &g->abilities[(int)lua_tonumber(l,1)];
    if (AbilityIsOnCooldown(a))
    {
        lua_pushboolean(l,false);
        return 1;
    }
    lua_pushboolean(l,true);
    return 1;
}
int SortThreat(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int L_GetThreatRank(lua_State* l)
{
    int j = GetNumThreats(&currGameObjRunning->threatList);
    Threat* next = &currGameObjRunning->threatList;
    lua_newtable(l);
    
    int* threats = calloc(j,sizeof(int));
    for (int i = 0; i < j; i++)
    {
        int threat = next->obj - objects;
        threats[i] = next->obj-objects;
        next = next->next;

        if (!next)
            break;
    }
    qsort(threats,j,sizeof(threats[0]),SortThreat);
    for (int i = 0; i < j; i++)
    {
        lua_pushnumber(l,i+1);
        lua_pushnumber(l,threats[i]);

        lua_settable(l,-3);
    }
    free(threats);
    return 1; 
}

Effect GetEffectFromTable(lua_State* l, int tableStackPos, int index)
{
    lua_pushnumber(l,index);
    lua_gettable(l,tableStackPos);

    Effect e;
    e.maxStacks = 9999;
    bool isField = false;
    e.trigger = GetTableField(l,-1,"trigger", &isField);
    e.effectType = GetTableField(l,-1,"type",&isField);
    float triggersPerSecond = GetTableField(l,-1,"triggersPerSecond",&isField);
    e.triggersPerSecond = triggersPerSecond;
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
    e.numTriggers = e.duration * triggersPerSecond;
    if (e.numTriggers == 0) 
        e.numTriggers = 1;
    int maxStacks = GetTableField(l,-1,"maxStacks",&isField);
    if (isField)
    {
        e.maxStacks = maxStacks;
    }

    e.tickTime = e.duration / e.numTriggers;
    e.value = GetTableField(l,-1,"value",&isField);

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
    currAbilityRunning->range = lua_tonumber(l,1);
    return 0;
}
int L_SetDamage(lua_State* l)
{
    float damage = lua_tonumber(l,1);
    currGameObjRunning->baseDamage = damage + GetAugmentDamageBonus(damage,currEncounterRunning->augment);
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
int L_AddAttackSprite(lua_State* l)
{
    const char* _path = lua_tostring(l,1);
    char* path = calloc(strlen(_path)+1,sizeof(char));
    strcpy(path,_path);
    int w = lua_tonumber(l,2);
    int h = lua_tonumber(l,3);
    float cd = lua_tonumber(l,4);


    int before = numAnimationEffectsPrefabs;
    int index = AddAnimationEffectPrefab(path, w, h, cd);

    if (before != index)
    {
        free(path);
    }
    currGameObjRunning->numAttackEffectIndices++;
    if (!currGameObjRunning->onAttackEffectsIndices)
        currGameObjRunning->onAttackEffectsIndices = calloc(1,sizeof(int));
    currGameObjRunning->onAttackEffectsIndices = realloc(currGameObjRunning->onAttackEffectsIndices,currGameObjRunning->numAttackEffectIndices*sizeof(int));
    currGameObjRunning->onAttackEffectsIndices[currGameObjRunning->numAttackEffectIndices-1] = index;
    return 0;
}
void CreateProjectile(lua_State* l, float x, float y, const char* portrait, int attackType, int speed, int duration, bool shouldCallback, int properties, GameObject* targ, uint32_t color, Effect* effects, size_t len)
{

    int w=0; int h=0;
    if (currGameObjRunning->spriteIndex)
    {
        w = al_get_bitmap_width(sprites[currGameObjRunning->spriteIndex].sprite);
        h = al_get_bitmap_height(sprites[currGameObjRunning->spriteIndex].sprite);
    }
    Attack a;
    a.x = currGameObjRunning->position.x + w/2;
    a.y = currGameObjRunning->position.y + h/2;
    a.radius = 1;
    a.easing=0;
    a.targetRadius = a.radius;
    a.target = targ;
    float x2 = x; float y2 = y;
    if (attackType == ATTACK_PROJECTILE_ANGLE)
    {
        x2 = x - a.x;  y2 = y - a.y;
        Normalize(&x2,&y2);
    }
    a.targx = x2;
    a.targy = y2;
    a.effects = calloc(len,sizeof(Effect));
    memcpy(a.effects,effects,sizeof(Effect)*len);
    a.numEffects = len;
    a.attackType = attackType;
    a.properties = properties;

    a.speed = speed;
    a.cameFrom = currAbilityRunning;
    a.ownedBy = currGameObjRunning;

    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.color = color;
    AddAttack(&a);

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


    size_t len =  lua_rawlen(l,11);

    ALLEGRO_MOUSE_STATE mouseState = GetMouseClamped();
    GameObject* targ = NULL;
    if (attackType == ATTACK_PROJECTILE_TARGETED)
    {   
        for (int i = 0; i < numObjects; i++)
        {
            Rect r = GetObjRect(&objects[i]);

            if (PointInRect(x,y,r))
            {
                targ = &objects[i];
                break;
            }
        }

    }


    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 11, i);
        e.from = currGameObjRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       
    for (int i = 0; i < numProjectiles; i++)
    {
        float angle = M_PI / (float)numProjectiles*i*2; 
        CreateProjectile(l, x+sin(angle), y+cos(angle), portrait, attackType, speed, duration, shouldCallback, properties, targ,color, effects, len);

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
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g))
        {
            if (GetPlayerOwnedBy(&objects[i]) == player)
            {
                numObjs++;
            }
        }
    }
    lua_pushnumber(l,numObjs);
    return 1;
}
int L_CreateProjectile(lua_State* l)
{
    const float x = lua_tonumber(l,1);
    const float y = lua_tonumber(l,2);
    const char* portrait = lua_tostring(l,3);
    const int attackType = lua_tonumber(l,4);
    const int speed = lua_tonumber(l,5);
    const int duration = lua_tonumber(l,6);
    const bool shouldCallback = lua_toboolean(l, 7);
    const int properties = lua_tonumber(l,8);
    const uint32_t color = lua_tonumber(l,9);
    size_t len =  lua_rawlen(l,10);

    ALLEGRO_MOUSE_STATE mouseState = GetMouseClamped();
    GameObject* targ = NULL;
    if (attackType == ATTACK_PROJECTILE_TARGETED)
    {   
        for (int i = 0; i < numObjects; i++)
        {
            Rect r = GetObjRect(&objects[i]);

            if (PointInRect(x,y,r))
            {
                targ = &objects[i];
                break;
            }
        }

    }
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 10, i);
        e.from = currGameObjRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       
    CreateProjectile(l, x, y, portrait, attackType, speed, duration, shouldCallback, properties, targ, color, effects, len);

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

    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
    a->cdTimer = cd;

    if (a->cdTimer > a->cooldown)
        a->cdTimer = a->cooldown;
    return 0;
}
int L_GetAbilityCooldownTimer(lua_State* l)
{
    int gameObjIndex = lua_tonumber(l,1);
    int abilityObjIndex = lua_tonumber(l,2);

    GameObject* g = &objects[gameObjIndex];
    Ability* a = &g->abilities[abilityObjIndex];
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
        e = GetEffectFromTable(l, 14, i);
        e.from = currGameObjRunning;
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
            a->properties &= !(ATTACK_PROPERTIES)ATTACK_DRAW_CIRCLE;
    }
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
    const float tickrate = lua_tonumber(l,5);
    const float duration = lua_tonumber(l, 6);
    const bool shouldCallback = lua_toboolean(l, 7);
    const int properties = lua_tonumber(l,8);
    const int color = lua_tonumber(l,9);
    const int dither = lua_tonumber(l,10);
    const bool isSoak = lua_toboolean(l,11);
    const int target = lua_tonumber(l,12);


    size_t len =  lua_rawlen(l,13);
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 13, i);
        e.from = currGameObjRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       

/*
    Attack a = {0};
    a.x = x;
    a.y = y;
    a.targx = x;
    a.targy = y;
    a.radius = 0;
    a.easing=0.1f;
    a.targetRadius = radius;
    a.effects = calloc(len,sizeof(Effect));
    memcpy(a.effects,effects,sizeof(Effect)*len);
    a.numEffects = len;
    a.ownedBy = currGameObjRunning;
    a.properties = properties;
    a.cameFrom = currAbilityRunning;
    a.ownedBy = currGameObjRunning;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.attackType = ATTACK_AOE;
    a.tickrate = tickrate;
    a.color = color;
    a.dither = dither;*/
    GameObject* targ = NULL;
    if (target >= 0 && target < MAX_OBJS)
    {   
        targ = &objects[target];
    }

    Attack* ref = CreateAoE(x,y, (char*)effectPortrait, radius, tickrate, duration, shouldCallback,  properties,  color,  dither,  len, effects, targ, currGameObjRunning);
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
    currEncounterRunning->automaticWinCheck = b;
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

int L_SetObjPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        obj->position.x = lua_tonumber(l,2);
        obj->position.y = lua_tonumber(l,3);
    }
    return 1;
}
int L_Teleport(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        Teleport(obj,lua_tonumber(l,2),lua_tonumber(l,3));
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
    return 0;
}
int L_CreateObject(lua_State* l)
{
    GameObject* g;
    const char* l_path = lua_tostring(l,1);
    if (!l_path)
    {
        printf("CreateObject: Path null\n");
        return 0;    
    }
    const int x = lua_tonumber(l,2);
    const int y = lua_tonumber(l,3);
    int PLAYER = lua_tonumber(l,4);
    float summonTime = lua_tonumber(l,5);

    bool prefabFound = false;
    for (int i = 0; i < numPrefabs; i++)
    {
        
        if (strcasecmp(prefabs[i]->path,l_path) == 0)
        {
            g = prefabs[i];
            prefabFound = true;
            break;
        }
    } 
    if (!prefabFound)
    {
        GameObject* prefab = LoadPrefab(l_path);
        g = AddGameobject(prefab,x,y);
        SetOwnedBy(g, PLAYER);

    }
    else
    {
        g = AddGameobject(g,x,y);
        SetOwnedBy(g, PLAYER);
    }
    g->summonTime = 0;
    g->summonMax = summonTime;

    lua_pushnumber(l,g-objects);
    return 1;
}
int L_GetClosestObjectInRange(lua_State* l)
{
    float x = lua_tonumber(l,1);
    float y = lua_tonumber(l,2);
    float range = lua_tonumber(l,3);

    float closestDist = range;
    GameObject* closestObj = NULL;

    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (!IsActive(&objects[i]))
            continue;
        float cx; float cy;
        GetCentre(&objects[i],&cx,&cy);
        if (dist(cx,cy,x,y) < closestDist)
        {
            closestObj = &objects[i];
            closestDist = dist(cx,cy,x,y);
        }
    }
    lua_pushnumber(l,closestObj-objects);
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
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (!IsActive(g))
            continue;
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
    currAbilityRunning->castType = lua_tonumber(l,1);
    return 0;   
}
int L_SetSprite(lua_State* l)
{
    currGameObjRunning->spriteIndex = LoadSprite(lua_tostring(l,-1),true);
    if (currGameObjRunning->channelingSpriteIndex == 0)
        currGameObjRunning->channelingSpriteIndex = currGameObjRunning->spriteIndex;
    return 0;
}
int L_SetMapSprite(lua_State* l)
{
    currMap->spriteIndex = LoadSprite(lua_tostring(l,-1),false);
    PreprocessMap(currMap);
    return 0;
}
int L_SetAbilityTargetHint(lua_State* l)
{
    currAbilityRunning->targetingHint = lua_tonumber(l,1);
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
int L_KillObj(lua_State* l)
{
    int index = lua_tonumber(l,1);
    bool triggerEffects = lua_toboolean(l,2);
    if (index >= 0 && index < MAX_OBJS)
    {
        KillObj(&objects[index],triggerEffects);
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
int L_GetObjFriendliness(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index < 0 || index >= MAX_OBJS)
        return 0;
    
    lua_pushnumber(l,GetPlayerOwnedBy(&objects[index]));
    return 1;
}
int L_GetAllObjsByFriendliness(lua_State* l)
{
    int friendliness = lua_tonumber(l,1);
    
    lua_newtable(l);
    int index = 0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];

        if (IsActive(g) && GetPlayerOwnedBy(g) == friendliness)
        {
            index++;
            lua_pushnumber(l,index);
            lua_pushnumber(l,i+1);
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
    
    Damage(currGameObjRunning,&objects[index],damage,false);
    return 0;
}
int L_ShowString(lua_State* l)
{
    numStringsToDraw++;
    stringsToDraw = realloc(stringsToDraw,numStringsToDraw * sizeof(char*));
    locationsToDrawString = realloc(locationsToDrawString,numStringsToDraw * sizeof(Point));

    const char* str = lua_tostring(l,1);
    stringsToDraw[numStringsToDraw-1] = calloc(strlen(str)+1,sizeof(char));
    strcpy(stringsToDraw[numStringsToDraw-1],str);
    locationsToDrawString[numStringsToDraw-1] = (Point){lua_tonumber(l,2),lua_tonumber(l,3)};
    return 0;
}
int L_PlaySound(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    Sound* s = &sounds[LoadSound(path)];
    PlaySound(s,lua_tonumber(l,2));
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
    return 0;
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
int L_SetBounty(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    int bounty = lua_tonumber(l,2);
    GameObject* g = &objects[objIndex];
    if (objIndex < 0 || objIndex >= MAX_OBJS)
        return 0;
    g->bounty = bounty;
    return 0;
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
int L_ApplyEffect(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0) 
        return -1; 
    if (objIndex >= MAX_OBJS) 
        return -1;
    GameObject* g = &objects[objIndex];

    size_t len =  lua_rawlen(l,2);

    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 2, i);
        e.from = currGameObjRunning;
        //lua_remove(l,-1);
        ApplyEffect(&e,currGameObjRunning,&objects[objIndex]);
    }       
    
    return 1;
}
int L_DealDamage(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0) return -1; 
    if (objIndex >= MAX_OBJS) return -1;

    int dmg = lua_tonumber(l,2);
    Damage(currGameObjRunning,&objects[objIndex],dmg,false);
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
        currGameObjRunning->properties &= !OBJ_CAN_PUSH;
    return 0;
}
int L_IsInCombat(lua_State* l)
{
    lua_pushboolean(l,IsInCombat(currGameObjRunning));
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
int L_CastAbility(lua_State* l)
{
    if (ObjIsChannelling(currGameObjRunning))
    {
        lua_pushboolean(l,false);
        return 1;
    }
    int index = lua_tonumber(l,1);
        
    if (index < 0)
        index = 0;
    if (index >= MAX_ABILITIES) 
        index = MAX_ABILITIES-1;

    if (AbilityIsOnCooldown(&currGameObjRunning->abilities[index]))//.cdTimer > 0.001f)
    {
        lua_pushboolean(l,false);
        return 1;
    }
    if (!AbilityIsInitialised(&currGameObjRunning->abilities[index]))
    {
        lua_pushboolean(l,false);
        return 1;
    }
    float channelTime = lua_tonumber(l,2);

    size_t len =  lua_rawlen(l,3);

    float x; float y; int obj = -1; float headingx=-1; float headingy=-1;
    GetCentre(currGameObjRunning,&x,&y);
    bool isAField;
    for (int i = 0; i < len; i++)
    {
        lua_pushnumber(l,i+1);
        lua_gettable(l,3);

        x = GetTableField(l,-1,"x",&isAField);
        y = GetTableField(l,-1,"y",&isAField);
        obj = GetTableField(l,-1,"target",&isAField);
        headingx = GetTableField(l,-1,"headingx",&isAField);
        headingy = GetTableField(l,-1,"headingy",&isAField);
    }
    if (channelTime > 0)
    {
        GameObject* target;
        if (obj >= 0 && obj < MAX_OBJS)
        {
            target = &objects[obj];
        }
        else
            target = NULL;
        Ability* a = &currGameObjRunning->abilities[index];
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
int L_RandRange(lua_State* l)
{
    double min = lua_tonumber(l,1);
    double max = lua_tonumber(l,2);
    lua_pushnumber(l,RandRange(min,max));
    return 1;
}
int L_SetAbilityHint(lua_State* l)
{
    currAbilityRunning->targetingHint = lua_tonumber(l,1);
    if (lua_isnumber(l,2))
    {
        currAbilityRunning->hintRadius = lua_tonumber(l,2);
    }
    return 0;
}
int L_ChangeMap(lua_State* l)
{
    //if we're already in the shop
    if (gameState == GAMESTATE_IN_SHOP)
        return 0;

    const char* path = lua_tostring(l,1);
    //SetGameStateToChangingMap(path);
    SetGameStateToInShop();

    if (pathToNextMap)
        free(pathToNextMap);
    pathToNextMap = calloc(strlen(path)+1,sizeof(char));
    strcpy(pathToNextMap,path);

    return 0;
}
int L_AddAbility(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0 || objIndex >= MAX_OBJS)
        return 0;
    GameObject* g = &objects[objIndex];
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
        Ability* prefab = AddAbility(path); 
        if (!g->abilities[index].path)
        {
            g->numAbilities++;
        }
        //g->abilities[index] = CloneAbilityPrefab(prefab,l);
        CloneAbilityPrefab(prefab,l,&g->abilities[index]);
        
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
        if (a)
        {

            float x = lua_tonumber(l,2);
            float y = lua_tonumber(l,3);
            a->x = x;
            a->y = y;
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
    currAbilityRunning->spriteIndex_Portrait = LoadSprite(lua_tostring(l,-1),true);
    return 1;   
}
int L_RemoveFromEncountersList(lua_State* l)
{
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
    
    if (al_key_down(keyStateLua,index))
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
        GameObject* g2 = AddGameobject(g,x,y);
        for (int i = 0; i < MAX_ABILITIES; i++)
        {
            g2->abilities[i].cdTimer = g->abilities[i].cdTimer;
        }
    }
    lua_pushnumber(l,g-objects);
    return 1;
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



    lua_pushcfunction(luaState, L_GetMouseX);
    lua_setglobal(luaState, "GetMouseX");
    lua_pushcfunction(luaState, L_GetMouseY);
    lua_setglobal(luaState, "GetMouseY");


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

    lua_pushcfunction(luaState, L_GetThisObj);
    lua_setglobal(luaState, "GetThisObj");

    lua_pushcfunction(luaState, L_SetAbilityRange);
    lua_setglobal(luaState, "SetAbilityRange");

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

    lua_pushcfunction(luaState, L_SetAbilityTargetHint);
    lua_setglobal(luaState, "SetAbilityTargetHint");

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

}

