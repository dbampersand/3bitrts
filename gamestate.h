#pragma once

#include <stdbool.h>
#define NUMGAMESTATES 7


typedef enum GameState {
    GAMESTATE_LOAD_SCREEN = 1,
    GAMESTATE_MAIN_MENU = 2,
    GAMESTATE_CHOOSING_ENCOUNTER = 3,
    GAMESTATE_CHOOSING_UNITS = 4,
    GAMESTATE_LOAD_ENCOUNTER = 5,
    GAMESTATE_INGAME = 6,
    GAMESTATE_END = 7,
    GAMESTATE_EXIT = 8
} GameState;
GameState gameState;

typedef enum Option_HealthBar
{
    OPTION_HPBAR_ALWAYS = 0,
    OPTION_HPBAR_SELECTED = 1, 
    OPTION_HPBAR_NEVER = 2
}Option_HealthBar;

typedef struct ALLEGRO_EVENT_QUEUE ALLEGRO_EVENT_QUEUE;
typedef struct GameOptions
{
    bool* particlesEnabled;
    Option_HealthBar* displayHealthBar; 
} GameOptions;
GameOptions gameOptions;
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


void StartCombat();
void InitGameState();
void SetGameStateToChoosingParty();
void SetGameStateToChoosingEncounter();
void SetGameStateToEndscreen();
void SetGameStateToInMenu();
void SetGameStateToLoadingEncounter(GameObject** list, int numObjectsToAdd, Encounter* e);


void Quit();
void CheckIfGameIsLost();
void CheckIfGameIsWon();
void SetGameStateToInGame();
void UpdateTransition(float dt);
void DrawTransition(float dt);
void FinishTransition();
bool GameStateIsTransition(GameState* g);
