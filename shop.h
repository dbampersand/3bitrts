#pragma once

#include "animation.h"
#include "item.h"
#include "ui.h"
#include "point.h"
#include "allegro5/allegro.h"
#include <stdbool.h>

typedef struct Item Item;

typedef struct ShopItem
{
    Item* item;
    Point position;
    Point desiredPosition;
} ShopItem;

typedef struct Shop
{
    Animation* idleSprites;
    int numIdleSprites;

    int spriteIndex_stall;

    Animation* currAnimation;

    UIElement continueButton;

    ShopItem items[NUM_ITEM_POOLS];

    ShopItem* heldItem;
    Point heldOffset;

    float timer;
    int startX; int startY;
} Shop;


Shop shop;

void DrawShop(float dt, ALLEGRO_MOUSE_STATE mouseState);
void LoadShop();
void UpdateShop(float dt, ALLEGRO_MOUSE_STATE mouseState, ALLEGRO_MOUSE_STATE mouseStateLastFrame);
void SwitchShopkeepAnimation(Animation* to);
void RefreshShop();
