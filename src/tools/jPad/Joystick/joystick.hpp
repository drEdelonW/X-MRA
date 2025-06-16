#include <stdint.h>
#include "Vector3d.hpp"
#include "terminal_tools.h"

extern void (*thJoyFunc)();
extern volatile bool joy_echo;
extern volatile bool jQuit;

#define IF_BTN_HIT(btn, action) if \
    (GCGetButton(btn)) \
    { while (GCGetButton(btn)) {} \
        {action}; \
    };

typedef enum {
    BUTTON_CROSS = 0,      /* net 0  */
    BUTTON_ROUND,          /* net 1  */
    BUTTON_SQUARE,         /* net 2  */
    BUTTON_TRIANGLE,       /* net 3  */

    BUTTON_SHARE,          /* net 8  */
    BUTTON_PS,             /* not on wire */
    BUTTON_MENU,           /* net 9  */

    BUTTON_L3,             /* net 10 */
    BUTTON_R3,             /* net 11 */

    /* BUTTON_L2,  BUTTON_R2,          net 6 / 7  (currently unused) */
    BUTTON_L1,             /* net 4  */
    BUTTON_R1,             /* net 5  */

    BUTTON_DPAD_UP,        /* net 12 */
    BUTTON_DPAD_DOWN,      /* net 13 */
    BUTTON_DPAD_LEFT,      /* net 14 */
    BUTTON_DPAD_RIGHT,     /* net 15 */

    BUTTON_MUTE,           /* not on wire */
    BUTTON_MAX
} GameControllerButton;
    // see SDL_GamepadButton externals/SDL/include/SDL3/SDL_gamepad.h:99


typedef struct {
    Vector3D left;
    Vector3D right;
    uint16_t btns;
} GamePad;
extern volatile GamePad gp;

bool GCGetButton(GameControllerButton btn);

void printAllButtons();

bool GCInit();
void GCDeinit();
void SDL_GCHandler();
void Web_GCHandler();