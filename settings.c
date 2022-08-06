#include "settings.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "sprite.h"
#include "allegro5/allegro.h"
void InitSettings(char* path)
{

    SetDefaultSettings(&defaultSettings);
    LoadSettingsFile(path);

}
void SetDefaultSettings(Settings* setting)
{
    setting->renderScale = &_RENDERSIZE;
    setting->particlesEnabled = true;
    setting->displayHealthBar = OPTION_HPBAR_ALWAYS;
    setting->masterVolume = 1.0f;
    setting->displayTimer = false;

}
void EndSettings()
{
    WriteSettingsFile("config.cfg");
}
void WriteSettingsFile(char* path)
{
    if (path)
    {
        char* str = calloc(4096,sizeof(char));
        if (!str) return;
        sprintf(str,"renderscale %i;\nparticles_enabled %i;\ndisplay_health_bar %i;\nvolume %f;\ndisplay_timer %i;\n",
        *   currSettings.renderScale,
        currSettings.particlesEnabled == true ? 1 : 0,
        currSettings.displayHealthBar,

        currSettings.masterVolume,
        currSettings.displayTimer ? 1 : 0
        );

        ALLEGRO_FILE* file = al_fopen(path,"w+");
        al_fwrite(file,str,strlen(str)*sizeof(char));
        al_fclose(file);

        free(str);
    }
}
float FindToken(char* str, char* token)
{
    if (!str || !token) return -1;

    int len = strlen(str);
    int indexOf = strstr(str,token)-str;
    if (indexOf < 0)
    {
        return -1;
    }
    int foundIndex = -1;
    //search forward for a ;
    for (int i = indexOf; i < len; i++)
    {
        if (str[i] == ';')
        {
            foundIndex = i;
            break;
        }
    }
    //value is between indexOf - foundIndex
    bool spaceFound;
    for (int i = indexOf+1; i < foundIndex; i++)
    {
        if (str[i-1] == ' '  && isdigit(str[i]))
        {
            char* value = calloc(len,sizeof(char));
            int index = 0;
            //found value
            for (int j = i; i < foundIndex; j++)
            {
                value[index++] = str[j];
                if (str[j] == ';')  
                {
                    //got value; convert value to int
                    value[index-1] = '\0';
                    float ret = atof(value);
                    free(value);
                    return ret;
                }
            }
            free(value);

        }
    }
    return -1;
}
bool LoadSettingsFile(char* path)
{
    currSettings = defaultSettings;
    if (path)
    {
        ALLEGRO_FILE* file = al_fopen(path,"r");
        if (file)
        {
            int size = al_fsize(file);
            char* str  = calloc(al_fsize(file)+2,sizeof(char));
            al_fread(file,str,al_fsize(file));

            float renderscale  = FindToken(str,"renderscale");
            if (renderscale > 0)
            {
                *currSettings.renderScale = renderscale;
            }
            float masterVol  = FindToken(str,"volume");
            if (masterVol >= 0)
            {
                currSettings.masterVolume =  masterVol;
            }
            float particlesEnabled  = FindToken(str,"particles_enabled");
            if (particlesEnabled >= 0)
            {
                currSettings.particlesEnabled = (int)particlesEnabled;
            }
            
            float displayHealthBar  = FindToken(str,"display_health_bar");
            if (displayHealthBar >= 0)
            {
                currSettings.displayHealthBar =  (Option_HealthBar)displayHealthBar;
            }

            float displayTimer  = FindToken(str,"display_timer");
            if (displayTimer >= 0)
            {
                if (displayTimer <= 0.001f)
                    currSettings.displayTimer = false;
                else
                    currSettings.displayTimer = true;
            }



            free(str);
            return true;
        }
    }
    return false;
}   