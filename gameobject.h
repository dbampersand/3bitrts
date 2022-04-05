#pragma once


#include "sprite.h"
#include "ability.h"
#include <allegro5/allegro.h>
#include "effect.h"
#include "rect.h"
#include "threat.h"
typedef enum GAMEOBJ_PROPERTIES
{
    OBJ_SELECTED = 1,
    OBJ_OWNED_BY = 2,
    OBJ_ACTIVE = 4,
    //OBJ_ATTACKING_TARGET = 8
} GAMEOBJ_PROPERTIES;
typedef struct Shield Shield;
typedef struct lua_State lua_State;
#define MAX_EFFECTS 6
typedef struct GameObject
{
    float x; float  y;
    //void (*update)(struct GameObject* self);
    char properties;
    float health;
    float maxHP;

    Shield* shields;
    int numShields;
    
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

    float aggroRadius;

    float    speed;

    float attackSpeed;
    float attackTimer;

    float range;

    float baseDamage;
    float armor;
     
    Effect effects[MAX_EFFECTS];

    int* onAttackEffectsIndices;
    int numAttackEffectIndices;

    Threat threatList;
    
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
void DrawGameObj(GameObject* g, bool forceInverse); 


void SetAttackingObj(GameObject* g, GameObject* target);
void AttackTarget(GameObject* g);
void NewObj(GameObject* g);

Rect GetObjRect(GameObject* g);
int GetPlayerOwnedBy(GameObject* g);

bool Damage(GameObject* g, float value);
void Heal(GameObject* g, float value);
void AddSpeed(GameObject* g, float value);
void ModifyMaxHP(GameObject* g, float value);

void Teleport(GameObject* g, float x, float y);
void GetOffsetCenter(GameObject* g, float* x, float* y);
void GetCentre(GameObject* g, float* x, float* y);
void KillObj(GameObject* g);
bool IsActive(GameObject* g);
void DoAI(GameObject* g);

float GetDist(GameObject* g1, GameObject* g2);
void CheckCollisions(GameObject* g, bool x, float dV);
GameObject* GetCollidedWith(GameObject* g);
GameObject* GetClicked(float x, float y);
void UnsetAll();
//void LoadFolderPrefabs(const char* dirPath);
//void LoadPrefabs(const char* dirPath);
