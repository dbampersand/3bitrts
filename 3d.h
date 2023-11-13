#pragma once
#include "colors.h"
#include "video.h"
#include "sprite.h"

#define PIXEL_SIZE 1
#define STEP_SIZE 2

#define DISTANCE_FIELD_MAX 4

#define NUM_THREADS 8
#ifdef _HAS_3D
    #define VOXEL_WORLD_SIZE 256
#else
    #define VOXEL_WORLD_SIZE 1
#endif
#ifdef _HAS_3D
    #define CHUNK_SIZE (VOXEL_WORLD_SIZE / 32)
#else
    #define CHUNK_SIZE 1.0f
#endif
#ifdef _HAS_3D
    #define NUM_CHUNKS (VOXEL_WORLD_SIZE / CHUNK_SIZE)
#else
    #define NUM_CHUNKS 1.0f
#endif

#define PLAYER_VELOCITY_ADD 10
#define PLAYER_MAX_SPEED 2
#define _GRAVITY 9.8f
#define _FRICTION 5

#define _TURN_MAG 0.8f

typedef struct Cube
{
    float x; float y; float z;
    float w; float h; float d;
}Cube;

typedef struct Voxel
{
    Color c;
    float distFunc;

} Voxel;

typedef struct Point3
{
    float x; float y; float z;
} Point3;

typedef struct Quaternion
{
    float x;
    float y; 
    float z;
    float w;
}Quaternion;
extern Point3 worldPos;
extern Point3 rotation;

typedef struct Chunk
{
    bool hasVoxels;
    float distToNearest;
}Chunk;

extern Voxel world[VOXEL_WORLD_SIZE][VOXEL_WORLD_SIZE][VOXEL_WORLD_SIZE];
extern Chunk chunks[(int)NUM_CHUNKS][(int)NUM_CHUNKS][(int)NUM_CHUNKS];

void Init3d();
void VoxelRender();
static inline int GetVoxelOffset(int x, int y, int z, int width, int height);
void Update3D(float dt, MouseState mouseStateLastFrame, MouseState mouseState, ALLEGRO_KEYBOARD_STATE* keyState);
Point3 NormalizeP3(Point3 p);
Point3 MultMatrix(float matrix[3][3], Point3 p);
void GetCamMatrix(Quaternion q, float m[3][3]);
Quaternion EulerToQuart(Point3 angle);
