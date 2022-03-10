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
int L_GetObjRef(lua_State* l)
{
    lua_pushnumber(l,(int)(currGameObjRunning-objects));
    return 1;
}

Effect GetEffectFromTable(lua_State* l, int tableStackPos, int index)
{
    lua_pushnumber(l,index);
    lua_gettable(l,tableStackPos);

    Effect e;
    /*e.trigger = GetTableField(l,tableStackPos+1,"trigger");
    e.effectType = GetTableField(l,tableStackPos+1,"type");
    e.numTriggers = GetTableField(l,tableStackPos+1,"numTriggers");
    e.duration = GetTableField(l,tableStackPos+1,"duration");
    e.tickTime = e.duration / e.numTriggers;
    e.value = GetTableField(l,tableStackPos+1,"value");*/
    e.trigger = GetTableField(l,-1,"trigger");
    e.effectType = GetTableField(l,-1,"type");
    e.numTriggers = GetTableField(l,-1,"numTriggers");
    e.duration = GetTableField(l,-1,"duration");
    e.tickTime = e.duration / e.numTriggers;
    e.value = GetTableField(l,-1,"value");

    return e;
}
int L_GetWidthOf(lua_State* l)
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
int L_GetHeightOf(lua_State* l)
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

int L_CreateProjectile(lua_State* l)
{
    const int x = lua_tonumber(l,1);
    const int y = lua_tonumber(l,2);
    const char* portrait = lua_tostring(l,3);
    const int attackType = lua_tonumber(l,4);
    const int speed = lua_tonumber(l,5);
    const int duration = lua_tonumber(l,6);

    const bool shouldCallback = lua_toboolean(l, 7);
    size_t len =  lua_rawlen(l,8);

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
        Effect e;
        e = GetEffectFromTable(l, 8, i);
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
    a.ownedBy = currGameObjRunning;
    a.properties |= ATTACK_HITS_ENEMIES;
    a.speed = speed;
    a.cameFrom = currAbilityRunning;
    //a.callback_onhit = currAbilityRunning->luafunc_onhit;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
    AddAttack(&a);


    return 1;
}
int L_CreateAOE(lua_State* l)
{
    //read from table of effects
    //eg [Effect: DoT of 10dps, Effect: Slows]
    const int x = lua_tonumber(l,1);
    const int y = lua_tonumber(l,2);
    const char* effectPortrait = lua_tostring(l,3);
    const float radius = lua_tonumber(l,4);
    const float tickrate = lua_tonumber(l,5);
    const float duration = lua_tonumber(l, 6);
    const bool shouldCallback = lua_toboolean(l, 7);

    size_t len =  lua_rawlen(l,8);
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 8, i);
        lua_remove(l,-1);
        effects[i-1] = e;
    }       

    Attack a;
    memset(&a,0,sizeof(Attack));
    a.x = x;
    a.y = y;
    a.targx = x;
    a.targy = y;
    a.radius = radius;
    a.effects = calloc(len,sizeof(Effect));
    memcpy(a.effects,effects,sizeof(Effect)*len);
    a.numEffects = len;
    a.ownedBy = currGameObjRunning;
    a.properties |= ATTACK_HITS_ENEMIES;
    //a.callback_onhit = currAbilityRunning->luafunc_onhit;
    a.cameFrom = currAbilityRunning;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.attackType = ATTACK_AOE;
    a.tickrate = tickrate;
    AddAttack(&a);

}
int L_SetObjTargetPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        obj->xtarg = lua_tonumber(l,2);
        obj->ytarg = lua_tonumber(l,3);
        obj->targObj = NULL;
    }

}
int L_SetObjPosition(lua_State* l)
{
    int index = lua_tonumber(l,1);
    if (index >= 0 && index < MAX_OBJS)
    {
        GameObject* obj = &objects[index];
        obj->x = lua_tonumber(l,2);
        obj->y = lua_tonumber(l,3);
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

    lua_pushinteger(l,TRIGGER_TIMER);
    lua_setglobal(l,"TRIGGER_TIMER");

    lua_pushinteger(l,TRIGGER_ONATTACK);
    lua_setglobal(l,"TRIGGER_ONATTACK");

    lua_pushinteger(l,TRIGGER_ONHIT);
    lua_setglobal(l,"TRIGGER_ONHIT");

    lua_pushinteger(l,TRIGGER_INSTANT);
    lua_setglobal(l,"TRIGGER_INSTANT");

    lua_pushinteger(l,EFFECT_MAXHP);
    lua_setglobal(l,"EFFECT_MAXHP");

    lua_pushinteger(l,EFFECT_DAMAGE);
    lua_setglobal(l,"EFFECT_DAMAGE");

    lua_pushinteger(l,EFFECT_HEAL);
    lua_setglobal(l,"EFFECT_HEAL");

    lua_pushinteger(l,EFFECT_HEAL);
    lua_setglobal(l,"EFFECT_POSITION");



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




}
int L_AddAbility(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    int index = lua_tonumber(l,2);
    if (index < 0) index = 0; 
    if (index > 3) index = 3;

    Ability* prefab = AddAbility(path); 
    currGameObjRunning->abilities[index] = CloneAbilityPrefab(prefab,l);

    printf("0: %i, 1: %i\n",currGameObjRunning->abilities[0].luafunc_onhit,currGameObjRunning->abilities[1].luafunc_onhit);
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

    lua_pushcfunction(luaState, L_CreateAOE);
    lua_setglobal(luaState, "CreateAOE");

    lua_pushcfunction(luaState, L_GetObjRef);
    lua_setglobal(luaState, "GetObjRef");

    lua_pushcfunction(luaState, L_SetObjPosition);
    lua_setglobal(luaState, "SetObjPosition");

    lua_pushcfunction(luaState, L_GetWidthOf);
    lua_setglobal(luaState, "GetWidthOf");

    lua_pushcfunction(luaState, L_GetHeightOf);
    lua_setglobal(luaState, "GetHeightOf");

    lua_pushcfunction(luaState, L_SetObjTargetPosition);
    lua_setglobal(luaState, "SetObjTargetPosition");

    lua_pushcfunction(luaState, L_Teleport);
    lua_setglobal(luaState, "Teleport");

}
