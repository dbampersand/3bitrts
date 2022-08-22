#include "pathfind.h"
#include "float.h"
#include "helperfuncs.h"
#include "math.h"
#include "map.h"
int Depth(Queue* q, PathfindNode* p)
{
    return p - q->elements;
}
bool IsEmpty(Queue* q)
{
    return (q->numElements == 0);
}
PathfindNode* Push(Queue* q, PathfindNode p)
{
    if (q->numElements >= PATHFIND_SEARCH_MAX)
    {
        return NULL;
    }
    q->elements[q->numElements] = p;
    q->numElements++;
    return &q->elements[q->numElements-1];
}
PathfindNode* Peek(Queue* q)
{
    return &q->elements[q->numElements-1];
}
bool IsInQueue(Queue* q, PathfindNode* p)
{
    for (int i = 0; i < q->numElements; i++)
    {
        if (PointIntIsEq(p->p,q->elements[i].p))
            return true;
    }
    return false;
}
PathfindNode RemoveAt(Queue* q, int index)
{
    PathfindNode n = q->elements[index];
    for (int i = index; i < q->numElements-1; i++)
    {
        q->elements[i] = q->elements[i+1];
    }
    q->numElements--;
    return n;
}
PathfindNode Pop(Queue* q)
{
    q->numElements--;
    return q->elements[q->numElements];
}
float h(PathfindNode* s, PointI end)
{
     return  dist(s->p.x,s->p.y,end.x,end.y);//sqrt(pow(s->p.x - end.x,2) + pow(s->p.y - end.y,2));
}
float g(PathfindNode* s, PointI start)
{
    return  dist(s->p.x,s->p.y,start.x,start.y);
}
void GenerateSucessors(PathfindNode* p, Queue* path, PathfindNode nodeArr[8])
{
    int index = 0;
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            if (x == 0 && y == 0)
                continue;
            PathfindNode successor = {0};
            successor.p.x =  p->p.x + x; 
            successor.p.y = p->p.y + y;
            successor.parent = p;
            //Push(path,successor);
            nodeArr[index] = successor;
            index++;
        }
    }
}
void UpdateFCost(PathfindNode* n, PointI current, PointI target, Queue* path)
{
    int idx = GetIndex(GetMapHeight()/_GRAIN, floor(n->p.x), floor(n->p.y));
    if (n->p.x < 0 || n->p.x >= GetMapWidth()/_GRAIN || n->p.y < 0 || n->p.y >= GetMapHeight()/_GRAIN )
    {
        n->fcost = FLT_MAX;
        return;
    }
    if (currMap->collision[idx] != COLLISION_OPEN)
        n->fcost = FLT_MAX;
    else
    {
        if (!PointIntIsEq(n->p,target) && Depth(path,n) == PATHFIND_SEARCH_MAX)
            n->fcost = FLT_MAX; 
        else
        {
            float parentCost;
            if (n->parent)
                parentCost = n->parent->fcost;
            else
                parentCost = 0;
            n->fcost = _MAX(parentCost, g(n,current) + h(n,target));

        }
    }

}
PathfindNode* GetLowestF(Queue* q)
{
    PathfindNode* lowest = &q->elements[0];
    for (int i = 0; i < q->numElements; i++)
    {
        if (q->elements[i].fcost < lowest->fcost)
            lowest = &q->elements[i];
    }
    return lowest;
}
PathfindNode* SuccessorsMinF(PathfindNode nodeArr[8])
{
    PathfindNode* lowest = &nodeArr[0];
    for (int i = 0; i < 8; i++)
    {
        if (nodeArr[i].fcost < lowest->fcost)
            lowest = &nodeArr[i];
    }
    return lowest;
}
PointI SMA(Queue* path, PointI current, PointI target, bool* success)
{
    PathfindNode currentSuccessors[8];
    Push(path, (PathfindNode){.fcost = 0, .p = current, .parent = NULL, .g = 0, .sucessors = {0}});
    while (1)
    {

        if (IsEmpty(path))
        {
            *success = false;
            return target;
        } 
        PathfindNode* node = GetLowestF(path);//Peek(path);
        if (PointIntIsEq(node->p,target))
        {
            *success = true;
            PathfindNode* last = node;
            while (1)
            {
                PathfindNode* new = last->parent;
                if (!new || !new->parent)
                    break;
                last = new;
            }
            path->numElements = 0;
            return last->p;
        } 

        int nodesInQueue = 0;
        int index = 0;
        GenerateSucessors(node, path, currentSuccessors);
        for (int i = 0; i < 8; i++)
        {   
            UpdateFCost(&currentSuccessors[i], current, target, path);
            if (IsInQueue(path,&currentSuccessors[i]))
                nodesInQueue++;
        }
        
        if (nodesInQueue == 8)
            RemoveAt(path, node - path->elements);
        
        if (path->numElements >= PATHFIND_SEARCH_MAX)
        {
            PathfindNode* badNode = &path->elements[0]; 
            for (int i = 0; i < path->numElements; i++)
            {
                PathfindNode* n = &path->elements[i];
                if (n->fcost > badNode->fcost)
                    badNode = n;
            }

            PathfindNode parent;
            PathfindNode* remove = badNode;
            while (1)
            {
                if (remove)
                {
                    parent = RemoveAt(path, remove - path->elements);
                }
                else
                    break;

                if (remove == parent.parent)
                    break;
                remove = parent.parent;
            }
        }
        /*if (nodesInQueue < 8)
            for (int i = 0; i < 8; i++)
            {
                node->sucessors[i] = Push(path,currentSuccessors[i]);
            }*/
        for (int i = 0; i < 8; i++)
        {
               // node->sucessors[i] = Push(path,currentSuccessors[i]);
            if (!IsInQueue(path,&currentSuccessors[i]))
                Push(path,currentSuccessors[i]);
            
        }
        node->fcost = FLT_MAX;
        
    }
}
