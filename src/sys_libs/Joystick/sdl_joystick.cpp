#include "joystick.hpp"
#include "common_tools.h"
#include <SDL2/SDL.h>

SDL_GameController *gPad = NULL;
volatile bool joy_echo = true;

#define J_SCALE_FACTOR  (0x7FFF)
#define SCALE_VALUE(val) ((float)val/J_SCALE_FACTOR)

volatile GamePad gp;

// bool GCGetButton(GameControllerButton btn){
//     return SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)btn);
// }

void printAllButtons() {
    if (!joy_echo) { return; }

    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++){
        bool pressed = GCGetButton((GameControllerButton)i);
        // bool pressed = SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)i);
        printf((pressed)? "[%i]":" %i ", i);
    }
    printf("\n");
}

void SDL_GCHandler() {
    if (SDL_Init( SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return;
    }

    int numJoysticks = SDL_NumJoysticks();
    printf("Number of joysticks: %d\n", numJoysticks);

    for (int i = 0; i < numJoysticks; i++) {
        if (SDL_IsGameController(i)) {
            gPad = SDL_GameControllerOpen(i);
            if (gPad) {
                printf("Game controller connected: %s\n", SDL_GameControllerName(gPad));
                break;
            } else {
                printf("Could not open game controller %i: %s\n", i, SDL_GetError());
            }
        } else {
            printf("Joystick %i is not a game controller.\n", i);
        }
    }

    if (gPad == NULL) {
        printf("No game controller found.\n");
//        SDL_DestroyWindow(window);
        // SDL_Quit();
        return;
    }
    // joystickThread.detach();
    jQuit = false;


    printf("GameController Started\n");
    SDL_Event event;
    while (!jQuit) {
        while (SDL_PollEvent(&event) != 0) {
            // printf("GameController Tick 0x%X\n", event.type);
            if (event.type == SDL_QUIT) {
                jQuit = true;
            } else if (event.type == SDL_CONTROLLERDEVICEADDED) {   // GamePad connect handling
                int device_index = event.cdevice.which;
                if (!gPad &&
                    SDL_IsGameController(device_index)
                ) {
                    gPad = SDL_GameControllerOpen(device_index);
                    if (gPad) {
                        printf("Game controller connected: %s\n", SDL_GameControllerName(gPad));
                    }
                }
            } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) { // GamePad disconnect handling
                SDL_JoystickID removed_id = event.cdevice.which;
                if (gPad &&
                    SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gPad)) == removed_id
                ) {
                    SDL_GameControllerClose(gPad);
                    gPad = nullptr;
                    printf("Game controller disconnected\n");
                }
            } else if (
                    (event.type == SDL_CONTROLLERBUTTONDOWN) ||
                    (event.type == SDL_CONTROLLERBUTTONUP)
                ) {
                    if (event.jbutton.button < 16) {
                        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                            gp.btns |= (  (uint16_t)1 << event.jbutton.button );
                        } else if
                           (event.type == SDL_CONTROLLERBUTTONUP) {
                            gp.btns &= (~((uint16_t)1 << event.jbutton.button));
                        }
                        // LOG(WORD_TO_BINARY_PATTERN "\n", WORD_TO_BINARY(gp.btns));
                        printAllButtons();
                    }

            } else if (event.type == SDL_CONTROLLERAXISMOTION) {
                switch (event.caxis.axis) {
                case SDL_CONTROLLER_AXIS_RIGHTX: {
                    gp.right.x = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_RIGHTY: {
                    gp.right.y = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_LEFTX: {
                    gp.left.x = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_LEFTY: {
                    gp.left.y = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_TRIGGERLEFT: {
                    gp.left.z = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: {
                    gp.right.z = SCALE_VALUE(event.caxis.value);
                } break;
                default:
                    printf("unhandled AXIS [0x%X]\n", event.caxis.axis);
                    break;
                }
                // if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
                //     // gp.right.x = SCALE_VALUE(event.caxis.value);
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
                //     // gp.right.y = SCALE_VALUE(event.caxis.value);
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                //     // gp.left.x = SCALE_VALUE(event.caxis.value);
                //     // printf("Left Stick X: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                //     // gp.left.y = SCALE_VALUE(event.caxis.value);
                //     // printf("Left Stick Y: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
                //     // printf("Left Trigg: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                //     // printf("Right Trigg: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // }
            }
        }
        usleep(1000);
        //TODO: remove
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    LOG("GameController ENDED\n");
}