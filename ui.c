#include <stdio.h>
#include <math.h>

#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "ui.h"

#include "gameobject.h"
#include "colors.h"
#include "player.h"
#include "video.h"
#include "sprite.h"
#include "encounter.h"
#include "gamestate.h"
#include "map.h"
#include "effect.h"
#include "helperfuncs.h"
#include "sound.h"
#include "luafuncs.h"

void GetAbilityClickedInsideUI(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    if (mouseStateLastFrame->buttons & 1)
    if (currGameObjRunning)
    {
        players[0].amoveSelected = false;
        int index = GetAbilityClicked(mouseState,mouseStateLastFrame);
        if (index != -1)
        {
            Ability* a = &currGameObjRunning->abilities[index];
            if (AbilityIsCastImmediately(a))
            {
                float x; float y; 
                GetCentre(currGameObjRunning,&x,&y);

                currAbilityRunning = a; 
                players[0].abilityHeld = NULL;
                CastAbility(currGameObjRunning,a,x,y,x,y,NULL);

            }
            else
            {
                players[0].abilityHeld = &currGameObjRunning->abilities[index]; 
                currAbilityRunning =  &currGameObjRunning->abilities[index];
            }
        }  
    }

}
void DrawMouse(ALLEGRO_MOUSE_STATE* mouseState, GameObject* mousedOver)
{
    if (players[0].abilityHeld)
    {
        DrawCursor(mouseState, ui.cursorCastingIndex, false);
    }
    else if (mousedOver)
    {
        if (mousedOver->properties & OBJ_OWNED_BY && players[0].numUnitsSelected > 0)
            DrawCursor(mouseState, ui.cursorAttackIndex,false);
        else if (!(mousedOver->properties & OBJ_OWNED_BY))
            DrawCursor(mouseState, ui.cursorFriendlyIndex, false);
        else
            DrawCursor(mouseState, ui.cursorDefaultIndex, false);

    }
    else if (players[0].amoveSelected)
    {
        DrawCursor(mouseState, ui.cursorAttackIndex,false);
    }
    else 
    {
        DrawCursor(mouseState, ui.cursorDefaultIndex, false);
    }

}
//float angle;
//ALLEGRO_BITMAP* wheel;
void DrawUnitChoiceUI(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
        //angle += 0.1f;

        //if (!wheel)
         //   wheel = al_load_bitmap("assets/decor/wheel.png");
       // al_draw_rotated_bitmap(wheel,41/2,41/2,50,50,DegToRad(angle),0);
        
        Rect selectedUnitsR = (Rect){8,146,240,41};
        Encounter* e = encounters[selectedEncounterIndex];

        int numUnitsInRect = GetNumObjectsInRect(&selectedUnitsR);

        UpdateButton(45,194,&ui.choosingUnits_Back,mouseState,mouseStateLastFrame);
        UpdateButton(109,194,&ui.choosingUnits_GO,mouseState,mouseStateLastFrame);


        DrawUIElement(&ui.choosingUnits_Back,45,194,mouseState,true,BG);
        DrawUIElement(&ui.choosingUnits_GO,109,194,mouseState,numUnitsInRect==e->numUnitsToSelect,BG);


        if (numUnitsInRect != e->numUnitsToSelect)
        {
            DrawOutlinedRect_Dithered(&selectedUnitsR,FRIENDLY);
        }
        else if (numUnitsInRect == e->numUnitsToSelect)
        {
            al_draw_rectangle(selectedUnitsR.x,selectedUnitsR.y,selectedUnitsR.x+selectedUnitsR.w,selectedUnitsR.y+selectedUnitsR.h,FRIENDLY,1);
        }
                char* number = calloc(log10(INT_MAX)*2+2,sizeof(char));
        sprintf(number,"%i/%i",numUnitsInRect,e->numUnitsToSelect);

        al_draw_text(ui.font,FRIENDLY,202,162,ALLEGRO_ALIGN_LEFT,number);

        free(number);

}
void DrawMouseSelectBox(ALLEGRO_MOUSE_STATE mouseState)
{
    Vector2 endSelection = (Vector2){mouseState.x,mouseState.y};
    Rect r;
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    if (al_key_down(&keyState, ALLEGRO_KEY_Q))
    {

    }
    r.x = _MIN(endSelection.x,players[0].selectionStart.x);
    r.y = _MIN(endSelection.y,players[0].selectionStart.y);
    r.w = _MAX(endSelection.x,players[0].selectionStart.x) - _MIN(endSelection.x,players[0].selectionStart.x);
    r.h = _MAX(endSelection.y,players[0].selectionStart.y) - _MIN(endSelection.y,players[0].selectionStart.y);
    al_draw_filled_rectangle(r.x, r.y, r.x+r.w, r.y+r.h, al_premul_rgba(255, 255, 255,128));
   
}

void UpdateInterface(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    UpdateUI(keyState,mouseState,keyStateLastFrame,mouseStateLastFrame,dt);
    if (ui.currentPanel == &ui.mainMenuPanel)
    {
        if (GetButton(&ui.mainMenuPanel,"Return"))
        {
            ui.currentPanel = NULL;
            if (gameState == GAMESTATE_MAIN_MENU)
            {
                gameState = GAMESTATE_CHOOSING_ENCOUNTER;
                StopMusic();
                combatStarted = false;
                ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");
            }
        }
        if (GetButton(&ui.mainMenuPanel,"Options"))
        {
            ChangeUIPanel(&ui.videoOptionsPanel);
        }
        if (GetButton(&ui.mainMenuPanel,"Exit"))
        {
            Quit();
        }
    }
    SetOptions();
}

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
void DrawEffectPortrait(int x, int y, Effect* e, ALLEGRO_COLOR c)
{
    if (e->spriteIndex_Portrait > 0 && e->enabled)
    {
        DrawSprite(&sprites[e->spriteIndex_Portrait],x,y,FRIENDLY,false);
    }
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
    if (!s) return;
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
                DrawDescriptionBox(selected->abilities[0].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,1,GetAbilityPortraitRect(1),al_key_down(keyState,ALLEGRO_KEY_W),mouseState))
        {
            if (selected->abilities[1].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[1].description,100,ui.font,UI_PADDING);
                int x = 65 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[1].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,2,GetAbilityPortraitRect(2),al_key_down(keyState,ALLEGRO_KEY_E),mouseState))
        {
            if (selected->abilities[2].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[2].description,100,ui.font,UI_PADDING);
                int x = 97 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[2].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY);

            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,3,GetAbilityPortraitRect(3),al_key_down(keyState,ALLEGRO_KEY_R),mouseState))
        {
            if (selected->abilities[3].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[3].description,100,ui.font,UI_PADDING);
                int x = 129 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[3].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY);
            }

        }
        DrawEffectPortrait(162,223,&selected->effects[0],FRIENDLY);
        DrawEffectPortrait(178,223,&selected->effects[1],FRIENDLY);
        DrawEffectPortrait(194,223,&selected->effects[2],FRIENDLY);
        DrawEffectPortrait(209,223,&selected->effects[3],FRIENDLY);
        DrawEffectPortrait(225,223,&selected->effects[4],FRIENDLY);
        DrawEffectPortrait(241,223,&selected->effects[5],FRIENDLY);

        DrawEffectPortrait(162,241,&selected->effects[6],FRIENDLY);
        DrawEffectPortrait(178,241,&selected->effects[7],FRIENDLY);
        DrawEffectPortrait(194,241,&selected->effects[8],FRIENDLY);
        DrawEffectPortrait(209,241,&selected->effects[9],FRIENDLY);
        DrawEffectPortrait(225,241,&selected->effects[10],FRIENDLY);
        DrawEffectPortrait(241,241,&selected->effects[11],FRIENDLY);
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
        DrawDescriptionBox(descriptionToDraw,2,ui.font,ui.boldFont,16,170,224,41,ENEMY);

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
        //gameState = GAMESTATE_CHOOSING_UNITS;
        SetGameStateToChoosingParty();
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
void AddButton(Panel* p, char* name, char* description, int x, int y, int w, int h)
{
    Button* b = calloc(1,sizeof(Button));
    b->description = calloc(strlen(description)+1,sizeof(char));
    strcpy(b->description,description);
    b->clicked = false; 
    UIElement u = {0};
    u.data = (void*)b;
    u.w = w;
    u.h = h;
    u.y = y;
    u.name = name;
    u.elementType = ELEMENT_BUTTON;
    u.x = x;
    u.enabled = true;

    AddElement(p,&u);
}
void UpdateSlider(Slider* s, int x, int y, int w, int h, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    Rect r = (Rect){x,y,w,h};
    if (mouseState->buttons & 1 && !(mouseStateLastFrame->buttons & 1))
    {
        if (PointInRect(mouseState->x,mouseState->y,r))
        {
            s->clicked = true;
        }
    }
    if (!(mouseState->buttons & 1))
    {
        s->clicked = false;
    }

    if (s->clicked)
    {
        float v = 1-((x+w) - mouseState->x) / (float)w;
        if (v>1)
            v = 1;
        if (v<0)
            v = 0;
        s->value = v;
    }
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
        InitButton(&list[i]->tabButton, "Tab", "", 0,0, 14, 33, 0);

        for (int j = 0; j < numPanels; j++)
        {
            list[i]->tabs[j] = list[j];
        }
    }
    free(list);
    va_end(argp);
}
void InitButton(UIElement* u, char* name, char* description, int x, int y, int w, int h, int sprite)
{
    Button* b = calloc(1,sizeof(Button));

    b->description = calloc(strlen(description)+1,sizeof(char));
    strcpy(b->description,description);

    b->clicked = false; 
    b->spriteIndex = sprite;
    u->data = (void*)b;
    u->w = w;
    u->h = h;
    u->name = name;
    u->elementType = ELEMENT_BUTTON;
    u->x = x;
    u->y = y;
}
void ChangeButtonText(Button* b, char* newstr)
{
    if (!b)
    {
        return;
    }
    if (b->description)
    {
        free(b->description);
    }
    b->description = calloc(strlen(newstr)+1,sizeof(char));
    strcpy(b->description,newstr);
}
void AddText(Panel* p,int x, int y, char* name, char* description)
{
    UIElement u = {0};
    UI_Text* t = calloc(1,sizeof(UI_Text));

    t->str = calloc(strlen(description)+1,sizeof(char));
    strcpy(t->str,description);
    u.data = (void*)t;

    u.x = x; 
    u.y = y;
    u.elementType = ELEMENT_TEXT;
    u.name = name;
    AddElement(p,&u);
}
Button* GetButtonB(Panel* p, char* name)
{
    if (!p) 
        return NULL;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            Button* b = (Button*)u->data;
            return b;
        }
    }
    return NULL;
}
void DrawSlider(UIElement* u, int x, int y, ALLEGRO_MOUSE_STATE* mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    Slider* s = (Slider*)u->data;
    al_draw_rectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
    float w = u->w * s->value;
    al_draw_filled_rectangle(x,y,x+w,y+u->h,FRIENDLY);
}
void UpdateCheckbox(Checkbox* c, int x, int y, int w, int h, bool isActive, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    if (!isActive)
        return;
    if (mouseState->buttons & 1 && !(mouseStateLastFrame->buttons & 1))
    {
        Rect r = (Rect){x,y,w,h};
        if (PointInRect(mouseState->x,mouseState->y,r))
        {
            c->clicked = true;
        }
    }
    if (mouseStateLastFrame->buttons & 1 && !(mouseState->buttons & 1))
    {
        Rect r = (Rect){x,y,w,h};
        if (c->clicked)
        {
            if (PointInRect(mouseState->x,mouseState->y,r))
            {
                c->activated = !c->activated;
            }
        }
    }
    if (!(mouseState->buttons & 1))
    {
        c->clicked = false;
    }

}
void DrawCheckbox(Checkbox* c, int x, int y, int w, int h, bool isActive,ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_COLOR bgColor)
{
    Rect checkbox = (Rect){x,y,w,h};
    if (c->activated && isActive)
    {
        al_draw_filled_rectangle(x,y,x+w,y+h,FRIENDLY);
        al_draw_rectangle(x,y,w,y+h,BG,1);
    }
    else
    {
        al_draw_filled_rectangle(x,y,x+w,y+h,bgColor);
        if (isActive)
        {
            al_draw_rectangle(x,y,x+w,y+h,FRIENDLY,1);
        }
        else
        {
            DrawOutlinedRect_Dithered(&checkbox,FRIENDLY);
        }

    }
}
void AddCheckbox(Panel* p, int x, int y, int w, int h, char* name, bool activated)
{
    Checkbox* c = calloc(1,sizeof(Checkbox));
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    c->activated = activated;
    u.name = name;
    u.data = (void*)c;
    u.enabled = true;
    u.elementType = ELEMENT_CHECKBOX;
    u.enabled = true;
    AddElement(p,&u);
}
void AddSlider(Panel* p, int x, int y, int w, int h, char* name, float filled)
{
    Slider* s = calloc(1,sizeof(Slider));
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    u.enabled = true;
    s->value = filled;
    u.name = name;
    u.data = (void*)s;
    u.elementType = ELEMENT_SLIDER;
    AddElement(p,&u);
}
void AddPulldownMenu(Panel* panel, int x, int y, int w, int h, char* name, int startIndex, int numElements, ...)
{
    va_list argp;
    va_start(argp, numElements);
    char** list = calloc(numElements,sizeof(char*)); 
    for (int i = 0; i < numElements; i++)
    {
        char* element = va_arg(argp, char*);
        char* new = calloc(strlen(element)+1,sizeof(char));
        strcpy(new,element);
        list[i] = new;
    }
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    u.enabled = true;
    u.name = name;

    
    Pulldown* p = calloc(1,sizeof(Pulldown));
    p->clicked = false;
    p->elements = list;
    p->selectedIndex = startIndex;
    p->numElements = numElements;

    u.data = (void*)p;
    u.elementType = ELEMENT_PULLDOWN;

    AddElement(panel,&u);

}
void UpdatePulldownMenu(Pulldown* p, int x, int y, int w, int h, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    if (p->clicked)
    {
        if (mouseState->buttons & 1 && !(mouseStateLastFrame->buttons & 1))
        {
            int y2 = y;
            for (int i = 0; i < p->numElements; i++)
            {
                Rect r = (Rect){x,y2,w,h};
                if (PointInRect(mouseState->x,mouseState->y,r))
                {
                    p->clicked = false;
                    p->selectedIndex = i;
                    return;
                }
                y2 += h;
            }

        }
    } 

    if (mouseState->buttons & 1 && !(mouseStateLastFrame->buttons & 1))
    {
        Rect r = (Rect){x,y,w,h};
        if (PointInRect(mouseState->x,mouseState->y,r))
        {
            p->clicked = true;
        }
        else
        {
            p->clicked = false; 
        }
    }
}
void DrawPullDownMenu(Pulldown* p, int x, int y, int w, int h, bool isActive, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_COLOR bgColor)
{
    Rect clip;
    al_get_clipping_rectangle(&clip.x,&clip.y,&clip.w,&clip.h);
    Rect r = (Rect){x,y,w,h};
    if (p->clicked && isActive)
    {
        int y2 = y;
        al_reset_clipping_rectangle();
        for (int i = 0; i < p->numElements; i++)
        {
            al_draw_filled_rectangle(x,y2,x+w,y2+h,bgColor);
            al_draw_rectangle(x,y2,x+w,y2+h,FRIENDLY,1);
            al_draw_text(ui.font,FRIENDLY,x+w/2,y2+h/2 - al_get_font_line_height(ui.font)/2.0,ALLEGRO_ALIGN_CENTRE,p->elements[i]);
            Rect r2 = (Rect){x,y2,w,h};
            if (PointInRect(mouseState->x,mouseState->y,r2))
            {
                al_draw_rectangle(x+2,y2+2,x+w-2,y2+h-2,FRIENDLY,1);
            }
            y2 += h;

        }
        //al_draw_text(ui.font,BG,x+w/2,y+h/2 - al_get_font_line_height(font)/2.0,ALLEGRO_ALIGN_CENTRE,b->description);
    }
    if (!p->clicked)
    {
        al_draw_filled_rectangle(x,y,x+w,y+h,bgColor);
        al_draw_rectangle(x,y,x+w,y+h,FRIENDLY,1);
        al_draw_text(ui.font,FRIENDLY,x+w/2,y+h/2 - al_get_font_line_height(ui.font)/2.0,ALLEGRO_ALIGN_CENTRE,p->elements[p->selectedIndex]);
        if (PointInRect(mouseState->x,mouseState->y,r) && isActive)
        {
            al_draw_rectangle(x+2,y+2,x+w-2,y+h-2,FRIENDLY,1);
        }

    }
    al_set_clipping_rectangle(clip.x,clip.y,clip.w,clip.h);
}
void DrawScrollbar(Panel* p)
{
    int w = SCROLLBARW;
    al_draw_rectangle(p->x+p->w-w,p->y,p->x+p->w,p->x+p->w,FRIENDLY,1);


    float yPos = p->y + p->scrollPercent * p->h;
    al_draw_filled_rectangle(p->x+p->w-w, yPos, p->x+p->w, yPos+14, FRIENDLY);

}
void UpdateScrollbar(Panel* p, ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    int w = SCROLLBARW;
    Rect scrollbar = (Rect){p->x+p->w-w,p->y,w,p->h};
    if (mouseState->buttons & 1 && !(mouseStateLastFrame->buttons & 1))
    {
        if (PointInRect(mouseState->x,mouseState->y,scrollbar))
        {
            p->scrollbarClicked = true;
        }
    }
    if (!(mouseState->buttons & 1))
    {
        p->scrollbarClicked = false;
    }
    if (p->scrollbarClicked)
    {
        float v = 1-((p->y+p->h) - mouseState->y) / (float)p->h;
        if (v>1)
            v = 1;
        if (v<0)
            v = 0;

        p->scrollPercent = v;
    }
}
void SetUITextStr(UI_Text* t, char* str)
{
    if (t->str)
        free(t->str);
    t->str = calloc(strlen(str)+1,sizeof(char));
    strcpy(t->str,str);
}
UIElement* GetUIElement(Panel* p, char* name)
{
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            return u;
        }
    }
    return NULL;
}
void InitUI()
{
    ui.mainMenuPanel = CreatePanel(48,48,160,112,15);
    AddButton(&ui.mainMenuPanel,"Return","Return",33,17,96,16);
    AddButton(&ui.mainMenuPanel,"Options","Options",33,49,96,16);
    AddButton(&ui.mainMenuPanel,"Exit","Exit Game",33,81,96,16);

    ui.videoOptionsPanel = CreatePanel(48,48,160,112,15);
    AddText(&ui.videoOptionsPanel,33,41,"Tag_RenderScale","RenderScale");
    AddText(&ui.videoOptionsPanel,132,43,"RenderScale","2x");
    
    char* renderScale = calloc(NumDigits(_RENDERSIZE)+2,sizeof(char));
    sprintf(renderScale,"%ix",_RENDERSIZE);
    SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,renderScale);
    free(renderScale);
    
    AddButton(&ui.videoOptionsPanel,"RenderScale+","+",132,29,11,11);
    AddButton(&ui.videoOptionsPanel,"RenderScale-","-",132,53,11,11);
    AddText(&ui.videoOptionsPanel,33,73,"Tag_Particles","Particles");
    AddCheckbox(&ui.videoOptionsPanel,131,72,13,13,"EnableParticles",true);
    AddText(&ui.videoOptionsPanel,33,105,"Display\nHealth Bar","Display\nHealth Bar");
    AddPulldownMenu(&ui.videoOptionsPanel,97,108,48,13,"HealthBarDisplay",0,3,"Always","Selected","Never");

    ui.audioOptionsPanel = CreatePanel(48,48,160,112,15);
    AddText(&ui.audioOptionsPanel,33,41,"Tag_MasterVolume","Master Volume");
    AddSlider(&ui.audioOptionsPanel,34,52,110,10,"MasterVolume",0.35f);


    ui.accessibilityOptionsPanel = CreatePanel(48,48,160,112,15);
    //AddButton(&ui.audioOptionsPanel,"MasterVolume", "MasterVolume", 132,29,96,16);
    //AddButton(&ui.audioOptionsPanel,"Music Volume","Music Volume",132,29,96,16);
    TabGroup(3,&ui.videoOptionsPanel,&ui.audioOptionsPanel,&ui.accessibilityOptionsPanel);


    ui.encounter_scroll = CreatePanel(16,224,224,16,0);
    InitButton(&ui.encounter_ButtonLeft,"<","<",0,224,48,16,0);
    InitButton(&ui.encounter_ButtonConfirm,"Select Party","Select Party",0,224,96,16,0);
    InitButton(&ui.encounter_ButtonRight,">",">",0,224,48,16,0);

    InitButton(&ui.choosingUnits_Back,"Back","Back",45,194,48,16,0);
    InitButton(&ui.choosingUnits_GO,"Adventure","Adventure",109,194,96,16,0);

    /*ui.mainMenuPanel.spriteIndex_tabIcon = LoadSprite("assets/ui/back_tab_icon.png",true);
    ui.videoOptionsPanel.spriteIndex_tabIcon = LoadSprite("assets/ui/video_tab_icon.png",true);
    ui.audioOptionsPanel.spriteIndex_tabIcon = LoadSprite("assets/ui/audio_tab_icon.png",true);
    ui.accessibilityOptionsPanel.spriteIndex_tabIcon = LoadSprite("assets/ui/accessiblity_tab_icon.png",true);
*/
    InitButton(&ui.videoOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.audioOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.accessibilityOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));


    InitButton(&ui.videoOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/video_tab_icon.png",true));
    InitButton(&ui.audioOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/audio_tab_icon.png",true));
    InitButton(&ui.accessibilityOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/accessiblity_tab_icon.png",true));

    ui.videoOptionsPanel.back = &ui.mainMenuPanel;
    ui.audioOptionsPanel.back = &ui.mainMenuPanel;
    ui.accessibilityOptionsPanel.back = &ui.mainMenuPanel;


    ui.animatePanel = UI_ANIMATE_STATIC;
    ui.panelShownPercent = 0;
    ui.changingTo = NULL;
    
    ui.currentPanel = NULL;

    Checkbox* particles = (Checkbox*)GetUIElement(&ui.videoOptionsPanel,"EnableParticles")->data;
    gameOptions.particlesEnabled = &particles->activated;

    Pulldown* healthbar = (Pulldown*)GetUIElement(&ui.videoOptionsPanel,"HealthBarDisplay")->data;
    gameOptions.displayHealthBar = (Option_HealthBar*)&healthbar->selectedIndex;

    ui.panel_sprite_index = LoadSprite("assets/ui/ui.png",false);

    ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Start");
    ui.panelShownPercent = 1.0f;


    LoadCursorSprite(&ui,&ui.cursorDefaultIndex,"assets/ui/cursor.png");
    LoadCursorSprite(&ui,&ui.cursorCastingIndex,"assets/ui/cursor_cast.png");
    LoadCursorSprite(&ui,&ui.cursorAttackIndex,"assets/ui/cursor_attack.png");
    LoadCursorSprite(&ui,&ui.cursorFriendlyIndex,"assets/ui/cursor_friendly.png");


    InitFonts();

    ui.currentPanel = &ui.mainMenuPanel;
    ui.panelShownPercent=1.0f;
    ui.animatePanel = UI_ANIMATE_STATIC;

}
void InitFonts()
{
    ui.font = al_load_ttf_font("assets/fonts/font.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.boldFont = al_load_ttf_font("assets/fonts/fontbold.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.tinyFont = al_load_ttf_font("assets/fonts/4x8.ttf", 8, ALLEGRO_TTF_MONOCHROME);


}
bool GetButton(Panel* p, char* name)
{
    if (!p) 
        return false;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            Button* b = (Button*)u->data;
            return (b->activated);
        }
    }
    return false;
}
void UpdateTabButtons(Panel* p, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    int y = p->y; 
    int x = p->x;

    if (p->backButton.data)
    {
        Button* b = (Button*)p->backButton.data;

        UpdateButton(x,y,&p->backButton,mouseState,mouseStateLastFrame);
        if (b->activated)
        {
            ChangeUIPanel(p->back);
        }

        y+=p->backButton.h;
    }
    for (int i = 0; i < p->numTabs; i++)
    {
        Panel* tab = p->tabs[i];
        Button* b = (Button*)tab->tabButton.data;
        int w; int h; 
        h = al_get_bitmap_height(sprites[b->spriteIndex].sprite);
        w = al_get_bitmap_width(sprites[b->spriteIndex].sprite);
        UpdateButton(x,y,&tab->tabButton,mouseState,mouseStateLastFrame);
        if (b->activated)
        {
            ChangeUIPanel(tab);
        }
        y += h;
    }
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
    if (!u->enabled)
        return;
    int x; int y;
    GetUILocation(p,u,&x,&y);
   
    if (u->elementType == ELEMENT_BUTTON)
    {
        UpdateButton(x,y,u,mouseState,mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_SLIDER)
    {
        UpdateSlider((Slider*)u->data, x,  y, u->w,u->h,mouseState, mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_CHECKBOX)
    {
        UpdateCheckbox((Checkbox*)u->data, x,  y, u->w,u->h,true,mouseState, mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_PULLDOWN)
    {
        UpdatePulldownMenu((Pulldown*)u->data, x,  y, u->w,u->h,mouseState, mouseStateLastFrame);
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
    UpdateScrollbar(p,mouseState,mouseStateLastFrame);

}

void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, float dt)
{
    if (ui.currentPanel)
    {
        UpdateTabButtons(ui.currentPanel, mouseState,mouseStateLastFrame);
    }

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
                ui.panelShownPercent += dt*4;
                if (ui.panelShownPercent >= 1)
                {
                    ui.panelShownPercent = 1;
                    ui.animatePanel = UI_ANIMATE_STATIC;
                    ui.currentPanel = ui.changingTo;
                }
            }
            if (ui.animatePanel == UI_ANIMATE_OUT)
            {
                ui.panelShownPercent -= dt*4;
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
    int displayW = al_get_display_width(display);
    int displayH = al_get_display_height(display);

    if ((_RENDERSIZE+1)*256 > displayH || (_RENDERSIZE+1)*256 > displayW)
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale+");
        u->enabled = false;
    }
    else
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale+");
        u->enabled = true;
    }

    if (_RENDERSIZE == 1)
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale-");
        u->enabled = false;
    }
    else
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale-");
        u->enabled = true;
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
    if (b->spriteIndex)
    {
        DrawSprite(&sprites[b->spriteIndex],x,y,FRIENDLY,false);
    }

}
void UIDrawText(UIElement* u, int x, int y)
{
   UI_Text* t = u->data;
   //al_draw_text(ui.font,FRIENDLY,x,y,ALLEGRO_ALIGN_LEFT,t->str);
   Text te = (Text){.f = ui.font,ui.boldFont,.x=x,.y=y,.color=FRIENDLY,.lineHeight=al_get_font_line_height(ui.font)};

   al_do_multiline_text(ui.font,256,t->str,cb,&te);


}
void DrawUIElement(UIElement* u, int x, int y, ALLEGRO_MOUSE_STATE* mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    if (u->elementType == ELEMENT_BUTTON)
    {
        DrawButton(u,x,y,mouseState,isActive,bgColor);
    }
    if (u->elementType == ELEMENT_TEXT)
    {
        UIDrawText(u,x,y);
    }
    if (u->elementType == ELEMENT_SLIDER)
    {
        DrawSlider(u,x,y,mouseState,isActive,bgColor);
    }
    if (u->elementType == ELEMENT_CHECKBOX)
    {
        DrawCheckbox((Checkbox*)u->data,x,y,u->w,u->h,isActive,mouseState,bgColor);
    }
    if (u->elementType == ELEMENT_PULLDOWN)
    {
        DrawPullDownMenu((Pulldown*)u->data,x,y,u->w,u->h,isActive,mouseState,bgColor);
    }



}
int GetPanelMaxY(Panel* p)
{
    if (p->numElements <= 0)
        return 0;
    int maxY = -INT_MAX;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        if (u->y+u->h > maxY)
        {
            maxY = u->y+u->h;
        }
    }
    return maxY;
}

void GetUILocation(Panel* p, UIElement* uF, int* x, int* y)
{
    int maxY = GetPanelMaxY(p);
    int currX=p->x+p->padding; int currY=p->y+p->padding; 
    /*
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
    }*/
    *x = p->x + uF->x;
    *y = (p->y  + uF->y) - ((maxY+p->padding-p->h) * p->scrollPercent);

    if (p->tabs)
    {
        //*x += 25;
    }

}
void DrawPanelTabs(Panel* p, ALLEGRO_MOUSE_STATE* mouseState)
{
    int y = p->y; 
    int x = p->x;

    if (p->backButton.data)
    {
        DrawButton(&p->backButton,x,y,mouseState,true,BG);
        y+=p->backButton.h;
    }
    for (int i = 0; i < p->numTabs; i++)
    {
        Panel* tab = p->tabs[i];
        if (tab->tabButton.data)
        {
            Button* b = (Button*)tab->tabButton.data;
            int w; int h; 
            h = al_get_bitmap_height(sprites[b->spriteIndex].sprite);
            w = al_get_bitmap_width(sprites[b->spriteIndex].sprite);
            al_draw_rectangle(x,y,x+w,y+h,FRIENDLY,1);
            DrawButton(&tab->tabButton,x,y,mouseState,true,BG);
            //DrawSprite(&sprites[b->spriteIndex],p->x,y,FRIENDLY,false);
            y += h;
        } 
    }
}
void DrawPanel(Panel* p, ALLEGRO_MOUSE_STATE* mouseState)
{

    al_set_clipping_rectangle(p->x-1,p->y,p->w+1,p->h*ui.panelShownPercent+1);

    al_draw_filled_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,BG);
    al_draw_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,FRIENDLY,1);

    al_set_clipping_rectangle(p->x-1,p->y,p->w,p->h*ui.panelShownPercent);


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
        DrawUIElement(u,x,y,mouseState,u->enabled,BG);

    }
    if (p->tabs)
    {
        DrawPanelTabs(p, mouseState);
    }
    if (GetPanelMaxY(p) > p->h)
    {
        DrawScrollbar(p);
    }
    else
    {
        p->scrollPercent = 0;
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
bool cb(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    int x = t->x;
    int y = t->y;
    ALLEGRO_FONT* f = t->f;
    int height = t->lineHeight;
    y += line_num*height;

    bool bold = false;
    if (strncmp("[b]",line,3)==0)
    {   
        bold = true;
    }
    
    char* buff = calloc(size+1,sizeof(char));
    if (bold)
    {
        memcpy(buff,line+3,(size-3)*sizeof(char));
        al_draw_text(t->bold,t->color,x,y,ALLEGRO_ALIGN_LEFT,buff);
    }
    else
    {
        memcpy(buff,line,size*sizeof(char));
        al_draw_text(t->f,t->color,x,y,ALLEGRO_ALIGN_LEFT,buff);
    }
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
    memcpy(size,&(Text){f,NULL,0,0,0,FRIENDLY,al_get_font_line_height(f)+2},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    int height = t->h + padding*2;
    free(size);

    return height;
}
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, ALLEGRO_FONT* bold, int x, int y, int wTextbox, int minH, ALLEGRO_COLOR color)
{
    if (!description) return;
    int w;  
    int h;
    int xoffset;
    int yoffset;
    int lineHeight = al_get_font_line_height(f)+2;

    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,bold,x,y,0,color,lineHeight},sizeof(Text));
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
    
    void* extra = malloc(sizeof(Text));
    memcpy(extra,&(Text){.f=f,.bold=bold,.x=x,.y=y,.color=color,.lineHeight=lineHeight},sizeof(Text));
    
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
void SetOptions()
{
    if (GetButton(&ui.videoOptionsPanel,"RenderScale+"))
    {

        _RENDERSIZE++;
        char* newText = calloc(NumDigits(_RENDERSIZE)+2  ,sizeof(char));
        sprintf(newText,"%ix",_RENDERSIZE);
        SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,newText);
        SetDisplaySize();

        free(newText);
    }
    if (GetButton(&ui.videoOptionsPanel,"RenderScale-"))
    {

        _RENDERSIZE--;
        char* newText = calloc(NumDigits(_RENDERSIZE)+2  ,sizeof(char));
        sprintf(newText,"%ix",_RENDERSIZE);
        SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,newText);
        SetDisplaySize();

        free(newText);
    }

}