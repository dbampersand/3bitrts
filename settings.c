#include "settings.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "sprite.h"

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
    setting->lightEffectEnabled = true;

    setting->displayHealthBar = OPTION_HPBAR_ALWAYS;
    setting->masterVolume = 1.0f;
    setting->musicVolume = 1.0f;
    setting->sfxVolume = 1.0f;


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

    setting->keymap.key_ShowMap = (Key){"Show Map",ALLEGRO_KEY_BACKQUOTE};

    setting->keymap.key_Pause = (Key){"Pause",ALLEGRO_KEY_P};
    setting->keymap.key_Cancel = (Key){"Pause",ALLEGRO_KEY_ESCAPE};

    setting->keymap.key_PanLeft = (Key){"Pause",ALLEGRO_KEY_LEFT};
    setting->keymap.key_PanRight = (Key){"Pause",ALLEGRO_KEY_RIGHT};
    setting->keymap.key_PanUp = (Key){"Pause",ALLEGRO_KEY_UP};
    setting->keymap.key_PanDown = (Key){"Pause",ALLEGRO_KEY_DOWN};

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
        "display_health_bar %i;\n"
        "volume %f;\n"
        "sfxVolume: %f;\n"
        "musicVolume: %f;\n"
        "display_timer %i;\n"
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

        currSettings.displayHealthBar,

        currSettings.masterVolume,
        currSettings.sfxVolume,
        currSettings.musicVolume,

        currSettings.displayTimer ? 1 : 0,
        
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
    if (key_Q >= 0)
        currSettings.keymap.key_Q.keyMappedTo = key_Q;

    int key_Q_Alt  = (int)FindToken(str,"key_Q_Alt");
    if (key_Q_Alt >= 0)
        currSettings.keymap.key_Q.secondKeyMappedTo = key_Q_Alt;

    int key_W = (int)FindToken(str,"key_W");
    if (key_W >= 0)
        currSettings.keymap.key_W.keyMappedTo = key_W;

    int key_W_Alt  = (int)FindToken(str,"key_W_Alt");
    if (key_W_Alt >= 0)
        currSettings.keymap.key_W.secondKeyMappedTo = key_W_Alt;

    int key_E = (int)FindToken(str,"key_E");
    if (key_E >= 0)
        currSettings.keymap.key_E.keyMappedTo = key_E;

    int key_E_Alt  = (int)FindToken(str,"key_E_Alt");
    if (key_E_Alt >= 0)
        currSettings.keymap.key_E.secondKeyMappedTo = key_E_Alt;

    int key_R = (int)FindToken(str,"key_R");
    if (key_R >= 0)
        currSettings.keymap.key_R.keyMappedTo = key_R;

    int key_R_Alt  = (int)FindToken(str,"key_R_Alt");
    if (key_R_Alt >= 0)
        currSettings.keymap.key_R.secondKeyMappedTo = key_R_Alt;

    int key_F = (int)FindToken(str,"key_F");
    if (key_F >= 0)
        currSettings.keymap.key_F.keyMappedTo = key_F;

    int key_F_Alt  = (int)FindToken(str,"key_F_Alt");
    if (key_F_Alt >= 0)
        currSettings.keymap.key_F.secondKeyMappedTo = key_F_Alt;


}
void SetControllingAbilities(char* str)
{
    int key_AMove  = (int)FindToken(str,"key_AMove");
    if (key_AMove >= 0)
        currSettings.keymap.key_AMove.keyMappedTo = key_AMove;

    int key_AMove_Alt  = (int)FindToken(str,"key_AMove_Alt");
    if (key_AMove_Alt >= 0)
        currSettings.keymap.key_AMove.secondKeyMappedTo = key_AMove_Alt;

    int key_Shift  = (int)FindToken(str,"key_Shift");
    if (key_Shift >= 0)
        currSettings.keymap.key_Shift.keyMappedTo = key_Shift;

    int key_Shift_Alt  = (int)FindToken(str,"key_Shift_Alt");
    if (key_Shift_Alt >= 0)
        currSettings.keymap.key_Shift.secondKeyMappedTo = key_Shift_Alt;

    int key_Tab  = (int)FindToken(str,"key_Tab");
    if (key_Tab >= 0)
        currSettings.keymap.key_Tab.keyMappedTo = key_Tab;

    int key_Tab_Alt  = (int)FindToken(str,"key_Tab_Alt");
    if (key_Tab_Alt >= 0)
        currSettings.keymap.key_Tab.secondKeyMappedTo = key_Tab_Alt;

    int key_Ctrl  = (int)FindToken(str,"key_Ctrl");
    if (key_Ctrl >= 0)
        currSettings.keymap.key_Ctrl.keyMappedTo = key_Ctrl;

    int key_Ctrl_Alt  = (int)FindToken(str,"key_Ctrl_Alt");
    if (key_Ctrl_Alt >= 0)
        currSettings.keymap.key_Ctrl.secondKeyMappedTo = key_Ctrl_Alt;

    int key_ShowMap  = (int)FindToken(str,"key_ShowMap");
    if (key_ShowMap >= 0)
        currSettings.keymap.key_ShowMap.keyMappedTo = key_ShowMap;

    int key_ShowMap_Alt  = (int)FindToken(str,"key_ShowMap_Alt");
    if (key_ShowMap_Alt >= 0)
        currSettings.keymap.key_ShowMap.secondKeyMappedTo = key_ShowMap_Alt;

    int key_Pause  = (int)FindToken(str,"key_Pause");
    if (key_Pause >= 0)
        currSettings.keymap.key_Pause.keyMappedTo = key_Pause;

    int key_Pause_Alt  = (int)FindToken(str,"key_Pause_Alt");
    if (key_Pause_Alt >= 0)
        currSettings.keymap.key_Pause.secondKeyMappedTo = key_Pause_Alt;

    int key_Cancel  = (int)FindToken(str,"key_Cancel");
    if (key_Cancel >= 0)
        currSettings.keymap.key_Cancel.keyMappedTo = key_Cancel;

    int key_Cancel_Alt  = (int)FindToken(str,"key_Cancel_Alt");
    if (key_Cancel_Alt >= 0)
        currSettings.keymap.key_Cancel.secondKeyMappedTo = key_Cancel_Alt;

}
void SetMovementKeys(char* str)
{
    int key_PanLeft  = (int)FindToken(str,"key_PanLeft");
    if (key_PanLeft >= 0)
        currSettings.keymap.key_PanLeft.keyMappedTo = key_PanLeft;

    int key_PanLeft_Alt  = (int)FindToken(str,"key_PanLeft_Alt");
    if (key_PanLeft_Alt >= 0)
        currSettings.keymap.key_PanLeft.secondKeyMappedTo = key_PanLeft_Alt;

    int key_PanRight  = (int)FindToken(str,"key_PanRight");
    if (key_PanRight >= 0)
        currSettings.keymap.key_PanRight.keyMappedTo = key_PanRight;

    int key_PanRight_Alt  = (int)FindToken(str,"key_PanRight_Alt");
    if (key_PanRight_Alt >= 0)
        currSettings.keymap.key_PanRight.secondKeyMappedTo = key_PanRight_Alt;

    int key_PanUp  = (int)FindToken(str,"key_PanUp");
    if (key_PanUp >= 0)
        currSettings.keymap.key_PanUp.keyMappedTo = key_PanUp;

    int key_PanUp_Alt  = (int)FindToken(str,"key_PanUp_Alt");
    if (key_PanUp_Alt >= 0)
        currSettings.keymap.key_PanUp.secondKeyMappedTo = key_PanUp_Alt;

    int key_PanDown  = (int)FindToken(str,"key_PanDown");
    if (key_PanDown >= 0)
        currSettings.keymap.key_PanDown.keyMappedTo = key_PanDown;

    int key_PanDown_Alt  = (int)FindToken(str,"key_PanDown_Alt");
    if (key_PanDown_Alt >= 0)
        currSettings.keymap.key_PanDown.secondKeyMappedTo = key_PanDown_Alt;


}
void SetBindControlGroups(char* str)
{
    int key_ctrlgroup1  = (int)FindToken(str,"key_ctrlgroup1");
    if (key_ctrlgroup1 >= 0)
        currSettings.keymap.key_ctrlgroups[1].keyMappedTo = key_ctrlgroup1;
    int key_ctrlgroup1_Alt  = (int)FindToken(str,"key_ctrlgroup1_Alt");
    if (key_ctrlgroup1_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[1].secondKeyMappedTo = key_ctrlgroup1_Alt;


    int key_ctrlgroup2  = (int)FindToken(str,"key_ctrlgroup2");
    if (key_ctrlgroup2 >= 0)
        currSettings.keymap.key_ctrlgroups[2].keyMappedTo = key_ctrlgroup2;
    int key_ctrlgroup2_Alt  = (int)FindToken(str,"key_ctrlgroup2_Alt");
    if (key_ctrlgroup2_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[2].secondKeyMappedTo = key_ctrlgroup2_Alt;

    int key_ctrlgroup3  = (int)FindToken(str,"key_ctrlgroup3");
    if (key_ctrlgroup3 >= 0)
        currSettings.keymap.key_ctrlgroups[3].keyMappedTo = key_ctrlgroup3;
    int key_ctrlgroup3_Alt  = (int)FindToken(str,"key_ctrlgroup3_Alt");
    if (key_ctrlgroup3_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[3].secondKeyMappedTo = key_ctrlgroup3_Alt;

    int key_ctrlgroup4  = (int)FindToken(str,"key_ctrlgroup4");
    if (key_ctrlgroup4 >= 0)
        currSettings.keymap.key_ctrlgroups[4].keyMappedTo = key_ctrlgroup4;
    int key_ctrlgroup4_Alt  = (int)FindToken(str,"key_ctrlgroup4_Alt");
    if (key_ctrlgroup4_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[4].secondKeyMappedTo = key_ctrlgroup4_Alt;

    int key_ctrlgroup5  = (int)FindToken(str,"key_ctrlgroup5");
    if (key_ctrlgroup5 >= 0)
        currSettings.keymap.key_ctrlgroups[5].keyMappedTo = key_ctrlgroup5;
    int key_ctrlgroup5_Alt  = (int)FindToken(str,"key_ctrlgroup5_Alt");
    if (key_ctrlgroup5_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[5].secondKeyMappedTo = key_ctrlgroup5_Alt;


    int key_ctrlgroup6  = (int)FindToken(str,"key_ctrlgroup6");
    if (key_ctrlgroup6 >= 0)
        currSettings.keymap.key_ctrlgroups[6].keyMappedTo = key_ctrlgroup6;
    int key_ctrlgroup6_Alt  = (int)FindToken(str,"key_ctrlgroup6_Alt");
    if (key_ctrlgroup6_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[6].secondKeyMappedTo = key_ctrlgroup6_Alt;

    int key_ctrlgroup7  = (int)FindToken(str,"key_ctrlgroup7");
    if (key_ctrlgroup7 >= 0)
        currSettings.keymap.key_ctrlgroups[7].keyMappedTo = key_ctrlgroup7;
    int key_ctrlgroup7_Alt  = (int)FindToken(str,"key_ctrlgroup7_Alt");
    if (key_ctrlgroup7_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[7].secondKeyMappedTo = key_ctrlgroup7_Alt;

    int key_ctrlgroup8  = (int)FindToken(str,"key_ctrlgroup8");
    if (key_ctrlgroup8 >= 0)
        currSettings.keymap.key_ctrlgroups[8].keyMappedTo = key_ctrlgroup8;
    int key_ctrlgroup8_Alt  = (int)FindToken(str,"key_ctrlgroup8_Alt");
    if (key_ctrlgroup8_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[8].secondKeyMappedTo = key_ctrlgroup8_Alt;

    int key_ctrlgroup9  = (int)FindToken(str,"key_ctrlgroup9");
    if (key_ctrlgroup9 >= 0)
        currSettings.keymap.key_ctrlgroups[9].keyMappedTo = key_ctrlgroup9;
    int key_ctrlgroup9_Alt  = (int)FindToken(str,"key_ctrlgroup9_Alt");
    if (key_ctrlgroup9_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[9].secondKeyMappedTo = key_ctrlgroup9_Alt;

    int key_ctrlgroup0  = (int)FindToken(str,"key_ctrlgroup0");
    if (key_ctrlgroup0 >= 0)
        currSettings.keymap.key_ctrlgroups[0].keyMappedTo = key_ctrlgroup0;
    int key_ctrlgroup0_Alt  = (int)FindToken(str,"key_ctrlgroup0_Alt");
    if (key_ctrlgroup0_Alt >= 0)
        currSettings.keymap.key_ctrlgroups[0].secondKeyMappedTo = key_ctrlgroup0_Alt;

}
void SetBinds(char* str)
{
    SetAbilityKeys(str);
    SetControllingAbilities(str);
    SetMovementKeys(str);
    SetBindControlGroups(str);
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

            float displayTimer  = FindToken(str,"display_timer");
            if (displayTimer >= 0)
            {
                if (displayTimer <= 0.001f)
                    currSettings.displayTimer = false;
                else
                    currSettings.displayTimer = true;
            }

            SetBinds(str);


            al_fclose(file);
            free(str);
            return true;
        }
    }
    return false;
}   