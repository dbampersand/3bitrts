#include "gamestate.h"

#include "sound.h"
#include "gameobject.h"
#include "map.h"
#include "player.h"
#include "encounter.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "colors.h"
#include "helperfuncs.h"
#include "math.h"
#include "ui.h"
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
    //gameState = GAMESTATE_INGAME;
    transitioningTo = GAMESTATE_INGAME;
    transitionTimer = 0;


    toSpawn = calloc(e->numUnitsToSelect,sizeof(GameObject*));
    for (int i = 0; i < e->numUnitsToSelect; i++)
    {
        toSpawn[i] = list[i];
    }
    encounterGoingTo = e;

}
void SetGameStateToChoosingEncounter()
{
    //gameState = GAMESTATE_INGAME;
    transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;
    transitionTimer = 0;

}
void FinishTransition()
{
    if (transitioningTo == GAMESTATE_INGAME)
    {
        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        RemoveAllGameObjects();
        SetMap(LoadMap(encounterGoingTo->mapPath));

        int xPos = 0; 
        for (int i = 0; i < encounterGoingTo->numUnitsToSelect; i++)
        {
            if (toSpawn[i] == NULL)
                continue;
            AddGameobject(toSpawn[i],80+xPos,180);   
            xPos += GetWidth(toSpawn[i]);
        }
        free(toSpawn);

    }
    if (transitioningTo == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        gameState = GAMESTATE_CHOOSING_ENCOUNTER;
        transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;

        ui.currentPanel = NULL;
        ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");

    }
    if (transitioningTo == GAMESTATE_CHOOSING_UNITS)
    {
        gameState = GAMESTATE_CHOOSING_UNITS;
        transitioningTo = GAMESTATE_CHOOSING_UNITS;
    }
    if (transitioningTo == GAMESTATE_END)
    {
        gameState = GAMESTATE_END;
        transitioningTo = GAMESTATE_END;

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
    transitioningTo = GAMESTATE_CHOOSING_UNITS;
    transitionTimer = 0;

}
void Quit()
{
    gameState = GAMESTATE_EXIT;
}
void CheckIfGameIsWon()
{
    if (GameStateIsTransition(&gameState)) 
        return;

    if (GetNumPlayerControlledObjs(&players[1]) == 0 && gameState == GAMESTATE_INGAME)
    {
        SetGameStateToEndscreen();
        //gameState = GAMESTATE_CHOOSING_ENCOUNTER;
        StopMusic();
        SetMap(&maps[0]);
    }

}
void CheckIfGameIsLost()
{
    if (GameStateIsTransition(&gameState)) 
        return;
    if (GetNumPlayerControlledObjs(&players[0]) == 0 && gameState == GAMESTATE_INGAME)
    {
        SetGameStateToEndscreen();
        StopMusic();

    }
}
void UpdateTransition(float dt)
{
    if (GameStateIsTransition(&gameState))
    {
        transitionTimer += dt * 0.8f;

        if (transitionTimer >= 1.0f)
        {
            FinishTransition();
        }
    }
    else
    {
        transitionTimer -= dt;
    }
    transitionTimer = clamp(transitionTimer,0,1);

}

//based on https://easings.net/#easeInOutBack
float easeInOutBack(float x) {
    const float c1 = 1.70158f;
    const float c2 = c1 * 1.525f;
    
    if (x < 0.5f)
    {
        return (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f;
    }
    else
    {
        return (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
    }
}

#define TRANSITION_CHAIN_SIZE 5
#define LATTICE_DISTANCE 20
void DrawTransition(float dt)
{
    float h = (_SCREEN_SIZE+1) * (easeInOutBack(transitionTimer)) - 1;
    al_draw_filled_rectangle(0,0,_SCREEN_SIZE,h,BG);
    al_draw_line(0,h,_SCREEN_SIZE,h,FRIENDLY,1);
    
    al_draw_line(22,0,22,h,FRIENDLY,1);
    al_draw_line(44,0,44,h,FRIENDLY,1);

    al_draw_line(211,0,211,h,FRIENDLY,1);
    al_draw_line(233,0,233,h,FRIENDLY,1);


    //draw chain
    float hpos = h-TRANSITION_CHAIN_SIZE;
    for (int i = 0; i < h/TRANSITION_CHAIN_SIZE + 1; i++)
    {
        al_draw_ellipse(32.5,hpos,TRANSITION_CHAIN_SIZE,TRANSITION_CHAIN_SIZE,FRIENDLY,1);
        al_draw_ellipse(222.5,hpos,TRANSITION_CHAIN_SIZE,TRANSITION_CHAIN_SIZE,FRIENDLY,1);

        hpos -= TRANSITION_CHAIN_SIZE*2;
    }

    //draw lattice
    al_set_clipping_rectangle(44,0,211,h);
    float y = h;
    for (int i = 0; i < h/LATTICE_DISTANCE + 5; i++)
    {
        al_draw_line(44,y,211,y+211/2.0f,FRIENDLY,1);
        al_draw_line(211,y,44,y+211/2.0f,FRIENDLY,1);

        y -= LATTICE_DISTANCE;
    }
    al_reset_clipping_rectangle();

}
bool GameStateIsTransition(GameState* g)
{
    return *g != transitioningTo;//(*g == GAMESTATE_TRANSITION_TO_CHOOSING_ENCOUNTER || *g == GAMESTATE_TRANSITION_TO_CHOOSING_UNITS || *g == GAMESTATE_TRANSITION_TO_INGAME || *g == GAMESTATE_TRANSITION_TO_END);
}
void SetGameStateToEndscreen()
{
    transitioningTo = GAMESTATE_END;
    transitionTimer = 0;    

}