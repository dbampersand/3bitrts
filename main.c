
#include "allegro5/allegro.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include <allegro5/allegro_audio.h> 


#include "gameobject.h"
#include "map.h"
#include "luafuncs.h"
#include "sprite.h"
#include "helperfuncs.h"
#include "vector2.h"
#include "rect.h"
#include "player.h"
#include "colors.h"
#include "video.h"
#include "ui.h"
#include "attack.h"
#include "particle.h"
#include <math.h>
#include "animationeffect.h"
#include "shield.h"
#include "encounter.h"
#include "gamestate.h"
#include "damagenumber.h"
#include "sound.h"



int _TARGET_FPS = 60;

void init()
{
    srand(time(NULL));

    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();
    al_install_audio();

    InitGameState();
    InitSound();
    InitColors();
    InitVideo();
    InitObjects();
    InitPlayers();
    InitMaps();
    InitAnimationEffects();

    memset(&gameOptions,0,sizeof(GameOptions));

    init_lua();
    InitAttacks();
    InitParticles();
    InitUI();
    
    LoadEncounters("assets/encounters",luaState);

    Map* m = LoadMap("assets/ui/map_unitselect.lua");  
    SetMap(&maps[0]);
    LoadPrefabs("assets/friendly");


}

void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    lua_settop(luaState,0);
    CheckSelected(mouseState,mouseStateLastFrame, keyState);
    UpdateAttacks(dt);
    SetControlGroups(keyState);
    GetControlGroup(keyState);
    UpdateDamageNumbers(dt);
    UpdatePlayerObjectInteractions(keyState,keyStateLastFrame,mouseState);
    for (int i = 0; i < numObjects; i++)
    {
        UpdateObject(&objects[i],dt);
    }
    UpdateAbilityInteractions(keyState, keyStateLastFrame,mouseState,mouseStateLastFrame);

    CheckIfGameIsLost();
    CheckIfGameIsWon();

    UpdateParticles(dt);
    ProcessAnimationEffects(dt);


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
float angle = 0;

void Render(float dt, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    al_hide_mouse_cursor(display);
    al_grab_mouse(display);

    al_set_target_bitmap(SCREEN);
    
    if (gameState == CHOOSING_ENCOUNTER)
    {
        DrawLevelSelect(mouseState,mouseStateLastFrame);
        DrawMouse(mouseState, NULL);
        return;
    }
    DrawSprite(&sprites[currMap->spriteIndex],0,0,GROUND,false);
    DrawAttacks(dt);

    if (gameState == CHOOSING_UNITS) 
    {
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


    int objSelected = -1;
    bool abilityCastOnTarget = players[0].abilityHeld && (mouseStateLastFrame->buttons & 1 || mouseState->buttons & 1);
    if (abilityCastOnTarget)
    {
        if (players[0].abilityHeld->castType & ABILITY_TARGET_ENEMY || players[0].abilityHeld->castType & ABILITY_TARGET_ALL)
            abilityCastOnTarget = true;
        else
            abilityCastOnTarget = false;
    }
    if ((mouseStateLastFrame->buttons & 2 || mouseState->buttons & 2) || abilityCastOnTarget)
    {
        for (int i = 0; i < numObjects; i++)
        {
            GameObject* g = &objects[i];
            if (g->properties & OBJ_ACTIVE && (g->properties & OBJ_OWNED_BY || abilityCastOnTarget))
            {
                if (PointInRect(mouseState->x,mouseState->y,GetObjRect(&objects[i])))
                {
                    objSelected = i;
                }
            }
        }
    }
    for (int i = 0; i < numObjects; i++)
    {
        DrawChannelHint(&objects[i]);
        if (i == objSelected || &objects[i] == players[0].clickedThisFrame)
        {
            DrawGameObj(&objects[i],true);
            
        }
        else
        {
            DrawGameObj(&objects[i],false);

        }
    }
    if (players[0].selecting)
        DrawMouseSelectBox(GetMouseClamped());
        
    if (*gameOptions.particlesEnabled)    
        DrawParticles();
    if (players[0].abilityHeld)
    {
        float cx; float cy;
        GameObject* g = players[0].selection[players[0].indexSelectedUnit];
        GetCentre(g, &cx, &cy);
        int w; int h;
        float radius = players[0].abilityHeld->range;

        al_draw_circle(cx,cy,radius,FRIENDLY,0);
        
        if (players[0].abilityHeld->targetingHint == HINT_LINE)
        {
            //normalise then project to min(radius,mousepos)
            float distX = mouseState->x - cx; 
            float distY = mouseState->y - cy;
            float dist = sqrt(distX*distX+distY*distY);
            
            float x = mouseState->x - cx;
            float y = mouseState->y - cy;
            Normalize(&x,&y); 
            x = cx + x*_MIN(radius,dist);
            y = cy + y*_MIN(radius,dist);

            //x *= radius;
            //y *= radius;
            al_draw_line(cx,cy,x,y,FRIENDLY,1);
        }
        GameObject* heldSelected = players[0].selection[players[0].indexSelectedUnit];
        float cxHeld; float cyHeld; 
        GetCentre(heldSelected,&cxHeld,&cyHeld);
        al_draw_line(mouseState->x+2,mouseState->y+2,cxHeld,cyHeld,FRIENDLY,1);
    }
    if (gameState == MAIN_MENU)
    {
        al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);
    }
    if (gameState != MAIN_MENU)
        DrawUI(keyState, keyStateLastFrame, mouseState);
    DrawMenus(mouseState);

    if (al_key_down(keyState,ALLEGRO_KEY_F) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_F))
    {
    }   
    DrawAnimationEffects();

    GameObject* mousedOver = NULL;
    for (int i = 0; i < numObjects; i++)
    {
        for (int i = 0; i < numObjects; i++)
        {
            GameObject* g = &objects[i];
            if (g->properties & OBJ_ACTIVE)
            {
                if (PointInRect(mouseState->x,mouseState->y,GetObjRect(g)))
                {
                    mousedOver = g;
                }
            }
        }
    }
    if (gameState == CHOOSING_UNITS)
    {

        Rect selectedUnitsR = (Rect){8,146,240,41};
        int numUnitsInRect = GetNumObjectsInRect(&selectedUnitsR);
        


        if (GetButtonIsClicked(&ui.choosingUnits_Back))
        {
            gameState = CHOOSING_ENCOUNTER;
            ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");
        }

        if (GetButtonIsClicked(&ui.choosingUnits_GO) && numUnitsInRect==encounters[selectedEncounterIndex]->numUnitsToSelect)
        {
            gameState = INGAME;
            combatStarted = false;

            ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");

            Encounter* e = encounters[selectedEncounterIndex];
            currEncounterRunning = e;
            GameObject** list = calloc(e->numUnitsToSelect,sizeof(GameObject*));

            int foundIndex = 0;
            for (int i = 0; i < MAX_OBJS; i++)
            {
                if (IsActive(&objects[i]))
                {
                    Rect r2 = GetObjRect(&objects[i]);

                    if (CheckIntersect(selectedUnitsR,r2))
                    {
                        list[foundIndex] = objects[i].prefab;
                        foundIndex++;
                    }
                }
            }
            RemoveAllGameObjects();
            SetMap(LoadMap(e->mapPath));

            int xPos = 0; 
            for (int i = 0; i < e->numUnitsToSelect; i++)
            {
                AddGameobject(list[i],80+xPos,180);   
                xPos += GetWidth(list[i]);
            }
            free(list);
        }

    }
    DrawDamageNumbers();
    DrawMenus(mouseState);
    DrawMouse(mouseState, mousedOver);
    players[0].clickedThisFrame = NULL;
    
}

void DrawMainMenu()
{
    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

}
int main(int argc, char* args[])
{

    init();

    ALLEGRO_TIMER* _FPS_TIMER = al_create_timer(1.0f / (double)_TARGET_FPS);

    al_start_timer(_FPS_TIMER);
    queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
   al_register_event_source(queue, al_get_timer_event_source(_FPS_TIMER));


   ALLEGRO_KEYBOARD_STATE keyStateLastFrame;
   al_get_keyboard_state(&keyStateLastFrame);
   ALLEGRO_MOUSE_STATE mouseStateLastFrame;
    mouseStateLastFrame = GetMouseClamped();

    //PlayMusic("assets/audio/first_boss.wav");

    while (!gameState != GAMESTATE_EXIT) {
        //al_set_mouse_xy(display, 128,128);

        
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);


        if (event.type == ALLEGRO_EVENT_TIMER) {
            
            int displayW = al_get_display_width(display);
            int displayH = al_get_display_height(display);

            int drawposx = displayW/2 - (_RENDERSIZE*256)/2; 
            int drawposy = displayH/2 - (_RENDERSIZE*256)/2;

            ALLEGRO_MOUSE_STATE mouseState;
            ALLEGRO_KEYBOARD_STATE keyState;

            al_get_keyboard_state(&keyState);
            mouseState = GetMouseClamped();
            
            al_set_target_bitmap(backbuffer);
            al_clear_to_color(BG);


            al_set_target_bitmap(SCREEN);
            al_clear_to_color(BG);

            if (al_key_down(&keyState,ALLEGRO_KEY_F1))
            {
                StopMusic();
            }
            UpdateMusic(1/(float)_TARGET_FPS);

            if (!ui.currentPanel)
                Update(1/(float)_TARGET_FPS,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            UpdateInterface(1/(float)_TARGET_FPS,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            if (gameState == GAMESTATE_EXIT)
                break;
            Render(1/(float)_TARGET_FPS, &mouseState, &mouseStateLastFrame, &keyState, &keyStateLastFrame);

            al_set_target_bitmap(backbuffer);
            al_draw_scaled_bitmap(SCREEN,0,0,_SCREEN_SIZE,_SCREEN_SIZE, drawposx, drawposy,_SCREEN_SIZE*_RENDERSIZE,_SCREEN_SIZE*_RENDERSIZE,0);
            al_flip_display();

            mouseStateLastFrame = mouseState;
            keyStateLastFrame = keyState;
            _FRAMES++;
            fflush(stdout);
        }
    }
    return 1;
}

