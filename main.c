
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


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
ALLEGRO_BITMAP* SCREEN;
ALLEGRO_DISPLAY* display;


int _TARGET_FPS = 60;

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
                Rect rObj = (Rect){obj->x,obj->y,al_get_bitmap_width(sp->sprite),al_get_bitmap_height(sp->sprite)};
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
                        Rect r = (Rect){g->x,g->y,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)}; 
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
void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    CheckSelected(mouseState,mouseStateLastFrame, keyState);
    UpdateAttacks(dt);
    SetControlGroups(keyState);
    GetControlGroup(keyState);
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

        Rect r = (Rect){currGameObjRunning->x,currGameObjRunning->y,w,h};

        bool shouldMove = true;
        bool shouldAttack = false;
        ProcessEffects(currGameObjRunning,dt);
        ProcessShields(currGameObjRunning,dt);
        if (currGameObjRunning->targObj && currGameObjRunning->queue[0].commandType == COMMAND_ATTACK) 
        {
            GameObject* tempAttack = currGameObjRunning->targObj;

            if (currGameObjRunning->properties & OBJ_ACTIVE)
            {
                int wTarg = al_get_bitmap_width(sprites[currGameObjRunning->targObj->spriteIndex].sprite);
                int hTarg = al_get_bitmap_height(sprites[currGameObjRunning->targObj->spriteIndex].sprite);

                currGameObjRunning->xtarg = currGameObjRunning->targObj->x + wTarg/2;
                currGameObjRunning->ytarg = currGameObjRunning->targObj->y + hTarg/2;

                Rect r2 = (Rect){currGameObjRunning->targObj->x,currGameObjRunning->targObj->y,wTarg,hTarg};
                #define DISTDELTA 0.001f
                Rect unioned = UnionRect(r,r2);
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
void Render(float dt, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    al_hide_mouse_cursor(display);
    al_grab_mouse(display);

    al_set_target_bitmap(SCREEN);
    DrawSprite(&sprites[currMap->spriteIndex],0,0,GROUND,false);
    DrawAttacks(dt);

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
    }
    DrawUI(keyState, keyStateLastFrame, mouseState);

    if (al_key_down(keyState,ALLEGRO_KEY_F) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_F))
    {
       //AddAnimationEffect("Assets/UI/slash_fx.png", mouseState->x, mouseState->y, 32,32, 0.1, 0);
        //AddAnimationEffect("Assets/UI/slash_fx2.png", mouseState->x, mouseState->y, 16, 16, 0.05f, 0);
        //AddAnimationEffect_Prefab(&animationEffectsPrefabs[0],0, mouseState->x,mouseState->y);
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
    players[0].clickedThisFrame = NULL;
}
int main(int argc, char* args[])
{
    srand(time(NULL));
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();
    init_lua();
    InitAttacks();
    InitParticles();
    InitAnimationEffects();
  //  init_sprites(); 

    init();

    BG = al_map_rgba(40,32,36,255);
    GROUND = al_map_rgba(115,119,148,255);

    ENEMY = al_map_rgba(60,255,255,255);
    FRIENDLY = al_map_rgba(255,255,255,255);

    WHITE = al_map_rgba(255,255,255,255);
  
    HEAL = al_map_rgba(92,204,12,255);
    POISON = al_map_rgba(195,94,241,255);
    DAMAGE = al_map_rgba(248,100,100,255);



    //dodge a lot of crashes by setting the 0th sprite to a zeroed bitmap
    sprites[0].sprite = al_create_bitmap(0,0);
    sprites[0].inverseSprite = al_create_bitmap(0,0);


    SCREEN = al_create_bitmap(256,256);

    display = al_create_display(256*_RENDERSIZE,256*_RENDERSIZE);
    ALLEGRO_BITMAP* backbuffer = al_get_backbuffer(display);

    ALLEGRO_FONT* font = al_load_ttf_font("Assets/Fonts/Roboto-Medium.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.font = font;
    ui.panel_sprite_index = LoadSprite("Assets/UI/ui.png",false);
    //ui.cursorDefaultIndex = LoadSprite("Assets/UI/cursor.png",false);
    LoadCursorSprite(&ui,&ui.cursorDefaultIndex,"Assets/UI/cursor.png");
    LoadCursorSprite(&ui,&ui.cursorCastingIndex,"Assets/UI/cursor_cast.png");
    LoadCursorSprite(&ui,&ui.cursorAttackIndex,"Assets/UI/cursor_attack.png");
    LoadCursorSprite(&ui,&ui.cursorFriendlyIndex,"Assets/UI/cursor_friendly.png");

    //int* s = LoadSprite("Encounters/01/map.png");

    //GameObject boss;
    //memset(&boss,0,sizeof(GameObject));
    //loadLuaGameObj(luaState,"Encounters/01/boss.lua",&boss);

    //lua_rawgeti(luaState,LUA_REGISTRYINDEX,boss.luafunc_setup);
    //lua_pcall(luaState,0,0,0);

    Map* m = LoadMap("Assets/Encounters/01/map.lua");  
    SetMap(&maps[0]);
    PreprocessMap(m);
    
    GameObject* g = LoadPrefab("Assets/Friendly/Bard/bard.lua");
    SetOwnedBy(g, 0);

    GameObject* warrior = LoadPrefab("Assets/Friendly/Warrior/warrior.lua");
    SetOwnedBy(warrior, 0);

    GameObject* p = LoadPrefab("Assets/Friendly/Priest/priest.lua");
    SetOwnedBy(p, 0);

    GameObject* r = LoadPrefab("Assets/Friendly/Rogue/rogue.lua");
    SetOwnedBy(p, 0);



    GameObject* g1 = AddGameobject(g,100,150);
    GameObject* warr = AddGameobject(warrior,128,150);
    GameObject* priest = AddGameobject(p,180,160);
    GameObject* rogue = AddGameobject(r,201,180);




    //AddGameobject(g)->x = 50;
   // AddGameobject(g)->x = 100;



    fflush(stdout);
    ALLEGRO_TIMER* _FPS_TIMER = al_create_timer(1.0f / (double)_TARGET_FPS);

    al_start_timer(_FPS_TIMER);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
   al_register_event_source(queue, al_get_timer_event_source(_FPS_TIMER));

   ALLEGRO_KEYBOARD_STATE keyboardStateLastFrame;
   al_get_keyboard_state(&keyboardStateLastFrame);
   ALLEGRO_MOUSE_STATE mouseStateLastFrame;
    mouseStateLastFrame = GetMouseClamped();

    while (true) {
        
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) {
            ALLEGRO_MOUSE_STATE mouseState;
            ALLEGRO_KEYBOARD_STATE keyboardState;

            al_get_keyboard_state(&keyboardState);
            mouseState = GetMouseClamped();

            al_set_target_bitmap(SCREEN);
            al_clear_to_color(al_map_rgb(40,32,36));

            Update(1/(float)_TARGET_FPS,&keyboardState,&mouseState, &keyboardStateLastFrame, &mouseStateLastFrame);
            Render(1/(float)_TARGET_FPS, &mouseState, &mouseStateLastFrame, &keyboardState, &keyboardStateLastFrame);
            al_set_target_bitmap(backbuffer);
            al_draw_scaled_bitmap(SCREEN,0,0,_SCREEN_SIZE,_SCREEN_SIZE,0,0,_SCREEN_SIZE*_RENDERSIZE,_SCREEN_SIZE*_RENDERSIZE,0);

            al_flip_display();

            mouseStateLastFrame = mouseState;
            keyboardStateLastFrame = keyboardState;
            //printf("\n");
            _FRAMES++;
            fflush(stdout);
        }

    }
}

