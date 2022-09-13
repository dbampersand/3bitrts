#pragma once

#include "sprite.h"
#include "ability.h"
#include "allegro5/allegro.h"
#include "effect.h"
#include "rect.h"   
#include "threat.h"
#include "command.h"
#include "point.h"
#include "pathfind.h"

#define MAX_PATHFINDING_NODES_HELD 20
#define MINIMUM_RANGE 6

typedef enum GAMEOBJ_PROPERTIES
{
    OBJ_SELECTED = 1,
    OBJ_OWNED_BY = 2,
    OBJ_ACTIVE = 4,
    OBJ_CAN_PUSH = 8,
    OBJ_IS_CHANNELLING = 16,
    OBJ_IS_DECORATION = 32,
    OBJ_IS_INVINCIBLE = 64
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
    TYPE_ALL = 255

} GAMEOBJ_TYPE_HINT;

typedef enum OBJ_FRIENDLINESS
{
    TYPE_FRIENDLY=0,
    TYPE_ENEMY=1,
    TYPE_DECORATION = 2
}OBJ_FRIENDLINESS;

typedef struct Shield Shield;
typedef struct lua_State lua_State;
typedef struct Player Player;
#define MAX_EFFECTS 32
#define MAX_QUEUED_CMD 8
#define MAX_ABILITIES 8
#define FLASH_TIMER 0.1
#define HEALTHBAR_HEIGHT 2
typedef struct GameObject
{    
    struct GameObject* prefab;
    char* lua_buffer;

    char* name; 
    char* path; 
    GameObject* targObj;
    int* onAttackEffectsIndices;
    Shield* shields;
    Ability abilities[MAX_ABILITIES]; 
    Effect effects[MAX_EFFECTS];
    Command queue[MAX_QUEUED_CMD];

    int numAbilities;


    Point position;
    Point offset; //offset for drawing the 'attack animation'
    float angle;

    float health;
    float maxHP;
    float healthRegen;

    int numShields;

    bool usesMana;
    float mana;
    float manaRegen;
    float maxMana;

    unsigned int spriteIndex;
    unsigned int selectedSpriteIndex;
    unsigned int channelingSpriteIndex;

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
    float threatMultiplier; 

    char properties;
    Point targetPosition;

    float invulnerableTime;

    GAMEOBJ_TYPE_HINT objType;
    GAMEOBJ_TYPE_HINT category;


    float channellingTime;
    float channellingTotal;  
    float channelled_x;
    float channelled_y;
    GameObject* channelled_target;

    float target_heading_x;
    float target_heading_y;


    Ability* channelledAbility;
    bool playerChoosable;

    float summonTime;
    float summonMax;

    int* attackSoundIndices;
    int numAttackSounds;    

    float flashTimer;

    bool deathTimerActivated;
    float deathTimer;

    float stunTimer;

    PathfindNode pathNodes[MAX_PATHFINDING_NODES_HELD];
    int currentPathingNode;
    bool pathfindNeedsRefresh;
} GameObject;


#define MAX_OBJS 64
GameObject* objects;
int numObjects;
int objectsAllocated;

GameObject** freeObjs;
int numFreeObjs;


GameObject** prefabs;
int numPrefabs;
int numPrefabsAllocated;



//add 8 more free objects when we need to realloc objects array
#define BUFFER_PREALLOC_AMT 8
void SetTargetPosition(GameObject* g, float x, float y);
void ClearPathfindingQueue(GameObject* g);
GameObject* GetMousedOver(ALLEGRO_MOUSE_STATE* mouseState);
void UpdatePlayerObjectInteractions(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState);
void ProcessAttackMoveMouseCommand(ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_KEYBOARD_STATE* keyState);
void UpdateObject(GameObject* g, float dt);
void InitObjects();
void GetControlGroup(ALLEGRO_KEYBOARD_STATE* keyState);
void SetControlGroups(ALLEGRO_KEYBOARD_STATE* keyState);
int GetCtrlGroup(int index);
void SetCtrlGroup(int index, GameObject** list, int numUnitsSelected);
void CheckSelected(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseLastFrame, ALLEGRO_KEYBOARD_STATE* keyState);
GameObject* AddGameobject(GameObject* prefab, float x, float y);
bool CheckFuncExists(const char* funcName, char* lua_buffer);
void loadLuaGameObj(lua_State* l,const char* filename, GameObject* g);
GameObject* LoadPrefab(const char* path);
bool IsSelected(GameObject* g);
void SetSelected(GameObject* g, bool select);

bool IsOwnedByPlayer(GameObject* g);
void SetOwnedBy(GameObject* g, int i);
void Move(GameObject* g, float delta);

void DrawHealthBar(GameObject* g, ALLEGRO_COLOR col);
void DrawGameObj(GameObject* g, bool forceInverse); 


void SetAttackingObj(GameObject* g, GameObject* target);
void AttackTarget(GameObject* g);
void NewObj(GameObject* g);

Rect GetObjRect(GameObject* g);
int GetPlayerOwnedBy(GameObject* g);

bool Damage(GameObject* source, GameObject* g, float value);
void Heal(GameObject* g, float value);
void Stun(GameObject* source, GameObject* g, float value);

void SetMoveSpeed(GameObject* g, float value);
void AddSpeed(GameObject* g, float value);
void AddDamage(GameObject* g, float value);
void ModifyMaxHP(GameObject* g, float value);
void AddMana(GameObject* g, float mana);
void SetManaRegen(GameObject* g, float regen);
void SetHPRegen(GameObject* g, float regen);
void SetRange(GameObject* g, float range);



void Teleport(GameObject* g, float x, float y);
void GetOffsetCenter(GameObject* g, float* x, float* y);
float GetDistCentre(GameObject* g1, GameObject* g2);
void GetCentre(GameObject* g, float* x, float* y);
void KillObj(GameObject* g, bool trigger);
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
int GetWidth(GameObject* g);
int GetHeight(GameObject* g);

bool ObjectCanPush(GameObject* g);
bool IsInCombat(GameObject* g);
bool ObjHasType(GameObject* g, GAMEOBJ_TYPE_HINT typeHint);
bool ObjIsChannelling(GameObject* g);
void SetObjChannelling(GameObject* g, Ability* a, float time,float x, float y, GameObject* target, float heading_x, float heading_y);
void UpdateChannellingdObj(GameObject* g, float dt);
void DrawChannelHint(GameObject* g);

void LoadFolderPrefabs(const char* dirPath, char* name);
void LoadPrefabs(const char* dirPath);
int GetNumObjectsInRect(Rect* r, bool onlyPlayerChoosable);
void RemoveAllGameObjects();
int GetNumPlayerControlledObjs(Player* p);
void SetDecoration(GameObject* g, bool b);
bool ObjIsDecoration(GameObject* g);
void SetInvincible(GameObject* g, bool b);
bool ObjIsInvincible(GameObject* g);
void DrawArrow(int cx, int cy, int targetx, int targety, ALLEGRO_COLOR color);
bool ObjectIsInUI(GameObject* g);
void DrawObjShadows();
void DrawMapHighlight(GameObject* g, int lightSize);
void DrawMapHighlights();
float GetSummonPercent(GameObject* g);
void GameObjDebugDraw();
