#include "luafuncs.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "map.h"
#include "effect.h"
#include "attack.h"
#include "animationeffect.h"
#include <math.h>
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
int L_SetAttackPosition(lua_State* l)
{
    return 0;
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
    return 0;
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
int L_GetObjRef(lua_State* l)
{
    lua_pushnumber(l,(int)(currGameObjRunning-objects));
    return 1;
}

int L_GetThreatRank(lua_State* l)
{
    int j = GetNumThreats(&currGameObjRunning->threatList);
    Threat* next = &currGameObjRunning->threatList;

    for (int i = 0; i < j; i++)
    {
        lua_pushnumber(l,(int)(next->obj - objects));
        next = next->next;

        if (!next) 
            break;
    }
    return j; 
}

Effect GetEffectFromTable(lua_State* l, int tableStackPos, int index)
{
    lua_pushnumber(l,index);
    lua_gettable(l,tableStackPos);

    Effect e;
    bool isField = false;
    /*e.trigger = GetTableField(l,tableStackPos+1,"trigger");
    e.effectType = GetTableField(l,tableStackPos+1,"type");
    e.numTriggers = GetTableField(l,tableStackPos+1,"numTriggers");
    e.duration = GetTableField(l,tableStackPos+1,"duration");
    e.tickTime = e.duration / e.numTriggers;
    e.value = GetTableField(l,tableStackPos+1,"value");*/
    e.trigger = GetTableField(l,-1,"trigger", &isField);
    e.effectType = GetTableField(l,-1,"type",&isField);
    float triggersPerSecond = GetTableField(l,-1,"triggersPerSecond",&isField);
    //e.numTriggers = GetTableField(l,-1,"numTriggers",&isField);
    e.duration = GetTableField(l,-1,"duration",&isField);
    e.timer = 0;
    //e.timer = e.duration;
    e.numTriggers = e.duration * triggersPerSecond;
    if (e.numTriggers == 0) 
        e.numTriggers = 1;

    e.tickTime = e.duration / e.numTriggers;
    e.value = GetTableField(l,-1,"value",&isField);


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
int L_SetAbilityRange(lua_State* l)
{
    currAbilityRunning->range = lua_tonumber(l,1);
    return 0;
}
int L_SetDamage(lua_State* l)
{
    float damage = lua_tonumber(l,1);
    currGameObjRunning->baseDamage = damage;
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
void CreateProjectile(lua_State* l, float x, float y, const char* portrait, int attackType, int speed, int duration, bool shouldCallback, int properties, GameObject* targ, Effect* effects, size_t len)
{

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
   // a.properties |= ATTACK_HITS_ENEMIES;
    a.properties = properties;

    a.speed = speed;
    a.cameFrom = currAbilityRunning;
    a.ownedBy = currGameObjRunning;

    //a.callback_onhit = currAbilityRunning->luafunc_onhit;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
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
    for (int i = 0; i < numProjectiles; i++)
    {
        float angle = M_PI / (float)numProjectiles*i*2; 
        CreateProjectile(l, x+sin(angle), y+cos(angle), portrait, attackType, speed, duration, shouldCallback, properties, targ, effects, len);

    }
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

    size_t len =  lua_rawlen(l,9);

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
        e = GetEffectFromTable(l, 9, i);
        e.from = currGameObjRunning;
        lua_remove(l,-1);
        effects[i-1] = e;
    }       
    CreateProjectile(l, x, y, portrait, attackType, speed, duration, shouldCallback, properties, targ, effects, len);

    return 1;
}
int L_SetAbilityCooldown(lua_State* l)
{
    const float cd = lua_tonumber(l,1);
    currAbilityRunning->cooldown = cd;
    return 0;
}
int L_SetMovePoint(lua_State* l)
{
    const float x = lua_tonumber(l,1);
    const float y = lua_tonumber(l,2);
    GameObject* target = GetClicked(x,y);
    if (target)
    {
        currGameObjRunning->targObj = target;
    }
    else
    {
        int w = al_get_bitmap_width(sprites[currGameObjRunning->spriteIndex].sprite);
        int h = al_get_bitmap_height(sprites[currGameObjRunning->spriteIndex].sprite);

        currGameObjRunning->xtarg = x-w/2;
        currGameObjRunning->ytarg = y-h/2;
        currGameObjRunning->targObj = NULL;
    }
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

    size_t len =  lua_rawlen(l,9);
    Effect effects[len];    
    memset(effects,0,sizeof(Effect)*len);
    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 9, i);
        e.from = currGameObjRunning;
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
    a.properties = properties;
    //a.callback_onhit = currAbilityRunning->luafunc_onhit;
    a.cameFrom = currAbilityRunning;
    a.ownedBy = currGameObjRunning;
    a.shouldCallback = shouldCallback;
    a.duration = duration;
    a.attackType = ATTACK_AOE;
    a.tickrate = tickrate;
    Attack* ref = AddAttack(&a);

    lua_pushnumber(l,ref - attacks);
    return 1;
}
int L_RemoveAttack(lua_State* l)
{
    RemoveAttack(lua_tonumber(l,1));
    return 0;
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
    return 0;
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
    for (int i = 0; i < numPrefabs; i++)
    {
        if (strcmp(prefabs[i].path,l_path) == 0)
        {
            g = &prefabs[i];
            prefabFound = true;
            break;
        }
    } 
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
       // g->baseDamage = 5;
        g->attackSpeed = 1;
        g->mana = 50;
        g->maxMana = 100;
        g->aggroRadius = 25;
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
        //g->baseDamage = 5;
        g->attackSpeed = 1;
        g->mana = 50;
        g->maxMana = 100;
        g->aggroRadius = 25;


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

    lua_pushinteger(l,TRIGGER_CONST);
    lua_setglobal(l,"TRIGGER_CONST");


    lua_pushinteger(l,EFFECT_MAXHP);
    lua_setglobal(l,"EFFECT_MAXHP");

    lua_pushinteger(l,EFFECT_DAMAGE);
    lua_setglobal(l,"EFFECT_DAMAGE");

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
int L_ApplyEffect(lua_State* l)
{
    int objIndex = lua_tonumber(l,1);
    if (objIndex < 0) 
        return -1; 
    if (objIndex >= MAX_OBJS) 
        return -1;

    size_t len =  lua_rawlen(l,2);

    for (int i = 1; i < len+1; i++)
    {
        Effect e;
        e = GetEffectFromTable(l, 2, i);
        e.from = currGameObjRunning;
        lua_remove(l,-1);
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
    Damage(&objects[objIndex],dmg);
    return 1;
}
int L_CastAbility(lua_State* l)
{
    int index = lua_tonumber(l,1);
        
    if (index < 0)
        index = 0;
    if (index > 3) 
        index = 3;
    if (!AbilityIsInitialised(&currGameObjRunning->abilities[index]))
        return 0;

    size_t len =  lua_rawlen(l,2);

    int x = -1; int y = -1; int obj = -1; float headingx=-1; float headingy=-1;

    bool isAField;
    for (int i = 0; i < len; i++)
    {
        lua_pushnumber(l,i+1);
        lua_gettable(l,2);

        x = GetTableField(l,-1,"x",&isAField);
        y = GetTableField(l,-1,"y",&isAField);
        obj = GetTableField(l,-1,"target",&isAField);
        headingx = GetTableField(l,-1,"headingx",&isAField);
        headingy = GetTableField(l,-1,"headingy",&isAField);
    }

    CastAbility(currGameObjRunning,&currGameObjRunning->abilities[index],x,y,headingx,headingy,&objects[obj]);
    return 1;
}
int L_AddAbility(lua_State* l)
{
    const char* path = lua_tostring(l,1);
    int index = lua_tonumber(l,2);
    if (index < 0) index = 0; 
    if (index > 3) index = 3;

    Ability* prefab = AddAbility(path); 
    currGameObjRunning->abilities[index] = CloneAbilityPrefab(prefab,l);
    //currGameObjRunning->abilities[index].ownedBy = currGameObjRunning;
    return 1;
}
int L_MoveAttack(lua_State* l)
{
    if (currAttackRunning)
    {
        int x = lua_tonumber(l,1);
        int y = lua_tonumber(l,2);
        currAttackRunning->x = x;
        currAttackRunning->y = y;
    }
    return 0;
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

    lua_pushcfunction(luaState, L_SetAbilityCooldown);
    lua_setglobal(luaState, "SetAbilityCooldown");

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

}