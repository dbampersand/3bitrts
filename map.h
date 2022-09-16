#pragma once

#include <stdbool.h>
#include "point.h"

#define _GRAIN 4
typedef struct Sprite Sprite; 
#define _MAPSIZE 256
typedef struct GameObject GameObject;

typedef enum CollisionMapValue {
    COLLISION_NEVER = 0,
    COLLISION_OPEN = 1,
    COLLISION_WORLD_AND_OBJECT = 2,
}CollisionMapValue;

typedef struct Map 
{   
    CollisionMapValue* collision;//[_MAPSIZE*_MAPSIZE/_GRAIN];
    
    char* path;
    unsigned int spriteIndex;
    unsigned int secondLayerSpriteIndex;


    Point spawnPoint;

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
void SetMapCollisionRect(int x, int y, int w, int h, bool objectIsHere);
void DisplayCollision();
bool PointIsFree(int x, int y, bool caresAboutUnits);
bool RectIsFree(int x, int y, int w, int h, bool caresAboutUnits);
