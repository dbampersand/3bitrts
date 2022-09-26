#include "shop.h"
#include "animation.h"
#include "colors.h"
#include "sprite.h"
#include "helperfuncs.h"

void LoadShop()
{
    int numIdleSprites = 4;
    shop.idleSprites = calloc(numIdleSprites,sizeof(Animation));
    shop.numIdleSprites = numIdleSprites;
    

    char* path = "assets/ui/shop/shop_idle.png";
    Sprite* s = &sprites[LoadSprite(path,false)];

    int frameW = 87;
    int frameH = 95;
    int lineW = GetWidthSprite(s)/frameW;

    int currIndice = -2;
    int currAnimation = 0;

    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,1.2f,0,2);
    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,0.2f,3,7);
    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,1.5f,9,2);
    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,1.5f,11,2);


    shop.currAnimation = &shop.idleSprites[0];

    shop.spriteIndex_stall = LoadSprite("assets/ui/shop/shop_base.png",false);


}
void SwitchShopkeepAnimation(Animation* to)
{
    shop.currAnimation->hasLooped = false;

    shop.currAnimation = to;
    to->currentIndice = to->startIndice;
    to->timer = 0;

}
void UpdateShop(float dt)
{
    ProcessAnimations(shop.currAnimation,dt);

    if (shop.currAnimation->hasLooped)
    {
        float r = RandRange(0,1);
        return; 
        //Play animation other than the default idle 
        if (r > 0.6)
        {
            Animation* to = shop.currAnimation;
            while (to == shop.currAnimation)
                to = &shop.idleSprites[RandRangeI(0,shop.numIdleSprites)];
            SwitchShopkeepAnimation(to);
        }
        else
        {
            Animation* to = &shop.idleSprites[0];
            SwitchShopkeepAnimation(to);
        }
    }
}
void DrawShop(float dt)
{
    int startX = 8; int startY = 8;
    int shopkeeperOffsetX = 18;
    int shopkeeperOffsetY = 44;

    DrawSprite(&sprites[shop.spriteIndex_stall],startX,startY,0,0,0,FRIENDLY,false);
    DrawAnimation(shop.currAnimation,startX + shopkeeperOffsetX,startY + shopkeeperOffsetY,COLOR_FRIENDLY,false);
}