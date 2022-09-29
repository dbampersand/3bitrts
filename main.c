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
#include "minimap.h"
#include "pathfind.h"
#include "shop.h"
#include "item.h"

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

    InitPath();
    
    InitSettings("config.cfg");
    InitAugments();
    InitGameState();
    InitSound();
    InitColors();
    InitVideo();
    init_lua();

    LoadItemFolder("assets/items");
    InitItems();
    InitObjects();
    InitPlayers();
    InitMaps();
    InitAnimationEffects();

    //memset(&gameOptions,0,sizeof(GameOptions));

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
        SetEncounterRandAugments(encounters[i]);
    }
    InitReplay();
    InitPathfinding();
    LoadShop();
}

void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{

    if (gameState == GAMESTATE_IN_SHOP)
        UpdateShop(dt,*mouseState,*mouseStateLastFrame);
    
    if (!GameIsPaused())
    {
        MoveCamera(*mouseState,keyState,dt);
        UpdateItems(dt);
        lua_settop(luaState,0);
        UpdateEncounter(dt);
        CheckSelected(mouseState,mouseStateLastFrame, keyState);
        ProcessAugments(currEncounterRunning->augments,dt);
        SetControlGroups(keyState); 
        GetControlGroup(keyState);
        UpdateDamageNumbers(dt);
        UpdatePlayerObjectInteractions(keyState,keyStateLastFrame,mouseState);
        for (int i = 0; i < numObjects; i++)
        {
            UpdateObject(&objects[i],dt);
        }
        UpdateAttacks(dt);

        currGameObjRunning = NULL;
        UpdateAbilityInteractions(keyState, keyStateLastFrame,*mouseState,*mouseStateLastFrame);

        CheckIfGameIsLost();
        CheckIfGameIsWon();


        if (gameState == GAMESTATE_INGAME)
        {
            gameStats.timeTaken += dt;
        }
        if (gameState == GAMESTATE_WATCHING_REPLAY)
        {
            if (replay.playing)
                replay.framePlayPosition++;
        }
    }
    UpdateParticles(dt);
    ProcessAnimationEffects(dt);
    UpdateWidgets(dt);
    UpdateChatbox(dt);
}
float easeOutQuint(float x) {
    if (x > 1) return 1;
    return 1 - pow(1 - x, 5);
}
float aefesfsd = 0;

void Render(float dt, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    al_hide_mouse_cursor(display);
    al_grab_mouse(display);
    al_set_target_bitmap(SCREEN);
    if (gameState == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        DrawAllLevelSelects(mouseState,mouseStateLastFrame);
        encounterMoveTimer += dt;
        encounterOffset = Towards(encounterOffset,encounterMoveTo,easeOutQuint(encounterMoveTimer)*_SCREEN_SIZE/8.0f);
        DrawMouse(mouseState, NULL);
        DrawTransition(dt);

        return;
    }

    DrawMap();
    
    DrawCommandQueue(players[0].selection[players[0].indexSelectedUnit]);
    
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
    DrawObjShadows();
    DrawMapHighlights();
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
    //Draw health bars on top of all objects
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g = &objects[i];
        if (!IsActive(g))
            continue;
        ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
        if (ObjIsDecoration(g))
            c = BG;

        if (!ObjIsInvincible(g) && g->summonTime > g->summonMax)
        {
            float percent = GetSummonPercent(g);  

            Sprite* s = ObjIsChannelling(g) ? &sprites[g->channelingSpriteIndex] :  &sprites[g->spriteIndex];

            float x = g->position.x + g->offset.x; 
            float y = g->position.y + g->offset.y;
            ToScreenSpace(&x,&y);


            Rect selectRect;
            selectRect.w = al_get_bitmap_width(s->sprite);
            selectRect.h = (al_get_bitmap_height(s->sprite) * percent);
            selectRect.x = x;
            selectRect.y = (y + (al_get_bitmap_height(s->sprite) - selectRect.h));
            if (g->stunTimer == 0)
                DrawRoundedRect(selectRect, c,false);
            //if (GetTotalShield(g) > 0)
            {
                float r = (GetWidth(g) > GetHeight(g) ? GetWidth(g) : GetHeight(g)) * easeInOutBack(g->shieldSizeTimer);
                al_draw_circle(floor(x+GetWidth(g)/2),floor(y+GetHeight(g)/2),r,c,1);
            }

            
            if (currSettings.displayHealthBar == OPTION_HPBAR_ALWAYS)
                DrawHealthBar(g,c);
            else if (currSettings.displayHealthBar == OPTION_HPBAR_SELECTED && (IsOwnedByPlayer(g) && IsSelected(g)))
                DrawHealthBar(g,c);
            else if (currSettings.displayHealthBar == OPTION_HPBAR_NEVER && (!IsOwnedByPlayer(g)))
                DrawHealthBar(g,c);
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

        if (ui.currentPanel != &ui.mainMenuPanel)
        {
            DrawPanel(&ui.mainMenuPanel,mouseState,1);
        }
    }
    DrawWidgets(gameState, DRAWORDER_BEFOREUI);

    if (gameState != GAMESTATE_MAIN_MENU)
         DrawUI(keyState, keyStateLastFrame, mouseState);

    DrawMenus(mouseState);
    DrawAnimationEffects();

    GameObject* mousedOver = GetMousedOver(mouseState);
    if (gameState == GAMESTATE_CHOOSING_UNITS) 
    {

        Rect selectedUnitsR = (Rect){8,146,240,41};
        int numUnitsInRect = GetNumObjectsInRect(&selectedUnitsR,true);

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
                if (IsActive(&objects[i]) && !ObjIsDecoration(&objects[i]) && objects[i].playerChoosable)
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
    if (gameState == GAMESTATE_END)
    {
        DrawEndScreen(mouseState,mouseStateLastFrame);
    }
    if (gameState == GAMESTATE_WATCHING_REPLAY)
    {
        PlayReplay(al_get_target_bitmap());
        DrawReplayUI(&replay,mouseState,mouseStateLastFrame);
    }
    if (gameState == GAMESTATE_INGAME)
    {
        if (IsBindDown(keyState,currSettings.keymap.key_ShowMap))
        {
            DrawMinimap(*mouseState);
        }
        DrawTimer(currSettings.displayTimer);

    }

    DrawBufferedStrings();
    if (gameState == GAMESTATE_INGAME || gameState == GAMESTATE_CHOOSING_UNITS)
    {
        DrawDamageNumbers();

        UpdateButton(ui.menuButton.x,ui.menuButton.y,&ui.menuButton,*mouseState,*mouseStateLastFrame);
        DrawUIElement(&ui.menuButton,ui.menuButton.x,ui.menuButton.y,mouseState,BG);

        if (GetButtonIsClicked(&ui.menuButton))
        {
            if (ui.currentPanel)
            {
                ChangeUIPanel(NULL);
                WriteSettingsFile("config.cfg");
            }
            else
            {
                ChangeUIPanel(&ui.pauseMenuPanel);
            }
        }

    }
    if (gameState == GAMESTATE_IN_SHOP)
    {
        DrawShop(dt,*mouseState);
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

    if (al_key_down(keyState,ALLEGRO_KEY_MINUS))
        DisplayCollision();
    //GameObjDebugDraw();
    //DebugDrawPathfinding();   
    //al_draw_filled_rectangle(0,0,255,255,BG);
    
}

void DrawMainMenu()
{
    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

}
int main(int argc, char* args[])
{
    init();
    printf("%lu\n",sizeof(PathfindNode));

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
                if (!IsModifierDown(&keyState))
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
                        if (!IsModifierDown(&keyState))
                            Chatbox_NextLine(); 
                }

            }
        }
        if (event.type == ALLEGRO_EVENT_TIMER) {
            
            ResetSoundsThisFrame();

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

