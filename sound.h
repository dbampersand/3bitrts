#pragma once

#include <allegro5/allegro_audio.h>
typedef int SoundIndex; 
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



void InitSound();
int LoadSound(const char* path);
void PlaySound(Sound* s, float relativeVolume);
void PlayMusic(const char* path, float loopPoint, float musicGain);
void UpdateMusic(float dt);
void PlayEncounterMusic();
void StopMusic();
void ResetSoundsThisFrame();



#define RESERVED_SAMPLES 16
#define NUMSOUNDSTOPREALLOC 32

extern ALLEGRO_AUDIO_STREAM* music;
extern ALLEGRO_AUDIO_STREAM* musicFadingTo;

extern float musicVolMixer1;
extern float musicVolMixer2;

float musicGain1; 
float musicGain2;

extern ALLEGRO_VOICE* musicVoice1;
extern ALLEGRO_VOICE* musicVoice2;

extern ALLEGRO_MIXER* musicMixer1;
extern ALLEGRO_MIXER* musicMixer2;

extern char* musicPath;

extern Sound* sounds;
extern int numSoundsAllocated;
extern int numSounds;

extern MusicState musicState;
