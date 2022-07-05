#pragma once

//100mb
#define REPLAY_PREALLOC 1048576 

typedef struct ReplayFrame
{
    char* compressedData;
    int dataLen;
}ReplayFrame;

typedef struct Replay
{
    ReplayFrame* frames;
    int numFrames;
    int framePlayPosition;

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

