#include <stdio.h>

#include "allegro5/allegro_acodec.h"

#include "sound.h"

#include "ui.h"
#include "helperfuncs.h"
#include "gamestate.h"
#include "encounter.h"
#include "settings.h"
#include "replay.h"
#include "map.h"
#include "player.h"
#include "dirent.h"
#include "console.h"

#include <string.h>
#include <math.h>


ALLEGRO_AUDIO_STREAM* music = NULL;
ALLEGRO_AUDIO_STREAM* musicFadingTo = NULL;

float musicVolMixer1 = 0;
float musicVolMixer2 = 0;

ALLEGRO_VOICE* musicVoice1 = NULL;
ALLEGRO_VOICE* musicVoice2 = NULL;

ALLEGRO_MIXER* musicMixer1 = NULL;
ALLEGRO_MIXER* musicMixer2 = NULL;

char* musicPath = NULL;

Sound* sounds = NULL;
int numSoundsAllocated = 0;
int numSounds = 0;

 MusicState musicState = {0};

int* ambientSounds = NULL; 
int numAmbientSounds = 0;
int numAmbientSoundsAlloced = 0;
  
float timeToNextAmbience = 0;

int** selectionSounds = NULL;
int* numSelectionSounds = NULL;

void PlaySoundStr(char* str, float volume, float pan)
{
    int index = LoadSound(str);
    Sound* s = &sounds[index];
    PlaySound(s,volume,pan);
}

int popcnt(int n)
{
    int bits = 0;
    while (n > 0)
    {
        n &= n - 1;
        bits++;
    }
    return bits;
}
void PlaySelectionSound(GameObject* g)
{
    if (g->category == TYPE_ALL)
    {

    }
    else if (g->category == 0)
    {
        return;
    }
    else
    {
        if (selectionSounds[g->category])
        {
            int ind = selectionSounds[g->category][RandRangeI(0,numSelectionSounds[g->category])];
            Sound* s = &sounds[ind];
            PlaySoundAtPosition(s,0.1,g->position.worldX,g->position.worldY);
        }
    }
}

bool ExtensionIsValidAudio(char* ext)
{
    return  (strcasecmp(ext,"wav") == 0 || strcasecmp(ext,"flac") == 0 || strcasecmp(ext,"ogg")  == 0|| strcasecmp(ext,"it")  == 0 || strcasecmp(ext,"s3m") == 0|| strcasecmp(ext,"xm") == 0);
}
Sound* GetRandomAmbient()
{
    int randSound = ambientSounds[RandRangeI(0,numAmbientSounds)];
    return &sounds[randSound];
}
void UpdateAmbience(float dt)
{
    timeToNextAmbience -= dt;

    if (timeToNextAmbience <= 0)
    {
        Sound* s = GetRandomAmbient();
        PlaySound(s,1,RandRange(-0.25,0.25));
        int len = al_get_sample_length(s->sample);
        int freq = al_get_sample_frequency(s->sample);
        timeToNextAmbience = freq / (float)len * 1000;
        timeToNextAmbience -= RandRange(1,5);
    }
}
void AddAmbientSound(int index)
{
    if (!ambientSounds)
        ambientSounds = calloc(NUMSOUNDSTOPREALLOC,sizeof(ambientSounds[0]));
    if (numAmbientSounds >= numAmbientSoundsAlloced)
    {
        numAmbientSoundsAlloced += NUMSOUNDSTOPREALLOC;
        ambientSounds = realloc(ambientSounds,numAmbientSoundsAlloced * sizeof(ambientSounds[0]));
    }

    ambientSounds[numAmbientSounds] = index;
    numAmbientSounds++;
}
void LoadAmbientSounds()
{
    const char* path = "assets/audio/ambient/";
    DIR* d;
    struct dirent* dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char* ext = dir->d_name;
            for (int i = 0; i < strlen(ext); i++)
            {
                if (ext[i] == '.')
                    ext = &ext[i+1];
            }

            if (ExtensionIsValidAudio(ext))
            {
                char* fullPath = calloc(strlen(path) + strlen(dir->d_name)+1,sizeof(char));
                strcpy(fullPath,path);
                strcat(fullPath,dir->d_name);
                int index = LoadSound(fullPath);
                AddAmbientSound(index);
                free(fullPath);
            }


        }
    }

}
void AddToSelectionSound(GAMEOBJ_TYPE_HINT typeHint, char* path)
{
    int index = LoadSound(path);
    if (!selectionSounds[typeHint])
    {
        selectionSounds[typeHint] = calloc(NUMSOUNDSTOPREALLOC,sizeof(int));
    }    
    if (numSelectionSounds[typeHint] +1 % NUMSOUNDSTOPREALLOC == 0)
    {
        selectionSounds[typeHint] = realloc(selectionSounds[typeHint],(numSelectionSounds[typeHint]+NUMSOUNDSTOPREALLOC)*sizeof(int));
    }
    
    selectionSounds[typeHint][numSelectionSounds[typeHint]] = index;

    numSelectionSounds[typeHint]++;
}
void LoadSelectionSounds(char* path, GAMEOBJ_TYPE_HINT typeHint)
{
    DIR* d;
    struct dirent* dir;
    d = opendir(path);

    if (!selectionSounds)
    {
        selectionSounds = calloc(TYPE_ALL+1,sizeof(int*));
        numSelectionSounds = calloc(TYPE_ALL+1,sizeof(int));
    }

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char* ext = dir->d_name;
            for (int i = 0; i < strlen(ext); i++)
            {
                if (ext[i] == '.')
                    ext = &ext[i+1];
            }
            if (ExtensionIsValidAudio(ext))
            {
                char* fullPath = calloc(strlen(path) + strlen(dir->d_name)+1,sizeof(char));
                strcpy(fullPath,path);
                strcat(fullPath,dir->d_name);

                AddToSelectionSound(typeHint,fullPath);
                free(fullPath);
            }

        }
    }

}

void InitSound()
{
    if (al_install_audio())
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

        LoadAmbientSounds();
        LoadSelectionSounds("assets/audio/selection_sounds/ranged_dps/",TYPE_RANGEDDPS);
        LoadSelectionSounds("assets/audio/selection_sounds/melee_dps/",TYPE_MELEEDPS);
        LoadSelectionSounds("assets/audio/selection_sounds/utility/",TYPE_UTILITY);
        LoadSelectionSounds("assets/audio/selection_sounds/tank/",TYPE_TANK);
        LoadSelectionSounds("assets/audio/selection_sounds/healer/",TYPE_HEALER);


    }

}
void ResetSoundsThisFrame()
{
    //memset(soundsPlayedThisFrame,0,NUM_SOUNDS_TO_SAVE*sizeof(Sound));
    soundPlayedThisFramePosition = 0;
}
int LoadSound(const char* path)
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
    ALLEGRO_SAMPLE* sample = al_load_sample(path);
    if (sample)
    {
        sounds[numSounds].sample = al_load_sample(path);
        sounds[numSounds].path = calloc(strlen(path)+1,sizeof(char));
        strcpy(sounds[numSounds].path,path);
        numSounds++;
    }
    else
    {
        ConsolePrintf("Sound: could not load path: %s\n",path ? path : "NULL");
        return 0;
    }

    return numSounds-1;
}
void PlaySoundAtPosition(Sound* s, float relativeVolume, int x, int y)
{
    float camX = GetCameraMiddleX();
    float camY = GetCameraMiddleY();

    float xPercent = clamp((x - camX) / _SCREEN_SIZE,-0.4f,0.4f);
    float yPercent = (y - camY) / _SCREEN_SIZE;

    float distance = dist(x,y,camX,camY) / (_SCREEN_SIZE/4);
    if (distance <= 0.0001f)
        distance = 0.0001f;

    float volumeGain = 1 / distance;//clamp(1 / distance,0,1); 
    volumeGain = clamp(volumeGain,0,1);



    PlaySound(s,relativeVolume*volumeGain,xPercent);

} 

void PlaySound(Sound* s, float relativeVolume, float pan)
{
    if (!s->sample)
    {
        if (s->path)
        {
            int i = LoadSound(s->path);
            s->sample = sounds[i].sample;
            s->path = sounds[i].path;
            ConsolePrintf("LOAD\n");
            ConsolePrintf("path: %s",s->path);
        }   
    }
    if (gameState == GAMESTATE_INGAME && soundPlayedThisFramePosition <= NUM_SOUNDS_TO_SAVE && replay.numFrames > 0)
    {
        {

            replay.frames[replay.numFrames].soundsPlayedThisFrame[soundPlayedThisFramePosition] = *s;
            soundPlayedThisFramePosition++;
        }
    }
    float volumeJitter = RandRange(-0.05,0.05);
    float pitchJitter = RandRange(-0.025,0.025);
    relativeVolume += volumeJitter;

    al_play_sample(s->sample, currSettings.masterVolume * relativeVolume * currSettings.sfxVolume, pan, 1.0f + pitchJitter, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void StopMusic()
{
    /*if (music)
        al_destroy_audio_stream(music);
    if (musicFadingTo)
        al_destroy_audio_stream(musicFadingTo);
*/
    //music = NULL;
    //musicFadingTo = NULL;
    //musicVolMixer1 = 0;
    //musicVolMixer2 = 0;
    if (musicPath)
        free(musicPath);
    musicPath = NULL;

    musicState = MUSICSTATE_FADINGOUT;

}
void PlayMusic(const char* path, float musicGain, double loopPoint)
{
    if (musicFadingTo)
        return;
    if (musicPath && path && PathCmp((char*)path,musicPath) == true)
        return;
    if (musicPath)
    {
        free(musicPath);
    }
    musicPath = calloc(strlen(path)+1,sizeof(char));
    strcpy(musicPath,path);
    musicFadingTo = al_load_audio_stream(path, 4, 2048);
        if (!musicFadingTo) {
        ConsolePrintf("Audio stream could not be created: %s\n",path);
        fflush(stdout);
        return;
    }
    if (musicFadingTo)
        if (!al_attach_audio_stream_to_mixer(musicFadingTo, musicMixer2))
        {
            ConsolePrintf("al_attach_audio_stream_to_mixer failed.\n");
        }
    musicVolMixer2 = 0;

    al_set_audio_stream_playmode(musicFadingTo,ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_loop_secs(musicFadingTo,(double)loopPoint,al_get_audio_stream_length_secs(musicFadingTo));
    musicState = MUSICSTATE_PLAYING;
    musicGain2 = musicGain;
    UpdateMusic(0);
    //al_set_voice_playing(musicVoice,true);
    //al_register_event_source(queue, al_get_audio_stream_event_source(musicFadingTo));
}
void PlayEncounterMusic()
{
    if (currEncounterRunning->musicPath)
    {
        PlayMusic(currEncounterRunning->musicPath,1,0);
    }
}
void UpdateMusic(float dt)
{
    float max = currSettings.musicVolume * currSettings.masterVolume;

    if (musicState  == MUSICSTATE_FADINGOUT)
    {
        musicVolMixer1 -= dt * .75f;
        musicVolMixer2 -= dt * .75f;
        musicVolMixer1 = clamp(musicVolMixer1,0,max);
        musicVolMixer2 = clamp(musicVolMixer2,0,max);

        al_set_mixer_gain(musicMixer1,musicVolMixer1 * musicGain1);
        al_set_mixer_gain(musicMixer2,musicVolMixer2 * musicGain2);
        if (music && musicVolMixer1 <= 0)
        {
            al_destroy_audio_stream(musicFadingTo);
            al_destroy_audio_stream(music);
            musicFadingTo = NULL;
            music = NULL;
        }
        if (musicFadingTo && musicVolMixer2 <= 0)
        {

            al_destroy_audio_stream(musicFadingTo);
            al_destroy_audio_stream(music);
            music = NULL;
            musicFadingTo = NULL;
        }

    }
    else
    {
        if (musicFadingTo)
        {
            musicVolMixer1 -= dt*2;
            musicVolMixer2 += dt*2;

            musicVolMixer1 = clamp(musicVolMixer1,0,max);
            musicVolMixer2 = clamp(musicVolMixer2,0,max);

            al_set_mixer_gain(musicMixer1,musicVolMixer1 * musicGain1);
            al_set_mixer_gain(musicMixer2,musicVolMixer2 * musicGain2);
            if (musicVolMixer2 >= max)
            {
                musicVolMixer2 = 0.0f;
                musicVolMixer1 = max;

                if (music)
                    al_destroy_audio_stream(music);

                ALLEGRO_MIXER* temp = musicMixer1;
                musicMixer1 = musicMixer2;
                musicMixer2 = temp;

                float tempGain = musicGain1;
                musicGain1 = musicGain2;
                musicGain2 = tempGain;

                music = musicFadingTo;
                musicFadingTo = NULL;

                al_set_mixer_gain(musicMixer1,musicVolMixer1 * musicGain1);
                al_set_mixer_gain(musicMixer2,musicVolMixer2 * musicGain2);

            }
            
        }
        else
        {
            musicVolMixer1 = max;
            al_set_mixer_gain(musicMixer1,musicVolMixer1 * musicGain1);


        }
    }
}
