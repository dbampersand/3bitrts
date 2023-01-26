#include "player.h"

#include "gameobject.h"
#include "ui.h"
#include "luafuncs.h"
#include "sound.h"
#include "map.h"
#include "settings.h"
#include "item.h"
#include "allegro5/allegro_ttf.h"
#include <math.h>
float cameraSpeed = 260;
Player* players  = NULL;

void RemoveIndexFromSelection(Player* p, int index)
{
    for (int i = index; i < MAXUNITSSELECTED-1; i++)
    {
        p->selection[i] = p->selection[i+1];
    }
    p->indexSelectedUnit = 0;
    p->selection[MAXUNITSSELECTED-1] = NULL;
    p->numUnitsSelected--;

}
void RemoveGameObjectFromSelection(Player* p, GameObject* g)
{
    int index = -1;
    for (int i = 0; i < MAXUNITSSELECTED-1; i++)
    {
        if (p->selection[i] == g)
        {
            index = i;
            break;
        }
    }
    if (index >= 0)
    {
        RemoveIndexFromSelection(p,index);
    }
}
void InitPlayers()
{
    players = calloc(3,sizeof(Player));
    AddGold(0);
}
void CheckAbilityClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseState)
{
    int index = GetAbilityIndexClicked(keyState,keyStateLastFrame);
    if (index > -1)
    if (players[0].selection[players[0].indexSelectedUnit])
    {
        if (!AbilityIsOnCooldown(&players[0].selection[players[0].indexSelectedUnit]->abilities[index]))
        {
            PlaySound(&sounds[ability_UI_click_sound],0.5f);
            players[0].abilityHeld = NULL;
            currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
            if (currGameObjRunning->numAbilities < 5 && index == 4)
                return;
            
            if (currGameObjRunning)
            {
                currAbilityRunning = &players[0].selection[players[0].indexSelectedUnit]->abilities[index];
                if (players[0].selection[players[0].indexSelectedUnit]->mana >= GetManaCost(currAbilityRunning))
                {
                    if (currAbilityRunning->castType == ABILITY_INSTANT || currAbilityRunning->castType == ABILITY_TOGGLE)
                    {
                         if (!IsBindDown(keyState,currSettings.keymap.key_Shift))
                            CastAbility(currGameObjRunning,currAbilityRunning,mouseState->worldX,mouseState->worldY,mouseState->worldX-currGameObjRunning->position.worldX,mouseState->worldY-currGameObjRunning->position.worldY,NULL);
                        else
                            CastCommand(currGameObjRunning,NULL,currAbilityRunning,mouseState->worldX,mouseState->worldY,IsBindDown(keyState,currSettings.keymap.key_Shift));
                    }
                    else
                    {
                        
                        players[0].abilityHeld = currAbilityRunning;
                    }
                }
            }
        }
    }
    if (IsBindReleasedThisFrame(keyState,keyStateLastFrame,currSettings.keymap.key_Cancel))
    {
        if (players[0].abilityHeld)
        {
            PlaySound(&sounds[ability_UI_click_up_sound],0.5f);
        }
        players[0].abilityHeld = NULL;
        players[0].amoveSelected = false;
    }


}
void ToScreenSpace(float* x,float* y)
{
    *x -= players[0].cameraPos.x;
    *y -= players[0].cameraPos.y;
}
void ToScreenSpaceI(int* x,int* y)
{
    *x -= players[0].cameraPos.x;
    *y -= players[0].cameraPos.y;
}
void ToWorldSpace(float* x,float* y)
{
    *x += players[0].cameraPos.x;
    *y += players[0].cameraPos.y;

}
void ToWorldSpaceI(int* x,int* y)
{
    *x += players[0].cameraPos.x;
    *y += players[0].cameraPos.y;

}
float ToWorldSpace_X(float x)
{
   return x + players[0].cameraPos.x;
}

float ToWorldSpace_Y(float y)
{
    return y + players[0].cameraPos.y;
}


float ToScreenSpace_X(float x)
{
   return x - players[0].cameraPos.x;
}
float ToScreenSpace_Y(float y)
{
    return y - players[0].cameraPos.y;
}
void FocusCameraOnPos(float x, float y)
{
    float dx = x - _SCREEN_SIZE/2;
    float dy = y - _SCREEN_SIZE/2;

    MoveCam(dx,dy);
}
void MoveCam(float x, float y)  
{
    players[0].cameraPos.x = x;
    players[0].cameraPos.y = y;
    

    if (x < 0)
        players[0].cameraPos.x = 0;
    if (y < 0)
        players[0].cameraPos.y = 0;
    
    if (x + _SCREEN_SIZE > _MAX(GetMapWidth(),_SCREEN_SIZE))
    {
        players[0].cameraPos.x = _MAX(GetMapWidth(),_SCREEN_SIZE) - _SCREEN_SIZE;
    }
    if (y + _SCREEN_SIZE > _MAX(GetMapHeight(),_SCREEN_SIZE))
    {
        players[0].cameraPos.y = _MAX(GetMapHeight(),_SCREEN_SIZE) - _SCREEN_SIZE;
    }

}
void MoveCamera(MouseState mouseState,ALLEGRO_KEYBOARD_STATE* keyState, float dt)
{    

    if (mouseState.screenX < SCREEN_EDGE || IsBindDown(keyState,currSettings.keymap.key_PanLeft))
    {
        MoveCam(players[0].cameraPos.x - (cameraSpeed*dt),players[0].cameraPos.y);
    }

    if (mouseState.screenX > _SCREEN_SIZE - SCREEN_EDGE || IsBindDown(keyState,currSettings.keymap.key_PanRight))
    {
        MoveCam(players[0].cameraPos.x + (cameraSpeed*dt),players[0].cameraPos.y);
    }

    if (mouseState.screenY < SCREEN_EDGE  || IsBindDown(keyState,currSettings.keymap.key_PanUp))
    {
        MoveCam(players[0].cameraPos.x,players[0].cameraPos.y - (cameraSpeed*dt));
    }

    if (mouseState.screenY > _SCREEN_SIZE - SCREEN_EDGE  || IsBindDown(keyState,currSettings.keymap.key_PanDown))
    {
        MoveCam(players[0].cameraPos.x,players[0].cameraPos.y + (cameraSpeed*dt));
    }

}
void ClearSelection()
{
    for (int i = 0; i < players[0].numUnitsSelected; i++)
    {
        SetSelected(players[0].selection[i],false);

        players[0].selection[i] = NULL;
    }
    players[0].numUnitsSelected = 0;

}
bool BuyItem(Item* i)
{
    if (players[0].gold < i->goldCost)
        return false;
    AddGold(-i->goldCost);
    return true;
}
int GetGold()
{
    return players[0].gold;
}
void UpdateGoldTextString(int gold)
{
    int numDigits = NumDigits(gold);
    if (!players[0].goldText)
    {
        players[0].goldText = calloc(numDigits+2,sizeof(char));
        players[0].goldAnimationTimer = 1;   
    }
    else
    {
        players[0].goldText = realloc(players[0].goldText,(numDigits+2)*sizeof(char));
    }
    sprintf(players[0].goldText,"%i",gold);
    players[0].goldText[numDigits] = '\0';

}
void AddGold(float count)
{
    int before = floor(players[0].gold);
    players[0].gold += count;

    //if there's been a change of at least 1 gold
    if (before != (int)floor(players[0].gold))
    {
        if (players[0].goldAnimationTimer >= 1)
        {
            players[0].goldAnimationTimer = 0;
            players[0].goldDisplaying = players[0].gold-count;
        }
    }
}
void UpdateGoldAnimationTimer(float dt)
{
    players[0].goldAnimationTimer += dt;
    if (players[0].goldAnimationTimer > 1)
    {
        players[0].goldAnimationTimer = 1;
        players[0].goldDisplaying = players[0].gold;
        UpdateGoldTextString(players[0].goldDisplaying);
        return;
    }

    int x = 8; int y = 8;
    int startY = -GetWidthSprite(&sprites[ui.gold_element_sprite_index]);
    float percent = easeInOutBack(players[0].goldAnimationTimer);

    float yEnd = (percent * y) - fabsf((1-percent) * startY);

    DrawSprite(&sprites[ui.gold_element_sprite_index],x,yEnd,0,0,0,FRIENDLY,false);

    float goldShowingPercent = EaseInOutCubic(players[0].goldAnimationTimer*1.25f);
    float goldDisplaying = players[0].gold-((players[0].gold - players[0].goldDisplaying)*(1-goldShowingPercent));
    UpdateGoldTextString(goldDisplaying);

}
void DrawGoldCount()
{
    int x = 8; int y = 8;
    if (gameState == GAMESTATE_PURCHASING_UNITS)
        x = 24;

    DrawSprite(&sprites[ui.gold_element_sprite_index],x,y,0,0,0,FRIENDLY,false);

    ALLEGRO_COLOR c = floor(players[0].gold) >= 0 ? FRIENDLY : ENEMY;
    al_draw_text(ui.tinyFont,c,x+GetWidthSprite(&sprites[ui.gold_element_sprite_index])+2,y,ALLEGRO_ALIGN_LEFT,players[0].goldText);
}
void ClearGold()
{
    players[0].goldAnimationTimer = 1;
    AddGold(-GetGold());
}