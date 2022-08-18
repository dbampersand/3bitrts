#include "colors.h"
#include "command.h"
#include "gameobject.h"
ALLEGRO_COLOR GetColor(Color c, int objectOwnedBy)
{
    if (c >= COLOR_ALL)
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
    unsigned char hash = HashColor(c);//(int)c.r ^ (int)c.g ^ (int)c.b ^ (int)c.a;
    return ColorHashTable[hash];
    /*if (AlColIsEq(c,BG))
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
    
    return COLOR_DEFAULT;*/
}
unsigned char HashColor(ALLEGRO_COLOR c)
{
    return (unsigned char)((int)(c.r*255) ^ (int)(c.g*255) ^ (int)(c.b*255) ^ (int)(c.a*255));

}
void InitColors()
{
    BG = al_map_rgba(24,5,26,255);
    GROUND = al_map_rgba(92,92,189,255);

    ENEMY = al_map_rgba(95,247,247,255);
    FRIENDLY = al_map_rgba(242,246,255,255);

    WHITE = al_map_rgba(255,255,255,255);
  
    HEAL = al_map_rgba(119,249,26,255);
    POISON = al_map_rgba(237,66,229,255);
    DAMAGE = al_map_rgba(250,95,95,255);
    GROUND_DARK = al_map_rgba(43,30,94,255);
    EDGE_HIGHLIGHT = al_map_rgba(106,106,212,255);


    ALColorLookup[COLOR_BG] = &BG;
    ALColorLookup[COLOR_GROUND] = &GROUND;
    ALColorLookup[COLOR_GROUND_DARK] = &GROUND_DARK;
    ALColorLookup[COLOR_ENEMY] = &ENEMY;
    ALColorLookup[COLOR_FRIENDLY] = &FRIENDLY;
    ALColorLookup[COLOR_WHITE] = &WHITE;
    ALColorLookup[COLOR_HEAL] = &HEAL;
    ALColorLookup[COLOR_POISON] = &POISON;
    ALColorLookup[COLOR_DAMAGE] = &DAMAGE;
    ALColorLookup[COLOR_EDGE_HIGHLIGHT] = &EDGE_HIGHLIGHT;


    memset(ColorHashTable,0,256*sizeof(ColorHashTable[0]));
    

    for (int i = 1; i < COLOR_ALL; i++)
    {
        ALLEGRO_COLOR c = *ALColorLookup[i];
        unsigned char hash = HashColor(c);
        if (ColorHashTable[hash])
        {
            printf("HASH COLLISION");
        }
        ColorHashTable[hash] = i;
    }


    queueCommandColors[COMMAND_NONE] = COLOR_BG;
    queueCommandColors[COMMAND_MOVE] = COLOR_FRIENDLY;
    queueCommandColors[COMMAND_ATTACK] = COLOR_DAMAGE;
    queueCommandColors[COMMAND_CAST] = COLOR_GROUND_DARK;
    queueCommandColors[COMMAND_STOP] = COLOR_FRIENDLY;
    queueCommandColors[COMMAND_ATTACKMOVE] = COLOR_DAMAGE;

    EffectColors[EFFECT_NONE] = COLOR_BG;
    EffectColors[EFFECT_MAXHP] = COLOR_HEAL;
    EffectColors[EFFECT_HURT] = COLOR_DAMAGE;
    EffectColors[EFFECT_HEAL] = COLOR_HEAL;
    EffectColors[EFFECT_POSITION] = COLOR_GROUND_DARK;
    EffectColors[EFFECT_THREAT] = COLOR_FRIENDLY;
    EffectColors[EFFECT_SPEED] = COLOR_FRIENDLY;
    EffectColors[EFFECT_SHIELD] = COLOR_HEAL;
    EffectColors[EFFECT_DAMAGE] = COLOR_DAMAGE;
    EffectColors[EFFECT_INVULN] = COLOR_HEAL;
    EffectColors[EFFECT_CURE] = COLOR_HEAL;

}

Color GetEffectColor(Effect* e)
{
    if (!e) return COLOR_DEFAULT;
    return EffectColors[e->effectType];
}