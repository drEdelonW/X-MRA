#include <stdlib.h>
#include <SDL2/SDL.h>

extern volatile bool joy_echo;

#define IF_BTN_HIT(btn, action) if \
    (SDL_GameControllerGetButton(gPad, toPS(btn))) \
    { while (SDL_GameControllerGetButton(gPad, toPS(btn))) {} \
        {action}; \
    };

// typedef enum {
//     SDL_CONTROLLER_AXIS_INVALID = -1,
//     SDL_CONTROLLER_AXIS_LEFTX,
//     SDL_CONTROLLER_AXIS_LEFTY,
//     SDL_CONTROLLER_AXIS_RIGHTX,
//     SDL_CONTROLLER_AXIS_RIGHTY,
//     SDL_CONTROLLER_AXIS_TRIGGERLEFT,
//     SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
//     SDL_CONTROLLER_AXIS_MAX
// } SDL_GameControllerAxis;

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
    BUTTON_DPAD_RIGHT
} GameControllerButton;


static inline SDL_GameControllerButton toPS(GameControllerButton in) {
    return (SDL_GameControllerButton)in;
}

extern volatile float x;
extern volatile float y;
extern volatile float lx;
extern volatile float ly;
extern volatile float lz;
extern volatile float rx;
extern volatile float ry;
extern volatile float rz;
extern volatile bool movement_allowed;
extern SDL_GameController *gPad;

bool GCGetButton(GameControllerButton btn);
float GCgetAxis(SDL_GameControllerAxis axis);

void printButtonState(SDL_GameControllerButton button, const char* name, SDL_GameController *controller = gPad);
void printAllButtonState();
void printAllButtons();

int GameControllerInit();
void GameControllerHandler();