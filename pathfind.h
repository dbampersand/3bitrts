#pragma once

#include "point.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#define PATHFIND_DEPTH 256
#define TIEBREAK 1/(float)PATHFIND_DEPTH   

#define PATHFIND_SEARCH_MAX PATHFIND_DEPTH

typedef enum NODE_IN_SET
{
    NODE_FREE = 0,
    NODE_INSIDE_OPEN_LIST,
    NODE_INSIDE_CLOSED_LIST
} NODE_IN_SET;

typedef struct PathfindNode
{
    float f;
    float g;
    float h;
    PointI p;
    struct PathfindNode* parent;    

}PathfindNode;

typedef struct Queue
{
    PathfindNode elements[PATHFIND_SEARCH_MAX];
    uint32_t numElements;
} Queue;

typedef struct PathfindMap
{
    NODE_IN_SET* set;
    int sizex; int sizey;
} PathfindMap;

Queue openSet;
Queue closedSet;
PathfindMap pathfindmap;



typedef struct GameObject GameObject;


PathfindNode* Push(Queue* q, PathfindNode p, bool sort, NODE_IN_SET setToAdd);
PathfindNode Pop(Queue* q); 
PathfindNode* Peek(Queue* q); 

bool IsEmpty(Queue* q);
int Depth(Queue* q, PathfindNode* p);

void AStar(PointI here, PointI target, bool* success, float w, float h, GameObject* g);
void InitPathfinding();
void DebugDrawPathfinding();
