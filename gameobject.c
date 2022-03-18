#include "gameobject.h"
#include <dirent.h>
#include "luafuncs.h"
#include <ctype.h>
#include <math.h>
#include "rect.h"
#include "map.h"
#include "helperfuncs.h"
#include "colors.h"
#include "video.h"
#include <allegro5/allegro_primitives.h>
#include "animationeffect.h"
#include <stdlib.h>
#include "video.h"
GameObject* AddGameobject(GameObject* prefab)
{
    GameObject* found = NULL;

    for (int i = 0; i < numFreeObjs; i++)
    {
        if (freeObjs[i])
        {       
            found = freeObjs[i];
            for (int j = i; j < numFreeObjs-1; j++)
            {
                freeObjs[j] = freeObjs[j+1];
            }
            freeObjs[numFreeObjs-1] = NULL;
            break;
        }
    }
    if (!found)
        return NULL;


    if (numObjects >= objectsAllocated)
    {
        // return NULL;
        //objectsAllocated += BUFFER_PREALLOC_AMT;
        //objects = realloc(objects,objectsAllocated*sizeof(GameObject));
    }
    //objects[numObjects] = *prefab; 
    //objects[numObjects].properties |= OBJ_ACTIVE;
    *found = *prefab;
    found->properties |= OBJ_ACTIVE;
    found->health = 100;
    found->maxHP = 100;
    found->baseDamage = 5;
    found->range = 1;
    found->attackSpeed = 1;
    found->mana = 50;
    found->maxMana = 100;

    
    numObjects++;



    return found;
}
void NewObj(GameObject* g)
{

}

bool CheckFuncExists(const char* funcName, char* lua_buffer)
{
    char* c = strstr(lua_buffer,funcName);
    char* full = c + strlen(funcName);

    if (c == NULL) return false;
    while (c < lua_buffer + strlen(lua_buffer))
    {
        while (c > lua_buffer)
        {
            //if we've hit a newline
            if (iscntrl(*c) || *c == '\n')
            {
                return true;
            }
            //if its a comment
            if (*c == '-')
            {
                c = strstr(full,funcName);
                full = c + strlen(funcName);

                if (c == NULL)
                    return false;

                continue;
            }
            //if it's in a string
            if (*c == '"' || *c ==  '\'')
            {
                c = strstr(full,funcName);
                full = c + strlen(funcName);

                if (c == NULL)
                    return false;
                continue;
            }
            c++;
        }
    }
    return false;
}
void loadLuaGameObj(lua_State* l, const char* filename, GameObject* g) 
{
    char* cpy;
    if (g)
    {
        if (g->lua_buffer)
        {

        }
        else
        {
            memset(g,0,sizeof(GameObject));    
            cpy = calloc(strlen(filename)+1,sizeof(char));
            strcpy(cpy,filename);
            LoadLuaFile(filename,g);

        }
    }
    else
    {

    }

     if (luaL_loadbuffer(l, g->lua_buffer,strlen(g->lua_buffer),NULL) || lua_pcall(l, 0, 0, 0))
     {
         printf("Can't load lua file %s",lua_tostring(l,-1));
     }
     else
     {
        currGameObjRunning=g;
        int funcIndex;
        if (CheckFuncExists("update",g->lua_buffer))
        {
            lua_getglobal(l, "update");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_update = funcIndex;
        }
        else
            g->luafunc_update = -1;

        if (CheckFuncExists("setup",g->lua_buffer))
        {
            lua_getglobal(l, "setup");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            lua_rawgeti(l,LUA_REGISTRYINDEX,funcIndex);
            lua_pcall(l,0,0,0);
            g->luafunc_setup = funcIndex;
        }
        else
            g->luafunc_setup = -1;


        if (CheckFuncExists("kill",g->lua_buffer))
        {
            lua_getglobal(l, "kill");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_kill = funcIndex;
        }
        else
            g->luafunc_kill = -1;

        if (CheckFuncExists("OnAttack",g->lua_buffer))
        {
            lua_getglobal(l, "OnAttack");
            funcIndex = luaL_ref(l, LUA_REGISTRYINDEX);
            g->luafunc_onattack = funcIndex;
        }
        else
            g->luafunc_onattack = -1;


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
        if (!g->name)
        {
            g->name = calloc(strlen(two)+1,sizeof(char));
            strcpy(g->name,two);
        }
        else if (strlen(g->name) == 0)
        {
            g->name = realloc(g->name,(strlen(two)+1)*sizeof(char));
            strcpy(g->name,two);

        }
        g->path = calloc(strlen(filename)+1,sizeof(char));
        strcpy(g->path,filename);
        //SET_BIT(&g->properties,0);

     }
     free(cpy);
}

void LoadFolderPrefabs(const char* dirPath)
{
    DIR *d;

    struct dirent *dir;
    d = opendir(dirPath);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG &&  strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                char* tok = strtok(dir->d_name,".");
                tok = strtok(NULL,".");
                if (tok)
                {
                    if (strcmp(tok,"lua") == 0)
                    {
                        char* dirConcat = calloc(strlen(dirPath)+strlen("/")+strlen(".lua")+strlen(dir->d_name)+1,sizeof(char));
                        strcpy(dirConcat,dirPath);
                        strcat(dirConcat,"/");
                        strcat(dirConcat,dir->d_name);
                        strcat(dirConcat,".lua");
                        GameObject g;
                        loadLuaGameObj(luaState,dirConcat,&g);
                        
                        free(dirConcat);
                    }
            }
            }
        }
    }
    


}
GameObject* LoadPrefab(const char* path)
{
    for (int i = 0; i < numPrefabs; i++)
    {
        if (strcmp(prefabs[i].path,path)==0)
        {
            return &prefabs[i];
        }
    }
    GameObject g;
    memset(&g,0,sizeof(GameObject));
    loadLuaGameObj(luaState,path,&g);

    if (numPrefabs  >= numPrefabsAllocated)
    {
        numPrefabsAllocated += BUFFER_PREALLOC_AMT;
        prefabs = realloc(prefabs,(numPrefabsAllocated)*sizeof(GameObject));
    }
    prefabs[numPrefabs] = g;
    numPrefabs++;

    return &prefabs[numPrefabs-1];
}
void LoadPrefabs(const char* dirPath)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirPath);
    
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                char* dirConcat = calloc(strlen(dirPath)+strlen(dir->d_name)+3,sizeof(char));
                strcpy(dirConcat,dirPath);
                strcat(dirConcat,"/");
                strcat(dirConcat,dir->d_name);
                strcat(dirConcat,"/");
                LoadFolderPrefabs(dirConcat);
                LoadPrefabs(dirConcat);
                free(dirConcat);
            }
        }
    }
}
bool IsSelected(GameObject* g)
{
    return g->properties & OBJ_SELECTED;
}
void SetSelected(GameObject* g, bool select)
{
    if (select)
    {
        g->properties |= OBJ_SELECTED;
    }
    else
    {
        g->properties &= ~OBJ_SELECTED;
    }
}
bool IsOwnedByPlayer(GameObject* g)
{
    return (g->properties & OBJ_OWNED_BY) == 0;
}
int GetPlayerOwnedBy(GameObject* g)
{
    return (g->properties & OBJ_OWNED_BY) > 0 ? 1 : 0;
}
void SetOwnedBy(GameObject* g, bool i)
{
    if (i == false)
    {
        g->properties &= ~OBJ_OWNED_BY;
    }
    else
    {
        g->properties |= OBJ_OWNED_BY;
    }
}
void CheckCollisions(GameObject* g, bool x, float dV)
{
    Sprite* s = &sprites[g->spriteIndex];
    Rect rG = (Rect){g->x,g->y,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)};
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g2 = &objects[i];
        if (g2 == g || !IsActive(g2)) 
            continue;
        Sprite* s2 = &sprites[g2->spriteIndex];
        Rect r2 = (Rect){g2->x,g2->y,al_get_bitmap_width(s2->sprite),al_get_bitmap_height(s2->sprite)};

        if (!CheckIntersect(rG,r2))
        {
            continue;
        }

        if (x)
        {
            if (dV > 0)
            {
                if (1)
                {
                    g->x = g2->x - al_get_bitmap_width(s->sprite);//xresult - al_get_bitmap_width(s->sprite);
                    //g->x = xresult - w - al_get_bitmap_width(s->sprite);

                }
            }
            else if (dV <= 0)
            {
                if (1)
                {
                    g->x = g2->x+al_get_bitmap_width(s2->sprite); 
                }
            }
        }
        else
        {
            if (1)
            {
                if (dV > 0)
                    g->y = g2->y - al_get_bitmap_width(s->sprite);
                else
                    g->y = g2->y + al_get_bitmap_width(s2->sprite);

            }
        }
    }
}
void CheckCollisionsWorld(GameObject* g, bool x, float dV)
{
    if (dV == 0) return;
    

    int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);

    float posX = g->x;
    float posY = g->y;

    if (posX < 0 || posX > _MAPSIZE)
        return;
    if (posY < 0 || posY > _MAPSIZE)
        return;

    
    int indexTop = GetIndex(_MAPSIZE/_GRAIN, floor(posX / (float)_GRAIN), floor(posY / (float)_GRAIN));
    int indexRight = GetIndex(_MAPSIZE/_GRAIN, floor((posX+w) / (float)_GRAIN), floor((posY+h/2) / (float)_GRAIN));
    int indexBottom = GetIndex(_MAPSIZE/_GRAIN, floor((posX+w/2) / (float)_GRAIN), floor((posY+h) / (float)_GRAIN));
    int indexLeft = GetIndex(_MAPSIZE/_GRAIN, floor((posX) / (float)_GRAIN), floor((posY+h/2) / (float)_GRAIN));

    if (x)
    {
        if (dV < 0)
        {
            if (currMap->collision[indexLeft] == false)
            {
                g->x = ((indexLeft/(_MAPSIZE/_GRAIN))*_GRAIN)+_GRAIN;
            }
        }
        if (dV > 0)
        {
            if (currMap->collision[indexRight] == false)
            {
                g->x = (indexRight/(_MAPSIZE/_GRAIN))*_GRAIN - w;
            }
        }

    }
    else
    {
        if (dV < 0)
        {
            if (currMap->collision[indexTop] == false)
            {
                int yCoord = (indexTop%(_MAPSIZE/_GRAIN)+1)*_GRAIN;
                g->y = yCoord;
            }

        }
        if (dV > 0)
        {
            if (currMap->collision[indexBottom] == false)
            {
                int yCoord = (indexBottom%(_MAPSIZE/_GRAIN))*_GRAIN - h;
                g->y = yCoord;
            }
        }
    }
}
void Move(GameObject* g, float delta)
{
    #define DIST_DELTA 1

    int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
    int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);


    int xtarg = g->xtarg;
    int ytarg = g->ytarg;

        float moveX = xtarg - g->x;
        float moveY = ytarg - g->y;

        float dist = sqrt(moveX*moveX+moveY*moveY);

        if (dist <= DIST_DELTA)
        {
            g->x = xtarg;
            g->y = ytarg;
            return;
        }
        float dX = (moveX / dist * g->speed) * delta;
        float dY = (moveY / dist * g->speed) * delta;

        g->x += dX;
        CheckCollisions(g,true, dX);
        CheckCollisionsWorld(g,true, dX);

        g->y += dY;
        CheckCollisions(g,false, dY);
        CheckCollisionsWorld(g,false, dY);


}
void DrawHealthBar(GameObject* g, ALLEGRO_COLOR col)
{
    Sprite* s = &sprites[g->spriteIndex];
    int padding = 4;

    Rect r;
    r.x = g->x;
    r.y = g->y - padding;
    r.w = al_get_bitmap_width(s->sprite);
    r.h = 2;

    al_draw_rectangle(r.x,r.y,r.x+r.w,r.y+r.h,col,1);

    float percent = g->health / (float)g->maxHP;
    int numPixels = percent * r.w;
    al_draw_filled_rectangle(r.x,r.y,r.x+numPixels,r.y+r.h,col);
    
}
void DrawGameObj(GameObject* g, bool forceInverse)
{
    if (!(g->properties & OBJ_ACTIVE))
        return;
    bool b = IsOwnedByPlayer(g);
    ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
    Sprite* s = &sprites[g->spriteIndex];
    //DrawSprite(&sprites[objects[i].spriteIndex],objects[i].x,objects[i].y,(ALLEGRO_COLOR){94/255.0f,98/255.0f,134/255.0f,255});
    DrawSprite(s,g->x,g->y,c, IsSelected(g) || forceInverse);
   
    Rect selectRect;
    selectRect.w = al_get_bitmap_width(s->sprite);
    selectRect.h = al_get_bitmap_height(s->sprite);
    selectRect.x = g->x;
    selectRect.y = g->y;

    DrawRoundedRect(selectRect, c);
    DrawHealthBar(g,c);
    
}
void SetAttackingObj(GameObject* g, GameObject* target)
{
    g->targObj = target;
}
void AttackTarget(GameObject* g)
{
    if (g->targObj)
    {
        if (g->luafunc_onattack >= 0)
        {
            CallLuaFunc(g->luafunc_onattack);
        }
        int damage = g->baseDamage;
       //    g->targObj->health -= damage   ;

        if (g->numAttackEffectIndices > 0)
        {

            int randomAtk = g->onAttackEffectsIndices[rand() % g->numAttackEffectIndices];
            AnimationEffect* ae =  &animationEffectsPrefabs[randomAtk];

            
            Rect r1 = GetObjRect(g);
            Rect r2 = GetObjRect(g->targObj);
            
            int midX = (r1.x+r1.w/2 + r2.x+r2.w/2) / 2;
            int midY = (r1.y+r1.h/2 + r2.y+r2.h/2) / 2;

            midX -= ae->rect.w/2;
            midY -= ae->rect.h/2;



            AddAnimationEffect_Prefab(ae, g->properties & OBJ_OWNED_BY,midX,midY);
        }


        AddThreat(g,g->targObj,damage);
        if (Damage(g->targObj,damage))
        {
            g->targObj = NULL;
        }

    }
}
Rect GetObjRect(GameObject* g)
{
    if (g->spriteIndex <= 0) return (Rect){g->x,g->y,0,0};
    Rect r = (Rect){g->x,g->y,al_get_bitmap_width(sprites[g->spriteIndex].sprite),al_get_bitmap_height(sprites[g->spriteIndex].sprite)};
    return r;
}
bool Damage(GameObject* g, float value)
{
    g->health -= value;
    if (g->health <= 0)
    {
        KillObj(g);
        return true;
    }
    return false;
}
void Heal(GameObject* g, float value)
{
    g->health += value;
}

void ModifyMaxHP(GameObject* g, float value)
{
    g->maxHP += value;
}
void Teleport(GameObject* g, float x, float y)
{
    float cX; float cY;
    GetOffsetCenter(g,&cX,&cY);
    g->x = x - cX/2;
    g->y = y - cY/2 ;
    g->xtarg = g->x;
    g->ytarg = g->y;

}
void GetOffsetCenter(GameObject* g, float* x, float* y)
{
    if (g->spriteIndex > 0)
    {
        *x = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
        *y = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
        return;
    }
    *x = 0; 
    *y = 0;
}
void DoAI(GameObject* g)
{
    if (g->threatList.obj)
    {
        Threat* t = GetHighestThreat(&g->threatList);
        g->targObj = t->obj;
    }
    //do one of these per frame, just to save some cycles
    if (_FRAMES % MAX_OBJS  == g-objects)
    {
        for (int i = 0; i < MAX_OBJS; i++)
        {
            if (objects[i].properties & OBJ_ACTIVE)
            {
                if ((g->properties & OBJ_OWNED_BY) != (objects[i].properties & OBJ_OWNED_BY))
                {
                    if (GetDist(g,&objects[i]) < g->aggroRadius)
                    {
                        //also bring in the others
                        for (int j = 0; j < MAX_OBJS; j++)
                        {
                            if ((g->properties & OBJ_OWNED_BY) == (objects[j].properties & OBJ_OWNED_BY))
                                AddThreat(&objects[i],&objects[j],0);
                        }
                        AddThreat(&objects[i],g, 0);
                    }

                }
            }

        }

    }
}
bool IsActive(GameObject* g)
{
    return (g->properties & OBJ_ACTIVE);
}

float GetDist(GameObject* g1, GameObject* g2)
{
    Rect r1 = GetObjRect(g1);
    Rect r2 = GetObjRect(g2);

    Rect unioned = UnionRect(r1,r2);
    unioned.w -= r1.w + r2.w;
    unioned.h -= r1.h + r2.h;

    return unioned.w+unioned.h;
}