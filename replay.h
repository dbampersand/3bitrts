#pragma once
#include <stdint.h>
#include <stdbool.h>
//100mb
#define REPLAY_PREALLOC 1048576 

typedef struct ReplayFrame
{
    char* compressedData;
    uint16_t dataLen;
}ReplayFrame;

typedef struct Replay
{
    ReplayFrame* frames;
    uint16_t numFrames;
    uint16_t framePlayPosition;

}Replay;

Replay replay;

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

void InitReplay();
void NewReplay();
ReplayFrame SaveFrame(ALLEGRO_BITMAP* screen);
void LoadFrame(ALLEGRO_BITMAP* screen, ReplayFrame* frame);
void RecordReplay(ALLEGRO_BITMAP* screen);
void PlayReplay(ALLEGRO_BITMAP* screen);

void ReplayToDisk(Replay* r);

bool LoadReplay(char* path);
