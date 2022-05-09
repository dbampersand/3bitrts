#include "ui.h"
#include "gameobject.h"
#include "colors.h"
#include "player.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include <math.h>
#include "video.h"
#include "sprite.h"
#include "encounter.h"
#include "gamestate.h"
#include "map.h"
void ChangeUIPanel(Panel* to)
{
    ui.animatePanel = true;
    ui.changingTo = to;
    ui.animatePanel = UI_ANIMATE_OUT;
}
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
bool DrawAbility(Ability* ability, int x, int y, ALLEGRO_COLOR color, ALLEGRO_MOUSE_STATE* mouse)
{
    Sprite* s = &sprites[ability->spriteIndex_Portrait];
    int w = al_get_bitmap_width(s->sprite);
    int h = al_get_bitmap_height(s->sprite);

    Rect r = (Rect){x,y,w,h};
    if (w > 0 && h > 0)
        al_draw_rectangle(x,y,x+w,y+h,color,1);
    bool shouldInvert = false;

    if (PointInRect(mouse->x,mouse->y,r))
    {
        shouldInvert = true;
    }

    DrawSprite(s,x,y,color,shouldInvert);

    if (shouldInvert)
        return true;
    else
        return false;
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
                DrawDescriptionBox(selected->abilities[0].description, 5, ui.font, x,y,100,0,FRIENDLY);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,1,GetAbilityPortraitRect(1),al_key_down(keyState,ALLEGRO_KEY_W),mouseState))
        {
            if (selected->abilities[1].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[1].description,100,ui.font,UI_PADDING);
                int x = 65 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[1].description, 5, ui.font, x,y,100,0,FRIENDLY);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,2,GetAbilityPortraitRect(2),al_key_down(keyState,ALLEGRO_KEY_E),mouseState))
        {
            if (selected->abilities[2].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[2].description,100,ui.font,UI_PADDING);
                int x = 97 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[2].description, 5, ui.font, x,y,100,0,FRIENDLY);

            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,3,GetAbilityPortraitRect(3),al_key_down(keyState,ALLEGRO_KEY_R),mouseState))
        {
            if (selected->abilities[3].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[3].description,100,ui.font,UI_PADDING);
                int x = 129 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[3].description, 5, ui.font, x,y,100,0,FRIENDLY);
            }

        }
    }
}
void DrawLevelSelect(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    Encounter* e = encounters[selectedEncounterIndex];

    al_draw_text(ui.font,FRIENDLY,16,18,0,"Augment");
    al_draw_text(ui.font,FRIENDLY,84,18,0,"3");

    al_draw_line(10,73,246,73,FRIENDLY,1);
    
    al_draw_text(ui.font,FRIENDLY,16,81,0,"Wyrm");
    DrawSprite(&sprites[e->spriteIndex],17,102,ENEMY,false);

    Ability* mousedOver = NULL;
    mousedOver = DrawAbility(&e->abilities[0], 96, 80, ENEMY, mouseState) == true ? &e->abilities[0] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[1], 136, 80, ENEMY, mouseState) == true ? &e->abilities[1] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[2], 175, 80, ENEMY, mouseState) == true ? &e->abilities[2] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[3], 214, 80, ENEMY, mouseState) == true ? &e->abilities[3] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[4], 96, 134, ENEMY, mouseState) == true ? &e->abilities[4] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[5], 136, 134, ENEMY, mouseState) == true ? &e->abilities[5] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[6], 175, 134, ENEMY, mouseState) == true ? &e->abilities[6] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[7], 214, 134, ENEMY, mouseState) == true ? &e->abilities[7] : mousedOver;


    char* descriptionToDraw;
    if (mousedOver)
    {
        descriptionToDraw = mousedOver->description;
    }
    else
    {
        descriptionToDraw = e->description;
    }
    if (descriptionToDraw)
    {
        DrawDescriptionBox(descriptionToDraw,2,ui.font,16,170,224,41,ENEMY);

    }
    ui.panelShownPercent=1.0f;
    //DrawPanel(&ui.encounter_scroll,mouseState);



    UpdateButton(16,224,&ui.encounter_ButtonLeft,mouseState,mouseStateLastFrame);
    UpdateButton(80,224,&ui.encounter_ButtonConfirm,mouseState,mouseStateLastFrame);
    UpdateButton(192,224,&ui.encounter_ButtonRight,mouseState,mouseStateLastFrame);

    DrawUIElement(&ui.encounter_ButtonLeft,16,224,mouseState,selectedEncounterIndex>0,BG);
    DrawUIElement(&ui.encounter_ButtonConfirm,80,224,mouseState,true,BG);
    DrawUIElement(&ui.encounter_ButtonRight,192,224,mouseState,selectedEncounterIndex+1<numEncounters,BG);

    if (GetButtonIsClicked(&ui.encounter_ButtonLeft))
    {
        selectedEncounterIndex--;
        if (selectedEncounterIndex < 0)
            selectedEncounterIndex = 0;
    }
    if (GetButtonIsClicked(&ui.encounter_ButtonConfirm))
    {
        gameState = CHOOSING_UNITS;
    }
    if (GetButtonIsClicked(&ui.encounter_ButtonRight))
    {
        selectedEncounterIndex++;
        if (selectedEncounterIndex >= numEncounters)
            selectedEncounterIndex = numEncounters-1;

    }
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
Panel CreatePanel(int x, int y, int w, int h, int padding)
{
    Panel p = {0};
    p.x = x;
    p.y = y;
    p.w = w;
    p.h = h;
    p.padding = padding;
    return p;
}
void TabGroup(int numPanels, ...)
{
    va_list argp;
    va_start(argp, numPanels);

    Panel** list = malloc(numPanels*sizeof(Panel*));
    

    for (int i = 0; i < numPanels; i++)
    {
        Panel* tab = va_arg(argp, Panel*);
        list[i] = tab;
    }
    for (int i = 0; i < numPanels; i++)
    {
        if (list[i]->tabs)
        {
            free(list[i]->tabs);
        }
        list[i]->tabs = malloc(numPanels*sizeof(Panel*));
        list[i]->numTabs = numPanels;
        for (int j = 0; j < numPanels; j++)
        {
            list[i]->tabs[j] = list[j];
        }
    }
    free(list);
    va_end(argp);
}
void InitButton(UIElement* u, char* name, char* description, int x, int w, int h, int padding, bool shouldLinebreak)
{
    Button* b = calloc(1,sizeof(Button));
    b->description = description;
    b->clicked = false; 
    u->data = (void*)b;
    u->w = w;
    u->h = h;
    u->padding = padding;
    u->linebreak = shouldLinebreak;
    u->name = name;
    u->elementType = ELEMENT_BUTTON;
    u->x = x;
}
void InitUI()
{
    ui.mainMenuPanel = CreatePanel(48,48,160,112,15);
    AddButton(&ui.mainMenuPanel,"Return","Return",80,96,16,15,true);
    AddButton(&ui.mainMenuPanel,"Options","Options",80,96,16,15,true);
    AddButton(&ui.mainMenuPanel,"Exit","Exit Game",80,96,16,15,true);

    ui.videoOptionsPanel = CreatePanel(48,48,160,112,15);
    AddButton(&ui.videoOptionsPanel,"RenderScale+","+",80,96,16,15,true);
    AddButton(&ui.videoOptionsPanel,"RenderScale-","-",80,96,16,15,true);

    ui.audioOptionsPanel = CreatePanel(48,48,160,112,15);
    AddButton(&ui.audioOptionsPanel,"MasterVolume", "MasterVolume", 80,96,16,15,true);
    AddButton(&ui.audioOptionsPanel,"Music Volume","Music Volume",80,96,16,15,true);

    ui.accessibilityOptionsPanel = CreatePanel(48,48,160,112,15);
    AddButton(&ui.audioOptionsPanel,"MasterVolume", "MasterVolume", 80,96,16,15,true);
    AddButton(&ui.audioOptionsPanel,"Music Volume","Music Volume",80,96,16,15,true);
    TabGroup(3,&ui.videoOptionsPanel,&ui.audioOptionsPanel,&ui.accessibilityOptionsPanel);

    ui.startMenuPanel = CreatePanel(48,48,160,112,15);
    AddButton(&ui.startMenuPanel,"Start Game", "Start Game", 80,96,16,15,true);
    AddButton(&ui.startMenuPanel,"Options", "Options", 80,96,16,15,true);
    AddButton(&ui.startMenuPanel,"End Game", "End Game", 80,96,16,15,true);

    ui.encounter_scroll = CreatePanel(16,224,224,16,0);
    InitButton(&ui.encounter_ButtonLeft,"<","<",0,48,16,0,false);
    InitButton(&ui.encounter_ButtonConfirm,"Select Party","Select Party",0,96,16,0,false);
    InitButton(&ui.encounter_ButtonRight,">",">",0,48,16,0,false);

    InitButton(&ui.choosingUnits_Back,"Back","Back",0,48,16,0,false);
    InitButton(&ui.choosingUnits_GO,"Adventure","Adventure",0,96,16,0,false);



    ui.animatePanel = UI_ANIMATE_STATIC;
    ui.panelShownPercent = 0;
    ui.changingTo = NULL;
    
    ui.currentPanel = NULL;
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

void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, float dt)
{
    if (!al_key_down(keyState,ALLEGRO_KEY_P) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_P))
    {
        if (ui.currentPanel)
        {
            ChangeUIPanel(NULL);
            //ui.currentPanel = NULL;
        }
        else
        {
            ChangeUIPanel(&ui.mainMenuPanel);

           // ui.currentPanel = &ui.mainMenuPanel;
        }
    }
    if (ui.animatePanel != UI_ANIMATE_STATIC)
    {
        if (ui.changingTo)
        {
            if (ui.animatePanel == UI_ANIMATE_IN)
            {
                ui.panelShownPercent += dt;
                if (ui.panelShownPercent >= 1)
                {
                    ui.panelShownPercent = 1;
                    ui.animatePanel = UI_ANIMATE_STATIC;
                    ui.currentPanel = ui.changingTo;
                }
            }
            if (ui.animatePanel == UI_ANIMATE_OUT)
            {
                ui.panelShownPercent -= dt;
                if (ui.panelShownPercent <= 0)
                {
                    ui.panelShownPercent = 0;
                    ui.animatePanel = UI_ANIMATE_IN;
                    ui.currentPanel = ui.changingTo;
                }
            }

        }
    }
    if (ui.currentPanel)
    {
        UpdatePanel(ui.currentPanel,mouseState,mouseStateLastFrame);
    }
}
void DrawButton(UIElement* u, int x, int y, ALLEGRO_MOUSE_STATE* mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    Button* b = (Button*)u->data;
    ALLEGRO_FONT* font = ui.font;
    Rect button = (Rect){x,y,u->w,u->h};
    if (b->clicked && isActive)
    {
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,FRIENDLY);
        al_draw_rectangle(x,y,x+u->w,y+u->h,BG,1);
        al_draw_text(ui.font,BG,x+u->w/2,y+u->h/2 - al_get_font_line_height(font)/2.0,ALLEGRO_ALIGN_CENTRE,b->description);
    }
    else
    {
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,bgColor);
        if (isActive)
        {
            al_draw_rectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
        }
        else
        {
            DrawOutlinedRect_Dithered(&button,FRIENDLY);
        }
        al_draw_text(ui.font,FRIENDLY,x+u->w/2,y+u->h/2 - al_get_font_line_height(font)/2.0,ALLEGRO_ALIGN_CENTRE,b->description);

    }
    if (PointInRect(mouseState->x,mouseState->y,button) && isActive)
    {
        al_draw_rectangle(x+2,y+2,x+u->w-2,y+u->h-2,FRIENDLY,1);
    }

}
void DrawUIElement(UIElement* u, int x, int y, ALLEGRO_MOUSE_STATE* mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    if (u->elementType == ELEMENT_BUTTON)
    {
        DrawButton(u,x,y,mouseState,isActive,bgColor);
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
    if (p->tabs)
    {
        *x += 25;
    }

}
void DrawPanel(Panel* p, ALLEGRO_MOUSE_STATE* mouseState)
{
    al_set_clipping_rectangle(p->x-1,p->y,p->w+1,p->h*ui.panelShownPercent+1);

    al_draw_filled_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,BG);
    al_draw_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,FRIENDLY,1);

    int currX=p->x+p->padding; int currY=p->y+p->padding; 
    for (int i = 0; i < p->numTabs; i++)
    {
        int x = p->x+p->padding;
        int y = p->y*25*i+p->padding;
        al_draw_rectangle(x,y,x+25,y+25,FRIENDLY,1);
    }
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        int x; int y;
        GetUILocation(p, u, &x, &y);
        DrawUIElement(u,x,y,mouseState,true,BG);

    }
    al_reset_clipping_rectangle();


}
void AddPanelTabs(Panel* p, int numTabs, ...)
{
    va_list argp;
    va_start(argp, numTabs);
    if (!p->tabs)
    {
        p->tabs = malloc(numTabs * sizeof(Panel*));
        p->numTabs = 0;
    }
    else
    {
        p->tabs = realloc(p->tabs,sizeof(Panel*)*(p->numTabs+numTabs));
    }
    for (int i = 0; i < numTabs; i++)
    {
        Panel* tab = va_arg(argp, Panel*);
        p->tabs[i+p->numTabs] = tab;
    }
    p->numTabs = numTabs + p->numTabs;

    va_end(argp);
}
void DrawMenus(ALLEGRO_MOUSE_STATE* mouseState)
{
    if (ui.currentPanel)
        DrawPanel(ui.currentPanel,mouseState);
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
bool GetButtonIsClicked(UIElement* u)
{
    if (u->elementType == ELEMENT_BUTTON)
    {
        Button* b = (Button*)(u->data);
        return b->activated;
    }
    return false;
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
    ALLEGRO_COLOR color;
    int lineHeight;
}Text;
bool cb(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    int x = t->x;
    int y = t->y;
    ALLEGRO_FONT* f = t->f;
    int height = t->lineHeight;
    y += line_num*height;

    char* buff = calloc(size+1,sizeof(char));
    memcpy(buff,line,size*sizeof(char));
    al_draw_text(f,t->color,x,y,ALLEGRO_ALIGN_LEFT,buff);
    free(buff);
    return true;
}
bool CB_GetHeight(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    ALLEGRO_FONT* f = t->f;
    t->h += t->lineHeight;
    return true;
}
int GetDescriptionBoxH(char* description, int wTextbox, ALLEGRO_FONT* f, int padding)
{
    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,0,0,0,FRIENDLY,al_get_font_line_height(f)+2},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    int height = t->h + padding*2;
    free(size);

    return height;
}
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, int x, int y, int wTextbox, int minH, ALLEGRO_COLOR color)
{
    if (!description) return;
    int w;  
    int h;
    int xoffset;
    int yoffset;
    int lineHeight = al_get_font_line_height(f)+2;

    //al_get_text_dimensions(f,description,&xoffset,&yoffset,&w,&h);
    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,x,y,0,color,lineHeight},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    t->color = color;
    
    Rect r = (Rect){x,y,wTextbox+padding,t->h+padding};
    r.h = r.h > minH ? r.h : minH;
    al_draw_filled_rectangle(r.x-padding,r.y-padding,x+r.w,y+r.h,BG);
    
    r.x-=padding;
    r.y-=padding;
    r.w+=padding;
    r.h+=padding;
    DrawOutlinedRect_Dithered(&r,color);
    
    //al_draw_multiline_text(f,FRIENDLY,x,y,wTextbox,8,ALLEGRO_ALIGN_LEFT,description);
    void* extra = malloc(sizeof(Text));
    memcpy(extra,&(Text){.f=f,.x=x,.y=y,.color=color,.lineHeight=lineHeight},sizeof(Text));
    
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
