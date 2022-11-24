#pragma once

#include <stdbool.h>
#include "point.h"
#include "luafuncs.h"

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
    bool* highlightMap;

    Point spawnPoint;

    int luafunc_update;
    int luafunc_setup;
    int luafunc_mapend;

    LuaBuffer lua_buffer;
    char* name;

    float percentComplete;
    float percentCompleteShowing;

    bool automaticWinCheck;
    bool forceNextLevelButtonEnabled;

} Map;

#define NUM_MAP_FUNCTIONS 3
#define bgThreshhold 0.6f

extern Map* currMap;
extern Map* maps;
extern int numMaps;

void PreprocessMap(Map* map, int randSpritesToAdd);
Map* LoadMap(char* path);
void SetMap(Map* m);
void InitMaps();
void DrawMap(Map* m, bool invert);
void UpdateMap(Map* m, float dt);
void RedrawMapSegmentUnderObj(GameObject* g);
int GetMapWidth();
int GetMapHeight();
void SetMapCollisionRect(int x, int y, int w, int h, bool objectIsHere);
void DisplayCollision();
bool PointIsFree(int x, int y, bool caresAboutUnits);
bool RectIsFree(int x, int y, int w, int h, bool caresAboutUnits);
void AddCompletionPercent(float percent);
void ChangeMap(const char* path);
void AddDirtSprites(int numToAdd, ALLEGRO_BITMAP* sprite);
