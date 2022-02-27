#include "video.h"
#include <allegro5/allegro_primitives.h>
void DrawRoundedRect(Rect r, ALLEGRO_COLOR color)
{
    //horizontals
    al_draw_line(r.x,r.y,r.x+r.w-1,r.y,color,1);
    al_draw_line(r.x,r.y+r.h,r.x+r.w-1,r.y+r.h,color,1);

    //verticals
    al_draw_line(r.x,r.y+1,r.x,r.y+r.h,color,1);
    al_draw_line(r.x+r.h,r.y+1,r.x+r.h,r.y+r.h,color,1);



}
