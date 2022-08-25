#pragma once
#include "gamestate.h"
#include "stdbool.h"

char* settingFilePath;

typedef int KEY;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE; 

typedef struct Key
{
    char* name;
    KEY keyMappedTo;

} Key;


typedef struct Keymap
{
    Key key_ctrlgroups[10];
    Key key_Q;
    Key key_W;
    Key key_E;
    Key key_R;
    Key key_AMove;
    Key key_Shift;
    Key key_Tab;
    Key key_Ctrl;
    Key key_ShowMap;
    Key key_Pause;

    Key key_PanLeft;
    Key key_PanRight;
    Key key_PanUp;
    Key key_PanDown;



} Keymap;

typedef struct Settings
{
    //VIDEO OPTIONS
    unsigned int* renderScale;
    bool particlesEnabled;
    Option_HealthBar displayHealthBar;
    bool displayTimer;
    
    //AUDIO OPTIONS
    float masterVolume;

    //ACCESSIBILITY OPTIONS
    Keymap keymap;
    //

} Settings;
Settings currSettings;
Settings defaultSettings;

void InitSettings(char* path);
bool IsModifierDown(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame);
void SetDefaultSettings(Settings* setting);
bool LoadSettingsFile(char* path);
float FindToken(char* str, char* token);
void WriteSettingsFile(char* path);
void EndSettings();
int GetMappedKey(int key);

bool IsBindReleasedThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k);
bool IsBindDownThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k);
bool IsBindDown(ALLEGRO_KEYBOARD_STATE* keyState,Key k);
