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
                break;
        }

        if (strstr(position,","))
            position = strstr(position,",")+1;
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
    for (int i = 1; i < strlen(position); i++)
    {
        char* c = position + i;
        if (position[i] == '"' && position[i-1] != '\\')
            numQuotes++;
        if (position[i] == ',' && numQuotes == 0)
        {
            //no quotes so move it along 2 to account for no ""
            numToMove += 2;
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
    DeleteStr(full,&position,numToMove-1);

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
            UpdateArgumentFloat(&editor.setupLines[i].line,xStr,x,true);
            char* yStr = GetPositionOfArgument(editor.setupLines[i].line,"CreateObject",3);
            UpdateArgumentFloat(&editor.setupLines[i].line,yStr,y,true);

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

        
        if ((isspace(*c) ||  *c == '\n' || c == end-1) && openBrackets == 0)
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
            
            char after[2];
            after[0] = *c;
            after[1] = '\0';


            *c = '\0';
            
            char* bufferLine = lineStart;
            lineStart = c + 1;

            *lines = realloc(*lines,numLines * sizeof(EditorLine));

            EditorLine* line = *lines;

            memset(&line[numLines-1],0,sizeof(EditorLine));

            line[numLines-1].line = calloc(strlen(bufferLine)+2,sizeof(char));

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
    //SetMap(m);
    currMap = m;

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
EditorLine* AddEditorLine(EditorLine** lines, int* numLines, char* str)
{
    *lines = realloc(*lines,((*numLines)+1)*sizeof(EditorLine));


    EditorLine* li = &(*lines)[*numLines];



    memset(li,0,sizeof(EditorLine));

    li->line = calloc(strlen(str)+1,sizeof(char));
    strcpy(li->line,str);
    (*numLines)++;

    return li;

}
void AddObjLineToFile(GameObject* g, float x, float y, OBJ_FRIENDLINESS ownedBy, float summonTime, float completionPercent)
{
    char* line = NULL; 

    char* fmt = "CreateObject(\"%s\",%.2f,%.2f,%s,%.2f)";
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


    EditorLine* e = AddEditorLine(&editor.setupLines,&editor.numSetupLines,line);
    
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
void SaveMap()
{
    SaveFunction(&currMap->lua_buffer.buffer,"setup",editor.setupLines,editor.numSetupLines);
    SaveFunction(&currMap->lua_buffer.buffer,"mapend",editor.endLines,editor.numEndLines);

    ALLEGRO_FILE* file = al_fopen(currMap->path, "w");
    if (file == NULL)
    {
        printf("Couldn't save file\n");   
        return;
    }

    al_fwrite(file,currMap->lua_buffer.buffer,strlen(currMap->lua_buffer.buffer) * sizeof(char));
    al_fclose(file);
}
void UpdateEditor(float dt,MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    if (MouseClickedThisFrame(&mouseState, &mouseStateLastFrame))
    {
        editor.heldObject = GetClicked(mouseState.worldX,mouseState.worldY);
        if (editor.heldObject)
        {
            editor.highlightedObject = editor.heldObject;  
            editor.editorUI.heldObjectIsDecor = ObjIsDecoration(editor.heldObject);
            ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex = GetPlayerOwnedBy_IncludeDecor(editor.heldObject);
        } 
    }
    if (editor.heldObject)
    {
        UpdatePosition(editor.heldObject,mouseState.worldX,mouseState.worldY);


    }

    if (!(mouseState.mouse.buttons & 1))
        editor.heldObject = NULL;

    if (mouseState.mouse.buttons & 2)
    {
        editor.heldObject = NULL;
        editor.highlightedObject = NULL;   
    }

    UpdatePanel(&editor.editorUI.saveLoad,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);
    UpdatePanel(&editor.editorUI.fileSelector,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);
    UpdatePanel(&editor.editorUI.unitSelector,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);
    UpdatePanel(&editor.editorUI.unitOptions,&mouseState,&mouseStateLastFrame,keyState, keyStateLastFrame);

    UpdateButton(editor.editorUI.save.x,editor.editorUI.save.y,&editor.editorUI.save,mouseState,mouseStateLastFrame);

    if (GetButtonIsClicked(&editor.editorUI.save))
    {
        SaveMap();
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
    if (GetButton(&editor.editorUI.fileSelector,"Back"))
    {
        editor.editorUI.showFileSelector = false;
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
                    char* newPath = calloc(strlen(editor.currentPath)+strlen(b->description)+2,sizeof(char));
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
                                        if (editor.editorUI.selectorPicked == EDITOR_FILE_NEXT_MAP)
                                        {
                                            if (editor.nextMap)
                                                free(editor.nextMap);
                                            editor.nextMap = calloc(strlen(newPath)+1,sizeof(char));
                                            strcpy(editor.nextMap,newPath);
                                            ChangeButtonText(GetButtonB(&editor.editorUI.saveLoad,"NextMap"),editor.nextMap);
                                            editor.editorUI.showFileSelector = false;


                                            bool foundFunc = false;
                                            for (int i = 0; i < editor.numEndLines; i++)
                                            {
                                                if (!editor.endLines[i].lineIsComment && strstr(editor.endLines[i].line,"ChangeMap("))
                                                {
                                                    foundFunc = true;

                                                    char* c = GetPositionOfArgument(editor.endLines[i].line,"ChangeMap",1);
                                                    UpdateArgumentStr(&editor.endLines[i].line,c,newPath,true);

                                                }
                                            }
                                            if (!foundFunc)
                                            {
                                                char* str = calloc(strlen(newPath) + strlen("   ChangeMap(\"\")\n") + 1,sizeof(char));
                                                sprintf(str,"   ChangeMap(\"%s\")\n",newPath);
                                                AddEditorLine(&editor.endLines, &editor.numEndLines, str);

                                                free(str);
                                            }
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
            GameObject* g = AddGameobject(toSpawn,GetCameraMiddleX(),GetCameraMiddleY(),SOURCE_SPAWNED_FROM_MAP);
            AddObjLineToFile(g,g->position.worldX,g->position.worldY,GetPlayerOwnedBy(g),0,0);
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
        SetDecoration(editor.highlightedObject,editor.editorUI.heldObjectIsDecor);

        ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex = GetPlayerOwnedBy_IncludeDecor(editor.highlightedObject);
    
        char* aggroGroupText = ((TextInput*)(GetUIElement(&editor.editorUI.unitOptions,"AggroGroup")->data))->text;
        if (strlen(aggroGroupText) > 0)
        {
            int aggroGroupBefore = editor.highlightedObject->aggroGroup;
            int aggroGroupAfter = atoi(aggroGroupText);

            if (aggroGroupBefore != aggroGroupAfter)
            {
                for (int i = 0; i < editor.numSetupLines; i++)
                {
                    if (editor.setupLines[i].associated == editor.highlightedObject)
                    {
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
                            
                            int numChars = snprintf(NULL,0,",%.2f)",(float)aggroGroupAfter);
                            char* args = calloc(numChars+1,sizeof(char));
                            sprintf(args,",%.2f)",(float)aggroGroupAfter);
                            InsertStr(&editor.setupLines[i].line,&posEnd,args);

                            free(args);

                            printf("%s\n",editor.setupLines[i].line);

                        }
                        break;
                    }
                }
            }

        }
        else
        {
            editor.highlightedObject->aggroGroup = 0;
            editor.highlightedObject->aggroGroupSet = 0;

        }

    }

    int owner = ((Pulldown*)(GetUIElement(&editor.editorUI.unitOptions,"Owner")->data))->selectedIndex;
    if (editor.highlightedObject)
    {
        SetOwnedBy(editor.highlightedObject,owner);
        SetDecoration(editor.highlightedObject,editor.editorUI.heldObjectIsDecor);
    }

}
void DrawEditorUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame)
{
    DrawPanel(&editor.editorUI.saveLoad, &mouseState, 1);

    if (editor.editorUI.showFileSelector)
    {
        DrawPanel(&editor.editorUI.fileSelector,&mouseState,1);
    }
    DrawPanel(&editor.editorUI.unitSelector,&mouseState,1);
    if (editor.highlightedObject)
        DrawPanel(&editor.editorUI.unitOptions,&mouseState,1);


    DrawUIElement(&editor.editorUI.save,editor.editorUI.save.x,editor.editorUI.save.y,&mouseState,COLOR_BG,COLOR_FRIENDLY);
}
void InitFileSelector(Panel* p)
{
    ClearPanelElements(p);
    AddButton(p,"Back","Back",0,0,35,10,true);
    AddText(p,40,0,"Path","Path");
}
void PopulateUnitSelector(Panel* p)
{
    int x = 0; int y = 0; int w = p->w;
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
    editor.editorUI.saveLoad = CreatePanel(1,UI_START_Y,_SCREEN_SIZE-1,_SCREEN_SIZE-UI_START_Y,1,true);
    UIElement* loc = AddButton(&editor.editorUI.saveLoad,"Location","Location",1,1,_SCREEN_SIZE-2,13,true);
    AddButton(&editor.editorUI.saveLoad,"NextMap","NextMap",1,1+loc->h+2,_SCREEN_SIZE-2,13,true);

    editor.editorUI.fileSelector = CreatePanel(10,10,_SCREEN_SIZE-20,UI_START_Y-20,1,true);
    InitFileSelector(&editor.editorUI.fileSelector);

    int unitSelectorW = 80;
    int unitSelectorH = 80;

    editor.editorUI.unitSelector = CreatePanel(_SCREEN_SIZE-1-unitSelectorW,20,unitSelectorW,unitSelectorH,1,true);

    UIElement* save = InitButton(&editor.editorUI.save,"Save","Save",10,10,40,20,0);

    editor.editorUI.unitOptions = CreatePanel(save->x,save->y+save->h+10,80,80,1,true);
    UIElement* owner = AddPulldownMenu(&editor.editorUI.unitOptions,1,1,editor.editorUI.unitOptions.w-2,20,"Owner",1,2,"Friendly","Enemy");
    UIElement* setDecor = AddCheckbox(&editor.editorUI.unitOptions,owner->x, owner->y + owner->h + 10,15,15,"IsDecor",&editor.editorUI.heldObjectIsDecor);
    AddTextInput(&editor.editorUI.unitOptions,setDecor->x,setDecor->y + setDecor->h + 10,editor.editorUI.unitOptions.w-2,20, "AggroGroup","",4,true);

}   