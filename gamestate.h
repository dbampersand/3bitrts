#pragma once

#include <stdbool.h>

#define _DEBUG false

typedef enum GameState {
    GAMESTATE_LOAD_SCREEN = 1,
    GAMESTATE_MAIN_MENU = 2,
    GAMESTATE_CHOOSING_ENCOUNTER = 3,
    GAMESTATE_CHOOSING_UNITS = 4,
    GAMESTATE_LOAD_ENCOUNTER = 5,
    GAMESTATE_CHANGE_MAP = 6,
    GAMESTATE_INGAME = 7,
    GAMESTATE_IN_CHATBOX = 8,
    GAMESTATE_END = 9,
    GAMESTATE_WATCHING_REPLAY = 10,
    GAMESTATE_IN_SHOP = 11,
    GAMESTATE_EXIT = 12,
    GAMESTATE_PURCHASING_UNITS = 13,
    GAMESTATE_IN_EDITOR = 14,

    NUMGAMESTATES = 15
} GameState;

typedef enum TRANSITION
{
    TRANSITION_CHAINS,
    TRANSITION_DOOR,
    TRANSITION_STAIRS,
    TRANSITION_TRIANGLES,
    TRANSITION_CHEVRONS,
    TRANSITION_CIRCLE,
    TRANSITION_MOUNTAINS

} TRANSITION;

typedef enum Option_HealthBar
{
    OPTION_HPBAR_ALWAYS = 0,
    OPTION_HPBAR_SELECTED = 1, 
    OPTION_HPBAR_NEVER = 2
}Option_HealthBar;

typedef struct ALLEGRO_EVENT_QUEUE ALLEGRO_EVENT_QUEUE;
typedef struct GameStats
{
    int damageDone;
    int healingDone;
    bool gameWon;
    float timeTaken;
}GameStats;

typedef struct GameObject GameObject;
typedef struct Encounter Encounter;

extern GameStats gameStats;
extern bool combatStarted;
extern ALLEGRO_EVENT_QUEUE* queue;

extern GameState transitioningTo;
extern float transitionTimer;
extern bool transitionAudioPlayed;

extern GameObject** toSpawn;
 extern GameObject* deadFriendlyObjects;


extern Encounter* encounterGoingTo;
extern char* pathToNextMap;

extern GameState gameState;

extern  TRANSITION transitionDrawing;

extern bool _COMING_FROM_EDITOR;



bool TransitionTo(GameState state);

void StartCombat();
void InitGameState();
void SetGameStateToChoosingParty();
void SetGameStateToChoosingEncounter();
void SetGameStateToEndscreen();
void SetGameStateToInMenu();
void SetGameStateToLoadingEncounter(GameObject** list, int numObjectsToAdd, Encounter* e);
void SetGameStateToChangingMap();
void SetGameStateToWatchingReplay();
void SetGameStateToInShop();
void SetGameStateToPurchasingUnits();
void SetGameStateToInEditor();


void Quit();
void CheckIfGameIsLost();
void CheckIfGameIsWon();
void SetGameStateToInGame();
void UpdateTransition(float dt);
void DrawTransition(float dt);
void FinishTransition();
bool GameStateIsTransition(GameState* g);
bool GameIsPaused();
bool GameIsIngame();

void SpawnPartySelects();



void RessurectGameObject(GameObject* g);    
void AddDeadGameObject(GameObject* g);

void GoTutorial();
void WinGame();
void LoseGame();
