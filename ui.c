#include "ui.h"
#include "gameobject.h"
#include "colors.h"
#include "player.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

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
bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, int startX, int startY, bool keydown, ALLEGRO_MOUSE_STATE* mouseState)
{
    if (selected->abilities[index].spriteIndex_Portrait <= 0) 
        return false;
    if (heldAbility == &selected->abilities[index])
        keydown = true;
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

    if (PointInRect(mouseState->x,mouseState->y,(Rect){startX,startY,w,h}))
    {
        return true;
    }
    return false;
}
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState)
{
    al_draw_filled_rectangle(0, 217, _SCREEN_SIZE, _SCREEN_SIZE, BG);
    
    Sprite* s = &sprites[ui.panel_sprite_index];
    DrawSprite(s,1,218,FRIENDLY,false);

    GameObject* selected = players[0].selection[players[0].indexSelectedUnit];
    if (selected)
    {
        DrawHealthUIElement(selected);
        DrawManaUIElement(selected);
        Ability* heldAbility = players[0].abilityHeld;
        
        if (DrawAbilityPortraits(selected,heldAbility,0,33,221,al_key_down(keyState,ALLEGRO_KEY_Q),mouseState))
        {
            DrawDescriptionBox("Test\nThis ability does stuff\nand also does this!", 5, ui.font, 5,64,100);

        }
        if (DrawAbilityPortraits(selected,heldAbility,1,65,221,al_key_down(keyState,ALLEGRO_KEY_W),mouseState))
        {

        }
        if (DrawAbilityPortraits(selected,heldAbility,2,97,221,al_key_down(keyState,ALLEGRO_KEY_E),mouseState))
        {

        }
        if (DrawAbilityPortraits(selected,heldAbility,3,129,221,al_key_down(keyState,ALLEGRO_KEY_R),mouseState))
        {
            
        }
    }
}
void LoadCursorSprite(UI* ui, int* index, char* path)
{
    *index = LoadSprite(path,false);    
    if (*index == -1)
        return;
    sprites[*index].inverseSprite = al_clone_bitmap(sprites[*index].sprite);
    ALLEGRO_BITMAP* inverse = sprites[*index].inverseSprite;

    int w = al_get_bitmap_width(inverse);
    int h = al_get_bitmap_height(inverse);


    al_lock_bitmap(inverse,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    al_set_target_bitmap(inverse);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            
            ALLEGRO_COLOR pixel = al_get_pixel(sprites[*index].sprite, x, y);
            if (!pixel.a)
            {
                for (int y2 = -1; y2 < 2; y2++)
                {
                    for (int x2 = -1; x2 < 2; x2++)
                    {
                        if (x2 == 0 && y2 == 0)
                            continue;
                        ALLEGRO_COLOR pixel2 = al_get_pixel(sprites[*index].sprite, x+x2, y+y2);
                        if (pixel2.a)
                        {
                            al_put_pixel(x,y,WHITE);
                        }
                    }
                }

            }
        }
    }

    al_unlock_bitmap(inverse);

}

void DrawCursor(ALLEGRO_MOUSE_STATE* mouseState, int index, bool clicked)
{
    if (mouseState->buttons & 1 || mouseState->buttons & 2)
    {
        DrawSprite(&sprites[index],mouseState->x,mouseState->y,FRIENDLY,true);
        DrawSprite(&sprites[index],mouseState->x,mouseState->y,BG,false);

    }
    else
    {
        DrawSprite(&sprites[index],mouseState->x,mouseState->y,BG,true);
        DrawSprite(&sprites[index],mouseState->x,mouseState->y,FRIENDLY,false);
    }


}
typedef struct Text
{
    ALLEGRO_FONT* f; 
    int x; 
    int y;
    int h; 
}Text;
bool cb(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    int x = t->x;
    int y = t->y;
    ALLEGRO_FONT* f = t->f;
    int height = al_get_font_line_height(f);
    y += line_num*height;

    char* buff = calloc(size+1,sizeof(char));
    memcpy(buff,line,size*sizeof(char));
    al_draw_text(f,WHITE,x,y,ALLEGRO_ALIGN_LEFT,buff);
    free(buff);
    return true;
}
bool CB_GetHeight(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    ALLEGRO_FONT* f = t->f;
    t->h += al_get_font_line_height(f);
    return true;
}
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, int x, int y, int wTextbox)
{
    if (!description) return;
    int w;  
    int h;
    int xoffset;
    int yoffset;

    //al_get_text_dimensions(f,description,&xoffset,&yoffset,&w,&h);
    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,x,y,0},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    
    al_draw_filled_rectangle(x-padding,y-padding,x+wTextbox+padding,y+t->h+padding,ENEMY);
    //al_draw_multiline_text(f,FRIENDLY,x,y,wTextbox,8,ALLEGRO_ALIGN_LEFT,description);
    void* extra = malloc(sizeof(Text));
    memcpy(extra,&(Text){f,x,y},sizeof(Text));
    
    al_do_multiline_text(f,wTextbox,description,cb,extra);
    free(extra);
    free(size);
}