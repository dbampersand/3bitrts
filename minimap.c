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
#include "gameobject.h"

void DrawMinimap(MouseState mouseState)
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

    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];
        if (IsActive(g))
        {
            float gX = x + (g->position.worldX * scale);
            float gY = y + (g->position.worldY * scale);
            float gW = GetWidth(g) * scale;
            float gH = GetHeight(g) * scale;
            gW = gW < 1 ? 1 : gW;
            gH = gH < 1 ? 1 : gH; 


            ALLEGRO_COLOR c = GetColor(COLOR_DEFAULT,GetPlayerOwnedBy(g));
            
            al_draw_rectangle(gX,gY, gX+gW, gY+gH,c,1);

        }

    }

    float xScreen = players[0].cameraPos.x;
    float yScreen = players[0].cameraPos.y;
    float pX = x +  xScreen * scale;
    float pY = y + yScreen * scale;


    al_draw_rectangle(pX,pY,pX+_SCREEN_SIZE*scale,pY+_SCREEN_SIZE*scale,FRIENDLY,1);

    Rect r = (Rect){x,y,w,h};
    if (mouseState.mouse.buttons & 1)
    {
        //ToScreenSpaceI(&mouseState.x,&mouseState.y);
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {

            float dx = (mouseState.screenX - x) / scale;
            float dy = (mouseState.screenY - y) / scale;

            FocusCameraOnPos(dx, dy);
        }
    }
}
