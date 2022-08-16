#pragma once

#include <stdbool.h>

#define _GRAIN 4
typedef struct Sprite Sprite; 
#define _MAPSIZE 256
typedef struct GameObject GameObject;

typedef struct Map 
{   
    bool* collision;//[_MAPSIZE*_MAPSIZE/_GRAIN];
    char* path;
    unsigned int spriteIndex;
    unsigned int secondLayerSpriteIndex;


    int luafunc_update;
    int luafunc_setup;
    char* lua_buffer;
    char* name;

} Map;

Map* currMap;
Map* maps;
int numMaps;

void PreprocessMap(Map* map);
Map* LoadMap(char* path);
void SetMap(Map* m);
void InitMaps();
void DrawMap();
void UpdateMap(Map* m, float dt);
void RedrawMapSegmentUnderObj(GameObject* g);
int GetMapWidth();
int GetMapHeight();
