#pragma once

#include "sprite.h"
#include "rect.h"
#include "stdbool.h"
#include "gamestate.h"
#include "sound.h"
#include "settings.h"
#include "colors.h"

//error printf colour
#define COL_ERR  "\x1B[31m"

#define DIALOGUE_X 10 
#define DIALOGUE_W _SCREEN_SIZE - DIALOGUE_X*2
#define DIALOGUE_H 40
#define DIALOGUE_Y 10

#define NUMSPRITESTATESTOALLOC 16

#define UI_PADDING 5


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
typedef struct KeyInput
{
    int x; int y; int w; int h;
    bool allowsInteraction;
    bool clicked;
    char* text;
    KEY* mappedTo;
} KeyInput;

typedef struct TextInput
{
    int x; int y; int w; int h;
    bool allowsInteraction;
    bool clicked;
    char* text;
    int maxChars;
    bool onlyAllowNumbers;
} TextInput;


typedef struct Scrollbar
{
    float percent;
    bool clicked;
} Scrollbar;
typedef struct Chatbox
{
    char* text;
    char* displayingUpTo;

    int x; int y; int w; int h;
    bool allowsInteraction;

    float charTimer;
} Chatbox;
typedef enum UIElement_Type
{
    ELEMENT_BUTTON = 1,
    ELEMENT_SLIDER=2,
    ELEMENT_CHECKBOX=3,
    ELEMENT_TEXT=4,
    ELEMENT_KEYINPUT=5,
    ELEMENT_PULLDOWN=6,
    ELEMENT_TEXTINPUT=7

    //ELEMENT_CHATBOX

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
    float* value;
}Slider;
typedef struct Checkbox
{
    bool clicked; 
    bool* activated;
}Checkbox;
typedef struct Button
{
    char* description;
    bool clicked; 
    bool activated;
    int spriteIndex;
    bool drawLine;
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
    SoundIndex sound_clickDown_Index;
    SoundIndex sound_clickUp_Index;
    Color bgColor;

    bool isHighlighted;
    ALLEGRO_FONT* font;

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

    bool showBorder;

} Panel;
#define SCROLLBARW 6
typedef enum UI_AnimState
{
    UI_ANIMATE_STATIC = 0,
    UI_ANIMATE_IN,
    UI_ANIMATE_OUT
} UI_AnimState;
typedef enum Widget_DrawOrder
{
    DRAWORDER_BEFOREUI,
    DRAWORDER_AFTERUI,
} Widget_DrawOrder;
typedef struct PurchasingUnitUI {
    int currentIndex;
    int indexTransitioningTo;
    float transitionTimer;
    bool isTransitionOut;
    bool isTransitioning;

    GameObject** prefabs;
    int numPrefabs;

    UIElement back;
    UIElement next;
    UIElement returnButton;
    UIElement purchaseButton;

}PurchasingUnitUI;
typedef struct UI
{
    int panel_sprite_index;
    int panel_5_abilities_sprite_index;
    int health_element_sprite_index;
    int health_and_mana_element_sprite_index;
    int gold_element_sprite_index;


    int cursorDefaultIndex;
    int cursorCastingIndex;
    int cursorAttackIndex;
    int cursorFriendlyIndex;

    int augmentIconIndex;

    
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* boldFont;
    ALLEGRO_FONT* tinyFont;

    bool escMenuEnabled;
    #define UI_START_Y (_SCREEN_SIZE-39)
    #define UI_ABILITY_START_Y UI_START_Y + 4
    #define UI_MAIN_MENU_W 160
    #define UI_MAIN_MENU_H 112

    Panel* currentPanel;
    Panel mainMenuPanel;
    Panel pauseMenuPanel;
    Panel videoOptionsPanel;
    Panel audioOptionsPanel;
    Panel accessibilityOptionsPanel;
    Panel loadReplayPanel;
    Panel controlsPanel;

    UI_AnimState animatePanel;
    Panel* changingTo;
    
    float panelShownPercent;

    //Encounter selection
    Panel encounter_scroll; 
    //UIElement encounter_ButtonLeft;
    //UIElement encounter_ButtonConfirm;
    //UIElement encounter_ButtonRight;

    UIElement choosingUnits_Back;
    UIElement choosingUnits_GO;
    UIElement choosingUnits_Hire;

    int uiClickedSound_Index;
    int uiClickedUpSound_Index;


    UIElement endScreen_Back;
    UIElement endScreen_Retry;
    UIElement endScreen_SaveReplay;


    //ingame elements
    UIElement menuButton;
    UIElement nextLevelButton;

    PurchasingUnitUI purchasingUnitUI;
    

} UI;   
 
typedef struct Widget
{
    int spriteIndex;
    float rotation;
    float rotationSpeed;
    Widget_DrawOrder drawOrder;
    int x; int y;
    int desiredX; int desiredY;
    float originX; float originY;
    int id;
    float velocity;

    float timer; 
    void (*updateFunc)(struct Widget* self, float dt);
} Widget;
extern Widget* Widgets_States[NUMGAMESTATES];
extern  int numSprites_States[NUMGAMESTATES];

extern Chatbox* chatboxes;
extern int numChatboxes;

extern Chatbox* chatboxShowing;

extern UI ui;
extern char* stackDrawBuffer;

extern bool _PANEL_CLICKED_THIS_FRAME;
extern bool _TEXTINPUT_HIGHLIGHTED;

typedef struct MouseState MouseState;
typedef struct Ability Ability;
typedef struct Replay Replay;
typedef struct MouseState MouseState;

void Chatbox_NextLine();
void EndChatbox();
void DrawUIChatbox();

void UpdateLanternWidget(Widget* self, float dt);

void ActivateDebounce();
void UpdateDebounce(float dt);
bool DebounceActive();


void DisableButton(UIElement* u);
Panel CreatePanel(int x, int y, int w, int h, int padding, bool showBorder);
Widget* CreateWidget(GameState gameStateToAttach, Sprite* spr, int x, int y, float originX, float originY, Widget_DrawOrder drawOrder, int id, void* func);
void DrawMouse(MouseState* mouseState, GameObject* mousedOver);
void DrawMouseSelectBox(MouseState mouseState);
void GetAbilityClickedInsideUI(MouseState mouseState, MouseState mouseStateLastFrame);
void UpdateChatbox(float dt);
void UpdateInterface(float dt, ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseStateLastFrame);
void DrawUI(ALLEGRO_KEYBOARD_STATE* keyState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseState);
bool DrawAbilityPortraits(GameObject* selected, Ability* heldAbility, int index, Rect r, bool keydown, MouseState mouseState, ALLEGRO_COLOR* c, bool drawKey, bool ignoreStacks);
void DrawCursor(MouseState* mouseState, int index, bool clicked, ALLEGRO_COLOR color);
void LoadCursorSprite(UI* ui, int* index, char* path);
void DrawDescriptionBox(char* description, int padding, ALLEGRO_FONT* f, ALLEGRO_FONT* bold, int x, int y, int wTextbox, int minH, ALLEGRO_COLOR color, bool dither);
int GetDescriptionBoxH(char* description, int wTextbox, ALLEGRO_FONT* f, int padding);
bool IsInsideUI(int x, int y);
bool MouseInsideUI(MouseState* mouse);
int GetAbilityClicked(MouseState* mouseState,MouseState* mouseStateLastFrame);
Rect GetAbilityPortraitRect(int index);
void InitUI();  
void InitFonts();
void DrawMenus(MouseState* mouseState);
void UpdateKeyInput(int rX, int rY, int w, int h, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame);
void UpdateButton(int rX, int rY, int w, int h, UIElement* u, MouseState mouseState, MouseState mouseStateLastFrame);
bool UpdateElement(Panel* p, UIElement* u, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void GetUILocation(Panel* p, UIElement* uF, int* x, int* y, int* w, int* h, bool update);
UIElement* GetUIElement(Panel* p, char* name);
void UpdatePanel(Panel* p, MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void UpdateUI(ALLEGRO_KEYBOARD_STATE* keyState, MouseState* mouseState, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame, MouseState* mouseStateLastFrame, float dt);
bool GetButton(Panel* p, char* name);
KeyInput* GetKeyInput(Panel* p, char* name);
Button* GetButtonB(Panel* p, char* name);
void DrawUIElement(UIElement* u, int x, int y, MouseState* mouseState, Color bgColor, Color foregroundColor, bool fromPanel);
void DrawPanel(Panel* p, MouseState* mouseState, float panelShownPercent);
void DrawLevelSelect(MouseState* mouseState, MouseState* mouseStateLastFrame, int index, int offsetX, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void DrawAllLevelSelects(MouseState* mouseState, MouseState* mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void ChangeUIPanel(Panel* to);
bool DrawAbility(Ability* ability, int x, int y, ALLEGRO_COLOR color, MouseState* mouse);
UIElement* InitButton(UIElement* u, char* name, char* description, int x, int y, int w, int h, int sprite);
UIElement* AddButton(Panel* p, char* name, char* description, int x, int y, int w, int h, bool shouldDrawLine);
UIElement* AddPulldownMenu(Panel* panel, int x, int y, int w, int h, char* name, int startIndex, int numElements, ...);
UIElement* AddCheckbox(Panel* p, int x, int y, int w, int h, char* name, bool* activated);
void AddKeyInput(Panel* p, char* name, char* description, int x, int y, int w, int h, int maxchars, KEY* mapTo);
bool GetButtonIsClicked(UIElement* u);
bool DrawEffectPortrait(int x, int y, Effect* e, ALLEGRO_COLOR c,MouseState* mouseState);
void UpdateTabButtons(Panel* p, MouseState* mouseState, MouseState* mouseStateLastFrame);
void ChangeButtonText(Button* b, char* newstr);
bool cb(int line_num, const char *line, int size, void *extra);
void SetOptions();
void SetUITextStr(UI_Text* t, char* str);
void AddText(Panel* p, int x, int y, char* name, char* description);
UIElement* AddTextInput(Panel* p, int x, int y, int w, int h,char* name, char* description, int maxChars, bool onlyAllowNumbers);
UI_Text* GetUIText(Panel* p, char* name);
void DrawUnitChoiceUI(MouseState* mouseState, MouseState* mouseStateLastFrame);
void DrawEndScreen(MouseState* mouseState, MouseState* mouseStateLastFrame);
void DrawWidgets(GameState currentState, Widget_DrawOrder drawOrderToDraw);
void UpdateWidgets(float dt);
void UpdateWidget(Widget* w, float dt);
void ClearPanelElements(Panel* p);
void GenerateFileListButtons(char* path, Panel* p);
void DrawReplayUI(Replay* r, MouseState* mouseState, MouseState* mouseStateLastFrame);
void DrawButtonText(UIElement* u,int x, int y, ALLEGRO_COLOR col);
void DrawButton(UIElement* u, int x, int y, MouseState mouseState, bool isActive, ALLEGRO_COLOR bgColor, bool drawRectWhenUnselected, ALLEGRO_COLOR foregroundColor, bool fromPanel);
void ChangeButtonImage(UIElement* u, int spriteIndex);
void DrawTimer(bool enabled);
char* GetButtonText(UIElement* u);
void DrawPurchasingUnitsUI(float dt, MouseState mouseState, MouseState mouseStateLastFrame, ALLEGRO_KEYBOARD_STATE* keyStateThisFrame, ALLEGRO_KEYBOARD_STATE* keyStateLastFrame);
void InitPurchasingUnitsUI();
void ClearChatbox();
void ClearPanel(Panel* p);
void SetTextInputStr(UIElement* u, char* str);
void ClearTextInputStr(UIElement* u);

UIElement* AddSlider(Panel* p, int x, int y, int w, int h, char* name, float filled, float* v);
void SetUIElementFont(UIElement* u, ALLEGRO_FONT* f);
