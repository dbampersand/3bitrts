#include "command.h"
#include "gameobject.h"
#include "helperfuncs.h"
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
    Command c = (Command){.x = g->x, .y = g->y, .commandType = COMMAND_STOP, .target = NULL, .ability = NULL};
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
        g->xtarg = c->x;
        g->ytarg = c->y;
        g->targObj = NULL;

        if (dist(g->x,g->y,c->x,c->y) <= DIST_DELTA)
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
        g->targObj = c->target;

        if (AbilityCanBeCast(c->ability,g,c->target,c->x,c->y))
        {
            CastAbility(g,c->ability,c->x,c->y,c->x-g->x,c->y-g->y,c->target);
            NextCommand(g);
            return;
        }
    }
    if (c->commandType == COMMAND_ATTACKMOVE)
    {
        g->xtarg = c->x;
        g->ytarg = c->y;
        g->targObj = NULL;

        if (dist(g->x,g->y,c->x,c->y) <= DIST_DELTA)
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