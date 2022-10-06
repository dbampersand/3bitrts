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
    KEY secondKeyMappedTo;

} Key;


typedef struct Keymap
{
    Key key_Q;
    Key key_W;
    Key key_E;
    Key key_R;
    Key key_F;

    Key key_AMove;
    Key key_Shift;
    Key key_Tab;
    Key key_Ctrl;
    Key key_ShowMap;
    Key key_Pause;
    Key key_Cancel;
    Key key_HoldPosition;

    Key key_PanLeft;
    Key key_PanRight;
    Key key_PanUp;
    Key key_PanDown;


    Key key_ctrlgroups[10];




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


typedef struct UIElement UIElement;

void InitSettings(char* path);
bool IsModifierDown(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame);
void SetDefaultSettings(Settings* setting);
bool LoadSettingsFile(char* path);
float FindToken(char* str, char* token);
void WriteSettingsFile(char* path);
void EndSettings();
int GetMappedKey(int key);
void UpdateBind(UIElement* u);

bool IsBindReleasedThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k);
bool IsBindDownThisFrame(ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame,Key k);
bool IsBindDown(ALLEGRO_KEYBOARD_STATE* keyState,Key k);
void InitPath();