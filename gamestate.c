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
#include "replay.h"
#include "shop.h"
#include "console.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


 GameStats gameStats = {0};
 bool combatStarted = false;
 ALLEGRO_EVENT_QUEUE* queue = {0};

 GameState transitioningTo = {0};
 float transitionTimer = 0;

 GameObject** toSpawn = NULL;
 GameObject* deadFriendlyObjects;

 Encounter* encounterGoingTo = NULL;
 char* pathToNextMap = NULL;

GameState gameState = {0};


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

    SetLoadscreen(sprites[currEncounterRunning->loadScreen_spriteIndex].path,NULL,1,1,1,1,1,e->name,"Press any key to begin.");

    if (toSpawn)
        free(toSpawn);
    toSpawn = calloc(e->numUnitsToSelect,sizeof(GameObject*));
    if (deadFriendlyObjects)
        free(deadFriendlyObjects);
    deadFriendlyObjects = calloc(e->numUnitsToSelect,sizeof(GameObject));
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
void SetGameStateToEnterShop()
{
    
}
void FinishTransition()
{
    if (transitioningTo == GAMESTATE_PURCHASING_UNITS)
    {
        gameState = GAMESTATE_PURCHASING_UNITS;
        transitioningTo = GAMESTATE_PURCHASING_UNITS;
        
    }
    if (transitioningTo == GAMESTATE_MAIN_MENU)
    {
        gameState = GAMESTATE_MAIN_MENU;
        transitioningTo = GAMESTATE_MAIN_MENU;
        ChangeUIPanel(&ui.mainMenuPanel);
        RemoveReplay(&replay);
    }
    if (transitioningTo == GAMESTATE_INGAME)
    {
        NewReplay();

        RemoveAllGameObjects();

        SetMap(LoadMap(encounterGoingTo->mapPath));

        int xPos = currMap->spawnPoint.x;
        int yPos = currMap->spawnPoint.y;

        Point camPos = (Point){0,0};


         
        if (toSpawn)
            for (int i = 0; i < encounterGoingTo->numUnitsToSelect; i++)
            {
                if (toSpawn[i] == NULL)
                    continue;
                GameObject* g = toSpawn[i];
                GameObject* gNew = AddGameobject(g,xPos,yPos,SOURCE_SPAWNED_FROM_MAP);   
                HoldCommand(gNew,false);
                xPos += GetWidth(g)+4;
                if (i == encounterGoingTo->numUnitsToSelect/2)
                {
                    camPos.x = xPos;
                    camPos.y = yPos;
                }
            }

        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsActive(activeObjects[i]) && IsOwnedByPlayer(activeObjects[i]))
            {
                HoldCommand(activeObjects[i],false);
            }
        }
        FocusCameraOnPos(camPos.x,camPos.y);
        memset(&gameStats,0,sizeof(GameState));

        ClearGold();


        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsActive(activeObjects[i]))
            {
                CureAll(activeObjects[i]);
            }   
        }

        RemoveAllAttacks();

        ClearSelection();

        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        //free(toSpawn);

    }
    if (transitioningTo == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        gameState = GAMESTATE_CHOOSING_ENCOUNTER;
        transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;

        ui.currentPanel = NULL;
        RemoveAllAttacks();

        ClearSelection();
        RemoveAllGameObjects();
        //ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");
        combatStarted = false;

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
        combatStarted = false;


        //if (toSpawn)
         //   free(toSpawn);
       // toSpawn = NULL;

    }

    if (transitioningTo == GAMESTATE_CHOOSING_UNITS)
    {
        gameState = GAMESTATE_CHOOSING_UNITS;
        transitioningTo = GAMESTATE_CHOOSING_UNITS;
        //SetMap(&maps[0]);
        players[0].cameraPos.x = 0;
        players[0].cameraPos.y = 0;
        combatStarted = false;


    }
    if (transitioningTo == GAMESTATE_END)
    {
        gameState = GAMESTATE_END;
        transitioningTo = GAMESTATE_END;
        StopMusic();
        combatStarted = false;

        SetMap(&maps[0]);

    }
    if (transitioningTo == GAMESTATE_IN_SHOP)
    {
        gameState = GAMESTATE_IN_SHOP;
        RefreshShop();


    }
    if (transitioningTo == GAMESTATE_CHANGE_MAP)
    {
        combatStarted = false;

        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        //ending the map early map generate gold events from killing objs
        int goldBefore = players[0].gold;
        for (int i = 0; i < MAX_OBJS; i++)
        {
            if (!IsOwnedByPlayer(&objects[i]))
            {
                KillObj(&objects[i],false);
            }
        }
        AddGold(goldBefore - players[0].gold);

        SetMap(LoadMap(pathToNextMap));

        int xPos = currMap->spawnPoint.x;
        int yPos = currMap->spawnPoint.y;

        Point camPos = (Point){0,0};

        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsActive(activeObjects[i]) && IsOwnedByPlayer(activeObjects[i]))
            {
               // objects[i].position.worldX = xPos;
                //objects[i].position.worldY = yPos;
                UpdateObjPosition(activeObjects[i],xPos,yPos);
                SetTargetPosition(activeObjects[i],xPos,yPos);

                xPos += GetWidth(activeObjects[i]);
                if (i == encounterGoingTo->numUnitsToSelect/2)
                {
                    camPos.x = xPos;
                    camPos.y = yPos;    
                }
            }
        }
        FocusCameraOnPos(camPos.x,camPos.y);

        RemoveAllAttacks();
        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsActive(activeObjects[i]))
            {
                CureAll(activeObjects[i]);
            }   
        }
        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsActive(activeObjects[i]) && IsOwnedByPlayer(activeObjects[i]))
            {
                HoldCommand(activeObjects[i],false);
            }
        }
        free(pathToNextMap);
        pathToNextMap = NULL;
        ClearSelection();

    }
    
}
void SetGameStateToPurchasingUnits()
{
    TransitionTo(GAMESTATE_PURCHASING_UNITS);
}
void SetGameStateToWatchingReplay()
{
    TransitionTo(GAMESTATE_WATCHING_REPLAY);
    ChangeUIPanel(NULL);
}
void SetGameStateToInShop()
{
    TransitionTo(GAMESTATE_IN_SHOP);
}
void SetGameStateToChangingMap()
{
    TransitionTo(GAMESTATE_CHANGE_MAP);
}
void SetGameStateToChoosingParty()
{
    
    if (TransitionTo(GAMESTATE_CHOOSING_UNITS))
    {
        RemoveAllGameObjects();
        SetMap(&maps[0]);
        int xPos = 17;  
        
        #define NUMUNITSPERROW 3

        #define TANKSTARTX 24
        #define TANKSTARTY 50

        #define HEALERSTARTX 110
        #define HEALERSTARTY 50

        #define MELEEDPSSTARTX 199
        #define MELEEDPSSTARTY 50

        #define RANGEDDPSSTARTX 24
        #define RANGEDDPSSTARTY 106

        #define UTILITYSTARTX 110
        #define UTILITYSTARTY 106

        #define UNCATAGORISEDX 199
        #define UNCATAGORISEDY 106


        int tankX = TANKSTARTX;
        int tankY = TANKSTARTY;
        int healerX = HEALERSTARTX;
        int healerY = HEALERSTARTY;
        int meleeX = MELEEDPSSTARTX;
        int meleeY = MELEEDPSSTARTY;
        int rangedX = RANGEDDPSSTARTX;
        int rangedY = RANGEDDPSSTARTY;
        int utilityX = UTILITYSTARTX;
        int utilityY = UTILITYSTARTY;
        int uncategorisedX = UTILITYSTARTX;
        int uncategorisedY = UTILITYSTARTY;


        int numTanks=0;
        int numHealers=0;
        int numMelee=0;
        int numRanged=0;
        int numUtility=0;
        int numUncategorised = -1;

        int largestTank = -1;
        int largestHealer = -1;
        int largestMelee = -1;
        int largestRanged = -1;
        int largestUtility = -1;
        int largestUncategorised = -1;





        for (int i = 0; i < numPrefabs; i++)
        {
            GameObject* g = prefabs[i];
            
            if (g->playerChoosable == true)
            {

                int x=0; int y=0    ;
                int w = GetWidth(g);
                int h = GetHeight(g);

                if (g->category & TYPE_TANK)
                {
                    numTanks++;

                    if (h > largestTank)
                        largestTank = h;

                    if ((numTanks-1) % 3 == 0)
                    {
                        tankX = TANKSTARTX;
                        tankY += largestTank+1;
                        largestHealer = -1;
                    }
                    x = tankX;
                    y = tankY;
                    tankX += w;

                }
               else  if (g->category & TYPE_HEALER)
                {
                    numHealers++;

                    if (h > largestHealer)
                        largestHealer = h;

                    if ((numHealers - 1) % 3 == 0)
                    {
                        healerX = HEALERSTARTX;
                        healerY += largestHealer+1;
                        largestHealer = -1;
                    }
                    x = healerX;
                    y = healerY;
                    healerX += w;
                }
                else if (g->category & TYPE_MELEEDPS)
                {
                    numMelee++;

                    if (h > largestMelee)
                        largestMelee = h;

                    if ((numMelee-1) % 3 == 0)
                    {
                        meleeX = MELEEDPSSTARTX;
                        meleeY += largestMelee+1;
                        largestMelee = -1;
                    }
                    x = meleeX;
                    y = meleeY;
                    meleeX += w;
                }
                
                else if (g->category & TYPE_RANGEDDPS)
                {
                    numRanged++;

                    if (h > largestRanged)
                        largestRanged = h;

                    if ((numRanged-1) % 3 == 0)
                    {
                        rangedX = RANGEDDPSSTARTX;
                        rangedY += largestRanged+1;
                        largestRanged = -1;
                    }
                    x = rangedX;
                    y = rangedY;
                    rangedX += w;
                }
                
                else if (g->category & TYPE_UTILITY)
                {
                    numUtility++;

                    if (h > largestUtility)
                        largestUtility = h;

                    if ((numUtility-1) % 3 == 0)
                    {
                        utilityX = UTILITYSTARTX;
                        utilityY += largestUtility+1;
                        largestUtility = -1;
                    }
                    x = utilityX;
                    y = utilityY;
                    utilityX += w;
                }
                else
                {
                    numUncategorised++;

                    if (h > largestUncategorised)
                        largestUncategorised = h;

                    if ((numUncategorised-1) % 3 == 0)
                    {
                        uncategorisedX = UTILITYSTARTX;
                        uncategorisedY += largestUncategorised+1;
                        largestUncategorised = -1;
                    }
                    x = uncategorisedX;
                    y = uncategorisedY;
                    uncategorisedX += w;
                }


                GameObject* gNew = AddGameobject(g,x,y,SOURCE_SPAWNED_FROM_MAP);
                HoldCommand(gNew,false);
                
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

    if (GameStateIsTransition(&gameState)) 
        return;

    if (GetNumPlayerControlledObjs(&players[1]) == 0 && gameState == GAMESTATE_INGAME)
    {
        WinGame();
    }

}
void CheckIfGameIsLost()
{
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
        if (transitionTimer < 0.4f)
        {
            transitionTimer += dt * 3;
        }
        else
        {
            transitionTimer += dt * 0.8f;
        }

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
//based on https://easings.net/#easeOutBack
float easeOutBack(float x) 
{
    const float c1 = 1.70158f;
    const float c3 = c1 + 1;

    return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}
//based on https://easings.net/#easeOutSine
float easeOutSine(float x)
{
  return sin((x * M_PI) / 2.0f);
}
//based on https://easings.net/#easeInOutBack
float easeInOutBack(float x) 
{
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
    float h = (_SCREEN_SIZE+10) * (easeInOutBack(transitionTimer)) - 1;
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
    SerializeSection(&replay,true);
    TransitionTo(GAMESTATE_END);
    ui.endScreen_Retry.enabled = true;
    ui.endScreen_SaveReplay.enabled = true;

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
    return (console.active || ui.currentPanel || (gameState == GAMESTATE_MAIN_MENU || gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER || gameState == GAMESTATE_IN_CHATBOX || gameState == GAMESTATE_END));
}
bool GameIsIngame()
{
    return (gameState == GAMESTATE_CHOOSING_UNITS || gameState == GAMESTATE_INGAME || gameState == GAMESTATE_IN_CHATBOX);
}
void RessurectGameObject(GameObject* g)
{
    int index = g - deadFriendlyObjects;
    AddGameobject(g,0,0,SOURCE_SPAWNED_FROM_MAP);

    deadFriendlyObjects[index].properties = 0;

}
void AddDeadGameObject(GameObject* g)
{
    if (gameState == GAMESTATE_INGAME)
    {
        for (int i = 0; i < currEncounterRunning->numUnitsToSelect; i++)
        {
            if (!(deadFriendlyObjects[i].properties & OBJ_ACTIVE))
            {
                deadFriendlyObjects[i] = *g;
                deadFriendlyObjects[i].properties |= OBJ_ACTIVE;   
                return;
            }
        }
    }
}
void GoTutorial()
{
    currEncounterRunning = GetEncounterByName("Tutorial");
    SetGameStateToLoadingEncounter(NULL,0,currEncounterRunning);
    ChangeUIPanel(NULL);
}