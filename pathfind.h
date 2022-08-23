#include "point.h"
#include "stdint.h"
#include "stdlib.h"
#define PATHFIND_DEPTH 256

#define PATHFIND_SEARCH_MAX PATHFIND_DEPTH*PATHFIND_DEPTH



typedef struct PathfindNode
{
    float fcost;
    float g;
    PointI p;
    struct PathfindNode* sucessors[8];
    struct PathfindNode* parent;
    bool isActive;
}PathfindNode;

typedef struct Queue
{
    PathfindNode elements[PATHFIND_SEARCH_MAX];
    uint32_t numElements;
} Queue;

Queue Path;

PathfindNode* Push(Queue* q, PathfindNode p);
PathfindNode Pop(Queue* q); 
PathfindNode* Peek(Queue* q); 
bool IsEmpty(Queue* q);
int Depth(Queue* q, PathfindNode* p);

PointI SMA(Queue* path, PointI current, PointI target, bool* success);
