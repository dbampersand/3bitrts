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
    GAMESTATE_EXIT = 11,

    NUMGAMESTATES = 12
} GameState;
GameState gameState;

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

GameStats gameStats;
bool combatStarted;
ALLEGRO_EVENT_QUEUE* queue;
typedef struct GameObject GameObject;
typedef struct Encounter Encounter;

GameState transitioningTo;
float transitionTimer;

GameObject** toSpawn;

Encounter* encounterGoingTo;
char* pathToNextMap;

bool TransitionTo(GameState state);

void StartCombat();
void InitGameState();
void SetGameStateToChoosingParty();
void SetGameStateToChoosingEncounter();
void SetGameStateToEndscreen();
void SetGameStateToInMenu();
void SetGameStateToLoadingEncounter(GameObject** list, int numObjectsToAdd, Encounter* e);
void SetGameStateToChangingMap(const char* mapPath);
void SetGameStateToWatchingReplay();


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

void GoTutorial();
void WinGame();
void LoseGame();

float easeOutBack(float x); 
float easeInOutBack(float x);
float easeOutSine(float x);
