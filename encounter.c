#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "encounter.h"

#include "helperfuncs.h"
#include "luafuncs.h"
#include "gameobject.h"
#include "map.h"
#include "player.h"
#include "console.h"


Encounter** encounters = NULL;
int numEncounters = 0;
int numEncountersAlloced = 0;

float encounterMoveTimer = 0;
float encounterOffset = 0;
float encounterMoveTo = 0;

Encounter* currEncounterRunning = NULL;

int selectedEncounterIndex = 0;

float goldLossRate = 0.12f;

void SetEncounterProfited(Encounter* e, int amountProfited)
{
    if (amountProfited <= 0)
        return;

    if (amountProfited > e->bestProfited)
    {
        e->bestProfited += amountProfited;
    }
    e->totalProfit += amountProfited;

}
bool HasChest(Encounter* e, int chestIndex, float time)
{
    if (chestIndex >= MAX_CHESTS || chestIndex < 0)
        return false;
    float minutes = time/60.0f;

    return minutes <= e->timeBreakpoints[chestIndex];
}
float GetReward(Encounter* e, int augmentLevel, int chestIndex)
{
    float range = RandRange(-0.1,0.1);
    float reward = e->baseReward * (chestIndex+1) * (e->augment+1);
    reward += reward * range;

    return reward;
}
void LoadEncounter(char* dirPath, lua_State* l)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);

    Encounter* e = calloc(1,sizeof(Encounter));
    
    //defaults
    e->numUnitsToSelect = 4;
    e->unlockBasePrice = 50;
    e->encounterShouldBeSkipped = false;
    e->timeBreakpoints[0] = 35;
    e->timeBreakpoints[1] = 30;
    e->timeBreakpoints[2] = 25;
    e->timeBreakpoints[3] = 20;
    e->timeBreakpoints[4] = 15;

    e->baseReward = 50;


    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                if (strcasecmp(dir->d_name,"encounter.lua")==0)
                {
                    e->path = calloc(strlen(dirPath)+1,sizeof(char));
                    strcpy(e->path,dirPath);


                    if (!e->lua_buffer.buffer)
                    {
                        char* concatted = calloc(strlen(dirPath)+strlen(dir->d_name)+1,sizeof(char)); 
                        strcpy(concatted,dirPath);
                        strcat(concatted,dir->d_name);
                        e->lua_buffer.buffer = readFile(concatted);
                        free(concatted);
                    }

                    if (!e->lua_buffer.functions)
                    {
                        e->lua_buffer.functions = calloc(NUM_ENCOUNTER_FUNCTIONS,sizeof(char*));
                    }

                    if (CheckFuncExists("setup",&e->lua_buffer))
                    {
                        if (luaL_loadbuffer(l, e->lua_buffer.buffer,strlen(e->lua_buffer.buffer),NULL) || lua_pcall(l, 0, 0, 0))
                        {
                            ConsolePrintf("Can't load lua file %s",lua_tostring(l,-1));
                            fflush(stdout);
                        }
                        else
                        {
                            currEncounterRunning = e;
                            lua_getglobal(l, "setup");
                            int funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
                            e->luafunc_setup = funcIndex;

                            lua_rawgeti(l,LUA_REGISTRYINDEX,funcIndex);
                            lua_pcall(l,0,0,0);

                            if (CheckFuncExists("update",&e->lua_buffer))
                            {
                                lua_getglobal(l, "update");
                                funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
                                e->luafunc_update = funcIndex;
                            }
                            if (CheckFuncExists("updateloadscreen",&e->lua_buffer))
                            {
                                lua_getglobal(l, "updateloadscreen");
                                funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
                                e->luafunc_updateloadscreen = funcIndex;
                            }

                        }
                    }
                    if (!e->name)
                    {                        
                        e->name = calloc(strlen(dirPath)+1,sizeof(char));
                        strcpy(e->name,dirPath);
                    }
                }   
            }
        }
    }
    if (numEncounters >= numEncountersAlloced)
    {
        numEncountersAlloced++;
        encounters = realloc(encounters,numEncountersAlloced*sizeof(Encounter*));
    }

    InitButton(&e->encounter_ButtonLeft,"<","<",0,224,48,16,0);
    InitButton(&e->encounter_ButtonConfirm,"Select Party","Select Party",0,224,96,16,0);
    InitButton(&e->encounter_ButtonRight,">",">",0,224,48,16,0);
    InitButton(&e->encounter_RerollAugments,"","",220,50,20,20,LoadSprite("assets/ui/augments/reroll.png",true));
    InitButton(&e->encounter_PurchaseAugment,"","",220,50,60,10,0);

    e->augment = 0;
    ClearAugments(e);
    encounters[numEncounters] = e;
    numEncounters++;

    if (d)
        closedir(d);


}
bool FileIsInFolder(char* dirPath, char* fileName)
{

    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);
    if (d) {
    while ((dir = readdir(d)) != NULL) {
        if (strcasecmp(dir->d_name,fileName)==0)
        {
            closedir(d);
            return true;
        }
    }
    closedir(d);
    }
    return false;

}
int sortEncounters(const void* a, const void* b)
{
    Encounter* e =  *(Encounter**)a;
    Encounter* e2 = *(Encounter**)b;
    if (e->encounterShouldBeSkipped) return -1;
    if (e->difficulty == e2->difficulty)
        return (strcmp(e->path,e2->path));
    return ( e->difficulty - e2->difficulty);
}

void LoadEncounters(char* dirPath, lua_State* l)
{

    encounterOffset = 0;
    encounterMoveTo = 0;
    
    numEncountersAlloced=0;
    numEncounters=0;
    encounters = NULL;
    selectedEncounterIndex = 0;

    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);
    //get number of encounters
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                char* dirConcat = calloc(strlen(dirPath)+strlen(dir->d_name)+3,sizeof(char));
                strcpy(dirConcat,dirPath);
                strcat(dirConcat,"/");
                strcat(dirConcat,dir->d_name);
                strcat(dirConcat,"/");
                
                if (FileIsInFolder(dirConcat,"encounter.lua"))
                {
                    numEncountersAlloced++;
                }
                free(dirConcat);
            }
        }   
        rewinddir(d);
    }
    encounters = calloc(numEncountersAlloced,sizeof(Encounter*));
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                char* dirConcat = calloc(strlen(dirPath)+strlen(dir->d_name)+3,sizeof(char));
                strcpy(dirConcat,dirPath);
                strcat(dirConcat,"/");
                strcat(dirConcat,dir->d_name);
                strcat(dirConcat,"/");
                LoadEncounter(dirConcat,l);
                free(dirConcat);
            }
        }
        closedir(d);
    }
    qsort(encounters,numEncounters,sizeof(Encounter*),sortEncounters);
    ResetEncounterPosition();
}
void NextEncounter()
{

    if (numEncounters == 0)
        return;

    selectedEncounterIndex++;

    /*if (selectedEncounterIndex < numEncounters)
    {
        Encounter* e = encounters[selectedEncounterIndex];
        if (e->encounterShouldBeSkipped || !e->unlocked)
        {
            selectedEncounterIndex--;

            return;    
        }
    }
    else
    {
        selectedEncounterIndex--;
        return;
    }*/
    encounterMoveTo += _SCREEN_SIZE;
    encounterMoveTimer = 0;

    selectedEncounterIndex = (selectedEncounterIndex) % (numEncounters);
    Encounter* e = encounters[selectedEncounterIndex];
    while (encounters[selectedEncounterIndex]->encounterShouldBeSkipped)
    {
        selectedEncounterIndex++;
        selectedEncounterIndex = (selectedEncounterIndex) % (numEncounters);
        encounterMoveTo += _SCREEN_SIZE;

    }


}
void PreviousEncounter()
{
    if (numEncounters == 0)
        return;

    selectedEncounterIndex--;

    /*if (selectedEncounterIndex >= 0)
    {
        Encounter* e = encounters[selectedEncounterIndex];
        if (e->encounterShouldBeSkipped || !e->unlocked)
        {
            selectedEncounterIndex++;
            return;    
        }
    }
    else
    {
        selectedEncounterIndex++;
        return;
    }*/
    encounterMoveTo -= _SCREEN_SIZE;
    encounterMoveTimer = 0;

    selectedEncounterIndex = (selectedEncounterIndex) % (numEncounters-1);
    while (encounters[selectedEncounterIndex]->encounterShouldBeSkipped)
    {
        selectedEncounterIndex--;
        if (selectedEncounterIndex < 0)
            selectedEncounterIndex = numEncounters-1;
        encounterMoveTo -= _SCREEN_SIZE;
        
    }

}
void ResetEncounterPosition()
{
    selectedEncounterIndex = 0;
    encounterMoveTo = 0;
    //qsort(encounters,numEncounters,sizeof(Encounter*),sortEncounters);
    while (encounters[selectedEncounterIndex]->encounterShouldBeSkipped)
    {
        NextEncounter();
    }
    encounterOffset = encounterMoveTo;
    encounterMoveTimer = 1;


}
Encounter* GetEncounterByName(char* name)
{
    for (int i = 0; i < numEncounters; i++)
    {
        Encounter* e = encounters[i];
        if (e->name)
        {
            if (strcmp(e->name,name)==0)
            {
                return e;
            }
        }
    }
    return NULL;
}
void UpdateEncounter(float dt)
{
    currGameObjRunning = NULL;
    currAbilityRunning = NULL;
    currAttackRunning = NULL;

    if (currEncounterRunning)
    {
        //CallLuaFunc(currEncounterRunning->luafunc_update);
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,currEncounterRunning->luafunc_update);
        lua_pushnumber(luaState,dt);
        lua_pcall(luaState,1,0,0);

        UpdateMap(currMap,dt);
        if (gameState == GAMESTATE_INGAME)
            AddGold(-dt * goldLossRate);
    }
}
bool IsSeperator(char c)
{
    return (c == '/' || c == '\\');
}
//currently unsupported: going backwards in directory,
//eg, assets/../assets/encounters/01 
bool PathCmp(char* path1, char* path2)
{
    if (strlen(path2) > strlen(path1))
    {
        char* tmp = path2;

        path2 = path1;
        path1 = tmp;
    }

    int path2len = strlen(path2);
    for (int i = 0; i < strlen(path1); i++)
    {
        if (IsSeperator(path1[i]))
            continue;

        if (i > path2len)
        {
            return false;
        }
        else
        {
            if (tolower(path1[i]) != tolower(path2[i]))
            {
                return false;
            }
        }
    }
    return true;
}
void UnlockEncounter(const char* path, int difficultyUnlocked, int bestChest, int bestProfit, int totalProfit, bool overwrite)
{
    for (int i = 0; i < numEncounters; i++)
    {
        if (encounters[i]->path && PathCmp(encounters[i]->path, (char*)path))
        {
            if (!encounters[i]->unlocked || overwrite)
            {
                encounters[i]->difficultyUnlocked = difficultyUnlocked;
                encounters[i]->bestChest = bestChest;
                encounters[i]->bestProfited = bestProfit;
                encounters[i]->totalProfit = totalProfit;
                
            }
            encounters[i]->unlocked = true;
        }
    }
    ResetEncounterPosition();

}
int GetAugmentCost(Encounter* e, int level)
{
    return (e->unlockBasePrice * level * level);
}