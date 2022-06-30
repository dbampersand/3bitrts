#include "player.h"

#include "gameobject.h"
#include "ui.h"
#include "luafuncs.h"
#include "sound.h"


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
    players = calloc(2,sizeof(GameObject));
}
void CheckAbilityClicked(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState)
{
    int index = GetAbilityIndexClicked(keyState,keyStateLastFrame);

    if (index > -1)
    if (players[0].selection[players[0].indexSelectedUnit])
    {
        if (players[0].selection[players[0].indexSelectedUnit]->abilities[index].cdTimer <= 0)
        {
            PlaySound(&sounds[ability_UI_click_sound],0.5f);
            players[0].abilityHeld = NULL;
            currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
            if (currGameObjRunning)
            {
                currAbilityRunning = &players[0].selection[players[0].indexSelectedUnit]->abilities[index];
                if (players[0].selection[players[0].indexSelectedUnit]->mana >= GetManaCost(currAbilityRunning))
                {
                    if (currAbilityRunning->castType == ABILITY_INSTANT || currAbilityRunning->castType == ABILITY_TOGGLE)
                    {
                        if (!al_key_down(keyState,ALLEGRO_KEY_LSHIFT))
                            ClearCommandQueue(currGameObjRunning);
                        CastCommand(currGameObjRunning,NULL,currAbilityRunning,mouseState->x,mouseState->y);
                    }
                    else
                    {
                        
                        players[0].abilityHeld = currAbilityRunning;
                    }
                }
            }
        }
    }

    if (!al_key_down(keyState,ALLEGRO_KEY_ESCAPE) && al_key_down(keyStateLastFrame,ALLEGRO_KEY_ESCAPE) )
    {
        if (players[0].abilityHeld)
        {
            PlaySound(&sounds[ability_UI_click_up_sound],0.5f);
        }
        players[0].abilityHeld = NULL;
        players[0].amoveSelected = false;
    }


}