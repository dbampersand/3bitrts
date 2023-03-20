#pragma once
#define MAX_PARTICLES 256

#include "stdbool.h"
#include "allegro5/allegro.h"
#include "colors.h"
#include "video.h" 
#include "pointspace.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef enum PARTICLE_PROPERTIES
{
    PARTICLE_ENABLED = 1,
} PARTICLE_PROPERTIES;

extern float particle_x[MAX_PARTICLES];
extern float particle_y[MAX_PARTICLES];
extern int particle_dir[MAX_PARTICLES];
extern float particle_lifetime[MAX_PARTICLES];
extern float particle_lifetime_total[MAX_PARTICLES];
extern char particle_properties[MAX_PARTICLES];
extern float particle_speed[MAX_PARTICLES]; 
extern Color particle_colors[MAX_PARTICLES];
extern int PARTICLES_TOP;


void UpdateParticles(float dt);
void DrawParticles();
void InitParticles();
void AddParticle(float x, float y, short lifetime, float speed, float angle, Color colour); 
void AddMouseRandomParticles(MouseState mouseState, int numParticles);
void RandParticleAroundEdgeOfCircle(float cx, float cy, float r, short lifetime, float maxspeed, Color col);
void AddParticleWithRandomProperties(float x, float y, Color color, float lifetimeMin, float lifetimeMax, float speedMin, float speedMax, float minAngle, float maxAngle);
bool ParticleIsActive(int index);
float GetParticleAlpha(int index);
