#pragma once

#include "rect.h"
#include "point.h"
#include "allegro5/allegro.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

unsigned long long _FRAMES;


#define _TARGET_FPS 60

ALLEGRO_BITMAP* backbuffer;
ALLEGRO_BITMAP* SCREEN;
ALLEGRO_BITMAP* background_screen;
ALLEGRO_DISPLAY* display;


#define NUM_TEXT_DISPLAYS 30
#define TEXT_DISPLAY_MAX_SIZE 128

typedef struct TextDisplay
{
    char str[TEXT_DISPLAY_MAX_SIZE+1];
    int x; int y;
} TextDisplay;
TextDisplay textDisplays[NUM_TEXT_DISPLAYS];
int numStringsToDraw;


Point DEBUG_P1;
Point DEBUG_P2;
Point DEBUG_P3;
Point DEBUG_P4;
float DEBUG_EXTRA;

void DrawBufferedStrings();

void DrawRoundedRect(Rect r, ALLEGRO_COLOR color, bool filled);
void DrawOutlinedRect_Dithered(Rect* r, ALLEGRO_COLOR color);
void DrawCone(int x, int y, float angle, float radius, int length, ALLEGRO_COLOR color);
void GetConeVertices(int cx, int cy, int* x1, int* y1, int* x2, int* y2, float angle, float radius, int length);
bool isInsideSector(int x, int y, int cx, int cy, float startX, float startY, float endX, float endY, float length, float radius, float amtRotated);
bool RectInCone(Rect r, int cx, int cy, float angle, float radius, float length);

float CircleRectDist(int cx, int cy, float radius, Rect r);
void CircleSegment(int xc, int yc, float radius, float start, float end, ALLEGRO_COLOR col, float length);
void SetDisplaySize();
void InitVideo();
void DrawFilledRect_Dithered(Rect r, ALLEGRO_COLOR color);
