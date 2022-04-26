#pragma once


#include "sprite.h"
#include "ability.h"
#include <allegro5/allegro.h>
#include "effect.h"
#include "rect.h"
#include "threat.h"
#include "command.h"
#include "point.h"
typedef enum GAMEOBJ_PROPERTIES
{
    OBJ_SELECTED = 1,
    OBJ_OWNED_BY = 2,
    OBJ_ACTIVE = 4,
    OBJ_CAN_PUSH = 8,
    OBJ_IS_CHANNELLING = 16
    //OBJ_ATTACKING_TARGET = 8
} GAMEOBJ_PROPERTIES;

//a unit can be multiple of these
typedef enum GAMEOBJ_TYPE_HINT
{
    TYPE_TANK = 1,
    TYPE_HEALER = 2,
    TYPE_MELEEDPS = 4,
    TYPE_RANGEDDPS = 8,
    TYPE_UTILITY = 16,
    TYPE_ALL = 256

} GAMEOBJ_TYPE_HINT;

typedef enum OBJ_FRIENDLINESS
{
    TYPE_ENEMY=0,
    TYPE_FRIENDLY=1
}OBJ_FRIENDLINESS;

typedef struct Shield Shield;
typedef struct lua_State lua_State;
#define MAX_EFFECTS 6
#define MAX_QUEUED_CMD 8

typedef struct GameObject
{    
    char* lua_buffer;

    char* name; 
    char* path; 
    GameObject* targObj;
    int* onAttackEffectsIndices;
    Shield* shields;
    Ability abilities[4]; 
    Effect effects[MAX_EFFECTS];
    Command queue[MAX_QUEUED_CMD];

    Point position;
    //float x; float  y;
    float health;
    float maxHP;

    int numShields;
    
    float mana;
    float maxMana;
    unsigned int spriteIndex;
    unsigned int selectedSpriteIndex;

    int luafunc_update;
    int luafunc_setup;
    int luafunc_kill;
    int luafunc_onattack;
    int numAttackEffectIndices;


    float aggroRadius;

    float speed;

    float attackSpeed;
    float attackTimer;

    float range;

    float baseDamage;
    float armor;
     


    Threat threatList;


    char properties;
    unsigned char xtarg;
    unsigned char ytarg;

    float invulnerableTime;

    GAMEOBJ_TYPE_HINT objType;

    float channellingTime;
    float channellingTotal;  
    float channelled_x;
    float channelled_y;
    GameObject* channelled_target;
    float target_heading_x;
    float target_heading_y;


    Ability* channelledAbility;
    
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

GameObject* AddGameobject(GameObject* prefab, float x, float y);
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
void AddDamage(GameObject* g, float value);
void ModifyMaxHP(GameObject* g, float value);

void Teleport(GameObject* g, float x, float y);
void GetOffsetCenter(GameObject* g, float* x, float* y);
float GetDistCentre(GameObject* g1, GameObject* g2);
void GetCentre(GameObject* g, float* x, float* y);
void KillObj(GameObject* g);
bool IsActive(GameObject* g);
void DoAI(GameObject* g);

float GetDist(GameObject* g1, GameObject* g2);
float RectDist(GameObject* g1, GameObject* g2);
void CheckCollisionsWorld(GameObject* g, bool x, float dV); 
void CheckCollisions(GameObject* g, bool x, float dV, bool objectCanPush);
GameObject* GetCollidedWith(GameObject* g);
GameObject* GetClicked(float x, float y);
void UnsetAll();
void MakeInvulnerable(GameObject* g, float time);
float GetWidth(GameObject* g);
float GetHeight(GameObject* g);

bool ObjectCanPush(GameObject* g);
bool IsInCombat(GameObject* g);
bool ObjHasType(GameObject* g, GAMEOBJ_TYPE_HINT typeHint);
bool ObjIsChannelling(GameObject* g);
void SetObjChannelling(GameObject* g, Ability* a, float time,float x, float y, GameObject* target, float heading_x, float heading_y);
void UpdateChannellingdObj(GameObject* g, float dt);
void DrawChannelHint(GameObject* g);

//void LoadFolderPrefabs(const char* dirPath);
//void LoadPrefabs(const char* dirPath);
