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