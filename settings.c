#include "settings.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "sprite.h"

#include "allegro5/allegro.h"

void InitSettings(char* path)
{

    SetDefaultSettings(&defaultSettings);
    LoadSettingsFile(path);
}   
void UpdateBind(UIElement* u)
{
    KeyInput* t = (KeyInput*)u->data;
    if (t->mappedTo)
    {
        //char ch = t->key;
        //currSettings.keymap.key_Q.keyMappedTo = ch;
        const char* str = al_keycode_to_name(*t->mappedTo);

        if (t->text)
        {
            t->text = realloc(t->text,(strlen(str)+1)*sizeof(char));
        }
        else
            t->text = calloc(strlen(str)+1,sizeof(char));
        strcpy(t->text,str);
    }
}
bool IsModifierDown(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame)
{
    return (
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_TAB)      || 
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_CAPSLOCK) || 
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_LSHIFT)   ||
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_RSHIFT)   || 
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_TAB)      ||
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_LWIN)     ||
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_RWIN)     ||
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_LCTRL)    ||
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_RCTRL)    ||
        al_key_down(keyStateThisFrame,ALLEGRO_KEY_COMMAND) 

    );

}
bool IsBindReleasedThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k)
{
    return ((!al_key_down(keyStateThisFrame, k.keyMappedTo) && al_key_down(keyStateLastFrame, k.keyMappedTo)) || (!al_key_down(keyStateThisFrame, k.secondKeyMappedTo) && al_key_down(keyStateLastFrame, k.secondKeyMappedTo)));
}
bool IsBindDownThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k)
{
    return (al_key_down(keyStateThisFrame, k.keyMappedTo) && !al_key_down(keyStateLastFrame, k.keyMappedTo) || al_key_down(keyStateThisFrame, k.secondKeyMappedTo) && !al_key_down(keyStateLastFrame, k.secondKeyMappedTo));
}
bool IsBindDown(ALLEGRO_KEYBOARD_STATE* keyState, Key k)
{
    return (al_key_down(keyState, k.keyMappedTo) || al_key_down(keyState, k.secondKeyMappedTo));
}
void SetDefaultSettings(Settings* setting)
{
    setting->renderScale = &_RENDERSIZE;
    setting->particlesEnabled = true;
    setting->displayHealthBar = OPTION_HPBAR_ALWAYS;
    setting->masterVolume = 1.0f;
    setting->displayTimer = false;
    
    setting->keymap.key_ctrlgroups[0] = (Key){"Ctrl Group 1",ALLEGRO_KEY_1};
    setting->keymap.key_ctrlgroups[1] = (Key){"Ctrl Group 2",ALLEGRO_KEY_2};
    setting->keymap.key_ctrlgroups[2] = (Key){"Ctrl Group 3",ALLEGRO_KEY_3};
    setting->keymap.key_ctrlgroups[3] = (Key){"Ctrl Group 4",ALLEGRO_KEY_4};
    setting->keymap.key_ctrlgroups[4] = (Key){"Ctrl Group 5",ALLEGRO_KEY_5};
    setting->keymap.key_ctrlgroups[5] = (Key){"Ctrl Group 6",ALLEGRO_KEY_6};
    setting->keymap.key_ctrlgroups[6] = (Key){"Ctrl Group 7",ALLEGRO_KEY_7};
    setting->keymap.key_ctrlgroups[7] = (Key){"Ctrl Group 8",ALLEGRO_KEY_8};
    setting->keymap.key_ctrlgroups[8] = (Key){"Ctrl Group 9",ALLEGRO_KEY_9};
    setting->keymap.key_ctrlgroups[9] = (Key){"Ctrl Group 0",ALLEGRO_KEY_0};

    setting->keymap.key_Q = (Key){"Ability One",ALLEGRO_KEY_Q};
    setting->keymap.key_W = (Key){"Ability Two",ALLEGRO_KEY_W};
    setting->keymap.key_E = (Key){"Ability Three",ALLEGRO_KEY_E};
    setting->keymap.key_R = (Key){"Ability Four",ALLEGRO_KEY_R};

    setting->keymap.key_AMove = (Key){"Attack Move",ALLEGRO_KEY_A};
    setting->keymap.key_Shift = (Key){"Set Waypoint",ALLEGRO_KEY_LSHIFT};
    setting->keymap.key_Tab = (Key){"Next unit in group",ALLEGRO_KEY_TAB};
    setting->keymap.key_Ctrl = (Key){"Add unit to group",ALLEGRO_KEY_LCTRL};

    setting->keymap.key_ShowMap = (Key){"Show Map",ALLEGRO_KEY_BACKQUOTE};

    setting->keymap.key_Pause = (Key){"Pause",ALLEGRO_KEY_P};
    setting->keymap.key_Cancel = (Key){"Pause",ALLEGRO_KEY_ESCAPE};

    setting->keymap.key_PanLeft = (Key){"Pause",ALLEGRO_KEY_LEFT};
    setting->keymap.key_PanRight = (Key){"Pause",ALLEGRO_KEY_RIGHT};
    setting->keymap.key_PanUp = (Key){"Pause",ALLEGRO_KEY_UP};
    setting->keymap.key_PanDown = (Key){"Pause",ALLEGRO_KEY_DOWN};

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