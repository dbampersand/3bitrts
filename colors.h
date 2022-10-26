#pragma once

#include "allegro5/allegro.h"
#include "effect.h"

typedef enum DITHER_PATTERN
{
    DITHER_NONE=0,
    DITHER_FILL=1,
    DITHER_HALF=2,
    DITHER_QUARTER=4,
    DITHER_EIGTH=8,

    DITHER_VERTICAL_HALF,
    DITHER_VERTICAL_QUARTER,
    DITHER_VERTICAL_EIGTH,


    DITHER_HORIZONTAL_HALF,
    DITHER_HORIZONTAL_QUARTER,
    DITHER_HORIZONTAL_EIGTH,

    DITHER_STAR_HALF,
    DITHER_STAR_QUARTER,
    DITHER_STAR_EIGTH,

    DITHER_ALL

} DITHER_PATTERN;
typedef enum Color
{
    COLOR_DEFAULT = 0, //COLOR_DEFAULT inherits whoever owns the object's color
    COLOR_BG,
    COLOR_FRIENDLY,
    COLOR_ENEMY,
    COLOR_GROUND,
    COLOR_POISON,
    COLOR_HEAL,
    COLOR_DAMAGE,
    COLOR_WHITE,
    COLOR_GROUND_DARK,
    COLOR_EDGE_HIGHLIGHT,
    COLOR_TRANSPARENT,

    COLOR_ALL //used to get the number of elements in enum
} Color;

extern ALLEGRO_COLOR BG;
extern ALLEGRO_COLOR FRIENDLY;
extern ALLEGRO_COLOR ENEMY;
extern ALLEGRO_COLOR GROUND;
extern ALLEGRO_COLOR GROUND_DARK;
extern ALLEGRO_COLOR EDGE_HIGHLIGHT;

extern ALLEGRO_COLOR WHITE;

extern ALLEGRO_COLOR POISON;
extern ALLEGRO_COLOR HEAL;
extern ALLEGRO_COLOR DAMAGE;
extern ALLEGRO_COLOR TRANSPARENT;


extern ALLEGRO_COLOR* ALColorLookup[COLOR_ALL];
extern Color EffectColors[EFFECT_ALL];


ALLEGRO_COLOR GetColor(Color c, int objectOwnedBy);
Color ALColorToCol(ALLEGRO_COLOR c);
void InitColors();
bool AlColIsEq(ALLEGRO_COLOR c, ALLEGRO_COLOR c2);
unsigned char HashColor(ALLEGRO_COLOR c);
Color GetEffectColor(Effect* e);
Color GameObjToColor(GameObject* g);