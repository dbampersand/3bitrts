#pragma once

#include <stdbool.h>
#define NUMGAMESTATES 6
typedef enum GameState {
    GAMESTATE_MAIN_MENU = 0,
    GAMESTATE_CHOOSING_ENCOUNTER = 1,
    GAMESTATE_CHOOSING_UNITS = 2,
    GAMESTATE_INGAME = 3,
    GAMESTATE_END = 4,
    GAMESTATE_EXIT = 5
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

void Quit();
void CheckIfGameIsLost();
void CheckIfGameIsWon();
void SetGameStateToInGame(GameObject** list, int numObjectsToAdd, Encounter* e);
void UpdateTransition(float dt);
void DrawTransition(float dt);
void FinishTransition();
bool GameStateIsTransition(GameState* g);
