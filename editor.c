
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
#include "encounter.h"
#include "allegro5/allegro_primitives.h"

Editor editor = {0};
bool FileExists(char* path)
{   
    ALLEGRO_FILE* f = al_fopen(path,"r");
    al_fclose(f);
    return (f != NULL);
}

void EditorSetMapSprite(char* newPath)
{
    currMap->spriteIndex = LoadSprite(newPath,true);
    PreprocessMap(currMap,0);

    bool foundSetMapSprite = false;

    for (int i = 0; i < editor.numSetupLines; i++)
    {
        if (!editor.setupLines[i].lineIsComment &&  strstr(editor.setupLines[i].line,"SetMapSprite"))
        {
            char* start = GetPositionOfArgument(editor.setupLines[i].line,"SetMapSprite",1);

            UpdateArgumentStr(&editor.setupLines[i].line,start,newPath,true);
            foundSetMapSprite = true;
            break;
        }
    }   
    if (!foundSetMapSprite)
    {
        char* fmt = "    SetMapSprite(\"%s\")\n";
        int numChars = snprintf(NULL,0,fmt,newPath);
        char* fullStr = calloc(numChars+1,sizeof(char));
        sprintf(fullStr,fmt,newPath);
        AddEditorLine(&editor.setupLines,&editor.numSetupLines, fullStr,true);
        free(fullStr);
    }


}
void DecreaseMapDimensions(EDITOR_HANDLE handle, int x, int y)
{
    ALLEGRO_BITMAP* old = sprites[currMap->spriteIndex].sprite;
    int w = al_get_bitmap_width(old) - abs(x); 
    int h = al_get_bitmap_height(old) - abs(y); 

    ALLEGRO_BITMAP* before = al_get_target_bitmap();
    ALLEGRO_BITMAP* new = al_create_bitmap(w,h);


    al_set_target_bitmap(new);
    al_clear_to_color(al_map_rgba(0,0,0,0));

    int drawOffsetX = 0;
    int drawOffsetY = 0;
    int sx = 0;
    int sy = 0;

    if (x > 0)
    {
        //drawOffsetX = abs(x);   
    }
    if (handle == HANDLE_TOP || handle == HANDLE_LEFT)
    {
        sx = x;
        sy = y;
    }
    currMap->spawnPoint.x -= sx;
    currMap->spawnPoint.y -= sy;
    UpdateSpawnPointStr();

    al_draw_tinted_bitmap_region(sprites[currMap->spriteIndex].sprite,WHITE,sx,sy,w,h,drawOffsetX,drawOffsetY,0);
    al_draw_tinted_bitmap_region(sprites[currMap->secondLayerSpriteIndex].sprite,WHITE,sx,sy,w,h,drawOffsetX,drawOffsetY,0);

    sprites[currMap->spriteIndex].sprite = new;

    al_destroy_bitmap(old);
    al_set_target_bitmap(before);

    PreprocessMap(currMap,0);

    MoveCam(players[0].cameraPos.x-sx,players[0].cameraPos.y);
    MoveCam(players[0].cameraPos.x,players[0].cameraPos.y-sy);

    if (sx != 0 || sy != 0)
    {
        for (int i = 0; i < editor.numSetupLines; i++)
        {
            if (editor.setupLines[i].associated)
            {
                float xNew = editor.setupLines[i].associated->position.worldX - sx;
                float yNew = editor.setupLines[i].associated->position.worldY - sy;

                UpdatePosition(editor.setupLines[i].associated,xNew,yNew);
            }
        }   

    }
}
void IncreaseMapDimensions(int x, int y)
{
    ALLEGRO_BITMAP* old = sprites[currMap->spriteIndex].sprite;
    int w = al_get_bitmap_width(old) + abs(x); 
    int h = al_get_bitmap_height(old) + abs(y); 

    ALLEGRO_BITMAP* before = al_get_target_bitmap();


    ALLEGRO_BITMAP* new = al_create_bitmap(w,h);
    al_set_target_bitmap(new);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    int drawOffsetX = 0;
    int drawOffsetY = 0;
    
    if (x <= 0)
    {
        drawOffsetX = abs(x);   
    }
    if (y <= 0)
    {
        drawOffsetY = abs(y);
    }
    currMap->spawnPoint.x += drawOffsetX;
    currMap->spawnPoint.y += drawOffsetY;
    UpdateSpawnPointStr();

    al_draw_tinted_bitmap(sprites[currMap->spriteIndex].sprite,WHITE,drawOffsetX,drawOffsetY,0);
    al_draw_tinted_bitmap(sprites[currMap->secondLayerSpriteIndex].sprite,WHITE,drawOffsetX,drawOffsetY,0);

    sprites[currMap->spriteIndex].sprite = new;


    al_destroy_bitmap(old);
    al_set_target_bitmap(before);

    PreprocessMap(currMap,0);
    if (x < 0)
        MoveCam(players[0].cameraPos.x+abs(x),players[0].cameraPos.y);
    if (y < 0)
        MoveCam(players[0].cameraPos.x,players[0].cameraPos.y+abs(y));

    if (drawOffsetX != 0 || drawOffsetY != 0)
    {
        for (int i = 0; i < editor.numSetupLines; i++)
        {
            if (editor.setupLines[i].associated)
            {
                float xNew = editor.setupLines[i].associated->position.worldX + drawOffsetX;
                float yNew = editor.setupLines[i].associated->position.worldY + drawOffsetY;

                UpdatePosition(editor.setupLines[i].associated,xNew,yNew);
            }
        }   

    }
}       
void ChangeMapDimensions_Handle(EDITOR_HANDLE handle, MouseState mouseState)
{
    Rect handleRect = GetMapHandleRect(handle);
    int edgeOffsetY =  mouseState.worldY - handleRect.y - editor.handleOffset;
    int edgeOffsetX =  mouseState.worldX - handleRect.x - editor.handleOffset;

    if (handle == HANDLE_TOP)
    {
        if (edgeOffsetY < 0)
        {
            IncreaseMapDimensions(0,edgeOffsetY);
        }
        if (edgeOffsetY > 0)
        {
            DecreaseMapDimensions(handle,0,edgeOffsetY);
        }
    }
    if (handle == HANDLE_RIGHT)
    {
        if (edgeOffsetX > 0)
        {
            IncreaseMapDimensions(edgeOffsetX,0);
        }
        if (edgeOffsetX < 0)
        {
            DecreaseMapDimensions(handle,edgeOffsetX,0);
        }
    }
    if (handle == HANDLE_BOTTOM)
    {
        if (edgeOffsetY > 0)
        {
            IncreaseMapDimensions(0,edgeOffsetY);
        }
        if (edgeOffsetY < 0)
        {
            DecreaseMapDimensions(handle,0,edgeOffsetY);
        }
    }
    if (handle == HANDLE_LEFT)
    {
        if (edgeOffsetX < 0)
        {
            IncreaseMapDimensions(edgeOffsetX,0);
        }
        if (edgeOffsetX > 0)
        {
            DecreaseMapDimensions(handle,edgeOffsetX,0);
        }

    }


}


void SetEditorStateToNormal()
{
    editor.editorState = EDITOR_STATE_NORMAL;
    editor.paintingMode = PAINTING_MODE_NONE;
}

void SetEditorStateToDrawing()
{
    editor.heldObject = NULL;
    editor.highlightedObject = NULL;
    editor.editorState = EDITOR_STATE_PAINTING;
}

char* GetCallEnd(char* str, char* func)
{
    char* start = strstr(str,func);
    int numBrackets = 0;
    char* end = NULL;
    bool insideQuotes = false;
    for (char* c = start; c < start + strlen(start); c++)
    {
        if (*c == '"')
            insideQuotes = !insideQuotes;
        if (!insideQuotes)
        {
            if (*c == '(')
                numBrackets++;
            if (*c == ')')
                numBrackets--;

            if (*c == ')' && numBrackets == 0)
            {
                end = c;
                break;
            }
        }

    }
    return end;
}
void InsertStr(char** start, char** location, char* str)
{
    if (!str)
        return;
     char* copy = calloc(strlen(*start)+1,sizeof(char));
    strcpy(copy,*start);

    size_t offset = *location - *start;

    int lenBefore = strlen(*start);
    int totalLen = strlen(*start) + strlen(str);
    *start = realloc(*start, (totalLen +1) * sizeof(char));

    memset(*start, 0, totalLen + 1);

    strncpy(*start,copy,offset);
    strcat(*start,str);
    strcat(*start,copy+offset);

    free(copy);

    *location = &(*start)[offset];

}
void DeleteStr(char** start, char** location, int numChars)
{
    int offset = *location - *start;

    if (numChars >= strlen(*start))
    {
        (*start)[0] = '\0';
    }
    else
    {
        int startLen = strlen(*start);
        for (int i = offset; i < startLen - numChars; i++)
        {
            (*start)[i] = (*start)[i+numChars];
        }
        int j = (startLen-numChars) - 1;
        (*start)[(startLen-numChars)] = '\0';
    }
    *start = realloc(*start,(strlen(*start) + 1) * sizeof(char));
    *location = &(*start)[offset];
}


char* GetFunctionEndPoint(char* buffer)
{
    if (!buffer)
        return NULL;
    char* c = buffer;

    bool foundFunc = false;

    while (*(c++))
    {
        foundFunc = false;
        if (strncmp(c,"function ",strlen("function ")) == 0)
        {
            foundFunc = true;
            char* c2 = c;
            while ((c2--) != buffer && *c2 != '\n')
            {
                if (strncmp(c2,"--",strlen("--")) == 0)
                {
                    foundFunc = false;
                    break;
                }
            }
        }
        if (*c == '\0')
            foundFunc = true;
        if (foundFunc)
            break;    
    }
    if (foundFunc)
    {
        while (strncmp((c--),"end",strlen("end")) != 0)
        {

        }
    }
    else
    {
        return NULL;
    }

    return c+1;


}

char* GetFunctionStartPosition(char* buffer, char* funcName)
{
    if (!buffer)
        return NULL;
    char* c = buffer;
    char* full = c + strlen(funcName);

    char* start;

    if (c == NULL) 
        return false;

    bool funcExists = false;
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
            {
                funcExists = true;
                break;
            }
        }
        c++;
    }
    if (!funcExists)
        return NULL;

    c += strlen(funcName);
    while (*(c++) != ')' && *c != '\0')
    {

    }
  


    return c;

}   
//extract function setup so we can run that one line at a time and create Line objects from that
char* ExtractFunction(char* funcName, char* buffer)
{
    char* c = buffer;
    char* full = c + strlen(funcName);

    char* start;

    if (c == NULL) 
        return false;

    bool funcExists = false;
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
            {
                funcExists = true;
                break;
            }
        }
        c++;
    }
    if (!funcExists)
        return NULL;
    char* end = c + strlen(c);



    for (char* c2 = start; c2 < buffer + strlen(buffer); c2++)
    {

        if (isspace(*c2) && strncmp(c2+1,"function ",strlen("function ")) == 0)
        {
            end = c2;
            break;
        }
    }
    //remove the 'end' at the ending part of functions
    while (*(end--) != 'e')
    {

    }end++;
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
            {
                position += j + 1;
                break;
            }
        }

       // if (strstr(position,","))
         //   position = strstr(position,",")+1;
    }   
       
    if (argumentToGet == 1)
    {
        //position = strstr(position,"(")+1;
    }
    return position;
}
void UpdateArgumentStr(char** full, char* position, char* str, bool addQuotes)
{
    int numToMove = 0;
    int numQuotes = 0;

    bool numOpenBrackets = 0;
    for (int i = 0; i < strlen(position); i++)
    {
        char* c = position + i;
        if (position[i] == '"')
            numQuotes++;
        if ((position[i] == ',' || position[i] == ')') && numQuotes == 0)
        {
            break;
        }
        if (position[i] == ')' && numOpenBrackets==0)
            break;

        if (position[i] == '(')
            numOpenBrackets++;
        if (position[i] == ')')
            numOpenBrackets--;
        numToMove++;

        if (numQuotes == 2)
            break;
    }
    DeleteStr(full,&position,numToMove);

    char* copy = calloc(strlen(str)+3,sizeof(char));
    if (addQuotes)
        strcat(copy,"\"");
    strcat(copy,str);
    if (addQuotes)
        strcat(copy,"\"");

    InsertStr(full,&position,copy);
     
    free(copy);
    printf("STR: %s\n",*full);



}
void UpdateArgumentFloat(char** full, char* position, float f, bool decimal)
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
    if (decimal)
        sprintf(digits,"%.2f",f);
    else
        sprintf(digits,"%i",(int)f);


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
    if (editor.heldObject && editor.heldObject == g)
    {
        x -= editor.heldObjectOffset.x;
        y -= editor.heldObjectOffset.y;
    }

    if (!g) return;
    for (int i = 0; i < editor.numSetupLines; i++)
    {
        if (editor.setupLines[i].associated == g)
        {
            if (editor.setupLines[i].line)
            {
                g->position.worldX = x;
                g->position.worldY = y;


                UpdateScreenPositions(g);

                //2-decimal precision for both
                int toIncreaseBy = NumDigits(x) + NumDigits(y) + 6;
                //editor.setupLines[i].line = realloc(editor.setupLines[i].line,(strlen(editor.setupLines[i].line)+toIncreaseBy+1)*sizeof(char));

                char* xStr = GetPositionOfArgument(editor.setupLines[i].line,"CreateObject",2);
                UpdateArgumentFloat(&editor.setupLines[i].line,xStr,x,true);
                char* yStr = GetPositionOfArgument(editor.setupLines[i].line,"CreateObject",3);
                UpdateArgumentFloat(&editor.setupLines[i].line,yStr,y,true);

                printf("%s\n",editor.setupLines[i].line);
            }
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
            if (ObjIsDecoration(editor.setupLines[i].associated))
            {
                GameObject* g = editor.setupLines[i].associated;
            }   
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
    if (!buffer)
        return;

    char* save = buffer;

    //char* bufferLine = strtok_r(buffer,"\n",&save);

    int numLines = 0;

    int openBrackets = 0;
    char* lineStart = buffer;
    char* end = buffer + strlen(buffer);
    for (char* c = buffer; c < end; c++)
    {
        if (*c == '(')
            openBrackets++;
        if (*c == ')')
            openBrackets--;
        if (openBrackets < 0)
            openBrackets = 0;

        
        if ((*c == '\n' || c == end-1) && openBrackets == 0)
        {
            char* c2 = c;
            bool isComment = false;
            while (c2-- != buffer && (*c2 != '\0' || (*c2) != '\n'))
            {
                if (strncmp(c2,"--",strlen("--")))
                {
                    isComment = true;
                }
            }
            numLines++;
            if (!*lines)
                *lines = calloc(1,sizeof(EditorLine));
            
            char after[3];
            after[0] = '\r';
            after[1] = *c;
            after[2] = '\0';


            *c = '\0';
            if ((c - 1) >= buffer && *(c - 1) == '\r')
                *(c - 1) = '\0';
            
            char* bufferLine = lineStart;
            lineStart = c + 1;

            *lines = realloc(*lines,numLines * sizeof(EditorLine));

            EditorLine* line = *lines;

            memset(&line[numLines-1],0,sizeof(EditorLine));

            line[numLines-1].line = calloc(strlen(bufferLine)+strlen(after)+1,sizeof(char));

            strcat(line[numLines-1].line,bufferLine);
            strcat(line[numLines-1].line,after);
            
            line[numLines-1].lineNumber = numLines-1; 
            line->lineIsComment = isComment;

        }

    }
    *lineCount = numLines;
}

void EditorSetMap(char* path)
{
    if (!path) 
        return;
    RemoveAllGameObjects();
    Map* m = LoadMap(path);
    currEncounterRunning->goingToShop = false;

    //SetMap(m);
    currMap = m;
    path = currMap->path;


    if (editor.setupLines)
    {
        for (int i = 0; i < editor.numSetupLines; i++)
        {
            free(editor.setupLines[i].line);
        }
        free(editor.setupLines);
        editor.setupLines = NULL;
        editor.numSetupLines = 0;

    }
    if (editor.endLines)
    {
        for (int i = 0; i < editor.numEndLines; i++)
        {
            free(editor.endLines[i].line);
        }
        free(editor.endLines);
        editor.endLines = NULL;
        editor.numEndLines = 0;

    }
    char* buff = ExtractFunction("setup",m->lua_buffer.buffer);
    char* end = ExtractFunction("mapend",m->lua_buffer.buffer);

    char* s = GetFunctionStartPosition(m->lua_buffer.buffer,"setup");
    char* e = GetFunctionEndPoint(s);

    int numChars = e - s;

    char* str = calloc(numChars+1,sizeof(char));
    strncpy(str,s,numChars);

    SplitLines(buff,&editor.setupLines,&editor.numSetupLines);
    SplitLines(end,&editor.endLines,&editor.numEndLines);




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
    editor.editorUI.goingShop = currEncounterRunning->goingToShop;

    ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"Location"),path);
    //ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"NextMap"),);

    if (editor.currentPath)
        free(editor.currentPath);
    editor.currentPath = calloc(strlen(path)+1,sizeof(char));

    //get the folder we're in
    bool normalChar = false;

    char* pathCopy = calloc(strlen(path)+1,sizeof(char));
    strcpy(pathCopy,path);
    for (int i = strlen(pathCopy)-1; i >= 0; i--)
    {
        if (isalnum(pathCopy[i]))
        {
            normalChar = true;
        }

        if (normalChar && (pathCopy[i] == '/' || pathCopy[i] == '\\'))
        {
            pathCopy[i+1] = '\0';
            pathCopy[i] = '/';
            break;
        }
    }

    SetUITextStr(GetUIText(&editor.editorUI.fileSelector,"Path"),pathCopy);
    strcpy(editor.currentPath,pathCopy);

    free(pathCopy);
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

    InitFileSelector(p,&editor.editorUI.fileNamingUI);
    int x = 40; int y = 40; int w = _SCREEN_SIZE-70; int h = 20;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name,".DS_Store")!=0 && strlen(dir->d_name) > 0)
            {
                //ignore the '.' file
                if (strlen(dir->d_name) == 1 && dir->d_name[0] == '.')
                    continue;
                bool foundPostfix = false;

                if (dir->d_type == DT_DIR)
                    foundPostfix = true;
                for (int i = 0; i < numPostfixes; i++)
                {
                    if (strstr(dir->d_name,postfixes[i]))
                    {
                        foundPostfix = true;
                        break;
                    }   
                    
                }
                if (foundPostfix)
                {
                    AddButton(p,"FilePath",dir->d_name,x,y,w,h,true);
                    y += h;
                }
            }

        }
    }

    for (int i = 0; i < numPostfixes; i++)
    {
        free(postfixes[i]);
    }
    free(postfixes);



}
EditorLine* AddEditorLine(EditorLine** lines, int* numLines, char* str, bool end)
{
    *lines = realloc(*lines,((*numLines)+1)*sizeof(EditorLine));


    EditorLine* li;

    //if we want to place it at the start of the function or the end
    if (end)
    {
        li = &(*lines)[*numLines];

        memset(li,0,sizeof(EditorLine));

        li->line = calloc(strlen(str)+1,sizeof(char));
        strcpy(li->line,str);
    }
    if (!end)
    {
        for (int i = (*numLines); i >= 1; i--)
        {
            (*lines)[i] = (*lines)[i-1];
        }
        li = &(*lines)[0];
        memset(li,0,sizeof(EditorLine));
    }
    li->line = calloc(strlen(str)+1,sizeof(char));
    strcpy(li->line,str);
    (*numLines)++;

    return li;

}
void AddObjLineToFile(GameObject* g, float x, float y, OBJ_FRIENDLINESS ownedBy, float summonTime, float completionPercent)
{
    char* line = NULL; 

    char* fmt = "    CreateObject(\"%s\",%.2f,%.2f,%s,%.2f)\n";
    char* friendliness;
    if (ownedBy == TYPE_ENEMY)
        friendliness = "TYPE_ENEMY";
    if (ownedBy == TYPE_FRIENDLY)
        friendliness = "TYPE_FRIENDLY";
    if (ownedBy == TYPE_DECORATION)
        friendliness = "TYPE_DECORATION";


    size_t bufferSize = snprintf(NULL, 0, fmt,g->path,x,y,friendliness,completionPercent);
    line = calloc(bufferSize+1,sizeof(char));
    sprintf(line,fmt,g->path,x,y,friendliness,completionPercent);


    EditorLine* e = AddEditorLine(&editor.setupLines,&editor.numSetupLines,line,true);
    
    e->associated = g;
    
    free(line);

}
void SaveFunction(char** buffer, char* funcName, EditorLine* lines, int numLines)
{
    char* s = GetFunctionStartPosition(*buffer,funcName);
    char* e = GetFunctionEndPoint(s);

    if (s)
        DeleteStr(buffer, &s, (e-s));
    
    


    if (!s)
    {
        size_t len = snprintf(NULL,0,"\nfunction %s()\n",funcName);

        char* start = calloc(len+1,sizeof(char));
        sprintf(start,"\nfunction %s()\n",funcName);

        char* end = "end\n";

        size_t totalLength = strlen(*buffer) + strlen(start) + strlen(end);
        *buffer = realloc(*buffer,(totalLength + 1) * sizeof(char));

        memset((*buffer) + strlen(*buffer), 0, (strlen(start) + strlen(end) + 1) * sizeof(char));
        
        strcat(*buffer,start);
        strcat(*buffer,end);

        free(start);
    }
    s = GetFunctionStartPosition(*buffer,funcName);

    for (int i = numLines-1; i >= 0; i--)
    {
        InsertStr(buffer, &s, lines[i].line);
    }
}
void SetNextMap(char* path)
{
    bool foundFunc = false;
    if (editor.nextMap)
        free(editor.nextMap);
    editor.nextMap = calloc(strlen(path)+1,sizeof(char));
    strcpy(editor.nextMap,path);

    ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"NextMap"),editor.nextMap);

    for (int i = 0; i < editor.numEndLines; i++)
    {
        if (!editor.endLines[i].lineIsComment && strstr(editor.endLines[i].line,"ChangeMap("))
        {
            foundFunc = true;

            char* c = GetPositionOfArgument(editor.endLines[i].line,"ChangeMap",1);
            UpdateArgumentStr(&editor.endLines[i].line,c,path,true);

        }
    }
    if (!foundFunc)
    {
        char* str = calloc(strlen(path) + strlen("   ChangeMap(\"\")\n") + 1,sizeof(char));
        sprintf(str,"   ChangeMap(\"%s\")\n",path);
        AddEditorLine(&editor.endLines, &editor.numEndLines, str,true);

        free(str);
    }
}
ALLEGRO_BITMAP* CreateCombinedBitmap(int w, int h, int numBitmaps, ...)
{
    ALLEGRO_BITMAP* before = al_get_target_bitmap();

    ALLEGRO_BITMAP* combined = al_create_bitmap(w,h);
    al_set_target_bitmap(combined);
    al_clear_to_color(al_map_rgba(0,0,0,0));

    va_list argp;   
    va_start(argp, numBitmaps);
    for (int i = 0; i < numBitmaps; i++)
    {
        ALLEGRO_BITMAP* b = va_arg(argp, ALLEGRO_BITMAP*);
        ALLEGRO_COLOR c = va_arg(argp,ALLEGRO_COLOR);

        al_draw_tinted_bitmap(b,c,0,0,0);
    }

    al_set_target_bitmap(before);
    return combined;

}
void SaveMap()
{
    SaveFunction(&currMap->lua_buffer.buffer,"setup",editor.setupLines,editor.numSetupLines);
    SaveFunction(&currMap->lua_buffer.buffer,"mapend",editor.endLines,editor.numEndLines);

    ALLEGRO_FILE* file = al_fopen(currMap->path, "wb");
    if (file == NULL)
    {
        printf("Couldn't save file\n");   
        return;
    }

    al_fwrite(file,currMap->lua_buffer.buffer,strlen(currMap->lua_buffer.buffer) * sizeof(char));
    al_fclose(file);

   // ALLEGRO_BITMAP* before = al_get_target_bitmap();

    int w = al_get_bitmap_width(sprites[currMap->spriteIndex].sprite); int h = al_get_bitmap_height(sprites[currMap->spriteIndex].sprite);
    
    //ALLEGRO_BITMAP* combined = al_create_bitmap(w,h);
    //al_set_target_bitmap(combined);
    //al_draw_tinted_bitmap(sprites[currMap->spriteIndex].sprite,WHITE,0,0,0);
    //al_draw_tinted_bitmap(sprites[currMap->secondLayerSpriteIndex].sprite,al_map_rgb(0,0,0),0,0,0);
    ALLEGRO_BITMAP* combined = CreateCombinedBitmap(w,h,2,sprites[currMap->spriteIndex].sprite,WHITE,sprites[currMap->secondLayerSpriteIndex].sprite,al_map_rgb(0,0,0));


    char* path = sprites[currMap->spriteIndex].path;
    al_save_bitmap(path,combined);

    al_destroy_bitmap(combined);
    //al_set_target_bitmap(before);
}
void UpdateGoingShop()
{
    bool goingShop = editor.editorUI.goingShop;
    bool foundStr = false;
    for (int i = 0; i < editor.numEndLines; i++)
    {
        if (strstr(editor.endLines[i].line,"GoShop"))
        {
            memset(editor.endLines[i].line,0,strlen(editor.endLines[i].line)*sizeof(char));

            //char* arg = GetPositionOfArgument(editor.endLines[i].line,"GoShop",1);
            //UpdateArgumentStr(&editor.endLines[i].line,arg,goingShop == true ? "true" : "false",false);
            //foundStr = true;
        }
    }
    char* fmt = "\n    GoShop(%s)";
    int numChars = snprintf(NULL,0,fmt,goingShop == true ? "true" : "false");
    char* str = calloc(numChars+1,sizeof(char));
    sprintf(str,fmt,goingShop == true ? "true" : "false");
    AddEditorLine(&editor.endLines,&editor.numEndLines,str,false);
    free(str);
}
void UpdateEditor(float dt,MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    bool previousGoingShop = editor.editorUI.goingShop;
    SpawnPointRectIsMoved(mouseState,mouseStateLastFrame);
    if (editor.editorUI.showFileNamingUI)
        UpdatePanel(&editor.editorUI.fileNamingUI,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);

    if (editor.editorUI.showFileSelector)
        UpdatePanel(&editor.editorUI.fileSelector,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);

    UpdatePanel(&editor.editorUI.saveLoad,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);
    if (previousGoingShop != editor.editorUI.goingShop)
    {
        UpdateGoingShop();
    }
    UpdatePanel(&editor.editorUI.unitSelector,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);
    if (editor.highlightedObject)
        UpdatePanel(&editor.editorUI.unitOptions,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);
    UpdatePanel(&editor.editorUI.mapImageEditor,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);

    if (!_PANEL_CLICKED_THIS_FRAME && editor.editorState == EDITOR_STATE_NORMAL && MouseClickedThisFrame(&mouseState, &mouseStateLastFrame))
    {
        editor.heldObject = GetClicked(mouseState.worldX,mouseState.worldY);
        if (editor.heldObject)
        {
            editor.heldObjectOffset.x = mouseState.worldX - editor.heldObject->position.worldX;
            editor.heldObjectOffset.y = mouseState.worldY - editor.heldObject->position.worldY;

            editor.highlightedObject = editor.heldObject;  
            editor.editorUI.heldObjectIsDecor = ObjIsDecoration(editor.heldObject);
            ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex = GetPlayerOwnedBy_IncludeDecor(editor.heldObject);
            
            UIElement* aggroGroup = GetUIElement(&editor.editorUI.unitOptions,"AggroGroup");
            if (editor.heldObject->aggroGroupSet)
            {
                char* fmt = "%i";
                int numAggroChars = snprintf(NULL,0,fmt,editor.heldObject->aggroGroup); 

                char* str = calloc(numAggroChars+1,sizeof(char));
                sprintf(str,fmt,editor.heldObject->aggroGroup);
                SetTextInputStr(aggroGroup,str);
                free(str);

            }
            else
            {   
                ClearTextInputStr(aggroGroup);
            }

        }
        else
        {
            editor.highlightedObject = NULL;
            editor.heldHandle = CheckMapHandleClicked(mouseState);
            if (editor.heldHandle != HANDLE_NONE)
            {
                Rect handle = GetMapHandleRect(editor.heldHandle);
                if (HandleIsVertical(editor.heldHandle))
                    editor.handleOffset = mouseState.worldY - handle.y;
                else
                    editor.handleOffset = mouseState.worldX - handle.x;
            }   

        } 
    }
    if (editor.heldHandle && !(mouseState.mouse.buttons & 1))
    {   
        ChangeMapDimensions_Handle(editor.heldHandle,mouseState);
        editor.heldHandle = HANDLE_NONE;
    }
    if (editor.heldObject)
    {
        UpdatePosition(editor.heldObject,mouseState.worldX,mouseState.worldY);
    }
    if (!(mouseState.mouse.buttons & 1))
    {
        editor.heldObject = NULL;
    }
    if (mouseState.mouse.buttons & 2)
    {
        editor.heldObject = NULL;
        editor.highlightedObject = NULL;   
    }



    if (GetButton(&editor.editorUI.mapImageEditor,"Draw") || KeyPressedThisFrame(ALLEGRO_KEY_B,keyState,keyStateLastFrame))
    {
        if (editor.editorState == EDITOR_STATE_PAINTING && editor.paintingMode == PAINTING_MODE_DRAW_FIRST_LAYER)
        {
            SetEditorStateToNormal();
        }
        else
        {
            SetEditorStateToDrawing();
            editor.paintingMode = PAINTING_MODE_DRAW_FIRST_LAYER;
        }
    }
    if (GetButton(&editor.editorUI.mapImageEditor,"SecondLayer") || KeyPressedThisFrame(ALLEGRO_KEY_G,keyState,keyStateLastFrame))
    {
        if (editor.editorState == EDITOR_STATE_PAINTING && editor.paintingMode == PAINTING_MODE_SECOND_LAYER)
        {
            SetEditorStateToNormal();
        }
        else
        {
            SetEditorStateToDrawing();
            editor.paintingMode = PAINTING_MODE_SECOND_LAYER;
        }
    }
    if (GetButton(&editor.editorUI.mapImageEditor,"Erase") || KeyPressedThisFrame(ALLEGRO_KEY_E,keyState,keyStateLastFrame))
    {
        if (editor.editorState == EDITOR_STATE_PAINTING && editor.paintingMode == PAINTING_MODE_ERASING)
        {
            SetEditorStateToNormal();
        }
        else
        {
            SetEditorStateToDrawing();
            editor.paintingMode = PAINTING_MODE_ERASING;
        }
    }
    if (editor.editorState  == EDITOR_STATE_PAINTING && (KeyPressedThisFrame(ALLEGRO_KEY_ENTER,keyState,keyStateLastFrame) || KeyPressedThisFrame(ALLEGRO_KEY_ESCAPE,keyState,keyStateLastFrame)))
    {
        editor.editorState = EDITOR_STATE_NORMAL;
    }
    if (!_PANEL_CLICKED_THIS_FRAME && (editor.editorState == EDITOR_STATE_PAINTING) && mouseState.mouse.buttons & 1)
    {
        if (editor.paintingMode == PAINTING_MODE_DRAW_FIRST_LAYER || editor.paintingMode == PAINTING_MODE_SECOND_LAYER)
        {
            int offsetX = 0; int offsetY = 0;
            if (mouseState.worldX < 0)
                offsetX = mouseState.worldX;
            if (mouseState.worldY < 0)
                offsetY = mouseState.worldY;
            if (mouseState.worldX > GetMapWidth())
                offsetX = mouseState.worldX - GetMapWidth();
            if (mouseState.worldY > GetMapHeight())
                offsetY = mouseState.worldY - GetMapHeight();
            IncreaseMapDimensions(offsetX,offsetY);

            
        }
        float brushSize = editor.paintSize;
        brushSize = PAINT_SIZE_MIN + (PAINT_SIZE_MAX*brushSize);

        ALLEGRO_BITMAP* before = al_get_target_bitmap();
        if (editor.paintingMode == PAINTING_MODE_DRAW_FIRST_LAYER)
            al_set_target_bitmap(sprites[currMap->spriteIndex].sprite);
        if (editor.paintingMode == PAINTING_MODE_SECOND_LAYER)   
            al_set_target_bitmap(sprites[currMap->secondLayerSpriteIndex].sprite);


        ALLEGRO_COLOR c = WHITE;
        if (editor.paintingMode == PAINTING_MODE_DRAW_FIRST_LAYER || editor.paintingMode == PAINTING_MODE_SECOND_LAYER)
        {
            c = WHITE;
        }
        else if (editor.paintingMode == PAINTING_MODE_ERASING)
        {
            c = _TRANSPARENT;
        }

        int opBefore; int srcBefore; int dstBefore;
        al_get_blender(&opBefore, &srcBefore, &dstBefore);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);

        if (editor.paintingMode == PAINTING_MODE_ERASING)
        {
            al_set_target_bitmap(sprites[currMap->spriteIndex].sprite);
            al_draw_filled_circle(mouseState.worldX, mouseState.worldY,brushSize,c);
            al_set_target_bitmap(sprites[currMap->secondLayerSpriteIndex].sprite);
            al_draw_filled_circle(mouseState.worldX, mouseState.worldY,brushSize,c);
        }
        else if (editor.paintingMode == PAINTING_MODE_SECOND_LAYER)
        {
            al_draw_filled_circle(mouseState.worldX, mouseState.worldY,brushSize,c);
        }
        else if (editor.paintingMode == PAINTING_MODE_DRAW_FIRST_LAYER)
        {
            al_set_target_bitmap(sprites[currMap->spriteIndex].sprite);
            al_draw_filled_circle(mouseState.worldX, mouseState.worldY,brushSize,c);
            al_set_target_bitmap(sprites[currMap->secondLayerSpriteIndex].sprite);
            al_draw_filled_circle(mouseState.worldX, mouseState.worldY,brushSize,_TRANSPARENT);
        }
            
        

        al_set_blender(opBefore,srcBefore,dstBefore);
        al_set_target_bitmap(before);
    }
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
    if (GetButton(&editor.editorUI.mapImageEditor,"MapPath"))
    {
        editor.editorUI.showFileSelector = true;
        editor.editorUI.selectorPicked = EDITOR_FILE_MAP_SPRITE;

        PopulateFileList(&editor.editorUI.fileSelector,editor.currentPath,1,".png");

    }


    if (GetButton(&editor.editorUI.fileSelector,"Back"))
    {
        editor.editorUI.showFileSelector = false;
    }
    if (GetButton(&editor.editorUI.fileSelector,"New"))
    {
        editor.editorUI.showFileNamingUI = true;
       
    }
    if (GetButton(&editor.editorUI.fileNamingUI,"Confirm"))
    {
        char* name = ((TextInput*)(GetUIElement(&editor.editorUI.fileNamingUI,"FileName")->data))->text;
        if (strlen(name) > 0)
        {
            char* fullStr = calloc(strlen(name)+strlen(editor.currentPath)+strlen(".lua")+3,sizeof(char));

            strcat(fullStr,editor.currentPath);
            if (fullStr[strlen(fullStr)-1] != '/')
                strcat(fullStr,"/");
            for (int i = 0; i < strlen(name); i++)
                name[i] = tolower(name[i]);
            strcat(fullStr,name);




            //create a new lua file and populate it with boilerplate
            if (editor.editorUI.selectorPicked == EDITOR_FILE_CURRENT_MAP || editor.editorUI.selectorPicked == EDITOR_FILE_NEXT_MAP)
            {
                if (!strstr(fullStr,".lua"))
                    strcat(fullStr,".lua");

                if (FileExists(fullStr))
                {
                    printf("Could not save %s: file already exists",fullStr);
                    free(fullStr);
                    return;
                }
                
                char* fmt = "function setup()\n\nend\nfunction update()\n\nend\nfunction mapend()\n\nend";
                ALLEGRO_FILE* file = al_fopen(fullStr, "wb");
                if (file == NULL)
                {
                    printf("Couldn't save file\n");   
                    free(fullStr);
                    return;
                }
                al_fwrite(file,fmt,strlen(fmt));

                al_fclose(file);


            }
            if (editor.editorUI.selectorPicked == EDITOR_FILE_MAP_SPRITE)
            {
                if (!strstr(fullStr,".png"))
                    strcat(fullStr,".png");

                if (FileExists(fullStr))
                {
                    free(fullStr);
                    printf("Could not save %s: file already exists",fullStr);
                    return;
                }

                ALLEGRO_BITMAP* before = al_get_target_bitmap();                
                Sprite* new = NewSprite(256,256);
                new->path = calloc(strlen(fullStr)+1,sizeof(char));
                strcpy(new->path,fullStr);
                al_set_target_bitmap(new->sprite);
                al_clear_to_color(WHITE);    

                al_save_bitmap(fullStr,new->sprite);

                al_set_target_bitmap(before);

                currMap->spriteIndex = new - sprites;
                GenerateInvertedSprite(&sprites[currMap->spriteIndex]);

                EditorSetMapSprite(fullStr);
                
            }

            if (editor.editorUI.selectorPicked == EDITOR_FILE_CURRENT_MAP)
            {
                EditorSetMap(fullStr);
            }
            if (editor.editorUI.selectorPicked == EDITOR_FILE_NEXT_MAP)
            {
                SetNextMap(fullStr);
            }
            
            DisableButton(GetUIElement(&editor.editorUI.fileNamingUI,"Confirm"));
            DisableButton(GetUIElement(&editor.editorUI.fileNamingUI,"Cancel"));

            editor.editorUI.showFileSelector = false;
            editor.editorUI.showFileNamingUI = false;






            free(fullStr);
        }
    }
    if (GetButton(&editor.editorUI.fileNamingUI,"Cancel"))
    {
        editor.editorUI.showFileNamingUI = false;

    }



    if (editor.editorUI.showFileSelector)
    {
        for (int i = 0; i < editor.editorUI.fileSelector.numElements; i++)
        {
            UIElement* u = &editor.editorUI.fileSelector.elements[i];
            if (u->name && strcmp(u->name,"FilePath")==0)
            {
                Button* b = (Button*)u->data;
                if (GetButtonIsClicked(u))
                {
                    char* newPath = calloc(strlen(editor.currentPath)+strlen(b->description)+3,sizeof(char));
                    DIR *d;
                       struct dirent *dir;
                    d = opendir(editor.currentPath);  

                    if (strcmp(b->description,"..") == 0)
                    {
                        strcat(newPath,editor.currentPath);
                        bool normalChar = false;
                        for (int j = strlen(newPath)-1; j >= 0; j--)
                        {   
                            //skip past the initial '/' we find if it's at the start
                            if (isalnum(newPath[j]))
                            {
                                normalChar = true;
                            }

                            if (normalChar && (newPath[j] == '/' || newPath[j]== '\\'))
                            {
                                newPath[j+1] = '\0';
                                break;
                            }
                        }
                        if (editor.currentPath)
                            free(editor.currentPath);
                        editor.currentPath = calloc(strlen(newPath)+1,sizeof(char));
                        strcpy(editor.currentPath,newPath);

                        PopulateFileList(&editor.editorUI.fileSelector,newPath,1,".lua");
                    }
                    else
                    {
                        strcat(newPath,editor.currentPath);
                        if (newPath[strlen(newPath)-1] != '/')
                            strcat(newPath,"/");
                        strcat(newPath,b->description);

                        
                        if (d) {
                            while ((dir = readdir(d)) != NULL) {
                                if (strcmp(dir->d_name,b->description)==0)
                                {
                                    if (dir->d_type == DT_REG)
                                    {
                                        if (editor.editorUI.selectorPicked == EDITOR_FILE_CURRENT_MAP)
                                        {
                                            EditorSetMap(newPath);
                                            editor.editorUI.showFileSelector = false;

                                        }
                                        if (editor.editorUI.selectorPicked == EDITOR_FILE_MAP_SPRITE)
                                        {
                                            EditorSetMapSprite(newPath);
                                            editor.editorUI.showFileSelector = false;
                                        }
                                        if (editor.editorUI.selectorPicked == EDITOR_FILE_NEXT_MAP)
                                        {
                                            editor.editorUI.showFileSelector = false;
                                            SetNextMap(newPath);
                                        }

                                        break;
                                    }
                                    if (dir->d_type == DT_DIR)
                                    {
                                        if (editor.currentPath)
                                            free(editor.currentPath);
                                        editor.currentPath = calloc(strlen(newPath)+1,sizeof(char));
                                        strcpy(editor.currentPath,newPath);

                                        PopulateFileList(&editor.editorUI.fileSelector,newPath,1,".lua");
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    free(newPath);



                }
            }
        }
    }

    for (int i = 0; i < editor.editorUI.unitSelector.numElements; i++)
    {
        UIElement* u = &editor.editorUI.unitSelector.elements[i];
        Button* b = (Button*)u->data;
        if (GetButtonIsClicked(u))
        {
            GameObject* toSpawn = prefabs[i];
            GameObject* g = AddGameobject(toSpawn,GetCameraMiddleX(),GetCameraMiddleY(),SOURCE_SPAWNED_FROM_MAP,TYPE_ENEMY);
            SetOwnedBy(g,TYPE_ENEMY);
            AddObjLineToFile(g,g->position.worldX,g->position.worldY,GetPlayerOwnedBy(g),0,0);
        }

    }
    if (!_TEXTINPUT_HIGHLIGHTED && editor.highlightedObject && KeyPressedThisFrame(ALLEGRO_KEY_BACKSPACE, keyState,keyStateLastFrame))
    {
        for (int i = 0; i < editor.numSetupLines; i++)
        {
            if (editor.setupLines[i].associated == editor.highlightedObject)
            {
                KillObj(editor.highlightedObject,false,false);
                memset(editor.setupLines[i].line,0,strlen(editor.setupLines[i].line)*sizeof(char));
                editor.setupLines[i].associated = NULL;
                editor.highlightedObject = NULL;
                editor.heldObject = NULL;
                break;
            }
        }
    }

    if (editor.highlightedObject)
    {
        int ownedBefore = GetPlayerOwnedBy_IncludeDecor(editor.highlightedObject);
        int ownedAfter = ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex;
        if (ownedBefore != ownedAfter)
        {
            char* str;
            if (ownedAfter == TYPE_FRIENDLY)
                str = "TYPE_FRIENDLY";
            if (ownedAfter == TYPE_ENEMY)
                str = "TYPE_ENEMY";

            for (int i = 0; i < editor.numSetupLines; i++)
            {
                if (editor.setupLines[i].associated == editor.highlightedObject)
                {
                    char* c = GetPositionOfArgument(editor.setupLines[i].line,"CreateObject",4);
                    UpdateArgumentStr(&editor.setupLines[i].line,c,str,false);
                }

            }

        }
        SetOwnedBy(editor.highlightedObject,ownedAfter);
        if (SetDecoration(editor.highlightedObject,editor.editorUI.heldObjectIsDecor))
        {
            for (int i = 0; i < editor.numSetupLines; i++)
            {
                if (editor.setupLines[i].associated == editor.highlightedObject)
                {
                    if (strstr(editor.setupLines[i].line,"SetDecoration"))
                    {
                        char* arg = GetPositionOfArgument(editor.setupLines[i].line,"SetDecoration",2);
                        if (ObjIsDecoration(editor.highlightedObject))
                            UpdateArgumentStr(&editor.setupLines[i].line,arg,"true",false);
                        else
                            UpdateArgumentStr(&editor.setupLines[i].line,arg,"false",false);


                        printf("%s\n",editor.setupLines[i].line);
                    }
                    else
                    {
                        char* pos = strstr(editor.setupLines[i].line,"CreateObject");

                        InsertStr(&editor.setupLines[i].line,&pos,"SetDecoration(");
                        char* posEnd = GetCallEnd(editor.setupLines[i].line,"CreateObject") + 1;
                        char* args = ",true)";
                        InsertStr(&editor.setupLines[i].line,&posEnd,args);

                        printf("%s\n",editor.setupLines[i].line);

                    }
                    break;
                }
            }
        }

        ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex = GetPlayerOwnedBy_IncludeDecor(editor.highlightedObject);
    
        char* aggroGroupText = ((TextInput*)(GetUIElement(&editor.editorUI.unitOptions,"AggroGroup")->data))->text;
        //if (strlen(aggroGroupText) > 0)
        {
            int aggroGroupBefore = editor.highlightedObject->aggroGroup;
            int aggroGroupAfter;//= atoi(aggroGroupText);
            if (strlen(aggroGroupText) > 0)
                aggroGroupAfter = atoi(aggroGroupText);
            else
            {
                editor.highlightedObject->aggroGroup = 0;
                editor.highlightedObject->aggroGroupSet = 0;

                aggroGroupAfter = 0;
            }
            if (aggroGroupBefore != aggroGroupAfter)
            {
                for (int i = 0; i < editor.numSetupLines; i++)
                {
                    if (editor.setupLines[i].associated == editor.highlightedObject)
                    {
                        editor.highlightedObject->aggroGroup = aggroGroupAfter;
                        editor.highlightedObject->aggroGroupSet = true;
                        if (strstr(editor.setupLines[i].line,"SetAggroGroup"))
                        {
                            char* arg = GetPositionOfArgument(editor.setupLines[i].line,"SetAggroGroup",2);
                            UpdateArgumentFloat(&editor.setupLines[i].line,arg,aggroGroupAfter,false);

                            printf("%s\n",editor.setupLines[i].line);
                        }
                        else
                        {
                            char* pos = strstr(editor.setupLines[i].line,"CreateObject");

                            InsertStr(&editor.setupLines[i].line,&pos,"SetAggroGroup(");
                            char* posEnd = GetCallEnd(editor.setupLines[i].line,"CreateObject") + 1;
                            
                            int numChars = snprintf(NULL,0,",%i)",aggroGroupAfter);
                            char* args = calloc(numChars+1,sizeof(char));
                            sprintf(args,",%i)",aggroGroupAfter);
                            InsertStr(&editor.setupLines[i].line,&posEnd,args);

                            free(args);

                            printf("%s\n",editor.setupLines[i].line);

                        }
                        break;
                    }
                }
            }

        }

    }

    int owner = ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex;
    if (editor.highlightedObject)
    {
        //SetOwnedBy(editor.highlightedObject,owner);
        //SetDecoration(editor.highlightedObject,editor.editorUI.heldObjectIsDecor);
    }

    UpdateButton(editor.editorUI.save.x,editor.editorUI.save.y,editor.editorUI.save.w,editor.editorUI.save.h,&editor.editorUI.save,mouseState,mouseStateLastFrame);

    if (GetButtonIsClicked(&editor.editorUI.save))
    {
        SaveMap();
    }

}
bool HandleIsVertical(EDITOR_HANDLE handle)
{
    return (handle == HANDLE_TOP || handle == HANDLE_BOTTOM);
}
bool HandleIsHorizontal(EDITOR_HANDLE handle)
{
    return (handle == HANDLE_LEFT || handle == HANDLE_RIGHT);
}

Rect GetMapHandleRect(EDITOR_HANDLE handle)
{   
    int offset = 16;
    int size = 30; 

    int depth = 8; 

    if (handle == HANDLE_TOP) return (Rect){(GetMapWidth()/2.0f - size/2.0f), -offset, size,depth};
    if (handle == HANDLE_RIGHT) return  (Rect){(GetMapWidth() + offset),GetMapHeight()/2.0f-size/2.0f, depth, size};
    if (handle == HANDLE_BOTTOM) return  (Rect){(GetMapWidth()/2.0f - size/2.0f),GetMapHeight()+offset, size, depth};
    if (handle == HANDLE_LEFT) return (Rect){(0 - offset),GetMapHeight()/2.0f-size/2.0f, depth, size};

    return (Rect){0,0,0,0};

}
void DrawMapHandles(MouseState mouseState)
{
    if (!currMap->spriteIndex)
        return;
    DrawFilledRectWorld(GetMapHandleRect(HANDLE_TOP),al_map_rgb(255,0,0));
    DrawFilledRectWorld(GetMapHandleRect(HANDLE_RIGHT),al_map_rgb(255,0,0));
    DrawFilledRectWorld(GetMapHandleRect(HANDLE_BOTTOM),al_map_rgb(255,0,0));
    DrawFilledRectWorld(GetMapHandleRect(HANDLE_LEFT),al_map_rgb(255,0,0));

    int edgeOffsetX = 0;
    int edgeOffsetY = 0;
    int edgeOffsetW = 0;
    int edgeOffsetH = 0;

    if (editor.heldHandle)
    {
        if (editor.heldHandle == HANDLE_TOP)
        {
            Rect handle = GetMapHandleRect(HANDLE_TOP);
            
            edgeOffsetY =  mouseState.worldY - handle.y - editor.handleOffset;
            edgeOffsetH -= edgeOffsetY;
        }
        if (editor.heldHandle == HANDLE_RIGHT)
        {
            Rect handle = GetMapHandleRect(HANDLE_RIGHT);
            
            edgeOffsetW =  mouseState.worldX - handle.x - editor.handleOffset;
        }
        if (editor.heldHandle == HANDLE_BOTTOM)
        {
            Rect handle = GetMapHandleRect(HANDLE_BOTTOM);
            
            edgeOffsetH =  mouseState.worldY - handle.y - editor.handleOffset;
        }
        if (editor.heldHandle == HANDLE_LEFT)
        {
            Rect handle = GetMapHandleRect(HANDLE_LEFT);
            
            edgeOffsetX =  mouseState.worldX - handle.x - editor.handleOffset;
            edgeOffsetW -= edgeOffsetX;

        }


    }


    Rect r = (Rect){-1 + edgeOffsetX,-1+edgeOffsetY,GetMapWidth()+2+edgeOffsetW, GetMapHeight()+2+edgeOffsetH};
    ToScreenSpace(&r.x,&r.y);
    al_draw_rectangle(r.x,r.y,r.x+r.w,r.y+r.h,al_map_rgb(255,0,0),1);

}
EDITOR_HANDLE CheckMapHandleClicked(MouseState mouse)
{
    if (!currMap->spriteIndex)
        return HANDLE_NONE;
    if (PointInRect(mouse.worldX,mouse.worldY,GetMapHandleRect(HANDLE_TOP)))
        return HANDLE_TOP;
    if (PointInRect(mouse.worldX,mouse.worldY,GetMapHandleRect(HANDLE_RIGHT)))
        return HANDLE_RIGHT;
    if (PointInRect(mouse.worldX,mouse.worldY,GetMapHandleRect(HANDLE_BOTTOM)))
        return HANDLE_BOTTOM;
    if (PointInRect(mouse.worldX,mouse.worldY,GetMapHandleRect(HANDLE_LEFT)))
        return HANDLE_LEFT;
    return HANDLE_NONE;

}
Rect GetSpawnPointRect()
{
    int numToSpawn = 1;
    if (currEncounterRunning)
    {
        numToSpawn = currEncounterRunning->numUnitsToSelect;
    }
    Rect r = (Rect){currMap->spawnPoint.x,currMap->spawnPoint.y, numToSpawn * 16, 16};
    return r;
}
void DrawSpawnPoint()
{
    Rect r = GetSpawnPointRect();
    DrawFilledRectWorld(r,al_map_rgba(255,0,128,128));
}
void UpdateSpawnPointStr()
{
    bool foundSpawnPoint = false;
    for (int i = 0; i < editor.numSetupLines; i++)
    {
        char* c = editor.setupLines[i].line;
        if (strstr(editor.setupLines[i].line,"SetSpawnPoint") && !editor.setupLines[i].lineIsComment)
        {
            foundSpawnPoint = true;
            char* xStr = GetPositionOfArgument(editor.setupLines[i].line,"SetSpawnPoint",1);
            UpdateArgumentFloat(&editor.setupLines[i].line,xStr,currMap->spawnPoint.x,false);
            char* yStr = GetPositionOfArgument(editor.setupLines[i].line,"SetSpawnPoint",2);
            UpdateArgumentFloat(&editor.setupLines[i].line,yStr,currMap->spawnPoint.y,false);

        }
    }   
    if (!foundSpawnPoint)
    {
        char* fmt = "    SetSpawnPoint(%i,%i)\n";
        int numChars = snprintf(NULL,0,fmt,(int)currMap->spawnPoint.x,(int)currMap->spawnPoint.y);
        char* line = calloc(numChars+1,sizeof(char));
        sprintf(line,fmt,(int)currMap->spawnPoint.x,(int)currMap->spawnPoint.y);
        EditorLine* e = AddEditorLine(&editor.setupLines,&editor.numSetupLines,line,true);
        free(line);
    }
}
void SpawnPointRectIsMoved(MouseState mouseState, MouseState mouseStateLastFrame)
{
    Rect r = GetSpawnPointRect();

    Point before = (Point){currMap->spawnPoint.x,currMap->spawnPoint.y};

    currMap->spawnPoint.x = clamp(currMap->spawnPoint.x,0,GetMapWidth()-r.w);
    currMap->spawnPoint.y = clamp(currMap->spawnPoint.y,0,GetMapHeight()-r.h);
    if (before.x != currMap->spawnPoint.x || before.x != currMap->spawnPoint.y)
    {
        UpdateSpawnPointStr();
    }
    if (MouseClickedThisFrame(&mouseState,&mouseStateLastFrame) && PointInRect(mouseState.worldX,mouseState.worldY,r))
    {
        editor.spawnPointHeld = true;
        editor.spawnPointHeldOffset.x = mouseState.worldX - currMap->spawnPoint.x;
        editor.spawnPointHeldOffset.y = mouseState.worldY - currMap->spawnPoint.y;
    }
    if (!(mouseState.mouse.buttons & 1))
    {
        if (editor.spawnPointHeld)
        {
            UpdateSpawnPointStr();
        }
        editor.spawnPointHeld = false;
    }
    if (editor.spawnPointHeld)
    {
        currMap->spawnPoint.x = mouseState.worldX - editor.spawnPointHeldOffset.x;
        currMap->spawnPoint.y = mouseState.worldY - editor.spawnPointHeldOffset.y;
        currMap->spawnPoint.x = clamp(currMap->spawnPoint.x,0,GetMapWidth()-r.w);
        currMap->spawnPoint.y = clamp(currMap->spawnPoint.y,0,GetMapHeight()-r.h);

    }
}
void DrawEditorUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame)
{
    DrawMapHandles(mouseState);

    DrawSpawnPoint();
    DrawPanel(&editor.editorUI.saveLoad, &mouseState, 1);

    DrawPanel(&editor.editorUI.unitSelector,&mouseState,1);
    if (editor.highlightedObject)
        DrawPanel(&editor.editorUI.unitOptions,&mouseState,1);

    DrawPanel(&editor.editorUI.mapImageEditor,&mouseState,1);
    DrawUIElement(&editor.editorUI.save,editor.editorUI.save.x,editor.editorUI.save.y,&mouseState,COLOR_BG,COLOR_FRIENDLY,false);

    if (editor.editorState == EDITOR_STATE_PAINTING)
    {
        float brushSize = editor.paintSize;
        brushSize = PAINT_SIZE_MIN + (PAINT_SIZE_MAX*brushSize);


        ALLEGRO_COLOR c;
        if (editor.paintingMode == PAINTING_MODE_DRAW_FIRST_LAYER)
            c = GROUND;
        if (editor.paintingMode == PAINTING_MODE_SECOND_LAYER)
            c = GROUND_DARK;
        if (editor.paintingMode == PAINTING_MODE_ERASING)
            c = BG;
        
        al_draw_filled_circle(mouseState.screenX,mouseState.screenY,brushSize,c);
    }   

    



    if (editor.editorUI.showFileSelector)
    {
        DrawPanel(&editor.editorUI.fileSelector,&mouseState,1);
        if (editor.editorUI.showFileNamingUI)
            DrawPanel(&editor.editorUI.fileNamingUI,&mouseState,1);

    }

}
void InitFileSelector(Panel* p, Panel* fileNamingUI)
{
    ClearPanelElements(p);
    ClearPanelElements(fileNamingUI);

    UIElement back = *AddButton(p,"Back","Back",0,0,35,10,true);
    AddButton(p,"New","New",back.x,back.y+back.h+2,35,10,true);

    AddText(p,40,0,"Path","Path");

    int fileNamingH = 80;

    int buttonW = 50;

    *fileNamingUI =  CreatePanel(1,_SCREEN_SIZE/2.0f - fileNamingH/2.0f,_SCREEN_SIZE-2,fileNamingH,1,true);
    UIElement textInp = *AddTextInput(&editor.editorUI.fileNamingUI,2,2,_SCREEN_SIZE-4,20, "FileName","",100,false);
    AddButton(fileNamingUI,"Confirm","Confirm",_SCREEN_SIZE/2.0f - buttonW,textInp.y+textInp.h+5,buttonW, 20,true);
    AddButton(fileNamingUI,"Cancel","Cancel",_SCREEN_SIZE/2.0f + buttonW,textInp.y+textInp.h+5,buttonW, 20,true);



    
}
void PopulateUnitSelector(Panel* p)
{
    int x = 0; int y = 0; int w = p->w-SCROLLBARW-1;
    for (int i = 0; i < numPrefabs; i++)
    {
        int h = 16;
        UIElement* u = AddButton(p,"UnitSelector",prefabs[i]->name,x,y,w,h,true);
        ChangeButtonImage(u,prefabs[i]->spriteIndex);
        y += h;
    }
}
void InitEditorUI()
{   

    UIElement* save = InitButton(&editor.editorUI.save,"Save","Save",10,10,40,20,0);

    editor.editorUI.saveLoad = CreatePanel(1,UI_START_Y-save->y,_SCREEN_SIZE-1,_SCREEN_SIZE-UI_START_Y+save->y,1,true);
    UIElement* loc = AddButton(&editor.editorUI.saveLoad,"Location","Location",1,1,_SCREEN_SIZE-2,13,true);
    loc = AddButton(&editor.editorUI.saveLoad,"NextMap","NextMap",1,1+loc->h+2,_SCREEN_SIZE-2,13,true);
    AddCheckbox(&editor.editorUI.saveLoad,loc->x+1, loc->y + loc->h + 2,15,15,"GoShop",&editor.editorUI.goingShop);


    editor.editorUI.fileSelector = CreatePanel(10,10,_SCREEN_SIZE-20,UI_START_Y-20,1,true);
    InitFileSelector(&editor.editorUI.fileSelector, &editor.editorUI.fileNamingUI);

    int unitSelectorW = 80;
    int unitSelectorH = 80;

    editor.editorUI.unitSelector = CreatePanel(_SCREEN_SIZE-10-unitSelectorW,20,unitSelectorW,unitSelectorH,1,true);


    editor.editorUI.unitOptions = CreatePanel(save->x,save->y+save->h+10,80,80,1,true);
    UIElement* owner = AddPulldownMenu(&editor.editorUI.unitOptions,1,1,editor.editorUI.unitOptions.w-2,20,"Owner",1,2,"Friendly","Enemy");
    UIElement* setDecor = AddCheckbox(&editor.editorUI.unitOptions,owner->x, owner->y + owner->h + 10,15,15,"IsDecor",&editor.editorUI.heldObjectIsDecor);
    AddTextInput(&editor.editorUI.unitOptions,setDecor->x,setDecor->y + setDecor->h + 10,editor.editorUI.unitOptions.w-2,20, "AggroGroup","",4,true);

    int mapImageEditorH = 80;
    editor.editorUI.mapImageEditor = CreatePanel(editor.editorUI.unitSelector.x, editor.editorUI.unitSelector.y + editor.editorUI.unitSelector.h+2,editor.editorUI.unitSelector.w,mapImageEditorH,1,true);
    UIElement* draw = AddButton(&editor.editorUI.mapImageEditor,"Draw","Draw",1,1,editor.editorUI.mapImageEditor.w-2,10,true);
    UIElement* drawSecondLayer = AddButton(&editor.editorUI.mapImageEditor,"SecondLayer","SecondLayer",draw->x,draw->y+draw->h+2,editor.editorUI.mapImageEditor.w-2,draw->h,true);
    UIElement* erase = AddButton(&editor.editorUI.mapImageEditor,"Erase","Erase",drawSecondLayer->x,drawSecondLayer->y+drawSecondLayer->h+2,editor.editorUI.mapImageEditor.w-2,drawSecondLayer->h,true);
    UIElement* paintSize = AddSlider(&editor.editorUI.mapImageEditor,erase->x,erase->y+erase->h+2,erase->w,10,"PaintSize",editor.paintSize,&editor.paintSize);
    UIElement* path = AddButton(&editor.editorUI.mapImageEditor,"MapPath","MapPath",paintSize->x,paintSize->y+paintSize->h+10,paintSize->w,10,true);
 }   