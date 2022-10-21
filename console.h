#pragma once

#include "allegro5/utf8.h"
#include <stdbool.h>

#define MAX_CONSOLE_LINES 32
#define MAX_LINE_SIZE 512
#define LINE_SIZE 10

#define CONSOLE_H 80

typedef struct Console
{
    int x; int y;
    int w; int h;
    ALLEGRO_USTR* currentLine;

    bool active;
}Console;

extern Console console;

void ToggleConsole();
void DrawConsole();
void AddCharacter(int keycode, int unichar);
void RemoveCharacter();
void RunLine();
void InitConsole();
