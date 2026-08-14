
#include "joystick.hpp"

bool GCGetButton(GameControllerButton btn){
#if 1
    return gp.btns & (1 << btn);
#else
    return SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)btn);
#endif
}

void printAllButtons() {
    if (!joy_echo)
        return;

    for (GameControllerButton i = BUTTON_CROSS; i < BUTTON_MAX; i = (GameControllerButton)(i + 1))
        LOG((GCGetButton(i)) ?
            (TEXT_WHITE TEXT_BOLD "[%i]" TEXT_RESET) :
            " %i ",
            i
        );
    // LOG("\n");
    LOG("%c", ASCII_CARRIAGE);
}