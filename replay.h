#pragma once

//100mb
#define REPLAY_PREALLOC 1048576 

typedef struct ReplayFrame
{
}ReplayFrame;

typedef struct Replay
{
    void* data;
    int numFrames;
    long long position;
}Replay;

Replay replay;

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

void InitReplay();
void NewReplay();
void SaveFrame(ALLEGRO_BITMAP* screen);
void LoadFrame(ALLEGRO_BITMAP* screen);

char* frameTest;
int frameTestSize;