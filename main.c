#include <math.h>

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
#include "animationeffect.h"
#include "shield.h"
#include "encounter.h"
#include "gamestate.h"
#include "damagenumber.h"
#include "sound.h"
#include "loadscreen.h"
#include "settings.h"
#include "replay.h"


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
    
    InitSettings("config.cfg");

    InitGameState();
    InitSound();
    InitColors();
    InitVideo();
    InitObjects();
    InitPlayers();
    InitMaps();
    InitAnimationEffects();

    //memset(&gameOptions,0,sizeof(GameOptions));

    init_lua();
    InitAttacks();
    InitAbilities();
    InitParticles();
    InitUI();
    InitLoadscreen("assets/ui/fullstartscreen.png");
    LoadEncounters("assets/encounters",luaState);

    Map* m = LoadMap("assets/ui/map_unitselect.lua");  
    SetMap(&maps[0]);
    LoadPrefabs("assets/friendly");

    toSpawn = NULL;

    atexit(EndSettings);


    for (int i = 0; i < numEncounters; i++)
    {
        //SetEncounterRandAugments(encounters[i]);
    }
}

void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{

    if (!GameIsPaused())
    {


        lua_settop(luaState,0);
        UpdateEncounter(dt);
        CheckSelected(mouseState,mouseStateLastFrame, keyState);
        ProcessAugments(currEncounterRunning->augments,dt);
        UpdateAttacks(dt);
        SetControlGroups(keyState);
        GetControlGroup(keyState);
        UpdateDamageNumbers(dt);
        UpdatePlayerObjectInteractions(keyState,keyStateLastFrame,mouseState);
        for (int i = 0; i < numObjects; i++)
        {
            UpdateObject(&objects[i],dt);
        }
        currGameObjRunning = NULL;
        UpdateAbilityInteractions(keyState, keyStateLastFrame,mouseState,mouseStateLastFrame);

        CheckIfGameIsLost();
        CheckIfGameIsWon();


        if (gameState == GAMESTATE_INGAME)
        {
            gameStats.timeTaken += dt;
        }
    }
    UpdateParticles(dt);
    ProcessAnimationEffects(dt);
    UpdateWidgets(dt);
    UpdateChatbox(dt);
}

void Render(float dt, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    al_hide_mouse_cursor(display);
    al_grab_mouse(display);
    al_set_target_bitmap(SCREEN);
    if (gameState == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        DrawLevelSelect(mouseState,mouseStateLastFrame);
        DrawMouse(mouseState, NULL);
        DrawTransition(dt);

        return;
    }

    DrawMap();
    
    
    DrawWidgets(gameState, DRAWORDER_BEFOREUI);
    DrawAttacks(dt);

    if (gameState == GAMESTATE_CHOOSING_UNITS)
    {
        DrawUnitChoiceUI(mouseState, mouseStateLastFrame);
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
        if ((i == objSelected || &objects[i] == players[0].clickedThisFrame) && !ObjIsInvincible(&objects[i]))
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
        
    if (currSettings.particlesEnabled)    
        DrawParticles();

    if (players[0].abilityHeld)
    {
        DrawHeldAbility(mouseState);
    }
    if (gameState == GAMESTATE_MAIN_MENU)
    {
        al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);
    }
    if (gameState != GAMESTATE_MAIN_MENU)
         DrawUI(keyState, keyStateLastFrame, mouseState);
    DrawMenus(mouseState);
    DrawAnimationEffects();

    GameObject* mousedOver = GetMousedOver(mouseState);
    if (gameState == GAMESTATE_CHOOSING_UNITS) 
    {

        Rect selectedUnitsR = (Rect){8,146,240,41};
        int numUnitsInRect = GetNumObjectsInRect(&selectedUnitsR);

        if (GetButtonIsClicked(&ui.choosingUnits_Back))
        {
            SetGameStateToChoosingEncounter();
            //gameState = GAMESTATE_TRANSITION_TO_CHOOSING_ENCOUNTER;
        }

        if (GetButtonIsClicked(&ui.choosingUnits_GO) && numUnitsInRect==encounters[selectedEncounterIndex]->numUnitsToSelect)
        {
            combatStarted = false;

            //ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");

            Encounter* e = encounters[selectedEncounterIndex];
            currEncounterRunning = e;
            GameObject** list = calloc(e->numUnitsToSelect,sizeof(GameObject*));

            //Get the number of units in the rect and make a list of them
            int foundIndex = 0;
            for (int i = 0; i < MAX_OBJS; i++)
            {
                if (IsActive(&objects[i]) && !ObjIsDecoration(&objects[i]))
                {
                    Rect r2 = GetObjRect(&objects[i]);

                    if (CheckIntersect(selectedUnitsR,r2))
                    {
                        list[foundIndex] = objects[i].prefab;
                        foundIndex++;
                    }
                }
            }
            SetGameStateToLoadingEncounter(list,foundIndex,e);//SetGameStateToInGame(list,foundIndex,e); 
            free(list);
        }
    }
    DrawDamageNumbers();
    if (gameState == GAMESTATE_END)
    {
        DrawEndScreen(mouseState,mouseStateLastFrame);
    }

    DrawMenus(mouseState);
    DrawWidgets(gameState, DRAWORDER_AFTERUI);
    DrawMouse(mouseState, mousedOver);
    players[0].clickedThisFrame = NULL;

    if (gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER) 
    {
        DrawLoadscreen();
    }

    DrawTransition(dt);
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
   al_register_event_source(queue, al_get_keyboard_event_source());


   ALLEGRO_KEYBOARD_STATE keyStateLastFrame;
   al_get_keyboard_state(&keyStateLastFrame);
   ALLEGRO_MOUSE_STATE mouseStateLastFrame;
    mouseStateLastFrame = GetMouseClamped();

    ALLEGRO_MOUSE_STATE mouseState;
    ALLEGRO_KEYBOARD_STATE keyState;


    //PlayMusic("assets/audio/first_boss.wav");


    fflush(stdout);

    while (gameState != GAMESTATE_EXIT) {
        //al_set_mouse_xy(display, 128,128);


        
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER)
        {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                FinishLoadScreen();
            }
        }
        if (gameState == GAMESTATE_IN_CHATBOX)
        {
            mouseState = GetMouseClamped();

            if (event.type == ALLEGRO_EVENT_KEY_DOWN || (mouseState.buttons & 1 && !(mouseStateLastFrame.buttons & 1)))
            {
                if (chatboxes)
                {
                    if (chatboxShowing && !chatboxShowing->allowsInteraction)
                        Chatbox_NextLine(); 
                }

            }
        }
        if (event.type == ALLEGRO_EVENT_TIMER) {
            
            int displayW = al_get_display_width(display);
            int displayH = al_get_display_height(display);

            int drawposx = displayW/2 - (_RENDERSIZE*256)/2; 
            int drawposy = displayH/2 - (_RENDERSIZE*256)/2;


            al_get_keyboard_state(&keyState);
            keyStateLua = &keyState;
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

            UpdateTransition(1/(float)_TARGET_FPS);

            //if (!ui.currentPanel)
                Update(1/(float)_TARGET_FPS,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            UpdateInterface(1/(float)_TARGET_FPS,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            if (gameState == GAMESTATE_EXIT)
                break;
            Render(1/(float)_TARGET_FPS, &mouseState, &mouseStateLastFrame, &keyState, &keyStateLastFrame);
            if (gameState == GAMESTATE_INGAME)
                RecordReplay(SCREEN);

            if (al_key_down(&keyState,ALLEGRO_KEY_I) && !al_key_down(&keyStateLastFrame,ALLEGRO_KEY_I))
                replay.framePlayPosition = 0;

            if (al_key_down(&keyState,ALLEGRO_KEY_M) && !al_key_down(&keyStateLastFrame,ALLEGRO_KEY_M))
                ReplayToDisk(&replay);


            if (al_key_down(&keyState,ALLEGRO_KEY_I))
                PlayReplay(SCREEN);
                
            al_set_target_bitmap(backbuffer);
            al_draw_scaled_bitmap(SCREEN,0,0,_SCREEN_SIZE,_SCREEN_SIZE, drawposx, drawposy,_SCREEN_SIZE*_RENDERSIZE,_SCREEN_SIZE*_RENDERSIZE,0);
            al_flip_display();

            mouseStateLastFrame = mouseState;
            keyStateLastFrame = keyState;
            _FRAMES++;
            fflush(stdout);
        }
    }
    WriteSettingsFile("config.cfg");
    return 1;
}

