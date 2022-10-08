#pragma once

#include <allegro5/allegro_audio.h>

typedef int SoundIndex; 
typedef struct HashTable HashTable;

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
int LoadSound();
void PlaySound(Sound* s, float relativeVolume);
void PlayMusic(const char* path);
void UpdateMusic(float dt);
void PlayEncounterMusic();
void StopMusic();
void ResetSoundsThisFrame();

HashTable* soundHashTable;

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

char* musicPath;

Sound* sounds;
int numSoundsAllocated;
int numSounds;

