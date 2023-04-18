#pragma once

typedef struct Map Map;
typedef struct GameObject GameObject;

typedef struct EditorLine
{
    char* line;
    int lineNumber;
    GameObject* associated;
} EditorLine;

typedef struct Editor
{
    char* currentPath;
    char* fileName;
    int numLines;
    EditorLine* lines; 
    Map* map;
} Editor;   

extern Editor editor;

void EditorSetMap(char* path);
