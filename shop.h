#pragma once

#include "animation.h"
#include "item.h"
#include "ui.h"

#include "allegro5/allegro.h"

typedef struct Shop
{
    Animation* idleSprites;
    int numIdleSprites;

    int spriteIndex_stall;

    Animation* currAnimation;

    UIElement continueButton;
} Shop;

Shop shop;

void DrawShop(float dt, ALLEGRO_MOUSE_STATE mouseState);
void LoadShop();
void UpdateShop(float dt, ALLEGRO_MOUSE_STATE mouseState, ALLEGRO_MOUSE_STATE mouseStateLastFrame);
void SwitchShopkeepAnimation(Animation* to);
