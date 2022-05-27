#include <math.h>

#include "particle.h"

#include "colors.h" 
#include "helperfuncs.h"

void UpdateParticles(float dt)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particle_x[i] += cosf(particle_dir[i]) * particle_speed[i];
        particle_y[i] += sinf(particle_dir[i]) * particle_speed[i];

        particle_lifetime[i] -= dt;
        if (particle_lifetime[i] < 0)
            particle_properties[i] &= ~PARTICLE_ENABLED;
    }
}
void AddMouseRandomParticles(ALLEGRO_MOUSE_STATE mouseState, int numParticles)
{
    for (int i = 0; i < numParticles; i++)
        AddParticle(mouseState.x,mouseState.y,2,2,RandRange(-M_PI,M_PI),true);
}
void AddParticle(float x, float y, short lifetime, float speed, float angle, bool colour)
{
    particle_x[PARTICLES_TOP] = x;
    particle_y[PARTICLES_TOP] = y;
    particle_lifetime[PARTICLES_TOP] = lifetime - RandRange(0,lifetime/2.0f);
    if (colour)
        particle_properties[PARTICLES_TOP] |= PARTICLE_COLOR; 
    particle_properties[PARTICLES_TOP] |= PARTICLE_ENABLED;
    particle_dir[PARTICLES_TOP] = angle;
    particle_speed[PARTICLES_TOP] = speed;
    PARTICLES_TOP++;
    if (PARTICLES_TOP >= MAX_PARTICLES)
    {
        PARTICLES_TOP = 0;
    }
}
void InitParticles()
{
    memset(particle_x,0,sizeof(particle_x[0])*MAX_PARTICLES);
    memset(particle_y,0,sizeof(particle_y[0])*MAX_PARTICLES);
    memset(particle_dir,0,sizeof(particle_dir[0])*MAX_PARTICLES);
    memset(particle_lifetime,0,sizeof(particle_lifetime[0])*MAX_PARTICLES);
    //memset(particle_color,0,sizeof(particle_color[0])*MAX_PARTICLES);
    memset(particle_properties,0,sizeof(particle_properties[0])*MAX_PARTICLES);

    PARTICLES_TOP = 0;
}
void DrawParticles()
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particle_properties[i] & PARTICLE_ENABLED)
        {
            ALLEGRO_COLOR col = (particle_properties[i] & PARTICLE_COLOR) ? FRIENDLY : ENEMY;
            al_draw_pixel(particle_x[i],particle_y[i],col);
        }
    }
}