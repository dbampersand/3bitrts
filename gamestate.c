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
#include "particle.h"
#include "editor.h"
#include "easings.h"
#include "gamesave.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

bool _COMING_FROM_EDITOR = false;

 GameStats gameStats = {0};
 bool combatStarted = false;
 ALLEGRO_EVENT_QUEUE* queue = {0};

 GameState transitioningTo = {0};
 float transitionTimer = 0;
 bool transitionAudioPlayed = false;

 GameObject** toSpawn = NULL;
 GameObject* deadFriendlyObjects;
GameObject** initialSelection = NULL;

 Encounter* encounterGoingTo = NULL;
 char* pathToNextMap = NULL;

GameState gameState = {0};

int numUnitsToSpawn;


TRANSITION transitionDrawing;

int numDeadFriendlyObjects;
int numDeadFriendlyObjectsHeld;


void Retry()
{
    float timeBefore = gameStats.timeTaken;
    float goldBefore = players[0].gold;

    bool err = false;
    LoadGameSave("continue.sav", &err);
    if (continuePoint && gameState == GAMESTATE_INGAME)
    {
        continuePoint->time = gameStats.timeTaken;
        continuePoint->gold = goldBefore;

    }
    if (!err)
        RunGameSave(continuePoint);

}

bool TransitionTo(GameState state)
{
    if (transitioningTo == state)
        return false;
    transitionAudioPlayed = false;
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
    transitionDrawing = TRANSITION_CHAINS;

    //transitioningTo = GAMESTATE_LOAD_ENCOUNTER;
    //transitionTimer = 0;
    TransitionTo(GAMESTATE_LOAD_ENCOUNTER);

    SetLoadscreen(sprites[currEncounterRunning->loadScreen_spriteIndex].path,NULL,1,1,1,1,1,e->name,"Press any key to begin.");

    if (toSpawn)
        free(toSpawn);
    if (initialSelection)
        free(initialSelection);
    toSpawn = calloc(e->numUnitsToSelect,sizeof(GameObject*));
    initialSelection = calloc(e->numUnitsToSelect,sizeof(GameObject*));

    if (deadFriendlyObjects)
        free(deadFriendlyObjects);
    deadFriendlyObjects = calloc(e->numUnitsToSelect,sizeof(GameObject));
    numDeadFriendlyObjectsHeld = e->numUnitsToSelect;
    for (int i = 0; i < numObjectsToAdd; i++)
    {
        toSpawn[i] = list[i];
        initialSelection[i] = list[i];
    }
    encounterGoingTo = e;
    numUnitsToSpawn = e->numUnitsToSelect;
}
void SetGameStateToInGame()
{
    transitionDrawing = TRANSITION_CIRCLE;

    //transitioningTo = GAMESTATE_INGAME;
    //transitionTimer = 0;
    TransitionTo(GAMESTATE_INGAME);
    StopMusic();



}
void SetGameStateToChoosingEncounter()
{
    //transitionDrawing = TRANSITION_CHAINS;
    transitionDrawing = TRANSITION_DOOR;

    //gameState = GAMESTATE_INGAME;
    //transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;
    //transitionTimer = 0;
    TransitionTo(GAMESTATE_CHOOSING_ENCOUNTER);

}
void SetGameStateToContinueSave()
{
    transitionDrawing = TRANSITION_DOOR;
    TransitionTo(GAMESTATE_CONTINUE_SAVE);
}
void SetGameStateToEnterShop()
{
    transitionDrawing = TRANSITION_STAIRS;
    
}
void SpawnAllFriendlies()
{   
    int xPos = currMap->spawnPoint.x;
    int yPos = currMap->spawnPoint.y;
    Point camPos = (Point){0,0};

    if (toSpawn)
        for (int i = 0; i < numUnitsToSpawn; i++)
        {
            if (toSpawn[i] == NULL)
                continue;
            GameObject* g = toSpawn[i];
            GameObject* gNew = AddGameobject(g,xPos,yPos,SOURCE_SPAWNED_FROM_MAP,TYPE_FRIENDLY);   
            //for (int j = 0; j < INVENTORY_SLOTS; j++)
              //  ItemOnMapChange(&gNew->inventory[j],gNew);
            //for (int j = 0; j < INVENTORY_SLOTS; j++)
           // {
             //   if (g->inventory[j].path)
               // {
                 //   Item* i = GetItemFromPath(g->inventory[j].path);
                  //  AttachItem(gNew,i);
               // } 
            //}
            HoldCommand(gNew,false);
            xPos += GetWidth(g)+4;
            if (i == numUnitsToSpawn/2)
            {
                camPos.x = xPos;
                camPos.y = yPos;
            }
        }
    FocusCameraOnPos(camPos.x,camPos.y);


}
void FinishTransition()
{
    ClearChatbox();
    RemoveAllAttacks();
    ChangeUIPanel(NULL);
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

        ClearGold();
        AddGold(players[0].bankedGold);

    }
    if (transitioningTo == GAMESTATE_INGAME)
    {
        gameStats.timeTaken = 0;
        NewReplay();    
        RemoveAllGameObjects();
        RemoveAllAttacks();

        combatStarted = false;


        ClearSpriteDecorations();
        //AddClouds(12);
        RandomSpriteDecor(16,dirtSprites,numDirtSprites);
        if (!_COMING_FROM_EDITOR)
            SetMap(LoadMap(encounterGoingTo->mapPath));
        if (_COMING_FROM_EDITOR)
            SetMap(LoadMap(currMap->path));

        int xPos = currMap->spawnPoint.x;
        int yPos = currMap->spawnPoint.y;

        SpawnAllFriendlies();

        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsOwnedByPlayer(activeObjects[i]))
            {
                HoldCommand(activeObjects[i],false);
            }
            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                Item* it = &activeObjects[i]->inventory[j];
                ItemOnMapChange(it,activeObjects[i]);
            }   

        }
        memset(&gameStats,0,sizeof(GameState));

        ClearGold();


        for (int i = 0; i < numActiveObjects; i++)
        {
            //CureAll(activeObjects[i]);
        }


        ClearSelection();

        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;


        memset(&ui.openedChests,0,sizeof(bool) * MAX_CHESTS);
        for (int i = 0; i < MAX_CHESTS; i++)
            ui.currChestAnimation[i] = ui.chestIdle;
        //free(toSpawn);

        GameObject** objectsToSave = calloc(numActiveObjects,sizeof(GameObject*));
        int numObjectsToSave = 0;
        for (int i = 0; i < numActiveObjects; i++)
        {
            GameObject* g = activeObjects[i];
            if (IsOwnedByPlayer(g))
            {
                objectsToSave[numObjectsToSave] = g;
                numObjectsToSave++;
            }
        }

        GameSave ga = SaveGameState(currMap->path,currEncounterRunning->path,players[0].gold,gameStats.timeTaken,numObjectsToSave,objectsToSave);
        GameSaveToDisk("continue.sav",&ga);
        DestroyGameSave(&ga);
        free(objectsToSave);


    }
    if (transitioningTo == GAMESTATE_CHOOSING_ENCOUNTER)
    {
        gameState = GAMESTATE_CHOOSING_ENCOUNTER;
        transitioningTo = GAMESTATE_CHOOSING_ENCOUNTER;

        ui.currentPanel = NULL;
        RemoveAllAttacks();

        ClearSelection();
        RemoveAllGameObjects();
        RemoveAllAttacks();
        //ChangeButtonText(GetButtonB(&ui.mainMenuPanel,"Return"),"Return");
        combatStarted = false;

        ClearGold();
        AddGold(players[0].bankedGold);
        




    }
        if (transitioningTo == GAMESTATE_WATCHING_REPLAY)
    {
        gameState = GAMESTATE_WATCHING_REPLAY;
        transitioningTo = GAMESTATE_WATCHING_REPLAY;

    }
    if (transitioningTo == GAMESTATE_SOFT_LOSS)
    {

        float gold = players[0].gold;

        RemoveAllAttacks();
        RemoveAllGameObjects();
        combatStarted = false;
        SetMap(LoadMap(currMap->path));
        SpawnAllFriendlies();
        
        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        ClearGold();
        AddGold(gold);

        numDeadFriendlyObjects = 0;

        return;
    }
    if (transitioningTo == GAMESTATE_LOAD_ENCOUNTER)
    {

        RemoveAllAttacks();
        gameState = GAMESTATE_LOAD_ENCOUNTER;
        transitioningTo = GAMESTATE_LOAD_ENCOUNTER;
        combatStarted = false;

        numDeadFriendlyObjects = 0;

        

        //if (toSpawn)
         //   free(toSpawn);
       // toSpawn = NULL;

    }

    if (transitioningTo == GAMESTATE_CHOOSING_UNITS)
    {
        gameState = GAMESTATE_CHOOSING_UNITS;
        transitioningTo = GAMESTATE_CHOOSING_UNITS;
        RemoveAllGameObjects();
        RemoveAllAttacks();
        SetMap(LoadMap("assets/ui/map_unitselect.lua"));

        free(currEncounterRunning->musicPath);
        currEncounterRunning->musicPath = NULL;

        //SetMap(&maps[0]);
        players[0].cameraPos.x = 0;
        players[0].cameraPos.y = 0;
        combatStarted = false;
        SpawnPartySelects();

        for (int i = 0; i < numActiveObjects; i++)
        {
            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                Item* it = &activeObjects[i]->inventory[j];
                ItemOnMapChange(it,activeObjects[i]);
            }   
        }

        ClearGold();
        AddGold(players[0].bankedGold);

        numDeadFriendlyObjects = 0;

    }
    if (transitioningTo == GAMESTATE_END)
    {
        gameState = GAMESTATE_END;
        transitioningTo = GAMESTATE_END;
        StopMusic();
        combatStarted = false;

        SetMap(&maps[0]);

        if (gameStats.gameWon)
        {
            players[0].bankedGold += players[0].gold;
            Save("_save.save");
        }
        numDeadFriendlyObjects = 0;


        AddGold(players[0].bankedGold);


    }
    if (transitioningTo == GAMESTATE_IN_SHOP)
    {
        MoveCam(0,0);
        gameState = GAMESTATE_IN_SHOP;

        if (!currEncounterRunning->hardLoss)
        {
            for (int i = 0; i < numDeadFriendlyObjects; i++)
            {
                RessurectGameObject(&deadFriendlyObjects[i]);
            }
        }
        numDeadFriendlyObjects = 0;

        RefreshShop();

    }
    if (transitioningTo == GAMESTATE_CONTINUE_SAVE)
    {
        RemoveAllGameObjects();
        RemoveAllAttacks();

        Encounter* e;
        for (int i = 0; i < numEncounters; i++)
        {
            if (strcmp(continuePoint->encounterPath,encounters[i]->path) == 0)
            {
                e = encounters[i];
            }
        }
        if (!e)
            return;

        currEncounterRunning = e;
            SetMap(LoadMap(continuePoint->mapPath));
        players[0].gold = continuePoint->gold;
        gameStats.timeTaken = continuePoint->time;


        if (toSpawn)
            free(toSpawn);
        toSpawn = calloc(continuePoint->numObjects,sizeof(GameObject*));
        if (deadFriendlyObjects)
            free(deadFriendlyObjects);
        deadFriendlyObjects = calloc(continuePoint->numObjects,sizeof(GameObject));
        numDeadFriendlyObjects = 0;
        numDeadFriendlyObjectsHeld = continuePoint->numObjects;
        for (int i = 0; i < continuePoint->numObjects; i++)
        {
            toSpawn[i] = LoadPrefab(continuePoint->objects[i].path);
        }
        encounterGoingTo = e;
        numUnitsToSpawn = continuePoint->numObjects;

        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        for (int i = 0; i < continuePoint->numObjects; i++)
        {
            GameObject* g = AddGameobject(toSpawn[i],continuePoint->objects[i].position.worldX,continuePoint->objects[i].position.worldY,SOURCE_SPAWNED_FROM_MAP,0);
            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                if (continuePoint->objects[i].inventory[j].path)
                {
                    AttachItem(g,GetItemFromPath(continuePoint->objects[i].inventory[j].path));
                }
            }
        }
        int xPos = currMap->spawnPoint.x + (continuePoint->numObjects*16)/2;
        int yPos = currMap->spawnPoint.y;

        FocusCameraOnPos(xPos,yPos);

        //SpawnAllFriendlies();
        DestroyGameSave(continuePoint);
    }
    if (transitioningTo == GAMESTATE_CHOOSING_ARTIFACTS)
    {
        gameState = GAMESTATE_CHOOSING_ARTIFACTS;
        transitioningTo = GAMESTATE_CHOOSING_ARTIFACTS;
    }
    if (transitioningTo == GAMESTATE_CHANGE_MAP)
    {
        RemoveAllAttacks();

        combatStarted = false;      

        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        //ending the map early map generate gold events from killing objs
        int goldBefore = players[0].gold;
        for (int i = 0; i < MAX_OBJS; i++)
        {
            if (!IsOwnedByPlayer(&objects[i]) || objects[i].objectIsSummoned)
            {
                KillObj(&objects[i],false,false);
            }
        }
        RemoveAllAttacks();

        AddGold(goldBefore - players[0].gold);

        SetMap(LoadMap(pathToNextMap));

        int xPos = currMap->spawnPoint.x;
        int yPos = currMap->spawnPoint.y;

        Point camPos = (Point){0,0};
        
        if (!currEncounterRunning->hardLoss)
        {
            for (int i = 0; i < numDeadFriendlyObjects; i++)
            {
                RessurectGameObject(&deadFriendlyObjects[i]);
            }
        }
        numDeadFriendlyObjects = 0;
        int index = 0;
        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsOwnedByPlayer(activeObjects[i]))
            {
               // objects[i].position.worldX = xPos;
                //objects[i].position.worldY = yPos;

                DeleteThreatList(activeObjects[i]);
                UpdateObjPosition(activeObjects[i],xPos,yPos);
                SetTargetPosition(activeObjects[i],xPos,yPos);

                Teleport(activeObjects[i],xPos,yPos,true);

                xPos += GetWidth(activeObjects[i]);
                if (index == encounterGoingTo->numUnitsToSelect/2)
                {
                    camPos.x = xPos;
                    camPos.y = yPos;    
                }
                index++;

                float healPercent = 0.25;
                float hpToHeal = activeObjects[i]->maxHP * healPercent;

                Heal(activeObjects[i],hpToHeal);
            }
            for (int j = 0; j < INVENTORY_SLOTS; j++)
            {
                Item* it = &activeObjects[i]->inventory[j];
                ItemOnMapChange(it,activeObjects[i]);
            }   

        }
        FocusCameraOnPos(camPos.x,camPos.y);

        for (int i = 0; i < numActiveObjects; i++)
        {
            CureAll(activeObjects[i]);
        }
        for (int i = 0; i < numActiveObjects; i++)
        {
            if (IsOwnedByPlayer(activeObjects[i]))
            {
                HoldCommand(activeObjects[i],false);
            }
        }
        GameObject** objectsToSave = calloc(numActiveObjects,sizeof(GameObject*));
        int numObjectsToSave = 0;
        for (int i = 0; i < numActiveObjects; i++)
        {
            GameObject* g = activeObjects[i];
            if (IsOwnedByPlayer(g))
            {
                objectsToSave[numObjectsToSave] = g;
                numObjectsToSave++;
            }
        }
        GameSave ga = SaveGameState(pathToNextMap,currEncounterRunning->path,players[0].gold,gameStats.timeTaken,numObjectsToSave,objectsToSave);
        GameSaveToDisk("continue.sav",&ga);
        DestroyGameSave(&ga);
        free(objectsToSave);
        free(pathToNextMap);
        pathToNextMap = NULL;
        ClearSelection();

    }
    //ClearParticles();

    _COMING_FROM_EDITOR = false;
}
void SetGameStateToPurchasingUnits()
{
    transitionDrawing = TRANSITION_MOUNTAINS;
    TransitionTo(GAMESTATE_PURCHASING_UNITS);
}
void SetGameStateToWatchingReplay()
{
    transitionDrawing = TRANSITION_CHAINS;
    TransitionTo(GAMESTATE_WATCHING_REPLAY);
    ChangeUIPanel(NULL);
}
void SetGameStateToInShop()
{
    transitionDrawing = TRANSITION_STAIRS;
    TransitionTo(GAMESTATE_IN_SHOP);
    StopMusic();
}
void SetGameStateToChangingMap()
{
    transitionDrawing = TRANSITION_CHAINS;
    TransitionTo(GAMESTATE_CHANGE_MAP);
    currEncounterRunning->goingToShop = false;
}
void SetGameStateToSoftLoss()
{
    transitionDrawing = TRANSITION_CHAINS;
    TransitionTo(GAMESTATE_SOFT_LOSS);

}
void SpawnPartySelects()
{
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

            int x=0; int y=0;
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


            GameObject* gNew = AddGameobject(g,x,y,SOURCE_SPAWNED_FROM_MAP,TYPE_FRIENDLY);
            HoldCommand(gNew,false);
            
        }   
    }

}
void SetGameStateToChoosingParty()
{
    transitionDrawing = TRANSITION_TRIANGLES;
    TransitionTo(GAMESTATE_CHOOSING_UNITS);
    /*
    if (TransitionTo(GAMESTATE_CHOOSING_UNITS))
    {
        SpawnPartySelects();
    }*/

}
void Quit()
{
    gameState = GAMESTATE_EXIT;
}
void WinGame()
{
    gameStats.gameWon = true;
    console.active = false;
    
    if (players[0].gold > 0)
    {
        SetEncounterProfited(currEncounterRunning,players[0].gold);
    }
    SetGameStateToEndscreen();

    lua_rawgeti(luaState,LUA_REGISTRYINDEX,currEncounterRunning->luafunc_win);
    lua_pcall(luaState,0,0,0);

    if (FileExists("continue.sav"))
    {
        al_remove_filename("continue.sav");    
    }
}
void LoseGame()
{
    gameStats.gameWon = false;
    console.active = false;
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
    if (currEncounterRunning)
    {
        if (GetNumPlayerControlledObjs(&players[0]) == 0 && gameState == GAMESTATE_INGAME)
        {
            if (currEncounterRunning->hardLoss)
                LoseGame();
            else
            {
                Retry();
            }
        }
    }
}

void UpdateTransition(float dt)
{
    if (GameStateIsTransition(&gameState))
    {   
        if (transitionDrawing == TRANSITION_CHAINS)
        {
            if (transitionTimer == 0)
            {
                int sound = LoadSound("assets/audio/chain.wav");
                Sound* s = &sounds[sound];
                PlaySound(s,1,0,true);
            }
            if (transitionTimer < 0.4f)
            {
                transitionTimer += dt * 3;
            }
            else
            {
                transitionTimer += dt * 0.8f;
            }
            if (transitionAudioPlayed == false && transitionTimer >= 0.725f)
            {
                int sound = LoadSound("assets/audio/slam.wav");
                Sound* s = &sounds[sound];
                PlaySound(s,1,0,true);

                sound = LoadSound("assets/audio/chain_pull_up.wav");
                s = &sounds[sound];
                PlaySound(s,1,0,true);


                transitionAudioPlayed = true;
                AddScreenshake(6,0.1f);
            }
        }
        else if (transitionDrawing == TRANSITION_TRIANGLES)
        {
            if (transitionTimer == 0)
            {
                int sound = LoadSound("assets/audio/transition_triangles.wav");
                Sound* s = &sounds[sound];
                PlaySound(s,1,0,true);
            }
            if (transitionTimer < 0.3f)
            {
                transitionTimer += dt * 3;
            }
            else
                transitionTimer += dt * 0.8f;
        }
        else if (transitionDrawing == TRANSITION_CIRCLE)
        {
            if (transitionTimer == 0)
            {
                int sound = LoadSound("assets/audio/transition_circle.wav");
                Sound* s = &sounds[sound];
                PlaySound(s,1,0,true);
            }
            transitionTimer += dt * 1.6f;
        }
        else if (transitionDrawing == TRANSITION_MOUNTAINS)
        {
            transitionTimer += dt;
        }
         else if (transitionDrawing == TRANSITION_DOOR)
        {
            if (transitionTimer == 0)
            {
                int sound = LoadSound("assets/audio/door_slide.wav");
                Sound* s = &sounds[sound];
                PlaySound(s,1,0,true);
            }
            if (transitionAudioPlayed == false && transitionTimer >= 0.85f)
            {
                int sound = LoadSound("assets/audio/slam_deep.wav");
                Sound* s = &sounds[sound];
                PlaySound(s,1,0,true);

                transitionAudioPlayed = true;
                AddScreenshake(3.5f,0.1f);

                float numParticlesToSpawn = 40;
                for (int i = 0; i < numParticlesToSpawn; i++)
                {
                    int randX = (_SCREEN_SIZE/2) + RandRange(-3,3);
                    int randY = RandRange(0,_SCREEN_SIZE);

                    AddParticleWithRandomProperties(randX,randY,COLOR_FRIENDLY,0.6f,1.4f,12,36,-2*M_PI,2*M_PI);
                }
            }
            if (transitionTimer < 0.4f)
            {
                transitionTimer += dt * 2;
            }
            else
            {
                transitionTimer += dt * 1.8f;
            }

        }

        else
        {
            if (transitionTimer < 0.4f)
            {
                transitionTimer += dt * 2;
            }
            else
            {
                transitionTimer += dt * 1.8f;
            }
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
    if (!GameStateIsTransition(&gameState) && transitionTimer <= 0)
        transitionDrawing = TRANSITION_NONE;
}


#define TRANSITION_CHAIN_SIZE 5
#define LATTICE_DISTANCE 20


void DrawTransition_Chains(float dt)
{

    float h = (_SCREEN_SIZE+10) * (easeInOutBack(transitionTimer)) - 1;
    DrawFilledRectangle(0,0,_SCREEN_SIZE,h,BG);

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
void DrawTransition_Triangle(float dt)
{
    double p = easeInOutQuint(transitionTimer);

    float move1 = (1-p) * _SCREEN_SIZE;
    float move2 = p * _SCREEN_SIZE;
    float move3 = p * _SCREEN_SIZE;


    float x1 = 0;
    float y1 = _SCREEN_SIZE + move1;
    
    float x2 = _SCREEN_SIZE;
    float y2 = _SCREEN_SIZE + move1;

    float x3 = _SCREEN_SIZE/2.0f;
    float y3 = 0 + move1;




    al_draw_filled_triangle(x1,y1,x2,y2,x3,y3,FRIENDLY);
    
    x1 = 0;
    y1 = -_SCREEN_SIZE   + move2;
    
    x2 = _SCREEN_SIZE/2.0f;
    y2 = -_SCREEN_SIZE + move2;

    x3 = 0;
    y3 = 0 + move2;

    al_draw_filled_triangle(x1,y1,x2,y2,x3,y3,FRIENDLY);

    x1 = _SCREEN_SIZE;
    y1 = -_SCREEN_SIZE   + move2;
    
    x2 = _SCREEN_SIZE/2.0f;
    y2 = -_SCREEN_SIZE + move2;

    x3 = _SCREEN_SIZE;
    y3 = 0 + move2;

    al_draw_filled_triangle(x1,y1,x2,y2,x3,y3,FRIENDLY);


}
void DrawTransition_Stairs(float dt)
{
    int numStairs = 12  ;
    double widthSingle = _SCREEN_SIZE / (float)numStairs;

    for (int i = 0; i < numStairs; i++)
    {
        double p = (EaseOutQuint(transitionTimer)); 

        p -= ((i) / (float)numStairs)/(double)numStairs;
        p = clamp(p,0,1);


        float x = i * widthSingle;
        float y = floor(((_SCREEN_SIZE/(double)numStairs)*(i+1)) * p);
        float y2 = floor(((_SCREEN_SIZE/(double)numStairs)*(numStairs-(i))) * p);

        DrawFilledRectangle(x,0,x+widthSingle,y2,BG);
        DrawFilledRectangle(x,_SCREEN_SIZE,x+widthSingle,_SCREEN_SIZE-y,FRIENDLY);
        
    }

}
void DrawTransition_Circle(float dt)
{
    float p = easeInOutSine(transitionTimer);

    al_draw_filled_circle(_SCREEN_SIZE/2.0f,_SCREEN_SIZE/2.0f,_SCREEN_SIZE*p,BG);

}
void DrawTransition_Mountains(float dt)
{
    int numMountains = 1;
    int w = _SCREEN_SIZE / numMountains;
    int h = _SCREEN_SIZE;

    double p = easeInOutQuint(transitionTimer);
    
    int offsetY = _SCREEN_SIZE - (_SCREEN_SIZE * p) * 2;

    for (int i = 0; i < numMountains; i++)
    {
        int offsetX = (w/2.0f) + (i * w);
        
        float x1 = offsetX - w/2.0f;
        float y1 = h + offsetY;

        float x2 = offsetX;
        float y2 = offsetY;

        float x3 = offsetX + w/2.0f;
        float y3 = h + offsetY;


        al_draw_filled_triangle(x1,y1,x2,y2,x3,y3,FRIENDLY);
        DrawFilledRectangle(x1,y1,x3,_SCREEN_SIZE,FRIENDLY);

    }
}
void DrawTransition_Chevrons(float dt)
{
    int numChevrons = 16;
    float singleHeight = (_SCREEN_SIZE*2) / numChevrons;



    for (int i = 0; i < numChevrons; i++)
    {
        float p = easeInOutSine(transitionTimer);


        int offset = singleHeight * i * p;
        ALLEGRO_COLOR col = i % 2 == 0 ? BG : FRIENDLY;
        ALLEGRO_VERTEX chevron[] = {
            {.x = 0, .y = _SCREEN_SIZE, .z = 0, .u = 128, .v = 0, .color = col},
            {.x = _SCREEN_SIZE/2.0f, .y = _SCREEN_SIZE - singleHeight, .z = 0, .u = 256, .v = 256, .color = col},
            {.x = _SCREEN_SIZE, .y = _SCREEN_SIZE, .z = 0, .u = 0, .v = 256, .color = col},
            
            {.x = _SCREEN_SIZE, .y = _SCREEN_SIZE + singleHeight, .z = 0, .u = 0, .v = 256, .color = col},
            {.x = _SCREEN_SIZE/2.0f, .y = _SCREEN_SIZE+singleHeight/2, .z = 0, .u = 256, .v = 256, .color = col},
            {.x = 0, .y = _SCREEN_SIZE + singleHeight, .z = 0, .u = 256, .v = 256, .color = col}
        };

        for (int z = 0; z < 6; z++)
        {
            chevron[z].y += singleHeight;
            chevron[z].y -= offset;
        }

        al_draw_prim(chevron, NULL, 0, 0, 6, ALLEGRO_PRIM_TRIANGLE_FAN);
        
    }
}

void DrawTransition_Door(float dt)
{
    float w = (_SCREEN_SIZE/2.0f) * (easeOutCubic(transitionTimer));
    DrawFilledRectangle(0,0,w,_SCREEN_SIZE,BG);
    DrawFilledRectangle(_SCREEN_SIZE,0,_SCREEN_SIZE-w,_SCREEN_SIZE,BG);

    float hingeX = w - _SCREEN_SIZE/2;
    float hingeY = 30;

    float hingeY2 = _SCREEN_SIZE - hingeY;

    int doorIndL = LoadSprite("assets/ui/transitions/door/door_l.png",false);
    int doorIndR = LoadSprite("assets/ui/transitions/door/door_r.png",false);


    DrawSprite(&sprites[doorIndL],w-_SCREEN_SIZE/2.0f,0,0,0,0,FRIENDLY,false,false,false);
    DrawSprite(&sprites[doorIndR],_SCREEN_SIZE-w,0,0.5f,0.5f,0,FRIENDLY,false,true,false);
}
void DrawTransition(float dt)
{   
    if (transitionDrawing == TRANSITION_CHAINS)
    {
        DrawTransition_Chains(dt);
    }
    if (transitionDrawing == TRANSITION_DOOR)
    {
        DrawTransition_Door(dt);
    }

    if (transitionDrawing == TRANSITION_STAIRS)
    {
        DrawTransition_Stairs(dt);
    }
    if (transitionDrawing == TRANSITION_TRIANGLES)
    {
        DrawTransition_Triangle(dt);
    }
    if (transitionDrawing == TRANSITION_CHEVRONS)
    {
        DrawTransition_Chevrons(dt);
    }
    if (transitionDrawing == TRANSITION_MOUNTAINS)
    {
        DrawTransition_Mountains(dt);
    }

    if (transitionDrawing == TRANSITION_CIRCLE)
    {
        DrawTransition_Circle(dt);
    }
}
bool GameStateIsTransition(GameState* g)
{
    return *g != transitioningTo;//(*g == GAMESTATE_TRANSITION_TO_CHOOSING_ENCOUNTER || *g == GAMESTATE_TRANSITION_TO_CHOOSING_UNITS || *g == GAMESTATE_TRANSITION_TO_INGAME || *g == GAMESTATE_TRANSITION_TO_END);
}
void SetGameStateToEndscreen()
{
    transitionDrawing = TRANSITION_CIRCLE;
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
    transitionDrawing = TRANSITION_CHAINS;
    StopMusic();
    TransitionTo(GAMESTATE_MAIN_MENU);
}
void SetGameStateToInEditor()
{
    gameState = GAMESTATE_IN_EDITOR;
    transitioningTo = GAMESTATE_IN_EDITOR;
    EditorSetMap(currMap->path);
    players[0].abilityHeld = NULL;
}

void DrawStartScreen()
{
    
}
bool GameIsPaused() 
{
    return (console.active || ui.currentPanel || (gameState == GAMESTATE_MAIN_MENU || gameState == GAMESTATE_LOAD_SCREEN || gameState == GAMESTATE_LOAD_ENCOUNTER || gameState == GAMESTATE_IN_CHATBOX || gameState == GAMESTATE_END) || gameState == GAMESTATE_IN_SHOP || gameState == GAMESTATE_IN_EDITOR);
}
bool GameIsIngame()
{
    return (gameState == GAMESTATE_CHOOSING_UNITS || gameState == GAMESTATE_INGAME || gameState == GAMESTATE_IN_CHATBOX);
}
void RessurectGameObject(GameObject* g)
{
    int index = g - deadFriendlyObjects;
    AddGameobject(g->prefab,0,0,SOURCE_SPAWNED_FROM_MAP,TYPE_FRIENDLY);

    deadFriendlyObjects[index].properties = 0;
    
}
void AddDeadGameObject(GameObject* g)
{
    if (gameState == GAMESTATE_INGAME)
    {
        if (numDeadFriendlyObjects >= numDeadFriendlyObjectsHeld)
        {
            deadFriendlyObjects = realloc(deadFriendlyObjects,(numDeadFriendlyObjectsHeld+1)*sizeof(GameObject));
            numDeadFriendlyObjects = numDeadFriendlyObjectsHeld;
            numDeadFriendlyObjectsHeld = numDeadFriendlyObjectsHeld+1; 
        }
        deadFriendlyObjects[numDeadFriendlyObjects] = *g;
        deadFriendlyObjects[numDeadFriendlyObjects].properties |= OBJ_ACTIVE;   
        numDeadFriendlyObjects++;
        /*
        for (int i = 0; i < currEncounterRunning->numUnitsToSelect; i++)
        {
            if (!(deadFriendlyObjects[i].properties & OBJ_ACTIVE))
            {
                deadFriendlyObjects[i] = *g;
                deadFriendlyObjects[i].properties |= OBJ_ACTIVE;   
                return;
            }
        }*/
    }
}
void GoTutorial()
{
    currEncounterRunning = GetEncounterByName("Tutorial");
    SetGameStateToLoadingEncounter(NULL,0,currEncounterRunning);
    ChangeUIPanel(NULL);
}
bool GameStateIsDrawingMap(GameState gameState)
{
    return (gameState == GAMESTATE_INGAME || gameState == GAMESTATE_CHOOSING_UNITS || gameState == GAMESTATE_IN_CHATBOX || gameState == GAMESTATE_IN_EDITOR);
}