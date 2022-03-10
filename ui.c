#include "ui.h"
#include "gameobject.h"
#include "colors.h"
#include "player.h"
#include <allegro5/allegro_primitives.h>
void DrawHealthUIElement(GameObject* selected)
{
    float percentHP = selected->health / selected->maxHP;
    int startHPY = 218 + 6;
    int startHPX = 4;
    int hpW = 9;
    int hpH = 26;

    int startY = startHPY + (hpH - (hpH*percentHP));
    int endY = (startHPY + hpH);
    al_draw_filled_rectangle(startHPX, startY, startHPX+hpW,endY, FRIENDLY);


}
void DrawManaUIElement(GameObject* selected)
{
    float percentMana = selected->mana / selected->maxMana;
    int startManaY = 218 + 6;
    int startManaX = 18;
    int manaW = 10;
    int manaH = 26;

    int startY = startManaY + (manaH - (manaH*percentMana));
    int endY = (startManaY + manaH);
    al_draw_filled_rectangle(startManaX, startY, startManaX+manaW,endY, FRIENDLY);


}
void DrawAbilityPortraits(GameObject* selected, int index, int startX, int startY, bool keydown)
{
    if (selected->abilities[index].spriteIndex_Portrait <= 0) 
        return;
    Sprite* s = &sprites[selected->abilities[index].spriteIndex_Portrait];
    //DrawSprite(s,startX,startY,FRIENDLY,keydown);
    Ability* a = &selected->abilities[index];
    float percent;
    if (a->cooldown == 0) 
        percent = 1.0f;
    else
    {
        percent = 1 - (a->cdTimer / a->cooldown);
    }
    int h = al_get_bitmap_height(s->sprite) * percent;
    int w = al_get_bitmap_width(s->sprite);
    if (a->cdTimer > 0)
    {
        keydown = false;
    }
    DrawSpriteRegion(s,0,0,w,h,startX,startY,FRIENDLY,keydown);
}
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    al_draw_filled_rectangle(0, 217, _SCREEN_SIZE, _SCREEN_SIZE, BG);
    
    Sprite* s = &sprites[ui.panel_sprite_index];
    DrawSprite(s,1,218,FRIENDLY,false);

    GameObject* selected = players[0].selection[0];
    if (selected)
    {
        DrawHealthUIElement(selected);
        DrawManaUIElement(selected);
        DrawAbilityPortraits(selected,0,33,220,al_key_down(keyState,ALLEGRO_KEY_Q));
        DrawAbilityPortraits(selected,1,65,220,al_key_down(keyState,ALLEGRO_KEY_W));
        DrawAbilityPortraits(selected,2,97,220,al_key_down(keyState,ALLEGRO_KEY_E));
        DrawAbilityPortraits(selected,3,129,220,al_key_down(keyState,ALLEGRO_KEY_R));

    }
}