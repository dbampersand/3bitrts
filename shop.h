#pragma once

#include "animation.h"
#include "item.h"
#include "ui.h"
#include "point.h"
#include "allegro5/allegro.h"
#include <stdbool.h>

typedef struct Item Item;

#define _SHOP_ITEM_RADIUS 12

typedef enum SHOP_STATE
{
    SHOP_STATE_REROLLING,
    SHOP_STATE_NORMAL
} SHOP_STATE;

typedef struct ShopItem
{
    Item* item;
    Point position;
    Point desiredPosition;

    float timer;
    float radiusPercent;

    bool enabled;
} ShopItem;

typedef struct Shop
{
    SHOP_STATE shopState;
    float rerollAnimationTimer;

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

    Item* removeClickedItem;

    float scrollBarPos;
    bool scrollbarClicked;

} Shop;


extern Shop shop;
extern int rerollCost;
extern int randomItemCost;
void DrawShop(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void LoadShop();
void UpdateShop(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void SwitchShopkeepAnimation(Animation* to);
void RefreshShop();
Rect GetRerollRect();
Rect GetRandomItemButton();
