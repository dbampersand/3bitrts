#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "ui.h"
#include "sound.h"
//100mb
#define REPLAY_PREALLOC 1048576 
#define NUM_SOUNDS_TO_SAVE 32
#define REP_UI_PLAY_SCRUBBER_SIZE 215

//for saving in replay
int soundPlayedThisFramePosition;

typedef struct ReplayFrame
{
    char* compressedData;
    uint16_t dataLen;
    Sound soundsPlayedThisFrame[NUM_SOUNDS_TO_SAVE];
}ReplayFrame;

typedef struct Replay
{
    ReplayFrame* frames;
    uint16_t numFrames;
    uint16_t framePlayPosition;
    bool playing;


}Replay;

Replay replay;
UIElement replayPlayButton; 

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

void InitReplay();
void NewReplay();
ReplayFrame SaveFrame(ALLEGRO_BITMAP* screen);
void LoadFrame(ALLEGRO_BITMAP* screen, ReplayFrame* frame);
void RecordReplay(ALLEGRO_BITMAP* screen);
void PlayReplay(ALLEGRO_BITMAP* screen);

void ReplayToDisk(Replay* r);

bool LoadReplay(char* path);

