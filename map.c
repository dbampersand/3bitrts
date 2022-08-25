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
void DisplayCollision()
{
    for (int x = 0; x < GetMapWidth(); x += _GRAIN)
    {
        for (int y = 0; y < GetMapHeight(); y += _GRAIN)
        {
            int idx = GetIndex(GetMapHeight()/(float)_GRAIN, floor(x / (float)_GRAIN), floor(y / (float)_GRAIN));
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
bool RectIsFree(int x, int y, int w, int h)
{
    x-=1; y-=1; w += 2; h += 2;
    for (int x2 = x; x2 < x + w; x2++)
    {
        for (int y2 = y; y2 < y + h; y2++)
        {
            if (!PointIsFree(x2,y2))
                return false;
        }
        
    }
    return true;
}
bool PointIsFree(int x, int y)
{
    int index = GetIndex(GetMapHeight()/_GRAIN, x, y);

    if (x < 0 || x >= GetMapWidth())
        return false;
    if (y < 0 || y >= GetMapHeight())
        return false;

    return (currMap->collision[index] == COLLISION_OPEN);
}
void SetMapCollisionRect(int x, int y, int w, int h, bool objectIsHere)
{
    x-=_GRAIN;y-=_GRAIN;
    w+=_GRAIN*3; h+=_GRAIN*3;

    int indexLeft = GetIndex(GetMapHeight()/_GRAIN, floor((x) / (float)_GRAIN), floor((y) / (float)_GRAIN));
    int indexRight = GetIndex(GetMapHeight()/_GRAIN, ceil((x+w) / (float)_GRAIN), ceil((y+h) / (float)_GRAIN));
    for (int xn = x/_GRAIN; xn < ceil((x + w)/_GRAIN); xn++)
    {
        for (int yn= y/_GRAIN; yn < ceil((y + h)/_GRAIN); yn++)
        {
            if (yn < 0 || yn >= GetMapHeight()/_GRAIN)
                continue;
            if (xn < 0 || xn >= GetMapWidth()/_GRAIN)
                continue;
            
            int index = GetIndex(GetMapHeight()/_GRAIN, ceil((xn)), ceil((yn)));

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
void PreprocessMap(Map* map)
{
        if (map->collision)
        free(map->collision);
    ALLEGRO_BITMAP* before = al_get_target_bitmap();

    ALLEGRO_BITMAP* sprite = sprites[map->spriteIndex].sprite;
    int w = al_get_bitmap_width(sprite);
    int h = al_get_bitmap_height(sprite);

    map->collision = calloc(w*h/_GRAIN,sizeof(CollisionMapValue));
    
    //memset(&map->collision,0,w*h/_GRAIN);

    al_lock_bitmap(sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);

    al_set_target_bitmap(sprite);
    float bgThreshhold = 0.6f;

   for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
          
        ALLEGRO_COLOR pixel = al_get_pixel(sprite, x, y);

        if (pixel.a && pixel.r > bgThreshhold && pixel.g > bgThreshhold && pixel.b > bgThreshhold) {
            int index = GetIndex(h/_GRAIN,x/_GRAIN,y/_GRAIN);
            map->collision[index] = true;
        }
      }
   }

    Sprite* secondLayer = NewSprite(w,h);
    secondLayer-> path = calloc(1,sizeof(char));    
    map->secondLayerSpriteIndex = secondLayer - sprites;
   al_set_target_bitmap(secondLayer->sprite);
   al_lock_bitmap(secondLayer->sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        ALLEGRO_COLOR pixel = al_get_pixel(sprite, x, y);
        if (pixel.a && pixel.r <= bgThreshhold && pixel.g <= bgThreshhold && pixel.b <= bgThreshhold) {
            al_draw_pixel(x,y,WHITE);
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
    m->lua_buffer = readFile(filename);



     if (luaL_loadfile(l, filename) || lua_pcall(l, 0, 0, 0))
     {
         printf("Can't load lua file %s",lua_tostring(l,-1));
     }
     else
     {
        currGameObjRunning = NULL;
        int funcIndex;
        if (CheckFuncExists("update",m->lua_buffer))
        {
            lua_getglobal(l, "update");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            m->luafunc_update = funcIndex;
        }
        if (CheckFuncExists("setup",m->lua_buffer))
        {
            lua_getglobal(l, "setup");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            m->luafunc_setup = funcIndex;
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

Map* LoadMap(char* path)
{
  Map m;
  memset(&m,0,sizeof(Map));
  m.spawnPoint.x = 80;
  m.spawnPoint.y = 180;
  loadLuaGameMap(luaState,path,&m);
  
  numMaps++;
  if (maps)
    maps = realloc(maps,numMaps*sizeof(Map));
  else
    maps = calloc(1,sizeof(Map));
  
  maps[numMaps-1] = m;
  return &maps[numMaps-1];
}

void SetMap(Map* m)
{
    if (m->collision)
        free(m->collision);
    m->collision = NULL;
    currMap = m;
    CallLuaFunc(m->luafunc_setup);
    //PreprocessMap(m);

}
void InitMaps()
{
    numMaps = 0;
}
void DrawMap()
{
    int x = -players[0].cameraPos.x;
    int y = -players[0].cameraPos.y;

    DrawSprite(&sprites[currMap->spriteIndex],x,y,0.5f,0.5f,0,GROUND,false);
    DrawSprite(&sprites[currMap->secondLayerSpriteIndex],x,y,0.5f,0.5f,0,GROUND_DARK,false);

}
void UpdateMap(Map* m, float dt)
{
    if (m)
    {
        //CallLuaFunc(m->luafunc_update);
        lua_rawgeti(luaState,LUA_REGISTRYINDEX,m->luafunc_update);
        lua_pushnumber(luaState,dt);
        lua_pcall(luaState,1,0,0);

    }
}

//This is for drawing over the shadows
void RedrawMapSegmentUnderObj(GameObject* g)
{
    float percent = GetSummonPercent(g);  
    float realY = g->position.y + g->offset.y;

    int x = g->position.x + g->offset.x;
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
    return GetWidthSprite(&sprites[currMap->spriteIndex]);
}
int GetMapHeight()
{
    return GetHeightSprite(&sprites[currMap->spriteIndex]);
}