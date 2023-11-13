#pragma once
#include <stdint.h>
#include <stdbool.h>    
typedef struct GameObject GameObject;

typedef struct GameSave
{
    char* mapPath;
    char* encounterPath;
    int32_t numObjects;
    GameObject* objects;
    float gold;
    float time;
    
}GameSave;


extern GameSave* continuePoint;

GameSave SaveGameState(char* mapPath, char* encounterPath, float gold, float time, int numObjectsToSave, GameObject** objects);
void GameSaveToDisk(char* path, GameSave* s);
GameSave LoadGameSave(char* path, bool* error);
void DestroyGameSave(GameSave* ga);
void RunGameSave(GameSave* s);
