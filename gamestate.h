#pragma once

#include <stdbool.h>

typedef enum GameState {
    GAMESTATE_MAIN_MENU,
    GAMESTATE_TRANSITION_TO_CHOOSING_ENCOUNTER,
    GAMESTATE_CHOOSING_ENCOUNTER,
    GAMESTATE_TRANSITION_TO_CHOOSING_UNITS,
    GAMESTATE_CHOOSING_UNITS,
    GAMESTATE_TRANSITION_TO_INGAME,
    GAMESTATE_INGAME,
    GAMESTATE_TRANSITION_TO_END,
    GAMESTATE_END,
    GAMESTATE_TRANSITION_END_TO_CHOOSE_ENCOUNTER,
    GAMESTATE_TRANSITION_END_TO_INGAME,
    GAMESTATE_EXIT
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