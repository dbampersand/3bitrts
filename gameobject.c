#include <dirent.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <ctype.h>

#include "allegro5/allegro_primitives.h"

#include "gameobject.h"

#include "luafuncs.h"
#include "rect.h"
#include "map.h"
#include "helperfuncs.h"
#include "colors.h"
#include "video.h"
#include "animationeffect.h"
#include "video.h"
#include "shield.h"
#include "player.h"
#include "ui.h"
#include "augment.h"
#include "encounter.h"
#include "damagenumber.h"
#include "gamestate.h"
#include "particle.h"
#include "settings.h"

GameObject* GetMousedOver(ALLEGRO_MOUSE_STATE* mouseState)
{
    for (int i = 0; i < numObjects; i++)
    {
        for (int i = 0; i < numObjects; i++)
        {
            GameObject* g = &objects[i];
            if (g->properties & OBJ_ACTIVE)
            {
                if (PointInRect(mouseState->x,mouseState->y,GetObjRect(g)))
                {
                    return(g);
                }
            }
        }
    }
    return NULL;
}
void UpdatePlayerObjectInteractions(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState)
{
    if (al_key_down(keyState, ALLEGRO_KEY_A) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_A))
    {
        players[0].amoveSelected = true;
    }
    if (players[0].abilityHeld) 
        players[0].amoveSelected = false;
    if (al_key_down(keyState, ALLEGRO_KEY_TAB) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_TAB))
    {
        players[0].indexSelectedUnit++;
        if (players[0].indexSelectedUnit >= MAXUNITSSELECTED || players[0].indexSelectedUnit >= players[0].numUnitsSelected)
        {
            players[0].indexSelectedUnit = 0;
        }
    }

}
void ProcessAttackMoveMouseCommand(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (players[0].amoveSelected)
    {
        players[0].amoveSelected = false;   
        for (int i = 0; i < numObjects; i++)
        {
            GameObject* g = &objects[i];
            if (IsSelected(g))
            {
                float w; float h; GetOffsetCenter(g,&w,&h);
                if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                    ClearCommandQueue(g);

                AttackMoveCommand(g,mouseState->x-w/2,mouseState->y-h/2);
                
            }

        }
    }
}
void AddMana(GameObject* g, float mana)
{
    g->mana += mana;
    if (g->mana < 0)
        g->mana = 0;
    if (g->mana > g->maxMana)
        g->mana = g->maxMana;
}
void UpdateMana(GameObject* g, float dt)
{
    float mana = g->manaRegen * dt;
    AddMana(g,mana);
}
void UpdateHPRegen(GameObject* g, float dt)
{
    float health = g->healthRegen * dt;
    Heal(g,health);
}
void SetHPRegen(GameObject* g, float regen)
{
    g->healthRegen = regen;
}
void SetManaRegen(GameObject* g, float regen)
{
    g->manaRegen = regen;
}

void UpdateObject(GameObject* g, float dt)
{
    currGameObjRunning = g;
    currGameObjRunning->offset.x = Towards(currGameObjRunning->offset.x,0,dt*2);
    currGameObjRunning->offset.y = Towards(currGameObjRunning->offset.y,0,dt*2);

    if (ObjIsDecoration(g))
    {
        if (currGameObjRunning->properties & OBJ_ACTIVE)
        {
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,g->luafunc_update);
            lua_pushnumber(luaState,dt);
            lua_pcall(luaState,1,0,0);
        }
    }
    if (g->deathTimerActivated)
    {
        g->deathTimer -= dt;
        if (g->deathTimer <= 0)
        {
            KillObj(g,true);
            return;
        }
    }
    UpdateMana(g, dt);
    UpdateHPRegen(g, dt);
    
    g->flashTimer -= dt;
    if (g->flashTimer < 0)
        g->flashTimer = 0;
    if (!IsActive(currGameObjRunning) || ObjIsDecoration(g))
        return;
    UpdateChannellingdObj(currGameObjRunning,dt);
    currGameObjRunning->invulnerableTime -= dt;
    if (currGameObjRunning->invulnerableTime < 0)
        currGameObjRunning->invulnerableTime = 0;
    if (currGameObjRunning->properties & OBJ_ACTIVE && !IsOwnedByPlayer(currGameObjRunning))
    {
        DoAI(currGameObjRunning);
    }
    DoCommands(currGameObjRunning);
    currGameObjRunning->attackTimer -= dt;
    if (currGameObjRunning->attackTimer < 0)
        currGameObjRunning->attackTimer = 0;
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        currGameObjRunning->abilities[i].cdTimer -= dt;
        if (currGameObjRunning->abilities[i].cdTimer < 0)
            currGameObjRunning->abilities[i].cdTimer = 0;

    }

    int w = al_get_bitmap_width(sprites[currGameObjRunning->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[currGameObjRunning->spriteIndex].sprite);

    Rect r = (Rect){currGameObjRunning->position.x,currGameObjRunning->position.y,w,h};

    bool shouldMove = true;
    bool shouldAttack = false;
    ProcessEffects(currGameObjRunning,dt);
    ProcessShields(currGameObjRunning,dt);

    if (currGameObjRunning->targObj && currGameObjRunning->queue[0].commandType == COMMAND_ATTACK) 
    {
        GameObject* tempAttack = currGameObjRunning->targObj;

        if (currGameObjRunning->properties & OBJ_ACTIVE)
        {
            int wTarg = al_get_bitmap_width(sprites[currGameObjRunning->targObj->spriteIndex].sprite);
            int hTarg = al_get_bitmap_height(sprites[currGameObjRunning->targObj->spriteIndex].sprite);

            currGameObjRunning->targetPosition.x = currGameObjRunning->targObj->position.x + wTarg/2;
            currGameObjRunning->targetPosition.y = currGameObjRunning->targObj->position.y + hTarg/2;

            Rect r2 = (Rect){currGameObjRunning->targObj->position.x,currGameObjRunning->targObj->position.y,wTarg,hTarg};
            #define DISTDELTA 0.001f
            Rect unioned = UnionRectR(r,r2);
            
            if (RectDist(currGameObjRunning,currGameObjRunning->targObj) < currGameObjRunning->range+DISTDELTA)
            {
                shouldMove = false;
                shouldAttack = true;
            }
            else
            {
                //if we're AI controlled and the object moves out of range 
                //but something is still in range - temporarily attack that, but keep moving towards the original target
                if (GetPlayerOwnedBy(currGameObjRunning) == 1)
                {
                    for (int i = 0; i < MAX_OBJS; i++)
                    {
                        GameObject* g2 = &objects[i];
                        if (IsActive(g2))
                        {
                            if (GetPlayerOwnedBy(g2) != GetPlayerOwnedBy(currGameObjRunning))
                            {
                                if (RectDist(currGameObjRunning,g2) < currGameObjRunning->range+DISTDELTA)
                                {
                                    tempAttack = g2;
                                    shouldMove = true;
                                    shouldAttack = true;
                                    break;
                                }
                                
                            }
                        }
                    }

                }
            }
        }   
        else
        {
            SetAttackingObj(currGameObjRunning, NULL);
        }
        GameObject* old = currGameObjRunning->targObj;
        currGameObjRunning->targObj = tempAttack;

        if (shouldAttack)
        {

            if (currGameObjRunning->attackTimer <= 0)
            {
                AttackTarget(currGameObjRunning);
                if (currGameObjRunning->targObj)
                {
                     currGameObjRunning->attackTimer = currGameObjRunning->attackSpeed;
                    GameObject* g = currGameObjRunning; GameObject* g2 = g->targObj;
                    float angle = (atan2(g2->position.y-g->position.y,g2->position.x-g->position.x));
                    
                    currGameObjRunning->offset.x = cos(angle);
                    currGameObjRunning->offset.y = sin(angle);

                }
                
            }

        }
        currGameObjRunning->targObj = old;
    }
    if (shouldMove)
        Move(currGameObjRunning, dt);
    if (currGameObjRunning->properties & OBJ_ACTIVE)
    {
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,g->luafunc_update);
        lua_pushnumber(luaState,dt);
        lua_pcall(luaState,1,0,0);
    }
}

void InitObjects()
{
    objects = calloc(MAX_OBJS,sizeof(GameObject));
    numObjects = 0;
    objectsAllocated = MAX_OBJS;

    freeObjs = calloc(MAX_OBJS,sizeof(GameObject*));
    numFreeObjs = MAX_OBJS;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        freeObjs[i] = &objects[i];
    }


    prefabs = calloc(1,sizeof(GameObject));
    numPrefabs = 0;
    numPrefabsAllocated = 1;
}
void GetControlGroup(ALLEGRO_KEYBOARD_STATE* keyState)
{
     if (!al_key_down(keyState,ALLEGRO_KEY_LCTRL))
    {
        if (al_key_down(keyState,ALLEGRO_KEY_1))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(1);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_2))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(2);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_3))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(3);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_4))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(4);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_5))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(5);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_6))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(6);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_7))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(7);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_8))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(8);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_9))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(9);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_0))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(0);
        }
    }  
}
void SetControlGroups(ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (al_key_down(keyState,ALLEGRO_KEY_LCTRL))
    {
        if (al_key_down(keyState,ALLEGRO_KEY_1))
        {
            SetCtrlGroup(1,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_2))
        {
            SetCtrlGroup(2,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_3))
        {
            SetCtrlGroup(3,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_4))
        {
            SetCtrlGroup(4,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_5))
        {
            SetCtrlGroup(5,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_6))
        {
            SetCtrlGroup(6,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_7))
        {
            SetCtrlGroup(7,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_8))
        {
            SetCtrlGroup(8,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_9))
        {
            SetCtrlGroup(9,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_0))
        {
            SetCtrlGroup(0,players[0].selection,players[0].numUnitsSelected);
        }
    }    
}

int GetCtrlGroup(int index)
{
    int count = 0; 
    for (int i = 0; i < MAXUNITSSELECTED; i++)
    {
        players[0].selection[i] = players[0].controlGroups[index][i];
        if (players[0].selection[i] != NULL)
        {
            count++;
            players[0].selection[i]->properties |= OBJ_SELECTED;
        }
    }
    return count; 

}


void SetCtrlGroup(int index, GameObject** list, int numUnitsSelected)
{
    if (list)
    {
        numUnitsSelected = numUnitsSelected > MAXUNITSSELECTED ? MAXUNITSSELECTED : numUnitsSelected;
        for (int i = 0; i < MAXUNITSSELECTED; i++)
        {
            players[0].controlGroups[index][i] = list[i];
        }
    }
}

void CheckSelected(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseLastFrame, ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (!(mouseLastFrame->buttons & 1)  && (mouseState->buttons & 1) && !players[0].abilityHeld)
    {
        AddMouseRandomParticles(*mouseState, 3);
        players[0].selecting = true;
        players[0].selectionStart = (Vector2){mouseState->x,mouseState->y};
    }
    if (players[0].selecting)
    {
    }
    if (!(mouseState->buttons & 1))
    {
        if (players[0].selecting)
        {
            Vector2 endSelection = (Vector2){mouseState->x,mouseState->y};
            Rect r;
            r.x = _MIN(endSelection.x,players[0].selectionStart.x);
            r.y = _MIN(endSelection.y,players[0].selectionStart.y);
            r.w = _MAX(endSelection.x,players[0].selectionStart.x) - _MIN(endSelection.x,players[0].selectionStart.x);
            r.h = _MAX(endSelection.y,players[0].selectionStart.y) - _MIN(endSelection.y,players[0].selectionStart.y);
            bool hasSelected = false;
            
            for (int i = 0; i < numObjects; i++)
            {
                GameObject* obj = &objects[i];
                if (!IsOwnedByPlayer(obj) || !IsActive(obj) || ObjIsDecoration(obj))
                    continue;
                Sprite* sp = &sprites[obj->spriteIndex];
                int j = al_get_bitmap_width(sp->sprite);
                Rect rObj = (Rect){obj->position.x,obj->position.y,al_get_bitmap_width(sp->sprite),al_get_bitmap_height(sp->sprite)};
                if (CheckIntersect(rObj,r))
                {
                    if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                    if (!hasSelected)
                    {
                        for (int j = 0; j < numObjects; j++)
                        {
                            SetSelected(&objects[j],false);
                            for (int i = 0; i < players[0].numUnitsSelected; i++)
                            {
                                players[0].selection[i] = NULL;
                            }
                            players[0].numUnitsSelected = 0;
                        }
                        hasSelected = true;
                    }
                    if (al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                    {
                        bool selected = IsSelected(obj);
                        SetSelected(obj,!selected);
                        //we're removing the unit from selection as it is already selected 
                        if (selected)
                        {
                            RemoveGameObjectFromSelection(&players[0],obj);
                        }
                    }
                    else
                    {
                        SetSelected(obj,true);
                        //players[0].selection[players[0].numUnitsSelected] = obj;
                        //players[0].numUnitsSelected++;

                    }
                    if (hasSelected) 
                        players[0].indexSelectedUnit = 0; 
                }
            }
        }
        players[0].selecting = false;
    }
    if (!(mouseState->buttons & 2) && (mouseLastFrame->buttons & 2))
    {
        if (!IsInsideUI(mouseState->x,mouseState->y))
        {
            if (players[0].abilityHeld == NULL)
                    {
                        AddMouseRandomParticles(*mouseState, 3);
                    }
                    for (int i = 0; i < players[0].numUnitsSelected; i++)
                    {
                        SetAttackingObj(players[0].selection[i],NULL);
                    }
                    for (int i = 0; i < MAXUNITSSELECTED; i++)
                    {
                    
                        GameObject* g = players[0].selection[i];
                        if (!IsActive(g))
                            continue;
                        if (IsSelected(g))
                        {
                            int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
                            int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
                                if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                                    ClearCommandQueue(g);
                            bool found = false;
                            for (int i = 0; i < MAX_OBJS; i++)
                            {
                                GameObject* g2 = &objects[i];
                                if (g == g2) continue;

                                Rect r = GetObjRect(g2);
                                if (PointInRect(mouseState->x,mouseState->y,r))
                                {
                                    found = true;
                                    AttackCommand(g,g2);
                                    break;
                                }
                            }
                            if (!found)
                                MoveCommand(g,mouseState->x-w/2,mouseState->y-h/2);
                        
                        }
                        Sprite* s = &sprites[g->spriteIndex];
                        Rect r = (Rect){g->position.x,g->position.y,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)}; 
                        if (PointInRect(mouseState->x,mouseState->y,r))
                        {
                            for (int i = 0; i < players[0].numUnitsSelected; i++)
                            {
                                if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                                    ClearCommandQueue(players[0].selection[i]);
                                if (!ObjIsInvincible(g))
                                    AttackCommand(players[0].selection[i],g);
                                else
                                    MoveCommand(players[0].selection[i],mouseState->x,mouseState->y);
                            }
                            break;
                        }

                    }
        }
    
        
    }
}
int GetNumObjectsInRect(Rect* r)
{
    int j = 0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (!IsActive(&objects[i]) || ObjIsDecoration(&objects[i]))
            continue;
        Rect r2 = GetObjRect(&objects[i]);
        if (CheckIntersect(*r,r2))
        {
            j++;
        }
    }
    return j;
}
void RemoveObjFromSelection(GameObject* g)
{
    for (int i = 0; i < MAXUNITSSELECTED; i++)
    {
        if (players[0].selection[i] == g)
        {
            for (int j = i; j < MAXUNITSSELECTED-1; j++)
            {
                players[0].selection[j] = players[0].selection[j+1];
            }
            players[0].selection[MAXUNITSSELECTED-1] = NULL;

            players[0].numUnitsSelected--;
            if (players[0].indexSelectedUnit == i)
            {
                players[0].indexSelectedUnit--;
                if (players[0].indexSelectedUnit < 0) 
                    players[0].indexSelectedUnit = 0;
            }
            break;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < MAXUNITSSELECTED; j++)
        {
            if (players[0].controlGroups[i][j] == g)
            {
                for (int z = j; z < MAXUNITSSELECTED-1; z++)
                {
                    players[0].controlGroups[i][z] =  players[0].controlGroups[i][z+1];
                }
                players[0].controlGroups[i][MAXUNITSSELECTED-1] = NULL;

            }
        }

    }
}
GameObject* AddGameobject(GameObject* prefab, float x, float y)
{
    GameObject* found = NULL;

    for (int i = 0; i < numFreeObjs; i++)
    {
        if (freeObjs[i])
        {       
            found = freeObjs[i];
            for (int j = i; j < numFreeObjs-1; j++)
            {
                freeObjs[j] = freeObjs[j+1];
            }
            freeObjs[numFreeObjs-1] = NULL;
            break;
        }
    }
    if (!found)
        return NULL;

    
    *found = *prefab;
    currGameObjRunning = found; 
    found->properties |= OBJ_ACTIVE;


    memset(found->abilities,0,sizeof(Ability)*4);
    memset(currGameObjRunning,0,sizeof(GameObject));

    currGameObjRunning->lua_buffer = prefab->lua_buffer; 
    currGameObjRunning->path = prefab->path;
    currGameObjRunning->name = prefab->name;
    currGameObjRunning->threatMultiplier = 1;

    currGameObjRunning->range = 1;
    currGameObjRunning->objType = TYPE_ALL;

    currGameObjRunning->health = 100;
    currGameObjRunning->maxHP = 100;

    currGameObjRunning->range = 1;
    currGameObjRunning->attackSpeed = 1;

    currGameObjRunning->mana = 50;
    currGameObjRunning->maxMana = 100;
    currGameObjRunning->manaRegen = 1;

    currGameObjRunning->aggroRadius = 25;

    SetMoveSpeed(currGameObjRunning,50);
    SetHPRegen(currGameObjRunning,1);
    //currGameObjRunning->speed = 50;

    loadLuaGameObj(luaState, found->path, found); 
    found->properties |= OBJ_ACTIVE;


    currGameObjRunning->position.x = _SCREEN_SIZE/2.0f;
    currGameObjRunning->position.y = _SCREEN_SIZE/2.0f;

    Teleport(currGameObjRunning,x,y);



    found->attackSpeed = 1;
    found->mana = 50;
    found->maxMana = 100;
    currGameObjRunning->prefab = prefab;

    numObjects++;

    return found;
}
void NewObj(GameObject* g)
{

}
void AddDamage(GameObject* g, float value)
{
    if (!g) return;
    g->baseDamage += value;
}
bool CheckFuncExists(const char* funcName, char* lua_buffer)
{
    char* c = strstr(lua_buffer,funcName);
    char* full = c + strlen(funcName);

    if (c == NULL) return false;
    while (c < lua_buffer + strlen(lua_buffer))
    {
        while (c > lua_buffer)
        {
            //if we've hit a newline
            if (iscntrl(*c) || *c == '\n')
            {
                return true;
            }
            //if its a comment
            if (*c == '-')
            {
                c = strstr(full,funcName);
                full = c + strlen(funcName);

                if (c == NULL)
                    return false;

                continue;
            }
            //if it's in a string
            if (*c == '"' || *c ==  '\'')
            {
                c = strstr(full,funcName);
                full = c + strlen(funcName);

                if (c == NULL)
                    return false;
                continue;
            }
            c++;
        }
    }
    return false;
}
bool ObjHasType(GameObject* g, GAMEOBJ_TYPE_HINT typeHint)
{
    int i = g->objType;
    int j = typeHint;

    bool b = ((g->objType & typeHint));
    return (g->objType & typeHint);
}
void loadLuaGameObj(lua_State* l, const char* filename, GameObject* g) 
{
    char* cpy;
    cpy = calloc(strlen(filename)+1,sizeof(char));
    strcpy(cpy,filename);

    currGameObjRunning = g;
    
    if (g)
    {
        if (g->lua_buffer)
        {

        }
        else
        {
            memset(g,0,sizeof(GameObject));    
            LoadLuaFile(filename,g);
        }
    }
    else
    {

    }
    if (luaL_loadbuffer(l, g->lua_buffer,strlen(g->lua_buffer),NULL) || lua_pcall(l, 0, 0, 0))
     {
         printf("%s\n\n---\nCan't load lua file:\n %s Filename: %s\n---\n\n\n",COL_ERR,lua_tostring(l,-1),filename);
         fflush(stdout);
     }
     else
     {
        int funcIndex;
        if (CheckFuncExists("update",g->lua_buffer))
        {
            lua_getglobal(l, "update");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_update = funcIndex;
        }
        else
            g->luafunc_update = -1;

        if (CheckFuncExists("setup",g->lua_buffer))
        {
            lua_getglobal(l, "setup");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            lua_rawgeti(l,LUA_REGISTRYINDEX,funcIndex);
            lua_pcall(l,0,0,0);
            g->luafunc_setup = funcIndex;
        }
        else
            g->luafunc_setup = -1;


        if (CheckFuncExists("kill",g->lua_buffer))
        {
            lua_getglobal(l, "kill");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_kill = funcIndex;
        }
        else
            g->luafunc_kill = -1;

        if (CheckFuncExists("OnAttack",g->lua_buffer))
        {
            lua_getglobal(l, "OnAttack");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_onattack = funcIndex;
        }
        else
            g->luafunc_onattack = -1;


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
        if (!g->name)
        {
            g->name = calloc(strlen(two)+1,sizeof(char));
            strcpy(g->name,two);
        }
        else if (strlen(g->name) == 0)
        {
            g->name = realloc(g->name,(strlen(two)+1)*sizeof(char));
            strcpy(g->name,two);

        }
        g->path = calloc(strlen(filename)+1,sizeof(char));
        strcpy(g->path,filename);
        //SET_BIT(&g->properties,0);

     }
     free(cpy);
}
void KillObj(GameObject* g, bool trigger)
{
    if (!g) return;
    if (trigger)
    {
        if (!IsOwnedByPlayer(g))
        {
            if (!ObjIsDecoration(g))
            {
                if (HasAugment(currEncounterRunning,AUGMENT_BAD_DEATHINCDMG))
                {
                    Bad_AugmentDeathAddDamage(g, currEncounterRunning->augment);
                }
                if (HasAugment(currEncounterRunning,AUGMENT_BAD_ENEMY_EXPLODES))
                {
                    Bad_AugmentDeathAddDamage(g, currEncounterRunning->augment);
                }
                if (HasAugment(currEncounterRunning,AUGMENT_BAD_ENEMY_EXPLODES) && GetPlayerOwnedBy(g) == 1)
                {
                    Bad_EnemyExplodes(g,currEncounterRunning->augment);
                }
            }

        }
    }

    g->properties &= ~OBJ_ACTIVE;
    g->spriteIndex = 0;
    DeleteThreatList(g);
    if (g->shields)
    {
        free(g->shields);
        g->shields = NULL;
    }
    if (g->onAttackEffectsIndices)
    {
        free(g->onAttackEffectsIndices);
        g->onAttackEffectsIndices = NULL;
    }
    RemoveObjFromSelection(g);

    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        if (g->abilities[i].toggled)
        {
            ToggleAbility(&g->abilities[i],g,false);
        }
        if (g->abilities[i].description)
        {
            free(g->abilities[i].description);
            g->abilities[i].description = NULL;
        }
    }
    if (g->channelledAbility)
    {
        
    }
}   

void LoadFolderPrefabs(const char* dirPath, char* name)
{
    DIR *d;

    struct dirent *dir;
    d = opendir(dirPath);

    char* file = calloc(strlen(name)+strlen(".lua")+1,sizeof(char));
    strcpy(file,name);
    strcat(file,".lua");


    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG &&  strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {

                if (strcasecmp(dir->d_name,file) == 0)
                {
                    char* dirConcat = calloc(strlen(dirPath)+strlen("/")+strlen(".lua")+strlen(dir->d_name)+1,sizeof(char));
                    strcpy(dirConcat,dirPath);
                    strcat(dirConcat,dir->d_name);
                    GameObject g;
                    //loadLuaGameObj(luaState,dirConcat,&g);
                    LoadPrefab(dirConcat);
                    free(dirConcat);
                }
            }
        }
    }
    free(file);


}
GameObject* LoadPrefab(const char* path)
{
    for (int i = 0; i < numPrefabs; i++)
    {
        if (strcasecmp(prefabs[i].path,path)==0)
        {
            return &prefabs[i];
        }
    }
    GameObject g;
    memset(&g,0,sizeof(GameObject));
    loadLuaGameObj(luaState,path,&g);

    if (numPrefabs  >= numPrefabsAllocated)
    {
        numPrefabsAllocated += BUFFER_PREALLOC_AMT;
        prefabs = realloc(prefabs,(numPrefabsAllocated)*sizeof(GameObject));
    }
    prefabs[numPrefabs] = g;
    numPrefabs++;

    return &prefabs[numPrefabs-1];
}
void MakeInvulnerable(GameObject* g, float time)
{
    g->invulnerableTime += time;
}
void LoadPrefabs(const char* dirPath)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);
    
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                char* dirConcat = calloc(strlen(dirPath)+strlen(dir->d_name)+3,sizeof(char));
                strcpy(dirConcat,dirPath);
                strcat(dirConcat,"/");
                strcat(dirConcat,dir->d_name);
                strcat(dirConcat,"/");
                LoadFolderPrefabs(dirConcat,dir->d_name);
                free(dirConcat);
            }
        }
    }
}
bool IsSelected(GameObject* g)
{
    return g->properties & OBJ_SELECTED;
}
void SetSelected(GameObject* g, bool select)
{
    if (select)
    {
        g->properties |= OBJ_SELECTED;
        players[0].selection[players[0].numUnitsSelected] = g;
        players[0].numUnitsSelected++;
    }
    else
    {
        g->properties &= ~OBJ_SELECTED;
    }


}
bool IsOwnedByPlayer(GameObject* g)
{
    if (!g) return false;
    return (g->properties & OBJ_OWNED_BY) == 0;
}
int GetPlayerOwnedBy(GameObject* g)
{
    if (!g) 
        return -1;
    return (g->properties & OBJ_OWNED_BY) > 0 ? 1 : 0;
}
void SetOwnedBy(GameObject* g, int i)
{
    if (i == TYPE_FRIENDLY)
    {
        g->properties &= ~OBJ_OWNED_BY;
    }
    else
    {
        g->properties |= OBJ_OWNED_BY;
    }
}
void CheckCollisions(GameObject* g, bool x, float dV, bool objectCanPush)
{
    if (!g)
    if (dV == 0) return;
    Sprite* s = &sprites[g->spriteIndex];
    Rect rG = (Rect){g->position.x,g->position.y,GetWidth(g),GetHeight(g)};
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g2 = &objects[i];
        if (!IsActive(g2) || g2 == g) 
            continue;
        Sprite* s2 = &sprites[g2->spriteIndex];
        Rect r2 = (Rect){g2->position.x,g2->position.y,GetWidth(g2),GetHeight(g2)};

        if (!CheckIntersect(rG,r2))
        {
            continue;
        }
        if (x)
        {
            if (dV > 0)
            {
                if (1)
                {
                    //moving right
                    if (objectCanPush)
                    {
                        float v = g2->position.x;
                        g2->position.x = g->position.x + GetWidth(g);
                        v -= g2->position.x;
                        if (g2->position.x <0)
                        {
                            v = -1;
                            g2->position.x=0;
                        }
                        if (g2->position.x + GetWidth(g2) > _MAPSIZE)
                        {   
                            g2->position.x = _MAPSIZE - GetWidth(g2);
                            v = 1;
                        }

                        float v2 = g2->position.x;
                        CheckCollisions(g2,true,-v,true);
                        v2 -= g2->position.x;
                        CheckCollisionsWorld(g2,true,1);

                    }
                    else
                    {
                        g->position.x = g2->position.x - al_get_bitmap_width(s2->sprite);
                        rG.x = g->position.x;
                    }

                }
            }
            else if (dV < 0)
            {
                if (1)
                {
                    //moving left
                    if (objectCanPush)
                    {
                        float v = g2->position.x;
                        g2->position.x = g->position.x - GetWidth(g2);

                        v -= g2->position.x;
                        if (g2->position.x < 0)
                        {
                            v = -1;
                            g2->position.x=0;
                        }
                        if (g2->position.x + GetWidth(g2) > _MAPSIZE)
                        {
                            g2->position.x = _MAPSIZE - GetWidth(g2);
                            v = 1;
                        }

                        float v2 = g2->position.x;
                        CheckCollisions(g2,true,-v,true);
                        v2 -= g2->position.x;

                        CheckCollisionsWorld(g2,true,-1);
                       
                    }
                    else
                    {
                        g->position.x = g2->position.x+al_get_bitmap_width(s2->sprite); 
                        rG.x = g->position.x;
                    }
                }
            }
        }
        else
        {
            if (1)
            {

                if (dV > 0)
                {
                    //moving down
                    if (objectCanPush)
                    {
                        float v = g2->position.y;
                        g2->position.y = g->position.y + GetHeight(g);
                        v -= g2->position.y;
                        float v2 = g2->position.y;
                        if (g2->position.y <0)
                        {
                            v = 1;
                            g2->position.y=0;
                        }
                        if (ObjectIsInUI(g2))
                        {
                            g2->position.y = UI_START_Y - GetHeight(g2);
                            v = 1;
                        }

                        CheckCollisions(g2,false,-v,true);
                        v2 -= g2->position.y;
                        CheckCollisionsWorld(g2,false,-v);
                    }
                    else
                    {
                        g->position.y = g2->position.y - al_get_bitmap_height(s->sprite);
                        rG.y = g->position.y;
                    }
                }
                else if (dV < 0)
                {
                    //moving up
                    if (objectCanPush)
                    {
                        float v = g2->position.y;
                        g2->position.y = g->position.y - GetHeight(g2);
                        v -= g2->position.y;
                        float v2 = g2->position.y;
                        if (g2->position.y <0)
                        {
                            v = -1;
                            g2->position.y=0;
                        }
                        if (ObjectIsInUI(g2))
                        {
                            g2->position.y = UI_START_Y - GetHeight(g2);
                            v2 = -1;
                        }

                        CheckCollisions(g2,false,-v,true);
                        v2 -= g2->position.y;
                        CheckCollisionsWorld(g2,false,-v);
                    }
                    else
                    {
                        g->position.y = g2->position.y + al_get_bitmap_height(s2->sprite);
                        rG.y = g->position.y;
                    }
                }
            }
        }
    }
}
GameObject* GetCollidedWith(GameObject* g)
{
    Sprite* s = &sprites[g->spriteIndex];
    Rect rG = (Rect){g->position.x,g->position.y,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)};
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g2 = &objects[i];
        if (g2 == g || !IsActive(g2)) 
            continue;
        Sprite* s2 = &sprites[g2->spriteIndex];
        Rect r2 = (Rect){g2->position.x,g2->position.y,al_get_bitmap_width(s2->sprite),al_get_bitmap_height(s2->sprite)};

        if (!CheckIntersect(rG,r2))
        {
            continue;
        }
        return g2;
    }
    return NULL;

}
bool ObjectIsInUI(GameObject* g)
{
    if (g->position.y + GetHeight(g) > UI_START_Y)
    {
        return true;
    }
    return false;

}
void CheckCollisionsWorld(GameObject* g, bool x, float dV)
{
        int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
    
    if (ObjectIsInUI(g))
    {
        g->position.y = UI_START_Y - GetHeight(g);
    }

    float posX = g->position.x;
    float posY = g->position.y;

    if (posX < 0 || posX+w > _MAPSIZE)
    {
        if (posX < 0) 
        {
            g->position.x = 0;
            CheckCollisions(g,true,1,true);

        }
        if (posX+w > _MAPSIZE) 
        {
            g->position.x = _MAPSIZE-w;
            CheckCollisions(g,true,-1,true);

        }
        return;

    }
    if (posY < 0 || posY+h > _MAPSIZE)
    {
        if (posY < 0)
        {
            g->position.y = 0;
            CheckCollisions(g,false,1,true);

        } 
        if (posY+h > _MAPSIZE)
        {
            g->position.y = _MAPSIZE-h;
            CheckCollisions(g,false,-1,true);
        } 

        return;
    }

    if (dV == 0) return;
    
    int indexTop = GetIndex(_MAPSIZE/_GRAIN, floor(posX/ (float)_GRAIN), floor(posY / (float)_GRAIN));
    int indexRight = GetIndex(_MAPSIZE/_GRAIN, floor((posX+w) / (float)_GRAIN), floor((posY) / (float)_GRAIN));
    int indexBottom = GetIndex(_MAPSIZE/_GRAIN, floor((posX) / (float)_GRAIN), floor((posY+h) / (float)_GRAIN));
    int indexLeft = GetIndex(_MAPSIZE/_GRAIN, floor((posX) / (float)_GRAIN), floor((posY) / (float)_GRAIN));

    if (x)
    {
        if (dV < 0)
        {
            if (currMap->collision[indexLeft] == false)
            {
               // printf("gg");
                g->position.x = ((indexLeft/(_MAPSIZE/_GRAIN))*_GRAIN)+_GRAIN;
                CheckCollisions(g,true,1,true);

            }
        }
        if (dV > 0)
        {
            if (currMap->collision[indexRight] == false)
            {
                g->position.x = (indexRight/(_MAPSIZE/_GRAIN))*_GRAIN - w;
                CheckCollisions(g,true,-dV,true);
            }
        }

    }
    else
    {
        if (dV < 0)
        {
            if (currMap->collision[indexTop] == false)
            {
                int yCoord = (indexTop%(_MAPSIZE/_GRAIN)+1)*_GRAIN;
                g->position.y = yCoord;
                CheckCollisions(g,false,-dV,true);
            }

        }
        if (dV > 0)
        {
            if (currMap->collision[indexBottom] == false)
            {
                int yCoord = (indexBottom%(_MAPSIZE/_GRAIN))*_GRAIN - h;
                g->position.y = yCoord;
                CheckCollisions(g,false,-dV,true);
            }
        }
    }
}
void Move(GameObject* g, float delta)
{
    if (!g) return;
    #define DIST_DELTA 1

    int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
    float xtarg;
    float ytarg;

    if (!g->targObj)
    {
        xtarg = g->targetPosition.x;
        ytarg = g->targetPosition.y;
    }
    else
    {
        GetCentre(g->targObj,&xtarg,&ytarg);
    }

        float moveX = xtarg - g->position.x;
        float moveY = ytarg - g->position.y;

        float dist = sqrt(moveX*moveX+moveY*moveY);

        float dX = (moveX / dist * g->speed) * delta;
        float dY = (moveY / dist * g->speed) * delta;

        if (dist <= DIST_DELTA)
        {
            g->position.x = xtarg;
            CheckCollisions(g,true, dX,ObjectCanPush(g));
            CheckCollisionsWorld(g,true, dX);

            g->position.y = ytarg;
            CheckCollisions(g,false, dY,ObjectCanPush(g));
            CheckCollisionsWorld(g,false, dY);


            return;
        }
        double mDist = sqrt(dX * dX + dY * dY);
        if (dist <= mDist)
        {
            dX = xtarg - g->position.x;
            dY = ytarg - g->position.y;
            
            g->position.y=ytarg;
            CheckCollisions(g,false, dY,ObjectCanPush(g));
            CheckCollisionsWorld(g,false, dY);
            g->position.x=xtarg;

            CheckCollisions(g,true, dX,ObjectCanPush(g));
            CheckCollisionsWorld(g,true, dX);

            g->targetPosition.x = xtarg;
            g->targetPosition.y = ytarg;
            
            return;
        }


        g->position.y += dY;
        CheckCollisions(g,false, dY,ObjectCanPush(g));
        CheckCollisionsWorld(g,false, dY);

        g->position.x += dX;
        CheckCollisions(g,true, dX,ObjectCanPush(g));
        CheckCollisionsWorld(g,true, dX);


}
bool ObjectCanPush(GameObject* g)
{
    return (g->properties & OBJ_CAN_PUSH);
}
void DrawHealthBar(GameObject* g, ALLEGRO_COLOR col)
{
    Sprite* s = &sprites[g->spriteIndex];
    int padding = 4;

    Rect r;
    r.x = g->position.x + g->offset.x;
    r.y = g->position.y - padding + g->offset.y;
    r.w = al_get_bitmap_width(s->sprite);
    r.h = 2;

    al_draw_rectangle((int)r.x,(int)r.y,(int)r.x+r.w,(int)r.y+r.h,col,1);

    float percent = (g->health) / (float)g->maxHP;

    float shield = GetTotalShield(g);
    float percentShield = shield/g->maxHP;
    if (percentShield > 1) percentShield = 1;
    int numPixels = percent * r.w;
    int numPixelsShield = percentShield*r.w;
    
    al_draw_filled_rectangle((int)r.x,(int)r.y,(int)(r.x+numPixels),(int)(r.y+r.h),col);
    al_draw_filled_rectangle((int)r.x,(int)r.y-5,(int)(r.x+numPixelsShield),(int)((r.y-5)+r.h-1),col);

    
}

void DrawArrow(int cx, int cy, int targetx, int targety, ALLEGRO_COLOR color)
{
    int arrowangle = 215;

    al_draw_line(cx,cy,targetx,targety,color,1);
    Point rotated = (Point){cx-targetx,cy-targety};
    float rx = rotated.x * cos(DegToRad(arrowangle)) - rotated.y * sin(DegToRad(arrowangle)) + cx;
    float ry = rotated.x * sin(DegToRad(arrowangle)) + rotated.y * cos(DegToRad(arrowangle)) + cy;
    float r2x = rotated.x * cos(DegToRad(-arrowangle)) - rotated.y * sin(DegToRad(-arrowangle)) + cx;
    float r2y = rotated.x * sin(DegToRad(-arrowangle)) + rotated.y * cos(DegToRad(-arrowangle)) + cy;

    al_draw_line(cx,cy,rx,ry,color,1);
    al_draw_line(cx,cy,r2x,r2y,color,1);

}
void DrawObjShadow(GameObject* g)
{
    int x = g->position.x + g->offset.x;
    int y = g->position.y + g->offset.y;
    int w = GetWidth(g);
    int h = GetHeight(g);

    int lineW = (floor(w/16.0f));
    int lineH = (floor(h/16.0f));

    lineW = lineW == 0 ? 1 : lineW;
    lineH = lineH == 0 ? 1 : lineH;
    Rect r = (Rect){x+1,y+1,w+lineW-1,h+lineH-1};
    DrawRoundedRect(r,BG,true);
}
void DrawObjShadows()
{
    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (IsActive(&objects[i]) && !ObjIsDecoration(&objects[i]))
            DrawObjShadow(&objects[i]);
    }
}
void DrawGameObj(GameObject* g, bool forceInverse)
{
    if (!(g->properties & OBJ_ACTIVE))
        return;
    
    bool b = IsOwnedByPlayer(g);
    ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
    if (ObjIsDecoration(g))
        c = BG;
    if (IsOwnedByPlayer(g))
    {
        DrawMapHighlight(g,30);
    }

    RedrawMapSegmentUnderObj(g);
        
    Sprite* s = &sprites[g->spriteIndex];
    bool isReversed = IsSelected(g) || forceInverse;
    isReversed = g->flashTimer > 0 ? !isReversed : isReversed;

    float x = g->position.x + g->offset.x; float y = g->position.y + g->offset.y;
    DrawSprite(s,x,y,0.5f,0.5f,g->angle,c, isReversed);
   
    Rect selectRect;
    selectRect.w = al_get_bitmap_width(s->sprite);
    selectRect.h = al_get_bitmap_height(s->sprite);
    selectRect.x = x;
    selectRect.y =y;
    if (!ObjIsInvincible(g))
    {
        DrawRoundedRect(selectRect, c,false);

        if (currSettings.displayHealthBar == OPTION_HPBAR_ALWAYS)
            DrawHealthBar(g,c);
        else if (currSettings.displayHealthBar == OPTION_HPBAR_SELECTED && (IsOwnedByPlayer(g) && IsSelected(g)))
            DrawHealthBar(g,c);
        else if (currSettings.displayHealthBar == OPTION_HPBAR_NEVER && (!IsOwnedByPlayer(g)))
            DrawHealthBar(g,c);
    }

    if ((g->queue[0].commandType == COMMAND_ATTACK || g->queue[0].commandType == COMMAND_CAST) && g->queue[0].target)
    {
        Point c1; GetCentre(g,&c1.x,&c1.y);
        Point c2; GetCentre(g->queue[0].target,&c2.x,&c2.y);

        float headingX = c1.x - c2.x;
        float headingY = c1.y - c2.y;
        Normalize(&headingX,&headingY);
        
        int offsetX; int offsetY;
        c1.x = c1.x - (headingX * (GetWidth(g)+1));
        c1.y = c1.y - (headingY * (GetHeight(g)+1));
        c2.x = c1.x + (headingX * 5);
        c2.y = c1.y + (headingY * 5);

        DrawArrow(c1.x,c1.y,c2.x,c2.y,c);
    }
    if (g->queue[0].commandType == COMMAND_MOVE || g->queue[0].commandType == COMMAND_ATTACKMOVE)
    {
        Point c1; GetCentre(g,&c1.x,&c1.y);
        Point c2; c2.x = g->queue[0].x+GetWidth(g)/2.0f; c2.y = g->queue[0].y+GetHeight(g)/2.0f;

        float headingX = c1.x - c2.x;
        float headingY = c1.y - c2.y;

        Normalize(&headingX,&headingY);
        float circleCenterX = c1.x - (headingX * (GetWidth(g)+3));
        float circleCenterY = c1.y - (headingY * (GetHeight(g)+3));
        
        al_draw_filled_circle(circleCenterX,circleCenterY,2,c);
    }
    
}
void DrawMapHighlight(GameObject* g, int lightSize)
{
    //Use circle algorithm to get the points to end at
    //Then calculate the angle between gCX and gCY and that point
    //Then go from g to that point
    float cx; float cy;
    GetCentre(g,&cx,&cy);
    int r = lightSize;
    int x = -r;
    int y = 0;
    int err = 2-2*r;

    al_lock_bitmap(sprites[currMap->spriteIndex].sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    while (x <= 0)
    {

        Point points[4];
        points[0].x = cx-x;  points[0].y = cy+y;
        points[1].x = cx-y;  points[1].y = cy-x;
        points[2].x = cx+x;  points[2].y = cy-y; 
        points[3].x = cx+y;  points[3].y = cy+x; 

        for (int i = 0; i < 4; i++)
        {
            float angle = PointsToAngleRad(cx,cy,points[i].x,points[i].y);
            float moveX = cos(angle);
            float moveY = sin(angle);
            int steps = 0;
            float mX = cx;
            float mY = cy;

            while (steps < lightSize)
            {
                mX += moveX;
                mY += moveY;
                ALLEGRO_COLOR col = al_get_pixel(sprites[currMap->spriteIndex].sprite,mX,mY);
                if (col.a <= 0.001f)
                {
                    al_draw_pixel(mX - moveX,mY - moveY,EDGE_HIGHLIGHT);
                    break;
                }
                steps++;
            }
        } 
      r = err;
      if (r <= y) 
      {
        y++;
        err += y * 2 + 1;       

      }
      if (r > x || err > y) 
      {
        x++;
        err += x * 2 + 1;
      }


     }
    al_unlock_bitmap(sprites[currMap->spriteIndex].sprite);


}
void SetAttackingObj(GameObject* g, GameObject* target)
{
    g->targObj = target;
}
void AttackTarget(GameObject* g)
{
    if (!g) return;
    if (!g->targObj) return;
    if (GetPlayerOwnedBy(g) == GetPlayerOwnedBy(g->targObj))
    {
        return;
    }
    if (g->targObj == g)
    {
        return;
    }
    if (g->targObj)
    {
        if (g->luafunc_onattack >= 0)
        {
            CallLuaFunc(g->luafunc_onattack);
        }
        int damage = g->baseDamage;
       //    g->targObj->health -= damage   ;

        if (g->numAttackEffectIndices > 0)
        {

            int randomAtk = g->onAttackEffectsIndices[rand() % g->numAttackEffectIndices];
            AnimationEffect* ae =  &animationEffectsPrefabs[randomAtk];

            
            Rect r1 = GetObjRect(g);
            Rect r2 = GetObjRect(g->targObj);
            
            int midX = (r1.x+r1.w/2 + r2.x+r2.w/2) / 2;
            int midY = (r1.y+r1.h/2 + r2.y+r2.h/2) / 2;

            midX -= ae->rect.w/2;
            midY -= ae->rect.h/2;



            AddAnimationEffect_Prefab(ae, g->properties & OBJ_OWNED_BY,midX,midY);
        }


        AddThreat(g,g->targObj,damage);
        if (Damage(g,g->targObj,damage))
        {
            g->targObj = NULL;
        }

    }
}
Rect GetObjRect(GameObject* g)
{
    if (!g) return (Rect){0,0,0,0};
    if (g->spriteIndex <= 0) return (Rect){g->position.x,g->position.y,0,0};
    Rect r = (Rect){g->position.x,g->position.y,al_get_bitmap_width(sprites[g->spriteIndex].sprite),al_get_bitmap_height(sprites[g->spriteIndex].sprite)};
    return r;
}
void SetMoveSpeed(GameObject* g, float value)
{
    if (IsOwnedByPlayer(g))
    {
        if (HasAugment(currEncounterRunning,AUGMENT_GOOD_MOVESPEED))
        {
            value += Good_GetAugmentMoveSpeed(value,currEncounterRunning->augment);
        }
    }
    else
    {
        if (HasAugment(currEncounterRunning,AUGMENT_BAD_MOVESPEED))
        {
            value += Bad_GetAugmentMoveSpeed(value,currEncounterRunning->augment);
        }

    }
    g->speed = value;
}

bool Damage(GameObject* source, GameObject* g, float value)
{
    if (!g) return false;
    if (!(g->properties & OBJ_ACTIVE)) return false;
    if (g->invulnerableTime > 0)
    {
        return false;
    }
    if (HasAugment(currEncounterRunning,AUGMENT_NEUTRAL_TOTALDAMAGE))
    {
        value += Neutral_GetAugmentAbilityDamage(value,currEncounterRunning->augment);
    }
    if (IsOwnedByPlayer(source))
    {
        if (HasAugment(currEncounterRunning,AUGMENT_GOOD_DAMAGE))
        {
            value += Neutral_GetAugmentAbilityDamage(value,currEncounterRunning->augment);
        }

        gameStats.damageDone += value;
    }
    AddDamageNumber((int)value,g->position.x+(rand()%(int)GetWidth(g)*1.1f),g->position.y+(rand()%(int)GetHeight(g)*1.1f),source);

    value = DamageShields(g,value);
    g->health -= value;
    if (value > 0)
        g->flashTimer = FLASH_TIMER;
    if (g->health <= 0)
    {
        KillObj(g,true);
        return true;
    }
    return false;
}
void Heal(GameObject* g, float value)
{
    if (!g) return;
    if (HasAugment(currEncounterRunning,AUGMENT_NEUTRAL_TOTALHEAL))
    {
        value += Neutral_GetAugmentAbilityHeal(value,currEncounterRunning->augment);
    }

    if (IsOwnedByPlayer(g))
    {
        if (HasAugment(currEncounterRunning,AUGMENT_GOOD_HEALS))
        {
            value += Good_GetAugmentAbilityHeal(value,currEncounterRunning->augment);
        }
        gameStats.healingDone += value;
    }

    g->health += value;
    if (g->health > g->maxHP)
        g->health = g->maxHP;
}
void AddSpeed(GameObject* g, float value)
{
    if (!g) return;
    g->speed += value;
}
void ModifyMaxHP(GameObject* g, float value)
{
    if (!g) return;
    g->maxHP += value;
}   
void Teleport(GameObject* g, float x, float y)
{
    if (!g) return;

    float dx = x - g->position.x;
    float dy = y - g->position.y;

    float beforeX = g->position.x;
    float beforeY = g->position.y;

    float cX; float cY;
    GetOffsetCenter(g,&cX,&cY);
    g->position.x = x - cX/2;
    g->position.y = y - cY/2;


    CheckCollisions(g,true, dx, false);
    CheckCollisions(g,false, dy, false);   


    float nrmX = dx;
    float nrmY = dy;
    Normalize(&nrmX,&nrmY);

    float xn = g->position.x;
    float yn = g->position.y;

    float w = GetWidth(g);
    float h = GetHeight(g);

    //step back until we find a spot where the object can stand
    if (nrmX != 0 && nrmY != 0)
        while (xn >= 0  && yn >= 0 && xn+w < _MAPSIZE && yn+h < _MAPSIZE)
        {
            int indexTop = GetIndex(_MAPSIZE/_GRAIN, floor(xn/ (float)_GRAIN), floor(yn / (float)_GRAIN));
            int indexRight = GetIndex(_MAPSIZE/_GRAIN, floor((xn+w) / (float)_GRAIN), floor((yn) / (float)_GRAIN));
            int indexBottom = GetIndex(_MAPSIZE/_GRAIN, floor((xn) / (float)_GRAIN), floor((yn+h) / (float)_GRAIN));
            int indexLeft = GetIndex(_MAPSIZE/_GRAIN, floor((xn) / (float)_GRAIN), floor((yn) / (float)_GRAIN));
            if (currMap->collision[indexLeft] && currMap->collision[indexRight] && currMap->collision[indexTop] && currMap->collision[indexBottom] )
            {
                g->position.x = xn;
                g->position.y = yn;

                break;
            }
            xn -= nrmX;
            yn -= nrmY;

        }


    g->targetPosition.x = g->position.x - cX/2.0f;
    g->targetPosition.y = g->position.y - cY/2.0f;

    //CheckCollisionsWorld(g,false, false);


    
}
void GetOffsetCenter(GameObject* g, float* x, float* y)
{
    if (g->spriteIndex > 0)
    {
        *x = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
        *y = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
        return;
    }
    *x = 0; 
    *y = 0;
}
void GetCentre(GameObject* g, float* x, float* y)
{
    if (!g)
    {
        *x = 0;
        *y = 0;
         return;
    }
    *x = g->position.x + al_get_bitmap_width(sprites[g->spriteIndex].sprite)/2.0f;
    *y = g->position.y + al_get_bitmap_height(sprites[g->spriteIndex].sprite)/2.0f;

}
void DoAI(GameObject* g)
{
    if (g->threatList.obj)
    {
        Threat* t = GetHighestThreat(&g->threatList);
        if (t)
        {
            ClearCommandQueue(g);
            AttackCommand(g,t->obj);
        }
            //g->targObj = t->obj;
    }
    //do one of these per frame, just to save some cycles
    if (_FRAMES % MAX_OBJS  == g-objects)
    {
        for (int i = 0; i < MAX_OBJS; i++)
        {
            if (objects[i].properties & OBJ_ACTIVE)
            {
                if ((g->properties & OBJ_OWNED_BY) != (objects[i].properties & OBJ_OWNED_BY))
                {
                    if (GetDist(g,&objects[i]) < g->aggroRadius)
                    {
                        //also bring in the others
                        for (int j = 0; j < MAX_OBJS; j++)
                        {
                            if ((g->properties & OBJ_OWNED_BY) == (objects[j].properties & OBJ_OWNED_BY))
                                AddThreat(&objects[i],&objects[j],0);
                        }
                        AddThreat(&objects[i],g, 0);
                    }

                }
            }

        }

    }
}
bool IsActive(GameObject* g)
{
    if (!g) return false;
    return (g->properties & OBJ_ACTIVE);
}
float GetDistCentre(GameObject* g1, GameObject* g2)
{
    float x1; float y1; float x2; float y2; 
    GetCentre(g1,&x1,&y1);
    GetCentre(g2,&x2,&y2);

    float movex = x2 - x1;
    float movey = y2 - y1;

    return sqrt(movex*movex+movey*movey);
}
float GetDist(GameObject* g1, GameObject* g2)
{
    Rect r1 = GetObjRect(g1);
    Rect r2 = GetObjRect(g2);

    Rect unioned = UnionRectR(r1,r2);
    unioned.w -= r1.w + r2.w;
    unioned.h -= r1.h + r2.h;

    return unioned.w+unioned.h;
}
GameObject* GetClicked(float x, float y)
{
    for (int i = 0; i < numObjects; i++)
    {
        Rect r = GetObjRect(&objects[i]);
        if (PointInRect(x,y,r))
        {
            return &objects[i];
        }
    }
    return NULL;

}
void UnsetAll()
{
    for (int i = 0; i < MAX_OBJS; i++)
    {
        SetSelected(&objects[i], false);
    }
}
int GetWidth(GameObject* g)
{
    return al_get_bitmap_width(sprites[g->spriteIndex].sprite);
}
int GetHeight(GameObject* g)
{
    return al_get_bitmap_height(sprites[g->spriteIndex].sprite);
}

float RectDist(GameObject* g1, GameObject* g2)
{
    Rect r1 = (Rect){g1->position.x,g1->position.y,GetWidth(g1),GetHeight(g1)};
    Rect r2 = (Rect){g2->position.x,g2->position.y,GetWidth(g2),GetHeight(g2)};
    Rect unioned = UnionRectR(r1,r2);
    float dist = (unioned.w+unioned.h) - (r1.w+r2.w+r1.h+r2.h);
    return dist;
}

bool IsInCombat(GameObject* g)
{
    if (g->threatList.obj || g->threatList.next)
    {

        return true;
    }
    return false;
}

bool ObjIsChannelling(GameObject* g)
{
    return (g->properties & OBJ_IS_CHANNELLING);
}
void SetObjChannelling(GameObject* g, Ability* a, float time,float x, float y, GameObject* target, float heading_x, float heading_y)
{
    g->properties |= OBJ_IS_CHANNELLING;
    g->channelledAbility = a;
    g->channellingTime = time;
    g->channellingTotal = time;
    g->channelled_x = x;
    g->channelled_y = y;
    g->channelled_target = target;
    g->target_heading_x = heading_x;
    g->target_heading_y = heading_y;
}
void UpdateChannellingdObj(GameObject* g, float dt)
{
    if (ObjIsChannelling(g))
    {
        g->channellingTime -= dt;
        if (g->channellingTime < 0)
        {
            g->properties &= ~OBJ_IS_CHANNELLING;
            CastAbility(g,g->channelledAbility,g->channelled_x,g->channelled_y,g->target_heading_x,g->target_heading_y,g->channelled_target);
            g->channelledAbility = NULL;
        }
    }
}
float ang = 0;
void DrawChannelHint(GameObject* g)
{
    if (IsActive(g))
    {
        if (ObjIsChannelling(g) && g->channelledAbility)
        {
            Ability* a  = g->channelledAbility;
            float x; float y; GetCentre(g,&x,&y);
            float x2; float y2;
            ALLEGRO_COLOR col = GetPlayerOwnedBy(g) == 0 ? FRIENDLY : ENEMY;
            if (g->channelled_target)
            {
                GetCentre(g->channelled_target,&x2,&y2);
            }
            else
            {
                x2 = g->channelled_x;
                x2 = g->channelled_y;
            }
            if (a->targetingHint == HINT_LINE)
            {
                al_draw_line(x,y,x2,y2,col,1);
            }
            if (a->targetingHint == HINT_CIRCLE)
            {
                al_draw_circle(x2,y2,a->hintRadius,col,1);
            }
            if (a->targetingHint == HINT_CONE)
            {
                float angle;
                angle = atan2(y2-y,x2-x);
                float length = dist(x,y,x2,y2);
                DrawCone(x,y,RadToDeg(angle),90,a->hintRadius,ENEMY);   
            }

        }
    }
   
}

void RemoveAllGameObjects()
{
    for (int i = 0; i < MAX_OBJS; i++)
    {
        KillObj(&objects[i],false);

    }
}


int GetNumPlayerControlledObjs(Player* p)
{
    int count = 0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g))
        {
            if (GetPlayerOwnedBy(g) == (p-players))
            {
                count++;
            }
        }
    }
    return count;
}
void SetDecoration(GameObject* g, bool b)
{
    if (g)
    {
        if (b)
        {
            g->properties |= OBJ_IS_DECORATION;
        }
        else
        {
            g->properties &= ~OBJ_IS_DECORATION;
        }
    }
   
}
bool ObjIsDecoration(GameObject* g)
{
    return (g->properties & OBJ_IS_DECORATION);
}
void SetInvincible(GameObject* g, bool b)
{
    if (g)
    {
        if (b)
        {
            g->properties |= OBJ_IS_INVINCIBLE;
        }
        else
        {
            g->properties &= ~OBJ_IS_INVINCIBLE;
        }
    }
}
bool ObjIsInvincible(GameObject* g)
{
    return (g->properties & OBJ_IS_INVINCIBLE);
}