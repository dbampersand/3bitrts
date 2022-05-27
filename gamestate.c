#include "gamestate.h"
#include "sound.h"
#include "gameobject.h"
#include "map.h"
#include "player.h"
#include "encounter.h"
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
    gameState = GAMESTATE_MAIN_MENU;

}
void SetGameStateToInGame(GameObject** list, int numObjectsToAdd, Encounter* e)
{
    gameState = GAMESTATE_INGAME;
    RemoveAllGameObjects();
    SetMap(LoadMap(e->mapPath));

    int xPos = 0; 
    for (int i = 0; i < e->numUnitsToSelect; i++)
    {
        AddGameobject(list[i],80+xPos,180);   
        xPos += GetWidth(list[i]);
    }


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
    gameState = GAMESTATE_CHOOSING_UNITS;

}
void Quit()
{
    gameState = GAMESTATE_EXIT;
}
void CheckIfGameIsWon()
{
    if (GetNumPlayerControlledObjs(&players[1]) == 0 && gameState == GAMESTATE_INGAME)
    {
        gameState = GAMESTATE_CHOOSING_ENCOUNTER;
        StopMusic();
        SetMap(&maps[0]);
    }

}
void CheckIfGameIsLost()
{
    if (GetNumPlayerControlledObjs(&players[0]) == 0 && gameState == GAMESTATE_INGAME)
    {
        Quit();
    }
}