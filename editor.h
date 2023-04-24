#pragma once

#include "ui.h"
#include "point.h"

typedef struct Map Map;
typedef struct GameObject GameObject;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;

#define PAINT_SIZE_MIN 1
#define PAINT_SIZE_MAX 40
typedef enum EDITOR_STATE
{
    EDITOR_STATE_NORMAL = 0,
    EDITOR_STATE_HOLDING_UNIT = 1,
    EDITOR_STATE_PAINTING = 2
}EDITOR_STATE;
typedef enum PAINTING_MODE
{
    PAINTING_MODE_NONE,
    PAINTING_MODE_DRAW_FIRST_LAYER,
    PAINTING_MODE_SECOND_LAYER,
    PAINTING_MODE_ERASING
}PAINTING_MODE;
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
    Panel mapImageEditor;

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
    Point heldObjectOffset;
    GameObject* highlightedObject;
    
    EditorUI editorUI;
    EDITOR_STATE editorState;
    PAINTING_MODE paintingMode;

    float paintSize;


} Editor;   

extern Editor editor;

void EditorSetMap(char* path);
void DrawEditorUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void UpdateEditor(float dt,MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void EditorSetMap(char* path);
void InitEditorUI();
void InitFileSelector();
void PopulateUnitSelector(Panel* p);
