#include <stdio.h>
#include <math.h>

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

Widget* Widgets_States[NUMGAMESTATES] = {0};
int numSprites_States[NUMGAMESTATES] = {0};

Chatbox* chatboxes = NULL;
int numChatboxes = 0;

Chatbox* chatboxShowing = NULL; 

UI ui = {0};
char* stackDrawBuffer = NULL;
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
void DrawPurchasingUnitsUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame)
{
    PurchasingUnitUI* purchaseUI = &ui.purchasingUnitUI;
    if (!purchaseUI->prefabs)
    {
        InitPurchasingUnitsUI();
    }
    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);
    GameObject* prefabDrawing = purchaseUI->prefabs[purchaseUI->currentIndex];
    ALLEGRO_COLOR* c = prefabDrawing->purchased ? &FRIENDLY : &GROUND;

    Sprite* s = &sprites[prefabDrawing->spriteIndex_PurchaseScreenSprite];

    int x = (_SCREEN_SIZE-GetWidthSprite(s)); 
    //if (!purchaseUI->isTransitionOut)
    float timer = easeInOutQuint(purchaseUI->transitionTimer);
    x = (_SCREEN_SIZE-GetWidthSprite(s))-(GetWidthSprite(s)*(-timer));

    int y = 0;
    DrawSprite(s,x,y,0,0,0,*c,false,false,false);


    int abilityY = 204;
    
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
            al_draw_filled_rectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,BG);
            //al_draw_rectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,FRIENDLY,1);
            //DrawSprite(&sprites[prefabDrawing->abilities[i].spriteIndex_Portrait],xPos,yPos,0,0,0,FRIENDLY,false,false,false);
            Rect r = (Rect){xPos,yPos,30,30};
            
            if (DrawAbilityPortraits(prefabDrawing,NULL,i,r,PointInRect(mouseState.screenX,mouseState.screenY,r),mouseState, c,false,true) && !mousedOver)
            {
                mousedOver = &prefabDrawing->abilities[i];
            } 

        }
    }
    
    int totalW = 126; 
    if (prefabDrawing->numAbilities >= 5)
    {
        abilityY = 170;
        
        for (int i = 0; i < 2; i++)
        {
            
            //int xPos = paragraphStartX + (totalW/2*(i+1)) - abilitySize*1.5f;
            int xPos = (paragraphStartX + (totalW/3*(i)) + abilitySize/2*(i+1) + (i*2)) + _SCREEN_SIZE;
            xPos -= (_SCREEN_SIZE)*(purchaseUI->transitionTimer+1);
            
            int yPos = abilityY;
            al_draw_filled_rectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,BG);
           // al_draw_rectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,FRIENDLY,1);
            //DrawSprite(&sprites[prefabDrawing->abilities[i].spriteIndex_Portrait],xPos,yPos,0,0,0,FRIENDLY,false,false,false);
            Rect r = (Rect){xPos,yPos,30,30};

            if (DrawAbilityPortraits(prefabDrawing,NULL,i,r,PointInRect(mouseState.screenX,mouseState.screenY,r),mouseState, c,false,true) && !mousedOver)
            {
                mousedOver = &prefabDrawing->abilities[i];
            } 

        }
        for (int i = 2; i < 5; i++)
        {
            int xPos = (paragraphStartX + (totalW/3*(i-2)) + ((i-2)*2)) + _SCREEN_SIZE;
            xPos -= (_SCREEN_SIZE)*(purchaseUI->transitionTimer+1);

            int yPos = abilityY+34;
            al_draw_filled_rectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,BG);
            //al_draw_rectangle(xPos,yPos,xPos+abilitySize,yPos+abilitySize,FRIENDLY,1);
            //DrawSprite(&sprites[prefabDrawing->abilities[i].spriteIndex_Portrait],xPos,yPos,0,0,0,FRIENDLY,false,false,false);
            Rect r = (Rect){xPos,yPos,30,30};
            if (DrawAbilityPortraits(prefabDrawing,NULL,i,r,PointInRect(mouseState.screenX,mouseState.screenY,r),mouseState, c,false,true) && !mousedOver)
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
    DrawSprite(&sprites[prefabDrawing->spriteIndex],spriteX,titleY,0,0,0,*c,false,false,false);

    al_draw_text(ui.font,*c,paragraphX + GetWidth(prefabDrawing)+2,titleY+GetHeight(prefabDrawing)/2, ALLEGRO_ALIGN_LEFT,prefabDrawing->name ? prefabDrawing->name : "");

    al_set_clipping_rectangle(paragraphX,paragraphY,paragraphWMax,clippingHeight);
    al_draw_multiline_text(ui.tinyFont,*c,paragraphX,paragraphY,paragraphWMax,8,ALLEGRO_ALIGN_LEFT,description ? description : "");
    al_reset_clipping_rectangle();
  
    if (purchaseUI->currentIndex == 0)
        purchaseUI->back.enabled = false;
    else
        purchaseUI->back.enabled = true;

    if (purchaseUI->currentIndex == purchaseUI->numPrefabs-1)
        purchaseUI->next.enabled = false;
    else
        purchaseUI->next.enabled = true;


    UpdateButton(purchaseUI->back.x,purchaseUI->back.y,&purchaseUI->back,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->back,purchaseUI->back.x,purchaseUI->back.y,&mouseState,ui.menuButton.bgColor);

    UpdateButton(purchaseUI->next.x,purchaseUI->next.y,&purchaseUI->next,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->next,purchaseUI->next.x,purchaseUI->next.y,&mouseState,ui.menuButton.bgColor);

    UpdateButton(purchaseUI->returnButton.x,purchaseUI->returnButton.y,&purchaseUI->returnButton,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->returnButton,purchaseUI->returnButton.x,purchaseUI->returnButton.y,&mouseState,ui.menuButton.bgColor);

    UpdateButton(purchaseUI->purchaseButton.x,purchaseUI->purchaseButton.y,&purchaseUI->purchaseButton,mouseState,mouseStateLastFrame);
    DrawUIElement(&purchaseUI->purchaseButton,purchaseUI->purchaseButton.x,purchaseUI->purchaseButton.y,&mouseState,ui.menuButton.bgColor);


    if (GetButtonIsClicked(&purchaseUI->back) && purchaseUI->currentIndex != 0)
    {
        purchaseUI->indexTransitioningTo--;
        purchaseUI->indexTransitioningTo = clamp(purchaseUI->indexTransitioningTo,0,purchaseUI->numPrefabs-1);

       // purchaseUI->transitionTimer = 0;
        purchaseUI->isTransitionOut = true;
        purchaseUI->isTransitioning = true;

    }


    if (GetButtonIsClicked(&purchaseUI->next) && purchaseUI->currentIndex != purchaseUI->numPrefabs-1)
    {
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
    if (GetButtonIsClicked(&purchaseUI->returnButton) )
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
        int hours = floor(gameStats.timeTaken/(60.0f*60.0f));
        int minutes = floor(gameStats.timeTaken/(60.0f) - hours*60);    
        int seconds = floor(gameStats.timeTaken - minutes*60); 

        size_t buffsiz = snprintf(NULL, 0, "%i:%i",minutes,seconds);
        char* buff = calloc(buffsiz+1,sizeof(char));
        sprintf(buff,"%i:%i",minutes,seconds);
        al_draw_text(ui.font,FRIENDLY,255-10,20,ALLEGRO_ALIGN_RIGHT,buff);
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
    gameState = GAMESTATE_INGAME;
    transitioningTo = GAMESTATE_INGAME;
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

    if (mouseStateLastFrame.mouse.buttons & 1)
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
    MouseState mouse = *mouseState;
    //ToScreenSpaceI(&mouse.x,&mouse.y);

    if (players[0].abilityHeld)
    {
        DrawCursor(&mouse, ui.cursorCastingIndex, false);
    }
    else if (mousedOver)
    {
        if (mousedOver->properties & OBJ_OWNED_BY && players[0].numUnitsSelected > 0)
            DrawCursor(&mouse, ui.cursorAttackIndex,false);
        else if (!(mousedOver->properties & OBJ_OWNED_BY))
            DrawCursor(&mouse, ui.cursorFriendlyIndex, false);
        else
            DrawCursor(&mouse, ui.cursorDefaultIndex, false);

    }
    else if (players[0].amoveSelected)
    {
        DrawCursor(&mouse, ui.cursorAttackIndex,false);
    }
    else 
    {
        DrawCursor(&mouse, ui.cursorDefaultIndex, false);
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

        UpdateButton(45,194,&ui.choosingUnits_Back,*mouseState,*mouseStateLastFrame);
        UpdateButton(109,194,&ui.choosingUnits_GO,*mouseState,*mouseStateLastFrame);
        UpdateButton(ui.choosingUnits_Hire.x,ui.choosingUnits_Hire.y,&ui.choosingUnits_Hire,*mouseState,*mouseStateLastFrame);

        if (GetButtonIsClicked(&ui.choosingUnits_Hire))
        {
            SetGameStateToPurchasingUnits();
        }

        if (numUnitsSelected==e->numUnitsToSelect)
        {
            ui.choosingUnits_GO.enabled = true;
        }
        else
        {
            ui.choosingUnits_GO.enabled = false;
        }
        DrawUIElement(&ui.choosingUnits_Back,ui.choosingUnits_Back.x,ui.choosingUnits_Back.y,mouseState,ui.choosingUnits_Back.bgColor);
        DrawUIElement(&ui.choosingUnits_GO,ui.choosingUnits_GO.x,ui.choosingUnits_GO.y,mouseState,ui.choosingUnits_Back.bgColor);
        DrawUIElement(&ui.choosingUnits_Hire,ui.choosingUnits_Hire.x,ui.choosingUnits_Hire.y,mouseState,ui.choosingUnits_Back.bgColor);


        char* number = calloc(log10(INT_MAX)*2+2,sizeof(char));
        sprintf(number,"%i/%i",numUnitsSelected,e->numUnitsToSelect);

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
    al_draw_filled_rectangle(r.x, r.y, r.x+r.w, r.y+r.h, al_premul_rgba(255, 255, 255,128));
   
}

void DrawReplayUI(Replay* r, MouseState* mouseState, MouseState* mouseStateLastFrame)
{
    al_draw_filled_rectangle(3,2,252,16,BG);
    al_draw_rectangle(3,2,252,16,BG,1);
    
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

    al_draw_rectangle(19,5,222,13,FRIENDLY,1);
    al_draw_filled_rectangle(position.x,position.y,position.x+position.w,position.h,FRIENDLY);

    DrawButton(&replayPlayButton,replayPlayButton.x,replayPlayButton.y,*mouseState,true,BG,true);
    UpdateButton(replayPlayButton.x,replayPlayButton.y,&replayPlayButton,*mouseState,*mouseStateLastFrame);

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
    DrawButton(&replayBackButton,replayBackButton.x,replayBackButton.y,*mouseState,true,BG,true);
    UpdateButton(replayBackButton.x,replayBackButton.y,&replayBackButton,*mouseState,*mouseStateLastFrame);
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
        if (GetButton(&ui.mainMenuPanel,"Load Replay"))
        {
            ChangeUIPanel(&ui.loadReplayPanel);       
            ClearPanelElements(&ui.loadReplayPanel);
            GenerateFileListButtons("replays/",&ui.loadReplayPanel);
        }
        if (GetButton(&ui.mainMenuPanel,"Return"))
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
        if (GetButton(&ui.mainMenuPanel,"Tutorial"))
        {
            GoTutorial();
        }
        if (GetButton(&ui.mainMenuPanel,"Options"))
        {
            ChangeUIPanel(&ui.videoOptionsPanel);
        }
        if (GetButton(&ui.mainMenuPanel,"Exit"))
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
    ui.animatePanel = UI_ANIMATE_OUT;

    if (ui.currentPanel == &ui.mainMenuPanel)
    {
        ui.currentPanel = to;   
        ui.panelShownPercent = 0;
        ui.animatePanel = UI_ANIMATE_IN;
    }
}
void DrawHealthUIElement(GameObject* selected)
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
    al_draw_filled_rectangle(startHPX, startY, startHPX+hpW,endY, FRIENDLY);
}

void DrawManaUIElement(GameObject* selected)
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
    al_draw_filled_rectangle(startManaX, startY, startManaX+manaW,endY, FRIENDLY);


}
Rect GetAbilityPortraitRect(int index)
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
bool DrawAbility(Ability* ability, int x, int y, ALLEGRO_COLOR color, MouseState* mouse)
{
    Sprite* s = &sprites[ability->spriteIndex_Portrait];
    int w = al_get_bitmap_width(s->sprite);
    int h = al_get_bitmap_height(s->sprite);

    Rect r = (Rect){x,y,w,h};
    if (w > 0 && h > 0)
        al_draw_rectangle(x,y,x+w,y+h,color,1);
    bool shouldInvert = false;

    if (PointInRect(mouse->screenX,mouse->screenY,r))
    {
        shouldInvert = true;
    }

    DrawSprite(s,x,y,0.5f,0.5f,0,color,shouldInvert,false,false);

    if (shouldInvert)
        return true;
    else
        return false;
}
bool DrawEffectPortrait(int x, int y, Effect* e, ALLEGRO_COLOR c, MouseState* mouseState)
{
    Rect r = (Rect){x,y,16,16};

    if (e->enabled)
    {
        if (e->spriteIndex_Portrait > 0)
            DrawSprite(&sprites[e->spriteIndex_Portrait],x,y,0.5f,0.5f,0,FRIENDLY,false,false,false);
        else
            al_draw_filled_rectangle(r.x,r.y,r.x+r.w,r.y+r.h,FRIENDLY);
        if (e->canStack)
        {
            if (e->stacks > 1)
            {
                size_t buffsize = snprintf(NULL, 0, "%i", e->stacks);
                char* stacks = calloc(buffsize+1,sizeof(char));
                sprintf(stacks,"%i",e->stacks);
                al_draw_filled_rectangle(x,y,x+al_get_text_width(ui.tinyFont,stacks),y+al_get_font_line_height(ui.tinyFont),BG);
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
        col = c;
    else
        col = (ObjectHasManaToCast(selected,a) && !AbilityIsOnCooldown(a) && percent >= 1.0f) ? &FRIENDLY : &GROUND;
    DrawSpriteRegion(s,0,0,w,h,r.x,r.y,*col,keydown);

    //Draw stacks counter
    if (a->maxStacks > 1 && !ignoreStacks)
    {
        int sizeStackCounter = 8;
        int xStackCounter = r.x+r.w-sizeStackCounter;
        int yStackCounter = r.y+r.h-sizeStackCounter;

        al_draw_filled_rectangle(xStackCounter,yStackCounter,xStackCounter+sizeStackCounter,yStackCounter+sizeStackCounter,BG);
        al_draw_rectangle(xStackCounter,yStackCounter,xStackCounter+sizeStackCounter,yStackCounter+sizeStackCounter,FRIENDLY,1);

        const int maxchars = ceil(log10(pow(2,sizeof(a->stacks)*8)))+2;
        if (!stackDrawBuffer)
            stackDrawBuffer = calloc(maxchars,sizeof(char));
        memset(stackDrawBuffer,0,maxchars*sizeof(char));
        sprintf(stackDrawBuffer,"%i",a->stacks);
        al_draw_text(ui.tinyFont, FRIENDLY, xStackCounter+sizeStackCounter/2, yStackCounter+sizeStackCounter/2-al_get_font_line_height(ui.tinyFont)/4, ALLEGRO_ALIGN_CENTRE, stackDrawBuffer);

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


    if (key && drawKey)
    {
        int keyW = al_get_text_width(ui.tinyFont,key)+4;
        int keyH = al_get_font_line_height(ui.tinyFont)+4;

        al_draw_filled_rectangle(r.x,r.y,r.x+keyW,r.y+keyH,BG);
        al_draw_rectangle(r.x,r.y,r.x+keyW,r.y+keyH,FRIENDLY,1);

        al_draw_text(ui.tinyFont, FRIENDLY, r.x+keyW/2,r.y+keyH/2-1, ALLEGRO_ALIGN_CENTRE, key);

    }

    if (PointInRect(mouseState.screenX,mouseState.screenY,r))
    {
        return true;
    }
    return false;
}
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseState)
{
    al_draw_filled_rectangle(0, UI_START_Y, _SCREEN_SIZE, _SCREEN_SIZE, BG);

    GameObject* selected = players[0].selection[players[0].indexSelectedUnit];
    Sprite* s =  &sprites[ui.panel_sprite_index];

    Sprite* health = &sprites[ui.health_element_sprite_index];
    if (selected)
        health = selected->usesMana ? &sprites[ui.health_and_mana_element_sprite_index] : &sprites[ui.health_element_sprite_index];
    if (selected)
    {
        if (selected->numAbilities <= 4)
            s = &sprites[ui.panel_sprite_index];
        else if (selected->numAbilities >= 5)
            s = &sprites[ui.panel_5_abilities_sprite_index];
    }
    if (!s) return;
    DrawSprite(s,1,UI_START_Y+1,0.5f,0.5f,0,FRIENDLY,false,false,false);
    DrawSprite(health,1,UI_START_Y+1,0.5f,0.5f,0,FRIENDLY,false,false,false);

    if (selected)
    {
        DrawHealthUIElement(selected);
        DrawManaUIElement(selected);
        Ability* heldAbility = players[0].abilityHeld;
        

        if (DrawAbilityPortraits(selected,heldAbility,0,GetAbilityPortraitRect(0),IsBindDown(keyState,currSettings.keymap.key_Q),*mouseState,false,true,false))
        {
            if (selected->abilities[0].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[0].description,100,ui.font,UI_PADDING);
                int x = 33 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[0].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,1,GetAbilityPortraitRect(1),IsBindDown(keyState,currSettings.keymap.key_W),*mouseState,false,true,false))
        {
            if (selected->abilities[1].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[1].description,100,ui.font,UI_PADDING);
                int x = 65 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[1].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,2,GetAbilityPortraitRect(2),IsBindDown(keyState,currSettings.keymap.key_E),*mouseState,false,true,false))
        {
            if (selected->abilities[2].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[2].description,100,ui.font,UI_PADDING);
                int x = 97 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[2].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);

            }

        }
        if (DrawAbilityPortraits(selected,heldAbility,3,GetAbilityPortraitRect(3),IsBindDown(keyState,currSettings.keymap.key_R),*mouseState,false,true,false))
        {
            if (selected->abilities[3].description)
            {
                int h = GetDescriptionBoxH(selected->abilities[3].description,100,ui.font,UI_PADDING);
                int x = 129 + ceil(UI_PADDING/2.0f);
                int y = 221 - h - 3;
                DrawDescriptionBox(selected->abilities[3].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
            }
        }
        if (selected->numAbilities >= 5)
        {
            if (DrawAbilityPortraits(selected,heldAbility,4,GetAbilityPortraitRect(4),IsBindDown(keyState,currSettings.keymap.key_F),*mouseState,false,true,false))
            {
                if (selected->abilities[4].description)
                {
                    int h = GetDescriptionBoxH(selected->abilities[4].description,100,ui.font,UI_PADDING);
                    int x = 140 + ceil(UI_PADDING/2.0f);
                    int y = 221 - h - 3;
                    DrawDescriptionBox(selected->abilities[4].description, 5, ui.font,ui.boldFont, x,y,100,0,FRIENDLY,true);
                }
            }

        }

        Effect* mousedOver = NULL; 
        //4-ability UI
        if (selected->numAbilities <= 4)
        {

            mousedOver = DrawEffectPortrait(162,UI_ABILITY_START_Y,&selected->effects[0],FRIENDLY,mouseState) == true ? &selected->effects[0] : mousedOver;
            mousedOver = DrawEffectPortrait(178,UI_ABILITY_START_Y,&selected->effects[1],FRIENDLY,mouseState)== true ? &selected->effects[1] : mousedOver;
            mousedOver = DrawEffectPortrait(194,UI_ABILITY_START_Y,&selected->effects[2],FRIENDLY,mouseState)== true ? &selected->effects[2] : mousedOver;
            mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y,&selected->effects[3],FRIENDLY,mouseState)== true ? &selected->effects[3] : mousedOver;
            mousedOver = DrawEffectPortrait(225,UI_ABILITY_START_Y,&selected->effects[4],FRIENDLY,mouseState)== true ? &selected->effects[4] : mousedOver;
            mousedOver = DrawEffectPortrait(241,UI_ABILITY_START_Y,&selected->effects[5],FRIENDLY,mouseState)== true ? &selected->effects[5] : mousedOver;

            mousedOver = DrawEffectPortrait(162,UI_ABILITY_START_Y+18,&selected->effects[6],FRIENDLY,mouseState)== true ? &selected->effects[6] : mousedOver;
            mousedOver = DrawEffectPortrait(178,UI_ABILITY_START_Y+18,&selected->effects[7],FRIENDLY,mouseState)== true ? &selected->effects[7] : mousedOver;
            mousedOver = DrawEffectPortrait(194,UI_ABILITY_START_Y+18,&selected->effects[8],FRIENDLY,mouseState)== true ? &selected->effects[8] : mousedOver;
            mousedOver = DrawEffectPortrait(209,UI_ABILITY_START_Y+18,&selected->effects[9],FRIENDLY,mouseState)== true ? &selected->effects[9] : mousedOver;
            mousedOver = DrawEffectPortrait(225,UI_ABILITY_START_Y+18,&selected->effects[10],FRIENDLY,mouseState)== true ? &selected->effects[10] : mousedOver;
            mousedOver = DrawEffectPortrait(241,UI_ABILITY_START_Y+18,&selected->effects[11],FRIENDLY,mouseState)== true ? &selected->effects[11] : mousedOver;
        }
        else //5-ability UI
        {
            mousedOver = DrawEffectPortrait(195,UI_ABILITY_START_Y,&selected->effects[0],FRIENDLY,mouseState) == true ? &selected->effects[0] : mousedOver;
            mousedOver = DrawEffectPortrait(211,UI_ABILITY_START_Y,&selected->effects[1],FRIENDLY,mouseState)== true ? &selected->effects[1] : mousedOver;
            mousedOver = DrawEffectPortrait(226,UI_ABILITY_START_Y,&selected->effects[2],FRIENDLY,mouseState)== true ? &selected->effects[2] : mousedOver;
            mousedOver = DrawEffectPortrait(240,UI_ABILITY_START_Y,&selected->effects[3],FRIENDLY,mouseState)== true ? &selected->effects[3] : mousedOver;
            
            
            mousedOver = DrawEffectPortrait(195,UI_ABILITY_START_Y+18,&selected->effects[4],FRIENDLY,mouseState)== true ? &selected->effects[4] : mousedOver;
            mousedOver = DrawEffectPortrait(211,UI_ABILITY_START_Y+18,&selected->effects[5],FRIENDLY,mouseState)== true ? &selected->effects[5] : mousedOver;
            mousedOver = DrawEffectPortrait(226,UI_ABILITY_START_Y+18,&selected->effects[6],FRIENDLY,mouseState)== true ? &selected->effects[6] : mousedOver;
            mousedOver = DrawEffectPortrait(240,UI_ABILITY_START_Y+18,&selected->effects[7],FRIENDLY,mouseState)== true ? &selected->effects[7] : mousedOver;
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

}
void DrawAllLevelSelects(MouseState* mouseState, MouseState* mouseStateLastFrame)
{
    for (int i = 0; i < numEncounters; i++)
    {
        int offset =  _SCREEN_SIZE*i - encounterOffset;//i*_SCREEN_SIZE - selectedEncounterIndex*_SCREEN_SIZE;
        DrawLevelSelect(mouseState,mouseStateLastFrame,i,offset);
    }
}
void DrawLevelSelect(MouseState* mouseState, MouseState* mouseStateLastFrame, int index, int offsetX)
{
    Encounter* e = encounters[index];

    char* augmentStr = calloc(NumDigits(e->augment)+1,sizeof(char));
    sprintf(augmentStr,"%i",e->augment);

    al_draw_text(ui.font,FRIENDLY,16+offsetX,18,0,"Augment");
    al_draw_text(ui.font,FRIENDLY,84+offsetX,18,0,augmentStr);

    int amtDamagePercent = GetAugmentDamageBonus(100,e->augment);
    char* dmgString = calloc(1 + NumDigits(amtDamagePercent) + 1 +strlen("Damage ") + 1,sizeof(char));
    sprintf(dmgString,"+%i%% Damage",amtDamagePercent);
    al_draw_text(ui.tinyFont,FRIENDLY,16+offsetX,40,0,dmgString);


    int amtHealthPercent = GetAugmentHealthBonus(100,e->augment);
    char* healthString = calloc(1 + NumDigits(amtHealthPercent) + 1 +strlen("HP ") + 1,sizeof(char));
    sprintf(healthString,"+%i%% HP",amtHealthPercent);
    al_draw_text(ui.tinyFont,FRIENDLY,92+offsetX,40,0,healthString);

    int amtAbilityPercent = GetAugmentAbilityDamage(100,e->augment);
    char* abilityDmg = calloc(1 + NumDigits(amtAbilityPercent) + 1 +strlen("Ability Damage ") + 1,sizeof(char));
    sprintf(abilityDmg,"+%i%% Ability Damage",amtAbilityPercent);
    al_draw_text(ui.tinyFont,FRIENDLY,138+offsetX,40,0,abilityDmg);
    

    int row = 50;
    int column = 16;

    UpdateButton(column+offsetX,row,&e->encounter_RerollAugments,*mouseState,*mouseStateLastFrame);
    DrawUIElement(&e->encounter_RerollAugments,column+offsetX,row,mouseState,e->encounter_RerollAugments.bgColor);
    column += 32;
    if (GetButtonIsClicked(&e->encounter_RerollAugments))
    {
        SetEncounterRandAugments(e);
    }

    char* augmentDescriptionToDraw = NULL;
    for (int i = 0; i < MAX_AUGMENTS; i++)
    {
        Augment* a = &e->augments[i];
        char* description = GetAugmentDescription(a->augment);
        ALLEGRO_COLOR* color = GetAugmentDescriptionColor(a);

        if (DrawAugmentPortrait(a,column+offsetX,row, mouseState))
            augmentDescriptionToDraw = description;
        //al_draw_text(ui.tinyFont,*color,column+offsetX,row,0,description);
        column += 24;
        //column += al_get_text_width(ui.tinyFont,description)+5;
        if (i+1 == MAX_AUGMENTS/2)
        {
          //  row += al_get_font_line_height(ui.tinyFont)+2;
            //column = 16;
        }
    }


    free(dmgString);
    free(healthString);


    int augmentX = 96;
    if (e->augment <= 0)
        e->augment = 1; 
    for (int i = 0; i < e->difficultyUnlocked; i++)
    {
        Rect drawRect = (Rect){augmentX+offsetX,20,GetWidthSprite(&sprites[ui.augmentIconIndex]),GetHeightSprite(&sprites[ui.augmentIconIndex])};
        DrawSprite(&sprites[ui.augmentIconIndex],drawRect.x,drawRect.y,0.5f,0.5f,0,i < e->augment ? FRIENDLY : GROUND,false,false,false);
        augmentX += drawRect.w+3;

        if (mouseStateLastFrame->mouse.buttons & 1 && !(mouseState->mouse.buttons & 1))
        {
            if (PointInRect(mouseState->screenX,mouseState->screenY,drawRect))
            {
                e->augment = i+1;
                SetEncounterRandAugments(currEncounterRunning);
            }
        }
    }
    e->encounter_PurchaseAugment.x = augmentX + 3 + offsetX;
    e->encounter_PurchaseAugment.y = 22 - (e->encounter_PurchaseAugment.h/2);
    int purchaseCost = GetAugmentCost(e, e->difficultyUnlocked+1);
    if (e->bestProfited > 0)
    {
        char* buttonText = calloc(NumDigits(purchaseCost),sizeof(char));
        sprintf(buttonText,"%i",purchaseCost);
        ChangeButtonText((Button*)(e->encounter_PurchaseAugment.data), buttonText);
        free(buttonText);
    }
    else
    {
        ChangeButtonText((Button*)(e->encounter_PurchaseAugment.data), "Locked");
    }
    if (purchaseCost > players[0].bankedGold || e->bestProfited <= 0)
    {
        e->encounter_PurchaseAugment.enabled = false;
    }
    else
    {
        e->encounter_PurchaseAugment.enabled = true;
    }
    UpdateButton(e->encounter_PurchaseAugment.x,e->encounter_PurchaseAugment.y,&e->encounter_PurchaseAugment,*mouseState,*mouseStateLastFrame);
    DrawUIElement(&e->encounter_PurchaseAugment,e->encounter_PurchaseAugment.x,e->encounter_PurchaseAugment.y,mouseState,COLOR_BG);

    if (GetButtonIsClicked(&e->encounter_PurchaseAugment))
    {
        AddGold(-purchaseCost);
        players[0].bankedGold -= purchaseCost;
        e->difficultyUnlocked++;
        e->encounter_PurchaseAugment.enabled = false;
        Save("_save.save");

    }


    free(augmentStr);

    al_draw_line(10+offsetX,73,246,73,FRIENDLY,1);
    
    al_draw_text(ui.font,FRIENDLY,16+offsetX,81,0,"Wyrm");
    DrawSprite(&sprites[e->spriteIndex],17+offsetX,102,0.5f,0.5f,0,ENEMY,false,false,false);

    Ability* mousedOver = NULL;
    mousedOver = DrawAbility(&e->abilities[0], 96+offsetX, 80, ENEMY, mouseState) == true ? &e->abilities[0] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[1], 136+offsetX, 80, ENEMY, mouseState) == true ? &e->abilities[1] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[2], 175+offsetX, 80, ENEMY, mouseState) == true ? &e->abilities[2] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[3], 214+offsetX, 80, ENEMY, mouseState) == true ? &e->abilities[3] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[4], 96+offsetX, 120, ENEMY, mouseState) == true ? &e->abilities[4] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[5], 136+offsetX, 120, ENEMY, mouseState) == true ? &e->abilities[5] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[6], 175+offsetX, 120, ENEMY, mouseState) == true ? &e->abilities[6] : mousedOver;
    mousedOver = DrawAbility(&e->abilities[7], 214+offsetX, 120, ENEMY, mouseState) == true ? &e->abilities[7] : mousedOver;


    char* descriptionToDraw;
    if (mousedOver)
    {
        descriptionToDraw = mousedOver->description;
    }
    else
    {
        descriptionToDraw = e->description;
    }
    if (augmentDescriptionToDraw)
        descriptionToDraw = augmentDescriptionToDraw;
    if (descriptionToDraw)
    {
        DrawDescriptionBox(descriptionToDraw,2,ui.font,ui.boldFont,16+offsetX,170,224,41,ENEMY,true);

    }
    ui.panelShownPercent=1.0f;
    //DrawPanel(&ui.encounter_scroll,mouseState);



    UpdateButton(16+offsetX,224,&e->encounter_ButtonLeft,*mouseState,*mouseStateLastFrame);
    UpdateButton(80+offsetX,224,&e->encounter_ButtonConfirm,*mouseState,*mouseStateLastFrame);
    UpdateButton(192+offsetX,224,&e->encounter_ButtonRight,*mouseState,*mouseStateLastFrame);

    if (index > 0 && (index > 0 && (!encounters[index-1]->encounterShouldBeSkipped && encounters[index-1]->unlocked)))
    {
        e->encounter_ButtonLeft.enabled = true;
    }
    else {
        e->encounter_ButtonLeft.enabled = false;
    }
    if (index+1 < numEncounters)
    {
        e->encounter_ButtonRight.enabled = true;
        if (encounters[index+1]->encounterShouldBeSkipped || !encounters[index+1]->unlocked)
            e->encounter_ButtonRight.enabled = false;
    }
    else {
        e->encounter_ButtonRight.enabled = false;
    }

    DrawUIElement(&e->encounter_ButtonLeft,14+offsetX,224,mouseState,e->encounter_ButtonLeft.bgColor);
    DrawUIElement(&e->encounter_ButtonConfirm,80+offsetX,224,mouseState,e->encounter_ButtonConfirm.bgColor);
    DrawUIElement(&e->encounter_ButtonRight,194+offsetX,224,mouseState,e->encounter_ButtonRight.bgColor);


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
    
    DrawGoldCount();
}
void AddElement(Panel* p, UIElement* u)
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
}
void AddKeyInput(Panel* p, char* name, char* description, int x, int y, int w, int h, int maxchars, KEY* mapTo)
{
    KeyInput* t = calloc(1,sizeof(KeyInput));
    t->allowsInteraction = true;
    t->text = calloc(2,sizeof(char));
    t->mappedTo = (int*)mapTo;

    UIElement u = {0};
    u.data = (void*)t;
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
void AddButton(Panel* p, char* name, char* description, int x, int y, int w, int h, bool shouldDrawLine)
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

    AddElement(p,&u);
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
            s->clicked = true;
        }
    }
    if (!(mouseState.mouse.buttons & 1))
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
void InitButton(UIElement* u, char* name, char* description, int x, int y, int w, int h, int sprite)
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
void AddText(Panel* p,int x, int y, char* name, char* description)
{
    UIElement u = {0};
    UI_Text* t = calloc(1,sizeof(UI_Text));

    t->str = calloc(strlen(description)+1,sizeof(char));
    strcpy(t->str,description);
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
    al_draw_rectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
    float w = u->w * *s->value;
    al_draw_filled_rectangle(x,y,x+w,y+u->h,FRIENDLY);
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
        al_draw_filled_rectangle(x,y,x+w,y+h,FRIENDLY);
        al_draw_rectangle(x,y,x+w,y+h,BG,1);
    }
    else
    {
        al_draw_filled_rectangle(x,y,x+w,y+h,bgColor);
        if (isActive)
        {
            al_draw_rectangle(x,y,x+w,y+h,FRIENDLY,1);
        }
        else
        {
            DrawOutlinedRect_Dithered(checkbox,FRIENDLY);
        }
    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,checkbox))
    {
        Color col = *c->activated ? COLOR_BG : COLOR_FRIENDLY;
        al_draw_rectangle(x+2,y+2,x+w-2,y+h-2,GetColor(col,0),1);
    }
}
void AddCheckbox(Panel* p, int x, int y, int w, int h, char* name, bool* activated)
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
    
    AddElement(p,&u);
}
void AddSlider(Panel* p, int x, int y, int w, int h, char* name, float filled, float* v)
{
    Slider* s = calloc(1,sizeof(Slider));
    UIElement u = {0};
    u.x = x;
    u.y = y;
    u.w = w;
    u.h = h;
    u.enabled = true;
    s->value = v;
    *v = filled;
    u.name = calloc(strlen(name)+1,sizeof(char));
    strcpy(u.name,name);
    u.data = (void*)s;
    u.elementType = ELEMENT_SLIDER;
    u.bgColor = COLOR_BG;

    AddElement(p,&u);
}
void AddPulldownMenu(Panel* panel, int x, int y, int w, int h, char* name, int startIndex, int numElements, ...)
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
    p->clicked = false;
    p->elements = list;
    p->selectedIndex = startIndex;
    p->numElements = numElements;

    u.data = (void*)p;
    u.elementType = ELEMENT_PULLDOWN;
    u.bgColor = COLOR_BG;

    AddElement(panel,&u);

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
            al_draw_filled_rectangle(x,y2,x+w,y2+h,bgColor);
            al_draw_rectangle(x,y2,x+w,y2+h,FRIENDLY,1);
            al_draw_text(ui.font,FRIENDLY,x+w/2,y2+h/2 - al_get_font_line_height(ui.font)/2.0,ALLEGRO_ALIGN_CENTRE,p->elements[i]);
            Rect r2 = (Rect){x,y2,w,h};
            if (PointInRect(mouseState->screenX,mouseState->screenY,r2))
            {
                al_draw_rectangle(x+2,y2+2,x+w-2,y2+h-2,FRIENDLY,1);
            }
            y2 += h;

        }
        //al_draw_text(ui.font,BG,x+w/2,y+h/2 - al_get_font_line_height(font)/2.0,ALLEGRO_ALIGN_CENTRE,b->description);
    }
    if (!p->clicked)
    {
        al_draw_filled_rectangle(x,y,x+w,y+h,bgColor);
        al_draw_rectangle(x,y,x+w,y+h,FRIENDLY,1);
        al_draw_text(ui.font,FRIENDLY,x+w/2,y+h/2 - al_get_font_line_height(ui.font)/2.0,ALLEGRO_ALIGN_CENTRE,p->elements[p->selectedIndex]);
        if (PointInRect(mouseState->screenX,mouseState->screenY,r) && isActive)
        {
            al_draw_rectangle(x+2,y+2,x+w-2,y+h-2,FRIENDLY,1);
        }

    }
    al_set_clipping_rectangle(clip.x,clip.y,clip.w,clip.h);
}
void DrawScrollbar(Panel* p)
{
    int w = SCROLLBARW;
    al_draw_rectangle(p->x+p->w-w,p->y,p->x+p->w,p->x+p->w,FRIENDLY,1);


    float yPos = p->y + p->scrollPercent * p->h;
    al_draw_filled_rectangle(p->x+p->w-w, yPos, p->x+p->w, yPos+14, FRIENDLY);

}
void UpdateScrollbar(Panel* p, MouseState* mouseState,MouseState* mouseStateLastFrame)
{
    int w = SCROLLBARW;
    Rect scrollbar = (Rect){p->x+p->w-w,p->y,w,p->h};
    if (mouseState->mouse.z != mouseStateLastFrame->mouse.z)
    {
        float v = mouseState->mouse.z - mouseStateLastFrame->mouse.z;
        p->scrollPercent -= v/6.0f;
        p->scrollPercent = clamp(p->scrollPercent,0,1);
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
void UpdateLanternWidget(Widget* self, float dt)
{
    self->timer += dt;
    self->rotation = sin(self->timer) * 0.1f;
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
        if (b->description) free(b->description);
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
        free(t->text);
    }
    if (u->name)
        free(u->name);
    if (u->data)
        free(u->data);

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
    ui.mainMenuPanel = CreatePanel(29,70,160,144,15,false);
    AddButton(&ui.mainMenuPanel,"Return","Start Game",0,16,96,16,false);
    AddButton(&ui.mainMenuPanel,"Tutorial","Tutorial",0,32+(padding),96,16,false);
    AddButton(&ui.mainMenuPanel,"Load Replay","Load Replay",0,48+(padding*2),96,16,false);
    AddButton(&ui.mainMenuPanel,"Options","Options",0,64+(padding*3),96,16,false);
    AddButton(&ui.mainMenuPanel,"Exit","Exit Game",0,80+(padding*4),96,16,false);

}
void InitPausePanel()
{
    ui.pauseMenuPanel = CreatePanel(48,48,160,160,15,true);
    AddButton(&ui.pauseMenuPanel,"Return","Return",33,17,96,16,true);
    AddButton(&ui.pauseMenuPanel,"Options","Options",33,49,96,16,true);
    AddButton(&ui.pauseMenuPanel,"Exit","Exit Game",33,81,96,16,true);
}
void InitVideoOptionsPanel()
{
    ui.videoOptionsPanel = CreatePanel(48,48,180,160,15,true);
    AddText(&ui.videoOptionsPanel,33,41,"Tag_RenderScale","RenderScale");
    AddText(&ui.videoOptionsPanel,132,43,"RenderScale","2x");
    
    char* renderScale = calloc(NumDigits(_RENDERSIZE)+2,sizeof(char));
    sprintf(renderScale,"%ix",_RENDERSIZE);
    SetUITextStr(GetUIElement(&ui.videoOptionsPanel,"RenderScale")->data,renderScale);
    free(renderScale);
    
    AddButton(&ui.videoOptionsPanel,"RenderScale+","+",132,29,11,11,true);
    AddButton(&ui.videoOptionsPanel,"RenderScale-","-",132,53,11,11,true);
    AddText(&ui.videoOptionsPanel,33,73,"Tag_Particles","Particles");
    AddCheckbox(&ui.videoOptionsPanel,131,72,13,13,"EnableParticles",&currSettings.particlesEnabled);
    AddText(&ui.videoOptionsPanel,33,105,"Display\nHealth Bar","Display\nHealth Bar");
    AddPulldownMenu(&ui.videoOptionsPanel,97,108,48,13,"HealthBarDisplay",0,3,"Always","Selected","Never");
    
    AddText(&ui.videoOptionsPanel,33,132,"Display\nLight Effect","Display\nLight Effect");
    AddCheckbox(&ui.videoOptionsPanel,132,132,13,13,"EnableLightEffect",&currSettings.lightEffectEnabled);

    AddText(&ui.videoOptionsPanel,33,156,"Display Timer","Display Timer");
    AddCheckbox(&ui.videoOptionsPanel,132,156,11,11,"DisplayTimerButton",&currSettings.displayTimer);

    AddText(&ui.videoOptionsPanel,33,180,"Label_TargetFPS","Target FPS");
    AddPulldownMenu(&ui.videoOptionsPanel,97,180,48,13,"Target FPS",currSettings.targetFPS,5,"30","60","90","144","240");


    InitButton(&ui.videoOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));

    InitButton(&ui.videoOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/video_tab_icon.png",true));

    ui.videoOptionsPanel.back = &ui.mainMenuPanel;

    Pulldown* healthbar = (Pulldown*)GetUIElement(&ui.videoOptionsPanel,"HealthBarDisplay")->data;
    currSettings.displayHealthBar = healthbar->selectedIndex;


}
void InitAudioOptionsPanel()
{
    ui.audioOptionsPanel = CreatePanel(48,48,180,160,15,true);
    AddText(&ui.audioOptionsPanel,33,41,"Tag_MasterVolume","Master Volume");
    AddSlider(&ui.audioOptionsPanel,34,52,110,10,"MasterVolume",currSettings.masterVolume,&currSettings.masterVolume);
 
    AddText(&ui.audioOptionsPanel,33,90,"Tag_SFXVolume","SFX Volume");
    AddSlider(&ui.audioOptionsPanel,34,100,110,10,"SfxVolume",currSettings.sfxVolume,&currSettings.sfxVolume);

    AddText(&ui.audioOptionsPanel,33,131,"Tag_MusicVolume","Music Volume");
    AddSlider(&ui.audioOptionsPanel,34,141,110,10,"MusicVolume",currSettings.musicVolume,&currSettings.musicVolume);

    InitButton(&ui.audioOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.audioOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/audio_tab_icon.png",true));
    ui.audioOptionsPanel.back = &ui.mainMenuPanel;

}
void InitAccessibilityOptionsPanel()
{
    ui.accessibilityOptionsPanel = CreatePanel(48,48,180,160,15,true);
    //AddButton(&ui.audioOptionsPanel,"MasterVolume", "MasterVolume", 132,29,96,16,true);
    //AddButton(&ui.audioOptionsPanel,"Music Volume","Music Volume",132,29,96,16,true);

    AddText(&ui.accessibilityOptionsPanel,33,41,"Tag_Slowdown","Percent slowdown");
    AddSlider(&ui.accessibilityOptionsPanel,34,52,110,10,"Slider_Slowdown",currSettings.slowdownPercent,&currSettings.slowdownPercent);


    InitButton(&ui.accessibilityOptionsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.accessibilityOptionsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/accessiblity_tab_icon.png",true));
    ui.accessibilityOptionsPanel.back = &ui.mainMenuPanel;


}
void InitChoosingUnitButtons()
{
    InitButton(&ui.choosingUnits_Back,"Back","Back",45,194,48,16,0);
    InitButton(&ui.choosingUnits_GO,"Adventure","Adventure",109,194,96,16,0);
    InitButton(&ui.choosingUnits_Hire,"Hire","Hire",168,4,40,11,0);

    
}
void InitEndScreen()
{
    //endScreen
    InitButton(&ui.endScreen_Back,"Back","Back",10,224,72,16,0);
    InitButton(&ui.endScreen_Retry,"Retry","Retry",91,224,72,16,0);
    InitButton(&ui.endScreen_SaveReplay,"Save Replay","Save Replay",172,224,72,16,0);

}
void InitGameUI()
{
    InitButton(&ui.menuButton,"Menu","Menu",213,4,40,11,0);

    int w = 80;
    InitButton(&ui.nextLevelButton,"Descend","Descend",_SCREEN_SIZE/2-w/2,10,w,10,0);
    ui.nextLevelButton.enabled = false;
    ui.nextLevelButton.bgColor =  COLOR__TRANSPARENT; 

    ui.gold_element_sprite_index = LoadSprite("assets/ui/gold.png",false);

}
void InitControlsPanel()
{
    ui.controlsPanel = CreatePanel(48,48,180,160,15,true);

    int xText = 16;


    int y = 16;
    int w = 24;
    int h = 16;
    
    int xButtons = 160-w-padding*4;

    
    AddKeyInput(&ui.controlsPanel,"Q","Q",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Q.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Q2","Q2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Q.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"First Ability","First Ability");


    AddKeyInput(&ui.controlsPanel,"W","W",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_W.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"W2","W2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_W.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Second Ability","Second Ability");

    AddKeyInput(&ui.controlsPanel,"E","E",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_E.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"E2","E2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_E.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Third Ability","Third Ability");

    AddKeyInput(&ui.controlsPanel,"R","R",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_R.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"R2","R2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_R.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Fourth Ability","Fourth Ability");

    AddKeyInput(&ui.controlsPanel,"F","F",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_F.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"F2","F2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_F.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Fifth Ability","Fifth Ability");


    AddKeyInput(&ui.controlsPanel,"AMove","AMove",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_AMove.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"AMove2","AMove2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_AMove.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Attack Move","Attack Move");

    AddKeyInput(&ui.controlsPanel,"Add Selection","Add Selection",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Shift.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Add Selection2","Add Selection2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Shift.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Add Selection","Add Selection");

    AddKeyInput(&ui.controlsPanel,"Shift Selected","Shift Selected",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Tab.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Shift Selected2","Shift Selected2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Tab.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Shift Selected","Shift Selected");

    AddKeyInput(&ui.controlsPanel,"Set Group","Set Group",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Ctrl.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Set Group2","Set Group2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Ctrl.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Set Group","Set Group");

    AddKeyInput(&ui.controlsPanel,"Show Map","Show Map",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ShowMap.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Show Map2","Show Map2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ShowMap.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Show Map","Show Map");

    AddKeyInput(&ui.controlsPanel,"Pause","Pause",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Pause.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pause2","Pause2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Pause.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pause","Pause");

    AddKeyInput(&ui.controlsPanel,"Cancel Cast","Cancel Cast",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_Cancel.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Cancel Cast","Cancel Cast",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_Cancel.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Cancel Cast","Cancel Cast");

    AddKeyInput(&ui.controlsPanel,"Pan Left","Pan Left",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanLeft.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Left2","Pan Left2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanLeft.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Left","Pan Left");

    AddKeyInput(&ui.controlsPanel,"Pan Right","Pan Right",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanRight.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Right2","Pan Right2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanRight.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Right","Pan Right");

    AddKeyInput(&ui.controlsPanel,"Pan Up","Pan Up",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanUp.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Up2","Pan Up2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanUp.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Up","Pan Up");

    AddKeyInput(&ui.controlsPanel,"Pan Down","Pan Down",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_PanDown.keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Pan Down2","Pan Down2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_PanDown.secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Pan Down","Pan Down");

    AddKeyInput(&ui.controlsPanel,"Control Group 1","Control Group 1",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[0].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 1_2","Control Group 1_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[0].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 1","Control Group 1");

    AddKeyInput(&ui.controlsPanel,"Control Group 2","Control Group 2",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[1].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 2_2","Control Group 2_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[1].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 2","Control Group 2");

    AddKeyInput(&ui.controlsPanel,"Control Group 3","Control Group 3",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[2].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 3_2","Control Group 3_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[2].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 3","Control Group 3");

    AddKeyInput(&ui.controlsPanel,"Control Group 4","Control Group 4",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[3].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 4_2","Control Group 4_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[3].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 4","Control Group 4");

    AddKeyInput(&ui.controlsPanel,"Control Group 5","Control Group 5",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[4].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 5_2","Control Group 5_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[4].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 5","Control Group 5");

    AddKeyInput(&ui.controlsPanel,"Control Group 6","Control Group 6",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[5].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 6_2","Control Group 6_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[5].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 6","Control Group 6");

    AddKeyInput(&ui.controlsPanel,"Control Group 7","Control Group 7",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[6].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 7_2","Control Group 7_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[6].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 7","Control Group 7");

    AddKeyInput(&ui.controlsPanel,"Control Group 8","Control Group 8",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[7].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 8_2","Control Group 8_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[7].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 8","Control Group 8");

    AddKeyInput(&ui.controlsPanel,"Control Group 9","Control Group 9",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[8].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 9_2","Control Group 9_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[8].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 9","Control Group 9");

    AddKeyInput(&ui.controlsPanel,"Control Group 0","Control Group 0",xButtons,y+=h+padding,w,h,1,&currSettings.keymap.key_ctrlgroups[9].keyMappedTo);
    AddKeyInput(&ui.controlsPanel,"Control Group 0_2","Control Group 0_2",xButtons+w+padding,y,w,h,1,&currSettings.keymap.key_ctrlgroups[9].secondKeyMappedTo);
    AddText(&ui.controlsPanel,xText,y+h/2-al_get_font_line_height(ui.font)/2,"Control Group 0","Control Group 0");


    InitButton(&ui.controlsPanel.backButton, "Back", "", 0,0, 14, 14,LoadSprite("assets/ui/back_tab_icon.png",true));
    InitButton(&ui.controlsPanel.tabButton, "Tab", "", 0,0, 14, 33,LoadSprite("assets/ui/controls_tab_icon.png",true));
    ui.controlsPanel.back = &ui.mainMenuPanel;


}
void InitUI()
{
    ui.augmentIconIndex = LoadSprite("assets/ui/augment.png",false);

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
    ui.panelShownPercent = 0;
    ui.changingTo = NULL;
    
    ui.currentPanel = NULL;

   // Checkbox* particles = (Checkbox*)GetUIElement(&ui.videoOptionsPanel,"EnableParticles")->data;
    //particles->activated = &currSettings.particlesEnabled;

    //currSettings.particlesEnabled = &particles->activated;


    ui.panel_sprite_index = LoadSprite("assets/ui/ui.png",false);
    ui.panel_5_abilities_sprite_index = LoadSprite("assets/ui/ui_5abilities.png",false);
    ui.health_element_sprite_index = LoadSprite("assets/ui/health.png",false);
    ui.health_and_mana_element_sprite_index = LoadSprite("assets/ui/health_and_mana.png",false);


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
    int name = LoadSprite("assets/ui/mainmenu/name.png",false);
    int block = LoadSprite("assets/ui/mainmenu/block.png",false);
    int edge = LoadSprite("assets/ui/mainmenu/mainmenuedges.png",false);

    CreateWidget(GAMESTATE_MAIN_MENU,&sprites[edge],0,0,0.5,0.5,DRAWORDER_BEFOREUI,0,NULL);

    CreateWidget(GAMESTATE_MAIN_MENU,&sprites[block],157,93,0.5,0.5,DRAWORDER_BEFOREUI,0,NULL);
    CreateWidget(GAMESTATE_MAIN_MENU,&sprites[lantern],155,100,0.46,0.033,DRAWORDER_BEFOREUI,0,UpdateLanternWidget);

    CreateWidget(GAMESTATE_MAIN_MENU,&sprites[name],29,50,0.5,0.5,DRAWORDER_BEFOREUI,0,NULL);

    int x = -5;

    for (int i = 0; i < _SCREEN_SIZE/32; i++)
    {   

        //CreateWidget(GAMESTATE_MAIN_MENU,&sprites[gear32],x,0,DRAWORDER_AFTERUI,0);
        //CreateWidget(GAMESTATE_MAIN_MENU,&sprites[gear32],x+16,18,DRAWORDER_AFTERUI,0);

        //CreateWidget(GAMESTATE_MAIN_MENU,&sprites[gear32],x,_SCREEN_SIZE-32,DRAWORDER_AFTERUI,0);
        //CreateWidget(GAMESTATE_MAIN_MENU,&sprites[gear32],x+16,_SCREEN_SIZE-50,DRAWORDER_AFTERUI,0);

        x+=31;
    }
    chatboxes = NULL;
    numChatboxes = 0;
}
void UpdateWidget(Widget* w, float dt)
{
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
Widget* CreateWidget(GameState gameStateToAttach, Sprite* spr, int x, int y, float originX, float originY, Widget_DrawOrder drawOrder, int id, void* func)
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

    if ((numSprites_States[gameStateToAttach] + 1) % NUMSPRITESTATESTOALLOC == 0)
    {
        int reallocTo = ceil(numSprites_States[gameStateToAttach] + NUMSPRITESTATESTOALLOC)/(float)NUMSPRITESTATESTOALLOC+1;
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
                Button* b = (Button*)u->data;
                if (b->activated)
                {
                    PlaySound(&sounds[u->sound_clickDown_Index],0.5f,0);
                }
                return (b->activated);
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

        UpdateButton(x,y,&p->backButton,*mouseState,*mouseStateLastFrame);
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
        UpdateButton(x,y,&tab->tabButton,*mouseState,*mouseStateLastFrame);
        if (b->activated)
        {
            ChangeUIPanel(tab);
        }
        y += h;
    }
}
void UpdateKeyInput(int rX, int rY, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);

    KeyInput* t = (KeyInput*)u->data;
    Rect r = (Rect){rX,rY,u->w,u->h};

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
                PlaySound(&sounds[u->sound_clickUp_Index],0.5f,0);
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
            if (al_key_down(keyStateThisFrame,i))
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
void UpdateButton(int rX, int rY, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    //ToScreenSpaceI(&mouseStateLastFrame.x,&mouseStateLastFrame.y);

    Button* b = (Button*)u->data;
    b->activated = false;
    if (mouseState.mouse.buttons & 1 && !(mouseStateLastFrame.mouse.buttons & 1))
    {
        Rect r = (Rect){rX,rY,u->w,u->h};
        if (PointInRect(mouseState.screenX,mouseState.screenY,r))
        {
            if (!b->clicked)
            {
                PlaySound(&sounds[u->sound_clickUp_Index],0.5f,0);
            }
            b->clicked = true;
        }
    }
    if (mouseStateLastFrame.mouse.buttons & 1 && !(mouseState.mouse.buttons & 1))
    {
        Rect r = (Rect){rX,rY,u->w,u->h};
        if (b->clicked)
        {
            if (PointInRect(mouseState.screenX,mouseState.screenY,r))
            {
                b->activated = true;
            }
        }
    }
    if (!(mouseState.mouse.buttons & 1))
    {
        b->clicked = false;
    }
}
bool UpdateElement(Panel* p, UIElement* u, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame)
{
    if (!u->enabled)
        return false;
    int x; int y;
    GetUILocation(p,u,&x,&y);
   
    if (u->elementType == ELEMENT_BUTTON)
    {
        UpdateButton(x,y,u,*mouseState,*mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_KEYINPUT)
    {
        UpdateKeyInput(x,y,u,*mouseState,*mouseStateLastFrame,keyStateThisFrame);
    }

    if (u->elementType == ELEMENT_SLIDER)
    {
        UpdateSlider((Slider*)u->data, x,  y, u->w, u->h, *mouseState, *mouseStateLastFrame);
    }
    if (u->elementType == ELEMENT_CHECKBOX)
    {
        UpdateCheckbox((Checkbox*)u->data, x,  y, u->w,u->h,true,*mouseState, *mouseStateLastFrame);
        Checkbox* c = (Checkbox*)u->data;
       // bool b = *c->activated;
    }
    if (u->elementType == ELEMENT_PULLDOWN)
    {
        return UpdatePulldownMenu((Pulldown*)u->data, x,  y, u->w,u->h,*mouseState, *mouseStateLastFrame);
    }

    return false;
}
void UpdatePanel(Panel* p, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame)
{
    if (p)
    {
        for (int i = p->numElements-1; i  >= 0; i--)
        {
            if (UpdateElement(p,&p->elements[i],mouseState,mouseStateLastFrame, keyStateThisFrame))
            {
                break;
            }
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

    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_Pause))
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
    if (ui.currentPanel)
    {
        UpdatePanel(ui.currentPanel,mouseState,mouseStateLastFrame, keyState);
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


}
void DrawButtonText(UIElement* u,int x, int y, ALLEGRO_COLOR col)
{
    Button* b = (Button*)u->data;
    int align = b->drawLine ? ALLEGRO_ALIGN_CENTRE : ALLEGRO_ALIGN_LEFT;
    int textX; int textY;
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
    al_draw_text(ui.font,col,textX,textY,align,b->description);
}   
void DrawButton(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor, bool drawRectWhenUnselected)
{
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    Button* b = (Button*)u->data;
    ALLEGRO_FONT* font = ui.font;
    Rect button = (Rect){x,y,(int)u->w,(int)u->h};

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
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,FRIENDLY);
        al_draw_rectangle(x,y,x+u->w,y+u->h,BG,1);
        al_draw_text(ui.font,BG,textX,textY,align,b->description);
    }
    else 
    {
        if (b->drawLine)
        {
            al_draw_filled_rectangle(x,y,x+u->w,y+u->h,bgColor);
            if (isActive)
            {
                al_draw_rectangle(x, y, x + button.w, y + button.h, FRIENDLY,1);
            }
            else
            {
                DrawOutlinedRect_Dithered(button,FRIENDLY);
            }
        }
        //al_draw_text(ui.font,FRIENDLY,textX,textY,align,b->description);
        DrawButtonText(u,x,y,FRIENDLY);
    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,button) && isActive && !b->clicked)
    {
        al_draw_rectangle(x+2,y+2,x+u->w-2,y+u->h-2,FRIENDLY,1);
    }
    if (b->spriteIndex)
    {
        DrawSprite(&sprites[b->spriteIndex],x,y,0.5f,0.5f,0,FRIENDLY,false,false,false);
    }

}
void UIDrawText(UIElement* u, int x, int y)
{
   UI_Text* t = u->data;
   //al_draw_text(ui.font,FRIENDLY,x,y,ALLEGRO_ALIGN_LEFT,t->str);
   Text te = (Text){.f = ui.font,ui.boldFont,.x=x,.y=y,.color=FRIENDLY,.lineHeight=al_get_font_line_height(ui.font)};

   al_do_multiline_text(ui.font,256,t->str,cb,&te);


}
void DrawKeyInput(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor)
{
    KeyInput* k = (KeyInput*)u->data;
    //ToScreenSpaceI(&mouseState.x,&mouseState.y);
    ALLEGRO_FONT* font = ui.font;
    Rect button = (Rect){x,y,u->w,u->h};

    if (!k->text)
    {
        UpdateBind(u);
    }

    if (k->clicked && isActive)
    {
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,FRIENDLY);
        al_draw_rectangle(x,y,x+u->w,y+u->h,BG,1);
        al_draw_text(ui.tinyFont,BG,x+u->w/2, y + u->h /2 - al_get_font_ascent(font)/2.0,ALLEGRO_ALIGN_CENTRE,k->text);
     }
    else
    {
        al_draw_filled_rectangle(x,y,x+u->w,y+u->h,bgColor);
        if (isActive)
        {
            al_draw_rectangle(x,y,x+u->w,y+u->h,FRIENDLY,1);
        }
        else
        {   
            DrawOutlinedRect_Dithered(button,FRIENDLY);
        }
        al_draw_text(ui.tinyFont,FRIENDLY,x+u->w/2,y + u->h / 2 - al_get_font_ascent(font)/2.0,ALLEGRO_ALIGN_CENTRE,k->text);

    }
    if (PointInRect(mouseState.screenX,mouseState.screenY,button) && isActive && !k->clicked)
    {
        al_draw_rectangle(x+2,y+2,x+u->w-2,y+u->h-2,FRIENDLY,1);
    }

}
void DrawUIElement(UIElement* u, int x, int y, MouseState* mouseState, Color bgColor)
{
    ALLEGRO_COLOR col = GetColor(bgColor,0);
    if (u->elementType == ELEMENT_BUTTON)
    {
        DrawButton(u,x,y,*mouseState,u->enabled,col,((Button*)(u->data))->drawLine);
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

void GetUILocation(Panel* p, UIElement* uF, int* x, int* y)
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
            al_draw_rectangle(x,y,x+w,y+h,FRIENDLY,1);
            DrawButton(&tab->tabButton,x,y,*mouseState,true,BG,true);
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
        al_draw_filled_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,BG);
        al_draw_rectangle(p->x,p->y,p->x+p->w,p->y+p->h,FRIENDLY,1);
    }


    al_set_clipping_rectangle(p->x-1,p->y,p->w,p->h*panelShownPercent-1);



    int currX=p->x+p->padding; int currY=p->y+p->padding; 
    if (p->backButton.data)
    {
        DrawButton(&p->backButton,p->x,p->y,*mouseState,true,BG,true);
        currY+=p->backButton.h;
    }

    for (int i = 0; i < p->numTabs; i++)
    {
        int x = p->x+p->padding;
        int y = p->y*25*i+p->padding;
        al_draw_rectangle(x,y,x+25,y+25,FRIENDLY,1);
    }
    for (int i = 0; i < p->numElements; i++)
    {
        UIElement* u = ((UIElement*)&p->elements[i]);
        int x; int y;
        GetUILocation(p, u, &x, &y);
        DrawUIElement(u,x,y,mouseState,u->bgColor);

    }
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
    if (u->enabled)
    {
        if (u->elementType == ELEMENT_BUTTON)
        {
            Button* b = (Button*)(u->data);
            if (b->activated)
            {
                if (u->sound_clickDown_Index)
                {
                    PlaySound(&sounds[u->sound_clickDown_Index],0.5f,0);
                }
            }
            return b->activated;
        }
    }
    return false;
}

void DrawCursor(MouseState* mouseState, int index, bool clicked)
{
    if (mouseState->mouse.buttons & 1 || mouseState->mouse.buttons & 2)
    {
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,FRIENDLY,true,false,false);
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,BG,false,false,false);

    }
    else
    {
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,BG,true,false,false);
        DrawSprite(&sprites[index],mouseState->screenX,mouseState->screenY,0.5f,0.5f,0,FRIENDLY,false,false,false);
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
    al_draw_filled_rectangle(r.x-padding,r.y-padding,x+r.w,y+r.h,BG);
    
    r.x-=padding;
    r.y-=padding;
    r.w+=padding;
    r.h+=padding;
    if (dither)
        DrawOutlinedRect_Dithered(r,color);
    else
    {
       // al_draw_rectangle(r.x+1,r.y+1,r.w+padding,r.h+padding,color,1);
        al_draw_rectangle(r.x+1,r.y,r.x+r.w+1,r.y+r.h,color,1);
    
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
    if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(0)))
        return 0;
    if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(1)))
        return 1;
    if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(2)))
        return 2;
    if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(3)))
        return 3;
    if (PointInRect(mouseState->screenX,mouseState->screenY,GetAbilityPortraitRect(4)))
        return 4;


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
void DrawEndScreen(MouseState* mouseState, MouseState* mouseStateLastFrame)
{
    char* buffer = calloc(1,sizeof(char));

    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,_SCREEN_SIZE,BG);

    al_draw_text(ui.font,FRIENDLY,20,18,0,gameStats.gameWon == true ? "Victory" : "Defeat");
    al_draw_line(20,30,56,30,FRIENDLY,1);
    al_draw_line(17,52,239,52,FRIENDLY,1);

    //Write the boss name and sprite
    al_draw_text(ui.font,ENEMY,16,70,0,currEncounterRunning->name);
    Sprite* sEnemy = &sprites[currEncounterRunning->spriteIndex];
    DrawSprite(sEnemy,17,91,0.5f,0.5f,0,ENEMY,false,false,false);

    //Write augment level and augment changes
    al_draw_text(ui.font,ENEMY,90,89,0,"Augment 3");
    al_draw_text(ui.font,ENEMY,90,111,0,"+20%% Damage");
    al_draw_text(ui.font,ENEMY,185,111,0,"+15%% HP");
    al_draw_text(ui.font,ENEMY,90,124,0,"Random damaging pools");

    //Write time taken
    int hours = floor(gameStats.timeTaken/(60.0f*60.0f));
    int minutes = floor(gameStats.timeTaken/(60.0f) - hours*60);    
    int seconds = floor(gameStats.timeTaken - minutes*60); 
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
    sprintf(buffer,"Gold gained: %i",(int)players[0].gold);
    al_draw_text(ui.font,FRIENDLY,17,208,0,buffer);



    UpdateButton(ui.endScreen_Back.x,ui.endScreen_Back.y,&ui.endScreen_Back,*mouseState,*mouseStateLastFrame);
    UpdateButton(ui.endScreen_Retry.x,ui.endScreen_Retry.y,&ui.endScreen_Retry,*mouseState,*mouseStateLastFrame);
    UpdateButton(ui.endScreen_SaveReplay.x,ui.endScreen_SaveReplay.y,&ui.endScreen_SaveReplay,*mouseState,*mouseStateLastFrame);

    DrawUIElement(&ui.endScreen_Back,ui.endScreen_Back.x,ui.endScreen_Back.y,mouseState,ui.endScreen_Back.bgColor);
    DrawUIElement(&ui.endScreen_Retry,ui.endScreen_Retry.x,ui.endScreen_Retry.y,mouseState,ui.endScreen_Retry.bgColor);
    DrawUIElement(&ui.endScreen_SaveReplay,ui.endScreen_SaveReplay.x,ui.endScreen_SaveReplay.y,mouseState,ui.endScreen_SaveReplay.bgColor);
    
    int x = 86;
    int y = 139;
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
        SetGameStateToChoosingEncounter();
        //transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;
        RemoveReplay(&replay);
        if (gameStats.gameWon && !GameStateIsTransition(&gameState))
        {
            players[0].bankedGold += players[0].gold;
        }
        
    }
    if (GetButtonIsClicked(&ui.endScreen_Retry))
    {
        transitioningTo = GAMESTATE_INGAME;
        RemoveReplay(&replay);
        
        if (gameStats.gameWon && !GameStateIsTransition(&gameState))
        {
            players[0].bankedGold += players[0].gold;
        }

    }
    if (GetButtonIsClicked(&ui.endScreen_SaveReplay))
    {
        ReplayToDisk(&replay);
        ui.endScreen_SaveReplay.enabled = false;
        RemoveReplay(&replay);

    }

    free(buffer);
}
char* GetButtonText(UIElement* u)
{
    if (u->elementType != ELEMENT_BUTTON)
        return NULL;
    Button* b = (Button*)u->data;
    return b->description;
}