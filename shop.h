#pragma once

#include "animation.h"
#include "item.h"

typedef struct Shop
{
    Animation* idleSprites;
    int numIdleSprites;

    int spriteIndex_stall;

    Animation* currAnimation;
} Shop;

Shop shop;

void DrawShop(float dt);
void LoadShop();
void UpdateShop(float dt);
void SwitchShopkeepAnimation(Animation* to);
