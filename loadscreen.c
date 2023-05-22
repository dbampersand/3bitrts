#include "loadscreen.h"
#include "sprite.h"
#include "stdio.h"
#include "math.h"
#include "colors.h"
#include "video.h" 
#include "pointspace.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro.h"
#include "ui.h"
#include "encounter.h"
#include "luafuncs.h"
#include "helperfuncs.h"

LoadScreen loadScreen = {0};

void InitLoadscreen(char* initialPath, char* mask)
{
    memset(&loadScreen,0,sizeof(LoadScreen));
    SetLoadscreen(initialPath,mask,1,3.5,0,2,3,GAME_NAME,"Press any key to continue.");
    const char* clouds[] =  {
                    "assets/ui/startscreen/clouds/cloud_01.png",
                    "assets/ui/startscreen/clouds/cloud_02.png",
                    "assets/ui/startscreen/clouds/cloud_03.png"
                    };
    for (int i = 0; i < 8; i++)
    {
        LoadScreenSprite* sp = &loadScreen.sprites[i];
        sp->spriteIndex = LoadSprite(clouds[rand()%(sizeof(clouds)/sizeof(clouds[0]))],false);
        sp->position.x = -GetWidthSprite(&sprites[sp->spriteIndex]) + rand()%128;
        sp->position.y = ((rand() % 100));
        sp->v = RandRange(2,20);
        sp->angle = 0;
        sp->draw = rand() % 2;
        sp->initialized = true;
    }
}
float Transition_EaseOutQuad(float timer) {
    return 1 - (1 - timer) * (1 - timer);
}

float Transition_EaseInOutQuad(float timer)
{
    float percent = 1.0f;
    if (timer >= 1.0f)
    {
    }
    else if (timer < 0.5f)
    {
         percent= ((2 * timer * timer));
    }
    else if (timer >= 0.5f)
    {
         percent =  ((1 - pow(-2*timer+2,2)/2.0f));
    }
    return percent;
}
void SetLoadscreen(char* path, char* mask, float transitionInTime, float moveTime, float pauseTime, float textInTime, float textHoldTime, char* header, char* description)
{
    if (loadScreen.header)
        free(loadScreen.header);
    if (loadScreen.description)
        free(loadScreen.description);

    memset(&loadScreen,0,sizeof(LoadScreen));
    loadScreen.spriteIndex = LoadSprite(path,false);
    if (mask)
        loadScreen.maskSpriteIndex = LoadSprite(mask,false);
    loadScreen.state = LOADSCREEN_TRANSITON_IN;

    loadScreen.transitionTimers[LOADSCREEN_TRANSITON_IN] = transitionInTime;
    loadScreen.transitionTimers[LOADSCREEN_MOVING] = moveTime;
    loadScreen.transitionTimers[LOADSCREEN_PAUSE] = pauseTime;
    loadScreen.transitionTimers[LOADSCREEN_TEXT_TRANSITION_IN] = textInTime;
    loadScreen.transitionTimers[LOADSCREEN_END] = 1;

    loadScreen.y = -GetHeightSprite(&sprites[loadScreen.spriteIndex]) + _SCREEN_SIZE;
    loadScreen.transition = Transition_EaseInOutQuad;
    loadScreen.textBoxTransition = Transition_EaseOutQuad;
    if (header)
    {
        loadScreen.header = calloc(strlen(header)+1,sizeof(char));
        strcpy(loadScreen.header,header);
    }
    if (description)
    {
        loadScreen.description = calloc(strlen(description)+1,sizeof(char));
        strcpy(loadScreen.description,description);
    }
}

void DrawLoadscreen()
{
    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);
    DrawLoadScreenSprites(ORDER_BEFORESPRITE);

    DrawSprite(&sprites[loadScreen.maskSpriteIndex],loadScreen.x,loadScreen.y,0.5f,0.5f,0,BG,false,false,false);
    DrawSprite(&sprites[loadScreen.spriteIndex],loadScreen.x,loadScreen.y,0.5f,0.5f,0,FRIENDLY,false,false,false);

    if (loadScreen.state == LOADSCREEN_TRANSITON_IN)
    {
        unsigned char r,g,b;
        al_unmap_rgb(BG,&r,&g,&b);
        ALLEGRO_COLOR c = al_premul_rgba(r,g,b,255- 255*loadScreen.timer/loadScreen.transitionTimers[loadScreen.state]);//al_map_rgba_f(BG.r/255.0f,BG.g/255.0f,BG.b/255.0f,1-loadScreen.timer/loadScreen.transitionTimers[loadScreen.state]);
        al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,c);
    }
    DrawLoadScreenSprites(ORDER_AFTERSPRITE);
    if (loadScreen.state >= LOADSCREEN_TEXT_TRANSITION_IN)
    {
        int x = loadScreen.textBoxX-LOADSCREEN_TEXTBOXW/2.0f;
        int y = loadScreen.textBoxY-LOADSCREEN_TEXTBOXH/2.0f;
        al_draw_filled_rectangle(x+2,y+2,x+LOADSCREEN_TEXTBOXW-4,y+LOADSCREEN_TEXTBOXH-4,BG);
        al_draw_rectangle(x+2,y+2,x+LOADSCREEN_TEXTBOXW-4,y+LOADSCREEN_TEXTBOXH-4,FRIENDLY,1);

        al_draw_text(ui.boldFont,FRIENDLY,x+LOADSCREEN_TEXTBOXW/2.0f,y+LOADSCREEN_TEXTBOXH/4.0f,ALLEGRO_ALIGN_CENTRE,loadScreen.header);
        al_draw_text(ui.font,FRIENDLY,x+LOADSCREEN_TEXTBOXW/2.0f,y+LOADSCREEN_TEXTBOXH/2.0f,ALLEGRO_ALIGN_CENTRE,loadScreen.description);
    
    }

}

void FinishLoadScreen()
{
    if (gameState == GAMESTATE_LOAD_SCREEN)
        SetGameStateToInMenu();
    if (gameState == GAMESTATE_LOAD_ENCOUNTER)
        SetGameStateToInGame();
}

void UpdateLoadscreenSprites(float dt)
{
    for (int i = 0; i < MAX_LOADSCREEN_SPRITES; i++)
    {
        LoadScreenSprite* sp = &loadScreen.sprites[i];
        if (!sp->initialized)
            continue;

        float vX =  cos(sp->angle) * sp->v * dt;
        float vY =  sinf(sp->angle) * sp->v * dt;
        sp->position.x += vX;
        sp->position.y += vY;

        if (sp->position.x  > _SCREEN_SIZE)
            sp->position.x = -GetWidthSprite(&sprites[sp->spriteIndex]);     
        if (sp->position.x  + GetWidthSprite(&sprites[sp->spriteIndex]) < 0)
            sp->position.x = _SCREEN_SIZE;        
   
    }
}
void DrawLoadScreenSprites(LoadScreenDrawOrder order)
{
    
    for (int i = 0; i < MAX_LOADSCREEN_SPRITES; i++)
    {
        LoadScreenSprite* sp = &loadScreen.sprites[i];
        
        if (sp->draw == order)
        {
            DrawSprite(&sprites[sp->spriteIndex],sp->position.x+loadScreen.x,sp->position.y+loadScreen.y,0,0,0,SHIELD,false,false,false);
        }
    }
}

void UpdateLoadscreen(float dt, ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState)
{

    UpdateLoadscreenSprites(dt);
    loadScreen.timer += dt;

    if (loadScreen.timer >= loadScreen.transitionTimers[loadScreen.state])
    {
        
        loadScreen.state++;
        loadScreen.timer = 0;

        if (loadScreen.state >= LOADSCREEN_END)
        {
            loadScreen.state = LOADSCREEN_END;
        }
    }
    if (loadScreen.transition && loadScreen.state == LOADSCREEN_MOVING)
    {
        float timer = loadScreen.timer/(float)loadScreen.transitionTimers[loadScreen.state];

        float percent = loadScreen.transition(timer);
        float h = GetHeightSprite(&sprites[loadScreen.spriteIndex]);
        loadScreen.y =  (-percent * (_SCREEN_SIZE - GetHeightSprite(&sprites[loadScreen.spriteIndex]))) - (h-_SCREEN_SIZE);
    }
    if (loadScreen.textBoxTransition && loadScreen.state == LOADSCREEN_TEXT_TRANSITION_IN)
    {
        float timer = loadScreen.timer/(float)loadScreen.transitionTimers[loadScreen.state];
        float percent = loadScreen.textBoxTransition(timer);
        
        loadScreen.textBoxY = (percent * ((_SCREEN_SIZE/2)+ LOADSCREEN_TEXTBOXH)) - LOADSCREEN_TEXTBOXH;
        loadScreen.textBoxX = _SCREEN_SIZE/2.0f ;
    }
    if (loadScreen.state >= LOADSCREEN_END)
    {
        if (mouseState->mouse.buttons & 1 || mouseState->mouse.buttons & 2)
        {
            FinishLoadScreen();
        }
    }
    if (gameState == GAMESTATE_LOAD_SCREEN)
    {
        for (int i = 0; i < 8; i++)
        {

        }
    }
    
    if (currEncounterRunning)
    {
        //TODO: functionality for spawning LoadScreenSprites
        lua_rawgeti(luaState, LUA_REGISTRYINDEX, currEncounterRunning->luafunc_updateloadscreen);
        lua_pushnumber(luaState,dt);
        lua_pcall(luaState,1,0,0);
    }

}