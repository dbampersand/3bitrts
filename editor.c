#include "editor.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "gameobject.h"
#include "luafuncs.h"
#include "helperfuncs.h"
#include "player.h"
#include <dirent.h>

Editor editor = {0};


//extract function setup so we can run that one line at a time and create Line objects from that
char* ExtractFunction(char* funcName, char* buffer)
{
    char* c = buffer;
    char* full = c + strlen(funcName);

    char* start;

    if (c == NULL) 
        return false;
    while (c < buffer + strlen(buffer))
    {
        int dfsf = strcmp(c,funcName);
        if (strncmp(c,funcName,strlen(funcName)) == 0)
        {
            char* c2 = c;
            start = c2;
            while (*(start++) != ')')
            {

            }
            bool foundFunc = false;
            bool isComment = false;
            while (c2-- > buffer)
            {
                if (strncmp(c2,"function ",strlen("function ")) == 0)
                {
                    foundFunc = true;
                }
                if (*c2 == '\n')   
                    break;
                if (strcmp(c2,"--") == 0)
                    isComment = true;

            }
            if (foundFunc && !isComment)
                break;
        }
        c++;
    }
    char* end = c + strlen(c);



    for (char* c2 = start; c2 < buffer + strlen(buffer); c2++)
    {
        int dfsf = strncmp(c2+1,"function ",strlen("function "));

        if (isspace(*c2) && strncmp(c2+1,"function ",strlen("function ")) == 0)
        {
            end = c2;
            break;
        }
    }
    //remove the 'end' at the ending part of functions
    while (*(end--) != 'd')
    {

    } end--;
    int numChars = end - start;
    
    char* new = calloc((end-start)+1,sizeof(char));
    memcpy(new,start,(end-start)*sizeof(char));

    return new;
}
char* GetPositionOfArgument(char* str, char* functionToFind, int argumentToGet)
{
    char* func = strstr(str,functionToFind);
    if (!func)
        return NULL;
    func += strlen(functionToFind);

    char* position = func;

    //move past initial ( eg CreateObject ( fjfs, fdsgs, )
    for (int i = 0; i < strlen(position); i++)
    {
        if (position[i] == '(')
        {
            position += i + 1;
            break;
        }
    }



    for (int i = 0; i < argumentToGet-1; i++)
    {
        
        //if we have an example like (x,y)
        int numOpenBrackets = 0;
        int hasOpenBracket = false;
        for (int j = 0; j < strlen(position); j++)
        {
            
            if (position[j] == '(')
            {
                hasOpenBracket = true;
                numOpenBrackets++;
            }
            if (position[j] == ')')
            {
                numOpenBrackets--;
            } 
            if (hasOpenBracket && position[j] == ',' && numOpenBrackets == 0)
            {
                position += j + 1;
                break;
            }
            if (position[j] == ',' && !hasOpenBracket)
                break;
        }

        if (strstr(position,","))
            position = strstr(position,",")+1;
    }   
       
    if (argumentToGet == 1)
    {
        position = strstr(position,"(")+1;
    }
    return position;
}
void UpdateArgumentFloat(char** full, char* position, float f)
{
    int numToMove = 0;
    for (int i = 0; i < strlen(position); i++)
    {
        if (position[i] == ',' || position[i] == ')')
            break;
        numToMove++;
    }
    for (int i = 0; i < strlen(position)-numToMove; i++)
    {
        position[i] = position[i+numToMove];
    }
    float start = strlen(position); 
    float end = strlen(position) - numToMove;
    memset(position + strlen(position) - numToMove, 0, sizeof(char) * numToMove);


    char* digits = calloc(NumDigits(f) + 4, sizeof(char));
    sprintf(digits,"%.2f",f);


    char* before = calloc((strlen(*full) - strlen(position)) + 1,sizeof(char));
    char* after = calloc(strlen(position)+ 1,sizeof(char));

    memcpy(before,*full, (strlen(*full) - strlen(position)) * sizeof(char));
    strcpy(after,position);


    char* new = calloc(strlen(before)+strlen(after) + strlen(digits) + 1, sizeof(char));
    strcat(new,before);
    strcat(new,digits);
    strcat(new,after);

    *full = realloc(*full,(strlen(new)+1)*sizeof(char));
    strcpy(*full,new);
    

    free(new);
    free(before);
    free(after);
    free(digits);
} 
void UpdatePosition(GameObject* g, float x, float y)
{
    if (!g) return;
    for (int i = 0; i < editor.numSetupLines; i++)
    {
        if (editor.setupLines[i].associated == g)
        {
            g->position.worldX = x;
            g->position.worldY = y;
            UpdateScreenPositions(g);

            //2-decimal precision for both
            int toIncreaseBy = NumDigits(x) + NumDigits(y) + 6;
            //editor.setupLines[i].line = realloc(editor.setupLines[i].line,(strlen(editor.setupLines[i].line)+toIncreaseBy+1)*sizeof(char));

            char* xStr = GetPositionOfArgument(editor.setupLines[i].line,"CreateObject",2);
            UpdateArgumentFloat(&editor.setupLines[i].line,xStr,x);
            char* yStr = GetPositionOfArgument(editor.setupLines[i].line,"CreateObject",3);
            UpdateArgumentFloat(&editor.setupLines[i].line,yStr,y);

            printf("%s\n",editor.setupLines[i].line);
        }
    }
}
void RunAllLines()
{
    for (int i = 0; i < editor.numSetupLines; i++)
    {
        int numObjsBefore = numActiveObjects;

        char* line = editor.setupLines[i].line;
        if (editor.setupLines[i].line)
            luaL_dostring(luaState,editor.setupLines[i].line);
        if (numActiveObjects > numObjsBefore)
        {
            editor.setupLines[i].associated = &objects[numActiveObjects-1];
        }
    }
    if (pathToNextMap)
        free(pathToNextMap);
    pathToNextMap = NULL;
    
    for (int i = 0; i < editor.numEndLines; i++)
    {
        if (editor.endLines[i].line)
            luaL_dostring(luaState,editor.endLines[i].line);
    }

    if (pathToNextMap)
        ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"NextMap"),pathToNextMap);
    else
        ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"NextMap"),"(None)");
        

}
//TODO: support multiline arguments eg;
// CreateObject(
//              3,
//              5)
void SplitLines(char* buffer, EditorLine** lines, int* lineCount)
{
    char* save = buffer;

    char* bufferLine = strtok_r(buffer,"\n",&save);
    int numLines = 0;
    do 
    {
        numLines++;

        if (!*lines)
            *lines = calloc(1,sizeof(EditorLine));


        *lines = realloc(*lines,numLines * sizeof(EditorLine));

        EditorLine* line = *lines;

        memset(&line[numLines-1],0,sizeof(EditorLine));

        line[numLines-1].line = calloc(strlen(bufferLine)+1,sizeof(char));
        strcpy(line[numLines-1].line,bufferLine);
        
        line[numLines-1].lineNumber = numLines-1;   
        bufferLine = strtok_r(NULL,"\n",&save);   
    } while (bufferLine != NULL);

    *lineCount = numLines;
}

void EditorSetMap(char* path)
{
    if (!path) 
        return;
    RemoveAllGameObjects();
    Map* m = LoadMap(path);
    //SetMap(m);
    currMap = m;

    char* buff = ExtractFunction("setup",m->lua_buffer.buffer);
    char* end = ExtractFunction("mapend",m->lua_buffer.buffer);

    SplitLines(buff,&editor.setupLines,&editor.numSetupLines);
    SplitLines(end,&editor.endLines,&editor.numEndLines);


    for (int i = 0; i < editor.numEndLines; i++)
    {
        printf("%s\n",editor.endLines[i].line);
    }
    printf("%i\n",editor.numSetupLines);

    free(buff);
    free(end);


    RunAllLines();

    for (int i = 0; i < editor.numSetupLines; i++)
    {
        if (editor.setupLines[i].associated)
        {  
            //UpdatePosition(editor.setupLines[i].associated,4,5);
        }
    }
    ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"Location"),path);
    //ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"NextMap"),);

    if (editor.currentPath)
        free(editor.currentPath);
    editor.currentPath = calloc(strlen(path)+1,sizeof(char));

    //get the folder we're in
    bool normalChar = false;
    for (int i = strlen(path)-1; i >= 0; i--)
    {
        if (isalnum(path[i]))
        {
            normalChar = true;
        }

        if (normalChar && (path[i] == '/' || path[i] == '\\'))
        {
            path[i+1] = '\0';
            path[i] = '/';
            break;
        }
    }

    SetUITextStr(GetUIText(&editor.editorUI.fileSelector,"Path"),path);
    strcpy(editor.currentPath,path);
}   

void PopulateFileList(Panel* p, char* path, int numPostfixes, ...)
{
    va_list argp;
    va_start(argp, numPostfixes);
    char** postfixes = calloc(numPostfixes,sizeof(char*));
    for (int i = 0; i < numPostfixes; i++)
    {
        char* str = va_arg(argp, char*);

        postfixes[i] = calloc(strlen(str)+1,sizeof(char));
        strcpy(postfixes[i],str);
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    InitFileSelector(p);
    int x = 40; int y = 40; int w = _SCREEN_SIZE-70; int h = 20;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".") != 0  && strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                AddButton(p,"Path",dir->d_name,x,y,w,h,true);
                y += h;
            }

        }
    }

    for (int i = 0; i < numPostfixes; i++)
    {
        free(postfixes[i]);
    }
    free(postfixes);



}
void UpdateEditor(float dt,MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState)
{
    if (MouseClickedThisFrame(&mouseState, &mouseStateLastFrame))
    {
        editor.heldObject = GetClicked(mouseState.worldX,mouseState.worldY);
    }

    if (editor.heldObject)
    {
        UpdatePosition(editor.heldObject,mouseState.worldX,mouseState.worldY);
    }

    if (!(mouseState.mouse.buttons & 1))
        editor.heldObject = NULL;

    UpdatePanel(&editor.editorUI.saveLoad,&mouseState,&mouseStateLastFrame,keyState);
    UpdatePanel(&editor.editorUI.fileSelector,&mouseState,&mouseStateLastFrame,keyState);

    if (GetButton(&editor.editorUI.saveLoad,"Location"))
    {
        editor.editorUI.showFileSelector = true;
        editor.editorUI.selectorPicked = EDITOR_FILE_CURRENT_MAP;

        PopulateFileList(&editor.editorUI.fileSelector,editor.currentPath,1,".lua");

    }
    if (GetButton(&editor.editorUI.saveLoad,"NextMap"))
    {
        editor.editorUI.showFileSelector = true;
        editor.editorUI.selectorPicked = EDITOR_FILE_NEXT_MAP;

        PopulateFileList(&editor.editorUI.fileSelector,editor.currentPath,1,".lua");

    }
    if (GetButton(&editor.editorUI.fileSelector,"Back"))
    {
        editor.editorUI.showFileSelector = false;
    }

}
void DrawEditorUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame)
{
    DrawPanel(&editor.editorUI.saveLoad, &mouseState, 1);

    if (editor.editorUI.showFileSelector)
    {
        DrawPanel(&editor.editorUI.fileSelector,&mouseState,1);
    }
}
void InitFileSelector(Panel* p)
{
    ClearPanelElements(p);
    AddButton(p,"Back","Back",0,0,35,10,true);
    AddText(p,40,0,"Path","Path");

}
void InitEditorUI()
{   
    editor.editorUI.saveLoad = CreatePanel(1,UI_START_Y,_SCREEN_SIZE-1,_SCREEN_SIZE-UI_START_Y,1,true);
    UIElement* loc = AddButton(&editor.editorUI.saveLoad,"Location","Location",1,1,_SCREEN_SIZE-2,13,true);
    AddButton(&editor.editorUI.saveLoad,"NextMap","NextMap",1,1+loc->h+2,_SCREEN_SIZE-2,13,true);

    editor.editorUI.fileSelector = CreatePanel(10,10,_SCREEN_SIZE-20,UI_START_Y-20,1,true);
    InitFileSelector(&editor.editorUI.fileSelector);
}   