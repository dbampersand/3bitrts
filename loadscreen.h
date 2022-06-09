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

typedef struct LoadScreen
{
    float timer;
    STARTSCREEN_STATE state;
    float (*transition)(float timer);
    int spriteIndex;
    float x; float y;
    float transitionTimers[MAX_LOADSCREEN_STATES];

    float textBoxX; float textBoxY;
    float (*textBoxTransition)(float timer);
    
} LoadScreen;

typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;
typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;

LoadScreen loadScreen;

void InitLoadscreen(char* initialPath);
float Transition_EaseInOutQuad(float timer);
void DrawLoadscreen();
void UpdateLoadscreen(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState);
void SetLoadscreen(char* path, float transitionInTime, float moveTime, float pauseTime, float textInTime, float textHoldTime);
void FinishLoadScreen();

