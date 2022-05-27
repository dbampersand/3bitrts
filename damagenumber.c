#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro.h"

#include "damagenumber.h"
#include "helperfuncs.h"
#include "ui.h"
#include "colors.h"
#include "gameobject.h"

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
            unsigned char r; unsigned char g; unsigned char b;
            al_unmap_rgb(*d->color,&r,&g,&b);
            ALLEGRO_COLOR color = al_premul_rgba(r,g,b,d->fade*255.0f);
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
void AddDamageNumber(int damage, int x, int y, GameObject* source)
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
    ALLEGRO_COLOR* color = &FRIENDLY;
    if (source)
        color = IsOwnedByPlayer(source) ? &FRIENDLY : &ENEMY;
    damageNumbers[currDamageNumber++] = (DamageNumber){.text = str, .pos = (Point){x,y}, .fade = 1, .color=color};
    if (currDamageNumber >= MAX_DAMAGE_NUMBERS)
        currDamageNumber = 0;
}