#pragma once
#include "sprite.h"
#include "rect.h"
#include "stdbool.h"


//error printf colour
#define COL_ERR  "\x1B[31m"

typedef struct ALLEGRO_FONT ALLEGRO_FONT;
typedef struct Effect Effect;
typedef struct Sound Sound;
typedef struct Text
{
    ALLEGRO_FONT* f; 
    ALLEGRO_FONT* bold;
    int x; 
    int y;
    int h; 
    ALLEGRO_COLOR color;
    int lineHeight;
} Text;

typedef struct Scrollbar
{
    float percent;
    bool clicked;
} Scrollbar;
typedef enum UIElement_Type
{
    ELEMENT_BUTTON,
    ELEMENT_SLIDER,
    ELEMENT_CHECKBOX,
    ELEMENT_TEXT,
    ELEMENT_PULLDOWN

} UIElement_Type;
typedef enum UIElement_Align
{
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} UIElement_Align;
typedef struct UI_Text
{
    char* str;
}UI_Text;
typedef struct Pulldown
{
    char** elements;
    int numElements;
    int selectedIndex;
    bool clicked; 
} Pulldown;
typedef struct Slider
{
    bool clicked; 
    float value;
}Slider;
typedef struct Checkbox
{
    bool clicked; 
    bool activated;
}Checkbox;
typedef struct Button
{
    char* description;
    bool clicked; 
    bool activated;
    int spriteIndex;
} Button;

typedef struct UIElement
{
    UIElement_Type elementType;
    int w; int h;
    void* data;
    char* name;
    UIElement_Align alignment;
    int x; int y;
    bool enabled;
} UIElement;

typedef struct Panel
{
    int spriteIndex_tabIcon;
    UIElement* elements;
    int numElements;
    int numElementsAllocated;
    int w;
    int h;
    Scrollbar scrollbar;
    int x; int y;
    int padding;
    struct Panel** tabs;    
    int numTabs;

    UIElement backButton;
    UIElement tabButton;
    struct Panel* back;
    float scrollPercent;
    bool scrollbarClicked;

} Panel;
#define SCROLLBARW 6;
typedef enum UI_AnimState
{
    UI_ANIMATE_STATIC = 0,
    UI_ANIMATE_IN,
    UI_ANIMATE_OUT
} UI_AnimState;
typedef struct UI
{
    int panel_sprite_index;
    int cursorDefaultIndex;
    int cursorCastingIndex;
    int cursorAttackIndex;
    int cursorFriendlyIndex;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* boldFont;
    ALLEGRO_FONT* tinyFont;

    bool escMenuEnabled;
    #define UI_START_Y 217
    #define UI_MAIN_MENU_W 160
    #define UI_MAIN_MENU_H 112

    Panel* currentPanel;
    Panel mainMenuPanel;
    Panel videoOptionsPanel;
    Panel audioOptionsPanel;
    Panel accessibilityOptionsPanel;
    
    UI_AnimState animatePanel;
    Panel* changingTo;
    
    float panelShownPercent;

    //Encounter selection
    Panel encounter_scroll; 
    UIElement encounter_ButtonLeft;
    UIElement encounter_ButtonConfirm;
    UIElement encounter_ButtonRight;

    UIElement choosingUnits_Back;
    UIElement choosingUnits_GO;

    Sound* uiClickedSound;

} UI;   
 
UI ui;

#define UI_PADDING 5
typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;
typedef struct Ability Ability;
void GetAbilityClickedInsideUI(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void UpdateInterface(float dt, ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseState);
bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, Rect r, bool keydown, ALLEGRO_MOUSE_STATE* mouseState);
void DrawCursor(ALLEGRO_MOUSE_STATE* mouseState, int index, bool clicked);
void LoadCursorSprite(UI* ui, int* index, char* path);
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, ALLEGRO_FONT* bold,    int x, int y, int wTextbox, int minH, ALLEGRO_COLOR color);
int GetDescriptionBoxH(char* description, int wTextbox, ALLEGRO_FONT* f, int padding);
bool IsInsideUI(int x, int y);
int GetAbilityClicked(ALLEGRO_MOUSE_STATE* mouseState,ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
Rect GetAbilityPortraitRect(int index);
void InitUI();  
void InitFonts();
void DrawMenus(ALLEGRO_MOUSE_STATE* mouseState);
void UpdateButton(int rX, int rY, UIElement* u, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void UpdateElement(Panel* p, UIElement* u, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void GetUILocation(Panel* p, UIElement* uF, int* x, int* y);
void UpdatePanel(Panel* p, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, ALLEGRO_MOUSE_STATE* mouseStateLastFrame, float dt);
bool GetButton(Panel* p, char* name);
Button* GetButtonB(Panel* p, char* name);
void DrawUIElement(UIElement* u, int x, int y, ALLEGRO_MOUSE_STATE* mouseState, bool isActive,ALLEGRO_COLOR bgColor); 
void DrawPanel(Panel* p, ALLEGRO_MOUSE_STATE* mouseState);
void DrawLevelSelect(ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void ChangeUIPanel(Panel* to);
bool DrawAbility(Ability* ability, int x, int y, ALLEGRO_COLOR color, ALLEGRO_MOUSE_STATE* mouse);
void InitButton(UIElement* u, char* name, char* description, int x, int y, int w, int h, int sprite);
void AddButton(Panel* p, char* name, char* description, int x, int y, int w, int h);
bool GetButtonIsClicked(UIElement* u);
void DrawEffectPortrait(int x, int y, Effect* e, ALLEGRO_COLOR c);
void UpdateTabButtons(Panel* p, ALLEGRO_MOUSE_STATE* mouseState, ALLEGRO_MOUSE_STATE* mouseStateLastFrame);
void ChangeButtonText(Button* b, char* newstr);
bool cb(int line_num, const char *line, int size, void *extra);
void SetOptions();
void SetUITextStr(UI_Text* t, char* str);
