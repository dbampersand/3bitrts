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

#include "pathfind.h"

 GameObject* objects = NULL;
 int numObjects = 0;
 int objectsAllocated = 0; 

 GameObject** freeObjs = NULL;
 int numFreeObjs = 0;


 GameObject** prefabs = NULL;
 int numPrefabs = 0;
 int numPrefabsAllocated = 0;

 ALLEGRO_BITMAP* lights[MAX_LIGHT_SIZE] = {0};


 CollisionEvent collisionEvents[MAX_OBJS] = {0};

float __sinTable[360*2] = {0};
float __cosTable[360*2] = {0};

float* sinTable = NULL; // = &__sinTable[360];
float* cosTable = NULL;// = &__cosTable[360];



GameObject* GetMousedOver(MouseState* mouseState)
{
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g = &objects[i];
        if (g->properties & OBJ_ACTIVE)
        {
            if (PointInRect(mouseState->worldX,mouseState->worldY,GetObjRect(g)))
            {
                return(g);
            }
        }
    }
    return NULL;
}
void UpdatePlayerObjectInteractions(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseState)
{
    if (IsBindDownThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_AMove))
    {
        players[0].amoveSelected = true;
    }
    if (players[0].abilityHeld) 
        players[0].amoveSelected = false;   
    if (IsBindDownThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_Tab))
    {
        players[0].indexSelectedUnit++;
        if (players[0].indexSelectedUnit >= MAXUNITSSELECTED || players[0].indexSelectedUnit >= players[0].numUnitsSelected)
        {
            players[0].indexSelectedUnit = 0;
        }
    }

}
void ProcessAttackMoveMouseCommand(MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyState)
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
                //if (!IsBindDown(keyState,currSettings.keymap.key_Shift))
                  //  ClearCommandQueue(g);

                AttackMoveCommand(g,mouseState->worldX-w/2,mouseState->worldY-h/2,IsBindDown(keyState,currSettings.keymap.key_Shift));
                
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
void ClearPathfindingQueue(GameObject* g)
{
    for (int i = 0; i < MAX_PATHFINDING_NODES_HELD; i++)
    {
        g->pathNodes[i].p.x = g->targetPosition.x;
        g->pathNodes[i].p.y = g->targetPosition.y;

    }
}
void SetTargetPosition(GameObject* g, float x, float y)
{
     g->targetPosition.x = x;
    g->targetPosition.y = y;
    int  w = GetWidth(g);
    int h = GetHeight(g);
    if (!IsOwnedByPlayer(g))
    {

        //PointI targetIndex = (PointI){((g->targetPosition.x) / (float)_GRAIN), ((g->targetPosition.y) / (float)_GRAIN)};
        //PointI currentIndex = (PointI){((g->position.worldX) / (float)_GRAIN), ((g->position.worldY) / (float)_GRAIN)};
        //bool success;


        //SetMapCollisionRect(g->position.worldX,g->position.worldY,w,h,false);
        //SetMapCollisionRect(g->targetPosition.x,g->targetPosition.y,w,h,false);
        g->pathfindNeedsRefresh = true;
        //AStar(currentIndex,targetIndex,&success,GetWidth(g),GetHeight(g),g);
        
    }

}
void UpdateObject(GameObject* g, float dt)
{
    currGameObjRunning = g;

    //things that should always run:
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        currGameObjRunning->abilities[i].cdTimer -= dt;
        if (currGameObjRunning->abilities[i].cdTimer < 0)
        {
            currGameObjRunning->abilities[i].cdTimer = currGameObjRunning->abilities[i].cooldown;
            currGameObjRunning->abilities[i].stacks++;
            if (currGameObjRunning->abilities[i].stacks > currGameObjRunning->abilities[i].maxStacks)
                 currGameObjRunning->abilities[i].stacks  = currGameObjRunning->abilities[i].maxStacks;
        }
    }
    UpdateMana(g, dt);
    UpdateHPRegen(g, dt);
    UpdateShield(g,dt);

    currGameObjRunning->invulnerableTime -= dt;
    if (currGameObjRunning->invulnerableTime < 0)
        currGameObjRunning->invulnerableTime = 0;


    g->stunTimer -= dt;
    if (g->stunTimer < 0)  
        g->stunTimer = 0;
    if (g->stunTimer > 0)
        return;

    g->flashTimer -= dt;
    if (g->flashTimer < 0)
        g->flashTimer = 0;

    if (g->deathTimerActivated)
    {
        g->deathTimer -= dt;
        if (g->deathTimer <= 0)
        {
            KillObj(g,true);
            return;
        }
    }

    ProcessEffects(currGameObjRunning,dt);
    ProcessShields(currGameObjRunning,dt);




    //things that should only run when not stunned:
    currGameObjRunning->offset.x = Towards(currGameObjRunning->offset.x,0,dt*2);
    currGameObjRunning->offset.y = Towards(currGameObjRunning->offset.y,0,dt*2);

    g->summonTime += dt;
    if (g->summonTime < g->summonMax)
    {
        float fxtimer = GetSummonPercent(g);

        float cx; float cy;
        GetCentre(g,&cx,&cy);
        float r = _MAX(GetWidth(g), GetHeight(g)) * fxtimer;
        ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
        if ((_FRAMES+(g-objects))%2==0)
        RandParticleAroundEdgeOfCircle(cx, cy, r, g->summonMax+1, 0.4, ALColorToCol(c));

        return;
    }
    if (ObjIsDecoration(g))
    {
        if (currGameObjRunning->properties & OBJ_ACTIVE)
        {
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,g->luafunc_update);
            lua_pushnumber(luaState,dt);
            lua_pcall(luaState,1,0,0);
        }
    }
    
    if (!IsActive(currGameObjRunning) || ObjIsDecoration(g))
        return;
    UpdateChannellingdObj(currGameObjRunning,dt);
    if (currGameObjRunning->properties & OBJ_ACTIVE && !IsOwnedByPlayer(currGameObjRunning))
    {
        DoAI(currGameObjRunning);
    }
    DoCommands(currGameObjRunning);
    currGameObjRunning->attackTimer -= dt;
    if (currGameObjRunning->attackTimer < 0)
        currGameObjRunning->attackTimer = 0;

    int w = al_get_bitmap_width(sprites[currGameObjRunning->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[currGameObjRunning->spriteIndex].sprite);

    Rect r = (Rect){currGameObjRunning->position.worldX,currGameObjRunning->position.worldY,w,h};

    bool shouldMove = true;
    bool shouldAttack = false;


    if (currGameObjRunning->properties & OBJ_ACTIVE)
    {
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,g->luafunc_update);
        lua_pushnumber(luaState,dt);
        if (lua_pcall(luaState,1,0,0) != LUA_OK)
        {
            printf("Error from %s: %s\n,",currGameObjRunning->name ? currGameObjRunning->name : "", lua_tostring(luaState,-1));
        }
    }

    if (currGameObjRunning->targObj && currGameObjRunning->queue[0].commandType == COMMAND_ATTACK) 
    {
        GameObject* tempAttack = currGameObjRunning->targObj;

        if (currGameObjRunning->properties & OBJ_ACTIVE)
        {
            int wTarg = al_get_bitmap_width(sprites[currGameObjRunning->targObj->spriteIndex].sprite);
            int hTarg = al_get_bitmap_height(sprites[currGameObjRunning->targObj->spriteIndex].sprite);

            SetTargetPosition(currGameObjRunning,currGameObjRunning->targObj->position.worldX + wTarg/2,currGameObjRunning->targObj->position.worldY + hTarg/2);

            Rect r2 = (Rect){currGameObjRunning->targObj->position.worldX,currGameObjRunning->targObj->position.worldY,wTarg,hTarg};
            #define DISTDELTA 0.001f
            Rect unioned = UnionRectR(r,r2);
            
            if (RectDist(currGameObjRunning,currGameObjRunning->targObj) < GetAttackRange(g))
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
                        if (IsActive(g2) && !ObjIsDecoration(g2))
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

        if (shouldAttack || (g->isBoss && tempAttack))
        {

            if (currGameObjRunning->attackTimer <= 0)
            {
                AttackTarget(currGameObjRunning,dt);
                if (currGameObjRunning->targObj)
                {
                     currGameObjRunning->attackTimer = currGameObjRunning->attackSpeed;
                    GameObject* g = currGameObjRunning; GameObject* g2 = g->targObj;
                    float angle = (atan2(g2->position.worldY-g->position.worldY,g2->position.worldX-g->position.worldX));
                    
                    currGameObjRunning->offset.x = cos(angle);
                    currGameObjRunning->offset.y = sin(angle);

                }
                
            }

        }
        currGameObjRunning->targObj = old;
    }
    if (shouldMove)
        Move(currGameObjRunning, dt);
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


    prefabs = calloc(1,sizeof(GameObject*));
    numPrefabs = 0;
    numPrefabsAllocated = 1;
    
}
void GetControlGroup(ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (!IsBindDown(keyState,currSettings.keymap.key_Ctrl))
    {
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[0]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(1);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[1]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(2);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[2]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(3);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[3]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(4);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[4]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(5);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[5]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(6);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[6]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(7);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[7]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(8);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[8]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(9);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[9]))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(0);
        }
    }  
}
void SetControlGroups(ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (IsBindDown(keyState,currSettings.keymap.key_Ctrl))
    {
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[0]))
        {
            SetCtrlGroup(1,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[1]))
        {
            SetCtrlGroup(2,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[2]))
        {
            SetCtrlGroup(3,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[3]))
        {
            SetCtrlGroup(4,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[4]))
        {
            SetCtrlGroup(5,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[5]))
        {
            SetCtrlGroup(6,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[6]))
        {
            SetCtrlGroup(7,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[7]))
        {
            SetCtrlGroup(8,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[8]))
        {
            SetCtrlGroup(9,players[0].selection,players[0].numUnitsSelected);
        }
        if (IsBindDown(keyState,currSettings.keymap.key_ctrlgroups[9]))
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

void CheckSelected(MouseState* mouseState, MouseState* mouseLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{

    if (!(mouseLastFrame->mouse.buttons & 1)  && (mouseState->mouse.buttons & 1) && !players[0].abilityHeld)
    {
        AddMouseRandomParticles(*mouseState, 3);
        players[0].selecting = true;
        players[0].selectionStart = (Point){mouseState->worldX,mouseState->worldY};
    }
    if (players[0].selecting)
    {
    }
    if (!(mouseState->mouse.buttons & 1))
    {
        if (players[0].selecting)
        {
            Point endSelection = (Point){mouseState->worldX,mouseState->worldY};
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
                Rect rObj = (Rect){obj->position.worldX,obj->position.worldY,al_get_bitmap_width(sp->sprite),al_get_bitmap_height(sp->sprite)};
                if (CheckIntersect(rObj,r))
                {
                    if (!IsBindDown(keyState,currSettings.keymap.key_Shift))
                    if (!hasSelected)
                    {
                        for (int j = 0; j < numObjects; j++)
                        {
                            SetSelected(&objects[j],false);
                            ClearSelection();
                            players[0].numUnitsSelected = 0;
                        }
                        hasSelected = true;
                    }
                    if (IsBindDown(keyState,currSettings.keymap.key_Shift))
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
    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_HoldPosition))
    {
        for (int i = 0; i < players[0].numUnitsSelected; i++)
        {
            HoldCommand(players[0].selection[i],IsBindDown(keyState,currSettings.keymap.key_Shift));
        }

    }
    if (!(mouseState->mouse.buttons & 2) && (mouseLastFrame->mouse.buttons & 2))
    {
        if (!IsInsideUI(mouseState->screenX,mouseState->screenY))
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
                                if (!IsBindDown(keyState,currSettings.keymap.key_Shift))
                                    ClearCommandQueue(g);
                            bool found = false;
                            for (int i = 0; i < MAX_OBJS; i++)
                            {
                                GameObject* g2 = &objects[i];
                                if (g == g2) continue;

                                Rect r = GetObjRect(g2);
                                if (PointInRect(mouseState->worldX,mouseState->worldY,r))
                                {
                                    found = true;
                                    if (IsOwnedByPlayer(g2))
                                        FollowCommand(g,g2,IsBindDown(keyState,currSettings.keymap.key_Shift));
                                    else
                                        AttackCommand(g,g2,IsBindDown(keyState,currSettings.keymap.key_Shift));
                                    break;
                                }
                            }
                            if (!found)
                                MoveCommand(g,mouseState->worldX-w/2,mouseState->worldY-h/2,IsBindDown(keyState,currSettings.keymap.key_Shift));
                        
                        }
                        Sprite* s = &sprites[g->spriteIndex];
                        Rect r = (Rect){g->position.worldX,g->position.worldY,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)}; 
                        if (PointInRect(mouseState->worldX,mouseState->worldY,r))
                        {
                            for (int i = 0; i < players[0].numUnitsSelected; i++)
                            {
                                //if (!IsBindDown(keyState,currSettings.keymap.key_Shift))
                                  //  ClearCommandQueue(players[0].selection[i]);
                                    if (!ObjIsInvincible(g) && !IsOwnedByPlayer(g))
                                        AttackCommand(players[0].selection[i],g,IsBindDown(keyState,currSettings.keymap.key_Shift));
                                    else
                                        MoveCommand(players[0].selection[i],mouseState->worldX-GetWidth(g)/2,mouseState->worldY-GetHeight(g)/2,IsBindDown(keyState,currSettings.keymap.key_Shift));
                            }
                            break;
                        }

                    }
        }
    
        
    }
}
int GetNumObjectsInRect(Rect* r, bool onlyPlayerChoosable)
{
    int j = 0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (!IsActive(&objects[i]) || ObjIsDecoration(&objects[i]))
            continue;
            
        if (onlyPlayerChoosable && !objects[i].playerChoosable)
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


   // memset(found->abilities,0,sizeof(Ability)*4);
    memset(currGameObjRunning,0,sizeof(GameObject));

    currGameObjRunning->objectIsStunnable = true;

    currGameObjRunning->lua_buffer = prefab->lua_buffer; 
    currGameObjRunning->path = prefab->path;
    currGameObjRunning->name = prefab->name;
    currGameObjRunning->threatMultiplier = 1;

    currGameObjRunning->bounty = DEFAULT_BOUNTY;


    SetRange(currGameObjRunning,1);
    currGameObjRunning->objType = TYPE_ALL;

    currGameObjRunning->health = 100;
    currGameObjRunning->maxHP = 100;

    currGameObjRunning->attackSpeed = 1;

    currGameObjRunning->maxMana = 100;
    currGameObjRunning->mana = currGameObjRunning->maxMana;

    currGameObjRunning->manaRegen = 1;

    currGameObjRunning->aggroRadius = 30;

    SetMoveSpeed(currGameObjRunning,50);
    SetHPRegen(currGameObjRunning,1);

    currGameObjRunning->lightR = 1;
    currGameObjRunning->lightG = 0.9f;
    currGameObjRunning->lightB = 0.9f;
    currGameObjRunning->lightIntensity = 0.25f;
    SetLightSize(currGameObjRunning,30);
    
    //currGameObjRunning->speed = 50;

    loadLuaGameObj(luaState, found->path, found); 
    found->properties |= OBJ_ACTIVE;

    //currGameObjRunning->position.worldX = _SCREEN_SIZE/2.0f;
    //currGameObjRunning->position.worldY = _SCREEN_SIZE/2.0f;
    UpdateObjPosition(currGameObjRunning,_SCREEN_SIZE/2.0f,_SCREEN_SIZE/2.0f);

    currGameObjRunning->shouldProcessAI = true;


    Teleport(currGameObjRunning,x,y);



    found->attackSpeed = 1;
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
        
        if (CheckFuncExists("OnMapChange",g->lua_buffer))
        {
            lua_getglobal(l, "OnMapChange");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_onmapchange = funcIndex;
        }
        else
            g->luafunc_onmapchange = -1;


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
void ScatterEffect(GameObject* g)
{

    ALLEGRO_BITMAP* sprite = sprites[g->spriteIndex].sprite;
    //if (IsSelected(g) && sprites[g->spriteIndex].inverseSprite)
      //  sprite = sprites[g->spriteIndex].inverseSprite;
    //if (!sprite)
      //  return;
    al_lock_bitmap(sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    int w = al_get_bitmap_width(sprite);
    int h = al_get_bitmap_height(sprite);

    Color c = GameObjToColor(g);
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            ALLEGRO_COLOR pixel = al_get_pixel(sprite, x, y);
            if (pixel.a)
            {
                AddParticleWithRandomProperties(g->position.worldX+x,g->position.worldY+y,c, 0.15, 2, 0, 0.025);
            }
        }
    }
    al_unlock_bitmap(sprite);


}
void KillObj(GameObject* g, bool trigger)
{
    if (!g) return;
    ScatterEffect(g);

    RemoveObjFromAllThreatlists(g);
    SetMapCollisionRect(g->position.worldX,g->position.worldY,GetWidth(g),GetHeight(g),false);
    GameObject* before = currGameObjRunning;
    currGameObjRunning = g;
    CallLuaFunc(g->luafunc_kill);
    currGameObjRunning = before;

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
    g->channelingSpriteIndex = 0;

    DeleteThreatList(g);
    if (g->shields)
    {
        free(g->shields);
        g->numShields = 0;
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
    if (!IsOwnedByPlayer(g) && gameState == GAMESTATE_INGAME)
        AddGold(g->bounty);
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
    closedir(d);    

    }
    free(file);


}
GameObject* LoadPrefab(const char* path)
{
    for (int i = 0; i < numPrefabs; i++)
    {
        if (strcasecmp(prefabs[i]->path,path)==0)
        {
            return prefabs[i];
        }
    }
    GameObject* g = calloc(1,sizeof(GameObject));
    loadLuaGameObj(luaState,path,g);

    if (numPrefabs  >= numPrefabsAllocated)
    {
        numPrefabsAllocated += BUFFER_PREALLOC_AMT;
        prefabs = realloc(prefabs,(numPrefabsAllocated)*sizeof(GameObject*));
    }
    prefabs[numPrefabs] = g;
    numPrefabs++;

    return prefabs[numPrefabs-1];
}
void MakeInvulnerable(GameObject* g, float time)
{
    g->invulnerableTime += time;
}
void UpdateObjPosition(GameObject* g, float x, float y)
{
    g->position.worldX = x;
    g->position.worldY = y;

    //g->position.screenX = ToScreenSpace_X(x);
   // g->position.screenY = ToScreenSpace_Y(y);

}
void UpdateObjPosition_X(GameObject* g, float x)
{
    g->position.worldX = x;
    //g->position.screenX = ToScreenSpace_X(x);
}
void UpdateObjPosition_Y(GameObject* g, float y)
{
    g->position.worldY = y;
    //g->position.screenY = ToScreenSpace_X(y);
}
void UpdateScreenPositions(GameObject* g)
{
    g->position.screenX = ToScreenSpace_X(g->position.worldX);
    g->position.screenY = ToScreenSpace_Y(g->position.worldY);

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
        closedir(d);

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
    if (ObjIsDecoration(g))
        return TYPE_DECORATION;
    return (g->properties & OBJ_OWNED_BY) > 0 ? TYPE_ENEMY : TYPE_FRIENDLY;
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
        return;
    if (dV == 0) return;
    Sprite* s = &sprites[g->spriteIndex];
    if (ObjIsDecoration(g)) 
        dV = -dV;
    int numEvents = 0;

    for (int i = 0; i < numObjects; i++)
    {
        Rect rG = (Rect){g->position.worldX,g->position.worldY,GetWidth(g),GetHeight(g)};

        GameObject* g2 = &objects[i];
        if ((!IsActive(g2) || g2 == g)) 
            continue;
        Sprite* s2 = &sprites[g2->spriteIndex];
        Rect r2 = (Rect){g2->position.worldX,g2->position.worldY,GetWidth(g2),GetHeight(g2)};
        //Decoration objects are always static
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
                        if (!ObjIsDecoration(g2))
                        {
                            //g2->position.worldX = g->position.worldX + GetWidth(g);
                            UpdateObjPosition_X(g2,g->position.worldX + GetWidth(g));
                            if (g2->position.worldX < 0)
                            {
                                //g2->position.worldX=0;
                                UpdateObjPosition_X(g2,0);
                            }
                            if (g2->position.worldX + GetWidth(g2) > GetMapWidth())
                            {   
                                collisionEvents[numEvents].obj = g2;
                                collisionEvents[numEvents].x = true;
                                collisionEvents[numEvents].direction = -1;


                                numEvents++;
                                //g2->position.worldX = GetMapWidth() - GetWidth(g2);
                                UpdateObjPosition_X(g2,GetMapWidth() - GetWidth(g2));
                                continue;
                            }
                        }
                        collisionEvents[numEvents].obj = g2;
                        collisionEvents[numEvents].x = true;
                        collisionEvents[numEvents].direction = 1;
                        numEvents++;
                    }
                    else
                    {

                        if (!ObjIsDecoration(g))
                        {
                            collisionEvents[numEvents].obj = g2;
                            collisionEvents[numEvents].x = true;
                            collisionEvents[numEvents].direction = -1;
                            numEvents++;

                            //g->position.worldX = (g2->position.worldX - al_get_bitmap_width(s->sprite));
                            UpdateObjPosition_X(g, (g2->position.worldX - al_get_bitmap_width(s->sprite)));

                        }

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
                        if (!ObjIsDecoration(g2))
                        {
                            //g2->position.worldX = g->position.worldX - GetWidth(g2);
                            UpdateObjPosition_X(g2,g->position.worldX - GetWidth(g2));
                            if (g2->position.worldX < 0)
                            {
                                collisionEvents[numEvents].obj = g2;
                                collisionEvents[numEvents].x = true;
                                collisionEvents[numEvents].direction = 1;
                                numEvents++;

                               // g2->position.worldX=0;
                                UpdateObjPosition_X(g2,0);

                               // CheckCollisions(g2,true,1,true);
                               //  CheckCollisionsWorld(g2,true,1);
                                continue;

                            }
                            if (g2->position.worldX + GetWidth(g2) > GetMapWidth())
                            {
                                UpdateObjPosition_X(g2,GetMapWidth() - GetWidth(g2));
                            
                            }
                        }
                        collisionEvents[numEvents].obj = g2;
                        collisionEvents[numEvents].x = true;
                        collisionEvents[numEvents].direction = -1;
                        numEvents++;
                        //CheckCollisions(g2,true,-1,true);
                        //CheckCollisionsWorld(g2,true,-1);
                    }
                    else
                    {
                        if (!ObjIsDecoration(g))
                        {
                            collisionEvents[numEvents].obj = g2;
                            collisionEvents[numEvents].x = true;
                            collisionEvents[numEvents].direction = -1;
                            numEvents++;

                            //g->position.worldX = g2->position.worldX+al_get_bitmap_width(s2->sprite); 
                            UpdateObjPosition_X(g,g2->position.worldX+al_get_bitmap_width(s2->sprite));
                            //CheckCollisions(g2,true,-1,true);
                            //CheckCollisionsWorld(g2,true,-1);
                        }
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
                            if (!ObjIsDecoration(g2))
                            {   

                                //g2->position.worldY = g->position.worldY + GetHeight(g);
                                UpdateObjPosition_Y(g2,g->position.worldY + GetHeight(g));

                                if (g2->position.worldY <0)
                                {
                                    //g2->position.worldY=0;
                                    UpdateObjPosition_Y(g2,0);

                                }
                                if (ObjectIsInUI(g2))
                                {
                                    collisionEvents[numEvents].obj = g2;
                                    collisionEvents[numEvents].x = false;
                                    collisionEvents[numEvents].direction = -1;
                                    numEvents++;

                                    g2->position.worldY = GetUIStartHeight() - GetHeight(g2);
                                    UpdateObjPosition_Y(g2,GetUIStartHeight() - GetHeight(g2));
                                    //CheckCollisions(g2,false,-1,true);
                                    //CheckCollisionsWorld(g2,false,-1);
                                    continue;
                                }
                        }
                        collisionEvents[numEvents].obj = g2;
                        collisionEvents[numEvents].x = false;
                        collisionEvents[numEvents].direction = 1;
                        numEvents++;
                        //CheckCollisions(g2,false,1,true);
                        //CheckCollisionsWorld(g2,false,1);

                    }
                    else
                    {
                        if (!ObjIsDecoration(g))
                        {
                            collisionEvents[numEvents].obj = g2;
                            collisionEvents[numEvents].x = false;
                            collisionEvents[numEvents].direction = 1;
                            numEvents++;
                            
                            //g->position.worldY = g2->position.worldY - al_get_bitmap_height(s->sprite);
                            UpdateObjPosition_Y(g,g2->position.worldY - al_get_bitmap_height(s->sprite));
                            //CheckCollisions(g2,false,1,true);
                            //CheckCollisionsWorld(g2,false,1);
                        }

                    }
                }
                else if (dV < 0)
                {
                    //moving up
                    if (objectCanPush)
                    {
                        if (!ObjIsDecoration(g2))
                        {
                            //g2->position.worldY = g->position.worldY - GetHeight(g2);
                            UpdateObjPosition_Y(g2,g->position.worldY - GetHeight(g2));
                            if (g2->position.worldY < 0)
                            {
                                //g2->position.worldY=0;
                                UpdateObjPosition_Y(g2,0);
                                collisionEvents[numEvents].obj = g2;
                                collisionEvents[numEvents].x = false;
                                collisionEvents[numEvents].direction = 1;
                                numEvents++;
                                //CheckCollisions(g2,false,1,true);
                              //  CheckCollisionsWorld(g2,false,1);
                                continue;
                            }
                            if (ObjectIsInUI(g2))
                            {
                               // g2->position.worldY = GetUIStartHeight() - GetHeight(g2);//UI_START_Y - GetHeight(g2);
                        }
                        }
                        collisionEvents[numEvents].obj = g2;
                        collisionEvents[numEvents].x = false;
                        collisionEvents[numEvents].direction = -1;
                        numEvents++;
                        //CheckCollisions(g2,false,-1,true);
                        //CheckCollisionsWorld(g2,false,-1);
                    }
                    else
                    {
                        if (!ObjIsDecoration(g))
                        {
                            collisionEvents[numEvents].obj = g2;
                            collisionEvents[numEvents].x = false;
                            collisionEvents[numEvents].direction = -1;
                            numEvents++;
                            //g->position.worldY = g2->position.worldY + al_get_bitmap_height(s2->sprite);
                            UpdateObjPosition_Y(g,g2->position.worldY + al_get_bitmap_height(s2->sprite));
                           // CheckCollisions(g2,false,-1,true);
                            //CheckCollisionsWorld(g2,false,-1);
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < numEvents; i++)
    {
        CollisionEvent* c = &collisionEvents[i];
        CheckCollisions(c->obj,c->x,c->direction,true);
        CheckCollisionsWorld(c->obj,c->x,c->direction);

    }
}
GameObject* GetCollidedWith(GameObject* g)
{
    Sprite* s = &sprites[g->spriteIndex];
    Rect rG = (Rect){g->position.worldX,g->position.worldY,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)};
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g2 = &objects[i];
        if (g2 == g || !IsActive(g2)) 
            continue;
        Sprite* s2 = &sprites[g2->spriteIndex];
        Rect r2 = (Rect){g2->position.worldX,g2->position.worldY,al_get_bitmap_width(s2->sprite),al_get_bitmap_height(s2->sprite)};

        if (!CheckIntersect(rG,r2))
        {
            continue;
        }
        return g2;
    }
    return NULL;

}
int GetUIStartHeight()
{
    return (_MAX(GetMapHeight(),_SCREEN_SIZE) - (_SCREEN_SIZE - UI_START_Y)) + 1;

}
bool ObjectIsInUI(GameObject* g)
{
    if (g->position.worldY + GetHeight(g) > GetUIStartHeight())
    {
        return true;
    }
    return false;

}
void CheckCollisionsWorld(GameObject* g, bool x, float dV)
{
    if (ObjIsDecoration(g))
        return;
        int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
    
    if (ObjectIsInUI(g))
    {
       // g->position.worldY = GetUIStartHeight() - GetHeight(g);// UI_START_Y - GetHeight(g);
        UpdateObjPosition_Y(g,GetUIStartHeight() - GetHeight(g));
    }

    float posX = g->position.worldX;
    float posY = g->position.worldY;

    if (posX < 0 || posX+w > GetMapWidth())
    {
        if (posX < 0) 
        {
            //g->position.worldX = 0;
            UpdateObjPosition_X(g,0);
            CheckCollisions(g,true,1,true);

        }
        if (posX+w > GetMapWidth()) 
        {
           // g->position.worldX = GetMapWidth()-w;
            UpdateObjPosition_X(g,GetMapWidth()-w);
            CheckCollisions(g,true,-1,true);

        }
        return;

    }
    if (posY < 0 || posY+h > GetMapHeight())
    {
        if (posY < 0)
        {
            //g->position.worldY = 0;
            UpdateObjPosition_Y(g,0);
            CheckCollisions(g,false,1,true);

        } 
        if (posY+h >  GetMapHeight())
        {
            //g->position.worldY =  GetMapHeight()-h;
            UpdateObjPosition_Y(g,GetMapHeight()-h);
            CheckCollisions(g,false,-1,true);
        } 

        return;
    }

    if (dV == 0) return;
    
    int indexTop = GetIndex( GetMapHeight()/_GRAIN, floor(posX/ (float)_GRAIN), floor(posY / (float)_GRAIN));
    int indexRight = GetIndex(GetMapHeight()/_GRAIN, floor((posX+w) / (float)_GRAIN), floor((posY) / (float)_GRAIN));
    int indexBottom = GetIndex(GetMapHeight()/_GRAIN, floor((posX) / (float)_GRAIN), floor((posY+h) / (float)_GRAIN));
    int indexLeft = GetIndex(GetMapHeight()/_GRAIN, floor((posX) / (float)_GRAIN), floor((posY) / (float)_GRAIN));

    if (x)
    {
        if (dV < 0)
        {
            if (currMap->collision[indexLeft] == false)
            {
               // printf("gg");
                //g->position.worldX = IndexToPoint(GetMapHeight()/_GRAIN,indexLeft).x*_GRAIN + _GRAIN;//indexLeft / _GRAIN;//((indexLeft/(GetMapHeight()/_GRAIN))*_GRAIN)+_GRAIN;
                UpdateObjPosition_X(g,IndexToPoint(GetMapHeight()/_GRAIN,indexLeft).x*_GRAIN + _GRAIN);

                CheckCollisions(g,true,1,true);

            }
        }
        if (dV > 0)
        {
            if (currMap->collision[indexRight] == false)
            {
                //g->position.worldX = (indexRight/(GetMapHeight()/_GRAIN))*_GRAIN - w;
                //g->position.worldX = IndexToPoint(GetMapHeight()/_GRAIN,indexRight).x*_GRAIN - w;//indexLeft / _GRAIN;//((indexLeft/(GetMapHeight()/_GRAIN))*_GRAIN)+_GRAIN;
                UpdateObjPosition_X(g,IndexToPoint(GetMapHeight()/_GRAIN,indexRight).x*_GRAIN - w);
                
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
                //int yCoord = (indexTop%(GetMapHeight()/_GRAIN)+1)*_GRAIN;
                //g->position.worldY = IndexToPoint(GetMapHeight()/_GRAIN,indexTop).y*_GRAIN + _GRAIN;//indexLeft / _GRAIN;//((indexLeft/(GetMapHeight()/_GRAIN))*_GRAIN)+_GRAIN;
                UpdateObjPosition_Y(g,IndexToPoint(GetMapHeight()/_GRAIN,indexTop).y*_GRAIN + _GRAIN);
               // g->position.worldY = yCoord;
                CheckCollisions(g,false,-dV,true);
            }

        }
        if (dV > 0)
        {
            if (currMap->collision[indexBottom] == false)
            {
                //int yCoord = (indexBottom%(GetMapHeight()/_GRAIN))*_GRAIN - h;
                //g->position.worldY = yCoord;
                //g->position.worldY = IndexToPoint(GetMapHeight()/_GRAIN,indexBottom).y*_GRAIN - h;//indexLeft / _GRAIN;//((indexLeft/(GetMapHeight()/_GRAIN))*_GRAIN)+_GRAIN;
                UpdateObjPosition_Y(g,IndexToPoint(GetMapHeight()/_GRAIN,indexBottom).y*_GRAIN - h);
                CheckCollisions(g,false,-dV,true);
            }
        }
    }
}
void GetTilesAroundPoint(Point p, float w, float h, int indexes[2])
{
    float x = p.x;
    float y = p.y;

    int indexLeft = GetIndex(GetMapHeight()/_GRAIN, floor((x) / (float)_GRAIN), floor((y) / (float)_GRAIN));
    int indexRight = GetIndex(GetMapHeight()/_GRAIN, floor((x+w) / (float)_GRAIN), floor((y+h) / (float)_GRAIN));

    indexes[0] = indexLeft;
    indexes[1] = indexRight;

}
float drawX; float drawY;
void GameObjDebugDraw()
{
    al_draw_circle(ToScreenSpace_X(drawX),ToScreenSpace_Y(drawY),5,al_map_rgba(255,0,128,255),1);
}
void DoCurrentPathingNode(GameObject* g)
{
    if (IsNear(g->position.worldX,g->targetPosition.x, 1.0f) && IsNear(g->position.worldY,g->targetPosition.y,1.0f))
    {
        ClearPathfindingQueue(g);
        return;
    }
    if (IsNear(g->position.worldX,g->pathNodes[g->currentPathingNode].p.x, 0.5) && IsNear(g->position.worldY,g->pathNodes[g->currentPathingNode].p.y, 0.5f) || g->pathfindNeedsRefresh)
    {
        bool success;
        PointI targetIndex = (PointI){((g->targetPosition.x) / (float)_GRAIN), ((g->targetPosition.y) / (float)_GRAIN)};
        PointI currentIndex = (PointI){((g->position.worldX) / (float)_GRAIN), ((g->position.worldY) / (float)_GRAIN)};


        if (g->pathfindNeedsRefresh)
            AStar(currentIndex,targetIndex,&success,GetWidth(g),GetHeight(g),g);
        else   
        {
            g->currentPathingNode++;
            if (g->currentPathingNode >= MAX_PATHFINDING_NODES_HELD || ((_FRAMES+(g-objects))%MAX_OBJS) == 0)
            {
                AStar(currentIndex,targetIndex,&success,GetWidth(g),GetHeight(g),g);
            }
        }
    }
}
void Move(GameObject* g, float delta)
{
    if (!g) return;
    if (!IsActive(g))
        return;
    if (ObjIsDecoration(g))
        return;
    if (g->speed == 0)
        return;
    #define DIST_DELTA 1
    PointSpace before = g->position;
    int w = GetWidth(g);//al_get_bitmap_width(sprites[g->spriteIndex].sprite);
    int h = GetHeight(g);//al_get_bitmap_height(sprites[g->spriteIndex].sprite);


    float xtarg;
    float ytarg;

    bool success = false;

    PointI path;
    path.x = g->targetPosition.x;
    path.y = g->targetPosition.y;
    
    SetMapCollisionRect(g->position.worldX,g->position.worldY,w,h,false);

   // SetMapCollisionRect(path.x,path.y,w,h,false);



    //if (strcmp(g->name,"viper") == 0)
    {
        //path = AStar(currentIndex,targetIndex,&success,GetWidth(g),GetHeight(g));
        
        //if ((_FRAMES+(g-objects))%60==0)
          //  AStar(currentIndex,targetIndex,&success,GetWidth(g),GetHeight(g),g);
        if (!IsOwnedByPlayer(g) && !ObjectCanPush(g)) {
            DoCurrentPathingNode(g);
            if (g->currentPathingNode < MAX_PATHFINDING_NODES_HELD)
            {
                path.x = (g->pathNodes[g->currentPathingNode].p.x + g->pathNodes[g->currentPathingNode+1].p.x) / 2;
                path.y = (g->pathNodes[g->currentPathingNode].p.y + g->pathNodes[g->currentPathingNode+1].p.y) / 2;
            }
            else
                path = g->pathNodes[g->currentPathingNode].p;

            drawX = path.x;
            drawY = path.y;

            xtarg = path.x;
            ytarg = path.y;
        }
    }

    xtarg = path.x;
    ytarg = path.y;


    if (!g->targObj)
    {
       // xtarg = g->targetPosition.x;
        //ytarg = g->targetPosition.y;
        xtarg = path.x;
        ytarg = path.y;

    }
    else
    {
        //GetCentre(g->targObj,&xtarg,&ytarg);
    }

        double moveX = xtarg - g->position.worldX;
        double moveY = ytarg - g->position.worldY;
        

        double dist = sqrt(moveX*moveX+moveY*moveY);

        float speed = _MAX(0,g->speed);

        double dX = (moveX / dist * speed) * delta;
        double dY = (moveY / dist * speed) * delta;

        if (dist <= DIST_DELTA)
        {
            //g->position.worldX = xtarg;
            UpdateObjPosition_X(g,xtarg);
            CheckCollisions(g,true, dX,ObjectCanPush(g));
            CheckCollisionsWorld(g,true, dX);

            g->position.worldY = ytarg;
            CheckCollisions(g,false, dY,ObjectCanPush(g));
            CheckCollisionsWorld(g,false, dY);
            SetMapCollisionRect(g->position.worldX,g->position.worldY,w,h,true);

            return;
        }
        double mDist = sqrt(dX * dX + dY * dY);
        if (dist <= mDist)
        {
            dX = xtarg - g->position.worldX;
            dY = ytarg - g->position.worldY;
            
            g->position.worldY=ytarg;
            CheckCollisions(g,false, dY,ObjectCanPush(g));
            CheckCollisionsWorld(g,false, dY);

            g->position.worldX=xtarg;
            CheckCollisions(g,true, dX,ObjectCanPush(g));
            CheckCollisionsWorld(g,true, dX);

            SetTargetPosition(g,g->position.worldX,g->position.worldY);


            SetMapCollisionRect(g->position.worldX,g->position.worldY,w,h,true);

            
            return;
        }
        
        if (1)
        {
            //if we're moving at > 1 pixels per second, we need to move it in subdivisions
            //a bit of a hack but will work until collision is refactored
            int numMoves = ceil(fabs(dY));
            dY /= numMoves;
            for (int i = 0; i < numMoves; i++)
            {
                g->position.worldY += dY;
                CheckCollisions(g,false, dY,ObjectCanPush(g));
                CheckCollisionsWorld(g,false, dY);
            }
            numMoves = ceil(fabs(dX));
            dX /= numMoves;
            for (int i = 0; i < numMoves; i++)
            {
                g->position.worldX += dX;

                CheckCollisions(g,true, dX,ObjectCanPush(g));
                CheckCollisionsWorld(g,true, dX);
                SetMapCollisionRect(g->position.worldX,g->position.worldY,w,h,true);
            }
        }
    UpdateObjPosition(g,g->position.worldX,g->position.worldY);

}
bool ObjectCanPush(GameObject* g)
{
    return (g->properties & OBJ_CAN_PUSH);
}
float GetSummonPercent(GameObject* g)
{
    float summPercent = g->summonTime / g->summonMax;
    summPercent = clamp(summPercent,0,1);
    if (g->summonTime < g->summonMax)
        summPercent = EaseInOutCubic(summPercent);  
    return summPercent;

}
float GetTotalDotted(GameObject* g)
{
    float totalDamage = 0;
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];
        if (EffectIsEnabled(e) && e->trigger == TRIGGER_TIMER)
        {
            if (e->effectType == EFFECT_HURT)
            {
                float v = e->value * e->numTriggers;
                totalDamage += v;
            }
        }
    }
    return totalDamage;
}
void DrawHealthBar(GameObject* g, ALLEGRO_COLOR col)
{
    //potential for division by 0 here
    if (g->maxHP == 0)
        return;

    float summPercent = GetSummonPercent(g);
    Sprite* s = &sprites[g->spriteIndex];
    int padding = HEALTHBAR_HEIGHT * 2;

    Rect r;
    r.h = HEALTHBAR_HEIGHT * summPercent;
    r.w = al_get_bitmap_width(s->sprite);
    r.x = g->position.screenX + g->offset.x;
    r.y = g->position.screenY - padding + g->offset.y + (HEALTHBAR_HEIGHT * (1-summPercent));
    //ToScreenSpace(&r.x,&r.y);

    al_draw_filled_rectangle((int)r.x-1,(int)r.y-1,(int)r.x+r.w+1,(int)r.y+r.h+1,GROUND);

    al_draw_rectangle((int)r.x-1,(int)r.y-1,(int)r.x+r.w+1,(int)r.y+r.h+1,BG,1);
    al_draw_rectangle((int)r.x,(int)r.y,(int)r.x+r.w,(int)r.y+r.h,col,1);

    float percent = (g->health) / (float)g->maxHP;

    float shield = GetTotalShield(g);
    float percentShield = shield/g->maxHP;
    if (percentShield > 1) percentShield = 1;
    int numPixels = percent * r.w;
    int numPixelsShield = percentShield*r.w;
    
    al_draw_filled_rectangle((int)r.x,(int)r.y,(int)(r.x+numPixels),(int)(r.y+r.h),col);
    al_draw_filled_rectangle((int)r.x,(int)r.y-7,(int)(r.x+numPixelsShield),(int)((r.y-7)+r.h-1),col);



    //Draw amount of HP which has a DoT effect on it if they're all summed
    float amtPoisoned = GetTotalDotted(g);

    float percentPoisoned = amtPoisoned/g->health;
    percentPoisoned = clamp(percentPoisoned,0,percent);

    if (percentPoisoned > 0)
        al_draw_filled_rectangle((int)r.x,(int)r.y+1,(int)r.x+(r.w*percentPoisoned),(int)r.y+r.h,POISON);

        
    //Draw indicator for bad effects
    int x = g->position.screenX;
    int y = g->position.screenY;
    int numEffects = 0;
    
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        Effect* e = &g->effects[i];
        if (EffectIsEnabled(e) && e->from)
        {
            //if (!IsOwnedByPlayer(e->from))
            {
                al_put_pixel(x,y,GetColor(EffectColors[e->effectType],0));
                numEffects++;
                if (numEffects == GetWidth(g)-1)
                {
                    //too many effects to display
                    if (y == g->position.screenY + GetHeight(g))
                    {
                        break;
                    }

                    x = g->position.screenX;
                    y = g->position.screenY + GetHeight(g);
                }
                x++;
            }
        }        
    }

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
    float percent = GetSummonPercent(g); 
    int w = GetWidth(g);
    int h = ceil(GetHeight(g) * percent); 
    int x = g->position.screenX + g->offset.x;
    int y = g->position.screenY + g->offset.y + ceil(GetHeight(g)*(1-percent));

    int lineW = (ceil(w/16.0f));
    int lineH = (ceil(h/16.0f));

    lineW = lineW == 0 ? 2 : lineW;
    lineH = lineH == 0 ? 2 : lineH;
    Rect r = (Rect){x+1,y+1,w+lineW-1,h+lineH-1};
    DrawRoundedRect(r,BG,true);
}
void DrawObjShadows()
{
    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (IsActive(&objects[i]) && !ObjIsDecoration(&objects[i]) && objects[i].summonTime > objects[i].summonMax)
            DrawObjShadow(&objects[i]);
    }
}   
void DrawMapHighlights()
{
    ALLEGRO_BITMAP* screen = al_get_target_bitmap();
    al_lock_bitmap(sprites[currMap->spriteIndex].sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    al_lock_bitmap(screen,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);

    clock_t begin = clock();

    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g) && IsOwnedByPlayer(g))
        {
            al_set_target_bitmap(screen);
            //DrawMapHighlight(g,30,screen);
            DrawMapHighlight(g,g->lightSize,screen);
        }
    }
    
    clock_t end = clock();
    double time = (double)(end - begin) / CLOCKS_PER_SEC;

    al_unlock_bitmap(sprites[currMap->spriteIndex].sprite);
    al_unlock_bitmap(screen);

    int beforeOp; int beforeSrc; int beforeDst;
    al_get_blender(&beforeOp, &beforeSrc, &beforeDst);

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);

    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g) && IsOwnedByPlayer(g))
        {
            g->lightR = 1;
            g->lightG = 0.9;
            g->lightB = 0.9;
            g->lightIntensity = 0.5f;

            float re = g->lightR; float gr = g->lightG; float bl = g->lightB;

            re *= g->lightIntensity;
            gr *= g->lightIntensity;
            bl *= g->lightIntensity;


            ALLEGRO_COLOR col = al_map_rgba_f(re,gr,bl,g->lightIntensity);
            al_draw_tinted_bitmap(lights[g->lightSize],col,g->position.screenX-g->lightSize+GetWidth(g)/2,g->position.screenY-g->lightSize+GetHeight(g)/2,0);
        }
    }

    al_set_blender(beforeOp, beforeSrc, beforeDst);

    DrawMap(true);

   // al_set_target_bitmap(screen);

}
void DrawSummonEffect(GameObject* g)
{
    ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
    float fxtimer = GetSummonPercent(g);
    if (g->summonTime > g->summonMax)
    {
        fxtimer = fxtimer - EaseInOutCubic((g->summonTime-g->summonMax) / (g->summonMax*1.15));
    }
    float x; float y;
    GetCentre_Screen(g,&x,&y);
    
    //ToScreenSpace(&x,&y);

    float r = _MAX(GetWidth(g), GetHeight(g)) * fxtimer;
    al_draw_circle(x,y,r,c,1);

    int w = _MIN(GetWidth(g),r);
    int h = _MIN(GetHeight(g),r);

    if (g->summonTime < g->summonMax)
    {   
        Rect r = (Rect){x - (w/2), y - (h/2),(w),(h)};
        DrawFilledRect_Dithered(r, c);

       // al_draw_filled_rectangle(x - (w/2), y - (h/2), x+(w/2), y+(h/2), c);
    }
}
void DrawGameObj(GameObject* g, bool forceInverse)
{   
    if (!(g->properties & OBJ_ACTIVE))
        return;
    
    //bool b = IsOwnedByPlayer(g);
    //ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
    //if (ObjIsDecoration(g))
    //    c = BG;
    ALLEGRO_COLOR c = GetColor(GameObjToColor(g),GetPlayerOwnedBy(g));
    DrawSummonEffect(g);

    if (g->summonTime >= g->summonMax)
        RedrawMapSegmentUnderObj(g);
        
    Sprite* s = ObjIsChannelling(g) ? &sprites[g->channelingSpriteIndex] :  &sprites[g->spriteIndex];
    bool isReversed = IsSelected(g) || forceInverse;
    isReversed = g->flashTimer > 0 ? !isReversed : isReversed;

    float x = g->position.screenX + g->offset.x; 
    float y = g->position.screenY + g->offset.y;
    
    float percent = GetSummonPercent(g);  


    Rect selectRect;
    selectRect.w = al_get_bitmap_width(s->sprite);
    selectRect.h = (al_get_bitmap_height(s->sprite) * percent);
    selectRect.x = x;
    selectRect.y = (y + (al_get_bitmap_height(s->sprite) - selectRect.h));


    if (g->summonTime < g->summonMax)
    {
        float sx = 0;
        float sy = (GetHeight(g) - (GetHeight(g)*percent));
        float sw = GetWidth(g);
        float sh = (GetHeight(g) * percent);
        c = GROUND_DARK;
        //DrawSpriteRegion(s,sx,sy,sw,sh,x,(y+sy),GetColor(COLOR_GROUND_DARK,GetPlayerOwnedBy(g)),false);
    }
    else
        DrawSprite(s,x,y,0.5f,0.5f,g->angle,c, isReversed);
    

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

        DrawArrow(ToScreenSpace_X(c1.x),ToScreenSpace_Y(c1.y),ToScreenSpace_X(c2.x),ToScreenSpace_Y(c2.y),c);
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
        
        al_draw_filled_circle(ToScreenSpace_X(circleCenterX),ToScreenSpace_Y(circleCenterY),2,c);
    }
    
}
void SetLightSize(GameObject* g, int size)
{
    g->lightSize = clamp(size,0,MAX_LIGHT_SIZE);
    GenerateCircleHighlight(size);
}
void GenerateCircleHighlight(int lightSize)
{
    if (lightSize < 0 || lightSize > MAX_LIGHT_SIZE)
        return;
    if (lights[lightSize])
    {
        return;
    }
    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    ALLEGRO_BITMAP* light = al_create_bitmap(lightSize*2,lightSize*2);
    al_set_target_bitmap(light);
    al_lock_bitmap(light,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    lights[lightSize] = light;
   
    int cx = lightSize;
    int cy = lightSize;

    int r = lightSize;
    int x = -r;
    int y = 0;
    int err = 2-2*r;


    while (x < 0)
    {
        PointI points[4];
        points[0].x = cx-x;  points[0].y = cy+y;
        points[1].x = cx-y;  points[1].y = cy-x;
        points[2].x = cx+x;  points[2].y = cy-y; 
        points[3].x = cx+y;  points[3].y = cy+x; 

        int angles[4];
        angles[0] = PointsToAngleDeg(cx,cy,points[0].x,points[0].y);
        angles[1] = angles[0] + 90;
        angles[2] = PointsToAngleDeg(cx,cy,points[2].x,points[2].y);
        angles[3] = (angles[2] + 90);

        for (int i = 0; i < 4; i++)
        {
            int angle = (angles[i]);//PointsToAngleRad(cx,cy,points[i].x,points[i].y);
            float moveX = cosTable[angle];
            float moveY = sinTable[angle];


            float mX = cx;
            float mY = cy;

            for (int steps = 0; steps < lightSize; steps++)
            {
                mX += moveX;
                mY += moveY;

                int touchedCoordX = mX;//((mX-cx)+lightSize);
                int touchedCoordY = mY;//((mY-cy)+lightSize);
                
                //if (!touched[touchedCoordX][touchedCoordY])
                {
                    ALLEGRO_COLOR col = al_get_pixel(sprites[currMap->spriteIndex].sprite,mX,mY);//al_get_pixel(sprites[currMap->spriteIndex].sprite,mX,mY);

                    //if (AlColIsEq(colGround,GROUND))
                    {
                        float fac = (float)(lightSize-steps+1);
                        float f = 1/(lightSize/(fac*fac));
                        f*=LIGHT_FACTOR;
                        f = f > 1 ? 1 : f;
                        ALLEGRO_COLOR c2 = al_map_rgba_f(f,f,f,f);
                        al_put_blended_pixel(mX,mY,c2);
                        //touched[touchedCoordX][touchedCoordY] = true;
                    }
                }
            }
        } 
      r = err;
      if (r <= y) 
      {
        y++;
        err += (y << 1) + 1;       

      }
      if (r > x || err > y) 
      {
        x++;
        //x*2
        err += (x << 1) + 1;
      }


     }
    al_unlock_bitmap(light);
    al_set_target_bitmap(before);
}
void DrawMapHighlight(GameObject* g, int lightSize, ALLEGRO_BITMAP* screen)
{
    //Use circle algorithm to get the points to end at
    //Then calculate the angle between gCX and gCY and that point
    //Then go from g to that point


    float cxf; float cyf;
    GetCentre(g,&cxf,&cyf);

    int cx = (cxf);
    int cy = (cyf);

    int r = lightSize;
    int x = -r;
    int y = 0;
    int err = 2-2*r;


    while (x < 0)
    {
        PointI points[4];
        points[0].x = cx-x;  points[0].y = cy+y;
        points[1].x = cx-y;  points[1].y = cy-x;
        points[2].x = cx+x;  points[2].y = cy-y; 
        points[3].x = cx+y;  points[3].y = cy+x; 

        int angles[4];
        angles[0] = PointsToAngleDeg(cx,cy,points[0].x,points[0].y);
        angles[1] = angles[0] + 90;
        angles[2] = PointsToAngleDeg(cx,cy,points[2].x,points[2].y);
        angles[3] = (angles[2] + 90);

        for (int i = 0; i < 4; i++)
        {
            int angle = (angles[i]);//PointsToAngleRad(cx,cy,points[i].x,points[i].y);
            float moveX = cosTable[angle];
            float moveY = sinTable[angle];


            float mX = cx;
            float mY = cy;

            for (int steps = 0; steps < lightSize; steps++)
            {
                mX += moveX;
                mY += moveY;

                ALLEGRO_COLOR col = al_get_pixel(sprites[currMap->spriteIndex].sprite,mX,mY);//al_get_pixel(sprites[currMap->spriteIndex].sprite,mX,mY);

                if (col.a <= 0.001f)
                {
                    float screenSpaceX = mX; float screenSpaceY = mY;
                    ToScreenSpace(&screenSpaceX,&screenSpaceY);
                    al_put_pixel(screenSpaceX - moveX,screenSpaceY - moveY,EDGE_HIGHLIGHT);
                    break;
                }

            }
        } 
      r = err;
      if (r <= y) 
      {
        y++;
        err += (y << 1) + 1;       

      }
      if (r > x || err > y) 
      {
        x++;
        //x*2
        err += (x << 1) + 1;
      }


     }


}
void SetRange(GameObject* g, float range)
{
    g->range = _MAX(range,MINIMUM_RANGE);
}
void SetAttackingObj(GameObject* g, GameObject* target)
{
    g->targObj = target;
}
void AttackTarget(GameObject* g, float dt)
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
            //CallLuaFunc(g->luafunc_onattack);
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,g->luafunc_onattack);
            lua_pushnumber(luaState,g->targObj - objects);
            lua_pcall(luaState,1,0,0);

        }

        float damage = g->baseDamage;
        ProcessItemsOnAttack(g,dt,&damage);
        
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
        if (Damage(g,g->targObj,damage,true))
        {
            g->targObj = NULL;
        }

    }
}
Rect GetObjRect(GameObject* g)
{
    if (!g) return (Rect){0,0,0,0};
    if (g->spriteIndex <= 0) return (Rect){g->position.worldX,g->position.worldY,0,0};
    Rect r = (Rect){g->position.worldX,g->position.worldY,al_get_bitmap_width(sprites[g->spriteIndex].sprite),al_get_bitmap_height(sprites[g->spriteIndex].sprite)};
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
float GetAttackRange(GameObject* g)
{
    return (_MAX(g->range,MINIMUM_RANGE));
}
void Stun(GameObject* source, GameObject* g, float value)
{
    if (!g) return;
    if (!g->objectIsStunnable)
        return;
    g->stunTimer += value;

    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        Ability* a = &g->abilities[i];
        a->cdTimer = _MAX(g->stunTimer,a->cdTimer);
    }
}


bool Damage(GameObject* source, GameObject* g, float value, bool triggerItems)
{
    if (!g) return false;

    if (!IsActive(g))
         return false;
    if (ObjIsInvincible(g))
        return false;
    if (g->invulnerableTime > 0)
    {
        return false;
    }
    if (triggerItems)
        ProcessItemsOnDamaged(source,g,&value);
    value -= g->armor;
    if (value < 1)
        value = 1;
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
    if (source && GetWidth(g) > 0 && GetHeight(g) > 0)
        AddDamageNumber((int)value,g->position.worldX+(rand()%(int)GetWidth(g)*1.1f),g->position.worldY+(rand()%(int)GetHeight(g)*1.1f),source);

    value = DamageShields(g,value);
    g->health -= value;
    if (source && source != g)
    {
        if (source->lifesteal > 0)
        {
            Heal(source,source->lifesteal * value);
        }
    }
    if (g != source && value > 0)
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
void SetObjectCanPush(GameObject* g, bool value)
{
    if (value)
    {
        g->properties |= OBJ_CAN_PUSH;
    }
    else
    {
        g->properties &= ~OBJ_CAN_PUSH;

    }
}

void Teleport(GameObject* g, float x, float y)
{
    if (!currMap->collision)
        return;
    if (!g) return;

    PointI target = (PointI){x/_GRAIN,y/_GRAIN};
    PointI here = (PointI){g->position.worldX/_GRAIN,g->position.worldY/_GRAIN};
    bool found = false;

    PointI move = GetClosestPathablePoint(target,here,&found,GetWidth(g)/_GRAIN,GetHeight(g)/_GRAIN,true);
    
    //g->position.worldX = move.x*_GRAIN;
    //g->position.worldY = move.y*_GRAIN;
    SetMapCollisionRect(g->position.worldX,g->position.worldY,GetWidth(g),GetHeight(g),false);


    UpdateObjPosition(g, move.x*_GRAIN, move.y*_GRAIN);



    //CheckCollisions(g,true, 1, true);
    //CheckCollisions(g,false, 1, true);  
 
    SetTargetPosition(g,g->position.worldX,g->position.worldY);
    SetMapCollisionRect(g->position.worldX,g->position.worldY,GetWidth(g),GetHeight(g),true);
    return;

    


}
void GetOffsetCenter(GameObject* g, float* x, float* y)
{
    if (!g) 
    {
        *x = 0;
        *y = 0;
        return;
    }
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
    *x = g->position.worldX + al_get_bitmap_width(sprites[g->spriteIndex].sprite)/2.0f;
    *y = g->position.worldY + al_get_bitmap_height(sprites[g->spriteIndex].sprite)/2.0f;

}
void GetCentre_Screen(GameObject* g, float* x, float* y)
{
    if (!g)
    {
        *x = 0;
        *y = 0;
         return;
    }
    *x = g->position.screenX + al_get_bitmap_width(sprites[g->spriteIndex].sprite)/2.0f;
    *y = g->position.screenY + al_get_bitmap_height(sprites[g->spriteIndex].sprite)/2.0f;

}

void DoAI(GameObject* g)
{
    if (!g->shouldProcessAI)
        return;
    if (ObjIsDecoration(g))
        return;
    if (g->threatList.obj && g->channellingTime <= 0)
    {
        Threat* t = GetHighestThreat(&g->threatList);
        if (t)
        {
            ClearCommandQueue(g);
            AttackCommand(g,t->obj,false);
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
                if (GetPlayerOwnedBy(g) != GetPlayerOwnedBy(&objects[i]) && !ObjIsDecoration(&objects[i]))
                {
                    if (GetDist(g,&objects[i]) < g->aggroRadius)
                    {
                        //also bring in the others
                        for (int j = 0; j < MAX_OBJS; j++)
                        {
                            GameObject* jg = &objects[j];
                            if (GetPlayerOwnedBy(g) == GetPlayerOwnedBy(&objects[j]) && !ObjIsDecoration(&objects[j]))
                            {
                                if (GetDist(&objects[i],&objects[j]) < objects[j].aggroRadius)
                                    AddThreat(&objects[i],&objects[j],0);
                                
                            }
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
    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (!IsActive(&objects[i]))
            continue;
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
    Rect r1 = (Rect){g1->position.worldX,g1->position.worldY,GetWidth(g1),GetHeight(g1)};
    Rect r2 = (Rect){g2->position.worldX,g2->position.worldY,GetWidth(g2),GetHeight(g2)};
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
        
        currGameObjRunning = g;
        currAbilityRunning = g->channelledAbility;
        
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,g->channelledAbility->luafunc_onchanneled);
        lua_pushnumber(luaState,g-objects);
        lua_pushnumber(luaState,g->channellingTime);
        lua_pushnumber(luaState,g->channellingTotal);
        lua_pushnumber(luaState,g->channelled_target - objects);
        lua_pushnumber(luaState,g->channelled_x);
        lua_pushnumber(luaState,g->channelled_y);
        lua_pushnumber(luaState,g->target_heading_x);
        lua_pushnumber(luaState,g->target_heading_y);

        lua_pcall(luaState,8,0,0);


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
                al_draw_line(ToScreenSpace_X(x),ToScreenSpace_Y(y),ToScreenSpace_X(x2),ToScreenSpace_Y(y2),col,1);
            }
            if (a->targetingHint == HINT_CIRCLE)
            {
                al_draw_circle(ToScreenSpace_X(x2),ToScreenSpace_Y(y2),a->hintRadius,col,1);
            }
            if (a->targetingHint == HINT_CONE)
            {
                float angle;
                angle = atan2(y2-y,x2-x);
                float length = dist(x,y,x2,y2);
                DrawCone(ToScreenSpace_X(x),ToScreenSpace_Y(y),RadToDeg(angle),a->hintRadius,200,ENEMY);   
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
void AddAttackSpeed(GameObject* g, float speed)
{
    g->attackSpeed += speed;
}
void AddAttackRange(GameObject* g, float range)
{
    g->range += range;
}
void AddArmor(GameObject* g, float armor)
{
    g->armor += armor;
}

int GetNumberOfActiveEffects(GameObject* g)
{
    if (!g) return 0;
    int numEffects = 0;
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (g->effects[i].enabled)
            numEffects++;
    }
    return numEffects;
}
void AddLifesteal(GameObject* g, float value)
{
    g->lifesteal += value;
}
void SetObjIsBoss(GameObject* g, bool value)
{
    g->isBoss = value;
}

int GetNumActiveObjects()
{
    int numObjs = 0;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        if (IsActive(&objects[i]))
            numObjs++;
    }
    return numObjs;
}