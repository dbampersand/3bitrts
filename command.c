#include "command.h"

#include "gameobject.h"
#include "helperfuncs.h"
#include "ability.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "colors.h"
#include "player.h"
#include "map.h"
#include <math.h>
void DrawCommand(Command* c, int x, int y)
{
    if (c->commandType == COMMAND_NONE)
        return;
    if (c->commandType == COMMAND_MOVE)
        al_draw_rectangle(x-2,y-2,x+2,y+2,GetColor(queueCommandColors[COMMAND_MOVE],0),1);
    if (c->commandType == COMMAND_FOLLOW)
        al_draw_rectangle(x-2,y-2,x+2,y+2,GetColor(queueCommandColors[COMMAND_MOVE],0),1);
    if (c->commandType == COMMAND_ATTACK || c->commandType == COMMAND_ATTACKMOVE)
        al_draw_triangle(x-2,y-2,x+2,y+2,x,y-2,GetColor(queueCommandColors[c->commandType],0),1);
    if (c->commandType == COMMAND_CAST)
        al_draw_circle(x,y,2,GetColor(queueCommandColors[COMMAND_CAST],0),1);
    if (c->commandType == COMMAND_STOP)
        al_draw_filled_rectangle(x-2,y-2,x+2,y+2,GetColor(queueCommandColors[COMMAND_MOVE],0));
}
void DrawCommandQueue(GameObject* g)
{
    if (!g) return;
    if (g->queue[0].commandType == COMMAND_NONE)
        return;
    float cx; float cy;
    GetCentre(g,&cx,&cy);
    ToScreenSpace(&cx,&cy);

    float firstX = g->queue[0].x + GetWidth(g)/2;
    float firstY = g->queue[0].y + GetHeight(g)/2; 

    if (g->queue[0].target)
        GetCentre(g->queue[0].target,&firstX,&firstY);
    ToScreenSpace(&firstX,&firstY);


    DrawCommand(&g->queue[0],firstX,firstY);
    al_draw_line(firstX,firstY,cx,cy,GetColor(queueCommandColors[g->queue[0].commandType],0),1);

    for (int i = 1; i < MAX_QUEUED_CMD; i++)
    {
        Command* thisCmd = &g->queue[i];
        Command* nextCmd = &g->queue[i+1];
        Command* prevCmd = &g->queue[i-1];

        if (thisCmd->commandType == COMMAND_NONE)
            return;

        float tX = thisCmd->x + GetWidth(g)/2; float tY = thisCmd->y+ GetHeight(g)/2;
        float pX = prevCmd->x + GetWidth(g)/2; float pY = prevCmd->y + GetHeight(g)/2;


        if (prevCmd->target)
        {
            GetCentre(prevCmd->target,&pX,&pY);
        }
        if (thisCmd->target)
        {
            GetCentre(thisCmd->target,&tX,&tY);
        }

        ToScreenSpace(&tX,&tY);
        ToScreenSpace(&pX,&pY);

        
        if (thisCmd->commandType != COMMAND_NONE)
            al_draw_line(tX,tY,pX,pY,GetColor(queueCommandColors[thisCmd->commandType],0),1);
        DrawCommand(thisCmd,tX,tY);
    }
}

void AddCommand(GameObject* g, Command c, bool shiftHeld)
{
    if (!shiftHeld)
        ClearCommandQueue(g);
    for (int i = 0; i < MAX_QUEUED_CMD; i++)
    {
        Command* c2 = &g->queue[i];
        if (c2->commandType == COMMAND_NONE)
        {
            g->queue[i] = c;
            return;
        }
    }
}
void ClearCommandQueue(GameObject* g)
{
    if (!g) return;
    memset(&g->queue,0,sizeof(Command)*MAX_QUEUED_CMD);
}
void MoveCommand(GameObject* g, float x, float y, bool shiftHeld)
{
    if (!g) return;
    
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_MOVE, .target = NULL, .ability = NULL};
    AddCommand(g,c,shiftHeld);
}
void AttackMoveCommand(GameObject* g, float x, float y, bool shiftHeld)
{
    if (!g) return;
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_ATTACKMOVE, .target = NULL, .ability = NULL};
    AddCommand(g,c,shiftHeld);
}
void FollowCommand(GameObject* g, GameObject* target, bool shiftHeld)
{
    if (!g) return;
    float x = 0; float y = 0;
    GetCentre(target,&x,&y);

    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_FOLLOW, .target = target, .ability = NULL};
    AddCommand(g,c,shiftHeld);

}

void AttackCommand(GameObject* g, GameObject* target, bool shiftHeld)
{
    if (!g) return;
    float x = 0; float y = 0;
    GetCentre(target,&x,&y);
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_ATTACK, .target = target, .ability = NULL};
    AddCommand(g,c,shiftHeld);
}
void CastCommand(GameObject* g, GameObject* target, Ability* a, float x, float y, bool shiftHeld)
{
    if (!g) return;
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_CAST, .target = target, .ability = a};
    AddCommand(g,c,shiftHeld);
}
void StopCommand(GameObject* g, bool shiftHeld)
{
    if (!g) return;
    Command c = (Command){.x = g->position.worldX, .y = g->position.worldY, .commandType = COMMAND_STOP, .target = NULL, .ability = NULL};
    AddCommand(g,c,shiftHeld);
}
void HoldCommand(GameObject* g, bool shiftHeld)
{
    if (!g) return;
    Command c = (Command){.x = g->position.worldX, .y = g->position.worldY, .commandType = COMMAND_HOLD, .target = NULL, .ability = NULL};
    AddCommand(g,c,shiftHeld);
}
bool CmdEqual(Command c1, Command c2)
{
    return (c1.commandType == c2.commandType && c1.target == c2.target && c1.ability == c2.ability && c1.x == c2.x && c1.y == c2.y);
}
void NextCommand(GameObject* g)
{
    if (!g) return;
    for (int i = 0; i < MAX_QUEUED_CMD-1; i++)
    {
        g->queue[i] = g->queue[i+1];
    }
    memset(&g->queue[MAX_QUEUED_CMD-1],0,sizeof(Command));
    if (g->queue[0].commandType == COMMAND_NONE)
    {
        g->targObj = NULL;
        SetTargetPosition(g,g->position.worldX,g->position.worldY);
    }
}
void FindEnemiesToAttack(GameObject* g)
{
    for (int i = 0; i < numObjects; i++)
    {
        GameObject* g2 = &objects[i];  
        if (IsActive(g2) && GetPlayerOwnedBy(g) != GetPlayerOwnedBy(g2) && !ObjIsDecoration(g2))
        {
            if (GetDist(g,g2) <= g->aggroRadius)
            {
                AttackCommand(g,g2,false);
                return;
            }

        }

    }
}
bool IsIdle(GameObject* g)
{
    return g->queue[0].commandType == COMMAND_NONE;
}
void DoCommands(GameObject* g)
{
    if (!g) return;
    Command* c = &g->queue[0];
    if (c->commandType == COMMAND_HOLD)
    {
        g->targObj = NULL;
        SetTargetPosition(g,g->position.worldX,g->position.worldY);
        return;
    }

    if (c->commandType == COMMAND_NONE)
    {
        FindEnemiesToAttack(g);
    }
    if (c->commandType == COMMAND_MOVE)
    {
        SetTargetPosition(g,c->x,c->y);

        g->targObj = NULL;


        if (dist(g->position.worldX,g->position.worldY,c->x,c->y) <= DIST_DELTA)
        {
            NextCommand(g);
            return;
        }
    }
    if (c->commandType == COMMAND_FOLLOW)
    {
        if (!c->target)
            NextCommand(g);
        SetTargetPosition(g,c->target->position.worldX,c->target->position.worldY);


        if (dist(g->position.worldX,g->position.worldY,c->x,c->y) <= DIST_DELTA)
        {
            NextCommand(g);
            return;
        }
    }

    if (c->commandType == COMMAND_ATTACK)
    {
        if (!c->target)
            NextCommand(g);
        float targX = c->target->position.worldX;
        float targY = c->target->position.worldY;
        
        g->targObj = c->target;

        //if target dies
        if (!IsActive(c->target))
        {
            NextCommand(g);
            if (IsIdle(g))
            {
                AttackMoveCommand(g,targX,targY,false);
            }
            return;
        }
    }
    if (c->commandType == COMMAND_CAST)
    {
            if (c->target)
            {
                c->x = c->target->position.worldX;
                c->y = c->target->position.worldY;

                if (AbilityShouldBeCastOnTarget(c->ability))
                    g->targObj = c->target;
                
            }

            if (AbilityCanBeCast(c->ability,g,c->target,c->x,c->y))
            {
                Command before = *c;
                CastAbility(g,c->ability,c->x,c->y,c->x-g->position.worldX,c->y-g->position.worldY,c->target);
                if (!CmdEqual(before,g->queue[0]))
                {
                    //if casting the ability has modified the queue
                    //eg, SetMovePoint
                    return;
                }
                if (g->queue[1].commandType == COMMAND_NONE)
                {
                    if (c->target)
                    {
                        if (IsOwnedByPlayer(g) != IsOwnedByPlayer(c->target))
                        {
                            g->queue[0].commandType = COMMAND_ATTACK;
                            g->queue[0].ability = NULL; 

                        }
                        else
                            NextCommand(g);
                    }
                    else
                    {
                        NextCommand(g);
                    }
                }
                else
                {
                    if (c->target)
                    {

                        if (IsOwnedByPlayer(g) != IsOwnedByPlayer(c->target))
                        {
                            NextCommand(g);
                        }
                    }
                    else
                    {
                        NextCommand(g);
                    }
                }
                return;
        }
        else
        {
            if (c->target)
                SetTargetPosition(g,c->target->position.worldX,c->target->position.worldY);
            else
                SetTargetPosition(g,c->x,c->y);
        }
    }
    if (c->commandType == COMMAND_ATTACKMOVE)
    {
        SetTargetPosition(g,c->x,c->y);

        
        g->targObj = NULL;

        if (dist(g->position.worldX,g->position.worldY,c->x,c->y) <= DIST_DELTA)
        {
            NextCommand(g);
            return;
        }
        for (int i = 0; i < numObjects; i++)
        {
            GameObject* g2 = &objects[i];  
            if (IsActive(g2))
            {
                if (GetPlayerOwnedBy(g) != GetPlayerOwnedBy(g2))
                {
                    if (GetDist(g,g2) <= g->aggroRadius)
                    {
                        g->targObj = g2;
                        g->queue[0].commandType = COMMAND_ATTACK;
                        g->queue[0].target = g2;
                        return;  
                    }
                }
            }
        }

    }
    if (c->commandType == COMMAND_STOP)
    {
        NextCommand(g);
    }
}