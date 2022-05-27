#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "encounter.h"

#include "helperfuncs.h"
#include "luafuncs.h"
#include "gameobject.h"

void LoadEncounter(char* dirPath, lua_State* l)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);

    Encounter* e = calloc(1,sizeof(Encounter));
    e->numUnitsToSelect = 4;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                if (strcasecmp(dir->d_name,"encounter.lua")==0)
                {
                    if (!e->lua_buffer)
                    {
                        char* concatted = calloc(strlen(dirPath)+strlen(dir->d_name)+1,sizeof(char)); 
                        strcpy(concatted,dirPath);
                        strcat(concatted,dir->d_name);
                        e->lua_buffer = readFile(concatted);
                        free(concatted);
                    }
                    if (CheckFuncExists("setup",e->lua_buffer))
                    {
                        if (luaL_loadbuffer(l, e->lua_buffer,strlen(e->lua_buffer),NULL) || lua_pcall(l, 0, 0, 0))
                        {
                            printf("Can't load lua file %s",lua_tostring(l,-1));
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
                        }
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
    encounters[numEncounters] = e;
    numEncounters++;

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

       return ( e->difficulty - e2->difficulty);
}
void LoadEncounters(char* dirPath, lua_State* l)
{
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
}