#include "gamesave.h"
#include <string.h>
#include "gameobject.h"
#include "console.h"
#include "gamestate.h"
#include "helperfuncs.h"
#include "encounter.h"
#include "map.h"
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

GameSave LoadGameSave(char* path)
{
    //ALLEGRO_FILE* file = al_fopen(path,"rb");
    char* buff = readFile(path);
    GameSave g = (GameSave){0};
    if (buff)
    {
        char* position = buff;
        int32_t numCharsHeader = 0;
        //header
        memcpy(&numCharsHeader,position,sizeof(int32_t));
        char* header = calloc(numCharsHeader+1,sizeof(char));
        position += sizeof(int32_t);
        memcpy(header,position,numCharsHeader*sizeof(char));

        position += numCharsHeader * sizeof(char);

        if (strcmp(header,GAME_VERSION) != 0)
        {
            free(buff);
            ConsolePrintf("LoadGameSave: wrong version. Could not load");
            return (GameSave){0};
        }
        //map path
        int32_t numCharsMapPath = 0;
        memcpy(&numCharsMapPath,position,sizeof(int32_t));
        position += sizeof(numCharsMapPath);
        char* mapPath = calloc(numCharsMapPath+1,sizeof(char));
        memcpy(mapPath,position,sizeof(char)*numCharsMapPath);
        position += sizeof(char) * numCharsMapPath;

        //encounter path
        int32_t numCharsEncounterPath = 0;
        memcpy(&numCharsEncounterPath,position,sizeof(int32_t));
        position += sizeof(numCharsEncounterPath);
        char* encounterPath = calloc(numCharsEncounterPath+1,sizeof(char));
        memcpy(encounterPath,position,sizeof(char)*numCharsEncounterPath);
        position += sizeof(char) * numCharsEncounterPath;

        float gold = 0;
        memcpy(&gold,position,sizeof(gold));
        position += sizeof(gold);

        float time = 0;
        memcpy(&time,position,sizeof(time));
        position += sizeof(time);
        
        int32_t numObjs = 0;
        memcpy(&numObjs,position,sizeof(numObjs));
        position += sizeof(numObjs);

        GameObject* objs = calloc(numObjs,sizeof(GameObject));
        for (int i = 0; i < numObjs; i++)
        {
            int32_t pathNumChars = 0;
            memcpy(&pathNumChars,position,sizeof(int32_t));
            position += sizeof(pathNumChars);

            objs[i].path = calloc(pathNumChars+1,sizeof(char));
            memcpy(objs[i].path,position,pathNumChars*sizeof(char));
            position += pathNumChars * sizeof(char);

            memcpy(&objs[i].health,position,sizeof(float));
            position += sizeof(float);
            memcpy(&objs[i].mana,position,sizeof(float));
            position += sizeof(float);
            memcpy(&objs[i].position.worldX,position,sizeof(float));
            position += sizeof(float);
            memcpy(&objs[i].position.worldY,position,sizeof(float));
            position += sizeof(float);  

            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                int32_t inventoryPathNumChars = 0;
                memcpy(&inventoryPathNumChars,position,sizeof(int32_t));
                position += sizeof(inventoryPathNumChars);

                if (inventoryPathNumChars > 0)
                {
                    objs[i].inventory[j].path = calloc(inventoryPathNumChars+1,sizeof(char));
                    memcpy(objs[i].inventory[j].path,position,sizeof(char) *inventoryPathNumChars);
                    position += sizeof(char) *inventoryPathNumChars;
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