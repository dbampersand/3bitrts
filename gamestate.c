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

 Encounter* encounterGoingTo = NULL;
 char* pathToNextMap = NULL;

GameState gameState = {0};

TRANSITION transitionDrawing;


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
void SetGameStateToEnterShop()
{
    transitionDrawing = TRANSITION_STAIRS;
    
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

        ClearSpriteDecorations();
        //AddClouds(12);
        RandomSpriteDecor(16,dirtSprites,numDirtSprites);
        if (!_COMING_FROM_EDITOR)
            SetMap(LoadMap(encounterGoingTo->mapPath));
        if (_COMING_FROM_EDITOR)
            SetMap(LoadMap(currMap->path));

        int xPos = currMap->spawnPoint.x;
        int yPos = currMap->spawnPoint.y;

        Point camPos = (Point){0,0};


         
        if (toSpawn)
            for (int i = 0; i < encounterGoingTo->numUnitsToSelect; i++)
            {
                if (toSpawn[i] == NULL)
                    continue;
                GameObject* g = toSpawn[i];
                GameObject* gNew = AddGameobject(g,xPos,yPos,SOURCE_SPAWNED_FROM_MAP,TYPE_FRIENDLY);   
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
            if (IsOwnedByPlayer(activeObjects[i]))
            {
                HoldCommand(activeObjects[i],false);
            }
        }
        memset(&gameStats,0,sizeof(GameState));

        ClearGold();


        for (int i = 0; i < numActiveObjects; i++)
        {
            CureAll(activeObjects[i]);
        }


        ClearSelection();

        gameState = GAMESTATE_INGAME;
        transitioningTo = GAMESTATE_INGAME;

        FocusCameraOnPos(camPos.x,camPos.y);

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

        ClearGold();
        AddGold(players[0].bankedGold);
        




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
        RemoveAllGameObjects();
        RemoveAllAttacks();
        SetMap(LoadMap("assets/ui/map_unitselect.lua"));

        //SetMap(&maps[0]);
        players[0].cameraPos.x = 0;
        players[0].cameraPos.y = 0;
        combatStarted = false;
        SpawnPartySelects();

        ClearGold();
        AddGold(players[0].bankedGold);



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

    }
    if (transitioningTo == GAMESTATE_IN_SHOP)
    {
        MoveCam(0,0);
        gameState = GAMESTATE_IN_SHOP;
        RefreshShop();


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
            if (!IsOwnedByPlayer(&objects[i]))
            {
                KillObj(&objects[i],false,false);
            }
        }
        AddGold(goldBefore - players[0].gold);

        SetMap(LoadMap(pathToNextMap));

        int xPos = currMap->spawnPoint.x;
        int yPos = currMap->spawnPoint.y;

        Point camPos = (Point){0,0};

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
                if (i == encounterGoingTo->numUnitsToSelect/2)
                {
                    camPos.x = xPos;
                    camPos.y = yPos;    
                }
            }
        }
        FocusCameraOnPos(camPos.x,camPos.y);

        printf("%i\n",attack_top);
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
    if (players[0].gold > 0)
    {
        SetEncounterProfited(currEncounterRunning,players[0].gold);
    }
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
        if (transitionDrawing == TRANSITION_CHAINS)
        {
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
                transitionAudioPlayed = true;
                AddScreenshake(6,0.1f);
            }
        }
        else if (transitionDrawing == TRANSITION_TRIANGLES)
        {
            if (transitionTimer < 0.3f)
            {
                transitionTimer += dt * 3;
            }
            else
                transitionTimer += dt * 0.8f;
        }
        else if (transitionDrawing == TRANSITION_CIRCLE)
        {
            transitionTimer += dt * 1.6f;
        }
        else if (transitionDrawing == TRANSITION_MOUNTAINS)
        {
            transitionTimer += dt;
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
        if (transitionDrawing == TRANSITION_DOOR)
        {
            if (transitionAudioPlayed == false && transitionTimer >= 0.925f)
            {
                transitionAudioPlayed = true;
                AddScreenshake(3.5f,0.1f);

                float numParticlesToSpawn = 40;
                for (int i = 0; i < numParticlesToSpawn; i++)
                {
                    int randX = (_SCREEN_SIZE/2) + RandRange(-3,3);
                    int randY = RandRange(0,_SCREEN_SIZE);

                    AddParticleWithRandomProperties(randX,randY,COLOR_FRIENDLY,0.6f,1.4f,0.2f,0.6f,-2*M_PI,2*M_PI);
                }
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

        al_draw_filled_rectangle(x,0,x+widthSingle,y2,BG);
        al_draw_filled_rectangle(x,_SCREEN_SIZE,x+widthSingle,_SCREEN_SIZE-y,FRIENDLY);
        
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
        al_draw_filled_rectangle(x1,y1,x3,_SCREEN_SIZE,FRIENDLY);

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
    al_draw_filled_rectangle(0,0,w,_SCREEN_SIZE,BG);
    al_draw_filled_rectangle(_SCREEN_SIZE,0,_SCREEN_SIZE-w,_SCREEN_SIZE,BG);

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
    AddGameobject(g,0,0,SOURCE_SPAWNED_FROM_MAP,TYPE_FRIENDLY);

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
bool GameStateIsDrawingMap(GameState gameState)
{
    return (gameState == GAMESTATE_INGAME || gameState == GAMESTATE_CHOOSING_UNITS || gameState == GAMESTATE_IN_CHATBOX || gameState == GAMESTATE_IN_EDITOR);
}