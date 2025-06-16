
#include "joystick.hpp"

bool GCGetButton(GameControllerButton btn){
    return gp.btns & (1<<btn);
    // return SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)btn);
}

void printAllButtons() {
    if (!joy_echo) { return; }

    for (int i = 0; i < BUTTON_MAX; i++){
        bool pressed = GCGetButton((GameControllerButton)i);
        // bool pressed = SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)i);
        printf((pressed)? "[%i]":" %i ", i);
    }
    printf("\n");
}