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
#include "loadscreen.h"
#include "attack.h"
bool TransitionTo(GameState state)
{
    if (transitioningTo == state)
        return false;
    transitioningTo = state;
    transitionTimer = 0;
    return true;
}
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
    gameState = GAMESTATE_LOAD_SCREEN;
    transitioningTo = gameState;
    memset(&gameStats,0,sizeof(GameState));
    pathToNextMap = NULL;

}
void SetGameStateToLoadingEncounter(GameObject** list, int numObjectsToAdd, Encounter* e)
{
    //transitioningTo = GAMESTATE_LOAD_ENCOUNTER;
    //transitionTimer = 0;
    TransitionTo(GAMESTATE_LOAD_ENCOUNTER);

    SetLoadscreen(sprites[currEncounterRunning->loadScreen_spriteIndex].path,1,1,1,1,1,e->name,"Press any key to begin.");

    if (toSpawn)
            free(toSpawn);
    toSpawn = calloc(e->numUnitsToSelect,sizeof(GameObject*));
    for (int i = 0; i < numObjectsToAdd; i++)
    {
        toSpawn[i] = list[i];
    }
    encounterGoingTo = e;
}
void SetGameStateToInGame()
{
    //transitioningTo = GAMESTATE_INGAME;
    //transitionTimer = 0;
    TransitionTo(GAMESTATE_INGAME);



}
void SetGameStateToChoosingEncounter()
{
    //gameState = GAMESTATE_INGAME;
    //transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;
    //transitionTimer = 0;
    TransitionTo(GAMESTATE_CHOOSING_ENCOUNTER);

}
void FinishTransition()
{
    if (transitioningTo == GAMESTATE_MAIN_MENU)
    {
        gameState = GAMESTATE_MAIN_MENU;
        transitioningTo = GAMESTATE_MAIN_MENU;
        ChangeUIPanel(&ui.mainMenuPanel);

    }
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
        memset(&gameStats,0,sizeof(GameState));

        //free(toSpawn);

    }
    if (transitioningTo == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        gameState = GAMESTATE_CHOOSING_ENCOUNTER;
        transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;

        ui.currentPanel = NULL;
        //ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");

    }
    if (transitioningTo == GAMESTATE_WATCHING_REPLAY)
    {
        gameState = GAMESTATE_WATCHING_REPLAY;
        transitioningTo = GAMESTATE_WATCHING_REPLAY;

    }
    if (transitioningTo == GAMESTATE_LOAD_ENCOUNTER)
    {
        RemoveAllAttacks();
        gameState = GAMESTATE_LOAD_ENCOUNTER;
        transitioningTo = GAMESTATE_LOAD_ENCOUNTER;
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
        StopMusic();
        SetMap(&maps[0]);

    }
    if (transitioningTo == GAMESTATE_CHANGE_MAP)
    {
        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        int xPos = 80;
        for (int i = 0; i < MAX_OBJS; i++)
        {
            objects[i].position.x = xPos;
            objects[i].position.y = 180;
            objects[i].targetPosition.x = xPos;
            objects[i].targetPosition.y = 180;

            if (IsActive(&objects[i]) && IsOwnedByPlayer(&objects[i]))
                xPos += GetWidth(&objects[i]);
        }
        RemoveAllAttacks();
        SetMap(LoadMap(pathToNextMap));

        free(pathToNextMap);
        pathToNextMap = NULL;
    }
    
}
void SetGameStateToWatchingReplay()
{
    TransitionTo(GAMESTATE_WATCHING_REPLAY);
    ChangeUIPanel(NULL);
}
void SetGameStateToChangingMap(const char* mapPath)
{
    TransitionTo(GAMESTATE_CHANGE_MAP);

    if (pathToNextMap)
        free(pathToNextMap);
    pathToNextMap = calloc(strlen(mapPath)+1,sizeof(char));
    strcpy(pathToNextMap,mapPath);

}
void SetGameStateToChoosingParty()
{
    
    if (TransitionTo(GAMESTATE_CHOOSING_UNITS))
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
    }
}
void Quit()
{
    gameState = GAMESTATE_EXIT;
}
void WinGame()
{
    gameStats.gameWon = true;
    SetGameStateToEndscreen();
}
void LoseGame()
{
    gameStats.gameWon = false;
    SetGameStateToEndscreen();
    StopMusic();
}
void CheckIfGameIsWon()
{
    if (!currEncounterRunning->automaticWinCheck)
        return;

    if (GameStateIsTransition(&gameState)) 
        return;

    if (GetNumPlayerControlledObjs(&players[1]) == 0 && gameState == GAMESTATE_INGAME)
    {
        WinGame();
    }

}
void CheckIfGameIsLost()
{
    if (!currEncounterRunning->automaticWinCheck)
        return;
    if (GameStateIsTransition(&gameState)) 
        return;
    if (GetNumPlayerControlledObjs(&players[0]) == 0 && gameState == GAMESTATE_INGAME)
    {
        LoseGame();
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
    //transitioningTo = GAMESTATE_END;
    //transitionTimer = 0;    
    TransitionTo(GAMESTATE_END);
    ui.endScreen_Retry.enabled = true;
}
void SetGameStateToInMenu()
{
    //transitioningTo = GAMESTATE_MAIN_MENU;
    //transitionTimer = 0;    
    TransitionTo(GAMESTATE_MAIN_MENU);
}
void DrawStartScreen()
{
    
}
bool GameIsPaused() 
{
    return (ui.currentPanel || (gameState == GAMESTATE_MAIN_MENU || gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER || gameState == GAMESTATE_IN_CHATBOX || gameState == GAMESTATE_END));
}
bool GameIsIngame()
{
    return (gameState == GAMESTATE_CHOOSING_UNITS || gameState == GAMESTATE_INGAME || gameState == GAMESTATE_IN_CHATBOX);
}

void GoTutorial()
{
    currEncounterRunning = GetEncounterByName("Tutorial");
    SetGameStateToLoadingEncounter(NULL,0,currEncounterRunning);
    ChangeUIPanel(NULL);
}