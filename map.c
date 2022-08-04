#include "allegro5/allegro.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"

#include "map.h"

#include "sprite.h"
#include "gameobject.h"
#include "luafuncs.h"
#include "helperfuncs.h"
#include "colors.h"

void PreprocessMap(Map* map)
{
    memset(&map->collision,0,_MAPSIZE*_MAPSIZE/_GRAIN);
    int w; int h; 
    ALLEGRO_BITMAP* sprite = sprites[map->spriteIndex].sprite;
    w = al_get_bitmap_width(sprite);
    h = al_get_bitmap_height(sprite);

    al_lock_bitmap(sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);

    al_set_target_bitmap(sprite);
    float bgThreshhold = 0.6f;

   for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
          
        ALLEGRO_COLOR pixel = al_get_pixel(sprite, x, y);

        if (pixel.a && pixel.r > bgThreshhold && pixel.g > bgThreshhold && pixel.b > bgThreshhold) {
            int index = GetIndex(_MAPSIZE/_GRAIN,x/_GRAIN,y/_GRAIN);
            map->collision[index] = true;
        }
      }
   }
    Sprite* secondLayer = NewSprite(w,h);
    secondLayer-> path = sprites[map->spriteIndex].path;
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
    currMap = m;
    CallLuaFunc(m->luafunc_setup);
    PreprocessMap(m);

}
void InitMaps()
{
    numMaps = 0;
}
void DrawMap()
{
    DrawSprite(&sprites[currMap->spriteIndex],0,0,0.5f,0.5f,0,GROUND,false);
    DrawSprite(&sprites[currMap->secondLayerSpriteIndex],0,0,0.5f,0.5f,0,GROUND_DARK,false);

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
    int x = g->position.x;
    int y = g->position.y;
    int w = GetWidth(g);
    int h = GetHeight(g);


    DrawSpriteRegion(&sprites[currMap->spriteIndex],x,y,w,h,x,y,GROUND,false);
    DrawSpriteRegion(&sprites[currMap->secondLayerSpriteIndex],x,y,w,h,x,y,GROUND_DARK,false);

}
