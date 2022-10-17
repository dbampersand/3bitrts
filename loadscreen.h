#pragma once

#include "point.h"

#define MAX_LOADSCREEN_STATES 5

typedef enum LOADSCREEN_STATE
{   
    LOADSCREEN_TRANSITON_IN = 0,
    LOADSCREEN_MOVING = 1,
    LOADSCREEN_PAUSE = 2,
    LOADSCREEN_TEXT_TRANSITION_IN = 3,
    LOADSCREEN_END
} STARTSCREEN_STATE;

#define LOADSCREEN_TEXTBOXW 200
#define LOADSCREEN_TEXTBOXH 60
#define MAX_LOADSCREEN_SPRITES 24

typedef enum LoadScreenDrawOrder
{
    ORDER_BEFORESPRITE,
    ORDER_AFTERSPRITE
}LoadScreenDrawOrder;

typedef struct LoadScreenSprite
{
    float angle;
    float v;
    Point position;
    int spriteIndex;
    bool initialized;
    LoadScreenDrawOrder draw;
}LoadScreenSprite;

typedef struct LoadScreen
{  

    LoadScreenSprite sprites[MAX_LOADSCREEN_SPRITES];

    float timer;
    STARTSCREEN_STATE state;
    float (*transition)(float timer);
    int spriteIndex;
    int maskSpriteIndex;
    float x; float y;
    float transitionTimers[MAX_LOADSCREEN_STATES];

    float textBoxX; float textBoxY;
    float (*textBoxTransition)(float timer);

    char* header;
    char* description;
} LoadScreen;

typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
typedef struct MouseState MouseState;

extern LoadScreen loadScreen;

#define GAME_NAME "MON IOCHDAR"

void InitLoadscreen(char* initialPath, char* mask);
float Transition_EaseInOutQuad(float timer);
void DrawLoadscreen();
void UpdateLoadscreen(float dt, ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState);
void SetLoadscreen(char* path, char* mask, float transitionInTime, float moveTime, float pauseTime, float textInTime, float textHoldTime, char* header, char* description);
void FinishLoadScreen();
void DrawLoadScreenSprites(LoadScreenDrawOrder order);
void UpdateLoadscreenSprites(float dt);
