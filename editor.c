#include "editor.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "gameobject.h"
#include "luafuncs.h"
#include "helperfuncs.h"
Editor editor = {0};


//extract function setup so we can run that one line at a time and create Line objects from that
char* ExtractFunction(char* funcName, char* buffer)
{
    char* c = buffer;
    char* full = c + strlen(funcName);

    char* start;

    if (c == NULL) 
        return false;
    while (c <= buffer + strlen(buffer))
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
            while (c2-- >= buffer)
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
    for (int i = 0; i < editor.numLines; i++)
    {
        if (editor.lines[i].associated == g)
        {
            g->position.worldX = x;
            g->position.worldY = y;
            UpdateScreenPositions(g);

            //2-decimal precision for both
            int toIncreaseBy = NumDigits(x) + NumDigits(y) + 6;
            //editor.lines[i].line = realloc(editor.lines[i].line,(strlen(editor.lines[i].line)+toIncreaseBy+1)*sizeof(char));

            char* xStr = GetPositionOfArgument(editor.lines[i].line,"SetAggroGroup",2);
            UpdateArgumentFloat(&editor.lines[i].line,xStr,3);
            char* yStr = GetPositionOfArgument(editor.lines[i].line,"CreateObject",3);
            UpdateArgumentFloat(&editor.lines[i].line,yStr,4);

            printf("%s\n",editor.lines[i].line);
        }
    }
}
void RunAllLines()
{
    for (int i = 0; i < editor.numLines; i++)
    {
        int numObjsBefore = numActiveObjects;
        if (editor.lines[i].line)
            luaL_dostring(luaState,editor.lines[i].line);
        if (numActiveObjects > numObjsBefore)
        {
            editor.lines[i].associated = &objects[numActiveObjects-1];
        }
    }
}
void SplitLines(char* buffer)
{
    char* save = buffer;

    char* line = strtok_r(buffer,"\n",&save);
    editor.numLines = 0;
    do 
    {
        editor.numLines++;

        if (!editor.lines)
            editor.lines = calloc(1,sizeof(EditorLine));

        editor.lines = realloc(editor.lines,editor.numLines * sizeof(EditorLine));
        memset(&editor.lines[editor.numLines-1],0,sizeof(EditorLine));

        editor.lines[editor.numLines-1].line = calloc(strlen(line)+1,sizeof(char));
        strcpy(editor.lines[editor.numLines-1].line,line);
        
        editor.lines[editor.numLines-1].lineNumber = editor.numLines-1;   
        line = strtok_r(NULL,"\n",&save);   
    } while (line != NULL);

}

void EditorSetMap(char* path)
{
    Map* m = LoadMap(path);
    //SetMap(path);

    char* buff = ExtractFunction("setup",m->lua_buffer.buffer);
    SplitLines(buff);
    free(buff);

    RunAllLines();

    for (int i = 0; i < editor.numLines; i++)
    {
        if (editor.lines[i].associated)
        {  
            UpdatePosition(editor.lines[i].associated,4,5);
        }
    }
}   