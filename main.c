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
#include "pointspace.h"
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
#include "console.h"
#include "vectorshape.h"
#include "easings.h"
#include "3d.h"

#include "editor.h"
#include "timer.h"

ALLEGRO_BITMAP* testBitmap;

#ifdef __WIN32__
#include <windows.h>
#endif

void init()
{
    srand(time(NULL));

    _IS_FOCUSED_WINDOW = true;

    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();

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
    InitEditorUI();
        
    InitConsole();

    InitLoadscreen("assets/ui/startscreen/fullstartscreen.png","assets/ui/startscreen/mask.png");
    LoadEncounters("assets/encounters",luaState);

    currMap = NULL;

    Map* m = LoadMap("assets/ui/map_unitselect.lua");  
    Map* mNull = LoadMap("assets/ui/map_null.lua");  


    SetMap(mNull);

    LoadPrefabs("assets/friendly");
    LoadPrefabs("assets/enemies");
    PopulateUnitSelector(&editor.editorUI.unitSelector);

    InitPurchasingUnitsUI();

    toSpawn = NULL;

    atexit(EndSettings);


    for (int i = 0; i < numEncounters; i++)
    {
        SetEncounterRandAugments(encounters[i]);
    }
    InitReplay();
    InitPathfinding();
    LoadShop();

    //Init3d();
    //gameState = GAMESTATE_IN_3D;
    //PlayMusic("assets/audio/intro.wav",0.5f,0);
}

void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseStateLastFrame)
{   
    if (!(mouseState->mouse.buttons & 1))
        _PANEL_CLICKED_THIS_FRAME = false;
    UpdateAmbience(dt);
    numChannellingInfosDrawn =  0;
    if (gameState == GAMESTATE_IN_SHOP)
    {
        UpdateShop(dt,*mouseState,*mouseStateLastFrame);
    }

    if (gameState == GAMESTATE_INGAME || gameState == GAMESTATE_CHOOSING_UNITS)
    {
        if (IsBindDownThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_Console))
            ToggleConsole();
    }    
    if (gameState == GAMESTATE_IN_EDITOR)
    {
        UpdateEditor(dt,*mouseState,*mouseStateLastFrame,keyState, keyStateLastFrame);
        MoveCamera(*mouseState,keyState,dt);
    }
    if (!GameIsPaused())
    {

        MoveCamera(*mouseState,keyState,dt);
        UpdateItems(dt);
        lua_settop(luaState,0);
        UpdateEncounter(dt);
        CheckSelected(mouseState,mouseStateLastFrame, keyState, keyStateLastFrame);
        if (currEncounterRunning)
            ProcessAugments(currEncounterRunning->augments,dt);
        SetControlGroups(keyState); 
        GetControlGroup(keyState);
        UpdateDamageNumbers(dt);
        UpdatePlayerObjectInteractions(keyState,keyStateLastFrame,mouseState);
        for (int i = 0; i < numActiveObjects; i++)
        {
            UpdateObject(activeObjects[i],dt);
        }
        UpdateAttacks(dt);

        currGameObjRunning = NULL;
        UpdateAbilityInteractions(keyState, keyStateLastFrame,*mouseState,*mouseStateLastFrame);

        CheckIfGameIsLost();
        //CheckIfGameIsWon();


        if (gameState == GAMESTATE_INGAME)
        {
            gameStats.timeTaken += dt;
        }
        if (gameState == GAMESTATE_WATCHING_REPLAY)
        {
            if (replay.playing)
                replay.framePlayPosition++;
        }
        UpdateTimers(dt);
    }
    ProcessSpriteDecorations(dt);
    UpdateParticles(dt);
    ProcessAnimationEffects(dt);
    UpdateWidgets(dt);
    UpdateChatbox(dt);


    if (al_key_down(keyState,ALLEGRO_KEY_LCTRL) && KeyPressedThisFrame(ALLEGRO_KEY_M,keyState,keyStateLastFrame))
    {
        if (gameState != GAMESTATE_IN_EDITOR)
            SetGameStateToInEditor(); 
        else
        {
            if (strcmp(currMap->path,"assets/ui/map_unitselect.lua") == 0)
                SetGameStateToChoosingParty();
            else
            {   
                _COMING_FROM_EDITOR = true;
                SetGameStateToInGame(); 
            }
        } 
    }
}
float aefesfsd = 0;

void Render(float dt, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    if (_IS_FOCUSED_WINDOW)
    {
        al_hide_mouse_cursor(display);
        al_grab_mouse(display);
    }
    else
    {
        al_show_mouse_cursor(display);
        al_ungrab_mouse();
    }
    al_set_target_bitmap(SCREEN);


    for (int i = 0; i < numActiveObjects; i++)
    {
        UpdateScreenPositions(activeObjects[i]);
    }
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        if (AttackIsActive(&attacks[i]))
            UpdateScreenPositionsAttack(&attacks[i]);
    }

    DrawSpriteDecorations(BEFORE_WORLD);
    if (GameStateIsDrawingMap(gameState))
        DrawMap(currMap,false);

    DrawSpriteDecorations(AFTER_WORLD);
    if (GameStateIsDrawingMap(gameState))
    DrawMapHighlights();


    DrawCommandQueue(players[0].selection[players[0].indexSelectedUnit]);
    

    UpdateGoldAnimationTimer(dt);


    if (gameState == GAMESTATE_CHOOSING_UNITS)
    {
        DrawUnitChoiceUI(mouseState, mouseStateLastFrame);
    }
   

    int objSelected = -1;
    bool abilityCastOnTarget = players[0].abilityHeld && (mouseStateLastFrame->mouse.buttons & 1 || mouseState->mouse.buttons & 1);
    if (abilityCastOnTarget)
    {
        if (players[0].abilityHeld->castType & ABILITY_TARGET_ENEMY || players[0].abilityHeld->castType & ABILITY_TARGET_ALL)
            abilityCastOnTarget = true;
        else
            abilityCastOnTarget = false;
    }
    if ((mouseStateLastFrame->mouse.buttons & 2 || mouseState->mouse.buttons & 2) || abilityCastOnTarget)
    {
        for (int i = 0; i < numActiveObjects; i++)
        {
            GameObject* g = activeObjects[i];
            if (g->properties & OBJ_ACTIVE && (g->properties & OBJ_OWNED_BY || abilityCastOnTarget))
            {
                if (PointInRect(mouseState->worldX,mouseState->worldY,GetObjRect(activeObjects[i])))
                {
                    objSelected = i;
                }
            }
        }
    }
    if (GameStateIsDrawingMap(gameState))
    {
        DrawObjShadows();
        DrawAggroIndicators();
    }
    DrawAttacks(dt,ATTACK_AOE | ATTACK_CONE | ATTACK_SHAPE);

    if (GameStateIsDrawingMap(gameState))
    {
        for (int i = 0; i < numActiveObjects; i++)
        {
            DrawChannelHint(activeObjects[i],dt,*mouseState);
            if ((i == objSelected || activeObjects[i] == players[0].clickedThisFrame) && !ObjIsInvincible(activeObjects[i]))
            {
                DrawGameObj(activeObjects[i],true);
            }
            else
            {
                DrawGameObj(activeObjects[i],false);

            }
        }
    }

    if (GameStateIsDrawingMap(gameState))
        DrawAttacks(dt, ATTACK_PROJECTILE_TARGETED | ATTACK_PROJECTILE_TARGETED | ATTACK_PROJECTILE_ANGLE | ATTACK_MELEE);

    //Draw health bars on top of all objects
    if (GameStateIsDrawingMap(gameState))
    {
        for (int i = 0; i < numActiveObjects; i++)
        {
            GameObject* g = activeObjects[i];
            ALLEGRO_COLOR c = IsOwnedByPlayer(g) == true ? FRIENDLY : ENEMY;
            if (ObjIsDecoration(g))
                c = BG;
            if (IsOwnedByPlayer(g) && !IsSelectable(g))
                c = BG;



            if (!ObjIsInvincible(g) && g->summonTime > g->summonMax)
            {
                float percent = GetSummonPercent(g);  

                Sprite* s = ObjIsChannelling(g) ? &sprites[g->channelingSpriteIndex] :  &sprites[g->spriteIndex];

                float x = g->position.screenX + g->offset.x; 
                float y = g->position.screenY + g->offset.y;
                //ToScreenSpace(&x,&y);


                Rect selectRect;
                //selectRect.w = al_get_bitmap_width(s->sprite);
                //selectRect.h = (al_get_bitmap_height(s->sprite) * percent);
                //selectRect.x = x;
                //selectRect.y = (y + (al_get_bitmap_height(s->sprite) - selectRect.h));
                
                selectRect = GetObjRect(g);
                
                ToScreenSpace(&selectRect.x,&selectRect.y);
                selectRect.x += g->offset.x;
                selectRect.y += g->offset.y;


                //if (GetTotalShield(g) > 0)
                {
                    float r = (GetWidth(g) > GetHeight(g) ? GetWidth(g) : GetHeight(g)) * easeInOutBack(g->shieldSizeTimer);
                    al_draw_circle(floorf(x+GetWidth(g)/2),floorf(y+GetHeight(g)/2),r,c,1);
                }

                
                if (currSettings.displayHealthBar == OPTION_HPBAR_ALWAYS)
                    DrawHealthBar(g,c);
                else if (currSettings.displayHealthBar == OPTION_HPBAR_SELECTED && (IsOwnedByPlayer(g) && IsSelected(g)))
                    DrawHealthBar(g,c);
                else if (currSettings.displayHealthBar == OPTION_HPBAR_NEVER && (!IsOwnedByPlayer(g)))
                    DrawHealthBar(g,c);

                if (g->flashTimer > 0)
                    c = DAMAGE;
                if (g->stunTimer == 0)
                {
                    if (GetPlayerOwnedBy(g) == 0)
                        DrawRoundedRect(selectRect, c,false);
                    else
                        al_draw_rectangle((int)selectRect.x,(int)selectRect.y,(int)(selectRect.x + selectRect.w), (int)(selectRect.y + selectRect.h),c,1);
                }

            }

        }
    }
    if (GameStateIsDrawingMap(gameState))
        DrawObjHeadingArrows();

    DrawSpriteDecorations(AFTER_GAMEOBJECTS);

    if (players[0].selecting)
        DrawMouseSelectBox(GetMouseClamped());
    if (currSettings.particlesEnabled)    
        DrawParticles();
    DrawSpriteDecorations(AFTER_PARTICLES);
    
        

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
        DrawParticles();

    }
    DrawWidgets(gameState, DRAWORDER_BEFOREUI);


    DrawMenus(mouseState);
    DrawAnimationEffects();

    GameObject* mousedOver = GetMousedOver(mouseState);
    if (gameState == GAMESTATE_CHOOSING_UNITS) 
    {

        int numUnitsSelected = 0;//GetNumObjectsInRect(&selectedUnitsR,true);
        for (int i = 0; i < players[0].numUnitsSelected; i++)
        {
            if (players[0].selection[i]->playerChoosable)
            {
                numUnitsSelected++;
            }
        }


        if (GetButtonIsClicked(&ui.choosingUnits_Back))
        {
            SetGameStateToChoosingEncounter();
            //gameState = GAMESTATE_TRANSITION_TO_CHOOSING_ENCOUNTER;
        }

        if (GetButtonIsClicked(&ui.choosingUnits_GO) && numUnitsSelected==encounters[selectedEncounterIndex]->numUnitsToSelect)
        {

            numDeadFriendlyObjects = 0;

            //ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");

            Encounter* e = encounters[selectedEncounterIndex];
            currEncounterRunning = e;
            GameObject** list = calloc(e->numUnitsToSelect,sizeof(GameObject*));

            int foundIndex = 0;
            for (int i = 0; i < players[0].numUnitsSelected; i++)
            {
                if (players[0].selection[i]->playerChoosable)
                {
                    list[foundIndex] = players[0].selection[i]->prefab; 
                    foundIndex++;
                }
            }
            SetGameStateToLoadingEncounter(list,foundIndex,e);//SetGameStateToInGame(list,foundIndex,e); 
            free(list);
        }
    }
    if (gameState == GAMESTATE_WATCHING_REPLAY)
    {
        PlayReplay(al_get_target_bitmap());
        DrawReplayUI(&replay,mouseState,mouseStateLastFrame);
    }
    if (gameState == GAMESTATE_INGAME)
    {
        if (!console.active && IsBindDown(keyState,currSettings.keymap.key_ShowMap))
        {
            DrawMinimap(*mouseState);
        }
        DrawTimer(currSettings.displayTimer);
    }

    DrawBufferedStrings();
    if (GameStateIsDrawingMap(gameState))
    {

        DrawDamageNumbers();
        UpdateButton(ui.menuButton.x,ui.menuButton.y,ui.menuButton.w,ui.menuButton.h,&ui.menuButton,*mouseState,*mouseStateLastFrame);
        DrawUIElement(&ui.menuButton,ui.menuButton.x,ui.menuButton.y,mouseState,ui.menuButton.bgColor, COLOR_FRIENDLY,false);
        
        if (gameState == GAMESTATE_INGAME)
        {
            char percentCompletionStr[NumDigits(INT_MAX)+3];
            if (currMap->automaticWinCheck || currMap->percentComplete >= 100)
            {
                if (GetNumEnemyObjects() == 0 || currMap->percentComplete >= 100 || currMap->forceNextLevelButtonEnabled)
                {
                    ui.nextLevelButton.enabled = true;
                    currMap->percentComplete = 100;


                }
                else
                    ui.nextLevelButton.enabled = false;
            }
            else
            {
                if (currMap->forceNextLevelButtonEnabled)
                {
                    ui.nextLevelButton.enabled = true;
                }
                else
                {
                    ui.nextLevelButton.enabled = false;
                }
            }
            if (ui.nextLevelButton.enabled)
            {
                DrawUIHighlight(&ui.nextLevelButton,ToWorldSpace_X(ui.nextLevelButton.x),ToWorldSpace_Y(ui.nextLevelButton.y));
            }

            Rect r = (Rect){ui.nextLevelButton.x+1,ui.nextLevelButton.y+1,ui.nextLevelButton.w-3,ui.nextLevelButton.h-3};
            #ifdef  __APPLE__
                r.y += 1;
            #endif
            UpdateButton(ui.nextLevelButton.x,ui.nextLevelButton.y,ui.nextLevelButton.w,ui.nextLevelButton.h,&ui.nextLevelButton,*mouseState,*mouseStateLastFrame);

            float distFrom = currMap->percentComplete - currMap->percentCompleteShowing;
            currMap->percentCompleteShowing = Towards(currMap->percentCompleteShowing,currMap->percentComplete, dt*10*(distFrom));

            float nrmPercentComplete = currMap->percentCompleteShowing/100.0f;

            if (currMap->percentComplete > 100) 
                nrmPercentComplete = 1;
            if (currMap->percentComplete <= 0) 
                nrmPercentComplete = 0;
            
                
            al_draw_filled_rectangle(r.x,r.y,r.x + (r.w * nrmPercentComplete),r.y+r.h,FRIENDLY);
            
            DrawUIElement(&ui.nextLevelButton,ui.nextLevelButton.x,ui.nextLevelButton.y,mouseState,ui.nextLevelButton.bgColor,COLOR_FRIENDLY,false);

            memset(percentCompletionStr,0,(NumDigits(INT_MAX)+3)*sizeof(char));
            sprintf(percentCompletionStr,"%i%%",(int)floorf(currMap->percentComplete));

            //al_draw_text(ui.font,FRIENDLY,ui.nextLevelButton.x+ui.nextLevelButton.w/2,ui.nextLevelButton.y+ui.nextLevelButton.h/2,ALLEGRO_ALIGN_CENTER,GetButtonText(&ui.nextLevelButton));
            DrawButtonText(&ui.nextLevelButton,ui.nextLevelButton.x,ui.nextLevelButton.y,FRIENDLY);
            al_set_clipping_rectangle(r.x, r.y, r.w*nrmPercentComplete, r.h);
            DrawButtonText(&ui.nextLevelButton,ui.nextLevelButton.x,ui.nextLevelButton.y,BG);
            
            //al_draw_text(ui.font,BG,ui.nextLevelButton.x+ui.nextLevelButton.w/2,ui.nextLevelButton.y+ui.nextLevelButton.h/2,ALLEGRO_ALIGN_CENTER,GetButtonText(&ui.nextLevelButton));
            al_reset_clipping_rectangle();


            if (GetButtonIsClicked(&ui.nextLevelButton))
            {
                lua_rawgeti(luaState, LUA_REGISTRYINDEX, currMap->luafunc_mapend);
                lua_pcall(luaState,0,1,0);
                bool retValue = lua_toboolean(luaState,-1);
                if (retValue)
                {
                    WinGame();
                }
            }

            DrawChestCompletionHint(ui.nextLevelButton.x,ui.nextLevelButton.y+ui.nextLevelButton.h+2);
        }

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
            DrawGoldCount(FRIENDLY,ENEMY,9,9);


    }

    if (gameState == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

        DrawAllLevelSelects(mouseState,mouseStateLastFrame, keyState, keyStateLastFrame);
        encounterMoveTimer += dt;
        encounterOffset = Towards(encounterOffset,encounterMoveTo,EaseOutQuint(encounterMoveTimer)*_SCREEN_SIZE/8.0f);
        DrawMouse(mouseState, NULL);
        DrawParticles();
        DrawTransition(dt);


        return;
    }
    if (gameState != GAMESTATE_MAIN_MENU)
         DrawUI(keyState, keyStateLastFrame, mouseState);

    if (gameState == GAMESTATE_IN_EDITOR)
    {
        DrawEditorUI(dt,*mouseState,*mouseStateLastFrame);
    }

    if (gameState == GAMESTATE_IN_SHOP)
    {
        DrawShop(dt,*mouseState,*mouseStateLastFrame);
    }
    if (gameState == GAMESTATE_END)
    {
        MoveCam(0,0);
        DrawEndScreen(mouseState,mouseStateLastFrame,dt);
        DrawParticles();
    }
     if (gameState == GAMESTATE_PURCHASING_UNITS)
    {
        DrawPurchasingUnitsUI(dt, *mouseState, *mouseStateLastFrame, keyState,keyStateLastFrame);
        DrawGoldCount(FRIENDLY,ENEMY,24,9);
    }


    DrawMenus(mouseState);
    DrawWidgets(gameState, DRAWORDER_AFTERUI);


    DrawMouse(mouseState, mousedOver);
        if (gameState == GAMESTATE_IN_3D)
    {
        Update3D(dt,*mouseStateLastFrame,*mouseState,keyState);
        VoxelRender();
    }

    players[0].clickedThisFrame = NULL;

    if (gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER) 
    {
        DrawLoadscreen();
    }

    DrawTransition(dt);

    if (al_key_down(keyState,ALLEGRO_KEY_MINUS))
        DisplayCollision();
    if (al_key_down(keyState,ALLEGRO_KEY_O) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_O))
    {
        //GameObject* g = AddGameobject(LoadPrefab("assets/enemies/prowler/prowler.lua"),mouseState->worldX,mouseState->worldY,SOURCE_SPAWNED_FROM_MAP);
        //SetOwnedBy(g, 1);
       
        //KillObj(g,true);
        //AddGold(10);
        //AddCompletionPercent(10);
    }
    //GameObjDebugDraw();
    if (al_key_down(keyState,ALLEGRO_KEY_V))
    {
        DebugDrawPathfinding(); 
        
    }
    if (al_key_down(keyState,ALLEGRO_KEY_F7))
    {
        AddGold(1000);
    }
    if (al_key_down(keyState,ALLEGRO_KEY_F8))
    {
        AddCompletionPercent(100);
    }
    #ifdef DEBUG
        al_put_blended_pixel(DEBUG_P1.x,DEBUG_P1.y,al_map_rgb(0,255,0));
        al_put_blended_pixel(DEBUG_P2.x,DEBUG_P2.y,al_map_rgb(255,0,0));
        al_put_blended_pixel(DEBUG_P3.x,DEBUG_P3.y,al_map_rgb(255,0,255));
        al_put_blended_pixel(DEBUG_P4.x,DEBUG_P4.y,al_map_rgb(255,255,255));
    #endif
}
int SortPointers(const void* a, const void* b)
{
    return (a - b);
}

void DrawMainMenu()
{
    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);
}
int main(int argc, char* args[])
{
    init();
    GenerateAngleDeg();
    ConsolePrintf("%lu\n",sizeof(ReplayFrame));

    float totalRenderTime = 0.00001f;

    double dt = 1 / (double)_TARGET_FPS * (double)(1-(double)currSettings.slowdownPercent);
    
    ALLEGRO_TIMER* _FPS_TIMER = al_create_timer(dt);

    al_start_timer(_FPS_TIMER);
    queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
   //al_register_event_source(queue, al_get_keyboard_event_source());
   al_register_event_source(queue, al_get_timer_event_source(_FPS_TIMER));

    ALLEGRO_EVENT_QUEUE* queue_KeyEvents = al_create_event_queue();
    al_register_event_source(queue_KeyEvents, al_get_keyboard_event_source());

   ALLEGRO_KEYBOARD_STATE keyStateLastFrame;
   al_get_keyboard_state(&keyStateLastFrame);
    MouseState mouseStateLastFrame;
    mouseStateLastFrame = GetMouseClamped();

    MouseState mouseState;
    ALLEGRO_KEYBOARD_STATE keyState;
    LoadSaveFile("_save.save");
    //PlayMusic("assets/audio/first_boss.wav");


    fflush(stdout);

    //EditorSetMap("assets/encounters/0/map_1.lua");

    //ConsolePrintf("%lu\n",sizeof(Attack));

    while (gameState != GAMESTATE_EXIT) {

        //sometimes event isn't registering when alt tabbed/debug mode?
        if (GameIsPaused()) 
            al_register_event_source(queue_KeyEvents,al_get_keyboard_event_source());
        

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        while (!al_is_event_queue_empty(queue_KeyEvents))
        {
            ALLEGRO_EVENT keyEvent;
            al_get_next_event(queue_KeyEvents, &keyEvent);
            
            if (gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER)
            {
                if (keyEvent.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    if (!IsModifierDown(&keyState))
                        FinishLoadScreen();
                }
            }

            if (keyEvent.keyboard.type != ALLEGRO_EVENT_KEY_CHAR)
                continue;
            switch (keyEvent.keyboard.keycode) {
                case ALLEGRO_KEY_ENTER:
                    RunLine();
                break;

                case ALLEGRO_KEY_BACKSPACE:
                    //Two events can be generated, filtering to only one
                    if (keyEvent.keyboard.type == ALLEGRO_EVENT_KEY_CHAR)
                        RemoveCharacter();
                break;

                case ALLEGRO_KEY_DELETE:

                break;

                default:
                    AddCharacter(keyEvent.keyboard.keycode, keyEvent.keyboard.unichar);
                break;
            }

            

        }


        if (gameState == GAMESTATE_IN_CHATBOX)
        {
            mouseState = GetMouseClamped();
            
            if (event.type == ALLEGRO_EVENT_KEY_DOWN || (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1)))
            {
                if (chatboxes)
                {
                    if (chatboxShowing && !chatboxShowing->allowsInteraction)
                        if (!IsModifierDown(&keyState))
                            Chatbox_NextLine(); 
                }

            }
        }
        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            /*
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_ENTER:
                    RunLine();
                break;

                case ALLEGRO_KEY_BACKSPACE:
                    RemoveCharacter();
                break;

                case ALLEGRO_KEY_DELETE:

                break;

                default:
                    ConsolePrintf("gg\n");
                    AddCharacter(event.keyboard.keycode, event.keyboard.unichar);
                break;
              }
              */
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
        {
            if (gameState == GAMESTATE_INGAME)
                ChangeUIPanel(&ui.pauseMenuPanel);
            _IS_FOCUSED_WINDOW = false;
            _UI_IGNORE_INPUT = true;
            
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
        {
            _IS_FOCUSED_WINDOW = true;

            al_set_mouse_xy(display,0,0);


        }
        #ifdef __WIN32__
            if (IsDebuggerPresent()) 
            {
                _IS_FOCUSED_WINDOW = true;
            }
        #endif
        if (event.type == ALLEGRO_EVENT_TIMER) {
            
            UpdateDebounce(dt);
            //todo: refactor this
            int targetFPSIndex = ((Pulldown*)(GetUIElement(&ui.videoOptionsPanel,"Target FPS")->data))->selectedIndex;
            currSettings.targetFPS = targetFPSIndex;
            if (targetFPSIndex == OPTION_30FPS && _TARGET_FPS != 30)
            {
                _TARGET_FPS = 30;
                al_set_timer_speed(_FPS_TIMER,1/(double)_TARGET_FPS);
            }
            if (targetFPSIndex == OPTION_60FPS && _TARGET_FPS != 60)
            {
                _TARGET_FPS = 60;
                al_set_timer_speed(_FPS_TIMER,1/(double)_TARGET_FPS);

            }
            if (targetFPSIndex == OPTION_90FPS && _TARGET_FPS != 90)
            {
                _TARGET_FPS = 90;
                al_set_timer_speed(_FPS_TIMER,1/(double)_TARGET_FPS);

            }
            if (targetFPSIndex == OPTION_144FPS && _TARGET_FPS != 144)
            {
                _TARGET_FPS = 144;
                al_set_timer_speed(_FPS_TIMER,1/(double)_TARGET_FPS);

            }
            if (targetFPSIndex == OPTION_240FPS && _TARGET_FPS != 240)
            {
                _TARGET_FPS = 240;
                al_set_timer_speed(_FPS_TIMER,1/(double)_TARGET_FPS);

            }

            dt = 1 / (double)_TARGET_FPS * (double)(1-(double)currSettings.slowdownPercent);
            _DT = dt;
            clock_t begin = clock();

            ResetSoundsThisFrame();

            int displayW = al_get_display_width(display);
            int displayH = al_get_display_height(display);

            int drawposx = displayW/2 - (_RENDERSIZE*_SCREEN_SIZE)/2; 
            int drawposy = displayH/2 - (_RENDERSIZE*_SCREEN_SIZE)/2;


            al_get_keyboard_state(&keyState);
            keyStateLua = &keyState;
            mouseState = GetMouseClamped();
            
            al_set_target_bitmap(backbuffer);
            al_clear_to_color(BG);


            al_set_target_bitmap(SCREEN);
            al_clear_to_color(BG);

            
            UpdateMusic(dt);
            UpdateReverbs(dt);

            UpdateTransition(dt);

            begin = clock();

            //if (!ui.currentPanel)
                Update(dt,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            if (!console.active)
                UpdateInterface(dt,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            if (gameState == GAMESTATE_EXIT)
                break;
            Render(dt, &mouseState, &mouseStateLastFrame, &keyState, &keyStateLastFrame);

            if (gameState == GAMESTATE_INGAME)
                RecordReplay(SCREEN);
            DrawConsole(&mouseState,&mouseStateLastFrame,&keyState, &keyStateLastFrame);
            
            UpdateScreenshake(dt);

            al_set_target_bitmap(backbuffer);
            al_draw_scaled_bitmap(SCREEN,0,0,_SCREEN_SIZE,_SCREEN_SIZE, drawposx+GetScreenshake(), drawposy,_SCREEN_SIZE*_RENDERSIZE,_SCREEN_SIZE*_RENDERSIZE,0);
            al_flip_display();
            clock_t end = clock();
            double time = (double)(end - begin) / CLOCKS_PER_SEC;

            mouseStateLastFrame = mouseState;
            keyStateLastFrame = keyState;
             _FRAMES += 60 / (float)_TARGET_FPS;
            totalRenderTime += time;

            //ConsolePrintf("Total time: %f\n",time);
            fflush(stdout);
            if (_IS_FOCUSED_WINDOW)
            {
                _UI_IGNORE_INPUT = false;
            }
        }
    }
    WriteSettingsFile("config.cfg");
    return 1;
}

