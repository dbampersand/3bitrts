#include "gamestate.h"
#include "sound.h"
#include "gameobject.h"
#include "map.h"
#include "player.h"
void StartCombat()
{
    if (combatStarted)
    {
        return;
    }
    PlayEncounterMusic();
    combatStarted = true;
}
void InitGameState()
{
    gameState = MAIN_MENU;

}
void SetGameStateToChoosingParty()
{
    RemoveAllGameObjects();
    SetMap(&maps[0]);
    int xPos = 100;
    for (int i = 0; i < numPrefabs; i++)
    {
        if (prefabs[i].playerChoosable == true)
        {
            AddGameobject(&prefabs[i],xPos,150);
            xPos+=GetWidth(&prefabs[i]);
        }   
    }
    gameState = CHOOSING_UNITS;

}
void Quit()
{
    gameState = GAMESTATE_EXIT;
}
void CheckIfGameIsWon()
{
    if (GetNumPlayerControlledObjs(&players[1]) == 0 && gameState == INGAME)
    {
        gameState = CHOOSING_ENCOUNTER;
        StopMusic();
        SetMap(&maps[0]);
    }

}
void CheckIfGameIsLost()
{
    if (GetNumPlayerControlledObjs(&players[0]) == 0 && gameState == INGAME)
    {
        Quit();
    }
}