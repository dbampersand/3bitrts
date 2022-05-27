#pragma once
#include <stdbool.h>

typedef enum GameState {
    MAIN_MENU,
    CHOOSING_ENCOUNTER,
    CHOOSING_UNITS,
    INGAME
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

void StartCombat();