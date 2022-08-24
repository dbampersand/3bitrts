#include "pathfind.h"
#include "float.h"
#include "helperfuncs.h"
#include "math.h"
#include "map.h"

PathfindNode Pop(Queue* q)
{
    q->numElements--;
    return q->elements[q->numElements];
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
PathfindNode* Peek(Queue* q)
{
    return &q->elements[q->numElements-1];
}
int IsInQueue(Queue* q, PathfindNode* p)
{
    for (int i = 0; i < q->numElements; i++)
    {
        if (PointIntIsEq(p->p,q->elements[i].p))
            return i;
    }
    return -1;
}
int cmpNode(const void* a, const void* b) {
    PathfindNode* p =  *(PathfindNode**)a;
    PathfindNode* p2 = *(PathfindNode**)b;
    return (p2->f - p->f);

}
void InsertAt(Queue* q, PathfindNode p, int index)
{
    q->numElements++;
    if (q->numElements-1 != index)
        for (int i = q->numElements-1; i > index; i--)
        {
            q->elements[i] = q->elements[i-1];
        }
    q->elements[index] = p;

}
void AddToMap(PathfindNode p, NODE_IN_SET set)
{
    int index = GetIndex(GetMapHeight()/(float)_GRAIN, p.p.x, p.p.y);
    pathfindmap.set[index] = set;
}
NODE_IN_SET GetSet(int index)
{
    return pathfindmap.set[index];
}
int DEBUG_NUMNODESPUSHED;
PathfindNode* Push(Queue* q, PathfindNode p, bool sort, NODE_IN_SET setToAdd)
{
    if (q->numElements >= PATHFIND_SEARCH_MAX)
    {
        return NULL;
    }
    if (sort)
        for (int i = 0; i < q->numElements; i++)
        {
            if (p.f > q->elements[i].f)
            {
                InsertAt(q,p,i);
                AddToMap(p,setToAdd);
                return &q->elements[i];
            }
        }
    InsertAt(q,p,q->numElements);
    AddToMap(p,setToAdd);
    return &q->elements[q->numElements-1];
}
void ClearMap(PathfindNode* nodes, int w, int h)
{
    for (int i = 0; i < w*h; i++)
    {
        nodes[i].g = FLT_MAX;
        nodes[i].f = FLT_MAX;
        nodes[i].parent = NULL;
    }
}
float h(PathfindNode* s, PointI end)
{
    return _MAX(abs(end.x - s->p.x), abs(end.y - s->p.y));
    //return dist(s->p.x,s->p.y,end.x,end.y);//sqrt(pow(s->p.x - end.x,2) + pow(s->p.y - end.y,2));
}
int cmpQueue (const void * a, const void * b) {
    PathfindNode* p =  (PathfindNode*)a;
    PathfindNode* p2 = (PathfindNode*)b;
    return (p->f - p2->f);
}

void SortQueue(Queue* q)
{
    qsort(q->elements,q->numElements,sizeof(PathfindNode),cmpQueue);
}

float d(PathfindNode* p1, PathfindNode* p2)
{
    if (PointIsFree(p2->p.x,p2->p.y))
        return 1;
    else return 100;
}
void InitPathfinding()
{
    pathfindmap.sizex = 0;
    pathfindmap.sizey = 0;
}
void ClearQueue(Queue* queue)
{
    queue->numElements = 0;
}
PointI GetClosestPathablePoint(PointI target, PointI current, bool* found, int w, int h)
{
    float closest = FLT_MAX;
    PointI closestP = current;
    *found = false;

    for (int x = -16; x < 16; x++)
    {
        for (int y = -16; y < 16; y++)
        {
            int nx = target.x + x;
            int ny = target.y + y;

            if (RectIsFree(nx,ny,w,h)) 
            {
                float distance = dist(nx,ny,target.x,target.y);// + dist(nx,ny,current.x,current.y);
                if (distance < closest)
                {
                    closest = distance;
                    closestP = (PointI){nx,ny};
                    *found = true;
                }
            }
        }
    }
    return closestP;
}
PointI GetPath(PathfindNode* currentNode)
{
    PathfindNode* p = currentNode;
    while (1)
    {
        if (p->parent)
        {

            if (p->parent->parent == NULL)
            {
                return p->p;
            }
            }
            else
            return p->p;   

        p = p->parent;
    }

}
PointI AStar(PointI here, PointI target, bool* success, float w, float h)
{
    DEBUG_NUMNODESPUSHED = 0;
    bool found;

    w /= _GRAIN; h /= _GRAIN;
    w=1;h=1;
   // target = GetClosestPathablePoint(target,here,&found,w,h);
    //if (!found) return target;
    //here = GetClosestPathablePoint(here,target,&found,w,h);
    //if (!found) return target;

    if (PointIntIsEq(here,target))
        return target;


    *success = true;
    ClearQueue(&openSet);
    ClearQueue(&closedSet);

    PathfindNode start = (PathfindNode){0};
    start.p = here;

    if (pathfindmap.sizex * pathfindmap.sizey != GetMapWidth() * GetMapHeight())
    {
        free(pathfindmap.set);
        pathfindmap.set = calloc(GetMapWidth()*GetMapHeight(),sizeof(NODE_IN_SET));
        pathfindmap.sizex = GetMapWidth();
        pathfindmap.sizey = GetMapHeight();
    }
    else
        memset(pathfindmap.set,0,pathfindmap.sizex*pathfindmap.sizey*sizeof(NODE_IN_SET));


    Push(&openSet,start,true,NODE_INSIDE_OPEN_LIST);


    while (openSet.numElements > 0)
    {
        PathfindNode currentNode = Pop(&openSet);
        PathfindNode* parent = Push(&closedSet,currentNode,false,NODE_INSIDE_CLOSED_LIST);

        if (PointIntIsEq(currentNode.p,target))
        {
            return GetPath(&currentNode);
        }
        if (closedSet.numElements >= PATHFIND_DEPTH)
        {
            //get the best guess
            PathfindNode* lowest = &openSet.elements[0];
            float distance = dist(lowest->p.x,lowest->p.y,target.x,target.y);
            for (int i = 0; i < openSet.numElements; i++)
            {
                PathfindNode* new = &openSet.elements[i];
                float distancenew = dist(new->p.x,new->p.y,target.x,target.y);
                if (distancenew < distance)
                {
                    lowest = new;
                    distance = distancenew;
                }
            }
            return GetPath(lowest);
        }
        for (int x = _MAX(0,currentNode.p.x-1); x < _MIN(currentNode.p.x+2,GetMapWidth()/_GRAIN); x++)
        {
            for (int y = _MAX(0,currentNode.p.y-1); y < _MIN(currentNode.p.y+2,GetMapHeight()); y++)
            {
                if (x == currentNode.p.x && y == currentNode.p.y)
                    continue;
                    
                PathfindNode child;
                child.p.x = x;
                child.p.y = y;
                int childIndex = GetIndex(GetMapHeight()/(float)_GRAIN, child.p.x, child.p.y);

                //if (child.p.x < 0 || child.p.x >= GetMapWidth()/_GRAIN)
                  //  continue;
               // if (child.p.y < 0 || child.p.y >= GetMapHeight()/_GRAIN)
                 //   continue;

                NODE_IN_SET set = GetSet(childIndex);
                //if (IsInQueue(&closedSet,&child) >= 0)
                //    continue;
                if (set == NODE_INSIDE_CLOSED_LIST)
                    continue;

                //bool walkable = (RectIsFree(child.p.x,child.p.y,w,h));
                bool walkable = (PointIsFree(child.p.x,child.p.y));
                
                if (!walkable) 
                    continue;
                float distcurrchild = (x != currentNode.p.x && y != currentNode.p.y) ? 1.41421356237f : 1;
                //distcurrchild = dist(currentNode.p.x,currentNode.p.y,child.p.x,child.p.y);
                printf("dist: %f\n",distcurrchild);
                //child.g = currentNode.g + (walkable ? dist(child.p.x,child.p.y,currentNode.p.x,currentNode.p.y) : 100);
                child.g = currentNode.g + distcurrchild;
                
                child.h = dist(child.p.x,child.p.y,target.x,target.y);
                child.f = child.g + child.h;
                child.parent =  parent;

                //int found = IsInQueue(&openSet,&child);
                //if (found >= 0)
                if (set == NODE_INSIDE_OPEN_LIST)
                {
                    int found = IsInQueue(&openSet,&child);

                    if (child.g > openSet.elements[found].g)
                    {
                        continue;
                        //openSet.elements[found] = child;
                    }
                    else
                    {
                        openSet.elements[found] = child;
                        continue;
                    }
                }
                Push(&openSet,child,true,NODE_INSIDE_OPEN_LIST);
                
            }
        }
    }
    *success = false;
    return target;
}