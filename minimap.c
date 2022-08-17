#include "minimap.h"
#include "colors.h"
#include "video.h"
#include "sprite.h"
#include "map.h"
#include "ui.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "player.h"
#include "allegro5/allegro.h"

void DrawMinimap(ALLEGRO_MOUSE_STATE mouseState)
{
    Sprite* s = &sprites[currMap->spriteIndex];
    int w = GetWidthSprite(s);
    int h = GetHeightSprite(s);

    int screenSize = _SCREEN_SIZE - (_SCREEN_SIZE-UI_START_Y);
    float scale = w > h ? screenSize / (float)w :screenSize / (float)h; 
    float x = (_SCREEN_SIZE - (w*scale))/2;
    float y = (screenSize - (h*scale))/2;
    al_draw_filled_rectangle(x,y,x+w*scale,y+h*scale,BG);
    al_draw_tinted_scaled_bitmap(s->sprite,GROUND,0,0,w,h,x,y,w*scale,h*scale,0);
    al_draw_rectangle(x,y,x+w*scale,y+h*scale,FRIENDLY,1);

    float xScreen = players[0].cameraPos.x;
    float yScreen = players[0].cameraPos.y;
    float pX = x +  xScreen * scale;
    float pY = y + yScreen * scale;


    al_draw_rectangle(pX,pY,pX+_SCREEN_SIZE*scale,pY+_SCREEN_SIZE*scale,FRIENDLY,1);

    Rect r = (Rect){x,y,w,h};
    if (mouseState.buttons & 1)
    {
        ToScreenSpaceI(&mouseState.x,&mouseState.y);
        if (PointInRect(mouseState.x,mouseState.y,r))
        {

            float dx = (mouseState.x - x) / scale;
            float dy = (mouseState.y - y) / scale;

            FocusCameraOnPos(dx, dy);
        }
    }
}
