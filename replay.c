#include "replay.h"
#include "gameobject.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "colors.h"
//FORMAT:
//uint32_t number of bytes in the current frame
//RLE encoded array of bytes representing the screen state
//represented as the Color indices defined in color.h Color enum

void InitReplay()
{
    replay.data = NULL;
    replay.numFrames = 0;
    replay.position = 0;
}
void NewReplay()
{
    if (!replay.data)
        replay.data = malloc(REPLAY_PREALLOC*sizeof(char));
    replay.numFrames = 0;
    replay.position = 0;

}
void LoadFrame(ALLEGRO_BITMAP* screen)
{
    int x=0; int y=0;
    al_lock_bitmap(screen,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);
    for (int i = 0; i < frameTestSize; i += (sizeof(uint16_t)+sizeof(char)))
    {
        uint16_t size; 
        memcpy(&size,&frameTest[i],sizeof(uint16_t));
        char c = frameTest[i+sizeof(uint16_t)];

        while (size > 0)
        {
            al_draw_pixel(x,y,GetColor(c,0));
            x++;
            size--;
            if (x >= _SCREEN_SIZE)
            {
                y++;
                x = 0;
            }
        }

    }
   al_unlock_bitmap(screen);
   printf("%i\n",frameTestSize/1024);

}
void SaveFrame(ALLEGRO_BITMAP* screen)
{
    if (!frameTest)
    {
        frameTest = malloc(_SCREEN_SIZE * _SCREEN_SIZE*(sizeof(char)+sizeof(uint16_t)));
    }
    int toGetTo = _SCREEN_SIZE * _SCREEN_SIZE;
    int count = 0;
    ALLEGRO_LOCKED_REGION* region = al_lock_bitmap(screen,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    ALLEGRO_COLOR* pixels = region->data;
    int x=0; int y=0;
    printf("color start");

    int RLEPosition = 0;
    while (count < toGetTo)
    {
        //can be made faster by reading directly?
        ALLEGRO_COLOR pixel = al_get_pixel(screen,x,y);
        // al_unmap_rgb(pixels[count],&r,&g,&b);
        char pix = ALColorToCol(pixel);

        //count++;
        x++;
        if (x >= _SCREEN_SIZE)
        {
            x = 0; 
            y++;
        }
        int x2 = x; int y2 = y;
        for (int i = count; i < toGetTo; i++)
        {
            if (x2 >= _SCREEN_SIZE)
            {
                x2=0;
                y2++;
            }
            ALLEGRO_COLOR pixel2 = al_get_pixel(screen,x2,y2);
            if (pix != ALColorToCol(pixel2))
            {
                uint16_t amt = i-count+1;
                count += amt;
                x=x2;
                y=y2;   
                memcpy(&frameTest[RLEPosition],&amt,sizeof(uint16_t));
                RLEPosition += sizeof(uint16_t);
                memcpy(&frameTest[RLEPosition],&pix,sizeof(char));
                RLEPosition+= sizeof(char);
                break;
            }
            x2++;

        }


    }
    frameTestSize = RLEPosition;

   al_unlock_bitmap(screen);
   int newSize;

    //RLECompressArray(colors,toGetTo,frameTest,&newSize);
    replay.numFrames++;
}