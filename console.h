#pragma once

#include "allegro5/utf8.h"
#include <stdbool.h>

#define MAX_CONSOLE_LINES 32
#define MAX_LINE_SIZE 512
#define LINE_SIZE 10
#define CONSOLE_SCROLL_SENSITIVITY 2
#define CONSOLE_MAX_SCROLL 11 * MAX_CONSOLE_LINES

#define CONSOLE_H 80

typedef struct ConsoleLine
{
    char* buffer;
    size_t sizeBuffer;
} ConsoleLine;

typedef struct Console
{
    int x; int y;
    int w; int h;
    ALLEGRO_USTR* currentLine;

    bool active;
    ConsoleLine lines[MAX_CONSOLE_LINES];

    int scroll;

}Console;

extern Console console;
typedef struct MouseState MouseState;
typedef struct ALLEGRO_KEYBOARD_STATE ALLEGRO_KEYBOARD_STATE;

void ToggleConsole();
void DrawConsole(MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void AddCharacter(int keycode, int unichar);
void RemoveCharacter();
void RunLine();
void InitConsole();
void AddConsoleLine(char* str);
void ConsolePrintf(char* fmt, ...);
