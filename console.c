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

Console console = {.x = 0, .y = _SCREEN_SIZE-CONSOLE_H, .w = _SCREEN_SIZE, .h = CONSOLE_H, };

void InitConsole()
{
    console.currentLine = al_ustr_new("");
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
    float y = console.y+5;

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
void DrawConsole()
{
    if (console.active)
    {
        al_draw_filled_rectangle(console.x,console.y,console.x + console.w, console.y + console.h, BG);
        //al_draw_multiline_ustr(ui.font,FRIENDLY,5,console.y+5,console.w-5,8,ALLEGRO_ALIGN_LEFT,console.currentLine);
        al_do_multiline_ustr(ui.font,
            9999999, console.currentLine,
            DrawConsoleLine,
            NULL);
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
        currGameObjRunning = players[0].selection[players[0].indexSelectedUnit];
        luaL_loadbuffer(luaState,cstr,strlen(cstr),NULL);
        lua_pcall(luaState,0,0,0);
        al_free(cstr);

        al_ustr_truncate(console.currentLine,0);

    }
}
