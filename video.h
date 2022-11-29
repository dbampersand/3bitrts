#pragma once

#include "rect.h"
#include "point.h"
#include "allegro5/allegro.h"
#include "colors.h"
#include "pointspace.h"


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


//refers to an index in Target FPS pulldown in videoOptionsPanel
typedef enum Option_FPS
{
    OPTION_30FPS = 0,
    OPTION_60FPS = 1,
    OPTION_90FPS = 2, 
    OPTION_144FPS = 3,
    OPTION_240FPS = 4
}Option_FPS;

extern int _TARGET_FPS;

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

typedef enum DecorationType
{
    SPRITE_NONE = 0,
    SPRITE_BG_DECOR,
} DecorationType;

typedef enum RenderOrder
{
    BEFORE_WORLD,
    AFTER_WORLD,
    AFTER_GAMEOBJECTS,
    AFTER_PARTICLES
}RenderOrder;

typedef struct SpriteDecoration
{
    DecorationType decorationType;
    PointSpace position;
    Point velocity;
    int spriteIndex;
    RenderOrder renderOrder;
    Color tint;
    float lifetime;
} SpriteDecoration;

#define MAX_SPRITE_DECORATIONS 255

extern SpriteDecoration spriteDecorations[MAX_SPRITE_DECORATIONS];
extern SpriteDecoration* freeSpriteDecorations[MAX_SPRITE_DECORATIONS];
extern int numFreeDecorations;
extern int* cloudSprites;
extern int numCloudSprites;

extern int* dirtSprites;
extern int numDirtSprites;

#define CLOUD_SPEED 6

extern TextDisplay textDisplays[NUM_TEXT_DISPLAYS];
extern int numStringsToDraw;


extern Point DEBUG_P1;
extern Point DEBUG_P2;
extern Point DEBUG_P3;
extern Point DEBUG_P4;
extern float DEBUG_EXTRA;

typedef struct GameObject GameObject;

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
float easeInOutQuint(float x);
float easeOutExpo(float x);


void ProcessSpriteDecorations(float dt);
void DrawSpriteDecorations(RenderOrder renderOrder);
SpriteDecoration AddCloud();
void AddClouds(int numClouds);
void ClearSpriteDecorations();
void RandomSpriteDecorAtPosition(int numToAdd, int* array, int numArrayElements, int x, int y, RenderOrder renderOrder, Color col);
void RandomSpriteDecor(int numToAdd, int* array, int numArrayElements);
void CreateSpriteDecorAtPosition(int selectedSprite,int x,int y,RenderOrder renderOrder,Color col);
void CreatePoisonEffect(GameObject* g, float lifetime);
void UpdateScreenPositions_Point(PointSpace* p);
