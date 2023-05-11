#include <math.h>

#include "particle.h"

#include "colors.h" 
#include "helperfuncs.h"
#include "player.h"
#include "gameobject.h"

 float particle_x[MAX_PARTICLES] = {0};
 float particle_y[MAX_PARTICLES] = {0};
 int particle_dir[MAX_PARTICLES] = {0};
 float particle_lifetime[MAX_PARTICLES] = {0};
 float particle_lifetime_total[MAX_PARTICLES] = {0};
 char particle_properties[MAX_PARTICLES] = {0};
 float particle_speed[MAX_PARTICLES] = {0}; 
 Color particle_colors[MAX_PARTICLES] = {0};
 int PARTICLES_TOP = 0;
void ClearParticles()
{
    memset(particle_properties,0,sizeof(particle_properties[0])*MAX_PARTICLES);
}

void UpdateParticles(float dt)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (ParticleIsActive(i))
        {
            particle_x[i] += cosTable[particle_dir[i]%360] * particle_speed[i];
            particle_y[i] += sinTable[particle_dir[i]%360] * particle_speed[i];

            particle_lifetime[i] -= dt;
            if (particle_lifetime[i] < 0)
                particle_properties[i] &= ~PARTICLE_ENABLED;
        }
    }
}
void AddMouseRandomParticles(MouseState mouseState, int numParticles)
{
    for (int i = 0; i < numParticles; i++)
        AddParticle(mouseState.worldX,mouseState.worldY,2,2,RandRange(-M_PI,M_PI),true);
}
void AddParticleWithRandomProperties(float x, float y, Color color, float lifetimeMin, float lifetimeMax, float speedMin, float speedMax, float minAngle, float maxAngle)
{
    AddParticle(x,y,RandRange(lifetimeMin,lifetimeMax),RandRange(speedMin,speedMax),RandRange(minAngle,maxAngle),color);
}

void AddParticle(float x, float y, float lifetime, float speed, float angle, Color colour)
{
    particle_x[PARTICLES_TOP] = x;
    particle_y[PARTICLES_TOP] = y;
    particle_lifetime[PARTICLES_TOP] = lifetime - RandRange(0,lifetime/2.0f);
    particle_lifetime_total[PARTICLES_TOP] = particle_lifetime[PARTICLES_TOP];
    particle_colors[PARTICLES_TOP] = colour;
    particle_properties[PARTICLES_TOP] |= PARTICLE_ENABLED;
    particle_dir[PARTICLES_TOP] = RadToDeg(angle);
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
inline float GetParticleAlpha(int index)
{
    return particle_lifetime[index] / (float)particle_lifetime_total[index];
}
void DrawParticles()
{
    int beforeOp; int beforeSrc; int beforeDst;
    al_get_blender(&beforeOp, &beforeSrc, &beforeDst);

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);


    al_lock_bitmap(al_get_target_bitmap(),ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (ParticleIsActive(i))
        {
            ALLEGRO_COLOR col = GetColor(particle_colors[i],0);
            //col.a = particle_lifetime[i] / (float)particle_lifetime_total[i];
            float a = GetParticleAlpha(i);
            col = al_map_rgba_f(col.r*a,col.g*a,col.b*a,a);
            al_put_blended_pixel(ToScreenSpace_X(particle_x[i]),ToScreenSpace_Y(particle_y[i]),col);
            //al_put_blended_pixel(ToScreenSpace_X(particle_x[i])+2,ToScreenSpace_Y(particle_y[i])+2,GROUND_DARK);

        }
    }
    al_unlock_bitmap(al_get_target_bitmap());

    al_set_blender(beforeOp, beforeSrc, beforeDst);

}
void RandParticleAroundEdgeOfCircle(float cx, float cy, float r, float lifetime, float maxspeed, Color col)
{
    float angle = RandRange(-2*M_PI,2*M_PI);
    float x = cx + cosf(angle)*r;
    float y = cy + sinf(angle)*r;
    AddParticle(x,y,RandRange(_MIN(lifetime,1),lifetime),RandRange(maxspeed/2.0f,maxspeed),RadToDeg(angle),col);
}
inline bool ParticleIsActive(int index)
{
    return (particle_properties[index] & PARTICLE_ENABLED);
}
