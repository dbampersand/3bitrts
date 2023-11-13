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
#include "item.h"
#include "gameobject.h"
#include "stdbool.h"
#include "video.h" 
#include "pointspace.h"
#include <stdint.h>
#include "luafuncs.h"


#define MINIMUM_RANGE 6
#define SHIELD_EFFECT_TIME 0.5f
#define INVENTORY_SLOTS 3
#define DEFAULT_BOUNTY 7
#define DEFAULT_COMPLETION_PERCENT 5

#define CLEAVE_RADIUS 30

typedef enum GAMEOBJ_PROPERTIES
{
    OBJ_SELECTED = 1,
    OBJ_OWNED_BY = 2,
    OBJ_ACTIVE = 4,
    OBJ_CAN_PUSH = 8,
    OBJ_IS_CHANNELLING = 16,
    OBJ_IS_DECORATION = 32,
    OBJ_IS_INVINCIBLE = 64,
    //OBJ_ATTACKING_TARGET = 8
} GAMEOBJ_PROPERTIES;

typedef enum GAMEOBJ_SOURCE
{
    SOURCE_SPAWNED_FROM_MAP,
    SOURCE_SPAWNED_FROM_OBJ
} GAMEOBJ_SOURCE;

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
#define MAX_EFFECTS 16
#define MAX_QUEUED_CMD 8
#define MAX_ABILITIES 8
#define FLASH_TIMER 0.1
#define HEALTHBAR_HEIGHT 2


typedef struct GameObject
{    
    bool isPrefab;
    struct GameObject* prefab;
    LuaBuffer lua_buffer;

    char* name; 
    char* description;

    char* path; 
    GameObject* targObj;

    int* onAttackEffectsIndices;
    
    Shield* shields;
    float shieldSizeTimer;

    Ability abilities[MAX_ABILITIES]; 
    Effect effects[MAX_EFFECTS];
    Command queue[MAX_QUEUED_CMD];

    int numAbilities;

    int lightSize;
    float lightR;
    float lightG;
    float lightB;
    float lightIntensity;



    PointSpace position;
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

    uint32_t spriteIndex;
    uint32_t selectedSpriteIndex;
    unsigned int channelingSpriteIndex;

    int luafunc_update;
    int luafunc_setup;
    int luafunc_kill;
    int luafunc_onattack;
    int luafunc_onmapchange;

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

    float lastDamage;
    float flashTimer;

    bool deathTimerActivated;
    float deathTimer;

    float stunTimer;
    bool objectIsStunnable;

    PathfindNode pathNodes[MAX_PATHFINDING_NODES_HELD];
    int currentPathingNode;
    bool pathfindNeedsRefresh;
    int numPathnodes;

    bool shouldProcessAI;

    float lifesteal;
    
    Item inventory[INVENTORY_SLOTS];

    int bounty;

    bool isBoss;

    float completionPercent;
    bool isRemovedFromCount;

    int ressurectionCost;

    int spriteIndex_PurchaseScreenSprite;
    bool purchased;
    int cost;

    bool enraged;

    float nextFootstepTime;
    
    bool aggroGroupSet;
    int aggroGroup;

    float pushTimer;
    float pushFullTime;
    Point pushDir;
    float pushSpeed;

    bool objIsPushable;

    float missChance;

    float cooldownRate;
    float abilityPotency;
    float cleave;
    float dodgeChance;

    bool objectIsSummoned;

    bool commandsBlocked;
} GameObject;

#define MAX_LIGHT_SIZE 90 
#define LIGHT_FACTOR 0.01f;

extern float _LIGHT_X_ANGLES[MAX_LIGHT_SIZE*2][MAX_LIGHT_SIZE*2];
extern float _LIGHT_Y_ANGLES[MAX_LIGHT_SIZE*2][MAX_LIGHT_SIZE*2];

extern float* lightXAngles;
extern float* lightYAngles;


#define NUM_GAMEOBJECT_FUNCTIONS 5

#define MAX_OBJS 64
extern GameObject* objects;
extern int objectsAllocated;

extern GameObject** activeObjects;
extern int numActiveObjects;



extern GameObject** prefabs;
extern int numPrefabs;
extern int numPrefabsAllocated;



extern ALLEGRO_BITMAP* lights[MAX_LIGHT_SIZE];

typedef struct CollisionEvent
{
    GameObject* obj;
    bool x;
    int direction;
}CollisionEvent;

extern CollisionEvent collisionEvents[MAX_OBJS];




extern float __sinTable[360*2];
extern float __cosTable[360*2];

extern float* sinTable;// = &__sinTable[360];
extern float* cosTable;// = &__cosTable[360];

extern int numChannellingInfosDrawn;

extern int* footstepSFXIndices;
extern int numFootsteps;
extern int lastFootstepPlayed;

extern HashTable prefabsIndicesHashTable;



//add 8 more free objects when we need to realloc objects array
#define BUFFER_PREALLOC_AMT 8

void MoveObjTo(GameObject* g, float x, float y, bool objCanPush);

void PushObj(GameObject* g, float velocity, float timeToPush, Point from);
void UpdatePush(GameObject* g, float dt);

bool PlayerHasEnemyUnitSelected();
void SetTargetPosition(GameObject* g, float x, float y);
void ClearPathfindingQueue(GameObject* g);
GameObject* GetMousedOver(MouseState* mouseState);
void UpdatePlayerObjectInteractions(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseState);
void ProcessAttackMoveMouseCommand(MouseState* mouseState,ALLEGRO_KEYBOARD_STATE* keyState);
void UpdateObject(GameObject* g, float dt);
void InitObjects();
void GetControlGroup(ALLEGRO_KEYBOARD_STATE* keyState);
void SetControlGroups(ALLEGRO_KEYBOARD_STATE* keyState);
int GetCtrlGroup(int index);
void SetCtrlGroup(int index, GameObject** list, int numUnitsSelected);
void CheckSelected(MouseState* mouseState, MouseState* mouseLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
GameObject* AddGameobject(GameObject* prefab, float x, float y, GAMEOBJ_SOURCE source, int playerOwnedBy);
bool CheckFuncExists(const char* funcName, LuaBuffer* lua_buffer);
bool loadLuaGameObj(lua_State* l,const char* filename, GameObject* g);
GameObject* LoadPrefab(const char* path);
bool IsSelected(GameObject* g);
bool IsSelectable(GameObject* g);
void SetSelected(GameObject* g, bool select);
bool ObjIsDecoration(GameObject* g);

bool IsOwnedByPlayer(GameObject* g);
void SetOwnedBy(GameObject* g, int i);
void Move(GameObject* g, float delta);

void DrawHealthBar(GameObject* g, ALLEGRO_COLOR col);
void DrawGameObj(GameObject* g, bool forceInverse); 
void DrawEffectVisuals(GameObject* g);


void SetAttackingObj(GameObject* g, GameObject* target);
void AttackTarget(GameObject* g, float dt);
void NewObj(GameObject* g);

Rect GetObjRect(GameObject* g);
int GetPlayerOwnedBy(GameObject* g);
int GetPlayerOwnedBy_IncludeDecor(GameObject* g);


bool Damage(GameObject* source, GameObject* g, float value,bool triggerItems, float min, Effect* effect);
void Heal(GameObject* g, float value);
void Stun(GameObject* source, GameObject* g, float value);

void SetMoveSpeed(GameObject* g, float value);
void AddSpeed(GameObject* g, float value);
void AddMissChance(GameObject* g, float value);
void AddDamage(GameObject* g, float value);
void ModifyMaxHP(GameObject* g, float value);
void ModifyMaxMana(GameObject* g, float value);
void ModifyManaRegen(GameObject* g, float value);
void ModifyAbilityPotency(GameObject* g, float value);
void AddMana(GameObject* g, float mana);
void SetManaRegen(GameObject* g, float regen);
void SetHPRegen(GameObject* g, float regen);
void SetRange(GameObject* g, float range);
void AddAttackSpeed(GameObject* g, float speed);
void AddAttackRange(GameObject* g, float range);
void AddArmor(GameObject* g, float armor);
void AddLifesteal(GameObject* g, float value);
void ModifyCleave(GameObject* g, float value);
void AddCooldownRate(GameObject* g, float value);
void ModifyDodgeChance(GameObject* g, float value);

void SetObjIsBoss(GameObject* g, bool value);

void Teleport(GameObject* g, float x, float y, bool updateold);
void GetOffsetCenter(GameObject* g, float* x, float* y);
float GetDistCentre(GameObject* g1, GameObject* g2);
void GetCentre(GameObject* g, float* x, float* y);
void KillObj(GameObject* g, bool trigger, bool spawnParticles);
bool IsActive(GameObject* g);
void DoAI(GameObject* g);

float GetDist(GameObject* g1, GameObject* g2);
float RectDist_R(Rect r1, Rect r2);
float RectDist(GameObject* g1, GameObject* g2);
void CheckCollisionsWorld(GameObject* g, bool x, float dV); 
void CheckCollisions(GameObject* g, bool x, float dV, bool objectCanPush);
GameObject* GetCollidedWith(GameObject* g);
GameObject* GetClicked(float x, float y);
void UnsetAll();
void MakeInvulnerable(GameObject* g, float time);
int GetWidth(GameObject* g);
int GetHeight(GameObject* g);
float GetAttackRange(GameObject* g);
int GetNumberOfBadEffects(GameObject* g);
int GetNumberOfActiveEffects(GameObject* g);

void SetObjectCanPush(GameObject* g, bool value);
bool ObjectCanPush(GameObject* g);
bool IsInCombat(GameObject* g);
bool ObjHasType(GameObject* g, GAMEOBJ_TYPE_HINT typeHint);
bool ObjIsChannelling(GameObject* g);
void SetObjChannelling(GameObject* g, Ability* a, float time,float x, float y, GameObject* target, float heading_x, float heading_y);
void UpdateChannellingdObj(GameObject* g, float dt);
void DrawChannelHint(GameObject* g, float dt, MouseState mouseState);

void LoadFolderPrefabs(const char* dirPath, char* name);
void LoadPrefabs(const char* dirPath);
int GetNumObjectsInRect(Rect* r, bool onlyPlayerChoosable);
void RemoveAllGameObjects();
int GetNumPlayerControlledObjs(Player* p);
bool SetDecoration(GameObject* g, bool b);
bool x(GameObject* g);
void SetInvincible(GameObject* g, bool b);
bool ObjIsInvincible(GameObject* g);
void DrawArrow(int cx, int cy, int targetx, int targety, ALLEGRO_COLOR color, ALLEGRO_COLOR* bg);
bool ObjectIsInUI(GameObject* g);
void DrawObjShadows();
void DrawMapHighlight(GameObject* g, int lightSize, ALLEGRO_BITMAP* screen);
void DrawMapHighlights();
float GetSummonPercent(GameObject* g);
void GameObjDebugDraw();
int GetUIStartHeight();
void UpdateObjPosition(GameObject* g, float x, float y);
void UpdateObjPosition_X(GameObject* g, float x);
void UpdateObjPosition_Y(GameObject* g, float y);
void UpdateScreenPositions(GameObject* g);
void GetCentre_Screen(GameObject* g, float* x, float* y);

void GenerateCircleHighlight(int lightSize);
void SetLightSize(GameObject* g, int size);
float GetTotalDotted(GameObject* g);

int GetNumActiveObjects();
bool RectIsInUI(float x, float y, float w, float h);


int GetNumEnemyObjects();
void PrintDiedFrom(GameObject* obj, GameObject* damageSource, Effect* effectSource, int damage);
bool ObjIsBoss(GameObject* g);
void DrawAggroIndicators();
void DrawObjHeadingArrows();
void DrawStunEffect(GameObject* g);
bool IsInvertedSprite(GameObject* g);

void ScatterEffect_Sprite(Sprite* s, int xPos, int yPos, Color c);
void ScatterEffect(GameObject* g);

bool GameObjectIndexInRange(int index);
Point GetClosestPointObjToObj(GameObject* g, GameObject* g2);
