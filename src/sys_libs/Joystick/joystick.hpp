#include <stdint.h>
#include "Vector3d.hpp"
#include "terminal_tools.h"

extern volatile bool joy_echo;
extern volatile bool jQuit;

#define IF_BTN_HIT(btn, action) if \
    (GCGetButton(btn)) \
    { while (GCGetButton(btn)) {} \
        {action}; \
    };

typedef enum {
    BUTTON_CROSS = 0, //0
    BUTTON_ROUND,     //1
    BUTTON_SQUARE,    //2
    BUTTON_TRIANGLE,  //3

    BUTTON_SHARE,   //4
    BUTTON_PS,      //5
    BUTTON_MENU,    //6

    BUTTON_L3,  //7
    BUTTON_R3,  //8
    BUTTON_L1,  //9
    BUTTON_R1,  //10

    BUTTON_DPAD_UP,     //11
    BUTTON_DPAD_DOWN,   //12
    BUTTON_DPAD_LEFT,   //13
    BUTTON_DPAD_RIGHT,  //14

    BUTTON_MUTE     //15
} GameControllerButton;


typedef struct {
    Vector3D left;
    Vector3D right;
    uint16_t btns;
} GamePad;
extern volatile GamePad gp;

bool GCGetButton(GameControllerButton btn);

void printAllButtons();

void GCInit();
void GCDeinit();
void SDL_GCHandler();