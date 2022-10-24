#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "ui.h"
#include "sound.h"
#include <stdio.h>
#include "gameobject.h"
#include "attack.h"

#define REPLAY_PREALLOC 39321600
//10 seconds worth @ 60fps 
#define FRAMES_PREALLOC 60 * 10

#define NUM_SOUNDS_TO_SAVE 32
#define REP_UI_PLAY_SCRUBBER_SIZE 204   

#define TEMP_REPLAY_NAME "CURRREP"



//TODO: move replay code to this kind of structure
/*

typedef struct GameStateFrame
{
    int numObjects;
    GameObject* objects[MAX_OBJS];

} GameStateFrame;

typedef struct GameStateReplay
{
    int numSprites;
    Sprite* sprites;

    GameStateFrame* frames;
    uint32_t numFrames; 
    uint32_t framePlayPosition;
    bool playing;

    uint32_t totalFrames;

} GameStateReplay;
*/

typedef struct ReplayFrame
{
    int32_t numObjects;
    GameObject objects[MAX_OBJS];
    int32_t numAttacks;
    Attack attacks[MAX_ATTACKS];


    char* compressedData;
    uint16_t dataLen;
    Sound soundsPlayedThisFrame[NUM_SOUNDS_TO_SAVE];
}ReplayFrame;

typedef struct Replay
{
    Sprite* sprites;
    int numSprites;

    ReplayFrame* frames;
    uint32_t numFrames; 
    uint32_t framePlayPosition;
    bool playing;

    uint32_t totalFrames;
}Replay;

extern Replay replay;
extern UIElement replayPlayButton; 
extern UIElement replayBackButton; 
 
extern const char* REPLAY_HEADER;


extern long bufferPosition;
extern char* replayBuffer;

extern int soundPlayedThisFramePosition;

extern FILE* tempFile;


typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

void InitReplay();
void NewReplay();
ReplayFrame SaveFrame(ALLEGRO_BITMAP* screen);
void LoadFrame(ALLEGRO_BITMAP* screen, ReplayFrame* frame);
void RecordReplay(ALLEGRO_BITMAP* screen);
void PlayReplay(ALLEGRO_BITMAP* screen);

void ReplayToDisk(Replay* r);

bool LoadReplay(char* path);
void RemoveReplay(Replay* r);
void SerializeSection(Replay* r, bool finished);

int def(FILE *source, FILE *dest, int level);
void zerr(int ret);
