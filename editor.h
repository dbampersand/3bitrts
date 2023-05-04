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
typedef enum EDITOR_HANDLE
{
    HANDLE_NONE = 0,
    HANDLE_TOP = 1,
    HANDLE_RIGHT = 2,
    HANDLE_BOTTOM = 3,
    HANDLE_LEFT = 4,
}EDITOR_HANDLE;
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
    EDITOR_FILE_NEXT_MAP = 2,
    EDITOR_FILE_MAP_SPRITE = 3
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
    Panel fileNamingUI;
    Panel unitSelector;

    Panel unitOptions;
    Panel mapImageEditor;

    bool showFileSelector;
    bool showFileNamingUI;

    EDITOR_FILE_SELECT selectorPicked;

    UIElement save;

    bool heldObjectIsDecor;
    bool goingShop;

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

    EDITOR_HANDLE heldHandle;
    float handleOffset;

    Point heldObjectOffset;
    GameObject* highlightedObject;
    
    EditorUI editorUI;
    EDITOR_STATE editorState;
    PAINTING_MODE paintingMode;

    float paintSize;

    bool spawnPointHeld;
    Point spawnPointHeldOffset;



} Editor;   

extern Editor editor;

void EditorSetMap(char* path);
void DrawEditorUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame);
void UpdateEditor(float dt,MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void EditorSetMap(char* path);
void InitEditorUI();
void InitFileSelector();
void PopulateUnitSelector(Panel* p);
void UpdatePosition(GameObject* g, float x, float y);
EDITOR_HANDLE CheckMapHandleClicked(MouseState mouse);
Rect GetMapHandleRect(EDITOR_HANDLE handle);
bool HandleIsVertical(EDITOR_HANDLE handle);
bool HandleIsHorizontal(EDITOR_HANDLE handle);
void UpdateArgumentStr(char** full, char* position, char* str, bool addQuotes);
char* GetPositionOfArgument(char* str, char* functionToFind, int argumentToGet);
EditorLine* AddEditorLine(EditorLine** lines, int* numLines, char* str, bool end);
void SpawnPointRectIsMoved(MouseState mouseState, MouseState mouseStateLastFrame);
void UpdateSpawnPointStr();