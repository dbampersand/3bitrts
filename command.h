#pragma once
typedef enum QueueCommand
{
    COMMAND_NONE = 0,
    COMMAND_MOVE,
    COMMAND_ATTACK,
    COMMAND_CAST,
    COMMAND_STOP,
    COMMAND_ATTACKMOVE
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

void AddCommand(GameObject* g, Command c);
void MoveCommand(GameObject* g, float x, float y);
void AttackCommand(GameObject* g, GameObject* target);
void CastCommand(GameObject* g, GameObject* target, Ability* a, float x, float y);
void StopCommand(GameObject* g);
void DoCommands(GameObject* g);
void NextCommand(GameObject* g);
void AddCommand(GameObject* g, Command c);
void ClearCommandQueue(GameObject* g);
void AttackMoveCommand(GameObject* g, float x, float y);
