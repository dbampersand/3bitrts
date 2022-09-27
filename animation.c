#include "animation.h"
#include "sprite.h"
#include "colors.h"

Sprite* GetAnimSprite(Animation* a)
{
    return &sprites[a->spriteIndex_Animation];
}
void ProcessAnimations(Animation* a, float dt)
{
    a->timer += dt;
    if (a->hasLooped)
        a->hasLooped = false;
    if (a->timer > a->cooldown)
    {
        a->currentIndice++;
        a->timer = 0;
        if (a->currentIndice >= a->startIndice + a->numFrames)
        {   
            a->currentIndice = a->startIndice;
            a->hasLooped = true;
        }
    }
}
void DrawAnimation(Animation* a, int x, int y, Color c, bool invert)
{
    ALLEGRO_COLOR col = GetColor(c,0);

    int numIndicesX = (GetWidthSprite(GetAnimSprite(a)) / a->frameW);
    int numIndicesY = (GetHeightSprite(GetAnimSprite(a)) / a->frameH);

    int indiceX = a->currentIndice % numIndicesX;
    int indiceY = a->currentIndice / numIndicesX;

    int sx = indiceX * a->frameW;
    int sy = indiceY * a->frameH;
    
    DrawSpriteRegion(GetAnimSprite(a),sx,sy,a->frameW,a->frameH,x,y,col,invert);
}
Animation LoadAnimation(char* path, int w, int h, float cooldown,int startIndice, int numFrames)
{
    Animation a = {0};
    a.spriteIndex_Animation = LoadSprite(path,false);
    a.cooldown = cooldown;
    a.frameW = w;
    a.frameH = h;
    a.numFrames = numFrames;
    a.startIndice = startIndice;
    return a;

}