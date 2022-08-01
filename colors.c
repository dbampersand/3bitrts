#include "colors.h"

ALLEGRO_COLOR GetColor(Color c, int objectOwnedBy)
{
    if (c ==  COLOR_DEFAULT)
    {
        if (objectOwnedBy == 0)
        {
            return FRIENDLY;
        }
        if (objectOwnedBy == 1)
        {
            return ENEMY;
        }
    }
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
    return BG;
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
    
    return COLOR_DEFAULT;
}
void InitColors()
{
    BG = al_map_rgba(24,5,26,255);
    GROUND = al_map_rgba(84,83,172,255);

    ENEMY = al_map_rgba(95,247,247,255);
    FRIENDLY = al_map_rgba(255,255,255,255);

    WHITE = al_map_rgba(242,246,255,255);
  
    HEAL = al_map_rgba(119,249,26,255);
    POISON = al_map_rgba(237,66,229,255);
    DAMAGE = al_map_rgba(250,95,95,255);
}