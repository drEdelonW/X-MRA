#include "terminal_tools.h"

typedef enum {
    BUTTON_CROSS = 0,
    BUTTON_ROUND,
    BUTTON_SQUARE,
    BUTTON_TRIANGLE,
    BUTTON_SHARE,
    BUTTON_PS,
    BUTTON_MENU,
    BUTTON_L3,
    BUTTON_R3,
    BUTTON_L1,
    BUTTON_R1,
    BUTTON_DPAD_UP,
    BUTTON_DPAD_DOWN,
    BUTTON_DPAD_LEFT,
    BUTTON_DPAD_RIGHT,
    BUTTON_LAST // num of buttons
} GameControllerButton;

typedef struct {
    float x;
    float y;
    float z;
} jAnalog;

extern volatile jAnalog jLeft;
extern volatile jAnalog jRight;
extern volatile bool btns[BUTTON_LAST];

extern volatile bool movement_allowed;

int webGameControllerInit();
int webGameControllerDeinit();
void webGameControllerHandler();