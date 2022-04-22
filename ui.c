#include "ui.h"
#include "gameobject.h"
#include "colors.h"
#include "player.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <math.h>
#include "video.h"
#include "sprite.h"
void DrawHealthUIElement(GameObject* selected)
{
    float percentHP = selected->health / selected->maxHP;
    int startHPY = UI_START_Y+1+ 6;
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
    int startManaY = UI_START_Y+1 + 6;
    int startManaX = 18;
    int manaW = 10;
    int manaH = 26;

    int startY = startManaY + (manaH - (manaH*percentMana));
    int endY = (startManaY + manaH);
    al_draw_filled_rectangle(startManaX, startY, startManaX+manaW,endY, FRIENDLY);


}
Rect GetAbilityPortraitRect(int index)
{
    Rect r;
    r.w = 30; r.h = 30;
    if (index == 0)
    {
        r.x = 33;
        r.y = 221;
    }
    if (index == 1)
    {
        r.x = 65;
        r.y = 221;
    }
    if (index == 2)
    {
        r.x = 97;
        r.y = 221;
    }
    if (index == 3)
    {
        r.x = 129;
        r.y = 221;
    }
    return r;


}

bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, Rect r, bool keydown, ALLEGRO_MOUSE_STATE* mouseState)
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
    DrawSpriteRegion(s,0,0,w,h,r.x,r.y,FRIENDLY,keydown);

    if (PointInRect(mouseState->x,mouseState->y,r))
    {
        return true;
    }
    return false;
}
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState)
{
    al_draw_filled_rectangle(0, UI_START_Y, _SCREEN_SIZE, _SCREEN_SIZE, BG);
    
    Sprite* s = &sprites[ui.panel_sprite_index];
    DrawSprite(s,1,UI_START_Y+1,FRIENDLY,false);

    GameObject* selected = players[0].selection[players[0].indexSelectedUnit];
    if (selected)
    {
        DrawHealthUIElement(selected);
        DrawManaUIElement(selected);
        Ability* heldAbility = players[0].abilityHeld;
        
        if (DrawAbilityPortraits(selected,heldAbility,0,GetAbilityPortraitRect(0),al_key_down(keyState,ALLEGRO_KEY_Q),mouseState))
        {
            if (selected->abilities[0].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[0].description,100,ui.font,UI_PADDING);
                int x = 33 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[0].description, 5, ui.font, x,y,100);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,1,GetAbilityPortraitRect(1),al_key_down(keyState,ALLEGRO_KEY_W),mouseState))
        {
            if (selected->abilities[1].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[1].description,100,ui.font,UI_PADDING);
                int x = 65 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[1].description, 5, ui.font, x,y,100);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,2,GetAbilityPortraitRect(2),al_key_down(keyState,ALLEGRO_KEY_E),mouseState))
        {
            if (selected->abilities[2].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[2].description,100,ui.font,UI_PADDING);
                int x = 97 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[2].description, 5, ui.font, x,y,100);

            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,3,GetAbilityPortraitRect(3),al_key_down(keyState,ALLEGRO_KEY_R),mouseState))
        {
            if (selected->abilities[3].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[3].description,100,ui.font,UI_PADDING);
                int x = 129 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[3].description, 5, ui.font, x,y,100);
            }

        }
    }
    DrawMenus();
}
void AddElement(Panel* p, UIElement* u)
{
    if (!p->elements)
    {
        p->elements = calloc(1,sizeof(UIElement));
        p->numElements=0;
        p->numElementsAllocated=1;
    }   
    if (p->numElements >= p->numElementsAllocated)
    {
        p->elements = realloc(p->elements,(p->numElementsAllocated+1)*sizeof(UIElement));
        p->numElements = p->numElementsAllocated;
        p->numElementsAllocated++;
    }
    p->elements[p->numElements] = *u;
    p->numElements++;
}
void AddButton(Panel* p, char* name, char* description, int x, int w, int h, int padding, bool shouldLinebreak)
{
    Button* b = calloc(1,sizeof(Button));
    b->description = description;
    b->clicked = false; 
    UIElement u = {0};
    u.data = (void*)b;
    u.w = w;
    u.h = h;
    u.padding = padding;
    u.linebreak = shouldLinebreak;
    u.name = name;
    u.elementType = ELEMENT_BUTTON;
    u.x = x;
    AddElement(p,&u);
}
void InitUI()
{
    ui.mainMenuPanel = (Panel){.elements = 0x0, .numElements = 0,.numElementsAllocated=0, .x = 48,.y=48, .w = 160,.h = 112,.shownPercent=0,.padding=15};
    //AddButton(&ui.mainMenuPanel,"","Return",96,16,15,true,ALIGN_CENTER);
    //AddButton(&ui.mainMenuPanel,"","Options",96,16,15,true,ALIGN_CENTER);
    //AddButton(&ui.mainMenuPanel,"","Exit Game",96,16,15,true,ALIGN_CENTER);
    AddButton(&ui.mainMenuPanel,"Return","Return",80,96,16,15,true);
    AddButton(&ui.mainMenuPanel,"Options","Options",80,96,16,15,true);
    AddButton(&ui.mainMenuPanel,"Exit","Exit Game",80,96,16,15,true);
    
    
    ui.currentPanel = &ui.mainMenuPanel;
}
bool GetButton(Panel* p, char* name)
{
    if (!p) 
        return false;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcmp(u->name,name)==0)
        {
            Button* b = (Button*)u->data;
            return (b->activated);
        }
    }
    return false;
}
void UpdateButton(int rX, int rY, UIElement* u, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    Button* b = (Button*)u->data;
    b->activated = false;
    if (mouseState->buttons & 1 && !(mouseStateLastFrame->buttons & 1))
    {
        Rect r = (Rect){rX,rY,u->w,u->h};
        if (PointInRect(mouseState->x,mouseState->y,r))
        {
            b->clicked = true;
        }
    }
    if (mouseStateLastFrame->buttons & 1 && !(mouseState->buttons & 1))
    {
        Rect r = (Rect){rX,rY,u->w,u->h};
        if (b->clicked)
        {
            if (PointInRect(mouseState->x,mouseState->y,r))
            {
                b->activated = true;
            }
        }
    }
    if (!(mouseState->buttons & 1))
    {
        b->clicked = false;
    }
}
void UpdateElement(Panel* p, UIElement* u, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    int x; int y;
    GetUILocation(p,u,&x,&y);
    if (u->elementType == ELEMENT_BUTTON)
    {
        UpdateButton(x,y,u,mouseState,mouseStateLastFrame);
    }
}
void UpdatePanel(Panel* p, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    if (p)
    {
        for (int i = 0; i < p->numElements; i++)
        {
            UpdateElement(p,&p->elements[i],mouseState,mouseStateLastFrame);
        }
    }

}
void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    if (!al_key_down(keyState,ALLEGRO_KEY_P) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_P))
    {
        if (ui.currentPanel)
        {
            ui.currentPanel = NULL;
        }
        else
        {
            ui.currentPanel = &ui.mainMenuPanel;
        }
    }
    if (ui.currentPanel)
    {
        UpdatePanel(ui.currentPanel,mouseState,mouseStateLastFrame);
    }
}
void DrawButton(UIElement* u, int x, int y)
{
    Button* b = (Button*)u->data;
    ALLEGRO_FONT* font = ui.font;
    if (b->clicked)
    {
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,FRIENDLY);
        al_draw_rectangle(x,y,x+u->w,y+u->h,BG,1);
        al_draw_text(ui.font,BG,x+u->w/2,y+u->h/2 - al_get_font_line_height(font)/2,ALLEGRO_ALIGN_CENTRE,b->description);
    }
    else
    {
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,BG);
        al_draw_rectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
        al_draw_text(ui.font,FRIENDLY,x+u->w/2,y+u->h/2 - al_get_font_line_height(font)/2,ALLEGRO_ALIGN_CENTRE,b->description);

    }

}
void DrawUIElement(UIElement* u, int x, int y)
{
    if (u->elementType == ELEMENT_BUTTON)
    {
        DrawButton(u,x,y);
    }
}
void GetUILocation(Panel* p, UIElement* uF, int* x, int* y)
{
    int currX=p->x+p->padding; int currY=p->y+p->padding; 

    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        currX += u->x - u->w/2 - u->padding;
        if (u == uF)
        {
            *x = currX; 
            *y = currY;
        }
        if (u->linebreak)
        {
            currY += u->h+u->padding;
            currX = p->x+p->padding;
        }
        else
        {
            currX += u->w + u->padding + u->x;
        }
    }

}
void DrawPanel(Panel* p)
{
    al_draw_filled_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,BG);
    al_draw_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,FRIENDLY,1);

    al_set_clipping_rectangle(p->x,p->y,p->w,p->h);
    int currX=p->x+p->padding; int currY=p->y+p->padding; 

    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        int x; int y;
        GetUILocation(p, u, &x, &y);
        DrawUIElement(u,x,y);

    }
    al_reset_clipping_rectangle();


}
void DrawMenus()
{
    if (ui.currentPanel)
        DrawPanel(ui.currentPanel);
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
int GetDescriptionBoxH(char* description, int wTextbox, ALLEGRO_FONT* f, int padding)
{
    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,0,0,0},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    int height = t->h + padding*2;
    free(size);

    return height;
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
    
    Rect r = (Rect){x,y,wTextbox+padding,t->h+padding};
    al_draw_filled_rectangle(r.x-padding,r.y-padding,x+r.w,y+r.h,BG);
    
    r.x-=padding;
    r.y-=padding;
    r.w+=padding;
    r.h+=padding;
    DrawOutlinedRect_Dithered(&r,FRIENDLY);
    
    //al_draw_multiline_text(f,FRIENDLY,x,y,wTextbox,8,ALLEGRO_ALIGN_LEFT,description);
    void* extra = malloc(sizeof(Text));
    memcpy(extra,&(Text){f,x,y},sizeof(Text));
    
    al_do_multiline_text(f,wTextbox,description,cb,extra);
    free(extra);
    free(size);
}
bool IsInsideUI(int x, int y)
{
    if (y >= UI_START_Y)
        return true;
    return false;
}
int GetAbilityClicked(ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    if (PointInRect(mouseState->x,mouseState->y,GetAbilityPortraitRect(0)))
        return 0;
    if (PointInRect(mouseState->x,mouseState->y,GetAbilityPortraitRect(1)))
        return 1;
    if (PointInRect(mouseState->x,mouseState->y,GetAbilityPortraitRect(2)))
        return 2;
    if (PointInRect(mouseState->x,mouseState->y,GetAbilityPortraitRect(3)))
        return 3;

    return -1;

}
