#include <float.h>

#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"

#include "sprite.h"

#include "gameobject.h"
#include "colors.h"


int GetWidthSprite(Sprite* s)
{
    return al_get_bitmap_width(s->sprite);
}
int GetHeightSprite(Sprite* s)
{
    return al_get_bitmap_height(s->sprite);
}

//loads a sprite, and also generates an inverse sprite for selection
void GenerateInvertedSprite(Sprite* s)
{
    al_lock_bitmap(s->sprite, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
    
    int w; int h; 
    w = al_get_bitmap_width(s->sprite);
    h = al_get_bitmap_height(s->sprite);

    ALLEGRO_BITMAP* newSprite = al_create_bitmap(w,h);

    ALLEGRO_COLOR white = al_map_rgba(255,255,255,255);//FRIENDLY;
    ALLEGRO_COLOR transparent = al_map_rgba(0,0,0,0);

    ALLEGRO_BITMAP* before = al_get_target_bitmap();

    al_lock_bitmap(newSprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);
    al_set_target_bitmap(newSprite);

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        ALLEGRO_COLOR pixel = al_get_pixel(s->sprite, x, y);
        if (pixel.a)
        {
            al_put_pixel(x,y,transparent);
        }
        else
        {
            al_put_pixel(x,y,white);
            
        }

      }
    }

    //corners
    al_put_pixel(0,0,transparent);
    al_put_pixel(w-1,0,transparent);
    al_put_pixel(w,h,transparent);
    al_put_pixel(0,h,transparent);


    al_unlock_bitmap(newSprite);
    al_unlock_bitmap(s->sprite);

    al_set_target_bitmap(before);

    s->inverseSprite = newSprite;

}
void SetSpriteToWhite(ALLEGRO_BITMAP* s)
{
    if (s)
    {
        al_lock_bitmap(s,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
        int w = al_get_bitmap_width(s);
        int h = al_get_bitmap_height(s);
        
        ALLEGRO_BITMAP* before = al_get_target_bitmap();
        al_set_target_bitmap(s);

        ALLEGRO_COLOR white = al_map_rgba(255,255,255,255);
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                ALLEGRO_COLOR pixel = al_get_pixel(s,x,y);
                if (pixel.a > 0.5f && pixel.g > 0.5f && pixel.b > 0.5f && pixel.g > 0.5f)
                {
                    al_put_pixel(x,y,white);
                }
            }
        }
        al_unlock_bitmap(s);

        al_set_target_bitmap(before);
    }
}
Sprite* NewSprite(int w, int h)
{
    if (!sprites)
    {
        sprites = calloc(2,sizeof(Sprite));
        sprites[0].path = "";
        numSprites=1;
        maxSprites=2;

        //dodge a lot of crashes by setting the 0th sprite to a zeroed bitmap
        sprites[0].sprite = al_create_bitmap(0,0);
        sprites[0].inverseSprite = al_create_bitmap(0,0);

    }

    if (numSprites+1 > maxSprites)
    {
        maxSprites+=32;
        sprites = realloc(sprites,maxSprites*sizeof(Sprite));
    }
    ALLEGRO_BITMAP* b = al_create_bitmap(w,h);
    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    al_set_target_bitmap(b);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_set_target_bitmap(before);



    Sprite* s = &sprites[numSprites];
    s->sprite = b;
    numSprites++;
    return s;
}
unsigned int LoadSprite(const char* path, bool needsInverted)
{
    if (!sprites)
    {
        sprites = calloc(2,sizeof(Sprite));
        sprites[0].path = "";
        numSprites=1;
        maxSprites=2;

            //dodge a lot of crashes by setting the 0th sprite to a zeroed bitmap
    sprites[0].sprite = al_create_bitmap(0,0);
    sprites[0].inverseSprite = al_create_bitmap(0,0);

    }
    for (int i = 0; i < numSprites; i++)
    {
        if (sprites)
        {
            if (strcmp(sprites[i].path,path) == 0)
            {
                return i;
            }
        }
    }

    ALLEGRO_BITMAP* sprite = al_load_bitmap(path);

    if (numSprites+1 > maxSprites)
    {
        maxSprites+=32;
        sprites = realloc(sprites,maxSprites*sizeof(Sprite));
    }

    if (sprite)
    {
        SetSpriteToWhite(sprite);
        Sprite loadedSprite;
        loadedSprite.sprite = sprite;
        loadedSprite.path = calloc(strlen(path)+1,sizeof(char));
        strcpy(loadedSprite.path,path);
        sprites[numSprites] = loadedSprite;
        numSprites++;
        if (needsInverted)
            GenerateInvertedSprite(&sprites[numSprites-1]);
        else
            sprites[numSprites-1].inverseSprite = 0x0;
        return numSprites-1;

    }

    return 0;
}
void DrawSprite(Sprite* sprite, int x, int y, float originX, float originY, float angle, ALLEGRO_COLOR tint, bool invert)
{
    if (sprite->sprite && !invert)
    {
        int w = al_get_bitmap_width(sprite->sprite);
        int h = al_get_bitmap_height(sprite->sprite);

        //al_draw_tinted_scaled_bitmap(sprite->sprite,tint,0,0,w,h,x,y,w,h,0);
        float cx = w * (originX);
        float cy = h * (originY);

        float oX = x+ w * originX;
        float oY = y+  h * originY;


        al_draw_tinted_scaled_rotated_bitmap(sprite->sprite,tint,cx,cy,oX,oY,1,1,angle,0);

    }
    if (sprite->inverseSprite && invert)
    {
        int w = al_get_bitmap_width(sprite->inverseSprite);
        int h = al_get_bitmap_height(sprite->inverseSprite);


        float cx = w * (originX);
        float cy = h * (originY);

        float oX = x+ w * originX;
        float oY = y+  h * originY;

        //al_draw_tinted_scaled_bitmap(sprite->inverseSprite,tint,0,0,w,h,x,y,w,h,0);
        al_draw_tinted_scaled_rotated_bitmap(sprite->inverseSprite,tint,cx,cy,oX,oY,1,1,angle,0);

    }
}
void DrawSpriteRegion(Sprite* sprite, float sx, float sy, float sw, float sh, float dx, float dy, ALLEGRO_COLOR tint, bool invert)
{
    if (sprite->sprite && !invert)
        al_draw_tinted_bitmap_region(sprite->sprite,tint,sx,sy,sw,sh,dx,dy,0);
    if (sprite->inverseSprite && invert)
        al_draw_tinted_bitmap_region(sprite->inverseSprite,tint,sx,sy,sw,sh,dx,dy,0);
    
}
void InitSprites()
{
    numSprites=1;
    maxSprites = 0;
}