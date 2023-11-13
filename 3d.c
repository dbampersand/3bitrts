#include "3d.h"
#include "video.h"
#include "sprite.h"
#include <math.h>
#include "helperfuncs.h"
#include <limits.h>
#include <float.h>
#include "allegro5/allegro_primitives.h"
#include <stdio.h>

typedef struct VOXEL_ARGS
{
    int threadIndex;
    int x; int y; int w; int h;
    float fov;
    float scale;
    float mat[3][3];

} VOXEL_ARGS;
typedef struct CHUNK_GEN_THREAD_DATA
{
    int threadIndex; int x; int y; int z; int w; int h; int d;
}CHUNK_GEN_THREAD_DATA;
CHUNK_GEN_THREAD_DATA chunkThreadData[NUM_THREADS];





Point3 rotation = {0};

Point3 playerVelocity = {0};
Cube playerPosition = {VOXEL_WORLD_SIZE/2,VOXEL_WORLD_SIZE/2,VOXEL_WORLD_SIZE/2,1,1,3};

float camMatrix[3][3];

ALLEGRO_MOUSE_STATE mousePrev;
ALLEGRO_MOUSE_STATE mouseThis;

ALLEGRO_COLOR _T_PIXELS[_SCREEN_SIZE][_SCREEN_SIZE];

volatile bool threadsFinished[NUM_THREADS];
VOXEL_ARGS threadData[NUM_THREADS];

Voxel world[VOXEL_WORLD_SIZE][VOXEL_WORLD_SIZE][VOXEL_WORLD_SIZE];
Chunk chunks[(int)NUM_CHUNKS][(int)NUM_CHUNKS][(int)NUM_CHUNKS];


//Point3 rightV = (Point3){0, 1, 0};
//Point3 frontV = (Point3){1, 0, 0};
#define upV (Point3){0, 0, 1}
#define rightV (Point3){0, 1, 0}

void PreprocessWorld();
bool RangeCheck(int x, int y, int z);

void normalizeQ(Quaternion* q)
{
    float abs = sqrtf(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
    q->x /= abs;
    q->y /= abs;
    q->x /= abs;
    q->w /= abs;
}
//https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
Quaternion MultQuat(Quaternion q1, Quaternion q2)
{
    Quaternion new;
    new.x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    new.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    new.z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    new.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    return new;
}
Point3 ToEulerAngles(Quaternion q) {
    Point3 angle;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angle.y = atan2f(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = sqrtf(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = sqrtf(1 - 2 * (q.w * q.y - q.x * q.z));
    angle.x = 2 * atan2f(sinp, cosp) - M_PI / 2.0f;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angle.z = atan2f(siny_cosp, cosy_cosp);

    return angle;
}
Quaternion QuatFromAngle(float angle, Point3 axis)
{
    Quaternion new; 

    float sinAng = sinf(angle/2.0f);
    float cosAng = cosf(angle/2.0f);

    new.x = (axis.x * sinAng);
    new.y = (axis.y * sinAng);
    new.z = (axis.z * sinAng);
    new.w = cosAng;
    normalizeQ(&new);
    return new;
}
Quaternion IdentityQ()
{
    return (Quaternion){0,0,0,1};
}
void AddVoxel(int x, int y, int z, Color c)
{
    if (!RangeCheck(x,y,z))
        return;
    int chunkX = (int)(floor(x/CHUNK_SIZE));
    int chunkY = (int)(floor(y/CHUNK_SIZE));
    int chunkZ = (int)(floor(z/CHUNK_SIZE));

    chunks[chunkX][chunkY][chunkZ].hasVoxels = true;
    world[x][y][z].c = c;
}
void AddVoxelLine(int x, int y, int z, int w, int h, int d, Color c)
{
    for (int x2 = x; x2 < x + w; x2++)
    {
        for (int y2 = y; y2 < y + h; y2++)
        {
            for (int z2 = z; z2 < z + d; z2++)
            {
                AddVoxel(x2,y2,z2,c);
            }
        }   
    }
}

void ClearWorld()
{
    memset(world,0,VOXEL_WORLD_SIZE*VOXEL_WORLD_SIZE*VOXEL_WORLD_SIZE*sizeof(Voxel));
    memset(chunks,0,NUM_CHUNKS*NUM_CHUNKS*NUM_CHUNKS*sizeof(chunks[0][0][0]));

}
void LoadWorld(char* path, int w, int h, int d)
{
    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    ALLEGRO_BITMAP* sprite = al_load_bitmap(path);
    if (!sprite)
        return;
    al_set_target_bitmap(sprite);
    al_lock_bitmap(sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    ClearWorld();
    for (int i = 0; i < d; i++)
    {
        int xStart = 0;
        int yStart = i * h;

        for (int x = xStart; x < xStart + w; x++)
        {
            for (int y = yStart; y < yStart + h; y++)
            {
                ALLEGRO_COLOR pixel = al_get_pixel(sprite,x,y);
                unsigned char r; 
                unsigned char g; 
                unsigned char b; 
                unsigned char a; 

                al_unmap_rgba(pixel,&r,&g,&b,&a);
                if (a >= 1)
                {   
                    if (r == 255 && g == 255 && b == 255)
                    {
                        AddVoxel(x-xStart,y-yStart,d-i,COLOR_FRIENDLY);
                    }
                    if (r == 0 && g == 0 && b == 0)
                    {
                        AddVoxel(x-xStart,y-yStart,d-i,COLOR_DAMAGE);
                    }

                    //world[x-xStart][y-yStart][d-i].c = rand() % (COLOR_ALL-1);    


                }
            }
        }
    }
    al_destroy_bitmap(sprite);
    al_set_target_bitmap(before);
    PreprocessWorld();

    printf("finished\n");
}

bool RenderThreadsFinished()
{
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (threadsFinished[i] == false)
        {   
            return false;
        }
    }
    return true;
}


static inline bool IsAir(Voxel v)
{   
    return v.c == 0;
}

bool RangeCheck(int x, int y, int z)
{
    return (x >= 0 && x < VOXEL_WORLD_SIZE && y >= 0 && y < VOXEL_WORLD_SIZE && z >= 0 && z < VOXEL_WORLD_SIZE);
}

bool CheckCube(Cube c, int stepSize, int* hitX, int* hitY, int* hitZ)
{
    for (int x = c.x - c.w/2.0f; x < c.x + c.w/2.0f; x++)
    {   
        for (int y = c.y-c.h/2.0f; y < c.y + c.h/2.0f; y++)
        {
            for (int z = c.z; z >= c.z - ((c.d)); z--)
            {

                if (!RangeCheck(x,y,z))
                {
                    if (hitX)
                        *hitX = x;
                    if (hitY)
                        *hitY = y;
                    if (hitZ)
                        *hitZ = z;

                    return false;
                }   
                if (!IsAir(world[x][y][z]))
                {
                    if (hitX)
                        *hitX = x;
                    if (hitY)
                        *hitY = y;
                    if (hitZ)
                        *hitZ = z;
                    return false;
                }
            }
        }
    }
    return true;
}   
float dist3(float x, float y, float z)
{
    return sqrtf(x*x+y*y+z*z);
}
float GenerateDistToNearest(float x, float y, float z)
{
    float lowestDist = DISTANCE_FIELD_MAX;
    if (!chunks[(int)(x/CHUNK_SIZE)][(int)(y/CHUNK_SIZE)][(int)(z/CHUNK_SIZE)].hasVoxels)
        return DISTANCE_FIELD_MAX/sqrtf(DISTANCE_FIELD_MAX);
    for (int x2 = x - DISTANCE_FIELD_MAX/2; x2 < x+DISTANCE_FIELD_MAX/2; x2++)
    {
        for (int y2 = y - DISTANCE_FIELD_MAX/2; y2 < y + DISTANCE_FIELD_MAX/2; y2++)
        {
            for (int z2 = z - DISTANCE_FIELD_MAX/2; z2 < z + DISTANCE_FIELD_MAX/2; z2++)
            {
                if (!RangeCheck(x2,y2,z2))
                {   
                    lowestDist = 1;
                    continue;
                }
                if (!IsAir(world[x2][y2][z2]))
                {
                    if (x == x2 && y == y2 && z == z2)
                        continue;
                    //float dist = _MAX(_MAX(fabsf(x-x2),fabsf(y-y2)),fabsf(z-z2));
                    //float dist = fabsf(x-x2) + fabsf(y-y2) + fabsf(z-z2);
                    float dist = dist3((x-x2),(y-y2),(z-z2));

                    if (dist < lowestDist)
                        lowestDist = dist;
                }
            }
        }
    }
    if (lowestDist < 1) 
        lowestDist = 1;
    lowestDist /= sqrtf(DISTANCE_FIELD_MAX);
    return (lowestDist);
}

static void* GenerateChunkDistanceCache(ALLEGRO_THREAD* t, void* args)
{
    CHUNK_GEN_THREAD_DATA* da = (CHUNK_GEN_THREAD_DATA*)args;
    int x = da->x;
    int y = da->y;
    int z = da->z;
    int w = da->w;
    int h = da->h;
    int d = da->d;

    for (int x2 = 0; x2 < x + w; x2++)
    {
        for (int y2 = 0; y2 < x + h; y2++)
        {
            for (int z2 = 0; z2 < z + d; z2++)
            {
                world[x2][y2][z2].distFunc = GenerateDistToNearest(x2,y2,z2);
            }
        }
    }
    threadsFinished[da->threadIndex] = true;
    return NULL;
}
void DoPlayerCollisions(Cube* player, float x, float y, float z, bool* onGround)
{
    int hitX; 
    int hitY; 
    int hitZ;

    double i;


    float numSteps; 


    numSteps = fabsf(y) > 1 ? ceil(fabsf(y)) : 1;
    if (fabsf(y) > 0)
    {
        for (float i = 0; i < numSteps; i++)
        {   
            player->y += y / numSteps; 

            if (!CheckCube(*player,STEP_SIZE,&hitX,&hitY,&hitZ))
            {
                int maxSteps = 50;
                while (!CheckCube(*player,STEP_SIZE,&hitX,&hitY,&hitZ) && maxSteps > 0)
                {
                    maxSteps--;

                    if (hitZ <= player->z - player->d + STEP_SIZE)
                    {
                        z = 0;
                        player->z++;
                    }
                    player->y -=  y / numSteps;
                }
            }
        }
    }
    numSteps = fabsf(x) > 1 ? ceil(fabsf(x)) : 1;
    if (fabsf(x) > 0)
    {
        for (float i = 0; i < numSteps; i++)
        {   
            player->x +=  x / numSteps; 
            if (!CheckCube(*player,STEP_SIZE,&hitX,&hitY,&hitZ))
            {
                int maxSteps = 50;
                while (!CheckCube(*player,STEP_SIZE,&hitX,&hitY,&hitZ) && maxSteps > 0)
                {
                    maxSteps--;
                    if (hitZ <= player->z - player->d + STEP_SIZE)
                    {   
                        z = 0;
                        player->z++;
                    }
                    player->x -= x / numSteps;
                }
            }
        }
    }

    numSteps = fabsf(z) > 1 ? ceil(fabsf(z)) : 1;
    *onGround = false;
    if (fabsf(z) > 0)
    {
        for (float i = 0; i < numSteps; i++)
        {   
            player->z += z / numSteps; 
            if (!CheckCube(*player,STEP_SIZE,&hitX,&hitY,&hitZ))
            {
                *onGround = true;
                int maxSteps = 50;
                while (!CheckCube(*player,STEP_SIZE,&hitX,&hitY,&hitZ) && maxSteps > 0)
                {
                    maxSteps--;
                    player->z -= z / numSteps;


                }
            }
        }
    }
    if (player->z < 0)
    {
        player->z = player->h;
    }
    if (player->z + player->h > VOXEL_WORLD_SIZE)
    {
        player->z = VOXEL_WORLD_SIZE - player->h;
    }
}

void MovePlayer(float x, float y, float z)
{
    bool onGround = false;
    DoPlayerCollisions(&playerPosition,x,y,-z, &onGround);
    if (onGround)
    {
        playerVelocity.z = 0;   
    }
}
void PreprocessWorld()
{
        ALLEGRO_THREAD* threads[NUM_THREADS];   

    int cubeSize = VOXEL_WORLD_SIZE/NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadsFinished[i] = false;
        int x = 0; 
        int y = 0;
        int z = VOXEL_WORLD_SIZE/(i+1);

        int w = VOXEL_WORLD_SIZE;
        int h = VOXEL_WORLD_SIZE;
        int d = VOXEL_WORLD_SIZE / NUM_THREADS;

        CHUNK_GEN_THREAD_DATA dat = (CHUNK_GEN_THREAD_DATA){i,x,y,z,w,h,d};
        chunkThreadData[i] = dat;

        threads[i] = al_create_thread(GenerateChunkDistanceCache,&chunkThreadData[i]);
        al_start_thread(threads[i]);
        //GenerateChunkDistanceCache(x,y,z,w,h,d);

    }
    while (!RenderThreadsFinished())
    {
        
    }
    for (int i = 0; i < NUM_THREADS; i++)
        al_destroy_thread(threads[i]);

}
void GenTestWorld()
{
    playerPosition.x = VOXEL_WORLD_SIZE/2;
    playerPosition.y = VOXEL_WORLD_SIZE/2;
    playerPosition.z = 30;

    ClearWorld();
    for (int x = 0; x < VOXEL_WORLD_SIZE; x++)
    {
        for (int y = 0; y < VOXEL_WORLD_SIZE; y++)
        {
            for (int z = 0; z < VOXEL_WORLD_SIZE; z++)
            {
                    Voxel* v = &world[x][y][z];
					if (z == 0 || z == VOXEL_WORLD_SIZE-1)
                    {
						v->c = COLOR_ENEMY;
                        AddVoxel(x,y,z,v->c); 
                    }   
                    if (y == 0 || y == VOXEL_WORLD_SIZE-1)
                    {
						v->c = COLOR_POISON;
                        AddVoxel(x,y,z,v->c); 
                    }   
                    if (x == 0 || x == VOXEL_WORLD_SIZE-1)
                    {
						v->c = COLOR_SHIELD;
                        if (rand()%32 == 0)
                        {
                            v->c = rand() % (COLOR_ALL-1);    
                            AddVoxel(x,y,z,v->c); 
                        }
                    }       

                    if (x >= 30 && y >= 30 && x <= 60 && y <= 60)
                        AddVoxel(x,y,z,COLOR_DAMAGE);
                    if (x >= 200 && y >= 200 && x <= 230 && y <= 230 && z > 0 && z < 30)
                        AddVoxel(x,y,z,COLOR_SPEED);
                    

                    if (rand()%20000 == 0)
                    {
                        //v->c = rand() % (COLOR_ALL-1);    
                        //AddVoxel(x,y,z,v->c); 
                    }


            }
        }
    }
    for (int i = 0; i < 30/STEP_SIZE; i++)
    {
        AddVoxelLine(170+i,200,i*STEP_SIZE,30,30,STEP_SIZE,COLOR_BG);
    }
    PreprocessWorld();
}
void Init3d()
{
    //GenTestWorld(); 
    LoadWorld("assets/room.png",255,255,255);

    playerPosition.x = 128;
    playerPosition.y = 128;
    playerPosition.z = 5;

}
static inline int GetVoxelOffset(int x, int y, int z, int width, int height)
{
	return x + width * (y + height * z);
}



Point3 MultMatrix(float matrix[3][3], Point3 p)
{
	Point3 result;
	result.x = p.x * matrix[0][0] + p.y * matrix[1][0] +p.z * matrix[2][0];
	result.y = p.x * matrix[0][1] + p.y * matrix[1][1] + p.z * matrix[2][1];
	result.z = p.x * matrix[0][2] + p.y * matrix[1][2] + p.z * matrix[2][2];
	return result;
}

Point3 NormalizeP3(Point3 p)
{
	float length = sqrtf((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
	if (length == 0)
	{
		p.x = 0;
		p.y = 0;
		p.z = 0;
	}
	else
	{
		p.x = p.x / length;
		p.y = p.y / length;
		p.z = p.z / length;
	}
	return p; 
}

void GetCamMatrix(Quaternion q, float m[3][3])
{
	float n = 1.0f/sqrt(q.x*q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	q.x = q.x * n;
	q.y = q.y * n;
	q.z = q.z * n;
	q.w = q.w * n;

	float s = pow(n,-2.0f);

	float i = q.x;
	float j = q.y;
	float k = q.z;
	float r = q.w;

	int z = 0;
    
	m[0][0] = 1-2*pow(q.y,2) - 2*pow(q.z,2);
	m[0][1] = 2*q.x*q.y - 2*q.w*q.z;
	m[0][2] = 2*q.x*q.z + 2*q.w*q.y;

	m[1][0] = 2*q.x*q.y + 2*q.w * q.z;
	m[1][1] = 1 - 2*pow(q.x,2) - 2*pow(q.z,2);
	m[1][2] = 2*q.y*q.z - 2*q.w*q.x;
	
	m[2][0] = 2*q.x*q.z - 2*q.w*q.y;
	m[2][1] = 2*q.y*q.z + 2 * q.w * q.x;
	m[2][2] =  1 - 2*pow(q.x,2) - 2*pow(q.y,2);

}
Quaternion EulerToQuart(Point3 angle)
{
    Quaternion q;
    float x0 = cosf(rotation.x/2.0f);
    float x1 = sinf(rotation.x/2.0f);

    float y0 = cosf(rotation.y/2.0f);
    float y1 = sinf(rotation.y/2.0f);
    
	float z0 = cosf(rotation.z/2.0f);
    float z1 = sinf(rotation.z/2.0f);

    q.x = x1 * y0 * z0 - x0 * y1 * z1;
    q.y = x0 * y1 * z0 + x1 * y0 * z1;
    q.z = x0 * y0 * z1 - x1 * y1 * z0;
    q.w = x0 * y0 * z0 + x1 * y1 * z1;

    return q;
}

Voxel VoxelCastRay(float startX, float startY, float startZ, Point3 dir, int* endX, int* endY, int* endZ)
{
    if (!RangeCheck(startX,startY,startZ))
		return (Voxel){0};


    float x = startX;
    float y = startY;
    float z = startZ;

    


    bool found = false;
    while (1)
    {
        Voxel v = world[(int)(x)][(int)(y)][(int)(z)];

        if (!IsAir(v))
        {
            *endX = x;
            *endY = y;
            *endZ = z;

            return v;
        }
        float deltaX = dir.x * v.distFunc;
        float deltaY = dir.y * v.distFunc;
        float deltaZ = dir.z * v.distFunc;

        float xNew = x + deltaX;
        float yNew = y + deltaY;
        float zNew = z + deltaZ;
        //if (!RangeCheck(xNew,yNew,zNew))
          //  goto range;

        x = xNew;
        y = yNew;
        z = zNew;

        /*y = yNew;
        v = world[(int)(x)][(int)(y)][(int)(z)];
        if (!IsAir(v))
            return v;

        x = xNew;
        v = world[(int)(x)][(int)(y)][(int)(z)];
        if (!IsAir(v))
            return v;

            
        z = zNew;
        v = world[(int)(x)][(int)(y)][(int)(z)];
        if (!IsAir(v))
            return v;*/

        if (!RangeCheck(x,y,z)) 
        {
            //range:
                x = clamp(x,0,VOXEL_WORLD_SIZE-1);
                y = clamp(y,0,VOXEL_WORLD_SIZE-1);
                z = clamp(z,0,VOXEL_WORLD_SIZE-1);
                *endX = x;
                *endY = y;
                *endZ = z;

                return world[(int)x][(int)y][(int)z];
        }

    }
    return (Voxel){0};
}
Point3 VecCross(Point3 P, Point3 Q)
{
	return (Point3){P.y*Q.z - P.z*Q.y, P.z * Q.x - P.x * Q.z, P.x*Q.y - P.y * Q.x};
}

Point3 GetForwardVector(Point3 v)
{
    return (Point3){
        cos(v.z)*cos(v.x),
        -sin(v.z),
        cos(v.z)*sin(v.x),

            };

}
Point3 GetLeftVector(Point3 v)
{
    Point3 forward = GetForwardVector(v);
	return VecCross(upV,forward);
}
void UpdatePlayer3D(float dt, ALLEGRO_KEYBOARD_STATE* keyState)
{
    playerVelocity.z += _GRAVITY * dt;
    playerVelocity.z = clamp(playerVelocity.z,-_GRAVITY,_GRAVITY);

    if (KeyDown(keyState,ALLEGRO_KEY_Z))
    {
        playerPosition.x = RandRange(0,VOXEL_WORLD_SIZE);
        playerPosition.y = RandRange(0,VOXEL_WORLD_SIZE);
        playerPosition.z = RandRange(0,VOXEL_WORLD_SIZE);

    }

    Quaternion up = QuatFromAngle(rotation.z,upV);
    Quaternion pitch = QuatFromAngle(rotation.x,rightV);
    Quaternion rotQ = MultQuat(pitch,up);

    Point3 rot = ToEulerAngles(rotQ);

    bool moveKeyDown = false;
    playerVelocity.x = 0;
    playerVelocity.y = 0;

    if (KeyDown(keyState,ALLEGRO_KEY_W))
    {
        Point3 fwd = GetForwardVector(rot);

        playerVelocity.x += fwd.x * PLAYER_MAX_SPEED;// * PLAYER_VELOCITY_ADD * dt;
        playerVelocity.y += fwd.y * PLAYER_MAX_SPEED;// * PLAYER_VELOCITY_ADD * dt;

        //playerPosition.x += fwd.x;
        //playerPosition.y += fwd.y;

        moveKeyDown = true;
    }
     if (KeyDown(keyState,ALLEGRO_KEY_S))
    {
        Point3 fwd = GetForwardVector(rot);

        playerVelocity.x -= fwd.x * PLAYER_MAX_SPEED; // * PLAYER_VELOCITY_ADD * dt;
        playerVelocity.y -= fwd.y * PLAYER_MAX_SPEED;// * PLAYER_VELOCITY_ADD * dt;

        //        playerPosition.x -= fwd.x;
        //playerPosition.y -= fwd.y;

        moveKeyDown = true;

    }
    if (KeyDown(keyState,ALLEGRO_KEY_A))
    {
        Point3 l = GetLeftVector(rot);

        playerVelocity.x -= l.x * PLAYER_MAX_SPEED;// * PLAYER_VELOCITY_ADD * dt;
        playerVelocity.y -= l.y * PLAYER_MAX_SPEED;// * PLAYER_VELOCITY_ADD * dt;

       //         playerPosition.x -= l.x;
        //playerPosition.y -= l.y;

        moveKeyDown = true;

    }


    if (KeyDown(keyState,ALLEGRO_KEY_D))
    {
        Point3 l = GetLeftVector(rot);

        playerVelocity.x += l.x * PLAYER_MAX_SPEED;// * PLAYER_VELOCITY_ADD * dt;
        playerVelocity.y += l.y * PLAYER_MAX_SPEED; // PLAYER_VELOCITY_ADD * dt;

        //playerPosition.x += l.x;
        //playerPosition.y += l.y;

        moveKeyDown = true;

    }
    if (!moveKeyDown)
    {    
       // playerVelocity.x = Towards(playerVelocity.x,0,_FRICTION*dt);
        //playerVelocity.y = Towards(playerVelocity.y,0,_FRICTION*dt);
    }
    Point3 fwd = GetForwardVector(rot);

   // playerVelocity.x = clamp(playerVelocity.x,-PLAYER_MAX_SPEED,PLAYER_MAX_SPEED);
   // playerVelocity.y = clamp(playerVelocity.y,-PLAYER_MAX_SPEED,PLAYER_MAX_SPEED);
    
    MovePlayer(playerVelocity.x,playerVelocity.y,playerVelocity.z);
}
float _MOUSE_POSITION_CENTER_X;
float _MOUSE_POSITION_CENTER_Y;

void Update3D(float dt, MouseState mouseStateLastFrame, MouseState mouseState, ALLEGRO_KEYBOARD_STATE* keyState)
{
    //al_get_mouse_state(&mouseThis);

    //not sure why it needs to be multiplied by this
    //perhaps hidpi stuff? - test on other monitor
    int wScreen = al_get_display_width(display)/2.0f;
    int hScreen = al_get_display_height(display)/2.0f;


    float mouseMoveX = (mouseState.screenX - (_MOUSE_POSITION_CENTER_X)) * _TURN_MAG;
    float mouseMoveY = (mouseState.screenY - (_MOUSE_POSITION_CENTER_Y)) * _TURN_MAG;
    //mouseMoveX = 0;
    //mouseMoveY = 0;
    
    //x is pitch
    //z is yaw
    //y is roll

    if (KeyDown(keyState,ALLEGRO_KEY_E))
    {
    rotation.z -= mouseMoveX * dt;
    rotation.x -= mouseMoveY * dt;

        al_set_mouse_xy(display,128*_RENDERSIZE,128*_RENDERSIZE);
    }
    MouseState after = GetMouseClamped();
    _MOUSE_POSITION_CENTER_X = after.screenX;
    _MOUSE_POSITION_CENTER_Y = after.screenY;

    //rotation.y += mouseMoveY * dt;

    mousePrev = mouseThis;

    UpdatePlayer3D(dt,keyState);
}
static void* VoxelRenderLines(ALLEGRO_THREAD *thr, void* v)
{
    VOXEL_ARGS* va = (VOXEL_ARGS*)v;

    float mat[3][3];
    mat[0][0] = va->mat[0][0];
    mat[0][1] = va->mat[0][1];
    mat[0][2] = va->mat[0][2];
    mat[1][0] = va->mat[1][0];
    mat[1][1] = va->mat[1][1];
    mat[1][2] = va->mat[1][2];
    mat[2][0] = va->mat[2][0];
    mat[2][1] = va->mat[2][1];
    mat[2][2] = va->mat[2][2];

    float fov = va->fov;
    float scale = va->scale;

    for (int x = va->x; x < va->x + va->w; x += PIXEL_SIZE)
    {
        for (int y = va->y; y < va->y + va->h; y += PIXEL_SIZE)
        {   
            float pX = (2.0f * (x) / (float)_SCREEN_SIZE - 1.0f) * scale;
			float pY = (1.0f  - 2.0f * (y) / (float)_SCREEN_SIZE) * scale; 
			Point3 angle = (Point3){1,pX,pY};
			angle = MultMatrix(mat, angle);
			NormalizeP3(angle);
            int endX; int endY; int endZ;
            Voxel v = VoxelCastRay(playerPosition.x,playerPosition.y,playerPosition.z, angle,&endX,&endY,&endZ);
            float dist = dist3(playerPosition.x-endX,playerPosition.y-endY,playerPosition.z-endZ);
            
            dist /= VOXEL_WORLD_SIZE;

            ALLEGRO_COLOR c = GetColor(v.c,0);

            int r; int g; int b;
            r = clamp((c.r - dist)*255,0,255);
            g = clamp((c.g - dist)*255,0,255);
            b = clamp((c.b - dist)*255,0,255);

            c = al_map_rgb(r,g,b);

            if (v.c == 0)   
                c = BG;    


            _T_PIXELS[x][y] = c;
            //DrawFilledRectangle(x,y,x+PIXEL_SIZE,y+PIXEL_SIZE,c);

        }
    }
    threadsFinished[va->threadIndex] = true;
    return NULL;
}
void VoxelRender()
{   
    al_lock_bitmap(al_get_target_bitmap(),ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);
    float mat[3][3]; 

    Quaternion up = QuatFromAngle(rotation.z,upV);
    Quaternion pitch = QuatFromAngle(rotation.x,rightV);
    Quaternion rot = MultQuat(pitch,up);

    GetCamMatrix(rot,mat);


    float fov = 90;
	float scale = tan((fov/2)*M_PI/180.0f); 

    int pxW = 1;
    int pxH = 1;

    ALLEGRO_THREAD* threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        threadsFinished[i] = false;

        int x = 0;
        int y = i * (_SCREEN_SIZE / NUM_THREADS);
        int w = _SCREEN_SIZE;
        int h = _SCREEN_SIZE / NUM_THREADS;
        VOXEL_ARGS va = (VOXEL_ARGS){i,x,y,w,h,fov,scale};
        va.mat[0][0] = mat[0][0];
        va.mat[0][1] = mat[0][1];
        va.mat[0][2] = mat[0][2];

        va.mat[1][0] = mat[1][0];
        va.mat[1][1] = mat[1][1];
        va.mat[1][2] = mat[1][2];

        va.mat[2][0] = mat[2][0];
        va.mat[2][1] = mat[2][1];
        va.mat[2][2] = mat[2][2];

        threadData[i] = va;


        threads[i] = al_create_thread(VoxelRenderLines,&threadData[i]);
        al_start_thread(threads[i]);

    }
    while (!RenderThreadsFinished())
    {
        
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        al_destroy_thread(threads[i]);
    }
    /*
    for (int x = 0; x < _SCREEN_SIZE; x += PIXEL_SIZE)
    {
        for (int y = 0; y < _SCREEN_SIZE; y += PIXEL_SIZE)
        {   
            float pX = (2.0f * (x + 0.5f) / (float)_SCREEN_SIZE - 1.0f) * scale;
			float pY = (1.0f  - 2.0f * (y + 0.5f) / (float)_SCREEN_SIZE) * scale; 

			Point3 angle = (Point3){1,pX,pY};
			angle = MultMatrix(mat, angle);
			NormalizeP3(angle);
            Voxel v = VoxelCastRay(playerPosition.x,playerPosition.y,playerPosition.z, angle);
            
            ALLEGRO_COLOR c = GetColor(v.c,0);
            if (v.c == 0)   
                c = BG;

            DrawFilledRectangle(x,y,x+PIXEL_SIZE,y+PIXEL_SIZE,c);

            //al_draw_pixel(0,0,al_map_rgb(rand()%255,rand()%255,rand()%255));
        }
    }   */
    for (int x = 0; x < _SCREEN_SIZE; x += PIXEL_SIZE)
    {
        for (int y = 0; y < _SCREEN_SIZE; y += PIXEL_SIZE)
        {   
            al_put_pixel(x,y,_T_PIXELS[x][y]);
        }
    }

    al_unlock_bitmap(al_get_target_bitmap());

}