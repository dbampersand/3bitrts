#pragma once

#include <allegro5/allegro_audio.h>

typedef struct Sound
{
    ALLEGRO_SAMPLE* sample;
    char* path;
} Sound;

void InitSound();
Sound* LoadSound();
void PlaySound(Sound* s);

#define RESERVED_SAMPLES 16
#define NUMSOUNDSTOPREALLOC 32

Sound* sounds;
int numSoundsAllocated;
int numSounds;