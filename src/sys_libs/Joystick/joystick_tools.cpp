#include "joystick.hpp"
#include <thread>

volatile bool joy_echo = false;

void printButtonState(
    SDL_GameControllerButton button,
    const char* name,
    SDL_GameController *controller) {
        printf("%s [%s]\n", name,
            (SDL_GameControllerGetButton(controller, button))?
             "pressed" : "released"
            );
    // if (SDL_GameControllerGetButton(controller, button)) {
    //     printf("%s pressed\n", name);
    // } else {
    //     printf("%s released\n", name);
    // }
}

void printAllButtonState() {
    printButtonState(SDL_CONTROLLER_BUTTON_A, "A", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_B, "B", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_X, "X", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_Y, "Y", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_DPAD_UP, "DPAD UP", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_DPAD_DOWN, "DPAD DOWN", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_DPAD_LEFT, "DPAD LEFT", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "DPAD RIGHT", gPad);
    printButtonState(SDL_CONTROLLER_BUTTON_PADDLE1, "LB", gPad); /* Xbox Elite paddle P1 (upper left, facing the back) */
    printButtonState(SDL_CONTROLLER_BUTTON_PADDLE2, "RB", gPad);  /* Xbox Elite paddle P3 (upper right, facing the back) */
    printButtonState(SDL_CONTROLLER_BUTTON_PADDLE3, "LT",  gPad);/* Xbox Elite paddle P2 (lower left, facing the back) */
    printButtonState(SDL_CONTROLLER_BUTTON_PADDLE4, "RT", gPad); /* Xbox Elite paddle P4 (lower right, facing the back) */
}

void printAllButtons() {
    if (!joy_echo) { return; }
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++){
        bool pressed = SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)i);
        printf((pressed)? "[%i]":" %i ", i);
    }
    printf("\n");
}
