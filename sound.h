#pragma once

#include <allegro5/allegro_audio.h>

typedef struct Sound
{
    ALLEGRO_SAMPLE* sample;
    char* path;
} Sound;

typedef enum MusicState {
    MUSICSTATE_PLAYING,
    MUSICSTATE_FADINGOUT,
    MUSICSTATE_FADINGIN

}MusicState;
MusicState musicState;
void InitSound();
Sound* LoadSound();
void PlaySound(Sound* s);
void PlayMusic(char* path);
void UpdateMusic(float dt);


#define RESERVED_SAMPLES 16
#define NUMSOUNDSTOPREALLOC 32

ALLEGRO_AUDIO_STREAM* music;
ALLEGRO_AUDIO_STREAM* musicFadingTo;

float musicVolMixer1;
float musicVolMixer2;

ALLEGRO_VOICE* musicVoice1;
ALLEGRO_VOICE* musicVoice2;

ALLEGRO_MIXER* musicMixer1;
ALLEGRO_MIXER* musicMixer2;


Sound* sounds;
int numSoundsAllocated;
int numSounds;