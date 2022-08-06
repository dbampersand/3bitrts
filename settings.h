#pragma once
#include "gamestate.h"

char* settingFilePath;

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

    //

} Settings;

Settings currSettings;
Settings defaultSettings;

void InitSettings(char* path);
void SetDefaultSettings(Settings* setting);
bool LoadSettingsFile(char* path);
float FindToken(char* str, char* token);
void WriteSettingsFile(char* path);
void EndSettings();
