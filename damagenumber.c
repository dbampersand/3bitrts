#include "damagenumber.h"
#include <stdlib.h>
#include "helperfuncs.h"
#include <math.h>
#include "ui.h"
#include "allegro5/allegro_ttf.h"
#include "colors.h"
#include <stdio.h>
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
        unsigned char r; unsigned char g; unsigned char b;
        al_unmap_rgb(FRIENDLY,&r,&g,&b);
        ALLEGRO_COLOR color = al_premul_rgba(r,g,b,d->fade*255.0f);
        if (d->text)
        {
            al_draw_text(ui.tinyFont,color,d->pos.x,d->pos.y,ALLEGRO_ALIGN_CENTER,d->text);
        }
    }
}
void UpdateDamageNumbers(float dt)
{
    for (int i = 0; i < MAX_DAMAGE_NUMBERS; i++)
    {
        DamageNumber* d = &damageNumbers[i];
        d->pos.y -= dt*4;
        d->fade -= dt/2.0f;

        if (d->fade <= 0) 
        {
            free(d->text);
            d->text = NULL;
        }
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
    damageNumbers[currDamageNumber++] = (DamageNumber){.text = str, .pos = (Point){x,y}, .fade = 1};
    
    if (currDamageNumber >= MAX_DAMAGE_NUMBERS)
        currDamageNumber = 0;
}