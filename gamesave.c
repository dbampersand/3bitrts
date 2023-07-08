#include "gamesave.h"
#include <string.h>
#include "gameobject.h"
#include "console.h"
#include "gamestate.h"
#include "helperfuncs.h"
#include "encounter.h"
#include "map.h"
#include "editor.h"
GameSave SaveGameState(char* mapPath, char* encounterPath, float gold, float time, int numObjectsToSave, GameObject** objects)
{
    GameSave g = {0};

    if (!mapPath || !encounterPath)
        return g;

    g.mapPath = calloc(strlen(mapPath)+1,sizeof(char));
    g.encounterPath = calloc(strlen(encounterPath)+1,sizeof(char));

    strcpy(g.mapPath,mapPath);
    strcpy(g.encounterPath,encounterPath);

    g.gold = gold;
    g.time = time;

    g.objects = calloc(numObjectsToSave,sizeof(GameObject));
    for (int i = 0; i < numObjectsToSave; i++)
    {
        GameObject* objToAdd = objects[i];

        g.objects[i] = *objToAdd;
        g.objects[i].path = calloc(strlen(objToAdd->path)+1,sizeof(char));
        g.objects[i].description = calloc(strlen(objToAdd->description)+1,sizeof(char));
        g.objects[i].name = calloc(strlen(objToAdd->name)+1,sizeof(char));

        strcpy(g.objects[i].path,objToAdd->path);
        strcpy(g.objects[i].description,objToAdd->description);
        strcpy(g.objects[i].name,objToAdd->name);

        for (int j = 0; j < INVENTORY_SLOTS; j++)
        {
            if (objToAdd->inventory[j].path)
            {
                g.objects[i].inventory[j].path = calloc(strlen(objToAdd->inventory[j].path)+1,sizeof(char));
                strcpy(g.objects[i].inventory[j].path,objToAdd->inventory[j].path);
            }
        }   

    }
    g.numObjects = numObjectsToSave;

    return g;
}
void DestroyGameSave(GameSave* ga)
{
    if (ga->mapPath)
        free(ga->mapPath);
    if (ga->encounterPath)
        free(ga->encounterPath);

    for (int i = 0; i < ga->numObjects; i++)
    {
        GameObject* obj = &ga->objects[i];
        if (obj->path)
            free(obj->path);
        for (int j = 0; j < INVENTORY_SLOTS; j++)
        {
            if (obj->inventory[j].path)
                free(obj->inventory[j].path);
        }
    }  
    memset(ga,0,sizeof(GameSave));

}
void GameSaveToDisk(char* path, GameSave* s)
{
    ALLEGRO_FILE* file = al_fopen(path, "wb+");
    if (file == NULL)
    {
        ConsolePrintf("Couldn't save file\n");   
        return;
    }
    int32_t lenMapPath = strlen(s->mapPath);
    int32_t lenEncounterPath = strlen(s->encounterPath);
    int32_t numCharsGameVersion = strlen(GAME_VERSION);
    al_fwrite(file,&numCharsGameVersion,sizeof(numCharsGameVersion));
    al_fwrite(file,GAME_VERSION,strlen(GAME_VERSION)*sizeof(char));

    al_fwrite(file,&lenMapPath,sizeof(lenMapPath));
    al_fwrite(file,s->mapPath,strlen(s->mapPath)*sizeof(char));

    al_fwrite(file,&lenEncounterPath,sizeof(lenMapPath));
    al_fwrite(file,s->encounterPath,strlen(s->encounterPath)*sizeof(char));

    al_fwrite(file,&s->gold,sizeof(s->gold));
    al_fwrite(file,&s->time,sizeof(s->time));

    al_fwrite(file,&s->numObjects,sizeof(s->numObjects));
    
    //not totally portable but should be fine (saves will probably break if sent to another pc)
    for (int i = 0; i < s->numObjects; i++)
    {
        char* path = s->objects[i].path;
        float hp = s->objects[i].health;
        float mana = s->objects[i].mana;
        float x = s->objects[i].position.worldX + GetWidth(&s->objects[i])/2.0f;
        float y = s->objects[i].position.worldY+ GetHeight(&s->objects[i])/2.0f;


        Item* inventory = s->objects[i].inventory;

        int32_t numCharsPath = strlen(path);

        al_fwrite(file,&numCharsPath,sizeof(int32_t));
        al_fwrite(file,path,sizeof(char) * numCharsPath);
        al_fwrite(file,&hp,sizeof(hp));
        al_fwrite(file,&mana,sizeof(mana));
        al_fwrite(file,&x,sizeof(x));
        al_fwrite(file,&y,sizeof(y));


        for (int j = 0; j < INVENTORY_SLOTS; j++)
        {
            int32_t inventoryPathNumChars = 0;
            if (inventory[j].enabled && inventory[j].path)
            {
                inventoryPathNumChars = strlen(inventory[j].path);
            }
            al_fwrite(file,&inventoryPathNumChars,sizeof(inventoryPathNumChars));
            if (inventory[j].enabled && inventory[j].path)
                al_fwrite(file,inventory[j].path,inventoryPathNumChars*sizeof(char));
        }
    }

    al_fclose(file);
}

void RunGameSave(GameSave* s)
{
    continuePoint = s;
    SetGameStateToContinueSave();
}
bool CheckGameSaveHeader(char* path)
{
    size_t fileSize;
    char* buff = readFile(path, &fileSize);
    if (!buff)
        return false;
    char* position = buff;
    int32_t numCharsHeader = 0;
    //header
    memcpy(&numCharsHeader,position,sizeof(int32_t));
    char* header = calloc(numCharsHeader+1,sizeof(char));
    position += sizeof(int32_t);
    if (strlen(position) < numCharsHeader)
        return false;
    memcpy(header,position,numCharsHeader*sizeof(char));
    bool ret =  (strcmp(header,GAME_VERSION) == 0);
    free(buff);
    free(header);
    return ret;

}   
bool HasEnoughBytes(char* buffer, char* position, size_t numBytes, size_t requiredBytes)
{
    size_t pos = position - buffer;
    int v = (numBytes - pos) - requiredBytes;
    return (v >= 0);
}
bool GetAndAdvancePosition(void* into, char** position,char* buffer, size_t bufferSize, size_t size)
{
    if (!HasEnoughBytes(buffer,*position, bufferSize,size))
        return false;
    memcpy(into,*position, size);
    *position += size;
    return true;
}
void DestroyGameSave_Obj(GameObject* g)
{
    if (g)
    {
        if (g->path)
        {
            free(g->path);
        }
        for (int i = 0; i < INVENTORY_SLOTS; i++)
        {
            if (g->inventory[i].path)
                free(g->inventory[i].path);
        }
    }
}
GameSave LoadGameSave(char* path, bool* error)
{
    *error = false;
    char* buffer; 

    char* header = NULL;
    char* mapPath = NULL;
    char* encounterPath = NULL;
    GameObject* objs = NULL;
    int32_t numObjs = 0;
    if (!CheckGameSaveHeader(path))
    {
        GameSaveError:
        if (header)
            free(header);
        if (mapPath)
            free(mapPath);
        if (encounterPath)
            free(encounterPath);
        if (objs)
        {
            for (int i = 0; i < numObjs; i++)
            {
                DestroyGameSave_Obj(&objs[i]);
            }
            free(objs);
        }
            if (error)
                *error = true;
            return (GameSave){0}; 
    }

    //ALLEGRO_FILE* file = al_fopen(path,"rb");
    size_t fileSize;
    char* buff = readFile(path, &fileSize);
    GameSave g = (GameSave){0};
    if (buff)
    {
        char* position = buff;
        int32_t numCharsHeader = 0;
        //header
        //memcpy(&numCharsHeader,position,sizeof(int32_t));
       // position += sizeof(int32_t);
        if (!GetAndAdvancePosition(&numCharsHeader,&position,buff,fileSize,sizeof(int32_t)))
            goto GameSaveError;
        header = calloc(numCharsHeader+1,sizeof(char));
        if (!GetAndAdvancePosition(header,&position,buff,fileSize,sizeof(char)*numCharsHeader))
        {   
            goto GameSaveError;
        }
        free(header);
        header = NULL;
        //map path
        int32_t numCharsMapPath = 0;
        if (!GetAndAdvancePosition(&numCharsMapPath,&position,buff,fileSize,sizeof(int32_t)))
        {
            goto GameSaveError;
        }
        mapPath = calloc(numCharsMapPath+1,sizeof(char));
        if (!GetAndAdvancePosition(mapPath,&position,buff,fileSize,sizeof(char)*numCharsMapPath))
        {
            goto GameSaveError;
        }

        //encounter path
        int32_t numCharsEncounterPath = 0;
        if (!GetAndAdvancePosition(&numCharsEncounterPath,&position,buff,fileSize,sizeof(int32_t)))
        {
            goto GameSaveError;
        }
        encounterPath = calloc(numCharsEncounterPath+1,sizeof(char));
        if (!GetAndAdvancePosition(encounterPath,&position,buff,fileSize,sizeof(char) * numCharsEncounterPath))
        {
            goto GameSaveError;
        }

        float gold = 0;
        if (!GetAndAdvancePosition(&gold,&position,buff,fileSize,sizeof(float)))
        {
            goto GameSaveError;
        }   

        float time = 0;
        if (!GetAndAdvancePosition(&time,&position,buff,fileSize,sizeof(float)))
        {
            goto GameSaveError;
        }

        numObjs = 0;
        if (!GetAndAdvancePosition(&numObjs,&position,buff,fileSize,sizeof(float)))
        {
            goto GameSaveError;
        }

        objs = calloc(numObjs,sizeof(GameObject));
        for (int i = 0; i < numObjs; i++)
        {
            int32_t pathNumChars = 0;
            if (!GetAndAdvancePosition(&pathNumChars,&position,buff,fileSize,sizeof(int32_t)))
            {
                goto GameSaveError;

            }

            objs[i].path = calloc(pathNumChars+1,sizeof(char));
            if (!GetAndAdvancePosition(objs[i].path,&position,buff,fileSize,sizeof(char) * pathNumChars))
            {
                goto GameSaveError;
            }
            if (!FileExists(objs[i].path))
                goto GameSaveError;

            if (!GetAndAdvancePosition(&objs[i].health,&position,buff,fileSize,sizeof(float)))
            {
                goto GameSaveError;
            }
            if (!GetAndAdvancePosition(&objs[i].mana,&position,buff,fileSize,sizeof(float)))
            {
                goto GameSaveError;
            }
            if (!GetAndAdvancePosition(&objs[i].position.worldX,&position,buff,fileSize,sizeof(float)))
            {
                goto GameSaveError;
            }
            if (!GetAndAdvancePosition(&objs[i].position.worldY,&position,buff,fileSize,sizeof(float)))
            {
                goto GameSaveError;
            }

            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                int32_t inventoryPathNumChars = 0;
                
                if (!GetAndAdvancePosition(&inventoryPathNumChars,&position,buff,fileSize,sizeof(int32_t)))
                {
                    goto GameSaveError;
                }


                if (inventoryPathNumChars > 0)
                {
                    objs[i].inventory[j].path = calloc(inventoryPathNumChars+1,sizeof(char));
                    if (!GetAndAdvancePosition(objs[i].inventory[j].path,&position,buff,fileSize,sizeof(char) * inventoryPathNumChars))
                    {
                        goto GameSaveError;
                    }
                    if (!FileExists(objs[i].inventory[j].path))
                        goto GameSaveError;

                }

            }
        }
        g.mapPath = mapPath;
        g.encounterPath = encounterPath;
        g.numObjects = numObjs;
        g.objects = objs;
        g.gold = gold;
        g.time = time;
        free(buff);

    }
    if (continuePoint)
    {
        DestroyGameSave(continuePoint);
        free(continuePoint);
    }
    continuePoint = calloc(1,sizeof(GameSave));
    *continuePoint = g;
    return g;
}