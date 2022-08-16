#include "minimap.h"
#include "colors.h"
#include "video.h"
#include "sprite.h"
#include "map.h"
#include "ui.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"


void DrawMinimap()
{
    Sprite* s = &sprites[currMap->spriteIndex];
    int w = GetWidthSprite(s);
    int h = GetHeightSprite(s);

    int screenSize = _SCREEN_SIZE - (_SCREEN_SIZE-UI_START_Y);
    float scale = w > h ? screenSize / (float)w :screenSize / (float)h; 
    float x = (_SCREEN_SIZE - (w*scale))/2;
    float y = (screenSize - (h*scale))/2;
    al_draw_filled_rectangle(x,y,x+w*scale,y+h*scale,BG);
    al_draw_tinted_scaled_bitmap(s->sprite,FRIENDLY,0,0,w,h,x,y,w*scale,h*scale,0);
    al_draw_rectangle(x,y,x+w*scale,y+h*scale,FRIENDLY,1);
}
