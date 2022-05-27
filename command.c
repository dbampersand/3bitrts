#include "command.h"
#include "gameobject.h"
#include "helperfuncs.h"
#include "ability.h"
void AddCommand(GameObject* g, Command c)
{
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
void MoveCommand(GameObject* g, float x, float y)
{
    if (!g) return;
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_MOVE, .target = NULL, .ability = NULL};
    AddCommand(g,c);
}
void AttackMoveCommand(GameObject* g, float x, float y)
{
    if (!g) return;
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_ATTACKMOVE, .target = NULL, .ability = NULL};
    AddCommand(g,c);
}

void AttackCommand(GameObject* g, GameObject* target)
{
    if (!g) return;
    float x = 0; float y = 0;
    GetCentre(target,&x,&y);
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_ATTACK, .target = target, .ability = NULL};
    AddCommand(g,c);
}
void CastCommand(GameObject* g, GameObject* target, Ability* a, float x, float y)
{
    if (!g) return;
    Command c = (Command){.x = x, .y = y, .commandType = COMMAND_CAST, .target = target, .ability = a};
    AddCommand(g,c);
}
void StopCommand(GameObject* g)
{
    if (!g) return;
    Command c = (Command){.x = g->position.x, .y = g->position.y, .commandType = COMMAND_STOP, .target = NULL, .ability = NULL};
    AddCommand(g,c);
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
        g->targetPosition.x = g->position.x;
        g->targetPosition.y = g->position.y;

    }
}
void DoCommands(GameObject* g)
{
    if (!g) return;
    Command* c = &g->queue[0];
    if (c->commandType == COMMAND_NONE)
    {
        return;
    }
    if (c->commandType == COMMAND_MOVE)
    {
        g->targetPosition.x = c->x;
        g->targetPosition.y = c->y;
        g->targObj = NULL;

        if (dist(g->position.x,g->position.y,c->x,c->y) <= DIST_DELTA)
        {
            NextCommand(g);
            return;
        }
    }
    if (c->commandType == COMMAND_ATTACK)
    {
        g->targObj = c->target;

        //if target dies
        if (!IsActive(c->target))
        {
            NextCommand(g);
            return;
        }
    }
    if (c->commandType == COMMAND_CAST)
    {
        if (c->target)
        {
            if (AbilityShouldBeCastOnTarget(c->ability))
                g->targObj = c->target;
        }

        if (AbilityCanBeCast(c->ability,g,c->target,c->x,c->y))
        {
            CastAbility(g,c->ability,c->x,c->y,c->x-g->position.x,c->y-g->position.y,c->target);
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
    }
    if (c->commandType == COMMAND_ATTACKMOVE)
    {
        g->targetPosition.x = c->x;
        g->targetPosition.y = c->y;
        g->targObj = NULL;

        if (dist(g->position.x,g->position.y,c->x,c->y) <= DIST_DELTA)
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