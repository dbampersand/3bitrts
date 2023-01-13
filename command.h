#pragma once
#include "colors.h"
#include <stdbool.h>

typedef enum QueueCommand
{
    COMMAND_NONE = 0,
    COMMAND_MOVE,
    COMMAND_ATTACK,
    COMMAND_CAST,
    COMMAND_STOP,
    COMMAND_ATTACKMOVE,
    COMMAND_FOLLOW,
    COMMAND_HOLD,


    COMMAND_ALL
} QueueCommand;

typedef struct GameObject GameObject;
typedef struct Ability Ability; 


typedef struct Command
{
    QueueCommand commandType;
    GameObject* target; 
    Ability* ability;
    float x; float y;
} Command;

extern Color queueCommandColors[COMMAND_ALL];


void AddCommand(GameObject* g, Command c, bool shiftHeld);
void MoveCommand(GameObject* g, float x, float y, bool shiftHeld);
void AttackCommand(GameObject* g, GameObject* target, bool shiftHeld);
void CastCommand(GameObject* g, GameObject* target, Ability* a, float x, float y, bool shiftHeld);
void StopCommand(GameObject* g, bool shiftHeld);
void FollowCommand(GameObject* g, GameObject* target, bool shiftHeld);
void HoldCommand(GameObject* g, bool shiftHeld);
void AttackMoveCommand(GameObject* g, float x, float y, bool shiftHeld);

int CountCommands(GameObject* g);
void DoCommands(GameObject* g);
void NextCommand(GameObject* g);
void ClearCommandQueue(GameObject* g);
void DrawCommandQueue(GameObject* g);

void AttackHighestThreatCommand(GameObject* g);
