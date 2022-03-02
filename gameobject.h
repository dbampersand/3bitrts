#pragma once


#include "sprite.h"
#include "ability.h"
#include <allegro5/allegro.h>

typedef enum GAMEOBJ_PROPERTIES
{
    OBJ_SELECTED = 1,
    OBJ_OWNED_BY = 2,
    OBJ_ACTIVE = 4,
    //OBJ_ATTACKING_TARGET = 8
} GAMEOBJ_PROPERTIES;
typedef struct lua_State lua_State;

typedef struct GameObject
{
    float x; float  y;
    //void (*update)(struct GameObject* self);
    char properties;
    float health;
    float maxHP;
    float mana;
    float maxMana;
    Ability abilities[4];
    unsigned int spriteIndex;
    unsigned int selectedSpriteIndex;

    int luafunc_update;
    int luafunc_setup;
    int luafunc_kill;
    int luafunc_onattack;

    char* lua_buffer;

    char* name; 
    char* path; 

    unsigned char xtarg;
    unsigned char ytarg;
    GameObject* targObj;

    char ownedby;

    unsigned char speed;

    float attackSpeed;
    float attackTimer;

    float range;

    float baseDamage;

} GameObject;

#define MAX_OBJS 64
GameObject* objects;
int numObjects;
int objectsAllocated;

GameObject** freeObjs;
int numFreeObjs;


GameObject* prefabs;
int numPrefabs;
int numPrefabsAllocated;


//add 8 more free objects when we need to realloc objects array
#define BUFFER_PREALLOC_AMT 8

GameObject* AddGameobject(GameObject* prefab);
bool CheckFuncExists(const char* funcName, char* lua_buffer);
void loadLuaGameObj(lua_State* l,const char* filename, GameObject* g);
GameObject* LoadPrefab(const char* path);
void LoadFolderPrefabs(const char* dirPath);
bool IsSelected(GameObject* g);
void SetSelected(GameObject* g, bool select);

bool IsOwnedByPlayer(GameObject* g);
void SetOwnedBy(GameObject* g, bool i);
void Move(GameObject* g, float delta);

void DrawHealthBar(GameObject* g, ALLEGRO_COLOR col);
void DrawGameObj(GameObject* g);


void SetAttackingObj(GameObject* g, GameObject* target);
void Attack(GameObject* g);
void NewObj(GameObject* g);

//void LoadFolderPrefabs(const char* dirPath);
//void LoadPrefabs(const char* dirPath);
