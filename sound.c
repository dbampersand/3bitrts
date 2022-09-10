#include <stdio.h>

#include "allegro5/allegro_acodec.h"

#include "sound.h"

#include "ui.h"
#include "helperfuncs.h"
#include "gamestate.h"
#include "encounter.h"
#include "settings.h"
#include "replay.h"
void InitSound()
{
    al_init_acodec_addon();
    al_reserve_samples(RESERVED_SAMPLES);

    if (!sounds)
    {
        sounds = calloc(NUMSOUNDSTOPREALLOC,sizeof(Sound));
        numSoundsAllocated = NUMSOUNDSTOPREALLOC;
        numSounds = 1;

        //null sound
        sounds[0].path = calloc(1,sizeof(char));
        sounds[0].sample = NULL;
    }
        
    ui.uiClickedSound_Index = LoadSound("assets/audio/click.wav");
    ui.uiClickedUpSound_Index = LoadSound("assets/audio/click_up.wav");


    musicMixer1 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    musicMixer2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    musicVoice1 = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    musicVoice2 = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);

    al_attach_mixer_to_voice(musicMixer1, musicVoice1);
    al_attach_mixer_to_voice(musicMixer2, musicVoice2);


}
void ResetSoundsThisFrame()
{
    //memset(soundsPlayedThisFrame,0,NUM_SOUNDS_TO_SAVE*sizeof(Sound));
    soundPlayedThisFramePosition = 0;
}
int LoadSound(char* path)
{
    if (!sounds)
    {
        sounds = calloc(NUMSOUNDSTOPREALLOC,sizeof(Sound));
        numSoundsAllocated = NUMSOUNDSTOPREALLOC;
        numSounds = 0;
    }    
    if (strlen(path) <= 0)
        return 0;


    for (int i = 0; i < numSounds; i++)
    {
        if (sounds[i].path)
        {
            if (strcmp(sounds[i].path,path)==0)
            {
                return i;
            }
        }
        
    }

    if (numSounds >= numSoundsAllocated)
    {
        sounds = realloc(sounds,(numSoundsAllocated+NUMSOUNDSTOPREALLOC)*sizeof(Sound));
        numSoundsAllocated = numSounds + NUMSOUNDSTOPREALLOC;
    }

    sounds[numSounds].sample = al_load_sample(path);
    sounds[numSounds].path = calloc(strlen(path)+1,sizeof(char));
    strcpy(sounds[numSounds].path,path);
    numSounds++;

    return numSounds-1;
}

void PlaySound(Sound* s, float relativeVolume)
{
    if (!s->sample)
    {
        if (s->path)
        {
            int i = LoadSound(s->path);
            s->sample = sounds[i].sample;
        }   
    }
    if (gameState == GAMESTATE_INGAME && soundPlayedThisFramePosition < NUM_SOUNDS_TO_SAVE)
    {
        {
            replay.frames[replay.numFrames-1].soundsPlayedThisFrame[soundPlayedThisFramePosition] = *s;
            soundPlayedThisFramePosition++;
        }
    }
    al_play_sample(s->sample, currSettings.masterVolume * relativeVolume, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void StopMusic()
{
    if (music)
        al_destroy_audio_stream(music);
    if (musicFadingTo)
        al_destroy_audio_stream(musicFadingTo);

    music = NULL;
    musicFadingTo = NULL;
    musicVolMixer1 = 0;
    musicVolMixer2 = 0;

}
void PlayMusic(const char* path)
{
    if (musicFadingTo)
        return;
    if (musicPath)
    {
        free(musicPath);
    }
    musicPath = calloc(strlen(path)+1,sizeof(char));
    strcpy(musicPath,path);
    musicFadingTo = al_load_audio_stream(path, 4, 2048);
    if (!musicFadingTo) {
        printf("Audio stream could not be created: %s\n",path);
        fflush(stdout);
    }
    if (musicFadingTo)
        if (!al_attach_audio_stream_to_mixer(musicFadingTo, musicMixer2))
        {
            printf("al_attach_audio_stream_to_mixer failed.\n");
        }
    musicVolMixer2 = 0;
    al_set_audio_stream_playmode(musicFadingTo,ALLEGRO_PLAYMODE_LOOP);
    //al_set_voice_playing(musicVoice,true);
    //al_register_event_source(queue, al_get_audio_stream_event_source(musicFadingTo));
}
void PlayEncounterMusic()
{
    if (currEncounterRunning->musicPath)
    {
        PlayMusic(currEncounterRunning->musicPath);
    }
}
void UpdateMusic(float dt)
{
    if (musicFadingTo)
    {
        musicVolMixer1 -= dt*2;
        musicVolMixer2 += dt*2;

        musicVolMixer1 = clamp(musicVolMixer1,0,1);
        musicVolMixer2 = clamp(musicVolMixer2,0,1);

        al_set_mixer_gain(musicMixer1,musicVolMixer1);
        al_set_mixer_gain(musicMixer2,musicVolMixer2);
        if (musicVolMixer2 >= 0.99f)
        {
            musicVolMixer2 = 0.0f;
            musicVolMixer1 = 1.0f;

            if (music)
                al_destroy_audio_stream(music);

            ALLEGRO_MIXER* temp = musicMixer1;
            musicMixer1 = musicMixer2;
            musicMixer2 = temp;

            music = musicFadingTo;
            musicFadingTo = NULL;

            al_set_mixer_gain(musicMixer1,musicVolMixer1);
            al_set_mixer_gain(musicMixer2,musicVolMixer2);
            

        }
        
    }
}