#pragma once

#include "ui.h"


typedef struct Map Map;
typedef struct GameObject GameObject;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;

typedef enum EDITOR_FILE_SELECT
{
    EDITOR_FILE_CURRENT_MAP = 1,
    EDITOR_FILE_NEXT_MAP = 2
} EDITOR_FILE_SELECT;
typedef struct EditorLine
{
    char* line;
    int lineNumber;
    GameObject* associated;
    bool lineIsComment;
} EditorLine;

typedef struct EditorUI
{
    Panel saveLoad;

    Panel fileSelector;
    Panel unitSelector;

    Panel unitOptions;

    bool showFileSelector;
    EDITOR_FILE_SELECT selectorPicked;

    UIElement save;

    bool heldObjectIsDecor;

} EditorUI;



typedef struct Editor
{
    char* currentPath;
    char* fileName;

    int numSetupLines;
    EditorLine* setupLines;
    int numEndLines;
    EditorLine* endLines;  

    char* nextMap;

    Map* map;

    GameObject* heldObject;
    GameObject* highlightedObject;
    
    EditorUI editorUI;



} Editor;   

extern Editor editor;

void EditorSetMap(char* path);
void DrawEditorUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void UpdateEditor(float dt,MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState);
void EditorSetMap(char* path);
void InitEditorUI();
void InitFileSelector();
void PopulateUnitSelector(Panel* p);
