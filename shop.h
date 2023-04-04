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

    Animation interestTransitionAnimation;
    Animation interestHeldAnimation;


    int spriteIndex_stall;

    Animation* currAnimation;

    UIElement continueButton;

    ShopItem items[NUM_ITEM_POOLS];

    ShopItem* heldItem;
    Point heldOffset;

    float timer;
    int startX; int startY;
} Shop;


extern Shop shop;
extern int rerollCost;

void DrawShop(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void LoadShop();
void UpdateShop(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void SwitchShopkeepAnimation(Animation* to);
void RefreshShop();
