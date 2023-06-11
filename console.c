#include "console.h"
#include "colors.h"
#include "video.h"
#include "pointspace.h"
#include "sprite.h"
#include "ui.h"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_ttf.h"

#include "luafuncs.h"
#include "player.h"

Console console = {.x = 0, .y = _SCREEN_SIZE-CONSOLE_H, .w = _SCREEN_SIZE, .h = CONSOLE_H, .active = false, .lines = {0}};

void InitConsole()
{
    console.currentLine = al_ustr_new("");
}
void AddConsoleLine(char* str)
{
    if (!str) 
        return;
    int lenLine = strlen(str);

    if (console.lines[MAX_CONSOLE_LINES-1].buffer)
        free(console.lines[MAX_CONSOLE_LINES-1].buffer);

    for (int i = MAX_CONSOLE_LINES-1; i > 0; i--)
    {
        console.lines[i] = console.lines[i-1];
    }

    
    ConsoleLine* l = &console.lines[0];
    l->buffer = calloc(lenLine > MAX_CONSOLE_LINE_CHARACTERS ? MAX_CONSOLE_LINE_CHARACTERS+1 : lenLine + 1,sizeof(char));
    if (lenLine > MAX_CONSOLE_LINE_CHARACTERS)
        strncpy(l->buffer,str,MAX_CONSOLE_LINE_CHARACTERS);
    else
        strcpy(l->buffer,str);

    if (lenLine > MAX_CONSOLE_LINE_CHARACTERS)
    {
        str += MAX_CONSOLE_LINE_CHARACTERS;
        AddConsoleLine(str);
    }
    else if (strcmp(str,"") != 0)
    {
        AddConsoleLine("");
    }

    console.scroll = 0;
}
void ToggleConsole()
{
    console.active = !console.active;
    if (console.active)
    {
        RemoveCharacter();
    }
    else
    {
        al_ustr_truncate(console.currentLine,0);
    }
}

bool DrawConsoleLine(int line_num, const ALLEGRO_USTR * line, void *extra)
{
    float currentLine = 0;
    float x = 5;
    float y = console.y+console.h-32;

    ALLEGRO_USTR* buffer = al_ustr_new("");

    int height = al_get_font_line_height(ui.font)*1.4f;
    y += (line_num-1) * height;
    int point = 0;
    for (int i = 0; i < al_ustr_size(line); i++)
    {
        al_ustr_insert_chr(buffer,point,al_ustr_get(line,i));
        point++;
        if (al_get_ustr_width(ui.font,buffer) > console.w - 10)
        {
            al_draw_ustr(ui.font,FRIENDLY,x,y,0,buffer);
            y += height;
            point = 0;

            al_ustr_remove_range(buffer,0,al_ustr_size(line));
        }
    }
    al_draw_ustr(ui.font,FRIENDLY,x,y,0,buffer);

    al_ustr_free(buffer);
    return true;

}
void DrawConsoleMessages()
{
    float x = 5;
    float y = console.y+console.h-56 + console.scroll;
    int height = al_get_font_line_height(ui.font)*1.4f;
    for (int i = 0; i < MAX_CONSOLE_LINES; i++)
    {
        if (console.lines[i].buffer)
        {
            al_draw_text(ui.font,FRIENDLY,x,y,0,console.lines[i].buffer);
        }
        y -= height;
    }
}
void DrawConsole(MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame)
{
    if (console.active)
    {   
        if (KeyPressedThisFrame(ALLEGRO_KEY_UP,keyState,keyStateLastFrame))
            console.scroll += 10;
        if (KeyPressedThisFrame(ALLEGRO_KEY_DOWN,keyState,keyStateLastFrame))
            console.scroll -= 10;

        console.scroll += mouseState->mouse.w * CONSOLE_SCROLL_SENSITIVITY;
        console.scroll = clamp(console.scroll,0,CONSOLE_MAX_SCROLL);


        al_draw_filled_rectangle(console.x,console.y,console.x + console.w, console.y + console.h, BG);
        al_draw_line(console.x,console.y,console.x+console.w,console.y,FRIENDLY,1);

        al_do_multiline_ustr(ui.font,
        9999999, console.currentLine,
        DrawConsoleLine,
        NULL);

        al_draw_line(console.x,console.y+console.h-45,console.x+console.w,console.y+console.h-45,FRIENDLY,1);
        al_set_clipping_rectangle(console.x,console.y,console.w,console.h-46);
        DrawConsoleMessages();
        al_reset_clipping_rectangle();
    }
}
void AddCharacter(int keycode, int unichar)
{
    if (console.active)
    {
        char buff[5] = {0};

        al_utf8_encode(buff,unichar);
        al_ustr_append_cstr(console.currentLine,buff);

        //const char* c = al_keycode_to_name(keycode);
        //if (strlen(console.currentLine)+strlen(buff)+1 <= MAX_LINE_SIZE)
         //   strcat(console.currentLine,buff);
    }
}
void RemoveCharacter()
{
    if (console.active)
    {
        al_ustr_truncate(console.currentLine,al_ustr_length(console.currentLine)-1);
       // int len = strlen(console.currentLine);
        //if (len == 0)
          //  return;
        //console.currentLine[len-1] = '\0';
    }
}
void RunLine()
{
    if (console.active)
    {
        char* cstr =  al_cstr_dup(console.currentLine);

        AddConsoleLine(cstr);

        currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
        luaL_loadbuffer(luaState,cstr,strlen(cstr),NULL);
        lua_pcall(luaState,0,0,0);

        al_free(cstr);

        al_ustr_truncate(console.currentLine,0);

    }
}
void ConsolePrintf(char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int numChars = vsnprintf(NULL,0,fmt,args);
    
    va_end(args);

    va_start(args, fmt);
    char* buff = calloc(numChars+1,sizeof(char));
    vsnprintf(buff,numChars,fmt,args);
    
    
    AddConsoleLine(buff);
    printf("%s\n",buff);

    free(buff);
}