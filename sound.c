#include "sound.h"
#include "ui.h"
#include "allegro5/allegro_acodec.h"

void InitSound()
{
    al_init_acodec_addon();
    al_reserve_samples(RESERVED_SAMPLES);

    if (!sounds)
    {
        sounds = calloc(NUMSOUNDSTOPREALLOC,sizeof(Sound));
        numSoundsAllocated = NUMSOUNDSTOPREALLOC;
        numSounds = 0;
    }
    ui.uiClickedSound = LoadSound("assets/audio/other_04.ogg");


}

Sound* LoadSound(char* path)
{
    if (!sounds)
    {
        sounds = calloc(NUMSOUNDSTOPREALLOC,sizeof(Sound));
        numSoundsAllocated = NUMSOUNDSTOPREALLOC;
        numSounds = 0;
    }

    for (int i = 0; i < numSounds; i++)
    {
        if (strcmp(sounds[i].path,path)==0)
        {
            return &sounds[i];
        }
    }

    sounds[numSounds].sample = al_load_sample(path);
    sounds[numSounds].path = path;
    numSounds++;

    return &sounds[numSounds-1];
}

void PlaySound(Sound* s)
{
    al_play_sample(s->sample, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
}