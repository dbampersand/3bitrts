#pragma once
#include "sprite.h"
#include "rect.h"

typedef struct ALLEGRO_FONT ALLEGRO_FONT;
typedef struct Scrollbar
{
    float percent;
    bool clicked;
} Scrollbar;
typedef enum UIElement_Type
{
    ELEMENT_BUTTON,
    ELEMENT_SLIDER,
    ELEMENT_CHECKBOX

} UIElement_Type;
typedef enum UIElement_Align
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} UIElement_Align;
typedef struct Button
{
    char* description;
    bool clicked; 
    bool activated;
} Button;

typedef struct UIElement
{
    UIElement_Type elementType;
    bool linebreak;
    int padding;
    int lineSpacing;
    int w; int h;
    void* data;
    char* name;
    UIElement_Align alignment;
    int x;
} UIElement;

typedef struct Panel
{
    UIElement* elements;
    int numElements;
    int numElementsAllocated;
    int w;
    int h;
    float shownPercent;
    Scrollbar scrollbar;
    int x; int y;
    int padding;
} Panel;

typedef struct UI
{
    int panel_sprite_index;
    int cursorDefaultIndex;
    int cursorCastingIndex;
    int cursorAttackIndex;
    int cursorFriendlyIndex;
    ALLEGRO_FONT* font;

    bool escMenuEnabled;
    #define UI_START_Y 217
    #define UI_MAIN_MENU_W 160
    #define UI_MAIN_MENU_H 112

    Panel* currentPanel;
    Panel mainMenuPanel;

} UI;   
 
UI ui;

#define UI_PADDING 5
typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;
typedef struct Ability Ability;

void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState);
bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, Rect r, bool keydown, ALLEGRO_MOUSE_STATE* mouseState);
void DrawCursor(ALLEGRO_MOUSE_STATE* mouseState, int index, bool clicked);
void LoadCursorSprite(UI* ui, int* index, char* path);
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, int x, int y, int wTextbox);
int GetDescriptionBoxH(char* description, int wTextbox, ALLEGRO_FONT* f, int padding);
bool IsInsideUI(int x, int y);
int GetAbilityClicked(ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
Rect GetAbilityPortraitRect(int index);
void InitUI();
void DrawMenus();
void UpdateButton(int rX, int rY, UIElement* u, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void UpdateElement(Panel* p, UIElement* u, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void GetUILocation(Panel* p, UIElement* uF, int* x, int* y);
void UpdatePanel(Panel* p, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
bool GetButton(Panel* p, char* name);
