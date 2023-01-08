#include <float.h>
#include <stdio.h>
#include <math.h>

#include "allegro5/allegro_primitives.h"

#include "video.h" 
#include "pointspace.h"

#include "colors.h"
#include "helperfuncs.h"
#include "point.h"
#include "sprite.h"
#include "ui.h"
#include "gameobject.h"
#include "map.h"
#include "helperfuncs.h"
#include "player.h"

#include "allegro5/allegro_font.h"

Point DEBUG_P1 = {0};
Point DEBUG_P2 = {0};
Point DEBUG_P3 = {0};
Point DEBUG_P4 = {0};
float DEBUG_EXTRA = 0;
int _TARGET_FPS = 60;

ALLEGRO_BITMAP* backbuffer = NULL;
ALLEGRO_BITMAP* SCREEN = NULL; 
ALLEGRO_BITMAP* background_screen = NULL;
ALLEGRO_DISPLAY* display = NULL;

unsigned long long _FRAMES = 1;

 TextDisplay textDisplays[NUM_TEXT_DISPLAYS] = {0};
 int numStringsToDraw = 0;

SpriteDecoration spriteDecorations[MAX_SPRITE_DECORATIONS];
SpriteDecoration* freeSpriteDecorations[MAX_SPRITE_DECORATIONS];

int* cloudSprites;
int numCloudSprites;
int numFreeDecorations;

int* dirtSprites;
int numDirtSprites;



void InitCloudSprites()
{
    const char* cloudSprPaths[] = 
    {
        "assets/decor/clouds/cloud_01.png",
        "assets/decor/clouds/cloud_02.png",
        "assets/decor/clouds/cloud_03.png",
        "assets/decor/clouds/cloud_04.png",
        "assets/decor/clouds/cloud_05.png",

    };
    numCloudSprites = sizeof(cloudSprPaths) / sizeof(cloudSprPaths[0]);
    cloudSprites = calloc(numCloudSprites,sizeof(cloudSprites[0]));
    for (int i = 0; i < numCloudSprites; i++)
    {
        cloudSprites[i] = LoadSprite(cloudSprPaths[i],false);
    }
}
void InitDirtSprites()
{
    const char* dirtSprPaths[] = 
    {
        "assets/decor/dirt/dirt_01.png",
        "assets/decor/dirt/dirt_02.png",
        "assets/decor/dirt/dirt_03.png",

    };
    numDirtSprites = sizeof(dirtSprPaths) / sizeof(dirtSprPaths[0]);
    dirtSprites = calloc(numDirtSprites,sizeof(dirtSprites[0]));
    for (int i = 0; i < numDirtSprites; i++)
    {
        dirtSprites[i] = LoadSprite(dirtSprPaths[i],false);
    }

}
void InitSpriteDecorations()
{
    InitCloudSprites();
    InitDirtSprites();
    for (int i = 0; i < MAX_SPRITE_DECORATIONS; i++)
    {
        freeSpriteDecorations[i] = &spriteDecorations[i];
    }
    numFreeDecorations = MAX_SPRITE_DECORATIONS;
    
}
void InitVideo()
{
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);

    _RENDERSIZE = _MIN(monitor.x2,monitor.y2)/_SCREEN_SIZE - 1;
    background_screen = al_create_bitmap(monitor.x2,monitor.y2);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(monitor.x2,monitor.y2);

    InitSpriteDecorations();


    int resX = _SCREEN_SIZE*_RENDERSIZE;
    int resY = _SCREEN_SIZE*_RENDERSIZE;

    SCREEN = al_create_bitmap(_SCREEN_SIZE,_SCREEN_SIZE);
    backbuffer = al_get_backbuffer(display);
    _FRAMES = 0;
    
    for (int i = 0; i < NUM_TEXT_DISPLAYS; i++)
    {
        memset(textDisplays[i].str,0,TEXT_DISPLAY_MAX_SIZE*sizeof(char));
    }
    numStringsToDraw = 0;



    for (int a = -360; a < 360; a++)
    {
        float rad = DegToRad(a);
        __sinTable[a+360] = sin(rad);
        __cosTable[a+360] = cos(rad);
    }
    sinTable = &__sinTable[360];
    cosTable = &__cosTable[360];



}
void DrawRoundedRect(Rect r, ALLEGRO_COLOR color, bool filled)
{
    int x = r.x; int y = r.y; int w = r.w; int h = r.h;

    if (filled)
        al_draw_filled_rectangle(x,y,x+w-1,y+h+1,color);
    //horizontals
    al_draw_line(x, y, x+w-1, y, color,1);
    al_draw_line(x, y+h, x+w-1, y+h, color,1);

    //verticals
    al_draw_line(x, y+1, x, y+h, color,1);
    al_draw_line(x + w, y+1, x+w, y+h, color,1);
}
void DrawFilledRect_Dithered(Rect r, ALLEGRO_COLOR color)
{
    al_lock_bitmap_region(al_get_target_bitmap(),r.x,r.y,r.w,r.h,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    int dither = 2;
    for (int x = r.x; x < r.x+r.w; x++)
    {
        for (int y = r.y; y < r.y+r.h; y++)
        {
            if (x%dither==0 && y%dither == 0)
            {
                al_put_pixel(x,y,color);
            }
        }
    }
    al_unlock_bitmap(al_get_target_bitmap());


}
void DrawOutlinedRect_Dithered(Rect r, ALLEGRO_COLOR color)
{
    r.x--;

    al_lock_bitmap_region(al_get_target_bitmap(),r.x,r.y,r.w+1,r.h+1,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    r.x = (int)r.x;
    r.y = (int)r.y;    
    r.w = (int)r.w;    
    r.h = (int)r.h;    


    //if ((int)r.x % 2 == 0)
      //  r.x--;
       

    for (int y = r.y; y < r.y+r.h; y++)
    {
        if (y%2==0)
        {
            al_put_pixel(r.x,y,color);
            al_put_pixel(r.x+r.w,y,color);
        }

    }
    for (int x = r.x; x < r.x+r.w; x++)
    {
        if (x%2==1)
        {
            al_put_pixel(x,r.y,color);
            al_put_pixel(x,r.y+r.h,color);
        }

    }

    //Draw corner details
    al_put_pixel(r.x,r.y,color);
    al_put_pixel(r.x+1,r.y,color);
    al_put_pixel(r.x,r.y+1,color);

    al_put_pixel(r.x+r.w,r.y,color);
    al_put_pixel(r.x+r.w-1,r.y,color);
    al_put_pixel(r.x+r.w,r.y+1,color);

    al_put_pixel(r.x,r.y+r.h,color);
    al_put_pixel(r.x,r.y+r.h-1,color);
    al_put_pixel(r.x+1,r.y+r.h,color);

    al_put_pixel(r.x+r.w,r.y+r.h,color);
    al_put_pixel(r.x+r.w-1,r.y+r.h,color);
    al_put_pixel(r.x+r.w,r.y+r.h-1,color);
    al_unlock_bitmap(al_get_target_bitmap());

}
int GetBarycentricCoord(int x0, int y0, int x1, int y1, int pX, int pY)
{
    return (y0 - y1) * pX + (x1 - x0) * pY + x0 * y1 - x1 * y0;
}
void SwapPoint(int* x0, int* y0, int* x1, int* y1)
{
    int temp = *x1;
    *x1 = *x0;
    *x0 = temp;
    temp = *y1;
    *y1 = *y0;
    *y0 = temp;
}

bool PointInTri(int x0, int y0, int x1, int y1, int x2, int y2, int pX, int pY)
{

    if (y0 > y1)
    {
        SwapPoint(&x0,&y0,&x1,&y1);
    }
    if (y0 > y2)
    {
        SwapPoint(&x0,&y0,&x2,&y2);
    }

    if (y1 > y2)
    {
        SwapPoint(&x1,&y1,&x2,&y2);
    }
    #ifdef DEBUG
        al_draw_line(x1,y1,x2,y2,HEAL,1);
        al_draw_line(x2,y2,x0,y0,HEAL,1);
        al_draw_line(x0,y0,x1,y1,HEAL,1);
    #endif
    float bary12 = GetBarycentricCoord(x1,y1,x2,y2,x0,y0);
    float bary20 = GetBarycentricCoord(x2,y2,x0,y0,x1,y1);
    float bary01 = GetBarycentricCoord(x0,y0,x1,y1,x2,y2);

    float alpha = GetBarycentricCoord(x1,y1,x2,y2,pX,pY) / bary12;
    float beta = GetBarycentricCoord(x2,y2,x0,y0,pX,pY) / bary20;
    float gamma = GetBarycentricCoord(x0,y0,x1,y1,pX,pY) / bary01;


    return (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1);
}

bool isClockwise(int x1, int y1, int x2, int y2)
{
    return -x1*y2 + y1*x2 > 0;
}
Rect GetTriBoundingBox(int x0, int y0, int x1, int y1, int x2, int y2)
{
    Rect r;
    r.x = _MIN(_MIN(x0,x1),x2);
    r.y = _MIN(_MIN(y0,y1),y2);

    r.w = _MAX(_MAX(x0,x1),x2) - r.x;
    r.y = _MAX(_MAX(y0,y1),y2) - r.y;
    return r;
}
bool isInsideSector(int x, int y, int cx, int cy, float startX, float startY, float endX, float endY, float length, float radius, float amtRotated) 
{
    float dX = cx - x;
    float dY = cy - y;

    double angle = (atan2(y-cy,x-cx));
    double angle2 = (atan2(startY-cy,startX-cx));
    double angle3 = (atan2(endY-cy,endX-cx));

    angle = Normalise(angle, 0, 2*M_PI);
    angle2 = Normalise(angle2,   0, 2*M_PI);
    angle3 = Normalise(angle3, 0, 2*M_PI);
    double rad = DegToRad(radius);
    rad = Normalise(rad, 0, 2*M_PI);

    if (angle2 < angle3)
    {
        float temp = angle2;
        angle2 = angle3;
        angle3 = temp;
    }


    if (dX*dX+dY*dY <= length*length)
    {
        float newX = startX-cx; float newY = startY-cy;
        float newX2 = endX-cx; float newY2 = endY-cy;
        Normalize(&newX,&newY);
        Normalize(&newX2,&newY2);
        newX*=10000;
        newY*=10000;

        newX2*=10000;
        newY2*=10000;
        newX += startX;
        newY += startY;
        newX2 += endX;
        newY2 += endY;


        float midX = startX;
        float midY = startY;

        float midAngle = M_PI - ((atan2(endY-startY,endX-startX)) + DegToRad(90) - radius/2.0) + amtRotated;
        
        midAngle = Normalise(midAngle, 0, 2*M_PI);

        RotatePointF(&midX,&midY,cx,cy,(midAngle));
        float midXNrm = midX - cx;
        float midYNrm = midY - cy;
        Normalize(&midXNrm,&midYNrm);
        midXNrm*=10000;
        midYNrm*=10000;
        midX += midXNrm;
        midY += midYNrm;
        
        bool tri1 = PointInTri(cx,cy,newX,newY,midX,midY,x,y);
        bool tri2 =  PointInTri(cx,cy,newX2,newY2,midX,midY,x,y);
        #ifdef DEBUG
            al_draw_line(cx,cy,midX,midY,POISON,1);
        #endif
        if (tri1 || tri2)
        {
            return true;
        }

    }
    return false;
}
void CircleSegment(int xc, int yc, float radius, float start, float end, ALLEGRO_COLOR col, float length)
{
    al_lock_bitmap_region(al_get_target_bitmap(),xc-radius,yc-radius,radius*2,radius*2,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    
    start = Normalise(start,0, M_PI*2);
    end = Normalise(end, 0, M_PI*2);

    int sign = start < end ? 1 : -1;

    float inc = 1 /  radius;
    
    if (sign == -1)
    {
        float temp = start;
        end -= radius * M_PI/180.0f - (length*M_PI/180.0f)+1;
        float rad = (360-radius) * M_PI/180.0f;
        start -= rad + (length*M_PI/180.0f)-1;
    }

    float theta = start;

    if (theta <= end)
    {
        while (theta <= end) {
            float xn; float yn;
            xn = xc + radius * (sin(theta));
            yn = yc + radius * (cos(theta));
            al_put_pixel(xn, yn, col);
            theta += inc;
        }
    }
    else if (theta >= end)
    {
        while (theta >= end) {
            float xn; float yn;
            xn = xc + radius * (sin(theta));
            yn = yc + radius * (cos(theta));
            al_put_pixel(xn, yn, col);
            theta += inc * -1;
        }

    }
    al_unlock_bitmap(al_get_target_bitmap());

}


//TODO: this isn't an accurate function.
//right now it will return true just when the center or corners are intersected
//does this need fixed? Unlikely for a cone to be *that* small radius
//either way, there should be a more efficient way to do this
bool RectInCone(Rect r, int cx, int cy, float angle, float radius, float length)
{
    float dx = _MIN(cx-r.x,cx-r.x+r.w);
    float dy = _MIN(cy-r.y,cy-r.y+r.h);
    if (CircleRectDist(cx,cy,length,r))
    {      
        Point rc = (Point){r.x+r.w/2.0f,r.y+r.h/2.0f};
        Point tl = (Point){r.x,r.y};
        Point tr = (Point){r.x+r.w,r.y};
        Point bl = (Point){r.x,r.y+r.h};
        Point br = (Point){r.x+r.h,r.y+r.h};
        
        int startX; int startY; int endX; int endY;
        GetConeVertices(cx,cy,&startX,&startY,&endX,&endY,angle,radius,length);


        if (isInsideSector(rc.x, rc.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(tl.x, tl.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(tr.x, tr.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(bl.x, bl.y,cx,cy,startX,startY,endX,endY,length,radius,angle) ||
            isInsideSector(br.x, br.y,cx,cy,startX,startY,endX,endY,length,radius,angle))
        {
            return true;
        }
    }
    return false;
}
void GetConeVertices(int cx, int cy, int* x1, int* y1, int* x2, int* y2, float angle, float radius, int length)
{
    angle -= DegToRad(45);
    

    *x1 = cx + length; 
    *y1 = cy + length;
    *x2 = cx + length; 
    *y2 = cy + length;

    radius = DegToRad(radius);//radius * M_PI/180.0f;

    RotatePoint(x1,y1,cx,cy, radius/2.0+angle);
    RotatePoint(x2,y2,cx,cy, -radius/2.0f+angle);

}
void DrawCone(int x, int y, float angle, float radius, int length, ALLEGRO_COLOR color)
{

    angle -= 45;
    int x2 = x + length; int y2 = y + length;
    int x3 = x + length; int y3 = y + length;

    radius = radius * M_PI/180.0f;
    angle = angle * M_PI/180.0f;
    float l_r = length * M_PI/180.0f;

    RotatePoint(&x2,&y2,x,y, -radius/2.0f+angle);
    RotatePoint(&x3,&y3,x,y, radius/2.0f+angle);

    al_draw_line(x,y,x2,y2,color,1);
    al_draw_line(x,y,x3,y3,color,1);

    float angle2 = atan2(y-y2,x-x2);
    float angle3 = atan2(y-y3,x-x3);
    angle2 -= (angle+(90*M_PI/180.0f)) * 2;
    angle3 -= (angle+(90*M_PI/180.0f)) * 2;

    if (angle2 > angle3)
    {
        //float temp = angle2;
        //angle2 = angle3;
        //angle3 = temp;
        //angle3 = -angle3;
    }
    //angle2 -= angle;
    //angle3 -= angle;

    MouseState m = GetMouseClamped();

    int startX = x2; int startY = y2;
    int endX = x3; int endY = y3;

    if (startY > endY)
    {
        int tempX = startX;
        int tempY = startY;

        // startX = endX;
            //startY = endY;

            //endX = tempX;
            //endY = tempY;

    }


    float distX = x - x2;
    float distY = y - y2;

    float l = sqrt(distX*distX+distY*distY);
    if (isInsideSector(m.worldX,m.worldY,x,y,x2,y2,x3,y3,l,radius,angle+DegToRad(45)))
        CircleSegment(x,y,l,angle2,angle3,color,length); 
    else
        CircleSegment(x,y,l,angle2,angle3,color,length);

}
void SetDisplaySize()
{
    //al_set_target_bitmap(backbuffer);

    //al_clear_to_color(BG);
    //al_flip_display();
    
    //al_set_target_bitmap(SCREEN);

}
void DrawBufferedStrings()
{   
    for (int i = 0; i < numStringsToDraw; i++)
    {
        char* str = textDisplays[i].str;
        al_draw_text(ui.font,FRIENDLY,textDisplays[i].x,textDisplays[i].y,textDisplays[i].align,str);
        //free(stringsToDraw[i]);
    }
    //locationsToDrawString = realloc(locationsToDrawString,1*sizeof(Point));
    //stringsToDraw = realloc(stringsToDraw,1*sizeof(Point));

    numStringsToDraw = 0;
}
void ProcessSpriteDecorations(float dt)
{
    for (int i = 0; i < MAX_SPRITE_DECORATIONS; i++)
    {
        SpriteDecoration* s = &spriteDecorations[i];
        s->position.worldX += s->velocity.x * dt;
        s->position.worldY += s->velocity.y * dt;
        s->position.screenX = ToScreenSpace_X(s->position.worldX);
        s->position.screenY = ToScreenSpace_Y(s->position.worldY);

        if (s->decorationType == SPRITE_BG_DECOR)
        {
            if (s->velocity.x > 0)
            {
                if (s->position.worldX > GetMapWidth())
                {
                    *s = AddCloud();
                }
            }
            if (s->velocity.y > 0)
            {
                if (s->position.worldY > GetMapHeight())
                {
                    *s = AddCloud();
                }
            }
            if (s->velocity.x <= 0)
            {
                if (s->position.worldX + GetWidthSprite(&sprites[s->spriteIndex]) < 0)
                {
                    *s = AddCloud();
                }
            }
            if (s->velocity.y <= 0)
            {
                if (s->position.worldY + GetHeightSprite(&sprites[s->spriteIndex]) < 0)
                {
                    *s = AddCloud();
                }
            }


        }
    }
}
                
void DrawSpriteDecorations(RenderOrder renderOrder)
{
    for (int i = 0; i < MAX_SPRITE_DECORATIONS; i++)
    {
        SpriteDecoration* s = &spriteDecorations[i];
        if (renderOrder == s->renderOrder && s->decorationType > SPRITE_NONE)
        {
            DrawSprite(&sprites[s->spriteIndex],s->position.screenX,s->position.screenY,0,0,0,GetColor(s->tint,0),false);
        }
    }
}
void ClearSpriteDecorations()
{
    memset(spriteDecorations,0,MAX_SPRITE_DECORATIONS*sizeof(SpriteDecoration));

    for (int i = 0; i < MAX_SPRITE_DECORATIONS; i++)
    {
        freeSpriteDecorations[i] = &spriteDecorations[i];
    }
    numFreeDecorations = MAX_SPRITE_DECORATIONS;
}
void AddSpriteDecoration(SpriteDecoration s)
{
    if (numFreeDecorations > 0)
    {
        if (s.decorationType > SPRITE_NONE)
        {
            *freeSpriteDecorations[numFreeDecorations-1] = s;
            numFreeDecorations--;
        }
    }
}
void CreateSpriteDecorAtPosition(int selectedSprite,int x,int y,RenderOrder renderOrder,Color col)
{
        SpriteDecoration s = {0};
        s.decorationType = SPRITE_BG_DECOR;
        s.spriteIndex = selectedSprite;
        s.renderOrder = renderOrder;
        s.tint = col;

        s.position.worldX = x;
        s.position.worldY = y;
        s.lifetime = FLT_MAX-1;

        AddSpriteDecoration(s);

}

void RandomSpriteDecorAtPosition(int numToAdd, int* array, int numArrayElements, int x, int y, RenderOrder renderOrder, Color col)
{
    for (int i = 0; i < numToAdd; i++)
    {
        SpriteDecoration s = {0};
        s.decorationType = SPRITE_BG_DECOR;
        s.spriteIndex = array[RandRangeI(0,numArrayElements)];
        int w = GetWidthSprite(&sprites[s.spriteIndex]);
        int h = GetHeightSprite(&sprites[s.spriteIndex]);
        s.renderOrder = renderOrder;
        s.tint = col;

        s.position.worldX = x;
        s.position.worldY = y;

        s.lifetime = FLT_MAX-1;


        AddSpriteDecoration(s);

    }
    
}
void RandomSpriteDecor(int numToAdd, int* array, int numArrayElements)
{
    for (int i = 0; i < numToAdd; i++)
    {
        SpriteDecoration s = {0};
        s.decorationType = SPRITE_BG_DECOR;
        s.spriteIndex = array[RandRangeI(0,numArrayElements)];
        int w = GetWidthSprite(&sprites[s.spriteIndex]);
        int h = GetHeightSprite(&sprites[s.spriteIndex]);
        s.renderOrder = BEFORE_WORLD;
        s.tint = COLOR_BG_DECOR;

        s.position.worldX = RandRange(-w,GetMapWidth()+w);
        s.position.worldY = RandRange(-h,GetMapWidth()+h);

        s.lifetime = FLT_MAX-1;

        AddSpriteDecoration(s);

    }
    
}
void AddClouds(int numClouds)
{
    for (int i = 0; i < numClouds; i++)
    {
        if (numFreeDecorations > 0)
        {
            SpriteDecoration s = AddCloud();
            AddSpriteDecoration(s);
        }
    }
}
SpriteDecoration AddCloud()
{
    SpriteDecoration s = {0};
    s.decorationType = SPRITE_BG_DECOR;
    s.spriteIndex = cloudSprites[RandRangeI(0,numCloudSprites)];
    int w = GetWidthSprite(&sprites[s.spriteIndex]);
    int h = GetHeightSprite(&sprites[s.spriteIndex]);


    int randSpawn = RandRange(0,4);
    //left
    if (randSpawn == 0)
    {
        s.position.worldX = RandRange(-w*2,-w);
        s.position.worldY = RandRange(-h*2,GetMapHeight()+h);

        s.velocity.x = RandRange(1,CLOUD_SPEED);
        s.velocity.y = RandRange(-CLOUD_SPEED,CLOUD_SPEED);

    }
    //top
    if (randSpawn == 1)
    {
        s.position.worldX = RandRange(-w*2,GetMapWidth()+w*2);
        s.position.worldY = RandRange(-h*2,-h);

        s.velocity.x = RandRange(-CLOUD_SPEED,CLOUD_SPEED);
        s.velocity.y = RandRange(1,CLOUD_SPEED);

    }
    //right
    if (randSpawn == 2)
    {
        s.position.worldX = RandRange(GetMapWidth()+w,GetMapWidth()+w*2);
        s.position.worldY = RandRange(-h*2,GetMapHeight()+h);

        s.velocity.x = RandRange(-CLOUD_SPEED,-1);
        s.velocity.y = RandRange(-CLOUD_SPEED,CLOUD_SPEED);
    }
    //bottom
    if (randSpawn == 3)
    {
        s.position.worldX = RandRange(-w*2,GetMapWidth()+w*2);
        s.position.worldY = RandRange(GetMapHeight()+h,GetMapHeight()+h*2);

        s.velocity.x = RandRange(-CLOUD_SPEED,CLOUD_SPEED);
        s.velocity.y = RandRange(-CLOUD_SPEED,-1);

    }


    s.renderOrder = AFTER_GAMEOBJECTS;
    s.tint = COLOR_BG_DECOR;
    s.lifetime = FLT_MAX-1;

    return s;
}
float easeOutQuint(float x) {
    if (x > 1) return 1;
    return 1 - pow(1 - x, 5);
}
float easeInCirc(float x) {
    return 1 - sqrt(1 - pow(x, 2));
}
float easeOutCirc(float x){
    return sqrt(1 - pow(x - 1, 2));
}
float easeInOutQuint(float x)
{
    if (x < 0.5f)
    {
        return 16 * x * x * x * x * x;
    }
    return 1 - pow(-2 * x + 2, 5) / 2.0f;
}
float easeOutExpo(float x)
{
    if (x >= 1)
        return 1;
    else return (1 - pow(2, -10 * x));
}
void UpdateScreenPositions_Point(PointSpace* p)
{
    p->screenX = ToScreenSpace_X(p->worldX);
    p->screenY = ToScreenSpace_Y(p->worldY);
}

