#include "allegro5/allegro.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"

#include <math.h>

#include "map.h"

#include "sprite.h"
#include "gameobject.h"
#include "luafuncs.h"
#include "helperfuncs.h"
#include "colors.h"
#include "player.h"
#include "replay.h"
#include "encounter.h"
#include "console.h"

 Map* currMap = NULL;
 Map* maps = NULL;
 int numMaps = 0;
void AddDirtSprites(int numToAdd, ALLEGRO_BITMAP* sprite)
{
    ALLEGRO_BITMAP* before = al_get_target_bitmap();

    al_set_target_bitmap(sprite);
    int w = al_get_bitmap_width(sprite);
    int h = al_get_bitmap_height(sprite);
    al_lock_bitmap(sprite,ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

    ClearSpriteDecorations();
    for (int i = 0; i < numToAdd; i++)
    {
        int numAttempts = 15;
        while (numAttempts-- > 0)
        {
            int x = RandRange(0,w);
            int y = RandRange(0,h);

            int selectedDirt = dirtSprites[RandRangeI(0,numDirtSprites)];
            bool put = true;
            for (int x2 = 0; x2 < GetWidthSprite(&sprites[selectedDirt]); x2++)
            {
                for (int y2 = 0; y2 < GetHeightSprite(&sprites[selectedDirt]); y2++)
                {
                    ALLEGRO_COLOR pixel = al_get_pixel(sprite, x+x2, y+y2);

                    if (pixel.a && pixel.r > bgThreshhold && pixel.g > bgThreshhold && pixel.b > bgThreshhold) {

                    }
                    else
                    {
                        put = false;
                    }
                }
            }

            if (put) {
                CreateSpriteDecorAtPosition(selectedDirt,x,y,AFTER_WORLD,COLOR_TEXTURED_GROUND);
                break;
            }
        }
    }
    al_unlock_bitmap(sprite);
    al_set_target_bitmap(before);
}
void DisplayCollision()
{
    if (GetMapWidth() == 0 || GetMapHeight() == 0 || !currMap->collision)
        return;
    for (int x = 0; x < GetMapWidth(); x += _GRAIN)
    {
        for (int y = 0; y < GetMapHeight(); y += _GRAIN)
        {
            int idx = GetIndex(currMap->collisionMapHeight, floorf(x / (float)_GRAIN), floorf(y / (float)_GRAIN));
            if (currMap->collision[idx])
            {
                ALLEGRO_COLOR c;
                if (currMap->collision[idx] == COLLISION_OPEN)
                    c = al_map_rgba(255,0,0,128);
                if (currMap->collision[idx] == COLLISION_WORLD_AND_OBJECT)
                    c = al_map_rgba(0,255,0,128);

                al_draw_filled_rectangle(ToScreenSpace_X(x),ToScreenSpace_Y(y),ToScreenSpace_X(x+_GRAIN),ToScreenSpace_Y(y+_GRAIN),c);
                
            }
        }
    }
}
bool RectIsFree(int x, int y, int w, int h, bool caresAboutUnits)
{
    for (int x2 = x; x2 < x + w; x2++)
    {
        if (!LineIsFree(x2,y,caresAboutUnits,h))
                return false;
        
    }
    return true;
}
bool LineIsFree(int x, int y, bool caresAboutUnits, int h)
{
    if (y + h + 1 >= (GetUIStartHeight())/(float)_GRAIN  || y < 0 || x < 0 || x >= GetMapWidth()/_GRAIN)
        return false;

    int startIndex = GetIndex(currMap->collisionMapHeight,x,y);
    

    for (int index = startIndex; index <= startIndex + h; index++)
    {
        CollisionMapValue m = currMap->collision[index];    
        if (caresAboutUnits)
        {
            if (m != COLLISION_OPEN)
            {
                return false;
            }
        }
        else
        {
            if (m != COLLISION_OPEN && m != COLLISION_WORLD_AND_OBJECT)
                return false;
        } 
    }
    return true;        
}
bool PointIsFree(int x, int y, bool caresAboutUnits)
{
    if (x < 0 | x >= GetMapWidth()/_GRAIN | y < 0 | y >= GetMapHeight()/_GRAIN)
        return false;

    int index = GetIndex(currMap->collisionMapHeight, x, y);

    if (caresAboutUnits)
        return (currMap->collision[index] == COLLISION_OPEN);
    else
        return (currMap->collision[index] == COLLISION_OPEN || currMap->collision[index] == COLLISION_WORLD_AND_OBJECT);
}
void SetMapCollisionRect(int x, int y, int w, int h, bool objectIsHere)
{
    //x+=_GRAIN*1.5f; y+=_GRAIN*1.5f;
   // w-=_GRAIN*2; h-=_GRAIN*2;

    //x-=1;
    x = (x / (int)_GRAIN) * _GRAIN;
    y = (y / (int)_GRAIN) * _GRAIN;

    //need to go up to the next _GRAIN level because
    //otherwise the collision rect might not cover the entire object
    w+=_GRAIN;
    h+=_GRAIN;


    int indexLeft = GetIndex(currMap->collisionMapHeight, floorf((x) / (float)_GRAIN), floorf((y) / (float)_GRAIN));
    int indexRight = GetIndex(currMap->collisionMapHeight, ceilf((x+w) / (float)_GRAIN), ceilf((y+h) / (float)_GRAIN));
    

    for (int xn = x/_GRAIN; xn < ceil((x + w)/_GRAIN); xn++)
    {
        for (int yn = y/_GRAIN; yn < ceil((y + h)/_GRAIN); yn++)
        {
            if (yn < 0 || yn >= currMap->collisionMapHeight)
                continue;
            if (xn < 0 || xn >= GetMapWidth()/_GRAIN)
                continue;
            
            int index = GetIndex(currMap->collisionMapHeight, ((xn)), ((yn)));

            if (objectIsHere)
            {
                if (currMap->collision[index] == COLLISION_OPEN || currMap->collision[index] == COLLISION_WORLD_AND_OBJECT)
                {
                    currMap->collision[index] = COLLISION_WORLD_AND_OBJECT;
                }
            }
            else
            {
                if (currMap->collision[index] == COLLISION_OPEN || currMap->collision[index] == COLLISION_WORLD_AND_OBJECT)
                {
                    currMap->collision[index] = COLLISION_OPEN;
                }
            }
        }
    }
}
void RefreshMapCollisonRects()
{
    
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        SetMapCollisionRect(g->position.worldX,g->position.worldY,GetWidth(g),GetHeight(g),true);
    }
}
void PreprocessMap(Map* map, int randSpritesToAdd)
{
    if (map->collision)
        free(map->collision);
    if (map->highlightMap)
        free(map->highlightMap);
    ALLEGRO_BITMAP* before = al_get_target_bitmap();

    ALLEGRO_BITMAP* sprite = sprites[map->spriteIndex].sprite;
    int w = al_get_bitmap_width(sprite);
    int h = al_get_bitmap_height(sprite);

    map->collision = calloc(w*h/_GRAIN,sizeof(CollisionMapValue));
    map->highlightMap = calloc(w*h,sizeof(bool));
    map->collisionMapHeight = h/(float)_GRAIN;

    
    //memset(&map->collision,0,w*h/_GRAIN);

    al_lock_bitmap(sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);

    al_set_target_bitmap(sprite);

   for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
          
        ALLEGRO_COLOR pixel = al_get_pixel(sprite, x, y);

        if (pixel.a && pixel.r > bgThreshhold && pixel.g > bgThreshhold && pixel.b > bgThreshhold) {
            int index = GetIndex(h/_GRAIN,x/_GRAIN,y/_GRAIN);
            map->collision[index] = true;
        }
        else
        {
            int index = GetIndex(h,x,y);
            map->highlightMap[index] = true;
        }
      }
   }

   
    Sprite* secondLayer = NewSprite(w,h);
    secondLayer-> path = calloc(1,sizeof(char));    

    map->secondLayerSpriteIndex = secondLayer - sprites;
   al_set_target_bitmap(secondLayer->sprite);
    al_lock_bitmap(secondLayer->sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        ALLEGRO_COLOR pixel = al_get_pixel(sprite, x, y);
        if (pixel.a && pixel.r <= bgThreshhold && pixel.g <= bgThreshhold && pixel.b <= bgThreshhold) {
            al_put_pixel(x,y,WHITE);
        }
        }
    }
   al_unlock_bitmap(secondLayer->sprite);
   al_unlock_bitmap(sprite);

   al_set_target_bitmap(before);


}

void loadLuaGameMap(lua_State* l, const char* filename, Map* m) 
{
    char* cpy = calloc(strlen(filename)+1,sizeof(char));
    strcpy(cpy,filename);
    m->lua_buffer.buffer = readFile(filename);
    


     if (luaL_loadfile(l, filename) || lua_pcall(l, 0, 0, 0))
     {
         ConsolePrintf("Map: Can't load lua file %s",lua_tostring(l,-1));
         return;
     }
     else
     {
        currGameObjRunning = NULL;
        int funcIndex;
        if (!m->lua_buffer.functions)
        {
            m->lua_buffer.functions = calloc(NUM_MAP_FUNCTIONS,sizeof(char*));
        }

        if (CheckFuncExists("update",&m->lua_buffer))
        {
            lua_getglobal(l, "update");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            m->luafunc_update = funcIndex;
        }
        if (CheckFuncExists("setup",&m->lua_buffer))
        {
            lua_getglobal(l, "setup");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            m->luafunc_setup = funcIndex;
        }
        if (CheckFuncExists("mapend",&m->lua_buffer))
        {
            lua_getglobal(l, "mapend");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            m->luafunc_mapend = funcIndex;
        }
        if (CheckFuncExists("objectdied",&m->lua_buffer))
        {
            lua_getglobal(l, "objectdied");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            m->luafunc_objectdied = funcIndex;
        }



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
        if (!m->name)
        {
            m->name = calloc(strlen(two)+1,sizeof(char));
            strcpy(m->name,two);
        }
        else if (strlen(m->name) == 0)
        {
            m->name = realloc(m->name,(strlen(two)+1)*sizeof(char));
            strcpy(m->name,two);

        }

     }
     free(cpy);
}
void DeleteMap(Map* m)
{
    if (m->path)
        free(m->path);
    if (m->highlightMap)
        free(m->highlightMap);

    if (m->name)   
        free(m->name);

    if (m->lua_buffer.buffer)
        free(m->lua_buffer.buffer);
    if (m->lua_buffer.functions)
        free(m->lua_buffer.functions);
    if (m->collision)
        free(m->collision);
    
    luaL_unref(luaState,LUA_REGISTRYINDEX,m->luafunc_update);
    luaL_unref(luaState,LUA_REGISTRYINDEX,m->luafunc_setup);
    luaL_unref(luaState,LUA_REGISTRYINDEX,m->luafunc_mapend);
    luaL_unref(luaState,LUA_REGISTRYINDEX,m->luafunc_objectdied);

    memset(m,0,sizeof(Map));
}

Map* LoadMap(char* path)
{
    Map m;

    memset(&m,0,sizeof(Map));
    m.automaticWinCheck = true;
    m.spawnPoint.x = 80;
    m.spawnPoint.y = 180;
    m.goldMultiplier = 1;
    loadLuaGameMap(luaState,path,&m);
    m.path = calloc(strlen(path)+1,sizeof(char));
    strcpy(m.path,path);

    int currMapIndex = currMap - maps;
    numMaps++;
    if (maps)
        maps = realloc(maps,numMaps*sizeof(Map));
    else
        maps = calloc(1,sizeof(Map));
    //update stale pointer
    if (currMapIndex  > 0)
        currMap = &maps[currMapIndex];

    int foundIndex = numMaps-1;
    bool foundMap = false;
    for (int i = 0; i < numMaps-1; i++)
    {
        if (maps[i].path && strcmp(path,maps[i].path)==0)
        {
            foundIndex = i;
            foundMap = true;
            DeleteMap(&maps[i]);
            numMaps--;
            break;
        }
    }


    maps[foundIndex] = m;
    return &maps[foundIndex];
}

void SetMap(Map* m)
{
     if (m->collision)
        free(m->collision);
    m->collision = NULL;

    if (currMap)
    {
        /*luaL_unref(luaState,LUA_REGISTRYINDEX,currMap->luafunc_update);
        luaL_unref(luaState,LUA_REGISTRYINDEX,currMap->luafunc_setup);
        luaL_unref(luaState,LUA_REGISTRYINDEX,currMap->luafunc_mapend);
        luaL_unref(luaState,LUA_REGISTRYINDEX,currMap->luafunc_objectdied);*/
    }
    currMap = m;
    CallLuaFunc(m->luafunc_setup);

    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g = activeObjects[i];
        CallLuaFunc(g->luafunc_onmapchange);

        for (int j = 0; j < INVENTORY_SLOTS; j++)
        {
            //Item* it = &g->inventory[j];
            //ItemOnMapChange(it,g);
        }
        
    }

       
    ClearSpriteDecorations();
    RandomSpriteDecor(16,dirtSprites,numDirtSprites);
    AddDirtSprites(16, sprites[currMap->spriteIndex].sprite);

    //PreprocessMap(m);

}
void InitMaps()
{
    numMaps = 0;
}
void DrawMap(Map* m, bool invert)
{
    ALLEGRO_COLOR c = GROUND;
    if (invert)
    {
        c = BG;
    }
    int x = -players[0].cameraPos.x;
    int y = -players[0].cameraPos.y;

    Sprite* s = &sprites[m->spriteIndex];
    Sprite* s2 = &sprites[m->secondLayerSpriteIndex];
    if (gameState == GAMESTATE_WATCHING_REPLAY)
    {
        s = &replay.sprites[m->spriteIndex];
        s2 = &replay.sprites[m->secondLayerSpriteIndex];
    }
    DrawSprite(s,x,y,0.5f,0.5f,0,c,invert,false,false);
    DrawSprite(s2,x,y,0.5f,0.5f,0,GROUND_DARK,false,false,false);

}
void UpdateMap(Map* m, float dt)
{
    if (m)
    {
        //CallLuaFunc(m->luafunc_update);
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,m->luafunc_update);
        lua_pushnumber(luaState,(double)dt);
        lua_pcall(luaState,1,0,0);

    }
}

//This is for drawing over the shadows
void RedrawMapSegmentUnderObj(GameObject* g)
{
    float percent = GetSummonPercent(g);  
    float realY = g->position.worldY + g->offset.y;

    int x = g->position.worldX + g->offset.x;
    int y = realY + (GetHeight(g) * (1-percent));
    int w = GetWidth(g);
    int h = (GetHeight(g) * percent);
    float sx = x; float sy = y;
    ToScreenSpace(&sx,&sy);

    DrawSpriteRegion(&sprites[currMap->spriteIndex],x,y,w,h,sx,sy,GROUND,false);
    DrawSpriteRegion(&sprites[currMap->secondLayerSpriteIndex],x,y,w,h,sx,sy,GROUND_DARK,false);
}
int GetMapWidth()
{
    #ifdef _REPLAY
    if (gameState == GAMESTATE_WATCHING_REPLAY)
    {
        if (replay.frames)
        {
            ReplayFrame* current = &replay.frames[replay.framePlayPosition];
            return GetWidthSprite(&replay.sprites[current->mapSpriteIndex]);
        }
    }
    #endif
    if (GetWidthSprite(&sprites[currMap->spriteIndex]) == 0)
        return 1;
    return GetWidthSprite(&sprites[currMap->spriteIndex]);
}
int GetMapHeight()
{
    #ifdef _REPLAY
    if (gameState == GAMESTATE_WATCHING_REPLAY)
    {
        ReplayFrame* current = &replay.frames[replay.framePlayPosition];
        return GetHeightSprite(&replay.sprites[current->mapSpriteIndex]);
    }
    #endif
    if (GetHeightSprite(&sprites[currMap->spriteIndex]) == 0)
        return 1;

    return GetHeightSprite(&sprites[currMap->spriteIndex]);
}
void AddCompletionPercent(float percent)
{
    currMap->percentComplete += percent;
    if (currMap->percentComplete > 100)
        currMap->percentComplete = 100;
    if (currMap->percentComplete < 0)
        currMap->percentComplete = 0;

}
void ChangeMap(const char* path)
{
    //if we're already in the shop
    if (gameState == GAMESTATE_IN_SHOP)
        return;
    if (gameState != GAMESTATE_IN_EDITOR)
    {
        if (currEncounterRunning->goingToShop)
            SetGameStateToInShop();
        else
            SetGameStateToChangingMap(path);
    }
    if (pathToNextMap)
        free(pathToNextMap);
    pathToNextMap = calloc(strlen(path)+1,sizeof(char));
    strcpy(pathToNextMap,path);
}
void AutoCompletionPercent()
{
    float numObjects = 0;
    for (int i = 0; i < numActiveObjects; i++)
    {
        GameObject* g2 = activeObjects[i];
        if (!ObjIsDecoration(g2) && GetPlayerOwnedBy(g2) > 0)
        {
            numObjects++;
        }
    }
    if (numObjects <= 0)
        numObjects = 1;
    if (currMap->numEnemyObjectsSpawned <= 0)
        currMap->numEnemyObjectsSpawned = 1;
    float p = 1 - (numObjects / currMap->numEnemyObjectsSpawned);
    float percent = 100 * (p);
    float difference = percent - currMap->percentComplete;
    AddCompletionPercent(difference);

}