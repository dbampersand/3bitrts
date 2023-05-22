#include "colors.h"
#include "command.h"
#include "gameobject.h"
#include <stdio.h>
#include "helperfuncs.h"
#include "effect.h"

ALLEGRO_COLOR BG = {0};
ALLEGRO_COLOR FRIENDLY = {0};
ALLEGRO_COLOR ENEMY = {0};
ALLEGRO_COLOR GROUND = {0};
ALLEGRO_COLOR GROUND_DARK = {0};
ALLEGRO_COLOR EDGE_HIGHLIGHT = {0};

ALLEGRO_COLOR WHITE = {0};

ALLEGRO_COLOR POISON = {0};
ALLEGRO_COLOR HEAL = {0};
ALLEGRO_COLOR DAMAGE = {0};
ALLEGRO_COLOR SPEED = {0};
ALLEGRO_COLOR SHIELD = {0};
ALLEGRO_COLOR BG_DECOR = {0};
ALLEGRO_COLOR TEXTURED_GROUND = {0};
    
ALLEGRO_COLOR _TRANSPARENT = {0};

ALLEGRO_COLOR FRIENDLY_POISON;
ALLEGRO_COLOR FRIENDLY_HEAL;
ALLEGRO_COLOR FRIENDLY_DAMAGE;
ALLEGRO_COLOR FRIENDLY_SPEED;
ALLEGRO_COLOR FRIENDLY_SHIELD;




ALLEGRO_COLOR* ALColorLookup[COLOR_ALL] = {0};
Color* EffectColors = {0};

ALLEGRO_COLOR GetColor(Color c, int objectOwnedBy)
{
    if (c >= COLOR_ALL)
        return BG;
    if (c < 0)
        return BG;

    if (c ==  COLOR_DEFAULT)
    {
        if (objectOwnedBy == TYPE_FRIENDLY)
        {
            return FRIENDLY;
        }
        if (objectOwnedBy == TYPE_ENEMY)
        {
            return ENEMY;
        }
        if (objectOwnedBy == TYPE_DECORATION)
            return BG;
    }
    return *ALColorLookup[c];

/*
    if (c == COLOR_BG)
        return BG;
    if (c == COLOR_FRIENDLY)
        return FRIENDLY;
    if (c == COLOR_ENEMY)
        return ENEMY;
    if (c == COLOR_GROUND)
        return GROUND;
    if (c == COLOR_POISON)
        return POISON;
    if (c == COLOR_HEAL)
        return HEAL;
    if (c == COLOR_DAMAGE)
        return DAMAGE;
    return BG;*/
}
bool AlColIsEq(ALLEGRO_COLOR c, ALLEGRO_COLOR c2)
{
    return (c.r == c2.r && c.g == c2.g && c.b == c2.b && c.a == c2.a);
}

Color ALColorToCol(ALLEGRO_COLOR c)
{

    if (AlColIsEq(c,BG))
        return COLOR_BG;
    if (AlColIsEq(c,FRIENDLY))
        return COLOR_FRIENDLY;
    if (AlColIsEq(c,ENEMY))
        return COLOR_ENEMY;
    if (AlColIsEq(c,GROUND))
        return COLOR_GROUND;
    if (AlColIsEq(c,POISON))
        return COLOR_POISON;
    if (AlColIsEq(c,HEAL))
        return COLOR_HEAL;
    if (AlColIsEq(c,DAMAGE))
        return COLOR_DAMAGE;
    if (AlColIsEq(c,WHITE))
        return COLOR_WHITE;
    if (AlColIsEq(c,GROUND_DARK))
        return COLOR_GROUND_DARK;
    if (AlColIsEq(c,EDGE_HIGHLIGHT))
        return COLOR_EDGE_HIGHLIGHT;
    if (AlColIsEq(c,BG_DECOR))
        return COLOR_BG_DECOR;
    if (AlColIsEq(c,TEXTURED_GROUND))
        return COLOR_TEXTURED_GROUND;



    
    return COLOR_DEFAULT;
}
unsigned char HashColor(ALLEGRO_COLOR c)
{
    return (unsigned char)((int)(c.r*255) ^ (int)(c.g*255) ^ (int)(c.b*255) ^ (int)(c.a*255));

}
void InitColors()
{
    BG = al_map_rgba(24,5,26,255);
    GROUND = al_map_rgba(74,64,201,255);

    ENEMY = al_map_rgba(95,247,247,255);
    FRIENDLY = al_map_rgba(242,246,255,255);

    WHITE = al_map_rgba(255,255,255,255);
  
    //Specific effect colours
    HEAL = al_map_rgba(119,249,26,255);
    POISON = al_map_rgba(237,66,229,255);
    DAMAGE = al_map_rgba(250,95,95,255);
    SPEED = al_map_rgba(248,232,22,255);
    SHIELD = al_map_rgba(106,171,235,255); 

    GROUND_DARK = al_map_rgba(42,20,97,255);
    EDGE_HIGHLIGHT = al_map_rgba(167,112,213,255);
    _TRANSPARENT = al_map_rgba(0,0,0,0);
    BG_DECOR = al_map_rgba((BG.r*255)+10,(BG.g*255)+15,(BG.b*255)+10,255);
    TEXTURED_GROUND = al_map_rgba((GROUND.r*255)-5,(GROUND.g*255) - 5,(GROUND.b*255)-5,255);


    
    FRIENDLY_HEAL = al_map_rgba(clamp((HEAL.r*255)+FRIENDLY_COLOR_ADD,0,255),clamp((HEAL.g*255)+FRIENDLY_COLOR_ADD,0,255),clamp((HEAL.b*255)+FRIENDLY_COLOR_ADD,0,255),255);
    FRIENDLY_POISON = al_map_rgba(clamp((POISON.r*255)+FRIENDLY_COLOR_ADD,0,255),clamp((POISON.g*255)+FRIENDLY_COLOR_ADD,0,255),clamp((POISON.b*255)+FRIENDLY_COLOR_ADD,0,255),255);
    FRIENDLY_DAMAGE = al_map_rgba(clamp((DAMAGE.r*255) + FRIENDLY_COLOR_ADD,0,255),clamp((DAMAGE.g*255)+FRIENDLY_COLOR_ADD,0,255),clamp((DAMAGE.b*255)+FRIENDLY_COLOR_ADD,0,255),255);
    FRIENDLY_SPEED = al_map_rgba(clamp((SPEED.r*255) + FRIENDLY_COLOR_ADD,0,255),clamp((SPEED.g*255)+FRIENDLY_COLOR_ADD,0,255),clamp((SPEED.b*255)+FRIENDLY_COLOR_ADD,0,255),255);
    FRIENDLY_SHIELD = al_map_rgba(clamp((SHIELD.r*255) + FRIENDLY_COLOR_ADD,0,255),clamp((SHIELD.g*255)+FRIENDLY_COLOR_ADD,0,255),clamp((SHIELD.b*255)+FRIENDLY_COLOR_ADD,0,255),255);

    ALColorLookup[COLOR_DEFAULT] = &BG;
    ALColorLookup[COLOR_BG] = &BG;
    ALColorLookup[COLOR_GROUND] = &GROUND;
    ALColorLookup[COLOR_GROUND_DARK] = &GROUND_DARK;
    ALColorLookup[COLOR_ENEMY] = &ENEMY;
    ALColorLookup[COLOR_FRIENDLY] = &FRIENDLY;
    ALColorLookup[COLOR_WHITE] = &WHITE;

    ALColorLookup[COLOR_HEAL] = &HEAL;
    ALColorLookup[COLOR_POISON] = &POISON;
    ALColorLookup[COLOR_DAMAGE] = &DAMAGE;
    ALColorLookup[COLOR_SPEED] = &SPEED;
    ALColorLookup[COLOR_SHIELD] = &SHIELD;

    ALColorLookup[COLOR_FRIENDLY_HEAL] = &FRIENDLY_HEAL;
    ALColorLookup[COLOR_FRIENDLY_POISON] = &FRIENDLY_POISON;
    ALColorLookup[COLOR_FRIENDLY_DAMAGE] = &FRIENDLY_DAMAGE;
    ALColorLookup[COLOR_FRIENDLY_SPEED] = &FRIENDLY_SPEED;
    ALColorLookup[COLOR_FRIENDLY_SHIELD] = &FRIENDLY_SHIELD;


    ALColorLookup[COLOR_EDGE_HIGHLIGHT] = &EDGE_HIGHLIGHT;
    ALColorLookup[COLOR__TRANSPARENT] = &_TRANSPARENT;
    ALColorLookup[COLOR_BG_DECOR] = &BG_DECOR;
    ALColorLookup[COLOR_TEXTURED_GROUND] = &TEXTURED_GROUND;






    queueCommandColors[COMMAND_NONE] = COLOR_BG;
    queueCommandColors[COMMAND_MOVE] = COLOR_FRIENDLY;
    queueCommandColors[COMMAND_ATTACK] = COLOR_DAMAGE;
    queueCommandColors[COMMAND_CAST] = COLOR_GROUND_DARK;
    queueCommandColors[COMMAND_STOP] = COLOR_FRIENDLY;
    queueCommandColors[COMMAND_ATTACKMOVE] = COLOR_DAMAGE;
    queueCommandColors[COMMAND_FOLLOW] = COLOR_FRIENDLY;
    queueCommandColors[COMMAND_HOLD] = COLOR_FRIENDLY;

    EffectColors = calloc(EFFECT_ALL,sizeof(Color));
    EffectColors[EFFECT_NONE] = COLOR_BG;
    EffectColors[EFFECT_MAXHP] = COLOR_HEAL;
    EffectColors[EFFECT_HURT] = COLOR_DAMAGE;
    EffectColors[EFFECT_HURT_PERCENT] = COLOR_DAMAGE;
    EffectColors[EFFECT_STUN] = COLOR_DAMAGE;
    EffectColors[EFFECT_HEAL] = COLOR_HEAL;
    EffectColors[EFFECT_POSITION] = COLOR_GROUND_DARK;
    EffectColors[EFFECT_THREAT] = COLOR_FRIENDLY;
    EffectColors[EFFECT_SPEED] = COLOR_SPEED;
    EffectColors[EFFECT_MISS_CHANCE] = COLOR_SPEED;
    EffectColors[EFFECT_SHIELD] = COLOR_SHIELD;
    EffectColors[EFFECT_ATTACKDAMAGE] = COLOR_DAMAGE;
    EffectColors[EFFECT_ATTACKSPEED] = COLOR_SPEED;
    EffectColors[EFFECT_ATTACKRANGE] = COLOR_DAMAGE;
    EffectColors[EFFECT_INVULN] = COLOR_HEAL;
    EffectColors[EFFECT_CURE] = COLOR_HEAL;
    EffectColors[EFFECT_ADD_MANA] = COLOR_HEAL;
    EffectColors[EFFECT_ARMOR] = COLOR_SHIELD;
    EffectColors[EFFECT_LIFESTEAL] = COLOR_HEAL;
    EffectColors[EFFECT_COOLDOWN_RATE] = COLOR_SPEED;



}

Color GetEffectColor(Effect* e)
{
    if (!e) return COLOR_DEFAULT;
    return EffectColors[e->effectType];
}
Color GameObjToColor(GameObject* g)
{
    Color c = IsOwnedByPlayer(g) == true ? COLOR_FRIENDLY : COLOR_ENEMY;
    if (ObjIsDecoration(g))
        c = COLOR_BG;
    if (g->summonTime < g->summonMax)
        c = COLOR_GROUND_DARK;
    return c;

}
