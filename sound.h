#pragma once

#include <allegro5/allegro_audio.h>
#include "hashtable.h"
typedef int SoundIndex;

#define RESERVED_SAMPLES 256
#define NUMSOUNDSTOPREALLOC 32

typedef struct Sound
{
    ALLEGRO_SAMPLE* sample;
    char* path;
} Sound;
typedef struct Reverb
{
    bool active;
    SoundIndex soundIndex;
    float time;
    float volume;
    float pan;
} Reverb;

#define MAX_REVERBS (RESERVED_SAMPLES / 2)
extern Reverb reverbs[MAX_REVERBS];
extern int _REVERB_TOP; 

extern float _REVERB_DISTANCE;

typedef enum MusicState {
    MUSICSTATE_PLAYING,
    MUSICSTATE_FADINGOUT,
    MUSICSTATE_FADINGIN

}MusicState;

typedef struct GameObject GameObject;

extern int** selectionSounds;
extern int* numSelectionSounds;

extern int numAmbientSounds;
extern int numAmbientSoundsAlloced;

extern ALLEGRO_AUDIO_STREAM* ambienceStream;
extern char* ambientPath;

extern HashTable soundIndexHashes;


void InitSound();
int LoadSound(const char* path);
void PlaySoundStr(char* str, float volume, float pan,bool shouldReverb);
void PlaySoundAtPosition(Sound* s, float relativeVolume, int x, int y, bool shouldReverb);
ALLEGRO_SAMPLE_ID PlaySound(Sound* s, float relativeVolume, float pan,bool shouldReverb);
void UpdateReverbs(float dt);
void PlayMusic(const char* path, float loopPoint, double musicGain);
void UpdateMusic(float dt);
void PlayEncounterMusic();
void StopMusic();
void ResetSoundsThisFrame();
void UpdateAmbience(float dt);

void PlaySelectionSound(GameObject* g);




extern ALLEGRO_AUDIO_STREAM* music;
extern ALLEGRO_AUDIO_STREAM* musicFadingTo;

extern float musicVolMixer1;
extern float musicVolMixer2;

extern float musicGain1; 
extern float musicGain2;

extern ALLEGRO_VOICE* musicVoice1;
extern ALLEGRO_VOICE* musicVoice2;

extern ALLEGRO_MIXER* musicMixer1;
extern ALLEGRO_MIXER* musicMixer2;

extern ALLEGRO_VOICE* ambientVoice;
extern ALLEGRO_MIXER* ambientMixer;


extern char* musicPath;

extern Sound* sounds;
extern int numSoundsAllocated;
extern int numSounds;

extern MusicState musicState;
