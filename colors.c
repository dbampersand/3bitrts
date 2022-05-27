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
void InitColors()
{
    BG = al_map_rgba(40,32,36,255);
    GROUND = al_map_rgba(115,119,148,255);

    ENEMY = al_map_rgba(60,255,255,255);
    FRIENDLY = al_map_rgba(255,255,255,255);

    WHITE = al_map_rgba(255,255,255,255);
  
    HEAL = al_map_rgba(92,204,12,255);
    POISON = al_map_rgba(195,94,241,255);
    DAMAGE = al_map_rgba(248,100,100,255);
}