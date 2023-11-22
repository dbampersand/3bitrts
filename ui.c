#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "ui.h"

#include "gameobject.h"
#include "colors.h"
#include "player.h"
#include "video.h" 
#include "pointspace.h"
#include "sprite.h"
#include "encounter.h"
#include "gamestate.h"
#include "map.h"
#include "effect.h"
#include "helperfuncs.h"
#include "sound.h"
#include "luafuncs.h"
#include "loadscreen.h"
#include "settings.h"
#include "augment.h"
#include "dirent.h"
#include "replay.h"
#include "particle.h"
#include "easings.h"
#include "shop.h"
#include "gamesave.h"
#include "editor.h"

Widget* Widgets_States[NUMGAMESTATES] = {0};
int numSprites_States[NUMGAMESTATES] = {0};

Chatbox* chatboxes = NULL;
int numChatboxes = 0;

Chatbox* chatboxShowing = NULL; 

UI ui = {0};
char* stackDrawBuffer = NULL;

float debounceTimer = 0;
float debounceTime = 0.15;

bool _PANEL_CLICKED_THIS_FRAME = false;
bool _TEXTINPUT_HIGHLIGHTED = false;

#define AUGMENT_NUMBER_FMT "Augment %i"
#define DAMAGE_NUMBER_FMT "+%i%% Damage"
#define HP_NUMBER_FMT "+%i%% HP"
#define GOLD_NUMBER_FMT "+%i%% Gold"

bool _UI_IGNORE_INPUT;


void DrawChestCompletionHint(int x, int y)
{
    float minutes = gameStats.timeTaken/60.0f;
    int end = -1;
    for (int i = MAX_CHESTS-1; i >= 0; i--)
    {
        if (minutes < currEncounterRunning->timeBreakpoints[i])
        {
            end = i;
            break;
        }
    } 
    int chestIndex = LoadSprite("assets/ui/endscreen/chest_tiny.png",false);
    Sprite* s = &sprites[chestIndex];

    for (int i = 0; i < end; i++)
    {
        DrawSprite(s,x+(GetWidthSprite(s)+2)*i,y,0.5f,0.5f,0,FRIENDLY,false,false,false);
    }
    if (end >= 0)
    {
        float range = 0;
        if (end != MAX_CHESTS-1)
            range += currEncounterRunning->timeBreakpoints[end+1];

        float percent = 1-((minutes-range) / (currEncounterRunning->timeBreakpoints[end]-range));

        DrawSpriteRegion(s,0,0,GetWidthSprite(s)*percent,GetHeightSprite(s),x+(GetWidthSprite(s)+2)*end,y,FRIENDLY,false);
    }

}

bool ButtonIsMousedOver(UIElement* u)
{
    Button* b = (Button*)u->data;
    return b->mousedOver;
}
bool ButtonIsHeld(UIElement* u)
{
    Button* b = (Button*)u->data;
    return b->clicked;
}

void DisableButton(UIElement* u)
{
    Button* b = (Button*)u->data;
    b->activated = false;
    b->clicked = false;

}
ALLEGRO_FONT* GetElementFont(UIElement* u)
{
    if (!u->font)
        return ui.font;
    ALLEGRO_FONT* f = *u->font;
    return f;
}
void DrawItems(GameObject* selected)
{
    int y = 28;
    int cx = 233;
    for (int i = 0; i < INVENTORY_SLOTS; i++)
    {
        if (selected->inventory[i].spriteIndex_Icon)
        {
            Sprite* s = &sprites[selected->inventory[i].spriteIndex_Icon];

            ALLEGRO_COLOR c = FRIENDLY;
            c.r *= 0.1f;
            c.g *= 0.1f;
            c.b *= 0.1f;
            c.a = 0.1f;
            if (selected->inventory[i].highlighted)
            {


                al_draw_circle(cx,y+GetHeightSprite(s)/2.0f,_SHOP_ITEM_RADIUS+sin(_FRAMES/16),FRIENDLY,1);
                al_draw_filled_circle(cx,y+GetHeightSprite(s)/2.0f,_SHOP_ITEM_RADIUS+sin(_FRAMES/16),c);
                DrawSprite(s,cx-GetWidthSprite(s)/2.0f,y,0,0,0,FRIENDLY,false,false,false);
                if (selected->inventory[i].stacksString)
                    al_draw_text(ui.tinyFont,FRIENDLY,cx,y+GetHeightSprite(s),ALLEGRO_ALIGN_CENTER,selected->inventory[i].stacksString);


            }
            else
            {

                al_draw_filled_circle(cx,y+GetHeightSprite(s)/2.0f,_SHOP_ITEM_RADIUS+sin(_FRAMES/16),c);
                DrawSprite(s,cx-GetWidthSprite(s)/2.0f,y,0,0,0,FRIENDLY,false,false,false);
                if (selected->inventory[i].stacksString)
                    al_draw_text(ui.tinyFont,FRIENDLY,cx,y+GetHeightSprite(s),ALLEGRO_ALIGN_CENTER,selected->inventory[i].stacksString);

            }
            y += GetHeightSprite(s) + 8;
        }
    }
}
void DrawUIHighlight(UIElement* u, float x, float y)
{
    if (!_FRAMES_HAS_MOVED_ONE)
        return;
    int total = u->w*2 + u->h*2;
    int amtThrough = (int)(_FRAMES) % total;

    int numParticlesToAdd = 2;

    float lifetimeMin = 0.1f;
    float lifetimeMax = 1.0f;

    float speedMin = 6;
    float speedMax = 54;

    //we are on the top of the rectangle
    if (amtThrough >= 0 && amtThrough < u->w)
    {
        int x2 = x + amtThrough;
        for (int i = 0; i < numParticlesToAdd; i++)
            AddParticleWithRandomProperties(x2,y,COLOR_FRIENDLY,lifetimeMin,lifetimeMax,speedMin,speedMax,-2*M_PI, 2*M_PI);

    }
    //right side vertical, going down
    if (amtThrough >= u->w && amtThrough < u->w + u->h)
    {
        int x2 = x + u->w;
        int y2 = y + (amtThrough - u->w);
        for (int i = 0; i < numParticlesToAdd; i++)
            AddParticleWithRandomProperties(x2,y2,COLOR_FRIENDLY,lifetimeMin,lifetimeMax,speedMin,speedMax,-2*M_PI, 2*M_PI);
    }
    //bottom side horizontal, going right to left
    if (amtThrough >= u->w+u->h && amtThrough < u->w*2 + u->h)
    {
        int x2 = (x+u->w) - (amtThrough - (u->w + u->h));
        int y2 = y + u->h;
        for (int i = 0; i < numParticlesToAdd; i++)
            AddParticleWithRandomProperties(x2,y2,COLOR_FRIENDLY,lifetimeMin,lifetimeMax,speedMin,speedMax,-2*M_PI, 2*M_PI);

    }
    //left side vertical, going up
    if (amtThrough >= u->w*2+u->h && amtThrough < u->w*2 + u->h*2)
    {
        int x2 = (x);
        int y2 = y +u->h - (amtThrough - (u->w*2+u->h));
        for (int i = 0; i < numParticlesToAdd; i++)
            AddParticleWithRandomProperties(x2,y2,COLOR_FRIENDLY,lifetimeMin,lifetimeMax,speedMin,speedMax,-2*M_PI, 2*M_PI);

    }


}
bool DebounceActive()
{
    return (debounceTimer > 0);
}
void UpdateDebounce(float dt)
{
    debounceTimer -= dt;
    if (debounceTimer < 0)
        debounceTimer = 0;
}
void ActivateDebounce()
{
    if (!DebounceActive())
        debounceTimer = debounceTime;
}
void InitPurchasingUnitsUI()
{
    PurchasingUnitUI* purchaseUI = &ui.purchasingUnitUI;
    if (!purchaseUI->prefabs)
    {
        int numPlayerChoosable = 0;
        for (int i = 0; i < numPrefabs; i++)
        {
            if (prefabs[i]->playerChoosable)
            {
                numPlayerChoosable++;
            }
        }
        purchaseUI->prefabs = calloc(numPlayerChoosable,sizeof(GameObject*));
        int index = 0;
        for (int i = 0; i < numPrefabs; i++)
        {
            if (prefabs[i]->playerChoosable)
            {
                purchaseUI->prefabs[index] = prefabs[i];
                index++;
            }
        }
        purchaseUI->numPrefabs = numPlayerChoosable;
    }
}
void DrawPurchasingUnitsUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    PurchasingUnitUI* purchaseUI = &ui.purchasingUnitUI;
    if (!purchaseUI->prefabs)
    {
        InitPurchasingUnitsUI();
    }
    DrawFilledRectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);
    GameObject* prefabDrawing = purchaseUI->prefabs[purchaseUI->currentIndex];
    ALLEGRO_COLOR c = FRIENDLY;
    if (!prefabDrawing->purchased)
    {
        c = GROUND;
        unsigned char r; unsigned char g; unsigned char b;
        al_unmap_rgb(c,&r,&g,&b);
        r = clamp(r+60,0,255);
        g = clamp(g+60,0,255);
        b = clamp(b+60,0,255);

        c = al_map_rgb(r,g,b);
    }

    Sprite* s = &sprites[prefabDrawing->spriteIndex_PurchaseScreenSprite];

    int x = (_SCREEN_SIZE-GetWidthSprite(s)); 
    //if (!purchaseUI->isTransitionOut)
    float timer = easeInOutQuint(purchaseUI->transitionTimer);
    x = (_SCREEN_SIZE-GetWidthSprite(s))-(GetWidthSprite(s)*(-timer));

    int y = 0;
    y += (sinf(_FRAMES/30.0f)+1)*5;
    DrawSprite(s,x,y,0,0,0,c,false,false,false);


    int abilityY = 198;
    
    int abilitySize = 30;
    int paragraphStartX = 8;

    Ability* mousedOver = NULL; 


    if (prefabDrawing->numAbilities <= 4)
    {
        
        for (int i = 0; i < prefabDrawing->numAbilities; i++)
        {
            int xPos = (paragraphStartX + (i*abilitySize) + i*2) + _SCREEN_SIZE;
            xPos -= (_SCREEN_SIZE)*(purchaseUI->transitionTimer+1);
            int yPos = abilityY;
            DrawFilledRectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,BG);
            //DrawRectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,FRIENDLY,1);
            //DrawSprite(&sprites[prefabDrawing->abilities[i].spriteIndex_Portrait],xPos,yPos,0,0,0,FRIENDLY,false,false,false);
            Rect r = (Rect){xPos,yPos,30,30};
            
            if (DrawAbilityPortraits(prefabDrawing,NULL,i,r,PointInRect(mouseState.screenX,mouseState.screenY,r),mouseState, &c,false,true) && !mousedOver)
            {
                mousedOver = &prefabDrawing->abilities[i];
            } 

        }
    }
    
    int totalW = 126; 
    if (prefabDrawing->numAbilities >= 5)
    {
        abilityY = 165;
        
        for (int i = 0; i < 2; i++)
        {
            
            //int xPos = paragraphStartX + (totalW/2*(i+1)) - abilitySize*1.5f;
            int xPos = (paragraphStartX + (totalW/3*(i)) + abilitySize/2*(i+1) + (i*2)) + _SCREEN_SIZE;
            xPos -= (_SCREEN_SIZE)*(purchaseUI->transitionTimer+1);
            
            int yPos = abilityY;
            DrawFilledRectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,BG);
           // DrawRectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,FRIENDLY,1);
            //DrawSprite(&sprites[prefabDrawing->abilities[i].spriteIndex_Portrait],xPos,yPos,0,0,0,FRIENDLY,false,false,false);
            Rect r = (Rect){xPos,yPos,30,30};

            if (DrawAbilityPortraits(prefabDrawing,NULL,i,r,PointInRect(mouseState.screenX,mouseState.screenY,r),mouseState, &c,false,true) && !mousedOver)
            {
                mousedOver = &prefabDrawing->abilities[i];
            } 

        }
        for (int i = 2; i < 5; i++)
        {
            int xPos = (paragraphStartX + (totalW/3*(i-2)) + ((i-2)*2)) + _SCREEN_SIZE;
            xPos -= (_SCREEN_SIZE)*(purchaseUI->transitionTimer+1);

            int yPos = abilityY+34;
            DrawFilledRectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,BG);
            //DrawRectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,FRIENDLY,1);
            //DrawSprite(&sprites[prefabDrawing->abilities[i].spriteIndex_Portrait],xPos,yPos,0,0,0,FRIENDLY,false,false,false);
            Rect r = (Rect){xPos,yPos,30,30};
            if (DrawAbilityPortraits(prefabDrawing,NULL,i,r,PointInRect(mouseState.screenX,mouseState.screenY,r),mouseState, &c,false,true) && !mousedOver)
            {
                mousedOver = &prefabDrawing->abilities[i];
            } 

        }

    }

    int paragraphX = paragraphStartX - (_SCREEN_SIZE*(purchaseUI->transitionTimer));
    
    int paragraphY = 34;
    int paragraphWMax = _SCREEN_SIZE - GetWidthSprite(s) - paragraphStartX - 5;
    int titleY = 15;
    //TODO: more than one page for this? If it overflows, add a next/previous page button
    int clippingHeight = abilityY - paragraphY -  8;  

    char* description = prefabDrawing->description;
    if (mousedOver)
        description = mousedOver->description;

    //Draw unit square small portrait
    int spriteX = paragraphX;
    DrawSprite(&sprites[prefabDrawing->spriteIndex],spriteX,titleY,0,0,0,c,false,false,false);

    al_draw_text(ui.font,c,paragraphX + GetWidth(prefabDrawing)+2,titleY+GetHeight(prefabDrawing)/2, ALLEGRO_ALIGN_LEFT,prefabDrawing->name ? prefabDrawing->name : "");

    al_set_clipping_rectangle(paragraphX,paragraphY,paragraphWMax,clippingHeight);
    al_draw_multiline_text(ui.tinyFont,c,paragraphX,paragraphY,paragraphWMax,8,ALLEGRO_ALIGN_LEFT,description ? description : "");
    al_reset_clipping_rectangle();
  
    if (purchaseUI->currentIndex == 0)
        purchaseUI->back.enabled = false;
    else
        purchaseUI->back.enabled = true;

    if (purchaseUI->currentIndex == purchaseUI->numPrefabs-1)
        purchaseUI->next.enabled = false;
    else
        purchaseUI->next.enabled = true;


    UpdateButton(purchaseUI->back.x,purchaseUI->back.y,purchaseUI->back.w,purchaseUI->back.h,&purchaseUI->back,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->back,purchaseUI->back.x,purchaseUI->back.y,&mouseState,ui.menuButton.bgColor,COLOR_FRIENDLY,false);

    UpdateButton(purchaseUI->next.x,purchaseUI->next.y,purchaseUI->next.w,purchaseUI->next.h,&purchaseUI->next,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->next,purchaseUI->next.x,purchaseUI->next.y,&mouseState,ui.menuButton.bgColor,COLOR_FRIENDLY,false);

    UpdateButton(purchaseUI->returnButton.x,purchaseUI->returnButton.y,purchaseUI->returnButton.w,purchaseUI->returnButton.h,&purchaseUI->returnButton,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->returnButton,purchaseUI->returnButton.x,purchaseUI->returnButton.y,&mouseState,ui.menuButton.bgColor,COLOR_FRIENDLY,false);

    if (!prefabDrawing->purchased)
    {
        int y = purchaseUI->purchaseButton.y-10;
        ALLEGRO_COLOR c = FRIENDLY;
        if (players[0].gold < prefabDrawing->cost)
            c = GROUND;
        char* cost = calloc(NumDigits(prefabDrawing->cost)+1,sizeof(char));
        sprintf(cost,"%i",prefabDrawing->cost);
        int x = purchaseUI->purchaseButton.x+purchaseUI->purchaseButton.w/2.0f;
        x -= (_SCREEN_SIZE*(purchaseUI->transitionTimer));

        al_draw_text(ui.font,c,x,y,ALLEGRO_ALIGN_CENTER,cost);
        x -= al_get_text_width(ui.font,cost)/2.0f;
        x -= 7;
        y += 2;
        DrawSprite(&sprites[ui.gold_element_sprite_index],x,y,0,0,0,c,false,false,false);

        free(cost);

    }
    UpdateButton(purchaseUI->purchaseButton.x,purchaseUI->purchaseButton.y,purchaseUI->purchaseButton.w,purchaseUI->purchaseButton.h,&purchaseUI->purchaseButton,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->purchaseButton,purchaseUI->purchaseButton.x,purchaseUI->purchaseButton.y,&mouseState,ui.menuButton.bgColor,COLOR_FRIENDLY,false);

    if ((GetButtonIsClicked(&purchaseUI->back) || IsBindDownThisFrame(keyStateThisFrame,keyStateLastFrame,currSettings.keymap.key_PanLeft)) && purchaseUI->currentIndex != 0)
    {
        int sound = LoadSound("assets/audio/ui_slide.wav");
        Sound* s = &sounds[sound];
        PlaySound(s,1,0,false);

        purchaseUI->indexTransitioningTo--;
        purchaseUI->indexTransitioningTo = clamp(purchaseUI->indexTransitioningTo,0,purchaseUI->numPrefabs-1);

       // purchaseUI->transitionTimer = 0;
        purchaseUI->isTransitionOut = true;
        purchaseUI->isTransitioning = true;

    }


    if ((GetButtonIsClicked(&purchaseUI->next) || IsBindDownThisFrame(keyStateThisFrame,keyStateLastFrame,currSettings.keymap.key_PanRight)) && purchaseUI->currentIndex != purchaseUI->numPrefabs-1)
    {
        int sound = LoadSound("assets/audio/ui_slide.wav");
        Sound* s = &sounds[sound];
        PlaySound(s,1,0,false);

        purchaseUI->indexTransitioningTo++;
        purchaseUI->indexTransitioningTo = clamp(purchaseUI->indexTransitioningTo,0,purchaseUI->numPrefabs-1);
       // purchaseUI->transitionTimer = 0;
        purchaseUI->isTransitionOut = true;
        purchaseUI->isTransitioning = true;
    }
    if (players[0].bankedGold < prefabDrawing->cost || prefabDrawing->purchased)
    {
        purchaseUI->purchaseButton.enabled = false;
    }
    else
    {
        purchaseUI->purchaseButton.enabled = true;
    }

    if (GetButtonIsClicked(&purchaseUI->purchaseButton))
    {
        prefabDrawing->purchased = true;
        AddGold(-prefabDrawing->cost);
        players[0].bankedGold = players[0].gold;

        Save("_save.save");
        SetGameStateToChoosingParty();
    }
    if (GetButtonIsClicked(&purchaseUI->returnButton) || IsBindDownThisFrame(keyStateThisFrame,keyStateLastFrame,currSettings.keymap.key_Cancel))
    {
        SetGameStateToChoosingParty();
    }
    if (purchaseUI->isTransitionOut)
        purchaseUI->transitionTimer += dt*3.5f;
    else
        purchaseUI->transitionTimer -= dt*3.5f;

    purchaseUI->transitionTimer = clamp(purchaseUI->transitionTimer,-1,1);

    if (purchaseUI->transitionTimer >= 1)
    {
        purchaseUI->currentIndex = purchaseUI->indexTransitioningTo;
        purchaseUI->isTransitionOut = false;
    }
    if (purchaseUI->transitionTimer < 0)
    {
        purchaseUI->isTransitioning = false;

    }
    if (!purchaseUI->isTransitioning)
    {
        purchaseUI->transitionTimer = 0;
    }




}
void DrawTimer(bool enabled)
{   
    if (enabled)
    {
        int hours = floorf(gameStats.timeTaken/(60.0f*60.0f));
        int minutes = floorf(gameStats.timeTaken/(60.0f));  
        int seconds = floorf(gameStats.timeTaken - minutes*60); 

        size_t buffsiz = snprintf(NULL, 0, "%i:%i",minutes,seconds);
        char* buff = calloc(buffsiz+1,sizeof(char));
        sprintf(buff,"%i:%i",minutes,seconds);
        al_draw_text(ui.font,FRIENDLY,233,24,ALLEGRO_ALIGN_CENTER,buff);
        free(buff);
    }

}

void DrawUIChatbox()
{
    if (chatboxes)
    {
        Chatbox* c = chatboxShowing;
        if (c && c->text)
        {
            
            DrawDescriptionBox(c->displayingUpTo,5,ui.font,ui.boldFont,c->x,c->y,c->w,c->h,FRIENDLY,c->allowsInteraction);
        }
    }
}
void UpdateChatbox(float dt)
{
    Chatbox* c = chatboxShowing;
    if (c)
    {
        if (c->text && c->displayingUpTo)
        {
             chatboxes[numChatboxes-1].charTimer += dt*10.0f;
             if (chatboxes[numChatboxes-1].charTimer >= 1.0f)
             {
                chatboxes[numChatboxes-1].charTimer = 0;
                int index = strlen(c->displayingUpTo);
                c->displayingUpTo[index] = c->text[index];
             }
        }
    }
}
void EndChatbox()
{
    ClearChatbox();
    if (currMap && strcmp(currMap->name,"unitselect") == 0)
    {
        gameState = GAMESTATE_CHOOSING_UNITS;
        transitioningTo = GAMESTATE_CHOOSING_UNITS;

    } 
    else
    {
        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME; 
    }
}
void ClearChatbox()
{
    for (int i = 0; i < numChatboxes; i++)
    {
        if (chatboxes[i].text)
            free(chatboxes[i].text);
        if (chatboxes[i].displayingUpTo)
            free(chatboxes[i].displayingUpTo);
    }
    if (chatboxes)
        free(chatboxes);
    chatboxes = NULL;
    chatboxShowing = NULL;
    numChatboxes = 0;


}
void Chatbox_NextLine()
{
    players[0].selecting = false;
    players[0].amoveSelected = false;
    players[0].abilityHeld = NULL;
    players[0].clickedThisFrame = NULL;
    if (strlen(chatboxShowing->displayingUpTo) == strlen(chatboxShowing->text))
    {
        chatboxShowing = &chatboxes[(chatboxShowing-chatboxes)+1];

        if ((chatboxShowing-chatboxes) >= numChatboxes)
        {
            EndChatbox();
            return; 
        }
        if (chatboxShowing->allowsInteraction)
        {
            gameState = GAMESTATE_INGAME;
            transitioningTo = GAMESTATE_INGAME;
        }
        else
        {
            gameState = GAMESTATE_IN_CHATBOX;
            transitioningTo = GAMESTATE_IN_CHATBOX;
        }
    }
    else
    {
        strcpy(chatboxShowing->displayingUpTo,chatboxShowing->text);
    }

}
void GetAbilityClickedInsideUI(MouseState mouseState, MouseState mouseStateLastFrame)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);
    if (PlayerHasEnemyUnitSelected())
        return;

    if (currGameObjRunning)
    {
        players[0].amoveSelected = false;
        int index = GetAbilityClicked(&mouseState,&mouseStateLastFrame);
        if (index != -1 && (currGameObjRunning->numAbilities -1 >= index))
        {
            Ability* a = &currGameObjRunning->abilities[index];
            if (AbilityIsCastImmediately(a))
            {
                float x; float y; 
                GetCentre(currGameObjRunning,&x,&y);

                currAbilityRunning = a; 
                players[0].abilityHeld = NULL;
                CastAbility(currGameObjRunning,a,x,y,x,y,NULL);

            }
            else
            {
                players[0].abilityHeld = &currGameObjRunning->abilities[index]; 
                currAbilityRunning =  &currGameObjRunning->abilities[index];
            }
        }   
    }

}
void DrawMouse(MouseState* mouseState, GameObject* mousedOver)
{
    #ifdef WINDOW_FOCUS_LOGIC
    if (!_IS_FOCUSED_WINDOW)
        return;
    #endif
    MouseState mouse = *mouseState;
    //ToScreenSpaceI(&mouse.x,&mouse.y);
    if (!GameIsIngame())
    {
        mousedOver = NULL; 
        players[0].abilityHeld = NULL;
    }
    if (players[0].abilityHeld)
    {
        ALLEGRO_COLOR color = FRIENDLY;
        int cursorIndex = ui.cursorCastingIndex;
        if (mousedOver)
        {
            if (IsOwnedByPlayer(mousedOver))
            {
                if (players[0].abilityHeld->castType & ABILITY_TARGET_FRIENDLY || players[0].abilityHeld->castType & ABILITY_TARGET_ALL)
                {
                        color = HEAL;
                }

            }
            if (!IsOwnedByPlayer(mousedOver))
            {
                if (players[0].abilityHeld->castType & ABILITY_TARGET_ENEMY || players[0].abilityHeld->castType & ABILITY_TARGET_ALL)
                {
                    color = DAMAGE;
                }
            }


        }
        else
        {
            if (players[0].abilityHeld->castType & ABILITY_POINT)
            {
                if (players[0].abilityHeld->castType & ABILITY_TARGET_FRIENDLY)
                {
                    color = HEAL;
                }
                else
                    color = DAMAGE;
                
                if (players[0].abilityHeld->castType & ABILITY_TARGET_FRIENDLY && players[0].abilityHeld->castType & ABILITY_TARGET_ENEMY)
                    color = DAMAGE; 
            }
        }
        DrawCursor(&mouse, ui.cursorCastingIndex, false, color);
    }
    else if (mousedOver)
    {
        if (mousedOver->properties & OBJ_OWNED_BY && players[0].numUnitsSelected > 0)
            DrawCursor(&mouse, ui.cursorAttackIndex,false,DAMAGE);
        else if (!(mousedOver->properties & OBJ_OWNED_BY))
            DrawCursor(&mouse, ui.cursorFriendlyIndex, false,FRIENDLY);
        else
            DrawCursor(&mouse, ui.cursorDefaultIndex, false,FRIENDLY);

    }
    else if (players[0].amoveSelected)
    {
        DrawCursor(&mouse, ui.cursorAttackIndex,false,DAMAGE);
    }
    else 
    {
        DrawCursor(&mouse, ui.cursorDefaultIndex, false,FRIENDLY);
    }

}
//float angle;
//ALLEGRO_BITMAP* wheel;
void DrawUnitChoiceUI(MouseState* mouseState, MouseState* mouseStateLastFrame)
{
        //angle += 0.1f;

        //if (!wheel)
         //   wheel = al_load_bitmap("assets/decor/wheel.png");
       // al_draw_rotated_bitmap(wheel,41/2,41/2,50,50,DegToRad(angle),0);
        
        Encounter* e = encounters[selectedEncounterIndex];

        int numUnitsSelected = 0;
        for (int i = 0; i < players[0].numUnitsSelected; i++)
        {
            if (IsActive(players[0].selection[i]) && players[0].selection[i]->playerChoosable)
            {
                numUnitsSelected++;
            }
        }

        UpdateButton(45,194,ui.choosingUnits_Back.w,ui.choosingUnits_Back.h,&ui.choosingUnits_Back,*mouseState,*mouseStateLastFrame);
        UpdateButton(109,194,ui.choosingUnits_GO.w,ui.choosingUnits_GO.h,&ui.choosingUnits_GO,*mouseState,*mouseStateLastFrame);
        UpdateButton(ui.choosingUnits_Hire.x,ui.choosingUnits_Hire.y,ui.choosingUnits_Hire.w,ui.choosingUnits_Hire.h,&ui.choosingUnits_Hire,*mouseState,*mouseStateLastFrame);

        if (GetButtonIsClicked(&ui.choosingUnits_Hire))
        {
            SetGameStateToPurchasingUnits();
        }

        if (numUnitsSelected==e->numUnitsToSelect)
        {
            ui.choosingUnits_GO.enabled = true;
            DrawUIHighlight(&ui.choosingUnits_GO,ToWorldSpace_X(ui.choosingUnits_GO.x),ToWorldSpace_Y(ui.choosingUnits_GO.y));
        }
        else
        {
            ui.choosingUnits_GO.enabled = false;
        }
        DrawUIElement(&ui.choosingUnits_Back,ui.choosingUnits_Back.x,ui.choosingUnits_Back.y,mouseState,ui.choosingUnits_Back.bgColor,COLOR_FRIENDLY,false);
        DrawUIElement(&ui.choosingUnits_GO,ui.choosingUnits_GO.x,ui.choosingUnits_GO.y,mouseState,ui.choosingUnits_Back.bgColor,COLOR_FRIENDLY,false);
        DrawUIElement(&ui.choosingUnits_Hire,ui.choosingUnits_Hire.x,ui.choosingUnits_Hire.y,mouseState,ui.choosingUnits_Back.bgColor,COLOR_FRIENDLY,false);
        //DrawUIElement(&ui.choosingUnits_Artifacts,ui.choosingUnits_Artifacts.x,ui.choosingUnits_Artifacts.y,mouseState,ui.choosingUnits_Back.bgColor,COLOR_FRIENDLY,false);

        char* format = "Select %i/%i units";
        char* number = calloc(snprintf(NULL,0,format,numUnitsSelected,e->numUnitsToSelect)+1,sizeof(char));
        sprintf(number,format,numUnitsSelected,e->numUnitsToSelect);

        al_draw_text(ui.font,FRIENDLY,ToScreenSpace_X(ui.choosingUnits_GO.x+ui.choosingUnits_GO.w/2),ToScreenSpace_Y(180),ALLEGRO_ALIGN_CENTER,number);

        free(number);

}
void DrawMouseSelectBox(MouseState mouseState)
{
    Point endSelection = (Point){mouseState.worldX,mouseState.worldY};
    Rect r;
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    int startx = players[0].selectionStart.x;
    int starty = players[0].selectionStart.y;
    int endx = endSelection.x;
    int endy = endSelection.y;


    ToScreenSpaceI(&startx,&starty);
    ToScreenSpaceI(&endx,&endy);

    r.x = _MIN(endx,startx);
    r.y = _MIN(endy,starty);
    r.w = _MAX(endx,startx) - _MIN(endx,startx);
    r.h = _MAX(endy,starty) - _MIN(endy,starty);
    DrawFilledRectangle(r.x, r.y, r.x+r.w, r.y+r.h, al_premul_rgba(255, 255, 255,128));
   
}

void DrawReplayUI(Replay* r, MouseState* mouseState, MouseState* mouseStateLastFrame)
{
    DrawFilledRectangle(3,2,252,16,BG);
    DrawRectangle(3,2,252,16,BG,1);
    
    float percent = r->framePlayPosition / (float)r->numFrames;
    int w = REP_UI_PLAY_SCRUBBER_SIZE * percent;
    if (percent > 1) percent = 1;

    Rect fullPlayBar = (Rect){19,5,REP_UI_PLAY_SCRUBBER_SIZE,13};
    Rect position = (Rect){fullPlayBar.x,fullPlayBar.y,w,fullPlayBar.h};
    if (PointInRect(mouseState->screenX,mouseState->screenY,fullPlayBar))
    {
        if (mouseState->mouse.buttons & 1)
        {
            float changedPercent = (mouseState->screenX-5) / (float)REP_UI_PLAY_SCRUBBER_SIZE;
            changedPercent = clamp(changedPercent,0,1);
            r->framePlayPosition = changedPercent * r->numFrames;
            position.w = fullPlayBar.w * changedPercent;
        }
    }

    DrawRectangle(19,5,222,13,FRIENDLY,1);
    DrawFilledRectangle(position.x,position.y,position.x+position.w,position.h,FRIENDLY);

    DrawButton(&replayPlayButton,replayPlayButton.x,replayPlayButton.y,*mouseState,true,BG,true,FRIENDLY,false);
    UpdateButton(replayPlayButton.x,replayPlayButton.y,replayPlayButton.w,replayPlayButton.h,&replayPlayButton,*mouseState,*mouseStateLastFrame);

    if (GetButtonIsClicked(&replayPlayButton))
    {
        replay.playing = !replay.playing;
        if (replay.playing)
        {
            ChangeButtonImage(&replayPlayButton,LoadSprite("assets/ui/button_pause.png",true));
        }
        else
        {
            ChangeButtonImage(&replayPlayButton,LoadSprite("assets/ui/button_play.png",true));
        }
    }
    DrawButton(&replayBackButton,replayBackButton.x,replayBackButton.y,*mouseState,true,BG,true,FRIENDLY,false);
    UpdateButton(replayBackButton.x,replayBackButton.y,replayBackButton.w,replayBackButton.h,&replayBackButton,*mouseState,*mouseStateLastFrame);
    if (GetButtonIsClicked(&replayBackButton))
    {
        SetGameStateToInMenu();

    }


}
void UpdateInterface(float dt, ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseStateLastFrame)
{
    UpdateUI(keyState,mouseState,keyStateLastFrame,mouseStateLastFrame,dt);
    if (ui.currentPanel == &ui.mainMenuPanel)
    {
        if (!FileExists("continue.sav"))
        {
            UIElement* u = GetUIElement(&ui.mainMenuPanel,"Continue");
            u->enabled = false;
            u->ignoreDraw = true;
        }
        else
        {
            UIElement* u = GetUIElement(&ui.mainMenuPanel,"Continue");
            u->enabled = true;
            u->ignoreDraw = false;
        }   
        bool buttonIsMousedOver = 
            //ButtonIsMousedOver(GetUIElement(&ui.mainMenuPanel,"Load Replay"))
            //||
            ButtonIsMousedOver(GetUIElement(&ui.mainMenuPanel,"Return"))
            ||
            ButtonIsMousedOver(GetUIElement(&ui.mainMenuPanel,"Tutorial"))
            ||
            ButtonIsMousedOver(GetUIElement(&ui.mainMenuPanel,"Options"))
            ||
            ButtonIsMousedOver(GetUIElement(&ui.mainMenuPanel,"Exit"));

        bool continueHeld = ButtonIsHeld(GetUIElement(&ui.mainMenuPanel,"Continue"));
        bool returnHeld = ButtonIsHeld(GetUIElement(&ui.mainMenuPanel,"Return"));
        bool tutorialHeld = ButtonIsHeld(GetUIElement(&ui.mainMenuPanel,"Tutorial"));
        bool optionsHeld = ButtonIsHeld(GetUIElement(&ui.mainMenuPanel,"Options"));
        bool exitHeld = ButtonIsHeld(GetUIElement(&ui.mainMenuPanel,"Exit"));

        bool continueClicked = GetButton(&ui.mainMenuPanel,"Continue");
        bool returnClicked = GetButton(&ui.mainMenuPanel,"Return");
        bool tutorialClicked = GetButton(&ui.mainMenuPanel,"Tutorial");
        bool optionsClicked = GetButton(&ui.mainMenuPanel,"Options");
        bool exitClicked = GetButton(&ui.mainMenuPanel,"Exit");

        bool buttonIsHeld = 
            returnHeld || tutorialHeld || optionsHeld || exitHeld | continueHeld;
        bool buttonIsClicked = 
            returnClicked || tutorialClicked || optionsClicked || exitClicked | continueClicked;

            
        if (buttonIsHeld || buttonIsClicked || GameStateIsTransition(&gameState))
        {
            SetWidgetSprite(GetWidgetByName(GAMESTATE_MAIN_MENU,"Hand"),"assets/ui/mainmenu/fist.png");
        }
        else if (buttonIsMousedOver)
        {
            SetWidgetSprite(GetWidgetByName(GAMESTATE_MAIN_MENU,"Hand"),"assets/ui/mainmenu/semiclosed_hand.png");
        }
        else
        {
            SetWidgetSprite(GetWidgetByName(GAMESTATE_MAIN_MENU,"Hand"),"assets/ui/mainmenu/hand.png");
        }
        if (GetButton(&ui.mainMenuPanel,"Load Replay"))
        {
            ChangeUIPanel(&ui.loadReplayPanel);       
            ClearPanelElements(&ui.loadReplayPanel);
            GenerateFileListButtons("replays/",&ui.loadReplayPanel);
        }
        if (continueClicked)
        {
            bool err = false;
            LoadGameSave("continue.sav", &err);
            if (!err)
                RunGameSave(continuePoint);
        }
        if (returnClicked)
        {
            //ui.currentPanel = NULL;
            if (gameState == GAMESTATE_MAIN_MENU)
            {
                SetGameStateToChoosingEncounter();
                //gameState = GAMESTATE_CHOOSING_ENCOUNTER;
                StopMusic();
                combatStarted = false;
            }
        }
        if (tutorialClicked)
        {
            GoTutorial();
        }
        if (optionsClicked)
        {
            ChangeUIPanel(&ui.videoOptionsPanel);
        }
        if (exitClicked)
        {
            Quit();
        }
    }
    if (ui.currentPanel == &ui.pauseMenuPanel)
    {

        if (GetButton(&ui.pauseMenuPanel,"Return"))
        {
            ChangeUIPanel(NULL);
        }
        if (GetButton(&ui.pauseMenuPanel,"Retry"))
        {
            Retry();

        }

        if (GetButton(&ui.pauseMenuPanel,"Options"))
        {
            ChangeUIPanel(&ui.videoOptionsPanel);
        }
        if (GetButton(&ui.pauseMenuPanel,"Exit"))
        {
            gameStats.gameWon = false;
            ChangeUIPanel(NULL);
            if (gameState == GAMESTATE_CHOOSING_UNITS)
            {
                SetGameStateToInMenu();
            }
            else
            {
                SetGameStateToEndscreen();
            }
            StopMusic();
        }

    }
    if (ui.currentPanel == &ui.loadReplayPanel)
    {
        for (int i = 0 ; i < ui.loadReplayPanel.numElements; i++)
        {
            UIElement* u = &ui.loadReplayPanel.elements[i];
            Button* b = (Button*)u->data;

            //int x; int y;
           // GetUILocation(&ui.loadReplayPanel,u,&x,&y);

           // UpdateButton(x,y,u,mouseState,mouseStateLastFrame);
            if (GetButtonIsClicked(u))
            {
               // gameState = GAMESTATE_WATCHING_REPLAY;
                SetGameStateToWatchingReplay();
                char* path = calloc(snprintf(NULL,0,"replays/%s",u->name)+1,sizeof(char));
                sprintf(path,"replays/%s",u->name);
                LoadReplay(path);
                free(path); 
            }   
        }
    }
    SetOptions();

}

void ChangeUIPanel(Panel* to)
{
    if (ui.currentPanel == to)
        return;
    ui.animatePanel = true;
    ui.changingTo = to;
    ui.animatePanel = UI_ANIMATE_STATIC;

    if (ui.currentPanel == &ui.mainMenuPanel)
    {
        ui.currentPanel = to;   
        ui.panelShownPercent = 0;
        ui.animatePanel = UI_ANIMATE_IN;
    }
}
void DrawHealthUIElement(GameObject* selected, ALLEGRO_COLOR color,MouseState mouseState)
{
    float percentHP = selected->health / selected->maxHP;
    int startHPY = UI_START_Y+1+ 6;
    int startHPX = 4;
    int hpW = 9;
    int hpH = 26;
    if (!selected->usesMana)
        hpW = 24;

    int startY = startHPY + (hpH - (hpH*percentHP));
    int endY = (startHPY + hpH);
    DrawFilledRectangle(startHPX, startY, startHPX+hpW,endY, color);
    if (IsOwnedByPlayer(selected))
        DrawOutlinedRect_Dithered((Rect){startHPX,startHPY,hpW,hpH},DAMAGE);
    Rect r = (Rect){startHPX,startHPY,hpW,hpH};

    if (PointInRect(mouseState.screenX,mouseState.screenY,r))
    {
        char* format = "%.0f / %.0f";
        char* text = calloc(snprintf(NULL,0,format,selected->health,selected->maxHP)+1,sizeof(char));
        sprintf(text,format,selected->health,selected->maxHP);

        int w = al_get_text_width(ui.font,text);
        int h = GetDescriptionBoxH(text,w,ui.font,UI_PADDING);
        int x = r.x;
        int y = 221 - h - 3;

        DrawDescriptionBox(text, 5, ui.font,ui.boldFont, x,y,w,0,GetColor(GameObjToColor(selected),GetPlayerOwnedBy(selected)),true);
        free(text);
    }
}

void DrawManaUIElement(GameObject* selected, ALLEGRO_COLOR color, MouseState mouseState)
{
    if (!selected->usesMana) 
        return;
    float percentMana = selected->mana / selected->maxMana;
    int startManaY = UI_START_Y+1 + 6;
    int startManaX = 18;
    int manaW = 10;
    int manaH = 26;

    int startY = startManaY + (manaH - (manaH*percentMana));
    int endY = (startManaY + manaH);
    DrawFilledRectangle(startManaX, startY, startManaX+manaW,endY, color);
    if (IsOwnedByPlayer(selected))
        DrawOutlinedRect_Dithered((Rect){startManaX,startManaY,manaW,manaH},GetColor(EffectColors[EFFECT_ADD_MANA],0));

    Rect r = (Rect){startManaX,startManaY,manaW,manaH};

    if (PointInRect(mouseState.screenX,mouseState.screenY,r))
    {
        char* format = "%.0f / %.0f";
        char* text = calloc(snprintf(NULL,0,format,selected->mana,selected->maxMana)+1,sizeof(char));
        sprintf(text,format,selected->mana,selected->maxMana);

        int w = al_get_text_width(ui.font,text);
        int h = GetDescriptionBoxH(text,w,ui.font,UI_PADDING);
        int x = r.x;
        int y = 221 - h - 3;

        DrawDescriptionBox(text, 5, ui.font,ui.boldFont, x,y,w,0,GetColor(GameObjToColor(selected),GetPlayerOwnedBy(selected)),true);
        free(text);
    }


}
Rect GetAbilityPortraitRect(int index, int numAbilities)
{
    Rect r;
    r.w = 30; r.h = 30;
    if (index == 0)
    {
        r.x = 33;
        r.y = UI_ABILITY_START_Y;
    }
    if (index == 1)
    {
        r.x = 65;
        r.y = UI_ABILITY_START_Y;
    }
    if (index == 2)
    {
        r.x = 97;
        r.y = UI_ABILITY_START_Y;
    }
    if (index == 3)
    {
        r.x = 129;
        r.y = UI_ABILITY_START_Y;
    }
    if (index == 4)
    {
        r.x = 161;
        r.y = UI_ABILITY_START_Y;
    }

    return r;


}
bool DrawAbility(Ability* ability, int x, int y, ALLEGRO_COLOR color, MouseState* mouse, MouseState* mouseStateLastFrame)
{
    Sprite* s = &sprites[ability->spriteIndex_Portrait];
    int w = al_get_bitmap_width(s->sprite);
    int h = al_get_bitmap_height(s->sprite);

    Rect r = (Rect){x,y,w,h};
    if (w > 0 && h > 0)
        DrawRectangle(x,y,x+w,y+h,color,1);
    bool shouldInvert = false;

    if (mouse && PointInRect(mouse->screenX,mouse->screenY,r))
    {
        shouldInvert = true;
    }
    if (mouse && mouseStateLastFrame)
    {
        if (PointInRect(mouse->screenX,mouse->screenY,r) && !PointInRect(mouseStateLastFrame->screenX,mouseStateLastFrame->screenY,r))
        {
            PlaySoundStr("assets/audio/hover2.wav",1,0,false);
        }
    }

    DrawSprite(s,x,y,0.5f,0.5f,0,color,shouldInvert,false,false);

    if (shouldInvert)
        return true;
    else
        return false;
}
bool DrawEffectPortrait(int x, int y, Effect* e, ALLEGRO_COLOR c, MouseState* mouseState)
{
    Rect r = (Rect){x,y,13,12};

    if (e->enabled && e->trigger != TRIGGER_INSTANT)
    {
        ALLEGRO_COLOR c = GetColor(GetEffectColor(e),0);
        float percent = 1 - (e->totalTimer / e->duration);

        if (percent < 0) 
            percent = 0;
        c = al_map_rgba(c.r*percent*255,c.g*percent*255,c.b*percent*255,c.a*255);

        if (e->spriteIndex_Portrait > 0)
            DrawSprite(&sprites[e->spriteIndex_Portrait],x,y,0.5f,0.5f,0,c,false,false,false);
        else
            DrawFilledRectangle(r.x,r.y,r.x+r.w,r.y+r.h,c);
        if (e->canStack)
        {
            if (e->stacks > 1)
            {
                size_t buffsize = snprintf(NULL, 0, "%i", e->stacks);
                char* stacks = calloc(buffsize+1,sizeof(char));
                sprintf(stacks,"%i",e->stacks);
                DrawFilledRectangle(x,y,x+al_get_text_width(ui.tinyFont,stacks),y+al_get_font_line_height(ui.tinyFont),BG);
                al_draw_text(ui.tinyFont,DAMAGE,x+1,y+1,ALLEGRO_ALIGN_LEFT,stacks);
                free(stacks);
            }
        }
    }
    if (PointInRect(mouseState->screenX,mouseState->screenY,r))
    {
        return true;
    }
    return false;
}
bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, Rect r, bool keydown, MouseState mouseState, ALLEGRO_COLOR* c, bool drawKey, bool ignoreStacks)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    if (selected->abilities[index].spriteIndex_Portrait <= 0) 
        return false;
    if (heldAbility == &selected->abilities[index])
        keydown = true;

    if (!IsOwnedByPlayer(selected))
        keydown = false;

    Sprite* s = &sprites[selected->abilities[index].spriteIndex_Portrait];
    //DrawSprite(s,startX,startY,FRIENDLY,keydown);
    Ability* a = &selected->abilities[index];
    float percent;
    if (a->cooldown == 0) 
        percent = 1.0f;
    else
    {
        percent = 1 - (a->cdTimer / a->cooldown);
    }
    if ((a->stacks > 0 && selected->stunTimer == 0) || ignoreStacks)
        percent = 1;
    int h = al_get_bitmap_height(s->sprite) * percent;
    int w = al_get_bitmap_width(s->sprite);
    if (!ignoreStacks && AbilityIsOnCooldown(a))//a->cdTimer > 0)
    {
        keydown = false;
    }
    ALLEGRO_COLOR* col;
    if (c)
        col = (ObjectHasManaToCast(selected,a) && !AbilityIsOnCooldown(a) && percent >= 1.0f) ? c : &GROUND;
    else
        col = (ObjectHasManaToCast(selected,a) && !AbilityIsOnCooldown(a) && percent >= 1.0f) ? &FRIENDLY : &GROUND;
    if (gameState == GAMESTATE_PURCHASING_UNITS)
        col = c;
    DrawSpriteRegion(s,0,0,w,h,r.x,r.y,*col,keydown);

    //Draw stacks counter
    if (a->maxStacks > 1 && !ignoreStacks)
    {
        int sizeStackCounter = 8;
        int xStackCounter = r.x+r.w-sizeStackCounter;
        int yStackCounter = r.y+r.h-sizeStackCounter;

        DrawFilledRectangle(xStackCounter,yStackCounter,xStackCounter+sizeStackCounter,yStackCounter+sizeStackCounter,BG);
        DrawRectangle(xStackCounter,yStackCounter,xStackCounter+sizeStackCounter,yStackCounter+sizeStackCounter,*c,1);

        const int maxchars = ceilf(log10(pow(2,sizeof(a->stacks)*8)))+2;
        if (!stackDrawBuffer)
            stackDrawBuffer = calloc(maxchars,sizeof(char));
        memset(stackDrawBuffer,0,maxchars*sizeof(char));
        sprintf(stackDrawBuffer,"%i",a->stacks);
        al_draw_text(ui.tinyFont, *c, xStackCounter+sizeStackCounter/2, yStackCounter+sizeStackCounter/2-al_get_font_line_height(ui.tinyFont)/4, ALLEGRO_ALIGN_CENTRE, stackDrawBuffer);

        //draw countdown animation
    }
    if (a->stacks != a->maxStacks && gameState != GAMESTATE_PURCHASING_UNITS)
    {
        float percent = 1-(a->cdTimer / a->cooldown);
        if (percent > 0.01f)
        {
            al_draw_line(r.x,r.y + (r.h*percent),r.x+r.w,r.y+(r.h*percent),BG,1);
            al_draw_line(r.x,r.y + 1 + (r.h*percent),r.x+r.w,r.y + 1 +(r.h*percent),*c,1);
        }
    }

    //Draw key to press for the ability
    const char* key = 0; 
    if (index == 0)
        key = al_keycode_to_name(currSettings.keymap.key_Q.keyMappedTo);
    if (index == 1)
        key = al_keycode_to_name(currSettings.keymap.key_W.keyMappedTo);
    if (index == 2)
        key = al_keycode_to_name(currSettings.keymap.key_E.keyMappedTo);
    if (index == 3)
        key = al_keycode_to_name(currSettings.keymap.key_R.keyMappedTo);
    if (index == 4)
        key = al_keycode_to_name(currSettings.keymap.key_F.keyMappedTo);

    if (selected->abilities[index].hintColorSet)
    {
        Rect rHint = (Rect){r.x+1,r.y+1,r.w-1,r.h-1};
        //dither full rect
        DrawOutlinedRect_Dithered(rHint,GetColor(selected->abilities[index].hintColor,GetPlayerOwnedBy(selected)));

        //horiz
        //al_draw_line(r.x+1,r.y,r.x+r.w,r.y,GetColor(selected->abilities[index].hintColor,GetPlayerOwnedBy(selected)),1);
        //al_draw_line(r.x+1,r.y+r.h-1,r.x+r.w,r.y+r.h-1,GetColor(selected->abilities[index].hintColor,GetPlayerOwnedBy(selected)),1);

        //vertical
        //al_draw_line(r.x+1,r.y,r.x,r.y+r.h-1,GetColor(selected->abilities[index].hintColor,GetPlayerOwnedBy(selected)),1);
        //al_draw_line(r.x+r.w,r.y,r.x+r.w,r.y+r.h-1,GetColor(selected->abilities[index].hintColor,GetPlayerOwnedBy(selected)),1);

        //full rect
        //al_draw_rectangle(r.x+1,r.y,r.x+r.w,r.y+r.h-1,GetColor(selected->abilities[index].hintColor,GetPlayerOwnedBy(selected)),1);
    }
    if (key && drawKey && IsOwnedByPlayer(selected))
    {
        int keyW = al_get_text_width(ui.tinyFont,key)+4;
        int keyH = al_get_font_line_height(ui.tinyFont)+4;

        DrawFilledRectangle(r.x,r.y,r.x+keyW,r.y+keyH,BG);
        DrawRectangle(r.x,r.y,r.x+keyW,r.y+keyH,*c,1);

        al_draw_text(ui.tinyFont, *c, r.x+keyW/2,r.y+keyH/2-1, ALLEGRO_ALIGN_CENTRE, key);

    }

    if (PointInRect(mouseState.screenX,mouseState.screenY,r))
    {
        return true;
    }
    return false;
}
void DrawEffectPortraits(GameObject* selected, Effect** mousedOver,MouseState* mouseState)
{
    if (!selected)  
        return;
        //4-ability UI
        if (selected->numAbilities == 4)
        {

            *mousedOver = DrawEffectPortrait(161,UI_ABILITY_START_Y,&selected->effects[0],FRIENDLY,mouseState) == true ? &selected->effects[0] : *mousedOver;
            *mousedOver = DrawEffectPortrait(177,UI_ABILITY_START_Y,&selected->effects[1],FRIENDLY,mouseState)== true ? &selected->effects[1] : *mousedOver;
            *mousedOver = DrawEffectPortrait(193,UI_ABILITY_START_Y,&selected->effects[2],FRIENDLY,mouseState)== true ? &selected->effects[2] : *mousedOver;
            *mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y,&selected->effects[3],FRIENDLY,mouseState)== true ? &selected->effects[3] : *mousedOver;
            *mousedOver = DrawEffectPortrait(225,UI_ABILITY_START_Y,&selected->effects[4],FRIENDLY,mouseState)== true ? &selected->effects[4] : *mousedOver;
            *mousedOver = DrawEffectPortrait(241,UI_ABILITY_START_Y,&selected->effects[5],FRIENDLY,mouseState)== true ? &selected->effects[5] : *mousedOver;

            *mousedOver = DrawEffectPortrait(161,UI_ABILITY_START_Y+18,&selected->effects[6],FRIENDLY,mouseState)== true ? &selected->effects[6] : *mousedOver;
            *mousedOver = DrawEffectPortrait(177,UI_ABILITY_START_Y+18,&selected->effects[7],FRIENDLY,mouseState)== true ? &selected->effects[7] : *mousedOver;
            *mousedOver = DrawEffectPortrait(193,UI_ABILITY_START_Y+18,&selected->effects[8],FRIENDLY,mouseState)== true ? &selected->effects[8] : *mousedOver;
            *mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y+18,&selected->effects[9],FRIENDLY,mouseState)== true ? &selected->effects[9] : *mousedOver;
            *mousedOver = DrawEffectPortrait(225,UI_ABILITY_START_Y+18,&selected->effects[10],FRIENDLY,mouseState)== true ? &selected->effects[10] : *mousedOver;
            *mousedOver = DrawEffectPortrait(241,UI_ABILITY_START_Y+18,&selected->effects[11],FRIENDLY,mouseState)== true ? &selected->effects[11] : *mousedOver;
        }
        else if (selected->numAbilities <= 3)
        {
            *mousedOver = DrawEffectPortrait(129,UI_ABILITY_START_Y,&selected->effects[0],FRIENDLY,mouseState) == true ? &selected->effects[0] : *mousedOver;
            *mousedOver = DrawEffectPortrait(145,UI_ABILITY_START_Y,&selected->effects[1],FRIENDLY,mouseState)== true ? &selected->effects[1] : *mousedOver;
            *mousedOver = DrawEffectPortrait(161,UI_ABILITY_START_Y,&selected->effects[2],FRIENDLY,mouseState)== true ? &selected->effects[2] : *mousedOver;
            *mousedOver = DrawEffectPortrait(177,UI_ABILITY_START_Y,&selected->effects[3],FRIENDLY,mouseState)== true ? &selected->effects[3] : *mousedOver;
            *mousedOver = DrawEffectPortrait(193,UI_ABILITY_START_Y,&selected->effects[4],FRIENDLY,mouseState)== true ? &selected->effects[4] : *mousedOver;
            *mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y,&selected->effects[5],FRIENDLY,mouseState)== true ? &selected->effects[5] : *mousedOver;
            *mousedOver = DrawEffectPortrait(225,UI_ABILITY_START_Y,&selected->effects[6],FRIENDLY,mouseState)== true ? &selected->effects[6] : *mousedOver;
            *mousedOver = DrawEffectPortrait(241,UI_ABILITY_START_Y,&selected->effects[7],FRIENDLY,mouseState)== true ? &selected->effects[7] : *mousedOver;

            *mousedOver = DrawEffectPortrait(129,UI_ABILITY_START_Y+18,&selected->effects[8],FRIENDLY,mouseState)== true ? &selected->effects[8] : *mousedOver;
            *mousedOver = DrawEffectPortrait(145,UI_ABILITY_START_Y+18,&selected->effects[9],FRIENDLY,mouseState)== true ? &selected->effects[9] : *mousedOver;
            *mousedOver = DrawEffectPortrait(161,UI_ABILITY_START_Y+18,&selected->effects[10],FRIENDLY,mouseState)== true ? &selected->effects[10] : *mousedOver;
            *mousedOver = DrawEffectPortrait(177,UI_ABILITY_START_Y+18,&selected->effects[11],FRIENDLY,mouseState)== true ? &selected->effects[11] : *mousedOver;
            *mousedOver = DrawEffectPortrait(193,UI_ABILITY_START_Y+18,&selected->effects[12],FRIENDLY,mouseState)== true ? &selected->effects[12] : *mousedOver;
            *mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y+18,&selected->effects[13],FRIENDLY,mouseState)== true ? &selected->effects[13] : *mousedOver;
            *mousedOver = DrawEffectPortrait(225,UI_ABILITY_START_Y+18,&selected->effects[14],FRIENDLY,mouseState)== true ? &selected->effects[14] : *mousedOver;
            *mousedOver = DrawEffectPortrait(241,UI_ABILITY_START_Y+18,&selected->effects[15],FRIENDLY,mouseState)== true ? &selected->effects[15] : *mousedOver;

        }
        else if (selected->numAbilities == 5) //5-ability UI
        {
            *mousedOver = DrawEffectPortrait(193,UI_ABILITY_START_Y,&selected->effects[0],FRIENDLY,mouseState) == true ? &selected->effects[0] : *mousedOver;
            *mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y,&selected->effects[1],FRIENDLY,mouseState)== true ? &selected->effects[1] : *mousedOver;
            *mousedOver = DrawEffectPortrait(223,UI_ABILITY_START_Y,&selected->effects[2],FRIENDLY,mouseState)== true ? &selected->effects[2] : *mousedOver;
            *mousedOver = DrawEffectPortrait(238,UI_ABILITY_START_Y,&selected->effects[3],FRIENDLY,mouseState)== true ? &selected->effects[3] : *mousedOver;
            
            
            *mousedOver = DrawEffectPortrait(193,UI_ABILITY_START_Y+18,&selected->effects[4],FRIENDLY,mouseState)== true ? &selected->effects[4] : *mousedOver;
            *mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y+18,&selected->effects[5],FRIENDLY,mouseState)== true ? &selected->effects[5] : *mousedOver;
            *mousedOver = DrawEffectPortrait(223,UI_ABILITY_START_Y+18,&selected->effects[6],FRIENDLY,mouseState)== true ? &selected->effects[6] : *mousedOver;
            *mousedOver = DrawEffectPortrait(238,UI_ABILITY_START_Y+18,&selected->effects[7],FRIENDLY,mouseState)== true ? &selected->effects[7] : *mousedOver;
        }
}
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseState)
{
    GameObject* selected = players[0].selection[players[0].indexSelectedUnit];
    Sprite* s =  &sprites[ui.panel_0_abilities_sprite_index];

    ALLEGRO_COLOR baseColor = (IsOwnedByPlayer(selected) || !selected) ? FRIENDLY : ENEMY;


    DrawFilledRectangle(0, UI_START_Y, _SCREEN_SIZE, _SCREEN_SIZE, BG);

    Effect* mousedOver = NULL; 
    DrawEffectPortraits(selected, &mousedOver, mouseState);


    Sprite* health = &sprites[ui.health_element_sprite_index];
    if (selected)
        health = selected->usesMana ? &sprites[ui.health_and_mana_element_sprite_index] : &sprites[ui.health_element_sprite_index];
    if (!selected)
        s = &sprites[ui.panel_unselected_abilities_sprite_index];
    if (selected)
    {

        if (selected->numAbilities == 0)
            s = &sprites[ui.panel_0_abilities_sprite_index];

        if (selected->numAbilities == 1)
            s = &sprites[ui.panel_1_abilities_sprite_index];

        if (selected->numAbilities == 2)
            s = &sprites[ui.panel_2_abilities_sprite_index];

        if (selected->numAbilities == 3)
            s = &sprites[ui.panel_3_abilities_sprite_index];

        else if (selected->numAbilities == 4)
            s = &sprites[ui.panel_sprite_index];
        else if (selected->numAbilities == 5)
            s = &sprites[ui.panel_5_abilities_sprite_index];


    }
    if (!s) return;

    if (selected)
    {
        DrawHealthUIElement(selected,baseColor,*mouseState);
        DrawManaUIElement(selected,baseColor,*mouseState);
        Ability* heldAbility = players[0].abilityHeld;
        
        DrawItems(selected);

        if (selected->numAbilities >= 1)
            if (DrawAbilityPortraits(selected,heldAbility,0,GetAbilityPortraitRect(0,selected->numAbilities),IsBindDown(keyState,currSettings.keymap.key_Q),*mouseState,&baseColor,true,false))
            {
                if (selected->abilities[0].description)
                {
                    int h = GetDescriptionBoxH(selected->abilities[0].description,100,ui.font,UI_PADDING);
                    int x = 33 + ceilf(UI_PADDING/2.0f);
                    int y = 221 - h - 3;
                    DrawDescriptionBox(selected->abilities[0].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
                }

            }
        if (selected->numAbilities >= 2)
            if (DrawAbilityPortraits(selected,heldAbility,1,GetAbilityPortraitRect(1,selected->numAbilities),IsBindDown(keyState,currSettings.keymap.key_W),*mouseState,&baseColor,true,false))
            {
                if (selected->abilities[1].description)
                {
                    int h = GetDescriptionBoxH(selected->abilities[1].description,100,ui.font,UI_PADDING);
                    int x = 65 + ceilf(UI_PADDING/2.0f);
                    int y = 221 - h - 3;
                    DrawDescriptionBox(selected->abilities[1].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
                }
            }
        if (selected->numAbilities >= 3)
            if (DrawAbilityPortraits(selected,heldAbility,2,GetAbilityPortraitRect(2,selected->numAbilities),IsBindDown(keyState,currSettings.keymap.key_E),*mouseState,&baseColor,true,false))
            {
                if (selected->abilities[2].description)
                {
                    int h = GetDescriptionBoxH(selected->abilities[2].description,100,ui.font,UI_PADDING);
                    int x = 97 + ceilf(UI_PADDING/2.0f);
                    int y = 221 - h - 3;
                    DrawDescriptionBox(selected->abilities[2].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);

                }
            }
        if (selected->numAbilities >= 4)
            if (DrawAbilityPortraits(selected,heldAbility,3,GetAbilityPortraitRect(3,selected->numAbilities),IsBindDown(keyState,currSettings.keymap.key_R),*mouseState,&baseColor,true,false))
            {
                if (selected->abilities[3].description)
                {
                    int h = GetDescriptionBoxH(selected->abilities[3].description,100,ui.font,UI_PADDING);
                    int x = 129 + ceilf(UI_PADDING/2.0f);
                    int y = 221 - h - 3;
                    DrawDescriptionBox(selected->abilities[3].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
                }
            }
        if (selected->numAbilities >= 5)
        {
            if (DrawAbilityPortraits(selected,heldAbility,4,GetAbilityPortraitRect(4,selected->numAbilities),IsBindDown(keyState,currSettings.keymap.key_F),*mouseState,&baseColor,true,false))
            {
                if (selected->abilities[4].description)
                {
                    int h = GetDescriptionBoxH(selected->abilities[4].description,100,ui.font,UI_PADDING);
                    int x = 140 + ceilf(UI_PADDING/2.0f);
                    int y = 221 - h - 3;
                    DrawDescriptionBox(selected->abilities[4].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
                }
            }
        }


        if (mousedOver && EffectIsEnabled(mousedOver))
        {
            size_t buffsize;
            char null = '\0';
            char* name = mousedOver->name != NULL? mousedOver->name : &null;
            char* description = mousedOver->description != NULL ? mousedOver->description : &null;
            buffsize = snprintf(NULL,0,"%s\n\n%s",name,description);
            char* concatted = calloc(buffsize+1,sizeof(char));
            sprintf(concatted,"%s\n\n%s",name,description);
            DrawDescriptionBox(concatted,2,ui.font,ui.boldFont,16,170,224,41,ENEMY,true);
            free(concatted);

        }
    }
    if (chatboxes)
    {
        DrawUIChatbox();
    }
        DrawSprite(s,1,UI_START_Y+1,0.5f,0.5f,0,baseColor,false,false,false);
    if (selected)
        DrawSprite(health,1,UI_START_Y+1,0.5f,0.5f,0,baseColor,false,false,false);



}
void DrawAllLevelSelects(MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{

    for (int i = 0; i < numEncounters; i++)
    {
        int offset =  _SCREEN_SIZE*i - encounterOffset;//i*_SCREEN_SIZE - selectedEncounterIndex*_SCREEN_SIZE;
        DrawLevelSelect(mouseState,mouseStateLastFrame,i,offset,keyStateThisFrame,keyStateLastFrame);
    }

    Encounter* e = encounters[selectedEncounterIndex];

    if (e->encounter_ButtonLeft.enabled && IsBindDownThisFrame(keyStateThisFrame,keyStateLastFrame,currSettings.keymap.key_PanLeft))
    {
        int sound = LoadSound("assets/audio/ui_slide.wav");
        Sound* s = &sounds[sound];
        PlaySound(s,1,0,false);

        PreviousEncounter();
        //selectedEncounterIndex--;
        if (selectedEncounterIndex < 0)
            selectedEncounterIndex = 0;
    }

    if (e->encounter_ButtonRight.enabled && IsBindDownThisFrame(keyStateThisFrame,keyStateLastFrame,currSettings.keymap.key_PanRight))
    {
        //selectedEncounterIndex++;
        int sound = LoadSound("assets/audio/ui_slide.wav");
        Sound* s = &sounds[sound];
        PlaySound(s,1,0,false);

        NextEncounter();
        if (selectedEncounterIndex >= numEncounters)
            selectedEncounterIndex = numEncounters-1;
    }

    if (e->unlocked && IsBindDownThisFrame(keyStateThisFrame,keyStateLastFrame,currSettings.keymap.key_Center))
    {
        SetGameStateToChoosingParty();
    }
}
void DrawLevelSelect(MouseState* mouseState, MouseState* mouseStateLastFrame, int index, int offsetX, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    Encounter* e = encounters[index];
    
    bool shouldHighlight = false;
    for (int i = selectedEncounterIndex+1; i < numEncounters; i++)
    {
        if (encounters[i]->bestChest == 0 && encounters[i]->unlocked)
            shouldHighlight = true;
    }
    e->encounter_ButtonRight.isHighlighted = shouldHighlight;

    ALLEGRO_COLOR disabled = GROUND_DARK;

    char* augFmt = "Augment %i";
    int numCharsAug = snprintf(NULL,0,augFmt,e->augment);

    char* augmentStr = calloc(numCharsAug+1,sizeof(char));
    sprintf(augmentStr,augFmt,e->augment);

    al_draw_text(ui.font,e->unlocked ? FRIENDLY : disabled,14+offsetX,187,0,augmentStr);

    int amtDamagePercent = GetAugmentDamageBonus(100,e->augment);
    char* dmgString = calloc(1 + NumDigits(amtDamagePercent) + 1 +strlen("Damage ") + 1,sizeof(char));
    sprintf(dmgString,"+%i%% Damage",amtDamagePercent);
    al_draw_text(ui.tinyFont,e->unlocked ? FRIENDLY : disabled,100+offsetX,187,0,dmgString);
    free(dmgString);


    int amtHealthPercent = GetAugmentHealthBonus(100,e->augment);
    char* healthString = calloc(1 + NumDigits(amtHealthPercent) + 1 +strlen("HP ") + 1,sizeof(char));
    sprintf(healthString,"+%i%% HP",amtHealthPercent);
    al_draw_text(ui.tinyFont,e->unlocked ? FRIENDLY : disabled,100+offsetX,203,0,healthString);
    free(healthString);

    int amtAbilityPercent = GetAugmentAbilityDamage(100,e->augment);
    char* abilityDmg = calloc(1 + NumDigits(amtAbilityPercent) + 1 +strlen("Ability Damage ") + 1,sizeof(char));
    sprintf(abilityDmg,"+%i%% Ability Damage",amtAbilityPercent);
    al_draw_text(ui.tinyFont,e->unlocked ? FRIENDLY : disabled,169+offsetX,187,0,abilityDmg);
    free(abilityDmg);

    int amtGoldPercent = GetAugmentGoldBonus(100,e->augment);
    char* goldBonus = calloc(snprintf(NULL,0,GOLD_NUMBER_FMT,amtGoldPercent)+1,sizeof(char));
    sprintf(goldBonus,GOLD_NUMBER_FMT,amtGoldPercent);
    al_draw_text(ui.tinyFont,e->unlocked ? FRIENDLY : disabled,169+offsetX,203,0,goldBonus);
    free(goldBonus);

    #ifdef _AUGMENTS_ENABLED
    int row = 157;
    int column = 95;

    //UpdateButton(column+offsetX,row,e->encounter_RerollAugments.w,e->encounter_RerollAugments.h,&e->encounter_RerollAugments,*mouseState,*mouseStateLastFrame);
    //DrawUIElement(&e->encounter_RerollAugments,column+offsetX,row,mouseState,e->encounter_RerollAugments.bgColor,e->unlocked ? COLOR_FRIENDLY : ALColorToCol(disabled),false);
    column += 32;

    char* augmentDescriptionToDraw = NULL;
    bool isRerolling = GetButtonIsClicked(&e->encounter_RerollAugments);
    for (int i = 0; i < MAX_AUGMENTS; i++)
    {
        Augment* a = &e->augments[i];
        char* description = GetAugmentDescription(a->augment);
        ALLEGRO_COLOR col = *GetAugmentDescriptionColor(a);

        if (!e->unlocked)   
            col = disabled;

        if (DrawAugmentPortrait(a,column+offsetX,row, mouseState, col))
            augmentDescriptionToDraw = description;
        Rect r = (Rect){column+offsetX,row,20,20};
        if (PointInRect(mouseState->screenX,mouseState->screenY,r) && !PointInRect(mouseStateLastFrame->screenX,mouseStateLastFrame->screenY,r))
        {
            PlaySoundStr("assets/audio/hover2.wav",1,0,false);
        }


        if (isRerolling)
        {
            ScatterEffect_Sprite(&sprites[e->augments[i].portrait_sprite_index],column,row,ALColorToCol(col));
        }

        //al_draw_text(ui.tinyFont,*color,column+offsetX,row,0,description);
        column += 24;
        //column += al_get_text_width(ui.tinyFont,description)+5;
        if (i+1 == MAX_AUGMENTS/2)
        {
          //  row += al_get_font_line_height(ui.tinyFont)+2;
            //column = 16;
        }
    }
    if (isRerolling)
    {
        PlaySoundStr("assets/audio/reroll.wav",1,0,false);
        SetEncounterRandAugments(e);
    }
    #endif




    float augmentX = 14;
    if (e->augment <= 0)
        e->augment = 0; 
    e->difficultyUnlocked = _MIN(e->difficultyUnlocked,MAX_DIFFICULTY_LEVELS);
    for (int i = 0; i < e->difficultyUnlocked; i++)
    {
        Rect drawRect = (Rect){augmentX+offsetX,203 ,GetWidthSprite(&sprites[ui.augmentIconIndex]),GetHeightSprite(&sprites[ui.augmentIconIndex])};
        ALLEGRO_COLOR c = i <= e->augment ? FRIENDLY : GROUND;
        if (PointInRect(mouseState->screenX,mouseState->screenY,drawRect) && !PointInRect(mouseStateLastFrame->screenX,mouseStateLastFrame->screenY,drawRect))
        {
            PlaySoundStr("assets/audio/hover.wav",0.25f,0,false);
        }
        if (PointInRect(mouseState->screenX,mouseState->screenY,drawRect))
            c = ENEMY;
        
        DrawSprite(&sprites[ui.augmentIconIndex],drawRect.x,drawRect.y,0.5f,0.5f,0,c,false,false,false);
        augmentX += drawRect.w+2;

        if (mouseStateLastFrame->mouse.buttons & 1 && !(mouseState->mouse.buttons & 1))
        {
            Rect selectRect = drawRect;
            selectRect.x -= 2;
            selectRect.y -= 2;
            selectRect.w += 5;
            selectRect.h += 5;

        
            if (PointInRect(mouseState->screenX,mouseState->screenY,drawRect) && i != e->augment)
            {
                PlaySoundStr("assets/audio/gem_click.wav",1,0,false);
                e->augment = i;
                #ifdef _AUGMENTS_ENABLED
                SetEncounterRandAugments(e);
                #endif
            }
        }
    }
    e->encounter_PurchaseAugment.x = 14 + offsetX;
    e->encounter_PurchaseAugment.y =  215;
    int purchaseCost = GetAugmentCost(e, e->difficultyUnlocked);
    if (e->bestChest >= e->difficultyUnlocked && e->difficultyUnlocked < MAX_DIFFICULTY_LEVELS)
    {
        e->encounter_PurchaseAugment.isHighlighted = true;
        char* buttonText = calloc(NumDigits(purchaseCost)+1,sizeof(char));
        sprintf(buttonText,"%i",purchaseCost);
        ChangeButtonText((Button*)(e->encounter_PurchaseAugment.data), buttonText);
        free(buttonText);
    }
    else
    {
        ChangeButtonText((Button*)(e->encounter_PurchaseAugment.data), e->difficultyUnlocked == MAX_DIFFICULTY_LEVELS ? "Max" : "Locked" );
    }
    if (purchaseCost > players[0].bankedGold || e->bestChest <= 0 || e->difficultyUnlocked > e->bestChest || e->difficultyUnlocked == MAX_DIFFICULTY_LEVELS)
    {
        e->encounter_PurchaseAugment.isHighlighted = false;
        e->encounter_PurchaseAugment.enabled = false;
    }
    else
    {
        e->encounter_PurchaseAugment.enabled = true;
    }
    UpdateButton(e->encounter_PurchaseAugment.x,e->encounter_PurchaseAugment.y,e->encounter_PurchaseAugment.w,e->encounter_PurchaseAugment.h,&e->encounter_PurchaseAugment,*mouseState,*mouseStateLastFrame);
    DrawUIElement(&e->encounter_PurchaseAugment,e->encounter_PurchaseAugment.x,e->encounter_PurchaseAugment.y,mouseState,COLOR_BG,e->unlocked ? COLOR_FRIENDLY : ALColorToCol(disabled),false);

    if (GetButtonIsClicked(&e->encounter_PurchaseAugment))
    {
        AddGold(-purchaseCost);
        players[0].bankedGold -= purchaseCost;
        e->difficultyUnlocked++;
        e->encounter_PurchaseAugment.enabled = false;
        Save("_save.save");

    }


    free(augmentStr);
    #ifdef _AUGMENTS_ENABLED
    //al_draw_line(10+offsetX,73,246,73,e->unlocked ? FRIENDLY : disabled,1);
    #else
    //al_draw_line(10+offsetX,64,246,64,e->unlocked ? FRIENDLY : disabled,1);
    #endif

    ALLEGRO_COLOR col = e->unlocked ? FRIENDLY : disabled;

    al_draw_text(ui.font,col,(_SCREEN_SIZE/2.0f)+offsetX,10,ALLEGRO_ALIGN_CENTRE,e->name ? e->name : "");

    Text te = (Text){.f = ui.font,ui.boldFont,.x=10+offsetX,.y=25,.color=col,.lineHeight=al_get_font_line_height(ui.font)};
    al_do_multiline_text(ui.font,246,e->description,cb,&te);

    //al_draw_text(ui.font,col,(_SCREEN_SIZE/2.0f)+offsetX,25,ALLEGRO_ALIGN_CENTRE,e->description ? e->description : "");
    /*
    DrawSprite(&sprites[e->spriteIndex],17+offsetX,102,0.5f,0.5f,0,e->unlocked ? ENEMY : disabled,false,false,false);

    Ability* mousedOver = NULL;
    if (e->unlocked)
    {
        mousedOver = DrawAbility(&e->abilities[0], 96+offsetX, 80, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[0] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[1], 136+offsetX, 80, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[1] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[2], 175+offsetX, 80, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[2] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[3], 214+offsetX, 80, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[3] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[4], 96+offsetX, 120, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[4] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[5], 136+offsetX, 120, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[5] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[6], 175+offsetX, 120, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[6] : mousedOver;
        mousedOver = DrawAbility(&e->abilities[7], 214+offsetX, 120, ENEMY, mouseState,mouseStateLastFrame) == true ? &e->abilities[7] : mousedOver;
    }
    else if (!e->unlocked)
    {
        DrawAbility(&e->abilities[0], 96+offsetX, 80, disabled, NULL,NULL);
        DrawAbility(&e->abilities[1], 136+offsetX, 80, disabled, NULL,NULL);
        DrawAbility(&e->abilities[2], 175+offsetX, 80, disabled, NULL,NULL);
        DrawAbility(&e->abilities[3], 214+offsetX, 80, disabled, NULL,NULL);
        DrawAbility(&e->abilities[4], 96+offsetX, 120, disabled, NULL,NULL);
        DrawAbility(&e->abilities[5], 136+offsetX, 120, disabled, NULL,NULL);
        DrawAbility(&e->abilities[6], 175+offsetX, 120, disabled, NULL,NULL);
        DrawAbility(&e->abilities[7], 214+offsetX, 120, disabled, NULL,NULL);
    }

    char* descriptionToDraw;
    if (mousedOver)
    {
        descriptionToDraw = mousedOver->description;
    }
    else
    {
        descriptionToDraw = e->description;
    }
    #ifdef _AUGMENTS_ENABLED
    if (augmentDescriptionToDraw)
        descriptionToDraw = augmentDescriptionToDraw;
    #endif
    if (descriptionToDraw)
    {
        DrawDescriptionBox(descriptionToDraw,2,ui.font,ui.boldFont,16+offsetX,170,224,41,e->unlocked ? ENEMY : disabled,true);

    }
    ui.panelShownPercent=1.0f;
    //DrawPanel(&ui.encounter_scroll,mouseState);
*/
    DrawSprite(&sprites[e->selectScreenSprite],offsetX,56,0,0,0,FRIENDLY,false,false,false);


    UpdateButton(16+offsetX,238,e->encounter_ButtonLeft.w,e->encounter_ButtonLeft.h,&e->encounter_ButtonLeft,*mouseState,*mouseStateLastFrame);
    UpdateButton(80+offsetX,238,e->encounter_ButtonConfirm.w,e->encounter_ButtonConfirm.h,&e->encounter_ButtonConfirm,*mouseState,*mouseStateLastFrame);
    UpdateButton(192+offsetX,238,e->encounter_ButtonRight.w,e->encounter_ButtonRight.h,&e->encounter_ButtonRight,*mouseState,*mouseStateLastFrame);

    if (index > 0 && (index > 0 && (!encounters[index-1]->encounterShouldBeSkipped)))
    {
        e->encounter_ButtonLeft.enabled = true;
    }
    else {
        e->encounter_ButtonLeft.enabled = false;
    }
    if (index+1 < numEncounters)
    {
        e->encounter_ButtonRight.enabled = true;
        //if (encounters[index+1]->encounterShouldBeSkipped)
          //  e->encounter_ButtonRight.enabled = false;
    }
    else {
        e->encounter_ButtonRight.enabled = false;
    }

    DrawUIElement(&e->encounter_ButtonLeft,14+offsetX,238,mouseState,e->encounter_ButtonLeft.bgColor,COLOR_FRIENDLY,false);
    DrawUIElement(&e->encounter_ButtonConfirm,80+offsetX,238,mouseState,e->encounter_ButtonConfirm.bgColor,COLOR_FRIENDLY,false);
    DrawUIElement(&e->encounter_ButtonRight,194+offsetX,238,mouseState,e->encounter_ButtonRight.bgColor,COLOR_FRIENDLY,false);


    if (GetButtonIsClicked(&e->encounter_ButtonLeft))
    {
        PreviousEncounter();
        //selectedEncounterIndex--;
        if (selectedEncounterIndex < 0)
            selectedEncounterIndex = 0;
    }
    if (GetButtonIsClicked(&e->encounter_ButtonConfirm))
    {
        //gameState = GAMESTATE_CHOOSING_UNITS;
        SetGameStateToChoosingParty();
    }
    if (GetButtonIsClicked(&e->encounter_ButtonRight))
    {
        //selectedEncounterIndex++;

        NextEncounter();
        if (selectedEncounterIndex >= numEncounters)
            selectedEncounterIndex = numEncounters-1;
    }
    if (!e->unlocked)
    {
        DrawSprite(LoadSprite_Pointer("assets/ui/levelselect/padlock.png",false),offsetX,0,0,0,0,FRIENDLY,false,false,false);
        Rect r = (Rect){72,66,112,143};
        if (selectedEncounterIndex == index && PointInRect(mouseState->screenX,mouseState->screenY,r) && mouseState->mouse.buttons & 1 && !(mouseStateLastFrame->mouse.buttons & 1))
        {
            AddScreenshake(3,0.2);
        }
        e->encounter_ButtonConfirm.enabled = false;
        e->encounter_RerollAugments.enabled = false;
        e->encounter_PurchaseAugment.enabled = false;
    }   
    else
    {
        e->encounter_ButtonConfirm.enabled = true;

    }



    DrawGoldCount(FRIENDLY,ENEMY,9,9);

}
UIElement* AddElement(Panel* p, UIElement* u)
{
    if (!p->elements)
    {
        p->elements = calloc(1,sizeof(UIElement));
        p->numElements=0;
        p->numElementsAllocated=1;
    }   
    if (p->numElements >= p->numElementsAllocated)
    {
        p->elements = realloc(p->elements,(p->numElementsAllocated+1)*sizeof(UIElement));
        p->numElements = p->numElementsAllocated;
        p->numElementsAllocated++;
    }
    p->elements[p->numElements] = *u;
    p->numElements++;

    return &p->elements[p->numElements-1];
}
void AddKeyInput(Panel* p, char* name, char* description, int x, int y, int w, int h, int maxchars, KEY* mapTo)
{
    KeyInput* t = calloc(1,sizeof(KeyInput));
    t->allowsInteraction = true;
    t->text = calloc(2,sizeof(char));
    t->mappedTo = (int*)mapTo;

    UIElement u = {0};
    u.data = (void*)t;
    u.font = &ui.tinyFont;
    u.w = w;    
    u.h = h;
    u.y = y;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.elementType = ELEMENT_KEYINPUT;
    u.x = x;
    u.enabled = true;
    u.sound_clickDown_Index = ui.uiClickedSound_Index;
    u.sound_clickUp_Index = ui.uiClickedUpSound_Index;
    u.bgColor = COLOR_BG;
    AddElement(p,&u);
    UpdateBind(&u);

}
void ClearTextInputStr(UIElement* u)
{
    TextInput* t = (TextInput*)u->data;
    memset(t->text,0,sizeof(char)*t->maxChars);
}   
void SetTextInputStr(UIElement* u, char* str)
{
    TextInput* t = (TextInput*)u->data;
    strncpy(t->text,str,t->maxChars);
}
UIElement* AddTextInput(Panel* p, int x, int y, int w, int h,char* name, char* description, int maxChars, bool onlyAllowNumbers)
{
    TextInput* t = calloc(1,sizeof(TextInput));
    t->allowsInteraction = true;
    t->text = calloc(maxChars+1,sizeof(char));
    t->maxChars = maxChars;
    t->onlyAllowNumbers = onlyAllowNumbers;
    
    UIElement u = {0};
    u.data = (void*)t;
    u.font = &ui.tinyFont;
    u.w = w;    
    u.h = h;
    u.y = y;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.elementType = ELEMENT_TEXTINPUT;
    u.x = x;
    u.enabled = true;
    u.sound_clickDown_Index = ui.uiClickedSound_Index;
    u.sound_clickUp_Index = ui.uiClickedUpSound_Index;
    u.bgColor = COLOR_BG;

    UIElement* ref = AddElement(p,&u);
    return ref;
}

UIElement* AddButton(Panel* p, char* name, char* description, int x, int y, int w, int h, bool shouldDrawLine)
{
    Button* b = calloc(1,sizeof(Button));
    b->description = calloc(strlen(description)+1,sizeof(char));
    strcpy(b->description,description);
    b->clicked = false; 
    b->drawLine = shouldDrawLine;
    UIElement u = {0};
    u.data = (void*)b;
    u.w = w;
    u.h = h;
    u.y = y;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.elementType = ELEMENT_BUTTON;
    u.x = x;
    u.enabled = true;
    u.sound_clickDown_Index = ui.uiClickedSound_Index;
    u.sound_clickUp_Index = ui.uiClickedUpSound_Index;
    u.bgColor = COLOR_BG;

    UIElement* ref = AddElement(p,&u);
    return ref;
}
void UpdateSlider(Slider* s, int x, int y, int w, int h, MouseState mouseState, MouseState mouseStateLastFrame)
{
   // ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);
    Rect r = (Rect){x,y,w,h};
    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            bool* b = &s->clicked;
            s->clicked = true;  
        }
    }
    if (!(mouseState.mouse.buttons & 1) && (mouseStateLastFrame.mouse.buttons & 1) && s->clicked)
    {
        s->clicked = false;
    }

    if (s->clicked)
    {

        float v = 1-((x+w) - mouseState.screenX) / (float)w;
        if (v>1)
            v = 1;
        if (v<0)
            v = 0;
        *s->value = v;
    }
}
Panel CreatePanel(int x, int y, int w, int h, int padding, bool showBorder)
{
    Panel p = {0};
    p.x = x;
    p.y = y;
    p.w = w;
    p.h = h;
    p.padding = padding;
    p.showBorder = showBorder;
    return p;
}
void TabGroup(int numPanels, ...)
{
    va_list argp;
    va_start(argp, numPanels);

    Panel** list = malloc(numPanels*sizeof(Panel*));
    

    for (int i = 0; i < numPanels; i++)
    {
        Panel* tab = va_arg(argp, Panel*);
        list[i] = tab;
    }
    for (int i = 0; i < numPanels; i++)
    {
        if (list[i]->tabs)
        {
            free(list[i]->tabs);
        }
        list[i]->tabs = malloc(numPanels*sizeof(Panel*));
        list[i]->numTabs = numPanels;
        if (!list[i]->tabButton.enabled)
            InitButton(&list[i]->tabButton, "Tab", "", 0,0, 14, 33, 0);

        for (int j = 0; j < numPanels; j++)
        {
            list[i]->tabs[j] = list[j];
        }
    }
    free(list);
    va_end(argp);
}
UIElement* InitButton(UIElement* u, char* name, char* description, int x, int y, int w, int h, int sprite)
{
    Button* b = calloc(1,sizeof(Button));

    b->description = calloc(strlen(description)+1,sizeof(char));
    strcpy(b->description,description);
    b->drawLine = true;
    b->clicked = false; 
    b->spriteIndex = sprite;
    u->data = (void*)b;
    u->w = w;
    u->h = h;
    u->name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u->name,name);
    u->elementType = ELEMENT_BUTTON;
    u->x = x;
    u->y = y;
    u->sound_clickDown_Index = ui.uiClickedSound_Index;
    u->sound_clickUp_Index = ui.uiClickedUpSound_Index;
    u->enabled = true;
    u->bgColor = COLOR_BG;

    return u;

}
void ChangeButtonText(Button* b, char* newstr)
{
    if (!b)
    {
        return;
    }
    if (b->description)
    {
        free(b->description);
    }
    b->description = calloc(strlen(newstr)+1,sizeof(char));
    strcpy(b->description,newstr);
}
void AddText(Panel* p, int x, int y, char* name, char* description, ALLEGRO_COLOR* colour, ALLEGRO_FONT** font)
{
    UIElement u = {0};
    UI_Text* t = calloc(1,sizeof(UI_Text));

    t->font = font;
    if (!font)
        t->font = &ui.font;

    t->str = calloc(strlen(description)+1,sizeof(char));
    strcpy(t->str,description);
    t->colour = colour; 
    u.data = (void*)t;

    u.x = x; 
    u.y = y;
    u.elementType = ELEMENT_TEXT;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    AddElement(p,&u);
}
KeyInput* GetKeyInput(Panel* p, char* name)
{
    if (!p) 
        return NULL;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            KeyInput* b = (KeyInput*)u->data;
            return b;
        }
    }
    return NULL;
}

Button* GetButtonB(Panel* p, char* name)
{
    if (!p) 
        return NULL;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            Button* b = (Button*)u->data;
            return b;
        }
    }
    return NULL;
}
void DrawSlider(UIElement* u, int x, int y, MouseState* mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    Slider* s = (Slider*)u->data;
    float w = u->w * *s->value;

    float r = s->r ? *s->r : 0;
    float g = s->g ? *s->g : 0;
    float b = s->b ? *s->b : 0;
    if (!r && !g && !b)
    {
        r = FRIENDLY.r;
        g = FRIENDLY.g;
        b = FRIENDLY.b;
    }
    ALLEGRO_COLOR c = al_map_rgb_f(r,g,b);
    DrawFilledRectangle(x,y,x+w,y+u->h,c);
    DrawRectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
}
void UpdateCheckbox(Checkbox* c, int x, int y, int w, int h, bool isActive, MouseState mouseState, MouseState mouseStateLastFrame)
{
    if (!isActive)
        return;
        
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);

    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        Rect r = (Rect){x,y,w,h};
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            c->clicked = true;
        }
    }
    if (mouseStateLastFrame.mouse.buttons & 1 && !(mouseState.mouse.buttons & 1))
    {
        Rect r = (Rect){x,y,w,h};
        if (c->clicked)
        {
            if (PointInRect(mouseState.screenX,mouseState.screenY,r))
            {
                *c->activated = !(*c->activated);
            }
        }
    }
    if (!(mouseState.mouse.buttons & 1))
    {
        c->clicked = false;
    }

}
void DrawCheckbox(Checkbox* c, int x, int y, int w, int h, bool isActive,MouseState mouseState, ALLEGRO_COLOR bgColor)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    Rect checkbox = (Rect){x,y,w,h};
    if (*c->activated && isActive)
    {
        DrawFilledRectangle(x,y,x+w,y+h,FRIENDLY);
        DrawRectangle(x,y,x+w,y+h,BG,1);
    }
    else
    {
        DrawFilledRectangle(x,y,x+w,y+h,bgColor);
        if (isActive)
        {
            DrawRectangle(x,y,x+w,y+h,FRIENDLY,1);
        }
        else
        {
            DrawOutlinedRect_Dithered(checkbox,FRIENDLY);
        }
    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,checkbox))
    {
        Color col = *c->activated ? COLOR_BG : COLOR_FRIENDLY;
        DrawRectangle(x+2,y+2,x+w-2,y+h-2,GetColor(col,0),1);
    }
}
UIElement* AddCheckbox(Panel* p, int x, int y, int w, int h, char* name, bool* activated)
{
    Checkbox* c = calloc(1,sizeof(Checkbox));
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    c->activated = activated;
    u.name = name;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.data = (void*)c;
    u.enabled = true;
    u.elementType = ELEMENT_CHECKBOX;
    u.enabled = true;
    u.bgColor = COLOR_BG;
    
    UIElement* ref = AddElement(p,&u);

    return ref;
}
Slider* InitSlider(int x, int y, int w, int h, float filled, float* v, float* r, float* g, float* b)
{
    Slider* s = calloc(1,sizeof(Slider));
    s->value = v;
    *v = filled;
    s->r = r;
    s->g = g;
    s->b = b;

    return s;
}

UIElement* AddSlider(Panel* p, int x, int y, int w, int h, char* name, float filled, float* v)
{
    Slider* s = InitSlider(x,y,w,h,filled,v,&FRIENDLY.r,&FRIENDLY.g,&FRIENDLY.b);
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    u.enabled = true;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.data = (void*)s;
    u.elementType = ELEMENT_SLIDER;
    u.bgColor = COLOR_BG;

    UIElement* ref = AddElement(p,&u);
    return ref;
}
UIElement* AddPulldownMenu(Panel* panel, int x, int y, int w, int h, char* name, int startIndex, void* callback, int numElements, ...)
{
    va_list argp;
    va_start(argp, numElements);
    char** list = calloc(numElements,sizeof(char*)); 
    for (int i = 0; i < numElements; i++)
    {
        char* element = va_arg(argp, char*);
        char* new = calloc(strlen(element)+1,sizeof(char));
        strcpy(new,element);
        list[i] = new;
    }
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    u.enabled = true;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);

    
    Pulldown* p = calloc(1,sizeof(Pulldown));
    p->callback = callback;
    p->clicked = false;
    p->elements = list;
    p->selectedIndex = startIndex;
    p->numElements = numElements;

    u.data = (void*)p;
    u.elementType = ELEMENT_PULLDOWN;
    u.bgColor = COLOR_BG;

    UIElement* ref = AddElement(panel,&u);

    return ref;

}
bool UpdatePulldownMenu(Pulldown* p, int x, int y, int w, int h, MouseState mouseState, MouseState mouseStateLastFrame)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);

    if (p->clicked)
    {
        if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
        {
            int y2 = y;
            for (int i = 0; i < p->numElements; i++)
            {
                Rect r = (Rect){x,y2,w,h};
                if (PointInRect(mouseState.screenX,mouseState.screenY,r))
                {
                    p->clicked = false;
                    if (i != p->selectedIndex && p->callback) 
                        p->callback(p,i);
                    p->selectedIndex = i;
                    return true;
                }
                y2 += h;
            }

        }
    } 

    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        Rect r = (Rect){x,y,w,h};
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            p->clicked = true;
        }
        else
        {
            p->clicked = false; 
        }
    }
    return false;
}
void DrawPullDownMenu(Pulldown* p, int x, int y, int w, int h, bool isActive, MouseState* mouseState, ALLEGRO_COLOR bgColor)
{
    Rect clip;
    int clipX; int clipY;
    al_get_clipping_rectangle(&clipX,&clipY,&clip.w,&clip.h);
    clip.x = clipX;
    clip.y = clipY;

    Rect r = (Rect){x,y,w,h};
    if (p->clicked && isActive)
    {
        int y2 = y;
        al_reset_clipping_rectangle();
        for (int i = 0; i < p->numElements; i++)
        {
            DrawFilledRectangle(x,y2,x+w,y2+h,bgColor);
            DrawRectangle(x,y2,x+w,y2+h,FRIENDLY,1);
            al_draw_text(ui.font,FRIENDLY,x+w/2,y2+h/2 - al_get_font_line_height(ui.font)/2.0,ALLEGRO_ALIGN_CENTRE,p->elements[i]);
            Rect r2 = (Rect){x,y2,w,h};
            if (PointInRect(mouseState->screenX,mouseState->screenY,r2))
            {
                DrawRectangle(x+2,y2+2,x+w-2,y2+h-2,FRIENDLY,1);
            }
            y2 += h;

        }
        //al_draw_text(ui.font,BG,x+w/2,y+h/2 - al_get_font_line_height(font)/2.0,ALLEGRO_ALIGN_CENTRE,b->description);
    }
    if (!p->clicked)
    {
        DrawFilledRectangle(x,y,x+w,y+h,bgColor);
        DrawRectangle(x,y,x+w,y+h,FRIENDLY,1);
        al_draw_text(ui.font,FRIENDLY,x+w/2,y+h/2 - al_get_font_line_height(ui.font)/2.0,ALLEGRO_ALIGN_CENTRE,p->elements[p->selectedIndex]);
        if (PointInRect(mouseState->screenX,mouseState->screenY,r) && isActive)
        {
            DrawRectangle(x+2,y+2,x+w-2,y+h-2,FRIENDLY,1);
        }

    }
    al_set_clipping_rectangle(clip.x,clip.y,clip.w,clip.h);
}
void DrawScrollbar(Panel* p)
{
    int w = SCROLLBARW;
    DrawRectangle(p->x+p->w-w,p->y,p->x+p->w,p->x+p->w,FRIENDLY,1);


    float yPos = p->y + p->scrollPercent * p->h;
    DrawFilledRectangle(p->x+p->w-w, yPos, p->x+p->w, yPos+14, FRIENDLY);

}
void UpdateScrollbar(Panel* p, MouseState* mouseState,MouseState* mouseStateLastFrame)
{
    int w = SCROLLBARW;
    Rect scrollbar = (Rect){p->x+p->w-w,p->y,w,p->h};

    if (PointInRect(mouseState->screenX,mouseState->screenY,(Rect){p->x,p->y,p->w,p->h}))
    {
        if (mouseState->mouse.z != mouseStateLastFrame->mouse.z)
        {
            float v = mouseState->mouse.z - mouseStateLastFrame->mouse.z;
            p->scrollPercent -= v/6.0f;
            p->scrollPercent = clamp(p->scrollPercent,0,1);
        }
    }

    if (mouseState->mouse.buttons & 1 && !(mouseStateLastFrame->mouse.buttons & 1))
    {
        if (PointInRect(mouseState->screenX,mouseState->screenY,scrollbar))
        {
            p->scrollbarClicked = true;
        }
    }
    if (!(mouseState->mouse.buttons & 1))
    {
            p->scrollbarClicked = false;
    }
    if (p->scrollbarClicked)
    {
        float v = 1-((p->y+p->h) - mouseState->screenY) / (float)p->h;
        if (v>1)
            v = 1;
        if (v<0)
            v = 0;

        p->scrollPercent = v;
    }
}
void SetUITextStr(UI_Text* t, char* str)
{
    if (t->str)
        free(t->str);
    t->str = calloc(strlen(str)+1,sizeof(char));
    strcpy(t->str,str);
}
UI_Text* GetUIText(Panel* p, char* name)
{
    if (!p)
        return NULL;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            UI_Text* t = (UI_Text*)u->data;
            return t;
        }
    }
    return NULL;
}
void UpdateLanternWidget(Widget* self, float dt)
{
    self->timer += dt;
    self->rotation = sinf(self->timer) * 0.1f;
}
void UpdateHandWidget(Widget* self, float dt)
{
    self->timer += dt;
    self->velocity = 0;
    self->y += sinf(self->timer) * 0.1f;
}

UIElement* GetUIElement(Panel* p, char* name)
{
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        if (strcasecmp(u->name,name)==0)
        {
            return u;
        }
    }
    return NULL;
}
void DeleteUIElement(UIElement* u)
{
    if (u->elementType == ELEMENT_BUTTON)
    {
        Button* b = (Button*)u->data;
        if (b->description) 
            free(b->description);
        b->description = NULL;
    }
    if (u->elementType == ELEMENT_SLIDER)
    {
    }
    if (u->elementType == ELEMENT_CHECKBOX)
    {
        
    }
    if (u->elementType == ELEMENT_TEXT)
    {
        
    }
    if (u->elementType == ELEMENT_PULLDOWN)
    {
        Pulldown* p = (Pulldown*)u->data;
        if (p->elements)
        {
            for (int i = 0; i < p->numElements; i++)
            {
                free(p->elements[i]);
            }
            free(p->elements);
        }
    }
    if (u->elementType == ELEMENT_KEYINPUT)
    {
        KeyInput* t = (KeyInput*)u->data;
        if (t->text)
            free(t->text);
    }
    if (u->elementType == ELEMENT_TEXTINPUT)
    {
        TextInput* t = (TextInput*)u->data;
        if (t->text)
            free(t->text);
    }

    if (u->name)
        free(u->name);
    if (u->data)
        free(u->data);

    u->name = NULL;
    u->data = NULL;

}
void ClearPanelElements(Panel* p)
{
    for (int i = 0; i < p->numElements; i++)
    {
        DeleteUIElement(&p->elements[i]);
    }
    free(p->elements);
    p->elements = NULL;
    p->numElements = 0;
    p->numElementsAllocated = 0;
}
void GenerateFileListButtons(char* path, Panel* p)
{
      DIR *d;
    struct dirent *dir;
    d = opendir(path);
    int yPos = 0;
    int xPos = 16;
    int w = 96;
    int h = 8;
    int padding = 2;

    int index = 0;

    if (d)
    {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".rep") >= 1)
            {
                char* name = calloc(strlen(dir->d_name)+1,sizeof(char));
                char* description = calloc(strlen(dir->d_name)+1,sizeof(char));
                int numChars = strlen(dir->d_name) <= 14 ? strlen(dir->d_name) : 14;
                strcpy(name,dir->d_name);
                memcpy(description,dir->d_name,numChars);

                AddButton(p,name,description,xPos,yPos+(index*(padding+h)),w,h,true);

                index++;
                free(name);
                free(description);
            }
        }
        closedir(d);
    }
}
void ChangeButtonImage(UIElement* u, int spriteIndex)
{
    if (u->elementType == ELEMENT_BUTTON)
    {
        Button* b = (Button*)u->data;
        b->spriteIndex = spriteIndex;
    }
}
int padding = 8;
void InitPurchaseScreenUI()
{
    InitButton(&ui.purchasingUnitUI.returnButton, "Return", "", 8,0, 14,14, LoadSprite("assets/ui/back_tab_icon.png", true));
    
    InitButton(&ui.purchasingUnitUI.back, "Back", "Back", 8,240, 30,11, 0);
    InitButton(&ui.purchasingUnitUI.next, "Next", "Next", 104,240, 30,11, 0);
    InitButton(&ui.purchasingUnitUI.purchaseButton, "Purchase", "Purchase", 43,240, 56,11,0);

}
void InitLoadReplayPanel()
{
    ui.loadReplayPanel = CreatePanel(29,80,160,144,15,true);
    InitButton(&ui.loadReplayPanel.backButton, "Back", "", 0,0, 14,14, LoadSprite("assets/ui/back_tab_icon.png", true));
    ui.loadReplayPanel.back = &ui.mainMenuPanel;
    InitButton(&replayPlayButton,"Play","",225,5,10,9,0);
    ChangeButtonImage(&replayPlayButton,LoadSprite("assets/ui/button_pause.png",true));
    InitButton(&replayBackButton,"","",8,5,9,9,0);
    ChangeButtonImage(&replayBackButton,LoadSprite("assets/ui/replay_back.png",true));

}
void InitMainMenuPanel()
{
    int y = 0;
    int yMove = 16 + padding;
    ui.mainMenuPanel = CreatePanel(29,70,160,144,15,false);
    AddButton(&ui.mainMenuPanel,"Continue","Continue",0,(y+=yMove),96,16,false);
    AddButton(&ui.mainMenuPanel,"Return","Start Game",0,(y+=yMove),96,16,false);
    AddButton(&ui.mainMenuPanel,"Tutorial","Tutorial",0,(y+=yMove),96,16,false);
    #ifdef _REPLAY
        AddButton(&ui.mainMenuPanel,"Load Replay","Load Replay",0,(y+=yMove),96,16,false);
    #endif
    AddButton(&ui.mainMenuPanel,"Options","Options",0,(y+=yMove),96,16,false);
    AddButton(&ui.mainMenuPanel,"Exit","Exit Game",0,(y+=yMove),96,16,false);

}
void InitPausePanel()
{
    ui.pauseMenuPanel = CreatePanel(48,48,160,114,15,true);
    int move = 32;
    int yMove = -move/2;
    AddButton(&ui.pauseMenuPanel,"Return","Return",33,yMove+=move,96,16,true);
    AddButton(&ui.pauseMenuPanel,"Retry","Retry",33,yMove+=move,96,16,true);
    AddButton(&ui.pauseMenuPanel,"Options","Options",33,yMove+=move,96,16,true);
    AddButton(&ui.pauseMenuPanel,"Exit","Exit Game",33,yMove+=move,96,16,true);
    ui.pauseMenuPanel.h = yMove+move/2.0f+16;
}   
void WindowTypeCallback(Pulldown* pulldown, int selectedIndex)
{
    if (selectedIndex == 0)
    {
        SetDisplayWindowed(ALLEGRO_WINDOWED);
    }
    if (selectedIndex == 1)
    {
        SetDisplayWindowed(ALLEGRO_FULLSCREEN);
    }    
    if (selectedIndex == 2)
    {
        SetDisplayWindowed(ALLEGRO_FULLSCREEN_WINDOW);
    }


}
void InitVideoOptionsPanel()
{

    ui.videoOptionsPanel = CreatePanel(48,48,180,OPTIONS_PANEL_HEIGHT,15,true);

    AddText(&ui.videoOptionsPanel,33,10,"Video_Options", "Video Options",NULL, &ui.boldFont);

    AddText(&ui.videoOptionsPanel,33,41,"Tag_RenderScale","RenderScale", NULL,NULL);
    AddText(&ui.videoOptionsPanel,132,43,"RenderScale","2x", NULL,NULL);
    
    char* renderScale = calloc(NumDigits(_RENDERSIZE)+2,sizeof(char));
    sprintf(renderScale,"%ix",_RENDERSIZE);
    SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,renderScale);
    free(renderScale);
    
    AddButton(&ui.videoOptionsPanel,"RenderScale+","+",132,29,11,11,true);
    AddButton(&ui.videoOptionsPanel,"RenderScale-","-",132,53,11,11,true);
    AddText(&ui.videoOptionsPanel,33,73,"Tag_Particles","Particles", NULL,NULL);
    AddCheckbox(&ui.videoOptionsPanel,131,72,13,13,"EnableParticles",&currSettings.particlesEnabled);
    AddText(&ui.videoOptionsPanel,33,105,"Display\nHealth Bar","Display\nHealth Bar", NULL,NULL);
    AddPulldownMenu(&ui.videoOptionsPanel,97,108,48,13,"HealthBarDisplay",0,NULL,3,"Always","Selected","Never");
    
    AddText(&ui.videoOptionsPanel,33,132,"Display\nLight Effect","Display\nLight Effect", NULL,NULL);
    AddCheckbox(&ui.videoOptionsPanel,132,132,13,13,"EnableLightEffect",&currSettings.lightEffectEnabled);

    AddText(&ui.videoOptionsPanel,33,156,"Display Timer","Display Timer", NULL,NULL);
    AddCheckbox(&ui.videoOptionsPanel,132,156,11,11,"DisplayTimerButton",&currSettings.displayTimer);

    AddText(&ui.videoOptionsPanel,33,180,"Label_TargetFPS","Target FPS", NULL,NULL);
    AddPulldownMenu(&ui.videoOptionsPanel,97,180,48,13,"Target FPS",currSettings.targetFPS,NULL,5,"30","60","90","144","240");

    AddText(&ui.videoOptionsPanel,33,204,"Label_ScreenShakeFactor","Screen Shake", NULL,NULL);
    AddSlider(&ui.videoOptionsPanel,120,204,45,10,"ScreenShake",currSettings.screenShakeFactor,&currSettings.screenShakeFactor);
    
    int windowTypeSelectedIndex = 0;
    if (currSettings.displayWindowStyle == ALLEGRO_WINDOWED)
        windowTypeSelectedIndex = 0;
    if (currSettings.displayWindowStyle == ALLEGRO_FULLSCREEN)
        windowTypeSelectedIndex = 1;
    if (currSettings.displayWindowStyle == ALLEGRO_FULLSCREEN_WINDOW)
        windowTypeSelectedIndex = 2;
    AddText(&ui.videoOptionsPanel,33,228,"Label_WindowType","Window Type", NULL,NULL);
    AddPulldownMenu(&ui.videoOptionsPanel,33,240,120,10,"WindowType",windowTypeSelectedIndex,WindowTypeCallback,3,"Windowed","Fullscreen", "Windowed Fullscreen");


    InitButton(&ui.videoOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));

    InitButton(&ui.videoOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/video_tab_icon.png",true));

    ui.videoOptionsPanel.back = &ui.mainMenuPanel;

    Pulldown* healthbar = (Pulldown*)GetUIElement(&ui.videoOptionsPanel,"HealthBarDisplay")->data;
    currSettings.displayHealthBar = healthbar->selectedIndex;


}
void InitAudioOptionsPanel()
{
    ui.audioOptionsPanel = CreatePanel(48,48,180,OPTIONS_PANEL_HEIGHT,15,true);

    AddText(&ui.audioOptionsPanel,33,10,"Audio_Options", "Audio Options",NULL, &ui.boldFont);


    AddText(&ui.audioOptionsPanel,33,41,"Tag_MasterVolume","Master Volume", NULL,NULL);
    AddSlider(&ui.audioOptionsPanel,34,52,110,10,"MasterVolume",currSettings.masterVolume,&currSettings.masterVolume);
 
    AddText(&ui.audioOptionsPanel,33,90,"Tag_SFXVolume","SFX Volume", NULL,NULL);
    AddSlider(&ui.audioOptionsPanel,34,100,110,10,"SfxVolume",currSettings.sfxVolume,&currSettings.sfxVolume);

    AddText(&ui.audioOptionsPanel,33,131,"Tag_MusicVolume","Music Volume", NULL,NULL);
    AddSlider(&ui.audioOptionsPanel,34,141,110,10,"MusicVolume",currSettings.musicVolume,&currSettings.musicVolume);

    AddText(&ui.audioOptionsPanel,33,172,"Tag_AmbientVolume","Ambience Volume", NULL,NULL);
    AddSlider(&ui.audioOptionsPanel,34,182,110,10,"AmbienceVolume",currSettings.ambienceVolume,&currSettings.ambienceVolume);


    InitButton(&ui.audioOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.audioOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/audio_tab_icon.png",true));
    ui.audioOptionsPanel.back = &ui.mainMenuPanel;

}
void InitAccessibilityOptionsPanel()
{
    ui.accessibilityOptionsPanel = CreatePanel(48,48,180,OPTIONS_PANEL_HEIGHT,15,true);

    AddText(&ui.accessibilityOptionsPanel,33,10,"Accessibility_Options", "Accessibility Options",NULL, &ui.boldFont);

    //AddButton(&ui.audioOptionsPanel,"MasterVolume", "MasterVolume", 132,29,96,16,true);
    //AddButton(&ui.audioOptionsPanel,"Music Volume","Music Volume",132,29,96,16,true);

    AddText(&ui.accessibilityOptionsPanel,33,41,"Tag_Slowdown","Percent slowdown", NULL,NULL);
    AddSlider(&ui.accessibilityOptionsPanel,34,52,110,10,"Slider_Slowdown",currSettings.slowdownPercent,&currSettings.slowdownPercent);


    InitButton(&ui.accessibilityOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.accessibilityOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/accessiblity_tab_icon.png",true));
    ui.accessibilityOptionsPanel.back = &ui.mainMenuPanel;

    int colorPickerH = 20;
    int colorPickerY = 100;
    int colorPickerPadding = 20;

    //AddPulldownMenu(&ui.accessibilityOptionsPanel,34,colorPickerY-25,48,10,"Colour Preset",0,2,"Default","Custom");
    AddButton(&ui.accessibilityOptionsPanel,"Reset","Reset",34,colorPickerY-25,30,10,true);

    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Background","Background", &WHITE,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"BG",34,colorPickerY,40,20, &BG);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Friendly","Friendly", &FRIENDLY,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"FRIENDLY",34,colorPickerY,40,20, &FRIENDLY);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Enemy","Enemy", &ENEMY,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"ENEMY",34,colorPickerY,40,20, &ENEMY);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Ground","Ground", &GROUND,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"GROUND",34,colorPickerY,40,20, &GROUND);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Ground Dark","Ground Dark", &GROUND_DARK,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"GROUND_DARK",34,colorPickerY,40,20, &GROUND_DARK);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Edge Highlight","Edge Highlight", &EDGE_HIGHLIGHT,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"EDGE_HIGHLIGHT",34,colorPickerY,40,20, &EDGE_HIGHLIGHT);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Background Decor","Background Decor", &BG_DECOR,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"BG_DECOR",34,colorPickerY,40,20, &BG_DECOR);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Textured Ground","Textured Ground", &TEXTURED_GROUND,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"TEXTURED_GROUND",34,colorPickerY,40,20, &TEXTURED_GROUND);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Poison","Poison",&POISON,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"POISON",34,colorPickerY,40,20, &POISON);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Heal","Heal",&HEAL,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"HEAL",34,colorPickerY,40,20, &HEAL);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Damage","Damage",&DAMAGE,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"DAMAGE",34,colorPickerY,40,20, &DAMAGE);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Speed","Speed",&SPEED,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"SPEED",34,colorPickerY,40,20, &SPEED);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Shield","Shield",&SHIELD,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"SHIELD",34,colorPickerY,40,20, &SHIELD);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Friendly Poison","Friendly Poison",&FRIENDLY_POISON,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"FRIENDLY_POISON",34,colorPickerY,40,20, &FRIENDLY_POISON);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Friendly Heal","Friendly Heal",&FRIENDLY_HEAL,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"FRIENDLY_HEAL",34,colorPickerY,40,20, &FRIENDLY_HEAL);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Friendly Damage","Friendly Damage",&FRIENDLY_DAMAGE,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"FRIENDLY_DAMAGE",34,colorPickerY,40,20, &FRIENDLY_DAMAGE);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Friendly Speed","Friendly Speed",&FRIENDLY_SPEED,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"FRIENDLY_SPEED",34,colorPickerY,40,20, &FRIENDLY_SPEED);
    colorPickerY+=colorPickerH+colorPickerPadding;
    AddText(&ui.accessibilityOptionsPanel,34,colorPickerY-12,"Friendly Shield","Friendly Shield",&FRIENDLY_SHIELD,NULL);
    AddColorPicker(&ui.accessibilityOptionsPanel,"FRIENDLY_SHIELD",34,colorPickerY,40,20, &FRIENDLY_SHIELD);
    colorPickerY+=colorPickerH+colorPickerPadding;









}
void InitChoosingUnitButtons()
{
    InitButton(&ui.choosingUnits_Back,"Back","Back",45,194,48,16,0);
    InitButton(&ui.choosingUnits_GO,"Adventure","Adventure",109,194,96,16,0);
    InitButton(&ui.choosingUnits_Hire,"","",166,4,44,16,LoadSprite("assets/ui/button_hire.png",true));
    //InitButton(&ui.choosingUnits_Artifacts,"","",120,4,44,16,LoadSprite("assets/ui/button_artifact.png",true));

    
}
void InitEndScreen()
{
    //endScreen
    int w = 104;
    int x2 = 136;

    #ifdef _REPLAY
        w = 72;
        x2 = 91;
        InitButton(&ui.endScreen_SaveReplay,"Save Replay","Save Replay",172,224,72,16,0);
    #endif

    InitButton(&ui.endScreen_Back,"Back","Back",10,224,w,16,0);
    InitButton(&ui.endScreen_Retry,"Retry","Retry",x2,224,w,16,0);
}
void InitGameUI()
{
    InitButton(&ui.menuButton,"Menu","Menu",213,4,40,16,0);

    int w = 80;
    InitButton(&ui.nextLevelButton,"Descend","Descend",_SCREEN_SIZE/2-w/2,10,w,12,0);
    ui.nextLevelButton.enabled = false;
    ui.nextLevelButton.bgColor =  COLOR__TRANSPARENT; 

    ui.gold_element_sprite_index = LoadSprite("assets/ui/gold.png",false);

}
void InitControlsPanel()
{
    ui.controlsPanel = CreatePanel(48,48,180,OPTIONS_PANEL_HEIGHT,15,true);

    AddText(&ui.controlsPanel,33,10,"Controls_Options", "Controls Options",NULL, &ui.boldFont);


    int xText = 16;


    int y = 16;
    int w = 24;
    int h = 16;
    
    int xButtons = 160-w-padding*4;

    AddButton(&ui.controlsPanel,"Reset","Reset",xButtons,y+=h+padding,30,10,true);

    
    AddKeyInput(&ui.controlsPanel,"Q","Q",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Q.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Q2","Q2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Q.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"First Ability","First Ability",NULL,NULL);


    AddKeyInput(&ui.controlsPanel,"W","W",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_W.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"W2","W2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_W.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Second Ability","Second Ability",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"E","E",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_E.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"E2","E2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_E.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Third Ability","Third Ability",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"R","R",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_R.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"R2","R2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_R.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Fourth Ability","Fourth Ability",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"F","F",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_F.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"F2","F2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_F.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Fifth Ability","Fifth Ability",NULL,NULL);


    AddKeyInput(&ui.controlsPanel,"AMove","AMove",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_AMove.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"AMove2","AMove2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_AMove.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Attack Move","Attack Move",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Add Selection","Add Selection",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Shift.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Add Selection2","Add Selection2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Shift.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Add Selection","Add Selection",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Shift Selected","Shift Selected",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Tab.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Shift Selected2","Shift Selected2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Tab.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Shift Selected","Shift Selected",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Set Group","Set Group",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Ctrl.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Set Group2","Set Group2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Ctrl.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Set Group","Set Group",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Show Map","Show Map",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ShowMap.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Show Map2","Show Map2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ShowMap.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Show Map","Show Map",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Pause","Pause",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Pause.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pause2","Pause2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Pause.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pause","Pause",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Cancel Cast","Cancel Cast",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Cancel.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Cancel Cast","Cancel Cast",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Cancel.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Cancel Cast","Cancel Cast",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Pan Left","Pan Left",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanLeft.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Left2","Pan Left2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanLeft.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Left","Pan Left",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Pan Right","Pan Right",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanRight.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Right2","Pan Right2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanRight.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Right","Pan Right",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Pan Up","Pan Up",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanUp.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Up2","Pan Up2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanUp.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Up","Pan Up",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Pan Down","Pan Down",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanDown.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Down2","Pan Down2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanDown.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Down","Pan Down",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 1","Control Group 1",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[0].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 1_2","Control Group 1_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[0].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 1","Control Group 1",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 2","Control Group 2",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[1].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 2_2","Control Group 2_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[1].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 2","Control Group 2",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 3","Control Group 3",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[2].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 3_2","Control Group 3_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[2].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 3","Control Group 3",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 4","Control Group 4",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[3].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 4_2","Control Group 4_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[3].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 4","Control Group 4",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 5","Control Group 5",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[4].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 5_2","Control Group 5_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[4].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 5","Control Group 5",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 6","Control Group 6",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[5].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 6_2","Control Group 6_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[5].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 6","Control Group 6",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 7","Control Group 7",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[6].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 7_2","Control Group 7_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[6].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 7","Control Group 7",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 8","Control Group 8",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[7].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 8_2","Control Group 8_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[7].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 8","Control Group 8",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 9","Control Group 9",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[8].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 9_2","Control Group 9_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[8].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 9","Control Group 9",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Control Group 0","Control Group 0",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[9].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 0_2","Control Group 0_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[9].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 0","Control Group 0",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Center","Center",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Center.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Center2","Center2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Center.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Center","Center",NULL,NULL);


    AddKeyInput(&ui.controlsPanel,"Hold","Hold",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_HoldPosition.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Hold2","Hold2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_HoldPosition.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Hold Position","Hold Position",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Stop","Stop",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Stop.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Stop2","Stop2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Stop.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Stop","Stop",NULL,NULL);

    AddKeyInput(&ui.controlsPanel,"Console","Console",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Console.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Console2","Console2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Console.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Console","Console",NULL,NULL);

    InitButton(&ui.controlsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.controlsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/controls_tab_icon.png",true));
    ui.controlsPanel.back = &ui.mainMenuPanel;


}
void SetWidgetSprite(Widget* w, char* sprite)
{
    int sprIndex = LoadSprite(sprite,false);
    w->spriteIndex = sprIndex;
}
Widget* GetWidgetByName(GameState gameState, char* name)
{
    for (int i = 0; i < numSprites_States[gameState]; i++)
    {
        Widget* w = &Widgets_States[gameState][i];
        if (w->name && strcmp(name,w->name) == 0)
            return w;
    }
    return NULL;
}
void InitUI()
{
    ui.augmentIconIndex = LoadSprite("assets/ui/augment.png",false);
    _UI_IGNORE_INPUT = false;
    //ui.mainMenuPanel = CreatePanel(29,97,144,15,UI_PADDING,false);
    InitFonts();

    InitMainMenuPanel();
    InitLoadReplayPanel();
    InitPausePanel();
    InitVideoOptionsPanel();
    InitAudioOptionsPanel();
    InitAccessibilityOptionsPanel();
    InitControlsPanel();
    
    InitPurchaseScreenUI();
    InitChoosingUnitButtons();


    TabGroup(4,&ui.videoOptionsPanel,&ui.audioOptionsPanel,&ui.accessibilityOptionsPanel,&ui.controlsPanel);

    InitEndScreen();
    InitGameUI();


    ui.animatePanel = UI_ANIMATE_STATIC;
    ui.panelShownPercent = 1;
    ui.changingTo = NULL;
    
    ui.currentPanel = NULL;

   // Checkbox* particles = (Checkbox*)GetUIElement(&ui.videoOptionsPanel,"EnableParticles")->data;
    //particles->activated = &currSettings.particlesEnabled;

    //currSettings.particlesEnabled = &particles->activated;


    ui.panel_sprite_index = LoadSprite("assets/ui/ingame/ui.png",false);
    ui.panel_unselected_abilities_sprite_index = LoadSprite("assets/ui/ingame/ui_unselected.png",false);
    ui.panel_0_abilities_sprite_index = LoadSprite("assets/ui/ingame/ui_0abilities.png",false);
    ui.panel_1_abilities_sprite_index = LoadSprite("assets/ui/ingame/ui_1abilities.png",false);
    ui.panel_2_abilities_sprite_index = LoadSprite("assets/ui/ingame/ui_2abilities.png",false);
    ui.panel_3_abilities_sprite_index = LoadSprite("assets/ui/ingame/ui_3abilities.png",false);
    ui.panel_5_abilities_sprite_index = LoadSprite("assets/ui/ingame/ui_5abilities.png",false);
    ui.health_element_sprite_index = LoadSprite("assets/ui/ingame/health.png",false);
    ui.health_and_mana_element_sprite_index = LoadSprite("assets/ui/ingame/health_and_mana.png",false);


    ui.panelShownPercent = 1.0f;


    LoadCursorSprite(&ui,&ui.cursorDefaultIndex,"assets/ui/cursor.png");
    LoadCursorSprite(&ui,&ui.cursorCastingIndex,"assets/ui/cursor_cast.png");
    LoadCursorSprite(&ui,&ui.cursorAttackIndex,"assets/ui/cursor_attack.png");
    LoadCursorSprite(&ui,&ui.cursorFriendlyIndex,"assets/ui/cursor_friendly.png");



    ui.currentPanel = &ui.mainMenuPanel;
    ui.panelShownPercent = 1.0f;
    ui.animatePanel = UI_ANIMATE_STATIC;


    for (int i = 0; i < NUMGAMESTATES; i++)
    {
        Widgets_States[i] = calloc(NUMSPRITESTATESTOALLOC,sizeof(Widget));
        numSprites_States[i] = 0;
    }
    int gear32 = LoadSprite("assets/ui/gear32x32.png",false);
    int gear64 = LoadSprite("assets/ui/gear64x64.png",false);

    int lantern = LoadSprite("assets/ui/mainmenu/lantern.png",false);
    int hand = LoadSprite("assets/ui/mainmenu/hand.png",false);
    int name = LoadSprite("assets/ui/mainmenu/name.png",false);
    int block = LoadSprite("assets/ui/mainmenu/block.png",false);
    int edge = LoadSprite("assets/ui/mainmenu/mainmenuedges.png",false);

    CreateWidget(GAMESTATE_MAIN_MENU,"Edge",&sprites[edge],0,0,0.5,0.5,DRAWORDER_BEFOREUI,0,NULL);

    //CreateWidget(GAMESTATE_MAIN_MENU,"Block",&sprites[block],157,93,0.5,0.5,DRAWORDER_BEFOREUI,0,NULL);
    //CreateWidget(GAMESTATE_MAIN_MENU,"Lantern",&sprites[lantern],155,100,0.46,0.033,DRAWORDER_BEFOREUI,0,UpdateLanternWidget);
    Widget* handWidget = CreateWidget(GAMESTATE_MAIN_MENU,"Hand",&sprites[hand],115,82,0.46,0.033,DRAWORDER_BEFOREUI,0,UpdateHandWidget);
    handWidget->desiredY = 0;
    CreateWidget(GAMESTATE_MAIN_MENU,"Name",&sprites[name],29,50,0.5,0.5,DRAWORDER_BEFOREUI,0,NULL);



    int x = -5;

    for (int i = 0; i < _SCREEN_SIZE/32; i++)
    {   

        //CreateWidget(GAMESTATE_MAIN_MENU,"Gear",&sprites[gear32],x,0,DRAWORDER_AFTERUI,0);
        //CreateWidget(GAMESTATE_MAIN_MENU,"Gear",&sprites[gear32],x+16,18,DRAWORDER_AFTERUI,0);

        //CreateWidget(GAMESTATE_MAIN_MENU,"Gear",&sprites[gear32],x,_SCREEN_SIZE-32,DRAWORDER_AFTERUI,0);
        //CreateWidget(GAMESTATE_MAIN_MENU,"Gear",&sprites[gear32],x+16,_SCREEN_SIZE-50,DRAWORDER_AFTERUI,0);

        x+=31;
    }
    chatboxes = NULL;
    numChatboxes = 0;


    ui.chestIdle = LoadAnimation("assets/ui/endscreen/chest.png",42,43,0.25f,0,1);
    ui.chestWiggle = LoadAnimation("assets/ui/endscreen/chest_wiggle.png",42,43,0.08f,0,20);
    ui.chestOpen = LoadAnimation("assets/ui/endscreen/chest_open.png",42,43,0.1f,0,5);
    ui.chestOpen.holdOnLastFrame = true;
    ui.chestWiggle.holdOnLastFrame = true;
}
void UpdateWidget(Widget* w, float dt)
{
    float directionX = w->desiredX - w->x;
    float directionY = w->desiredY - w->y;


    w->x = Towards(w->x,w->desiredX,w->velocity*dt);
    w->y = Towards(w->y,w->desiredY,w->velocity*dt);
    w->rotation += w->rotationSpeed*dt;     

    if (w->updateFunc)
    {
        w->updateFunc(w,dt);
    }

    if (gameState == GAMESTATE_MAIN_MENU)
    {
        float amt = transitionTimer * rand()/(float)RAND_MAX;
        //w->rotationSpeed = (sqrt(amt)*sqrt(amt)) * 1000 * dt;
    }
}

void UpdateWidgets(float dt)
{
    for (int i = 0; i < NUMGAMESTATES; i++)
    {
        for (int j = 0; j < numSprites_States[i]; j++)
        {
            Widget* w = &Widgets_States[i][j];
            UpdateWidget(w,dt);
        }
    }
}
Widget* CreateWidget(GameState gameStateToAttach, char* name, Sprite* spr, int x, int y, float originX, float originY, Widget_DrawOrder drawOrder, int id, void* func)
{
    Widget s = {0};
    s.spriteIndex = spr - sprites;
    s.drawOrder = drawOrder;
    s.x = x;
    s.y = y;
    s.desiredX = x;
    s.desiredY = y;
    s.id = id;
    s.originX = originX;
    s.originY = originY;
    s.updateFunc = func;
    s.timer = 0;
    if (name)
    {
        s.name = calloc(strlen(name)+1,sizeof(char));
        strcpy(s.name,name);
    }
    if ((numSprites_States[gameStateToAttach] + 1) % NUMSPRITESTATESTOALLOC == 0)
    {
        int reallocTo = ceilf((float)numSprites_States[gameStateToAttach] + NUMSPRITESTATESTOALLOC)/(float)NUMSPRITESTATESTOALLOC+1;
        Widgets_States[gameStateToAttach] = realloc(Widgets_States[gameStateToAttach],NUMSPRITESTATESTOALLOC * reallocTo*sizeof(Widget));
    }
    Widgets_States[gameStateToAttach][numSprites_States[gameStateToAttach]] = s;
    numSprites_States[gameStateToAttach]++;
    return &Widgets_States[gameStateToAttach][numSprites_States[gameStateToAttach]-1];
}
void DrawWidgets(GameState currentState, Widget_DrawOrder drawOrderToDraw)
{
    int numWidgets = numSprites_States[currentState];
    for (int i = 0; i < numWidgets; i++)
    {
        Widget* u = &Widgets_States[currentState][i];
        if (u->drawOrder == drawOrderToDraw)
        {
            Sprite* s = &sprites[u->spriteIndex];
            DrawSprite(s,u->x,u->y,u->originX,u->originY,u->rotation,FRIENDLY,false,false,false);
        }
    }
}
void InitFonts()
{
    ui.font = al_load_ttf_font("assets/fonts/font.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.boldFont = al_load_ttf_font("assets/fonts/fontbold.ttf", 8, ALLEGRO_TTF_MONOCHROME);
    ui.tinyFont = al_load_ttf_font("assets/fonts/4x8.ttf", 8, ALLEGRO_TTF_MONOCHROME);


}
bool GetButton(Panel* p, char* name)
{
    if (!p) 
        return false;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = &p->elements[i];
        //if (u->enabled)
        {
            if (strcasecmp(u->name,name)==0)
            {
                return GetButtonIsClicked(u);
               /* Button* b = (Button*)u->data;
                if (b->activated)
                {
                    PlaySound(&sounds[u->sound_clickDown_Index],0.5f,0,false);
                }
                return (b->activated);*/
            }
        }
    }
    return false;
}
void UpdateTabButtons(Panel* p, MouseState* mouseState, MouseState* mouseStateLastFrame)
{
    int y = p->y; 
    int x = p->x;

    if (p->backButton.data)
    {
            Button* b = (Button*)p->backButton.data;

        UpdateButton(x,y,p->backButton.w,p->backButton.h,&p->backButton,*mouseState,*mouseStateLastFrame);
        if (b->activated)
        {
            ChangeUIPanel(p->back);
        }

        y+=p->backButton.h;
    }
    for (int i = 0; i < p->numTabs; i++)
    {
        Panel* tab = p->tabs[i];
        Button* b = (Button*)tab->tabButton.data;
        int w; int h; 
        h = al_get_bitmap_height(sprites[b->spriteIndex].sprite);
        w = al_get_bitmap_width(sprites[b->spriteIndex].sprite);
        UpdateButton(x,y,tab->tabButton.w,tab->tabButton.h,&tab->tabButton,*mouseState,*mouseStateLastFrame);
        if (b->activated)
        {
            ChangeUIPanel(tab);
        }
        y += h;
    }
}
void UpdateKeyInput(int rX, int rY, int w, int h, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);

    KeyInput* t = (KeyInput*)u->data;
    Rect r = (Rect){rX,rY,w,h};

    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        if (t->clicked)
        {
            //for unbinding key
            if (!PointInRect(mouseState.screenX,mouseState.screenY,r))
            {
                *t->mappedTo = 0;
                UpdateBind(u);
                t->clicked = false;
                return;
            }
        }

        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            if (!t->clicked)
            {
                PlaySound(&sounds[u->sound_clickUp_Index],0.5f,0,false);
            }
            t->clicked = true;
        }
        else
        {
            if (t->clicked)
            {
                //UpdateBind(u);
                //t->clicked = false;
            }
            t->clicked = false;
            UpdateBind(u);
        }
    }


    if (t->clicked)
    {

        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
        {
            if (KeyDown(keyStateThisFrame,i))
            {
                if (t->mappedTo)
                    *t->mappedTo = i;
                t->clicked = false;
                UpdateBind(u);
                break;
            }

        }
    }
}
void UpdateTextInput(int rX, int rY, int w, int h, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    TextInput* t = (TextInput*)u->data;
    Rect r = (Rect){rX,rY,w,h};

    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            t->clicked = true;
            _TEXTINPUT_HIGHLIGHTED = true;
        }
        else
        {
            t->clicked = false;
            _TEXTINPUT_HIGHLIGHTED = false;
        }
    }
    if (KeyDown(keyStateThisFrame,ALLEGRO_KEY_ENTER))
    {
        t->clicked = false;
        _TEXTINPUT_HIGHLIGHTED = false;
    }
    if (t->clicked)
    {
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
        {
            if (KeyDown(keyStateThisFrame,i) && !KeyDown(keyStateLastFrame,i))
            {
                const char* keyName = al_keycode_to_name(i);
                char key = 0;
                bool shift = KeyDown(keyStateThisFrame,ALLEGRO_KEY_LSHIFT);

                if (strlen(keyName) == 1)
                    key = keyName[0];
                else
                {
                    if (i == ALLEGRO_KEY_BACKSPACE)
                        key = 8;   

                    if (i == ALLEGRO_KEY_MINUS)
                        key = '-';   
                    if (i == ALLEGRO_KEY_MINUS && shift)
                        key = '_';
                }
                //backspace
                if (key == 8)
                {
                    if (strlen(t->text) > 0)
                    {
                        int jdfgdf = strlen(t->text);
                        t->text[strlen(t->text)-1] = '\0';
                    }
                    break;
                }
                if (t->onlyAllowNumbers)
                {
                    if (isdigit(key)) 
                    {
                        if (strlen(t->text) < t->maxChars)
                            t->text[strlen(t->text)] = key;
                    }

                }
                else
                {
                    if (strlen(t->text) < t->maxChars)
                        t->text[strlen(t->text)] = key;

                }
                break;
            }

        }
    }
}
void UpdateButton(int rX, int rY, int w, int h, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame)
{
    #ifdef WINDOW_FOCUS_LOGIC
        if (_UI_IGNORE_INPUT)
            return;
    #endif
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);
    if (_PANEL_CLICKED_THIS_FRAME)
        return;
    Button* b = (Button*)u->data;
    b->activated = false;
    b->mousedOver = false;

    Rect r = (Rect){rX,rY,w,h};

    if (PointInRect(mouseState.screenX,mouseState.screenY,r))
    {
        b->mousedOver = true;
    }
    if (u->enabled && PointInRect(mouseState.screenX,mouseState.screenY,r) && !PointInRect(mouseStateLastFrame.screenX,mouseStateLastFrame.screenY,r))
    {
        PlaySoundStr("assets/audio/button_hover.wav",1,0,false);
    }


    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            if (!b->clicked)
            {
                PlaySound(&sounds[u->sound_clickUp_Index],0.5f,0,false);
            }
            b->clicked = true;
        }
    }
    if (mouseStateLastFrame.mouse.buttons & 1 && !(mouseState.mouse.buttons & 1))
    {
        if (b->clicked)
        {
            if (PointInRect(mouseState.screenX,mouseState.screenY,r))
            {
                b->activated = true;
                u->isHighlighted = false;
            }
        }
    }
    if (!(mouseState.mouse.buttons & 1))
    {
        b->clicked = false;
    }
}
bool UpdateElement(Panel* p, UIElement* u, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    if (!u->enabled)
        return false;
    #ifdef WINDOW_FOCUS_LOGIC
    if (_UI_IGNORE_INPUT)
        return false;
    #endif
    int x; int y; 
    int w; int h;
    GetUILocation(p,u,&x,&y, &w,&h,true);

    if (u->elementType == ELEMENT_BUTTON)
    {
        UpdateButton(x,y,w,h,u,*mouseState,*mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_KEYINPUT)
    {
        UpdateKeyInput(x,y,w,h,u,*mouseState,*mouseStateLastFrame,keyStateThisFrame);
    }
    if (u->elementType == ELEMENT_TEXTINPUT)
    {
        UpdateTextInput(x,y,w,h,u,*mouseState,*mouseStateLastFrame,keyStateThisFrame,keyStateLastFrame);
    }


    if (u->elementType == ELEMENT_SLIDER)
    {
        UpdateSlider((Slider*)u->data, x,  y, w, h, *mouseState, *mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_CHECKBOX)
    {
        UpdateCheckbox((Checkbox*)u->data, x,  y,w,h,true,*mouseState, *mouseStateLastFrame);
        Checkbox* c = (Checkbox*)u->data;
       // bool b = *c->activated;
    }
    if (u->elementType == ELEMENT_PULLDOWN)
    {
        return UpdatePulldownMenu((Pulldown*)u->data, x,  y, w,h,*mouseState, *mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_COLOR_PICKER)
    {
         UpdateColorPicker((ColorPicker*)u->data, x,  y, w,h,*mouseState, *mouseStateLastFrame);
    }

    return false;
}
void UpdatePanel(Panel* p, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    if (p)
    {
        if (!_PANEL_CLICKED_THIS_FRAME)
        {
            for (int i = p->numElements-1; i  >= 0; i--)
            {
                
                if (UpdateElement(p,&p->elements[i],mouseState,mouseStateLastFrame, keyStateThisFrame, keyStateLastFrame))
                {   
                    break;
                }
            }
            Rect r = (Rect){p->x,p->y,p->w,p->h};

            if ((mouseState->mouse.buttons & 1) && !(mouseStateLastFrame->mouse.buttons & 1) && PointInRect(mouseState->screenX,mouseState->screenY,r))
                _PANEL_CLICKED_THIS_FRAME = true;
            else
                _PANEL_CLICKED_THIS_FRAME = false;

        }

    }
    UpdateScrollbar(p,mouseState,mouseStateLastFrame);

}
void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseStateLastFrame, float dt)
{

    //todo: refactor this to be automatic
    if (GameIsIngame()) 
    {
        ui.videoOptionsPanel.back = &ui.pauseMenuPanel;
        ui.audioOptionsPanel.back = &ui.pauseMenuPanel;
        ui.accessibilityOptionsPanel.back = &ui.pauseMenuPanel;
        ui.controlsPanel.back = &ui.pauseMenuPanel;
    }
    else
    {
        ui.videoOptionsPanel.back = &ui.mainMenuPanel;
        ui.audioOptionsPanel.back = &ui.mainMenuPanel;
        ui.accessibilityOptionsPanel.back = &ui.mainMenuPanel;
        ui.controlsPanel.back = &ui.mainMenuPanel;
    }
    if (ui.currentPanel)
    {
        UpdateTabButtons(ui.currentPanel, mouseState,mouseStateLastFrame);
    }

    if (IsBindDownThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_Pause))
    {
        if (GameIsIngame())
        {
            if (ui.currentPanel)
            {
                ChangeUIPanel(NULL);
                WriteSettingsFile("config.cfg");
                //ui.currentPanel = NULL;
            }
            else
            {
                ChangeUIPanel(&ui.pauseMenuPanel);
            }
        }
    }
    if (ui.animatePanel != UI_ANIMATE_STATIC)
    {
        //if (ui.changingTo)
        {
            if (ui.animatePanel == UI_ANIMATE_IN)
            {
                ui.panelShownPercent += dt*4;
                if (ui.panelShownPercent >= 1)
                {
                    ui.panelShownPercent = 1;
                    ui.animatePanel = UI_ANIMATE_STATIC;
                    ui.currentPanel = ui.changingTo;
                }
            }
            if (ui.animatePanel == UI_ANIMATE_OUT)
            {
                ui.panelShownPercent -= dt*4;
                if (ui.panelShownPercent <= 0)
                {
                    ui.panelShownPercent = 0;
                    ui.animatePanel = UI_ANIMATE_IN;
                    ui.currentPanel = ui.changingTo;
                }
            }

        }
    }
    if (ui.animatePanel == UI_ANIMATE_STATIC)
    {
        ui.panelShownPercent = 1;
        ui.currentPanel = ui.changingTo;
    }

    if (ui.currentPanel)
    {
        UpdatePanel(ui.currentPanel,mouseState,mouseStateLastFrame, keyState, keyStateLastFrame);
    }
    int displayW = al_get_display_width(display);
    int displayH = al_get_display_height(display);

    if ((_RENDERSIZE+1)*256 > displayH || (_RENDERSIZE+1)*256 > displayW)
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale+");
        u->enabled = false;
    }
    else
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale+");
        u->enabled = true;
    }

    if (_RENDERSIZE == 1)
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale-");
        u->enabled = false;
    }
    else
    {
        UIElement* u = GetUIElement(&ui.videoOptionsPanel,"RenderScale-");
        u->enabled = true;
    }

    if (gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER)
    {
        UpdateLoadscreen(dt,keyState,mouseState);
    }
    if (ui.currentPanel == &ui.accessibilityOptionsPanel)
    {
        if (GetButton(&ui.accessibilityOptionsPanel,"Reset"))
        {
            SetColors(PRESET_DEFAULT);
        }
    }
        if (ui.currentPanel == &ui.controlsPanel)
    {
        if (GetButton(&ui.controlsPanel,"Reset"))
        {
            currSettings.keymap = defaultSettings.keymap;
            
            for (int i = 0; i < ui.controlsPanel.numElements; i++)
            {
                UpdateBind(&ui.controlsPanel.elements[i]);
            }
        }
    }


}
void SetUIElementFont(UIElement* u, ALLEGRO_FONT** f)
{
    u->font = f;
}
void DrawButtonText(UIElement* u,int x, int y, ALLEGRO_COLOR col)
{
    if (!u)
        return;
    Button* b = (Button*)u->data;
    int align = b->drawLine ? ALLEGRO_ALIGN_CENTRE : ALLEGRO_ALIGN_LEFT;
    float textX; float textY;
    if (align == ALLEGRO_ALIGN_CENTRE)
    {
        textX = x + u->w/2;
        textY = y + u->h/2 - al_get_font_line_height(ui.font)/2.0;
    }
    if (align == ALLEGRO_ALIGN_LEFT)
    {
        textX = x+3;
        textY = y + u->h/2 - al_get_font_line_height(ui.font)/2.0;
    }
    ALLEGRO_FONT* font = GetElementFont(u);
    
    al_draw_text(font,col,textX,floor(textY),align,b->description);
}   
void DrawColorPicker(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor, ALLEGRO_COLOR foregroundColor, bool fromPanel)
{
    int panelX; int panelY; int panelW; int panelH;
    al_get_clipping_rectangle(&panelX,&panelY,&panelW,&panelH);
    Rect panel = (Rect){panelX,panelY,panelW,panelH};
    ColorPicker* c = (ColorPicker*)u->data;
    Rect slider = (Rect){x,y,(int)u->w*3+padding*3,(int)u->h};

    Rect clip = (Rect){x,y,slider.w,slider.h};

    ALLEGRO_FONT* font = GetElementFont(u);

    if (fromPanel)
    {

        clip.x = clamp(clip.x,panel.x,panel.x+panel.w) - 1;
        clip.y = clamp(clip.y,panel.y,panel.y+panel.h) - 1;
        clip.w = _MAX(0,slider.w) + 1;
        clip.h = _MAX(0,(panel.y+panel.h) - clip.y);

        al_set_clipping_rectangle(clip.x,clip.y,clip.w,clip.h);
    }
    for (int i = 0; i < 3; i++)
    {
        UIElement u2 = {0};
        u2.x = x + (u->w*i)+(padding*i);
        u2.y = y;
        ColorPicker* c = (ColorPicker*)u->data;
        Slider* s = c->sliders[i];
        u2.data = s;
        u2.w = u->w;
        u2.h = u->h;
        

        DrawSlider(&u2,u2.x,u2.y,&mouseState,true,GetColor(u->bgColor,0));
    }
    al_reset_clipping_rectangle();

}
void UpdateColorPicker(ColorPicker* c, int x, int y, int w, int h, MouseState mouseState, MouseState mouseStateLastFrame)
{
    for (int i = 0; i < 3; i++)
    {
        int x2 = x + (w*i)+(padding*i);
        UpdateSlider(c->sliders[i],x2,y,w,h,mouseState,mouseStateLastFrame);
    }
}

UIElement* AddColorPicker(Panel* p, char* name, int x, int y, int w, int h, ALLEGRO_COLOR* colour)
{
    ColorPicker* c = calloc(1,sizeof(ColorPicker));
    c->sliders[0] = InitSlider(x+w*0+padding*0,y,w,h,colour->r,&colour->r,&colour->r,NULL,NULL);
    c->sliders[1] = InitSlider(x+w*1+padding*1,y,w,h,colour->g,&colour->g,NULL,&colour->g,NULL);
    c->sliders[2] = InitSlider(x+w*2+padding*2,y,w,h,colour->b,&colour->b,NULL,NULL,&colour->b);

    UIElement u = {0};
    u.data = (void*)c;
    u.w = w;
    u.h = h;
    u.x = x;
    u.y = y;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.elementType = ELEMENT_COLOR_PICKER;
    u.enabled = true;
    u.bgColor = COLOR_BG;
    UIElement* ref = AddElement(p,&u);
    return ref;
}
void DrawButton(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor, bool drawRectWhenUnselected, ALLEGRO_COLOR foregroundColor, bool fromPanel)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    int panelX; int panelY; int panelW; int panelH;
    al_get_clipping_rectangle(&panelX,&panelY,&panelW,&panelH);
    Rect panel = (Rect){panelX,panelY,panelW,panelH};
    Button* b = (Button*)u->data;
    Rect button = (Rect){x,y,(int)u->w,(int)u->h};

    Rect clip = (Rect){x,y,button.w,button.h};

    ALLEGRO_FONT* font = GetElementFont(u);

    if (fromPanel)
    {

        clip.x = clamp(clip.x,panel.x,panel.x+panel.w) - 1;
        clip.y = clamp(clip.y,panel.y,panel.y+panel.h) - 1;
        clip.w = _MAX(0,button.w) + 1;
        clip.h = _MAX(0,(panel.y+panel.h) - clip.y);

        al_set_clipping_rectangle(clip.x,clip.y,clip.w,clip.h);
    }

    int align = b->drawLine ? ALLEGRO_ALIGN_CENTRE : ALLEGRO_ALIGN_LEFT;
    int textX; int textY;
    if (align == ALLEGRO_ALIGN_CENTRE)
    {
        textX = x + u->w/2;
        textY = y + u->h/2 - al_get_font_line_height(font)/2.0;
    }
    if (align == ALLEGRO_ALIGN_LEFT)
    {
        textX = x+3;
        textY = y + u->h/2 - al_get_font_line_height(font)/2.0;
    }

    if (b->clicked && isActive)
    {
        DrawFilledRectangle(x,y,x+u->w,y+u->h,foregroundColor);
        DrawRectangle(x,y,x+u->w,y+u->h,bgColor,1);
        al_draw_text(font,BG,textX,textY,align,b->description);
    }
    else 
    {
        if (b->drawLine)
        {
            DrawFilledRectangle(x,y,x+u->w,y+u->h,bgColor);
            if (isActive)
            {
                DrawRectangle(x, y, x + button.w, y + button.h, foregroundColor,1);
            }
            else
            {
                DrawOutlinedRect_Dithered(button,foregroundColor);
            }
        }
        DrawButtonText(u,x,y,foregroundColor);
    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,button) && isActive && !b->clicked)
    {
        DrawRectangle(x+2,y+2,x+u->w-2,y+u->h-2,foregroundColor,1);
    }
    if (b->spriteIndex)
    {
        if (!b->clicked) 
        {
            DrawSprite(&sprites[b->spriteIndex],x,y,0.5f,0.5f,0,foregroundColor,false,false,false);
        }
        if (b->clicked)
        {
            DrawSprite(&sprites[b->spriteIndex],x,y,0.5f,0.5f,0,bgColor,false,false,false);

        }
    }
    if (fromPanel)
        al_reset_clipping_rectangle();


}
void UIDrawText(UIElement* u, int x, int y)
{
   UI_Text* t = u->data;
    ALLEGRO_COLOR* c = t->colour ? t->colour : &FRIENDLY;
    ALLEGRO_FONT* font = *t->font;
    if (!font)
        font = ui.font;
   Text te = (Text){.f = font,ui.boldFont,.x=x,.y=y,.color=*c,.lineHeight=al_get_font_line_height(ui.font)};

   al_do_multiline_text(ui.font,256,t->str,cb,&te);


}
void DrawKeyInput(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    KeyInput* k = (KeyInput*)u->data;
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    ALLEGRO_FONT* font = GetElementFont(u);
    Rect button = (Rect){x,y,u->w,u->h};

    if (!k->text)
    {
        UpdateBind(u);
    }

    if (k->clicked && isActive)
    {
        DrawFilledRectangle(x,y,x+u->w,y+u->h,FRIENDLY);
        DrawRectangle(x,y,x+u->w,y+u->h,BG,1);
        al_draw_text(font,BG,x+u->w/2, y + u->h /2 - al_get_font_ascent(font)/2.0,ALLEGRO_ALIGN_CENTRE,k->text);
     }
    else
    {
        DrawFilledRectangle(x,y,x+u->w,y+u->h,bgColor);
        if (isActive)
        {
            DrawRectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
        }
        else
        {   
            DrawOutlinedRect_Dithered(button,FRIENDLY);
        }
        al_draw_text(font,FRIENDLY,x+u->w/2,y + u->h / 2 - al_get_font_ascent(font)/2.0,ALLEGRO_ALIGN_CENTRE,k->text);

    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,button) && isActive && !k->clicked)
    {
        DrawRectangle(x+2,y+2,x+u->w-2,y+u->h-2,FRIENDLY,1);
    }

}
void DrawTextInput(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    TextInput* t = (TextInput*)u->data;
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    ALLEGRO_FONT* font = GetElementFont(u);
    Rect button = (Rect){x,y,u->w,u->h};

    if (!t->text)
    {
        UpdateBind(u);
    }

    if (t->clicked && isActive)
    {
        DrawFilledRectangle(x,y,x+u->w,y+u->h,FRIENDLY);
        DrawRectangle(x,y,x+u->w,y+u->h,BG,1);
        al_draw_text(font,BG,x+u->w/2, y + u->h /2 - al_get_font_ascent(font)/2.0,ALLEGRO_ALIGN_CENTRE,t->text);
     }
    else
    {
        DrawFilledRectangle(x,y,x+u->w,y+u->h,bgColor);
        if (isActive)
        {
            DrawRectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
        }
        else
        {   
            DrawOutlinedRect_Dithered(button,FRIENDLY);
        }
        al_draw_text(font,FRIENDLY,x+u->w/2,y + u->h / 2 - al_get_font_ascent(font)/2.0,ALLEGRO_ALIGN_CENTRE,t->text);

    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,button) && isActive && !t->clicked)
    {
        DrawRectangle(x+2,y+2,x+u->w-2,y+u->h-2,FRIENDLY,1);
    }

}
void DrawUIElement(UIElement* u, int x, int y, MouseState* mouseState, Color bgColor, Color foregroundColor, bool fromPanel)
{
    if (u->ignoreDraw)
        return;
    ALLEGRO_COLOR col = GetColor(bgColor,0);
    ALLEGRO_COLOR colForeground = GetColor(foregroundColor,0);
    if (u->isHighlighted)
        DrawUIHighlight(u,x,y);
    if (u->elementType == ELEMENT_BUTTON)
    {
        DrawButton(u,x,y,*mouseState,u->enabled,col,((Button*)(u->data))->drawLine,colForeground,fromPanel);
    }
    if (u->elementType == ELEMENT_TEXT)
    {
        UIDrawText(u,x,y);
    }
    if (u->elementType == ELEMENT_SLIDER)
    {
        DrawSlider(u,x,y,mouseState,u->enabled,col);
    }
    if (u->elementType == ELEMENT_CHECKBOX)
    {
        DrawCheckbox((Checkbox*)u->data,x,y,u->w,u->h,u->enabled,*mouseState,col);
    }
    if (u->elementType == ELEMENT_PULLDOWN)
    {
        DrawPullDownMenu((Pulldown*)u->data,x,y,u->w,u->h,u->enabled,mouseState,col);
    }
    if (u->elementType == ELEMENT_KEYINPUT)
    {
        DrawKeyInput(u,x,y,*mouseState,u->enabled,col);
    }
    if (u->elementType == ELEMENT_TEXTINPUT)
    {
        DrawTextInput(u,x,y,*mouseState,u->enabled,col);
    }

    if (u->elementType == ELEMENT_COLOR_PICKER)
    {
        DrawColorPicker(u,x,y,*mouseState,u->enabled,col,WHITE,true);
    }


}
int GetPanelMaxY(Panel* p)
{
    if (p->numElements <= 0)
        return 0;
    int maxY = -INT_MAX;
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        if (u->y+u->h > maxY)
        {
            maxY = u->y+u->h;
        }
    }
    return maxY;
}

void GetUILocation(Panel* p, UIElement* uF, int* x, int* y, int* w, int* h, bool update)
{
    int maxY = GetPanelMaxY(p);
    int currX=p->x+p->padding; int currY=p->y+p->padding; 
    /*
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        currX += u->x - u->w/2 - u->padding;
        if (u == uF)
        {
            *x = currX; 
            *y = currY;
        }
        if (u->linebreak)
        {
            currY += u->h+u->padding;
            currX = p->x+p->padding;
        }
        else
        {
            currX += u->w + u->padding + u->x;
        }
    }*/
    *x = p->x + uF->x;
    *y = (p->y  + uF->y) - ((maxY+p->padding-p->h) * p->scrollPercent);
    *h = uF->h;
    *w = uF->w;

    //if we want the 'update' position
    //which is changed to clamp the rectangle to the panel size
    if (update)
    {
        if (*y + *h >= p->y + p->h)
        {
            *h -= ((*y+*h) - (p->y+p->h));
            *h = _MAX(0,*h);
        }
        if (*y < p->y)
        {
            *h -= (p->y - *y);
            *y += (p->y - *y);
            *h = _MAX(0,*h);

            if (*y + *h < p->y)
                *h = 0;
        }
    }
    if (p->tabs)
    {
        //*x += 25;
    }

}
void DrawPanelTabs(Panel* p, MouseState* mouseState)
{
    int y = p->y; 
    int x = p->x;
    if (p->backButton.data)
    {
       // DrawButton(&p->backButton,x,y,*mouseState,true,BG);
        y+=p->backButton.h;
    }
    for (int i = 0; i < p->numTabs; i++)
    {
        Panel* tab = p->tabs[i];
        if (tab->tabButton.data)
        {
            Button* b = (Button*)tab->tabButton.data;
            int w; int h; 
            h = al_get_bitmap_height(sprites[b->spriteIndex].sprite);
            w = al_get_bitmap_width(sprites[b->spriteIndex].sprite);
            DrawRectangle(x,y,x+w,y+h,FRIENDLY,1);
            DrawButton(&tab->tabButton,x,y,*mouseState,true,BG,true,FRIENDLY,false);
            //DrawSprite(&sprites[b->spriteIndex],p->x,y,FRIENDLY,false);
            y += h;
        } 
    }
}
//ensure ELEMENT_PULLDOWN is always rendered last
int panelSort(const void* a, const void* b) {
    UIElement* u1 = (UIElement*)a;
    UIElement* u2 = (UIElement*)b;
    int j = u1->elementType;
    int z = u2->elementType;

    return (u1->elementType - u2->elementType);
}
void DrawPanel(Panel* p, MouseState* mouseState, float panelShownPercent)
{   
    panelShownPercent = easeOutSine(panelShownPercent);

    qsort(p->elements,p->numElements,sizeof(UIElement),panelSort);

    al_set_clipping_rectangle(p->x-1,p->y-1,p->w+1,p->h*panelShownPercent+2);
    if (p->showBorder)
    {
        DrawFilledRectangle(p->x,p->y,p->x+p->w,p->y+p->h,BG);
        DrawRectangle(p->x,p->y,p->x+p->w,p->y+p->h,FRIENDLY,1);
    }


    al_set_clipping_rectangle(p->x-1,p->y,p->w,p->h*panelShownPercent-1);



    int currX=p->x+p->padding; int currY=p->y+p->padding; 
    if (p->backButton.data)
    {
        DrawButton(&p->backButton,p->x,p->y,*mouseState,true,BG,true,FRIENDLY,false);
        currY+=p->backButton.h;
    }

    for (int i = 0; i < p->numTabs; i++)
    {
        int x = p->x+p->padding;
        int y = p->y*25*i+p->padding;
        DrawRectangle(x,y,x+25,y+25,FRIENDLY,1);
    }
    for (int i = 0; i < p->numElements; i++)
    {
        al_set_clipping_rectangle(p->x-1,p->y,p->w,p->h*panelShownPercent-1);

        UIElement* u = ((UIElement*)&p->elements[i]);
        int x; int y; int w; int h;
        GetUILocation(p, u, &x, &y, &w, &h,false);
        DrawUIElement(u,x,y,mouseState,u->bgColor, COLOR_FRIENDLY,true);

    }
    al_set_clipping_rectangle(p->x-1,p->y,p->w,p->h*panelShownPercent-1);

    if (p->tabs)
    {
        DrawPanelTabs(p, mouseState);
    }
    if (GetPanelMaxY(p) > p->h)
    {
        DrawScrollbar(p);
    }
    else
    {
        p->scrollPercent = 0;
    }
    al_reset_clipping_rectangle();
    


}
void AddPanelTabs(Panel* p, int numTabs, ...)
{
    va_list argp;
    va_start(argp, numTabs);
    if (!p->tabs)
    {
        p->tabs = malloc(numTabs * sizeof(Panel*));
        p->numTabs = 0;
    }
    else
    {
        p->tabs = realloc(p->tabs,sizeof(Panel*)*(p->numTabs+numTabs));
    }
    for (int i = 0; i < numTabs; i++)
    {
        Panel* tab = va_arg(argp, Panel*);
        p->tabs[i+p->numTabs] = tab;
    }
    p->numTabs = numTabs + p->numTabs;

    va_end(argp);
}
void DrawMenus(MouseState* mouseState)
{
    //TODO: change rendering panels to a system that can show multiple at once
    //and use an index to decide order

    if (ui.currentPanel)
    {
        float shownPercent = ui.panelShownPercent;
        if (ui.currentPanel == &ui.mainMenuPanel)
            shownPercent = 1;
        DrawPanel(ui.currentPanel,mouseState,shownPercent);
    }
}
void LoadCursorSprite(UI* ui, int* index, char* path)
{
    *index = LoadSprite(path,false);    
    if (*index == -1)
        return;
    sprites[*index].inverseSprite = al_clone_bitmap(sprites[*index].sprite);
    ALLEGRO_BITMAP* inverse = sprites[*index].inverseSprite;

    int w = al_get_bitmap_width(inverse);
    int h = al_get_bitmap_height(inverse);


    al_lock_bitmap(inverse,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    al_set_target_bitmap(inverse);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            
            ALLEGRO_COLOR pixel = al_get_pixel(sprites[*index].sprite, x, y);
            if (!pixel.a)
            {
                for (int y2 = -1; y2 < 2; y2++)
                {
                    for (int x2 = -1; x2 < 2; x2++)
                    {
                        if (x2 == 0 && y2 == 0)
                            continue;
                        ALLEGRO_COLOR pixel2 = al_get_pixel(sprites[*index].sprite, x+x2, y+y2);
                        if (pixel2.a)
                        {
                            al_put_pixel(x,y,WHITE);
                        }
                    }
                }

            }
        }
    }

    al_unlock_bitmap(inverse);

}
bool GetButtonIsClicked(UIElement* u)
{

    if (_PANEL_CLICKED_THIS_FRAME)
    {
        Button* b = (Button*)(u->data);
        b->activated = false;

        return false;
    }
    if (u->enabled)
    {
        if (u->elementType == ELEMENT_BUTTON)
        {
            Button* b = (Button*)(u->data);
            if (b->activated)
            {
                if (u->sound_clickDown_Index)
                {
                    PlaySound(&sounds[u->sound_clickDown_Index],0.5f,0,false);
                }
            }
            bool isActive = b->activated;
            b->activated = false;
            return isActive;
        }
    }
    return false;
}

void DrawCursor(MouseState* mouseState, int index, bool clicked,ALLEGRO_COLOR color)
{
    if (mouseState->mouse.buttons & 1 || mouseState->mouse.buttons & 2)
    {
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,color,true,false,false);
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,BG,false,false,false);

    }
    else
    {
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,BG,true,false,false);
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,color,false,false,false);
    }


}
bool cb(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    int x = t->x;
    int y = t->y;
    ALLEGRO_FONT* f = t->f;
    int height = t->lineHeight;
    y += line_num*height;

    bool bold = false;
    if (strncmp("[b]",line,3)==0)
    {   
        bold = true;
    }
    
    char* buff = calloc(size+1,sizeof(char));
    if (bold)
    {
        memcpy(buff,line+3,(size-3)*sizeof(char));
        al_draw_text(t->bold,t->color,x,y,ALLEGRO_ALIGN_LEFT,buff);
    }
    else
    {
        memcpy(buff,line,size*sizeof(char));
        al_draw_text(t->f,t->color,x,y,ALLEGRO_ALIGN_LEFT,buff);
    }
    free(buff);
    return true;
}
bool CB_GetHeight(int line_num, const char *line, int size, void *extra)
{
    Text* t = (Text*)extra;
    ALLEGRO_FONT* f = t->f;
    t->h += t->lineHeight;
    return true;
}
int GetDescriptionBoxH(char* description, int wTextbox, ALLEGRO_FONT* f, int padding)
{
    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,NULL,0,0,0,FRIENDLY,al_get_font_line_height(f)+2},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    int height = t->h + padding*2;
    free(size);

    return height;
}
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, ALLEGRO_FONT* bold, int x, int y, int wTextbox, int minH, ALLEGRO_COLOR color, bool dither)
{
    if (!description) return;
    int w;  
    int h;
    int xoffset;
    int yoffset;
    int lineHeight = al_get_font_line_height(f)+2;

    void* size = malloc(sizeof(Text));
    memcpy(size,&(Text){f,bold,x,y,0,color,lineHeight},sizeof(Text));
    al_do_multiline_text(f,wTextbox,description,CB_GetHeight,size);
    Text* t = (Text*)size;
    t->color = color;
    
    Rect r = (Rect){x,y,wTextbox+padding,t->h+padding};
    r.h = r.h > minH ? r.h : minH;
    DrawFilledRectangle(r.x-padding,r.y-padding,x+r.w,y+r.h,BG);
    
    r.x-=padding;
    r.y-=padding;
    r.w+=padding;
    r.h+=padding;
    if (dither)
        DrawOutlinedRect_Dithered(r,color);
    else
    {
       // DrawRectangle(r.x+1,r.y+1,r.w+padding,r.h+padding,color,1);
        DrawRectangle(r.x+1,r.y,r.x+r.w+1,r.y+r.h,color,1);
    
    }
    void* extra = malloc(sizeof(Text));
    memcpy(extra,&(Text){.f=f,.bold=bold,.x=x,.y=y,.color=color,.lineHeight=lineHeight},sizeof(Text));
    
    al_do_multiline_text(f,wTextbox,description,cb,extra);
    free(extra);
    free(size);
}
bool MouseInsideUI(MouseState* mouse)
{
    return mouse->screenY >= UI_START_Y;
}
bool IsInsideUI(int x, int y)
{

    if (y >= _MAX(GetMapHeight(),_SCREEN_SIZE) - (_SCREEN_SIZE-UI_START_Y))
        return true;
    return false;
}
int GetAbilityClicked(MouseState* mouseState,MouseState* mouseStateLastFrame)
{
    GameObject* selected = players[0].selection[players[0].indexSelectedUnit];
    if (selected)
    {
        if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(0,selected->numAbilities)))
            return 0;
        if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(1,selected->numAbilities)))
            return 1;
        if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(2,selected->numAbilities)))
            return 2;
        if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(3,selected->numAbilities)))
            return 3;
        if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(4,selected->numAbilities)))
            return 4;
    }

    return -1;

}
void SetOptions()
{
    if (GetButton(&ui.videoOptionsPanel,"RenderScale+"))
    {

        _RENDERSIZE++;
        char* newText = calloc(NumDigits(_RENDERSIZE)+2  ,sizeof(char));
        sprintf(newText,"%ix",_RENDERSIZE);
        SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,newText);
        SetDisplaySize();

        free(newText);
    }
    if (GetButton(&ui.videoOptionsPanel,"RenderScale-"))
    {

        _RENDERSIZE--;
        char* newText = calloc(NumDigits(_RENDERSIZE)+2  ,sizeof(char));
        sprintf(newText,"%ix",_RENDERSIZE);
        SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,newText);
        SetDisplaySize();

        free(newText);
    }
}


Rect GetChestRect(int index)
{
    Rect r = (Rect){18+(ui.chestIdle.frameW+5)*index,25,40,43};
    return r;
}

void OpenChest(int index)
{
    if (ui.openedChests[index])
        return;
    if (!currEncounterRunning)
        return;
    if (!HasChest(currEncounterRunning,index, gameStats.timeTaken))
        return;
    if (!gameStats.gameWon)
        return;
    
    currEncounterRunning->bestChest = _MAX(currEncounterRunning->bestChest,currEncounterRunning->augment+index+1);
    ui.openedChests[index] = true;
    float goldToAdd = GetReward(currEncounterRunning, currEncounterRunning->augment, index);
    AddGold(goldToAdd);
    currEncounterRunning->bestProfited = _MAX(currEncounterRunning->bestProfited,players[0].gold-players[0].bankedGold);

    for (int i = 0; i < goldToAdd; i++)
    {
        Rect r = GetChestRect(index);

        int textX; int textY; int textW; int textH;
        al_get_text_dimensions(ui.tinyFont,players[0].goldText ? players[0].goldText : "",&textX,&textY,&textW,&textH);

        float x = r.x + RandRange(0,ui.chestIdle.frameW) + GetWidthSprite(&sprites[ui.gold_element_sprite_index])+2 + (textW/2.0f);
        float y = r.y + 16 + RandRange(0,10) + textH/2.0f;
        
        float goldPosX = GetChestRect(MAX_CHESTS-1).x;
        float goldPosY = 18;

        float d = dist(x,y,goldPosX,goldPosY);
        float speed = RandRange(60,120);

        float lifetime = d / speed;
        float angle = PointsToAngleRad(x,y,goldPosX,goldPosY);
        AddParticle(x,y,lifetime,speed,angle,COLOR_FRIENDLY);
    }

}
void DrawEndScreen(MouseState* mouseState, MouseState* mouseStateLastFrame, float dt)
{
    char* buffer = calloc(1,sizeof(char));

    DrawFilledRectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

    al_draw_text(ui.font,FRIENDLY,20,18,0,gameStats.gameWon == true ? "Victory" : "Defeat");
    al_draw_line(20,30,56,30,FRIENDLY,1);
    al_draw_line(17,73,239,73,FRIENDLY,1);

    for (int i = 0; i < MAX_CHESTS; i++)
    {
        Rect r = GetChestRect(i);

        bool hasChest = HasChest(currEncounterRunning,i, gameStats.timeTaken);
        if (!gameStats.gameWon)
            hasChest = false;

        if (ui.openedChests[i])
        {
            if (ui.currChestAnimation[i].spriteIndex_Animation != ui.chestOpen.spriteIndex_Animation)
            {
                PlaySoundStr("assets/audio/chest_open.wav",1,0,false);
                ui.currChestAnimation[i] = ui.chestOpen;
            }
        }
        else if (PointInRect(mouseState->screenX,mouseState->screenY,r) && hasChest)
        {
            if (mouseState->mouse.buttons & 1)
                OpenChest(i);
            if (PointInRect(mouseState->screenX,mouseState->screenY,r) && !PointInRect(mouseStateLastFrame->screenX,mouseStateLastFrame->screenY,r) && ui.currChestAnimation[i].spriteIndex_Animation != ui.chestWiggle.spriteIndex_Animation)
            {
                ui.currChestAnimation[i] = ui.chestWiggle;
                PlaySoundStr("assets/audio/chest_wiggle.wav",1,0,false);
            }
        }
        else 
        {
            ui.currChestAnimation[i] = ui.chestIdle;
        }

        ProcessAnimations(&ui.currChestAnimation[i], dt);
        
        DrawAnimation(&ui.currChestAnimation[i],18+(ui.chestIdle.frameW+5)*i,25,hasChest ? COLOR_FRIENDLY : COLOR_GROUND,false);
     
    }   
    Rect goldPos = GetChestRect(MAX_CHESTS-1);
    DrawGoldCount(FRIENDLY,ENEMY,goldPos.x,18);

    //Write the boss name and sprite
    al_draw_text(ui.font,ENEMY,17,76,0,currEncounterRunning->name);
    Sprite* sEnemy = &sprites[currEncounterRunning->spriteIndex];
    DrawSprite(sEnemy,17,91,0.5f,0.5f,0,ENEMY,false,false,false);

    //Write augment level and augment changes
    
    char* augmentStr = calloc(snprintf(NULL, 0, AUGMENT_NUMBER_FMT, currEncounterRunning->augment)+1,sizeof(char));
    char* percentDamageNumber = calloc(snprintf(NULL, 0, DAMAGE_NUMBER_FMT, (int)GetAugmentDamageBonus(100,currEncounterRunning->augment))+1,sizeof(char));
    char* percentHPNumber = calloc(snprintf(NULL, 0, HP_NUMBER_FMT, (int)GetAugmentHealthBonus(100,currEncounterRunning->augment))+1,sizeof(char));

    sprintf(augmentStr,AUGMENT_NUMBER_FMT,currEncounterRunning->augment);
    sprintf(percentDamageNumber,DAMAGE_NUMBER_FMT,(int)GetAugmentDamageBonus(100,currEncounterRunning->augment));
    sprintf(percentHPNumber,HP_NUMBER_FMT,(int)GetAugmentHealthBonus(100,currEncounterRunning->augment));

    al_draw_text(ui.font,ENEMY,90,85,0,augmentStr);
    al_draw_text(ui.font,ENEMY,90,106,0,percentDamageNumber);
    al_draw_text(ui.font,ENEMY,185,106,0,percentHPNumber);

    #ifdef _AUGMENTS_ENABLED
    for (int i = 0; i < MAX_AUGMENTS; i++)
        al_draw_text(ui.font,ENEMY,90,120+(10*i),0,GetAugmentDescription(currEncounterRunning->augments[i].augment));
    #endif
    free(augmentStr);
    free(percentDamageNumber);
    free(percentHPNumber);



    //Write time taken
    int hours = floorf(gameStats.timeTaken/(60.0f*60.0f));
    int minutes = floorf(gameStats.timeTaken/(60.0f) - hours*60.0f);    
    int seconds = floorf(gameStats.timeTaken - minutes*60.0f); 
    buffer = realloc(buffer,(strlen("Time: ")+log10(pow(2,sizeof(hours)*8+1))*3+4)*sizeof(char));
    sprintf(buffer,"Time: %i:%i:%i",hours,minutes,seconds);
    al_draw_text(ui.font,FRIENDLY,17,173,0,buffer);

    buffer = realloc(buffer,(strlen("Damage dealt: ")+log10(pow(2,sizeof(gameStats.damageDone)*8))+3)*sizeof(char));
    sprintf(buffer,"Damage dealt: %i",gameStats.damageDone);
    al_draw_text(ui.font,FRIENDLY,17,184,0,buffer);

    buffer = realloc(buffer,(strlen("Healing done: ")+log10(pow(2,sizeof(gameStats.healingDone)*8))+3) * sizeof(char));
    sprintf(buffer,"Healing done: %i",gameStats.healingDone);
    al_draw_text(ui.font,FRIENDLY,17,196,0,buffer);

    buffer = realloc(buffer,(strlen("Gold gained: ")+log10(pow(2,sizeof(players[0].gold)*8))+3) * sizeof(char));
    sprintf(buffer,"Gold gained: %i",(int)players[0].gold - players[0].bankedGold);
    al_draw_text(ui.font,FRIENDLY,17,208,0,buffer);

    buffer = realloc(buffer,(strlen("Best chest: ")+NumDigits(currEncounterRunning->bestChest+3) * sizeof(char)));
    sprintf(buffer,"Best chest: %i",currEncounterRunning->bestChest);
    al_draw_text(ui.font,FRIENDLY,140,208,0,buffer);


    UpdateButton(ui.endScreen_Back.x,ui.endScreen_Back.y,ui.endScreen_Back.w,ui.endScreen_Back.h,&ui.endScreen_Back,*mouseState,*mouseStateLastFrame);
    UpdateButton(ui.endScreen_Retry.x,ui.endScreen_Retry.y,ui.endScreen_Retry.w,ui.endScreen_Retry.h,&ui.endScreen_Retry,*mouseState,*mouseStateLastFrame);
    #ifdef _REPLAY
        UpdateButton(ui.endScreen_SaveReplay.x,ui.endScreen_SaveReplay.y,ui.endScreen_SaveReplay.w,ui.endScreen_SaveReplay.h,&ui.endScreen_SaveReplay,*mouseState,*mouseStateLastFrame);
    #endif
    DrawUIElement(&ui.endScreen_Back,ui.endScreen_Back.x,ui.endScreen_Back.y,mouseState,ui.endScreen_Back.bgColor,COLOR_FRIENDLY,false);
    DrawUIElement(&ui.endScreen_Retry,ui.endScreen_Retry.x,ui.endScreen_Retry.y,mouseState,ui.endScreen_Retry.bgColor,COLOR_FRIENDLY,false);
    #ifdef _REPLAY
        DrawUIElement(&ui.endScreen_SaveReplay,ui.endScreen_SaveReplay.x,ui.endScreen_SaveReplay.y,mouseState,ui.endScreen_SaveReplay.bgColor,COLOR_FRIENDLY,false);
    #endif
    int x = 88;
    int y = 162;
    if (encounterGoingTo)
    for (int i = 0; i < encounterGoingTo->numUnitsToSelect; i++)
    {
        if (toSpawn[i])
        {
            if (toSpawn[i]->spriteIndex >= 0 && toSpawn[i]->spriteIndex < numSprites)
            {
                Sprite* s = &sprites[toSpawn[i]->spriteIndex];
                DrawSprite(s,x,y,0.5f,0.5f,0,FRIENDLY,false,false,false);
                x += GetWidth(toSpawn[i])+5;
            }
        }
    }
    if (GetButtonIsClicked(&ui.endScreen_Back))
    {
        if (gameStats.gameWon && !GameStateIsTransition(&gameState))
        {
            for (int i = 0; i < MAX_CHESTS; i++)
            {
                OpenChest(i);
            }
            //players[0].bankedGold += players[0].gold;
        }

        SetGameStateToInMenu();
        //transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;
        RemoveReplay(&replay);
        if (players[0].gold >= 0)
            players[0].bankedGold += players[0].gold - players[0].bankedGold;

        
        
    }
    if (GetButtonIsClicked(&ui.endScreen_Retry))
    {
        if (gameStats.gameWon && !GameStateIsTransition(&gameState))
        {
            for (int i = 0; i < MAX_CHESTS; i++)
            {
                OpenChest(i);
            }
        }
        if (toSpawn)
            free(toSpawn);
        toSpawn = calloc(currEncounterRunning->numUnitsToSelect,sizeof(GameObject*));
        numUnitsToSpawn = currEncounterRunning->numUnitsToSelect;
        for (int i = 0; i < currEncounterRunning->numUnitsToSelect; i++)
        {
            toSpawn[i] = initialSelection[i];
        }

        transitioningTo = GAMESTATE_INGAME;
        RemoveReplay(&replay);
        
        if (players[0].gold >= 0)
            players[0].bankedGold += players[0].gold- players[0].bankedGold;

    }
    #ifdef _REPLAY
        if (GetButtonIsClicked(&ui.endScreen_SaveReplay))
        {
            ReplayToDisk(&replay);
            ui.endScreen_SaveReplay.enabled = false;
            RemoveReplay(&replay);
        }
    #endif

    free(buffer);
}
char* GetButtonText(UIElement* u)
{
    if (u->elementType != ELEMENT_BUTTON)
        return NULL;
    Button* b = (Button*)u->data;
    return b->description;
}
