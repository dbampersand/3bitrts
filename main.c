
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
void CheckSelected(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseLastFrame)
{
    if (!(mouseLastFrame->buttons & 1)  && (mouseState->buttons & 1))
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
                    SetSelected(obj,true);
                    players[0].selection[players[0].numUnitsSelected] = obj;
                    players[0].numUnitsSelected++;

                }
            }
        }
        players[0].selecting = false;
    }
    if (!(mouseState->buttons & 2) && (mouseLastFrame->buttons & 2))
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

                g->xtarg = mouseState->x - w/2;
                g->ytarg = mouseState->y - h/2;
            }
            Sprite* s = &sprites[g->spriteIndex];
            Rect r = (Rect){g->x,g->y,al_get_bitmap_width(s->sprite),al_get_bitmap_height(s->sprite)}; 
            if (PointInRect(mouseState->x,mouseState->y,r))
            {
                for (int i = 0; i < players[0].numUnitsSelected; i++)
                {
                    SetAttackingObj(players[0].selection[i],g);
                }
                break;
            }
        }
    }
}
//ALLEGRO_MOUSE_STATE mouseStateLastFrame;
//ALLEGRO_KEYBOARD_STATE keyStateLastFrame;

void Update(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame)
{
    CheckSelected(mouseState,mouseStateLastFrame);
    UpdateAttacks(dt);
    for (int i = 0; i < numObjects; i++)
    {
        currGameObjRunning = &objects[i];
        if (currGameObjRunning->properties & OBJ_ACTIVE && !IsOwnedByPlayer(currGameObjRunning))
        {
            DoAI(currGameObjRunning);
        }

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

        if (currGameObjRunning->targObj)
        {
            if (currGameObjRunning->properties & OBJ_ACTIVE)
            {
                int wTarg = al_get_bitmap_width(sprites[currGameObjRunning->targObj->spriteIndex].sprite);
                int hTarg = al_get_bitmap_height(sprites[currGameObjRunning->targObj->spriteIndex].sprite);

                currGameObjRunning->xtarg = currGameObjRunning->targObj->x + wTarg/2;
                currGameObjRunning->ytarg = currGameObjRunning->targObj->y + hTarg/2;

                Rect r2 = (Rect){currGameObjRunning->targObj->x,currGameObjRunning->targObj->y,wTarg,hTarg};
                #define DISTDELTA 0.001f
                Rect unioned = UnionRect(r,r2);
                if (RectsTouch(r, r2, DISTDELTA))
                {
                    shouldMove = false;
                    shouldAttack = true;
                }
            }   
            else
            {
                SetAttackingObj(currGameObjRunning, NULL);
            }
            if (shouldAttack)
            {
                if (currGameObjRunning->attackTimer <= 0)
                {
                    AttackTarget(currGameObjRunning);
                    currGameObjRunning->attackTimer = currGameObjRunning->attackSpeed;
                }
            }

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
        if (players[0].selection[0])
        {
            if (players[0].selection[0]->abilities[index].cdTimer <= 0)
            {

                players[0].abilityHeld = NULL;
                currGameObjRunning = players[0].selection[0];
                currAbilityRunning = &players[0].selection[0]->abilities[index];
                if (currAbilityRunning->castType == ABILITY_INSTANT || currAbilityRunning->castType == ABILITY_TOGGLE)
                {
                    CastAbility(currGameObjRunning, currAbilityRunning,0,0,0,0,currGameObjRunning);
                }
                else
                {
                    players[0].abilityHeld = currAbilityRunning;
                }
            }


        }
    }
    if (!al_key_down(keyState,ALLEGRO_KEY_ESCAPE) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_ESCAPE))
    {
        players[0].abilityHeld = NULL;
    }
    if (mouseState->buttons & 1) 
    {
        currGameObjRunning = players[0].selection[0];
        currAbilityRunning = players[0].abilityHeld;

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
            CastAbility(currGameObjRunning,currAbilityRunning,mouseState->x,mouseState->y,mouseState->x-midX,mouseState->y-midY,target);

        }
        currAbilityRunning = NULL;
        players[0].abilityHeld = NULL;
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

    int objSelected = -1;
    bool abilityCastOnTarget = players[0].abilityHeld && (mouseStateLastFrame->buttons & 1 || mouseState->buttons & 1);
    if (abilityCastOnTarget)
    {
        if (players[0].abilityHeld->castType == ABILITY_TARGET_ENEMY || players[0].abilityHeld->castType == ABILITY_TARGET_ALL)
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
        if (i == objSelected)
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
        
    DrawAttacks(dt);
    DrawParticles();
    if (players[0].abilityHeld)
    {
        float cx; float cy;
        GetCentre(players[0].selection[0], &cx, &cy);
        al_draw_circle(cx,cy,players[0].abilityHeld->range,FRIENDLY,0);
    }
    DrawUI(keyState, keyStateLastFrame);

    if (al_key_down(keyState,ALLEGRO_KEY_F) && !al_key_down(keyStateLastFrame,ALLEGRO_KEY_F))
    {
       //AddAnimationEffect("Assets/UI/slash_fx.png", mouseState->x, mouseState->y, 32,32, 0.1, 0);
        //AddAnimationEffect("Assets/UI/slash_fx2.png", mouseState->x, mouseState->y, 16, 16, 0.05f, 0);
        //AddAnimationEffect_Prefab(&animationEffectsPrefabs[0],0, mouseState->x,mouseState->y);
    }   
    DrawAnimationEffects();
    if (players[0].abilityHeld)
    {
        DrawCursor(mouseState, ui.cursorCastingIndex, false);
    }
    else 
    {
        DrawCursor(mouseState, ui.cursorDefaultIndex, false);
    }
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
    FRIENDLY = al_map_rgba(255,255,255,255);
    ENEMY = al_map_rgba(255,131,131,255);
    GROUND = al_map_rgba(94,98,134,255);
    WHITE = al_map_rgba(255,255,255,255);

    //dodge a lot of crashes by setting the 0th sprite to a zeroed bitmap
    sprites[0].sprite = al_create_bitmap(0,0);
    sprites[0].inverseSprite = al_create_bitmap(0,0);


    SCREEN = al_create_bitmap(256,256);

    display = al_create_display(256*_RENDERSIZE,256*_RENDERSIZE);
    ALLEGRO_BITMAP* backbuffer = al_get_backbuffer(display);

    ALLEGRO_FONT* font = al_load_ttf_font("Assets/Fonts/Roboto-Medium.ttf", 64, ALLEGRO_TTF_MONOCHROME);
    ui.panel_sprite_index = LoadSprite("Assets/UI/ui.png",false);
    //ui.cursorDefaultIndex = LoadSprite("Assets/UI/cursor.png",false);
    LoadCursorSprite(&ui,&ui.cursorDefaultIndex,"Assets/UI/cursor.png");
    LoadCursorSprite(&ui,&ui.cursorCastingIndex,"Assets/UI/cursor_cast.png");

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
    g->speed = 50;
    SetOwnedBy(g, 0);

    //GameObject* warrior = LoadPrefab("Assets/Friendly/Bard/bard.lua");
    //warrior->speed = 50;
    //SetOwnedBy(warrior, 0);


    GameObject* g1 = AddGameobject(g);
    GameObject* warr = AddGameobject(g);

    g1->x = 100;
    g1->y = 150;
    g1->xtarg = 100;
    g1->ytarg = 150;

   // GameObject* warr = AddGameobject(warrior);
    warr->x = 128;
    warr->y = 150;
    warr->xtarg = 128;
    warr->ytarg = 150;




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

