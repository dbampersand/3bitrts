    #include "settings.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "sprite.h"
#include "video.h" 
#include "pointspace.h"
#include "gameobject.h"
#include "player.h"
#include "encounter.h"

#include "allegro5/allegro.h"
#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <unistd.h>
#endif

Settings currSettings = {0};
Settings defaultSettings = {0};
char* settingFilePath = NULL;


void InitPath()
{
    #ifdef __APPLE__
        uint32_t buffSize = 0;
        char* buffer = calloc(1,sizeof(char));
        _NSGetExecutablePath(buffer,&buffSize);
        buffer = realloc(buffer,(buffSize+1)*sizeof(char));
        memset(buffer,0,sizeof(char)*buffSize);
        _NSGetExecutablePath(buffer,&buffSize);
        
        //Get to home directory
        char* curr = calloc(2,sizeof(char));
        while (1)
        {
            chdir("..");
            //path should be just '/' and so should return a pointer
            //when the buffer size is capable of holding it
            char* here = getcwd(curr, 2);
            if (here)
                break;

        }   
        free(curr);

        //Traverse buffer until we get to the directory
        char *token = strtok(buffer, "/");
        while(token) {
            chdir(token);  

            token = strtok(NULL, "/");
        }
        free(buffer);
    #endif
}

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
        if (!str)
        {
            str = "(None)";
        }
        if (t->text)
        {
            t->text = realloc(t->text,(strlen(str)+1)*sizeof(char));
        }
        else
        {
            t->text = calloc(strlen(str)+1,sizeof(char));
        }
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
    bool b = ((k.keyMappedTo >= 0 && !al_key_down(keyStateThisFrame, k.keyMappedTo) && al_key_down(keyStateLastFrame, k.keyMappedTo)) || (k.secondKeyMappedTo >= 0 && !al_key_down(keyStateThisFrame, k.secondKeyMappedTo) && al_key_down(keyStateLastFrame, k.secondKeyMappedTo)));
    return b;
}
bool IsBindDownThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k)
{
    if (k.keyMappedTo == -1 && k.secondKeyMappedTo == -1)
        return false;
    bool b = ((k.keyMappedTo >= 0 && al_key_down(keyStateThisFrame, k.keyMappedTo) && !al_key_down(keyStateLastFrame, k.keyMappedTo)) || (k.secondKeyMappedTo >= 0 && al_key_down(keyStateThisFrame, k.secondKeyMappedTo) && !al_key_down(keyStateLastFrame, k.secondKeyMappedTo)));
    return b;
}
bool IsBindDown(ALLEGRO_KEYBOARD_STATE* keyState, Key k)
{
    bool b = ((k.keyMappedTo >= 0 && al_key_down(keyState, k.keyMappedTo)) || (k.secondKeyMappedTo >= 0 && al_key_down(keyState, k.secondKeyMappedTo)));
    return b;
}
void SetDefaultSettings(Settings* setting)
{
    *setting = (Settings){0};
    setting->renderScale = &_RENDERSIZE;
    setting->particlesEnabled = true;
    setting->lightEffectEnabled = true;

    setting->displayHealthBar = OPTION_HPBAR_ALWAYS;
    setting->masterVolume = 1.0f;
    setting->musicVolume = 1.0f;
    setting->sfxVolume = 1.0f;

    setting->slowdownPercent = 0.0f;
    setting->targetFPS = OPTION_60FPS;

    setting->screenShakeFactor = 1.0f;



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
    setting->keymap.key_F = (Key){"Ability Five",ALLEGRO_KEY_F};

    

    setting->keymap.key_AMove = (Key){"Attack Move",ALLEGRO_KEY_A};
    setting->keymap.key_Shift = (Key){"Set Waypoint",ALLEGRO_KEY_LSHIFT};
    setting->keymap.key_Tab = (Key){"Next unit in group",ALLEGRO_KEY_TAB};
    setting->keymap.key_Ctrl = (Key){"Add unit to group",ALLEGRO_KEY_LCTRL};

    setting->keymap.key_ShowMap = (Key){"Show Map",ALLEGRO_KEY_M};

    setting->keymap.key_Pause = (Key){"Pause",ALLEGRO_KEY_P};
    setting->keymap.key_Cancel = (Key){"Pause",ALLEGRO_KEY_ESCAPE};

    setting->keymap.key_PanLeft = (Key){"Pause",ALLEGRO_KEY_LEFT};
    setting->keymap.key_PanRight = (Key){"Pause",ALLEGRO_KEY_RIGHT};
    setting->keymap.key_PanUp = (Key){"Pause",ALLEGRO_KEY_UP};
    setting->keymap.key_PanDown = (Key){"Pause",ALLEGRO_KEY_DOWN};
    setting->keymap.key_Center = (Key){"Center",ALLEGRO_KEY_SPACE};


    setting->keymap.key_HoldPosition = (Key){"Hold Position",ALLEGRO_KEY_H};

    setting->keymap.key_Console = (Key){"Activate Console",ALLEGRO_KEY_OPENBRACE};


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
        sprintf(str,
        "renderscale %i;\n"
        "particles_enabled %i;\n"
        "light_effect_enabled %i;\n"
        "targetFPS %i;\n"
        "display_health_bar %i;\n"

        "volume %f;\n"
        "sfxVolume: %f;\n"
        "musicVolume: %f;\n"
        "slowdownPercent: %f;\n"
        "display_timer %i;\n"
        "screen_shake_factor: %f\n"
        "key_Q %i;\n"
        "key_Q_Alt %i;\n"
        "key_W %i;\n"
        "key_W_Alt %i;\n"
        "key_E %i;\n"
        "key_E_Alt %i;\n"
        "key_R %i;\n"
        "key_R_Alt %i;\n"
        "key_F %i;\n"
        "key_F_Alt %i;\n"
        "key_AMove %i;\n"
        "key_AMove_Alt %i;\n"
        "key_Shift %i;\n"
        "key_Shift_Alt %i;\n"
        "key_Tab %i;\n"
        "key_Tab_Alt %i;\n"
        "key_Ctrl %i;\n"
        "key_Ctrl_Alt %i;\n"
        "key_ShowMap %i;\n"
        "key_ShowMap_Alt %i;\n"
        "key_Pause %i;\n"
        "key_Pause_Alt %i;\n"
        "key_Cancel %i;\n"
        "key_Cancel_Alt %i;\n"
        "key_PanLeft %i;\n"
        "key_PanLeft_Alt %i;\n"
        "key_PanRight %i;\n"
        "key_PanRight_Alt %i;\n"
        "key_PanUp %i;\n"
        "key_PanUp_Alt %i;\n"
        "key_PanDown %i;\n"
        "key_PanDown_Alt %i;\n"
        "key_ctrlgroup1 %i;\n"
        "key_ctrlgroup1_Alt %i;\n"
        "key_ctrlgroup2 %i;\n"
        "key_ctrlgroup2_Alt %i;\n"
        "key_ctrlgroup3 %i;\n"
        "key_ctrlgroup3_Alt %i;\n"
        "key_ctrlgroup4 %i;\n"
        "key_ctrlgroup4_Alt %i;\n"
        "key_ctrlgroup5 %i;\n"
        "key_ctrlgroup5_Alt %i;\n"
        "key_ctrlgroup6 %i;\n"
        "key_ctrlgroup6_Alt %i;\n"
        "key_ctrlgroup7 %i;\n"
        "key_ctrlgroup7_Alt %i;\n"
        "key_ctrlgroup8 %i;\n"
        "key_ctrlgroup8_Alt %i;\n"
        "key_ctrlgroup9 %i;\n"
        "key_ctrlgroup9_Alt %i;\n"
        "key_ctrlgroup0 %i;\n"
        "key_ctrlgroup0_Alt %i;\n"


        ,
        *   currSettings.renderScale,
        currSettings.particlesEnabled == true ? 1 : 0,
        currSettings.lightEffectEnabled == true ? 1 : 0,
        currSettings.targetFPS,


        currSettings.displayHealthBar,

        (double)currSettings.masterVolume,
        (double)currSettings.sfxVolume,
        (double)currSettings.musicVolume,

        (double)currSettings.slowdownPercent,

        currSettings.displayTimer ? 1 : 0,
        
        (double)currSettings.screenShakeFactor,
        
        currSettings.keymap.key_Q.keyMappedTo,
        currSettings.keymap.key_Q.secondKeyMappedTo,
        currSettings.keymap.key_W.keyMappedTo,
        currSettings.keymap.key_W.secondKeyMappedTo,
        currSettings.keymap.key_E.keyMappedTo,
        currSettings.keymap.key_E.secondKeyMappedTo,
        currSettings.keymap.key_R.keyMappedTo,
        currSettings.keymap.key_R.secondKeyMappedTo,
        currSettings.keymap.key_F.keyMappedTo,
        currSettings.keymap.key_F.secondKeyMappedTo,

        currSettings.keymap.key_AMove.keyMappedTo,
        currSettings.keymap.key_AMove.secondKeyMappedTo,

        currSettings.keymap.key_Shift.keyMappedTo,
        currSettings.keymap.key_Shift.secondKeyMappedTo,

        currSettings.keymap.key_Tab.keyMappedTo,
        currSettings.keymap.key_Tab.secondKeyMappedTo,

        currSettings.keymap.key_Ctrl.keyMappedTo,
        currSettings.keymap.key_Ctrl.secondKeyMappedTo,

        currSettings.keymap.key_ShowMap.keyMappedTo,
        currSettings.keymap.key_ShowMap.secondKeyMappedTo,

        currSettings.keymap.key_Pause.keyMappedTo,
        currSettings.keymap.key_Pause.secondKeyMappedTo,

        currSettings.keymap.key_Cancel.keyMappedTo,
        currSettings.keymap.key_Cancel.secondKeyMappedTo,

        currSettings.keymap.key_PanLeft.keyMappedTo,
        currSettings.keymap.key_PanLeft.secondKeyMappedTo,

        currSettings.keymap.key_PanRight.keyMappedTo,
        currSettings.keymap.key_PanRight.secondKeyMappedTo,

        currSettings.keymap.key_PanUp.keyMappedTo,
        currSettings.keymap.key_PanUp.secondKeyMappedTo,

        currSettings.keymap.key_PanDown.keyMappedTo,
        currSettings.keymap.key_PanDown.secondKeyMappedTo,

        currSettings.keymap.key_ctrlgroups[1].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[1].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[2].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[2].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[3].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[3].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[4].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[4].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[5].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[5].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[6].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[6].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[7].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[7].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[8].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[8].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[9].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[9].secondKeyMappedTo,
        currSettings.keymap.key_ctrlgroups[0].keyMappedTo,
        currSettings.keymap.key_ctrlgroups[0].secondKeyMappedTo

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
                    //got value; convert value to float
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
void SetAbilityKeys(char* str)
{
    int key_Q  = (int)FindToken(str,"key_Q");
        currSettings.keymap.key_Q.keyMappedTo = key_Q;

    int key_Q_Alt  = (int)FindToken(str,"key_Q_Alt");
        currSettings.keymap.key_Q.secondKeyMappedTo = key_Q_Alt;

    int key_W = (int)FindToken(str,"key_W");
    currSettings.keymap.key_W.keyMappedTo = key_W;

    int key_W_Alt  = (int)FindToken(str,"key_W_Alt");
        currSettings.keymap.key_W.secondKeyMappedTo = key_W_Alt;

    int key_E = (int)FindToken(str,"key_E");
        currSettings.keymap.key_E.keyMappedTo = key_E;

    int key_E_Alt  = (int)FindToken(str,"key_E_Alt");
        currSettings.keymap.key_E.secondKeyMappedTo = key_E_Alt;

    int key_R = (int)FindToken(str,"key_R");
        currSettings.keymap.key_R.keyMappedTo = key_R;

    int key_R_Alt  = (int)FindToken(str,"key_R_Alt");
        currSettings.keymap.key_R.secondKeyMappedTo = key_R_Alt;

    int key_F = (int)FindToken(str,"key_F");
        currSettings.keymap.key_F.keyMappedTo = key_F;

    int key_F_Alt  = (int)FindToken(str,"key_F_Alt");
        currSettings.keymap.key_F.secondKeyMappedTo = key_F_Alt;


}
void SetControllingAbilities(char* str)
{
    int key_AMove  = (int)FindToken(str,"key_AMove");
        currSettings.keymap.key_AMove.keyMappedTo = key_AMove;

    int key_AMove_Alt  = (int)FindToken(str,"key_AMove_Alt");
        currSettings.keymap.key_AMove.secondKeyMappedTo = key_AMove_Alt;

    int key_Shift  = (int)FindToken(str,"key_Shift");
        currSettings.keymap.key_Shift.keyMappedTo = key_Shift;

    int key_Shift_Alt  = (int)FindToken(str,"key_Shift_Alt");
        currSettings.keymap.key_Shift.secondKeyMappedTo = key_Shift_Alt;

    int key_Tab  = (int)FindToken(str,"key_Tab");
        currSettings.keymap.key_Tab.keyMappedTo = key_Tab;

    int key_Tab_Alt  = (int)FindToken(str,"key_Tab_Alt");
        currSettings.keymap.key_Tab.secondKeyMappedTo = key_Tab_Alt;

    int key_Ctrl  = (int)FindToken(str,"key_Ctrl");
        currSettings.keymap.key_Ctrl.keyMappedTo = key_Ctrl;

    int key_Ctrl_Alt  = (int)FindToken(str,"key_Ctrl_Alt");
        currSettings.keymap.key_Ctrl.secondKeyMappedTo = key_Ctrl_Alt;

    int key_ShowMap  = (int)FindToken(str,"key_ShowMap");
        currSettings.keymap.key_ShowMap.keyMappedTo = key_ShowMap;

    int key_ShowMap_Alt  = (int)FindToken(str,"key_ShowMap_Alt");
    currSettings.keymap.key_ShowMap.secondKeyMappedTo = key_ShowMap_Alt;

    int key_Pause  = (int)FindToken(str,"key_Pause");
        currSettings.keymap.key_Pause.keyMappedTo = key_Pause;

    int key_Pause_Alt  = (int)FindToken(str,"key_Pause_Alt");
        currSettings.keymap.key_Pause.secondKeyMappedTo = key_Pause_Alt;

    int key_Cancel  = (int)FindToken(str,"key_Cancel");
        currSettings.keymap.key_Cancel.keyMappedTo = key_Cancel;

    int key_Cancel_Alt  = (int)FindToken(str,"key_Cancel_Alt");
        currSettings.keymap.key_Cancel.secondKeyMappedTo = key_Cancel_Alt;

}
void SetMovementKeys(char* str)
{
    int key_PanLeft  = (int)FindToken(str,"key_PanLeft");
        currSettings.keymap.key_PanLeft.keyMappedTo = key_PanLeft;

    int key_PanLeft_Alt  = (int)FindToken(str,"key_PanLeft_Alt");
        currSettings.keymap.key_PanLeft.secondKeyMappedTo = key_PanLeft_Alt;

    int key_PanRight  = (int)FindToken(str,"key_PanRight");
        currSettings.keymap.key_PanRight.keyMappedTo = key_PanRight;

    int key_PanRight_Alt  = (int)FindToken(str,"key_PanRight_Alt");
        currSettings.keymap.key_PanRight.secondKeyMappedTo = key_PanRight_Alt;

    int key_PanUp  = (int)FindToken(str,"key_PanUp");
        currSettings.keymap.key_PanUp.keyMappedTo = key_PanUp;

    int key_PanUp_Alt  = (int)FindToken(str,"key_PanUp_Alt");
        currSettings.keymap.key_PanUp.secondKeyMappedTo = key_PanUp_Alt;

    int key_PanDown  = (int)FindToken(str,"key_PanDown");
        currSettings.keymap.key_PanDown.keyMappedTo = key_PanDown;

    int key_PanDown_Alt  = (int)FindToken(str,"key_PanDown_Alt");
        currSettings.keymap.key_PanDown.secondKeyMappedTo = key_PanDown_Alt;


}
void SetBindControlGroups(char* str)
{
    int key_ctrlgroup1  = (int)FindToken(str,"key_ctrlgroup1");
        currSettings.keymap.key_ctrlgroups[1].keyMappedTo = key_ctrlgroup1;
    int key_ctrlgroup1_Alt  = (int)FindToken(str,"key_ctrlgroup1_Alt");
        currSettings.keymap.key_ctrlgroups[1].secondKeyMappedTo = key_ctrlgroup1_Alt;


    int key_ctrlgroup2  = (int)FindToken(str,"key_ctrlgroup2");
        currSettings.keymap.key_ctrlgroups[2].keyMappedTo = key_ctrlgroup2;
    int key_ctrlgroup2_Alt  = (int)FindToken(str,"key_ctrlgroup2_Alt");
        currSettings.keymap.key_ctrlgroups[2].secondKeyMappedTo = key_ctrlgroup2_Alt;

    int key_ctrlgroup3  = (int)FindToken(str,"key_ctrlgroup3");
        currSettings.keymap.key_ctrlgroups[3].keyMappedTo = key_ctrlgroup3;
    int key_ctrlgroup3_Alt  = (int)FindToken(str,"key_ctrlgroup3_Alt");
        currSettings.keymap.key_ctrlgroups[3].secondKeyMappedTo = key_ctrlgroup3_Alt;

    int key_ctrlgroup4  = (int)FindToken(str,"key_ctrlgroup4");
        currSettings.keymap.key_ctrlgroups[4].keyMappedTo = key_ctrlgroup4;
    int key_ctrlgroup4_Alt  = (int)FindToken(str,"key_ctrlgroup4_Alt");
        currSettings.keymap.key_ctrlgroups[4].secondKeyMappedTo = key_ctrlgroup4_Alt;

    int key_ctrlgroup5  = (int)FindToken(str,"key_ctrlgroup5");
        currSettings.keymap.key_ctrlgroups[5].keyMappedTo = key_ctrlgroup5;
    int key_ctrlgroup5_Alt  = (int)FindToken(str,"key_ctrlgroup5_Alt");
        currSettings.keymap.key_ctrlgroups[5].secondKeyMappedTo = key_ctrlgroup5_Alt;


    int key_ctrlgroup6  = (int)FindToken(str,"key_ctrlgroup6");
        currSettings.keymap.key_ctrlgroups[6].keyMappedTo = key_ctrlgroup6;
    int key_ctrlgroup6_Alt  = (int)FindToken(str,"key_ctrlgroup6_Alt");
        currSettings.keymap.key_ctrlgroups[6].secondKeyMappedTo = key_ctrlgroup6_Alt;

    int key_ctrlgroup7  = (int)FindToken(str,"key_ctrlgroup7");
        currSettings.keymap.key_ctrlgroups[7].keyMappedTo = key_ctrlgroup7;
    int key_ctrlgroup7_Alt  = (int)FindToken(str,"key_ctrlgroup7_Alt");
        currSettings.keymap.key_ctrlgroups[7].secondKeyMappedTo = key_ctrlgroup7_Alt;

    int key_ctrlgroup8  = (int)FindToken(str,"key_ctrlgroup8");
        currSettings.keymap.key_ctrlgroups[8].keyMappedTo = key_ctrlgroup8;
    int key_ctrlgroup8_Alt  = (int)FindToken(str,"key_ctrlgroup8_Alt");
        currSettings.keymap.key_ctrlgroups[8].secondKeyMappedTo = key_ctrlgroup8_Alt;

    int key_ctrlgroup9  = (int)FindToken(str,"key_ctrlgroup9");
        currSettings.keymap.key_ctrlgroups[9].keyMappedTo = key_ctrlgroup9;
    int key_ctrlgroup9_Alt  = (int)FindToken(str,"key_ctrlgroup9_Alt");
        currSettings.keymap.key_ctrlgroups[9].secondKeyMappedTo = key_ctrlgroup9_Alt;

    int key_ctrlgroup0  = (int)FindToken(str,"key_ctrlgroup0");
        currSettings.keymap.key_ctrlgroups[0].keyMappedTo = key_ctrlgroup0;
    int key_ctrlgroup0_Alt  = (int)FindToken(str,"key_ctrlgroup0_Alt");
        currSettings.keymap.key_ctrlgroups[0].secondKeyMappedTo = key_ctrlgroup0_Alt;

}
void SetBinds(char* str)
{
    SetAbilityKeys(str);
    SetControllingAbilities(str);
    SetMovementKeys(str);
    SetBindControlGroups(str);
}
void Save(char* path)
{
    ALLEGRO_FILE* file = al_fopen(path,"w+");
    if (file)
    {
        //al_fwrite(file, "bankedGold: ", strlen("bankedGold: "));
        //al_fwrite(file, &players[0].bankedGold,sizeof(players[0].bankedGold));
        //al_fwrite(file, ";\n", strlen(";\n"));

        al_fprintf(file,"bankedGold: %i;\n",players[0].bankedGold);
        
        al_fwrite(file,"unlocked\n{\n",strlen("unlocked\n{\n"));
        for (int i = 0; i < numPrefabs; i++)
        {
            GameObject* prefab = prefabs[i];
            if (prefab->playerChoosable && prefab->purchased && prefab->path)
            {
                al_fwrite(file,"\t",1);
                al_fwrite(file,"\"",1);

                al_fwrite(file,prefab->path,strlen(prefab->path));

                al_fwrite(file,"\"",1);
                al_fwrite(file,";",1);
                al_fwrite(file,"\n",1);


            }
        }
        al_fwrite(file,"};\n",strlen("};\n"));
        
        
        al_fwrite(file,"unlockedEncounters\n{\n", strlen("unlockedEncounters\n{\n"));
        for (int i = 0; i < numEncounters; i++)
        {
            Encounter* e = encounters[i];
            if (e->path && e->unlocked)
            {
                al_fwrite(file,"\t",1);
                al_fwrite(file,"\"",1);
                al_fwrite(file,e->path,strlen(e->path));
                al_fwrite(file,"\"",1);
               
                al_fprintf(file," %i, %i, %i, %i",e->difficultyUnlocked,e->bestChest,e->bestProfited,e->totalProfit);
 
                al_fwrite(file,";",1);
                al_fwrite(file,"\n",1);
            }
        }
        al_fwrite(file,"};\n",strlen("};\n"));


        al_fclose(file);
    }
    else
        printf("Could not open save file to write! Path %s\n",path ? path : "NULL");

}
bool LoadSaveFile(char* path)
{
    if (path)
    {
        ALLEGRO_FILE* file = al_fopen(path,"rb");
        if (file)
        {
            int size = al_fsize(file);
            char* str  = calloc(al_fsize(file)+2,sizeof(char));
            al_fread(file,str,al_fsize(file));
            
            char* unlockStr = strstr(str,"unlocked");
            if (unlockStr)
            {
                    
                for (int i = unlockStr-str+strlen("unlocked"); i < size; i++)
                {
                    //if the next character is a { ignoring whitespace
                    if (!isspace(str[i]))
                    {
                        if (str[i] == '{')
                        {
                            int bounds = 0;
                            for (int j = i+1; j < size; j++)
                            {
                                bounds++;
                                if (str[j] == '}')
                                {
                                    break;
                                }
                            }

                            char* unlockedPathsStr = calloc(bounds+1,sizeof(char));
                            strncpy(unlockedPathsStr,str+i+1,bounds-1);
                            char* token = strtok(unlockedPathsStr,";");
                            while (token)
                            {
                                for (int j = 0; j < strlen(token); j++)
                                {
                                    if (token[j] == '"' || isspace(token[j]))
                                    {
                                        for (int z = j; z < strlen(token)-1; z++)
                                        {
                                            token[z] = token[z+1];
                                        }
                                        token[strlen(token)-1] = '\0';
                                        j--;    
                                    }
                                }

                                for (int j = 0; j < numPrefabs; j++)
                                {
                                    GameObject* prefab = prefabs[j];
                                    if (prefab->cost == 0 || (prefab->path && strcmp(token,prefab->path)==0))
                                    {
                                        prefab->purchased = true;
                                    }
                                }
                                token = strtok(NULL,";");
                            }
                            free(unlockedPathsStr);
                        }
                        else
                            break;
                    }
                }
            
            }
            char* unlockEnctrStr = strstr(str,"unlockedEncounters");

            if (unlockEnctrStr)
            {
            for (int i = unlockEnctrStr-str+strlen("unlockedEncounters"); i < size; i++)
            {
                //if the next character is a { ignoring whitespace
                if (!isspace(str[i]))
                {
                    if (str[i] == '{')
                    {
                        int bounds = 0;
                        for (int j = i+1; j < size; j++)
                        {
                            bounds++;
                            if (str[j] == '}')
                            {
                                break;
                            }
                        }

                        char* unlockedPathsStr = calloc(bounds+1,sizeof(char));
                        strncpy(unlockedPathsStr,str+i+1,bounds-1);
                        char* token = strtok(unlockedPathsStr,";");
                        while (token)
                        {
                            int positionEnd = strlen(token);
                            for (int j = 0; j < strlen(token); j++)
                            {
                                if (token[j] == '"')
                                    positionEnd = j;

                                if (token[j] == '"' || isspace(token[j]))
                                {
                                    for (int z = j; z < strlen(token)-1; z++)
                                    {
                                        token[z] = token[z+1];
                                    }
                                    token[strlen(token)-1] = '\0';
                                    j--;    
                                }
                            }
                            char* levelStats = &token[positionEnd];
                            if (strlen(levelStats) > 1)
                            {
                                int unlockLevel = 0;
                                int bestChest = 0;
                                int bestProfit = 0;
                                int totalProfit = 0;
                                char comma = ',';
                                char* save;
                                unlockLevel = atoi(strtok_r(levelStats,&comma,&save));
                                bestChest = atoi(strtok_r(NULL,&comma,&save));
                                bestProfit = atoi(strtok_r(NULL,&comma,&save));
                                totalProfit = atoi(strtok_r(NULL,&comma,&save));


                                token[positionEnd] = '\0';
                                UnlockEncounter(token,unlockLevel,bestChest,bestProfit,totalProfit,true);
                            }

                            token = strtok(NULL,";");
                        }
                        free(unlockedPathsStr);
                    }
                    else
                        break;
                }
            }
            }


            float gold = FindToken(str,"bankedGold");
            if (gold >= 0)
            {
                players[0].bankedGold = gold;
            }


            free(str);
            al_fclose(file);
        }
        else
        {
            players[0].bankedGold = 0;
            for (int j = 0; j < numPrefabs; j++)
            {
                GameObject* prefab = prefabs[j];
                if (prefab->cost == 0)
                {
                    prefab->purchased = true;
                }
            }
            Save("_save.save");
            return false;

        }

    }
    else
        return false;
    return true;
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
            float sfxVolume  = FindToken(str,"sfxVolume");
            if (sfxVolume >= 0)
            {
                currSettings.sfxVolume = sfxVolume;
            }

            float musicVolume  = FindToken(str,"musicVolume");
            if (musicVolume >= 0)
            {
                currSettings.musicVolume =  musicVolume;
            }
            float slowdownPercent  = FindToken(str,"slowdownPercent");
            if (slowdownPercent >= 0)
            {
                currSettings.slowdownPercent = slowdownPercent;
            }


            float particlesEnabled  = FindToken(str,"particles_enabled");
            if (particlesEnabled >= 0)
            {
                currSettings.particlesEnabled = (int)particlesEnabled;
            }
            float lightEffect  = FindToken(str,"light_effect_enabled");
            if (lightEffect >= 0)
            {
                currSettings.lightEffectEnabled = (int)lightEffect;
            }

            
            float displayHealthBar  = FindToken(str,"display_health_bar");
            if (displayHealthBar >= 0)
            {
                currSettings.displayHealthBar =  (Option_HealthBar)displayHealthBar;
            }
            float targetFPS  = FindToken(str,"targetFPS");
            if (targetFPS >= 0)
            {
                currSettings.targetFPS =  (Option_FPS)targetFPS;
            }


            float displayTimer  = FindToken(str,"display_timer");
            if (displayTimer >= 0)
            {
                if (displayTimer <= 0.001f)
                    currSettings.displayTimer = false;
                else
                    currSettings.displayTimer = true;
            }

            float screenShakeFactor  = FindToken(str,"screen_shake_factor");
            if (screenShakeFactor >= 0)
            {
                currSettings.screenShakeFactor = screenShakeFactor;
            }


        
            SetBinds(str);


            al_fclose(file);
            free(str);
            return true;
        }
    }
    return false;
}   