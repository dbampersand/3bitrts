typedef enum GameState {
    MAIN_MENU,
    CHOOSING_ENCOUNTER,
    CHOOSING_UNITS,
    INGAME
} GameState;
GameState gameState;

typedef struct GameOptions
{
    bool* particlesEnabled;
    int renderScale;
} GameOptions;