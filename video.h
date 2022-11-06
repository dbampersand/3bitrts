#pragma once

#include "rect.h"
#include "point.h"
#include "allegro5/allegro.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef struct MouseState
{
    ALLEGRO_MOUSE_STATE mouse;
    int worldX; int worldY;
    int screenX; int screenY;
} MouseState;

extern unsigned long long _FRAMES;

typedef struct PointSpace
{
    float worldX; float worldY; 
    float screenX; float screenY;
} PointSpace;

#define _TARGET_FPS 60

extern ALLEGRO_BITMAP* backbuffer;
extern ALLEGRO_BITMAP* SCREEN;
extern ALLEGRO_BITMAP* background_screen;
extern ALLEGRO_DISPLAY* display;


#define NUM_TEXT_DISPLAYS 30
#define TEXT_DISPLAY_MAX_SIZE 128

typedef struct TextDisplay
{
    char str[TEXT_DISPLAY_MAX_SIZE+1];
    int x; int y;
} TextDisplay;

extern TextDisplay textDisplays[NUM_TEXT_DISPLAYS];
extern int numStringsToDraw;


extern Point DEBUG_P1;
extern Point DEBUG_P2;
extern Point DEBUG_P3;
extern Point DEBUG_P4;
extern float DEBUG_EXTRA;

void DrawBufferedStrings();

void DrawRoundedRect(Rect r, ALLEGRO_COLOR color, bool filled);
void DrawOutlinedRect_Dithered(Rect r, ALLEGRO_COLOR color);
void DrawCone(int x, int y, float angle, float radius, int length, ALLEGRO_COLOR color);
void GetConeVertices(int cx, int cy, int* x1, int* y1, int* x2, int* y2, float angle, float radius, int length);
bool isInsideSector(int x, int y, int cx, int cy, float startX, float startY, float endX, float endY, float length, float radius, float amtRotated);
bool RectInCone(Rect r, int cx, int cy, float angle, float radius, float length);

float CircleRectDist(int cx, int cy, float radius, Rect r);
void CircleSegment(int xc, int yc, float radius, float start, float end, ALLEGRO_COLOR col, float length);
void SetDisplaySize();
void InitVideo();
void DrawFilledRect_Dithered(Rect r, ALLEGRO_COLOR color);
float easeOutQuint(float x);
float easeInCirc(float x);
float easeOutCirc(float x);