#include "loadscreen.h"
#include "sprite.h"
#include "stdio.h"
#include "math.h"
#include "colors.h"
#include "video.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro.h"
#include "ui.h"
void InitLoadscreen(char* initialPath)
{
    SetLoadscreen(initialPath,1,3.5,0,2,3);
}
float Transition_EaseOutCirc(float timer) {
    return sqrt(1 - pow(timer - 1, 2));
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
void SetLoadscreen(char* path, float transitionInTime, float moveTime, float pauseTime, float textInTime, float textHoldTime)
{
    memset(&loadScreen,0,sizeof(LoadScreen));
    loadScreen.spriteIndex = LoadSprite(path,false);
    loadScreen.state = LOADSCREEN_TRANSITON_IN;

    loadScreen.transitionTimers[LOADSCREEN_TRANSITON_IN] = transitionInTime;
    loadScreen.transitionTimers[LOADSCREEN_MOVING] = moveTime;
    loadScreen.transitionTimers[LOADSCREEN_PAUSE] = pauseTime;
    loadScreen.transitionTimers[LOADSCREEN_TEXT_TRANSITION_IN] = textInTime;
    loadScreen.transitionTimers[LOADSCREEN_END] = 1;

    loadScreen.y = -GetHeightSprite(&sprites[loadScreen.spriteIndex]) + _SCREEN_SIZE;
    loadScreen.transition = Transition_EaseInOutQuad;
    loadScreen.textBoxTransition = Transition_EaseOutQuad;
}
void DrawLoadscreen()
{
        al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

    DrawSprite(&sprites[loadScreen.spriteIndex],loadScreen.x,loadScreen.y,0,FRIENDLY,false);

    if (loadScreen.state == LOADSCREEN_TRANSITON_IN)
    {
        unsigned char r,g,b;
        al_unmap_rgb(BG,&r,&g,&b);
        ALLEGRO_COLOR c = al_premul_rgba(r,g,b,255- 255*loadScreen.timer/loadScreen.transitionTimers[loadScreen.state]);//al_map_rgba_f(BG.r/255.0f,BG.g/255.0f,BG.b/255.0f,1-loadScreen.timer/loadScreen.transitionTimers[loadScreen.state]);
        al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,c);
    }
    if (loadScreen.state >= LOADSCREEN_TEXT_TRANSITION_IN)
    {
        int x = loadScreen.textBoxX-LOADSCREEN_TEXTBOXW/2.0f;
        int y = loadScreen.textBoxY-LOADSCREEN_TEXTBOXH/2.0f;
        al_draw_filled_rectangle(x+2,y+2,x+LOADSCREEN_TEXTBOXW-4,y+LOADSCREEN_TEXTBOXH-4,BG);
        al_draw_rectangle(x+2,y+2,x+LOADSCREEN_TEXTBOXW-4,y+LOADSCREEN_TEXTBOXH-4,FRIENDLY,1);

        al_draw_text(ui.boldFont,FRIENDLY,x+LOADSCREEN_TEXTBOXW/2.0f,y+LOADSCREEN_TEXTBOXH/4.0f,ALLEGRO_ALIGN_CENTRE,"MON IOCHDAR");
        al_draw_text(ui.font,FRIENDLY,x+LOADSCREEN_TEXTBOXW/2.0f,y+LOADSCREEN_TEXTBOXH/2.0f,ALLEGRO_ALIGN_CENTRE,"Press any key to continue.");
    
    }

}

void FinishLoadScreen()
{
    if (gameState == GAMESTATE_LOAD_SCREEN)
        SetGameStateToInMenu();
    if (gameState == GAMESTATE_LOAD_ENCOUNTER)
        SetGameStateToInGame();
}
void UpdateLoadscreen(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState)
{

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
        if (mouseState->buttons & 1 || mouseState->buttons & 2)
        {
            FinishLoadScreen();
        }
    }

}