#include "luafuncs.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "map.h"
#include "effect.h"
#include "attack.h"
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
void LoadLuaFile(const char* filename, GameObject* g)
{
    
    if (!g->lua_buffer)
    {
        g->lua_buffer = readFile(filename);
     }
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
    return 1;
}
int L_GetX(lua_State* l)
{
    lua_pushnumber(l,currGameObjRunning->x);
    return 1;
}
int L_GetY(lua_State* l)
{
    lua_pushnumber(l,currGameObjRunning->y);
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
int GetTableField(lua_State* l, int tableIndex, const char* name)
{
    lua_getfield(l,tableIndex,name);
    if (!lua_isnil(l,-1))
    {
        int j = lua_tonumber(l,-1);
        lua_remove(l,-1);
        return j;
    }
    lua_remove(l,-1);
    return 0;

}
int L_CreateProjectile(lua_State* l)
{
    const int x = lua_tonumber(l,1);
    const int y = lua_tonumber(l,2);
    const char* portrait = lua_tostring(l,3);
    const int attackType = lua_tonumber(l,4);
    const int speed = lua_tonumber(l,5);

    size_t len =  lua_rawlen(l,6);

    ALLEGRO_MOUSE_STATE mouseState = GetMouseClamped();
    GameObject* targ = NULL;
    if (attackType == ATTACK_PROJECTILE_TARGETED)
    {   
        for (int i = 0; i < numObjects; i++)
        {
            Rect r = GetObjRect(&objects[i]);

            if (PointInRect((Vector2){x,y},r))
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
        lua_pushnumber(l,i);
        lua_gettable(l,6);

        Effect e;
        /*lua_getfield(l,7,"trigger");
        e.trigger = lua_tonumber(l,-1);
        lua_remove(l, -1);
        e.trigger = GetTableField(l,7,"trigger");

        lua_getfield(l,7,"type");
        e.effectType = lua_tonumber(l,-1);
        lua_remove(l, -1); 

        lua_getfield(l,7,"numTriggers");
        e.numTriggers = lua_tonumber(l,-1);
        lua_remove(l, -1);  

        lua_getfield(l,7,"duration");
        if (!lua_isnil(l,-1))
        {
            e.duration = lua_tonumber(l,-1);
        }
        lua_remove(l,-1);*/
        e.trigger = GetTableField(l,7,"trigger");
        e.effectType = GetTableField(l,7,"type");
        e.numTriggers = GetTableField(l,7,"numTriggers");
        e.duration = GetTableField(l,7,"duration");
        e.tickTime = e.duration / e.numTriggers;
        e.value = GetTableField(l,7,"value");

        lua_remove(l,-1);

        effects[i-1] = e;
    }       
    int w=0; int h=0;
    if (currGameObjRunning->spriteIndex)
    {
        w = al_get_bitmap_width(sprites[currGameObjRunning->spriteIndex].sprite);
        h = al_get_bitmap_height(sprites[currGameObjRunning->spriteIndex].sprite);
    }
    Attack a;
    a.x = currGameObjRunning->x + w/2;
    a.y = currGameObjRunning->y + h/2;
    a.radius = 1;
    a.target = targ;
    a.targx = x;
    a.targy = y;
    a.effects = calloc(len,sizeof(Effect));
    memcpy(a.effects,effects,sizeof(Effect)*len);
    a.numEffects = len;
    a.attackType = attackType;
    a.ownedBy = currGameObjRunning;
    a.properties |= ATTACK_HITS_ENEMIES;
    a.speed = speed;
    AddAttack(&a);


    return 1;
}
int L_CreateAOE(lua_State* l)
{
    //read from table of effects
    //eg [Effect: DoT of 10dps, Effect: Slows]
}

int L_CreateObject(lua_State* l)
{
    GameObject* g;
    const char* l_path = lua_tostring(l,1);
    const int x = lua_tonumber(l,2);
    const int y = lua_tonumber(l,3);
    int PLAYER = lua_tonumber(l,4);
    if (!lua_isnumber(l,PLAYER)) 
        PLAYER = 1;


    bool prefabFound = false;
    /*for (int i = 0; i < numPrefabs; i++)
    {
        if (strcmp(prefabs[i].path,l_path) == 0)
        {
            g = &prefabs[i];
            prefabFound = true;
            break;
        }
    } */
    if (!prefabFound)
    {
        GameObject* prefab = LoadPrefab(l_path);
        g = AddGameobject(prefab);
        g->x = x;
        g->y = y;
        g->xtarg = x;
        g->ytarg = y;
        g->speed = 5;
        g->health = 100;
        g->maxHP = 100;
        g->range = 1;
        g->baseDamage = 5;
        g->attackSpeed = 1;
        g->mana = 50;
        g->maxMana = 100;
        SetOwnedBy(g, PLAYER);

    }
    else
    {
        g = AddGameobject(g);
        g->x = x;
        g->y = y;
        g->xtarg = x;
        g->ytarg = y;
        g->speed = 5;
        g->health = 100;
        g->maxHP = 100;
        g->range = 1;
        g->baseDamage = 5;
        g->attackSpeed = 1;
        g->mana = 50;
        g->maxMana = 100;


        SetOwnedBy(g, PLAYER);
    }

    return 1;
}
int L_AbilitySetCastType(lua_State* l)
{
    currAbilityRunning->castType = lua_tonumber(l,1);
    return 1;   
}
int L_SetSprite(lua_State* l)
{
    currGameObjRunning->spriteIndex = LoadSprite(lua_tostring(l,-1),true);
    return 1;
}
int L_SetMapSprite(lua_State* l)
{
    currMap->spriteIndex = LoadSprite(lua_tostring(l,-1),false);
    return 1;
}

void SetGlobals(lua_State* l)
{
    lua_pushinteger(l,0);
    lua_setglobal(l,"PLAYER");

    lua_pushinteger(l,1);
    lua_setglobal(l,"ENEMY");

    lua_pushinteger(l,0);
    lua_setglobal(l,"TRIGGER_TIMER");

    lua_pushinteger(l,1);
    lua_setglobal(l,"TRIGGER_ONATTACK");

    lua_pushinteger(l,2);
    lua_setglobal(l,"TRIGGER_ONHIT");

    lua_pushinteger(l,3);
    lua_setglobal(l,"TRIGGER_INSTANT");

    lua_pushinteger(l,0);
    lua_setglobal(l,"EFFECT_MAXHP");

    lua_pushinteger(l,1);
    lua_setglobal(l,"EFFECT_DAMAGE");

    lua_pushinteger(l,2);
    lua_setglobal(l,"EFFECT_HEAL");


    lua_pushinteger(l,0);
    lua_setglobal(l,"ABILITY_INSTANT");
    lua_pushinteger(l,1);
    lua_setglobal(l,"ABILITY_POINT");
    lua_pushinteger(l,2);
    lua_setglobal(l,"ABILITY_TARGET_FRIENDLY");
    lua_pushinteger(l,3);
    lua_setglobal(l,"ABILITY_TARGET_ENEMY");
    lua_pushinteger(l,4);
    lua_setglobal(l,"ABILITY_TARGET_ALL");
    





}
int L_AddAbility(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    int index = lua_tonumber(l,2);
    if (index < 0) index = 0; 
    if (index > 3) index = 3;

    Ability* prefab = AddAbility(path); 
    currGameObjRunning->abilities[index] = CloneAbilityPrefab(prefab,l);
    return 1;
}
int L_AbilitySetPortrait(lua_State* l)
{
    currAbilityRunning->spriteIndex_Portrait = LoadSprite(lua_tostring(l,-1),true);
    return 1;   
}

void SetLuaFuncs()
{
    SetGlobals(luaState);

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

    lua_pushcfunction(luaState, L_GetMouseX);
    lua_setglobal(luaState, "GetMouseX");
    lua_pushcfunction(luaState, L_GetMouseY);
    lua_setglobal(luaState, "GetMouseY");


    lua_pushcfunction(luaState, L_CreateProjectile);
    lua_setglobal(luaState, "CreateProjectile");

}
