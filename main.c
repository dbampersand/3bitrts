
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

bool shouldExit;
void init()
{
    objects = calloc(MAX_OBJS,sizeof(GameObject));
    numObjects = 0;
    objectsAllocated = MAX_OBJS;

    freeObjs = calloc(MAX_OBJS,sizeof(GameObject*));
    numFreeObjs = MAX_OBJS;
    for (int i = 0; i < MAX_OBJS; i++)
    {
        freeObjs[i] = &objects[i];
    }


    prefabs = calloc(1,sizeof(GameObject));
    numPrefabs = 0;
    numPrefabsAllocated = 1;

    players = calloc(2,sizeof(GameObject));

    gameState = MAIN_MENU;
    ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Start");

    ui.panelShownPercent = 1.0f;
    numMaps = 0;
    numSprites=1;
    maxSprites = 0;
    _FRAMES = 0;

    numAnimationEffectsPrefabs = 0;

    memset(&gameOptions,0,sizeof(GameOptions));

    
}
void CheckSelected(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseLastFrame, ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (!(mouseLastFrame->buttons & 1)  && (mouseState->buttons & 1) && !players[0].abilityHeld)
    {
        AddMouseRandomParticles(*mouseState, 3);
        players[0].selecting = true;
        players[0].selectionStart = (Vector2){mouseState->x,mouseState->y};
    }
    if (players[0].selecting)
    {
    }
    if (!(mouseState->buttons & 1))
    {
        if (players[0].selecting)
        {
            Vector2 endSelection = (Vector2){mouseState->x,mouseState->y};
            Rect r;
            r.x = _MIN(endSelection.x,players[0].selectionStart.x);
            r.y = _MIN(endSelection.y,players[0].selectionStart.y);
            r.w = _MAX(endSelection.x,players[0].selectionStart.x) - _MIN(endSelection.x,players[0].selectionStart.x);
            r.h = _MAX(endSelection.y,players[0].selectionStart.y) - _MIN(endSelection.y,players[0].selectionStart.y);
            bool hasSelected = false;
            
            for (int i = 0; i < numObjects; i++)
            {
                GameObject* obj = &objects[i];
                if (!IsOwnedByPlayer(obj) || !IsActive(obj))
                    continue;
                Sprite* sp = &sprites[obj->spriteIndex];
                int j = al_get_bitmap_width(sp->sprite);
                Rect rObj = (Rect){obj->position.x,obj->position.y,al_get_bitmap_width(sp->sprite),al_get_bitmap_height(sp->sprite)};
                if (CheckIntersect(rObj,r))
                {
                    if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                    if (!hasSelected)
                    {
                        for (int j = 0; j < numObjects; j++)
                        {
                            SetSelected(&objects[j],false);
                            for (int i = 0; i < players[0].numUnitsSelected; i++)
                            {
                                players[0].selection[i] = NULL;
                            }
                            players[0].numUnitsSelected = 0;
                        }
                        hasSelected = true;
                    }
                    if (al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                    {
                        bool selected = IsSelected(obj);
                        SetSelected(obj,!selected);
                        //we're removing the unit from selection as it is already selected 
                        if (selected)
                        {
                            RemoveGameObjectFromSelection(&players[0],obj);
                        }
                    }
                    else
                    {
                        SetSelected(obj,true);
                        players[0].selection[players[0].numUnitsSelected] = obj;
                        players[0].numUnitsSelected++;

                    }
                    if (hasSelected) 
                        players[0].indexSelectedUnit = 0; 
                }
            }
        }
        players[0].selecting = false;
    }
    if (!(mouseState->buttons & 2) && (mouseLastFrame->buttons & 2))
    {
        if (!IsInsideUI(mouseState->x,mouseState->y))
        {
            if (players[0].abilityHeld == NULL)
                    {
                        AddMouseRandomParticles(*mouseState, 3);
                    }
                    for (int i = 0; i < players[0].numUnitsSelected; i++)
                    {
                        SetAttackingObj(players[0].selection[i],NULL);
                    }

                    for (int i = 0; i < numObjects; i++)
                    {
                        GameObject* g = &objects[i];
                        if (!IsActive(g))
                            continue;
                        if (IsSelected(g))
                        {
                            int w = al_get_bitmap_width(sprites[g->spriteIndex].sprite);
                            int h = al_get_bitmap_height(sprites[g->spriteIndex].sprite);
                                if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                                    ClearCommandQueue(g);
                            MoveCommand(g,mouseState->x-w/2,mouseState->y-h/2);
                        
                        // g->xtarg = mouseState->x - w/2;
                            //g->ytarg = mouseState->y - h/2;
                        }
                        Sprite* s = &sprites[g->spriteIndex];
                        Rect r = (Rect){g->position.x,g->position.y,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)}; 
                        if (PointInRect(mouseState->x,mouseState->y,r))
                        {
                            for (int i = 0; i < players[0].numUnitsSelected; i++)
                            {
                                if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                                    ClearCommandQueue(players[0].selection[i]);
                                AttackCommand(players[0].selection[i],g);
                                //SetAttackingObj(players[0].selection[i],g);
                            }
                            break;
                        }
                    }
        }
    
        
    }
}
//ALLEGRO_MOUSE_STATE mouseStateLastFrame;
//ALLEGRO_KEYBOARD_STATE keyStateLastFrame;
void SetCtrlGroup(int index, GameObject** list, int numUnitsSelected)
{
    if (list)
    {
        numUnitsSelected = numUnitsSelected > MAXUNITSSELECTED ? MAXUNITSSELECTED : numUnitsSelected;
        for (int i = 0; i < MAXUNITSSELECTED; i++)
        {
            players[0].controlGroups[index][i] = list[i];
        }
        //memcpy(players[0].controlGroups[index],list,numUnitsSelected*sizeof(GameObject*));
    }
}
int GetCtrlGroup(int index)
{
    //memcpy(players[0].selection,players[0].controlGroups[index],MAXUNITSSELECTED*sizeof(GameObject*));
    int count = 0; 
    for (int i = 0; i < MAXUNITSSELECTED; i++)
    {
        players[0].selection[i] = players[0].controlGroups[index][i];
        if (players[0].selection[i] != NULL)
        {
            count++;
            players[0].selection[i]->properties |= OBJ_SELECTED;
        }
    }
    return count; 

}
void SetControlGroups(ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (al_key_down(keyState,ALLEGRO_KEY_LCTRL))
    {
        if (al_key_down(keyState,ALLEGRO_KEY_1))
        {
            SetCtrlGroup(1,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_2))
        {
            SetCtrlGroup(2,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_3))
        {
            SetCtrlGroup(3,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_4))
        {
            SetCtrlGroup(4,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_5))
        {
            SetCtrlGroup(5,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_6))
        {
            SetCtrlGroup(6,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_7))
        {
            SetCtrlGroup(7,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_8))
        {
            SetCtrlGroup(8,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_9))
        {
            SetCtrlGroup(9,players[0].selection,players[0].numUnitsSelected);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_0))
        {
            SetCtrlGroup(0,players[0].selection,players[0].numUnitsSelected);
        }
    }    
}
void GetControlGroup(ALLEGRO_KEYBOARD_STATE* keyState)
{
     if (!al_key_down(keyState,ALLEGRO_KEY_LCTRL))
    {
        if (al_key_down(keyState,ALLEGRO_KEY_1))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(1);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_2))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(2);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_3))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(3);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_4))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(4);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_5))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(5);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_6))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(6);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_7))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(7);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_8))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(8);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_9))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(9);
        }
         if (al_key_down(keyState,ALLEGRO_KEY_0))
        {
            UnsetAll();
            players[0].numUnitsSelected = GetCtrlGroup(0);
        }
    }  
}
void UpdateInterface(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    UpdateUI(keyState,mouseState,keyStateLastFrame,mouseStateLastFrame,dt);
    if (ui.currentPanel == &ui.mainMenuPanel)
    {
        if (GetButton(&ui.mainMenuPanel,"Return"))
        {
            ui.currentPanel = NULL;
            if (gameState == MAIN_MENU)
            {
                gameState = CHOOSING_ENCOUNTER;
                ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");
            }
        }
        if (GetButton(&ui.mainMenuPanel,"Options"))
        {
            ChangeUIPanel(&ui.videoOptionsPanel);
        }
        if (GetButton(&ui.mainMenuPanel,"Exit"))
        {
            shouldExit = true;
        }
    }
    SetOptions();
}
void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    lua_settop(luaState,0);
    CheckSelected(mouseState,mouseStateLastFrame, keyState);
    UpdateAttacks(dt);
    SetControlGroups(keyState);
    GetControlGroup(keyState);
    UpdateDamageNumbers(dt);
    if (al_key_down(keyState, ALLEGRO_KEY_A) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_A))
    {
        players[0].amoveSelected = true;
    }
    if (players[0].abilityHeld) 
        players[0].amoveSelected = false;
    if (al_key_down(keyState, ALLEGRO_KEY_TAB) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_TAB))
    {
        players[0].indexSelectedUnit++;
        if (players[0].indexSelectedUnit >= MAXUNITSSELECTED || players[0].indexSelectedUnit >= players[0].numUnitsSelected)
        {
            players[0].indexSelectedUnit = 0;
        }
    }
    for (int i = 0; i < numObjects; i++)
    {
        currGameObjRunning = &objects[i];
        if (!IsActive(currGameObjRunning))
            continue;
        UpdateChannellingdObj(currGameObjRunning,dt);
        currGameObjRunning->invulnerableTime -= dt;
        if (currGameObjRunning->invulnerableTime < 0)
            currGameObjRunning->invulnerableTime = 0;
        if (currGameObjRunning->properties & OBJ_ACTIVE && !IsOwnedByPlayer(currGameObjRunning))
        {
            DoAI(currGameObjRunning);
        }
        DoCommands(currGameObjRunning);
        currGameObjRunning->attackTimer -= dt;
        if (currGameObjRunning->attackTimer < 0)
            currGameObjRunning->attackTimer = 0;
        for (int i = 0; i < 4; i++)
        {
            currGameObjRunning->abilities[i].cdTimer -= dt;
            if (currGameObjRunning->abilities[i].cdTimer < 0)
                currGameObjRunning->abilities[i].cdTimer = 0;

        }

        int w = al_get_bitmap_width(sprites[currGameObjRunning->spriteIndex].sprite);
        int h = al_get_bitmap_height(sprites[currGameObjRunning->spriteIndex].sprite);

        Rect r = (Rect){currGameObjRunning->position.x,currGameObjRunning->position.y,w,h};

        bool shouldMove = true;
        bool shouldAttack = false;
        ProcessEffects(currGameObjRunning,dt);
        ProcessShields(currGameObjRunning,dt);
           // if (currGameObjRunning->queue[0].commandType == COMMAND_CAST &&  RectDist(currGameObjRunning,currGameObjRunning->targObj) < currGameObjRunning->queue[0].ability->range+DISTDELTA)
            //{
              //  shouldMove = true;
            //}

        if (currGameObjRunning->targObj && currGameObjRunning->queue[0].commandType == COMMAND_ATTACK) 
        {
            GameObject* tempAttack = currGameObjRunning->targObj;

            if (currGameObjRunning->properties & OBJ_ACTIVE)
            {
                int wTarg = al_get_bitmap_width(sprites[currGameObjRunning->targObj->spriteIndex].sprite);
                int hTarg = al_get_bitmap_height(sprites[currGameObjRunning->targObj->spriteIndex].sprite);

                currGameObjRunning->targetPosition.x = currGameObjRunning->targObj->position.x + wTarg/2;
                currGameObjRunning->targetPosition.y = currGameObjRunning->targObj->position.y + hTarg/2;

                Rect r2 = (Rect){currGameObjRunning->targObj->position.x,currGameObjRunning->targObj->position.y,wTarg,hTarg};
                #define DISTDELTA 0.001f
                Rect unioned = UnionRectR(r,r2);
                //if (RectsTouch(r, r2, currGameObjRunning->range+DISTDELTA))
                
                if (RectDist(currGameObjRunning,currGameObjRunning->targObj) < currGameObjRunning->range+DISTDELTA)
                {
                    shouldMove = false;
                    shouldAttack = true;
                }
                else
                {
                    //if we're AI controlled and the object moves out of range 
                    //but something is still in range - temporarily attack that, but keep moving towards the original target
                    if (GetPlayerOwnedBy(currGameObjRunning) == 1)
                    {
                        for (int i = 0; i < MAX_OBJS; i++)
                        {
                            GameObject* g2 = &objects[i];
                            if (IsActive(g2))
                            {
                                if (GetPlayerOwnedBy(g2) != GetPlayerOwnedBy(currGameObjRunning))
                                {
                                    if (RectDist(currGameObjRunning,g2) < currGameObjRunning->range+DISTDELTA)
                                    {
                                        tempAttack = g2;
                                        shouldMove = true;
                                        shouldAttack = true;
                                        break;
                                    }
                                    
                                }
                            }
                        }

                    }
                }
            }   
            else
            {
                SetAttackingObj(currGameObjRunning, NULL);
            }
            GameObject* old = currGameObjRunning->targObj;
            currGameObjRunning->targObj = tempAttack;

            if (shouldAttack)
            {

                if (currGameObjRunning->attackTimer <= 0)
                {
                    AttackTarget(currGameObjRunning);
                    currGameObjRunning->attackTimer = currGameObjRunning->attackSpeed;
                }

            }
            currGameObjRunning->targObj = old;
        }
        if (shouldMove)
            Move(currGameObjRunning, dt);
        if (currGameObjRunning->properties & OBJ_ACTIVE)
        {
            lua_rawgeti(luaState,LUA_REGISTRYINDEX,objects[i].luafunc_update);
            lua_pcall(luaState,0,0,0);
        }
    }
    if (1)
    {
        int index = -1;
        if (!al_key_down(keyState,ALLEGRO_KEY_Q) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_Q))
        {
            index = 0;
        }
        if (!al_key_down(keyState,ALLEGRO_KEY_W) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_W))
        {
            index = 1;
        }
        if (!al_key_down(keyState,ALLEGRO_KEY_E) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_E))
        {
            index = 2;
        }
        if (!al_key_down(keyState,ALLEGRO_KEY_R) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_R))
        {
            index = 3;
        }


        if (index > -1)
        if (players[0].selection[players[0].indexSelectedUnit])
        {
            if (players[0].selection[players[0].indexSelectedUnit]->abilities[index].cdTimer <= 0)
            {
                PlaySound(ui.uiClickedSound);
                players[0].abilityHeld = NULL;
                currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
                if (currGameObjRunning)
                {
                    currAbilityRunning = &players[0].selection[players[0].indexSelectedUnit]->abilities[index];

                    if (currAbilityRunning->castType == ABILITY_INSTANT || currAbilityRunning->castType == ABILITY_TOGGLE)
                    {
                        if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                            ClearCommandQueue(currGameObjRunning);
                        CastCommand(currGameObjRunning,NULL,currAbilityRunning,mouseState->x,mouseState->y);

                        //if (AbilityCanBeCast(currAbilityRunning, currGameObjRunning, currGameObjRunning))
                        //  CastAbility(currGameObjRunning, currAbilityRunning,0,0,0,0,currGameObjRunning);
                    }
                    else
                    {
                        players[0].abilityHeld = currAbilityRunning;
                    }
                }
            }


        }
    }
    if (!al_key_down(keyState,ALLEGRO_KEY_ESCAPE) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_ESCAPE) )
    {
        players[0].abilityHeld = NULL;
        players[0].amoveSelected = false;
    }
    if (mouseState->buttons & 1) 
    {
        currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
        currAbilityRunning = players[0].abilityHeld;
        if (IsInsideUI(mouseState->x,mouseState->y))
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
        else
        {
            if (players[0].amoveSelected)
            {
                players[0].amoveSelected = false;   
                for (int i = 0; i < numObjects; i++)
                {
                    GameObject* g = &objects[i];
                    if (IsSelected(g))
                    {
                        float w; float h; GetOffsetCenter(g,&w,&h);
                        if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                            ClearCommandQueue(g);

                        AttackMoveCommand(g,mouseState->x-w/2,mouseState->y-h/2);
                        
                    }

                }
            }
        if (players[0].abilityHeld)
        {
            GameObject* target = NULL;
            for (int i = 0; i < MAX_OBJS; i++)
            {
                if (objects[i].properties & OBJ_ACTIVE)
                {
                    if (PointInRect(mouseState->x,mouseState->y,GetObjRect(&objects[i])))
                    {
                        target = &objects[i];
                        break;
                    }
                }
            }
            float midX=0; float midY=0;
            GetOffsetCenter(currGameObjRunning,&midX,&midY);
            
            //if (AbilityCanBeCast(currAbilityRunning,currGameObjRunning,target))
            {
                if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                    ClearCommandQueue(currGameObjRunning);
                CastCommand(currGameObjRunning,target,currAbilityRunning,mouseState->x,mouseState->y);
                //CastAbility(currGameObjRunning,currAbilityRunning,mouseState->x,mouseState->y,mouseState->x-midX,mouseState->y-midY,target);
                players[0].clickedThisFrame = target;

            }
            //else
            {
            }
        }
        currAbilityRunning = NULL;
        players[0].abilityHeld = NULL;

        }

    }
    if (!(mouseState->buttons & 2) && (mouseStateLastFrame->buttons & 2))
    {
        currAbilityRunning = NULL;
        players[0].abilityHeld = NULL;

    }
    if (players[0].abilityHeld)
    {
        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
    }
    else
    {
        al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW);

    }



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
            ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");

            Encounter* e = encounters[selectedEncounterIndex];
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
    srand(time(NULL));
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();
    al_install_audio();
    InitSound();
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);

    _RENDERSIZE = _MIN(monitor.x2,monitor.y2)/256 -1;



    init();

    init_lua();
    InitAttacks();
    InitParticles();
    InitAnimationEffects();
    InitUI();
  //  init_sprites(); 


    BG = al_map_rgba(40,32,36,255);
    GROUND = al_map_rgba(115,119,148,255);

    ENEMY = al_map_rgba(60,255,255,255);
    FRIENDLY = al_map_rgba(255,255,255,255);

    WHITE = al_map_rgba(255,255,255,255);
  
    HEAL = al_map_rgba(92,204,12,255);
    POISON = al_map_rgba(195,94,241,255);
    DAMAGE = al_map_rgba(248,100,100,255);

    LoadEncounters("assets/encounters",luaState);


    int resX = 256*_RENDERSIZE;
    int resY = 256*_RENDERSIZE;
    SCREEN = al_create_bitmap(256,256);
    background_screen = al_create_bitmap(monitor.x2,monitor.y2);

    //al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

    //display = al_create_display(256*_RENDERSIZE,256*_RENDERSIZE);
    display = al_create_display(monitor.x2,monitor.y2);
    
    backbuffer = al_get_backbuffer(display);

    ALLEGRO_FONT* font = al_load_ttf_font("assets/fonts/font.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.boldFont = al_load_ttf_font("assets/fonts/fontbold.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.tinyFont = al_load_ttf_font("assets/fonts/4x8.ttf", 8, ALLEGRO_TTF_MONOCHROME);

    ui.font = font;
    ui.panel_sprite_index = LoadSprite("assets/ui/ui.png",false);

    LoadCursorSprite(&ui,&ui.cursorDefaultIndex,"assets/ui/cursor.png");
    LoadCursorSprite(&ui,&ui.cursorCastingIndex,"assets/ui/cursor_cast.png");
    LoadCursorSprite(&ui,&ui.cursorAttackIndex,"assets/ui/cursor_attack.png");
    LoadCursorSprite(&ui,&ui.cursorFriendlyIndex,"assets/ui/cursor_friendly.png");

    //int* s = LoadSprite("Encounters/01/map.png");

    //GameObject boss;
    //memset(&boss,0,sizeof(GameObject));
    //loadLuaGameObj(luaState,"Encounters/01/boss.lua",&boss);

    //lua_rawgeti(luaState,LUA_REGISTRYINDEX,boss.luafunc_setup);
    //lua_pcall(luaState,0,0,0);
    //Map* selectionMap = 
    Map* m = LoadMap("assets/ui/map_unitselect.lua");  
    SetMap(&maps[0]);
    
    LoadPrefabs("assets/friendly");

    int xPos = 100;
    for (int i = 0; i < numPrefabs; i++)
    {
        if (prefabs[i].playerChoosable == true)
        {
            AddGameobject(&prefabs[i],xPos,150);
            xPos+=GetWidth(&prefabs[i]);
        }   

    }




    //AddGameobject(g)->x = 50;
   // AddGameobject(g)->x = 100;


    printf("gggg\n"); fflush(stdout);

    printf("bbbb\n"); fflush(stdout);

    fflush(stdout);
    ALLEGRO_TIMER* _FPS_TIMER = al_create_timer(1.0f / (double)_TARGET_FPS);

    al_start_timer(_FPS_TIMER);
    queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
   al_register_event_source(queue, al_get_timer_event_source(_FPS_TIMER));

   ALLEGRO_KEYBOARD_STATE keyStateLastFrame;
   al_get_keyboard_state(&keyStateLastFrame);
   ALLEGRO_MOUSE_STATE mouseStateLastFrame;
    mouseStateLastFrame = GetMouseClamped();
    if (gameState == MAIN_MENU)
    {
        ui.currentPanel = &ui.mainMenuPanel;
        ui.panelShownPercent=1.0f;
        ui.animatePanel = UI_ANIMATE_STATIC;
    }

    while (!shouldExit) {
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
            //al_draw_bitmap(backbuffer,0,0,0);
            al_clear_to_color(BG);


            al_set_target_bitmap(SCREEN);
            al_clear_to_color(BG);

            if (!ui.currentPanel)
                Update(1/(float)_TARGET_FPS,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            UpdateInterface(1/(float)_TARGET_FPS,&keyState,&mouseState, &keyStateLastFrame, &mouseStateLastFrame);
            Render(1/(float)_TARGET_FPS, &mouseState, &mouseStateLastFrame, &keyState, &keyStateLastFrame);

            al_set_target_bitmap(backbuffer);

            al_draw_scaled_bitmap(SCREEN,0,0,_SCREEN_SIZE,_SCREEN_SIZE, drawposx, drawposy,_SCREEN_SIZE*_RENDERSIZE,_SCREEN_SIZE*_RENDERSIZE,0);

            al_flip_display();

            mouseStateLastFrame = mouseState;
            keyStateLastFrame = keyState;
            //printf("\n");
            _FRAMES++;
            fflush(stdout);
        }
    }
    return 1;
}

