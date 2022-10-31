#include "shop.h"
#include "animation.h"
#include "colors.h"
#include "sprite.h"
#include "helperfuncs.h"
#include "player.h"
#include "video.h"
#include "allegro5/allegro_primitives.h"
#include <math.h>
#include "gameobject.h"
#include "allegro5/allegro_ttf.h"
#include "ui.h"
#include "encounter.h"

Shop shop = {0};

void LoadShop()
{
    int numIdleSprites = 4;
    shop.idleSprites = calloc(numIdleSprites,sizeof(Animation));
    shop.numIdleSprites = numIdleSprites;
    

    char* path = "assets/ui/shop/shop.png";
    Sprite* s = &sprites[LoadSprite(path,false)];

    int frameW = 87;
    int frameH = 101;
    int lineW = GetWidthSprite(s)/frameW;

    int currIndice = -2;
    int currAnimation = 0;

    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,0.9f,0,2);
    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,0.2f,3,7);
    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,1.1f,9,2);
    shop.idleSprites[currAnimation++] = LoadAnimation(path,frameW,frameH,1.1f,11,2);

    shop.interestTransitionAnimation = LoadAnimation(path,frameW,frameH,0.75f,13,3);
    shop.interestHeldAnimation = LoadAnimation(path,frameW,frameH,1.1f,15,2);


    shop.currAnimation = &shop.idleSprites[0];

    shop.spriteIndex_stall = LoadSprite("assets/ui/shop/shop_base.png",false);

    InitButton(&shop.continueButton,"Continue","Continue",170,234,81,15,0);

    shop.startX = 8;
    shop.startY = 8;

}
void SwitchShopkeepAnimation(Animation* to)
{
    shop.currAnimation->hasLooped = false;

    shop.currAnimation = to;
    to->currentIndice = to->startIndice;
    to->timer = 0;

}
void RefreshShop()
{
    Item* i1 = GetRandomItem(LEVEL_POOR);
    Item* i2 = GetRandomItem(LEVEL_MID);
    Item* i3 = GetRandomItem(LEVEL_HIGH);

    shop.items[0].item = i1;
    shop.items[1].item = i2;
    shop.items[2].item = i3;

    shop.items[0].position.x = 70;
    shop.items[0].position.y = 41;   
    shop.items[0].desiredPosition = shop.items[0].position;


    shop.items[1].position.x = 115;
    shop.items[1].position.y = 41;    
    shop.items[1].desiredPosition = shop.items[1].position;


    shop.items[2].position.x = 92;
    shop.items[2].position.y = 86;   
    shop.items[2].desiredPosition = shop.items[2].position;

}
void UpdateShop(float dt, MouseState mouseState, MouseState mouseStateLastFrame)
{
    ProcessAnimations(shop.currAnimation,dt);

    if (shop.currAnimation->hasLooped)
    {
        float rad = 12;
        

        if (!shop.heldItem)
        {
            float r = RandRange(0,1);
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
        else
        {
            if (shop.currAnimation == &shop.interestTransitionAnimation || shop.currAnimation == &shop.interestHeldAnimation)
            {
                SwitchShopkeepAnimation(&shop.interestHeldAnimation);
            }
            else
            {
                SwitchShopkeepAnimation(&shop.interestTransitionAnimation);
            }
            
        }
    }
    UpdateButton(shop.continueButton.x,shop.continueButton.y,&shop.continueButton,mouseState,mouseStateLastFrame);
    if (GetButtonIsClicked(&shop.continueButton))
    {
        SetGameStateToChangingMap();
    }

//    ToScreenSpaceI(&mouseState.x,&mouseState.y);





    if (shop.heldItem)
    {
        shop.heldItem->position.x = mouseState.screenX - shop.heldOffset.x;
        shop.heldItem->position.y = mouseState.screenY - shop.heldOffset.y;
    }
    shop.timer += dt;
    

    for (int i = 0; i < NUM_ITEM_POOLS; i++)
    {
        ShopItem* si = &shop.items[i];

        si->desiredPosition.y += sin((shop.timer)*6)/16.0f;

        if (si == shop.heldItem)
            continue;
        si->position = Towards_Angled(si->position,si->desiredPosition,dt*60);

    }


    for (int i = 0; i < NUM_ITEM_POOLS; i++)
    {
        ShopItem* si = &shop.items[i];

        int w = si->item->spriteIndex_Icon > 0 ? GetWidthSprite(&sprites[si->item->spriteIndex_Icon]) : 24;
        int h = si->item->spriteIndex_Icon > 0 ? GetHeightSprite(&sprites[si->item->spriteIndex_Icon]) : 24;
        float r = _MAX(w,h)/2.0f;

        float cx = si->position.x + w/2.0f;
        float cy = si->position.y + h/2.0f;
        if (ItemIsPurchasable(si->item)  && mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1) && dist(mouseState.screenX,mouseState.screenY,cx,cy) <= r)
        {
            shop.heldItem = &shop.items[i];
            shop.heldOffset.x = mouseState.screenX - si->position.x;
            shop.heldOffset.y = mouseState.screenY - si->position.y;
        }
    }



}
bool ItemIsPurchasable(Item* i)
{
    if (GetGold() >= i->goldCost)
        return true;
    return false;
}
void DrawShopItems(float dt, MouseState mouseState)
{
    for (int i = 0; i < NUM_ITEM_POOLS; i++)
    {
        ShopItem* si = &shop.items[i];

        int w = si->item->spriteIndex_Icon > 0 ? GetWidthSprite(&sprites[si->item->spriteIndex_Icon]) : 24;
        int h = si->item->spriteIndex_Icon > 0 ? GetHeightSprite(&sprites[si->item->spriteIndex_Icon]) : 24;
        float r = _MAX(w,h)/2.0f;

        float cx = si->position.x + w/2.0f;
        float cy = si->position.y + h/2.0f;

        ALLEGRO_COLOR col = FRIENDLY;
        if (!ItemIsPurchasable(si->item))
            col = GROUND;

        if ((!shop.heldItem || shop.heldItem == si) && ItemIsPurchasable(si->item) && dist(mouseState.screenX,mouseState.screenY,cx,cy) <= r)
        {
            al_draw_filled_circle(cx,cy, r,FRIENDLY);
            DrawSprite(&sprites[si->item->spriteIndex_Icon],si->position.x,si->position.y,0,0,0,BG,false);
        }
        else
        {
            al_draw_circle(cx,cy, r,col,1);
            DrawSprite(&sprites[si->item->spriteIndex_Icon],si->position.x,si->position.y,0,0,0,col,false);
        }
        char* price = calloc(NumDigits(si->item->goldCost)+1,sizeof(char));
        sprintf(price,"%i",si->item->goldCost);
        al_draw_text(ui.tinyFont,col,cx,si->position.y+h+3,ALLEGRO_ALIGN_CENTRE,price);
        free(price);


        if (dist(mouseState.screenX,mouseState.screenY,cx,cy) <= r || shop.heldItem)
        {
            ShopItem* draw = si;
            if (shop.heldItem)
                draw = shop.heldItem;

            al_set_clipping_rectangle(16,161,136,81);
            al_draw_multiline_text(ui.boldFont, FRIENDLY, 18, 168, 126, 10, ALLEGRO_ALIGN_LEFT, draw->item->name);
            al_draw_multiline_text(ui.font, FRIENDLY, 18, 184, 126, 10, ALLEGRO_ALIGN_LEFT, draw->item->description);
            al_reset_clipping_rectangle();
        }

    }

    
}
void DrawShopObjects(MouseState mouseState, MouseState mouseStateLastFrame)
{
    int x = shop.startX * 2 + GetWidthSprite(&sprites[shop.spriteIndex_stall]);
    int y = shop.startY;


    for (int i = 0; i < MAX_OBJS; i++)
    {
        GameObject* g = &objects[i];

        if (!IsActive(g) || !IsOwnedByPlayer(g))
            continue;

        DrawSprite(&sprites[g->spriteIndex],x,y,0,0,0,FRIENDLY,false);
        y += GetHeight(g)+1;
        
        float slotX = x;
        float slotY = y;
        for (int i = 0; i < INVENTORY_SLOTS; i++)
        {
            al_draw_circle(slotX+12,slotY+12,12,FRIENDLY,1);
            DrawSprite(&sprites[g->inventory[i].spriteIndex_Icon],slotX,slotY,0,0,0,FRIENDLY,false);
            slotX += 25;
        }
        if (shop.heldItem)
        {
            Rect r = (Rect){x-1,y-1,slotX-x+1,25};
            if (NumAttachedItems(g) == INVENTORY_SLOTS)
            {
                Rect r = (Rect){x-1,y-1,1+slotX-x,25};
                DrawOutlinedRect_Dithered(r,FRIENDLY);
            }
            else
                al_draw_rectangle(r.x, r.y, r.x + r.w, r.y + r.h,FRIENDLY,1);
                //al_draw_rectangle(x-1,y-1,x + 1 + slotX-x,y+25,FRIENDLY,1);
            
            if (!(mouseState.mouse.buttons & 1) && PointInRect(mouseState.screenX,mouseState.screenY,r))
            {
                BuyItem(shop.heldItem->item);
                AttachItem(g,shop.heldItem->item);
                
                shop.heldItem = NULL;
                RefreshShop();
            }
        }
        y += 24+4;

    }

    for (int i = 0; i < currEncounterRunning->numUnitsToSelect; i++)
    {   
        GameObject* g = &deadFriendlyObjects[i];
        if (IsActive(g))
        {
            Sprite* ghost = &sprites[LoadSprite("assets/ui/shop/ghost.png",true)];

            Rect r = (Rect){x-1,y-1,(x + (x-_SCREEN_SIZE)-5),_MAX(GetHeightSprite(ghost),GetHeight(g)+1)};
            
            if (players[0].gold >= g->ressurectionCost)
                al_draw_rectangle(r.x, r.y, r.x + r.w, r.y + r.h, FRIENDLY,1);

            DrawSprite(&sprites[g->spriteIndex],x,y,0,0,0,FRIENDLY,true);
            DrawSprite(ghost,x+GetWidthSprite(&sprites[g->spriteIndex])+10,y,0,0,0,FRIENDLY,false);

            char textBuffer[NumDigits(INT_MAX)+3];
            memset(textBuffer,0,sizeof(char)*NumDigits(INT_MAX)+3);
            sprintf(textBuffer,"%i",g->ressurectionCost);
            
        
            int textX = x+GetWidthSprite(&sprites[g->spriteIndex])+35;
            int textY = y+_MAX(GetHeightSprite(ghost),GetHeight(g))/2 - al_get_font_line_height(ui.font)/2;
            DrawSprite(&sprites[ui.gold_element_sprite_index],textX,textY,0,0,0,FRIENDLY,false);

            al_draw_text(ui.font,FRIENDLY,textX+GetWidthSprite(&sprites[ui.gold_element_sprite_index])+1,textY,ALLEGRO_ALIGN_LEFT,textBuffer);
            y += GetHeight(g)+1;

            y += 24 + 4;
   
            if (players[0].gold >= g->ressurectionCost)
                 if ((mouseStateLastFrame.mouse.buttons) & 1 && !(mouseState.mouse.buttons & 1) && PointInRect(mouseState.screenX,mouseState.screenY,r))
                {
                    RessurectGameObject(g);
                }
        }
    }

    if (!(mouseState.mouse.buttons & 1))
        shop.heldItem = NULL;

}
void DrawShop(float dt, MouseState mouseState, MouseState mouseStateLastFrame)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    int startX = shop.startX; int startY = shop.startY;
    int shopkeeperOffsetX = 18;
    int shopkeeperOffsetY = 44;

    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

    DrawSprite(&sprites[shop.spriteIndex_stall],startX,startY,0,0,0,FRIENDLY,false);
    DrawAnimation(shop.currAnimation,startX + shopkeeperOffsetX,startY + shopkeeperOffsetY,COLOR_FRIENDLY,false);
    DrawButton(&shop.continueButton,shop.continueButton.x,shop.continueButton.y,mouseState,true,BG,true);

    DrawShopObjects(mouseState,mouseStateLastFrame);
    DrawShopItems(dt,mouseState);
}