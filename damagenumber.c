#include "damagenumber.h"
#include <stdlib.h>
#include "helperfuncs.h"
#include <math.h>
#include "ui.h"
#include "allegro5/allegro_ttf.h"
#include "colors.h"
void InitDamageNumbers()
{
    memset(damageNumbers,0,sizeof(DamageNumber)*MAX_DAMAGE_NUMBERS);
    currDamageNumber = 0;
}
void DrawDamageNumbers()
{
    for (int i = 0; i < MAX_DAMAGE_NUMBERS; i++)
    {
        DamageNumber* d = &damageNumbers[i];
        if (d->text)
        {
            al_draw_text(ui.font,FRIENDLY,d->pos.x,d->pos.y,ALLEGRO_ALIGN_CENTER,d->text);
        }
    }
}
void UpdateDamageNumbers(float dt)
{
    for (int i = 0; i < MAX_DAMAGE_NUMBERS; i++)
    {
        DamageNumber* d = &damageNumbers[i];
        d->pos.y -= dt*4;
    }
}
void AddDamageNumber(int damage, int x, int y)
{
    //todo: change this to not use log10 as its slowww
    char* str;
    if (damage == 0)
    {
        return;
        str = calloc(2,sizeof(char));
    }
    else
    {
        str = calloc((int)ceil(log10(damage))+2,sizeof(char));
    }

    sprintf(str,"%i",damage);

    if (damageNumbers[currDamageNumber].text)
    {
        free(damageNumbers[currDamageNumber].text);
    }
    damageNumbers[currDamageNumber++] = (DamageNumber){.text = str, .pos = (Point){x,y}};
    
    if (currDamageNumber >= MAX_DAMAGE_NUMBERS)
        currDamageNumber = 0;
}