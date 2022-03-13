#pragma once
#define MAX_PARTICLES 256
#include "stdbool.h"
#include <allegro5/allegro.h>

typedef enum PARTICLE_PROPERTIES
{
    PARTICLE_ENABLED = 1,
    PARTICLE_COLOR = 2,

} PARTICLE_PROPERTIES;

float particle_x[MAX_PARTICLES];
float particle_y[MAX_PARTICLES];
float particle_dir[MAX_PARTICLES];
float particle_lifetime[MAX_PARTICLES];
char particle_properties[MAX_PARTICLES];
char particle_speed[MAX_PARTICLES]; 
int PARTICLES_TOP;

void UpdateParticles(float dt);
void DrawParticles();
void InitParticles();
void AddParticle(float x, float y, short lifetime, float speed, float angle, bool colour);  
void AddMouseRandomParticles(ALLEGRO_MOUSE_STATE mouseState, int numParticles);

