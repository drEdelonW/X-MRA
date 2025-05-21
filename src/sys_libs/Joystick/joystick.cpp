#include "joystick.hpp"


SDL_GameController *gPad = NULL;

#define J_SCALE_FACTOR  (0x7FFF)
#define SCALE_VALUE(val) ((float)val/J_SCALE_FACTOR)

volatile float x = 0.0f;
volatile float y = 0.0f;
volatile float lx = 0.0f;
volatile float ly = 0.0f;
volatile float lz = 0.0f;
volatile float rx = 0.0f;
volatile float ry = 0.0f;
volatile float rz = 0.0f;
volatile bool movement_allowed = false;


bool GCGetButton(GameControllerButton btn){
    return SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)btn);
}

float GCgetAxis(SDL_GameControllerAxis axis) {
    return SDL_GameControllerGetAxis(gPad, axis) / 32768.0f;
}

void GameControllerHandler() {
    printf("GameController Started\n");
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            // printf("GameController Tick 0x%X\n", event.type);
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
                printAllButtons();
                // printAllButtonState();
            } else if (event.type == SDL_CONTROLLERAXISMOTION) {
                switch (event.caxis.axis) {
                case SDL_CONTROLLER_AXIS_RIGHTX: {
                    rx = SCALE_VALUE(event.caxis.value);
                    if (movement_allowed) {
                        x = rx;
                        // printf("Right Stick X: %+.3f Y: %+.3f\n", x, y);
                    }
                } break;
                case SDL_CONTROLLER_AXIS_RIGHTY: {
                    ry = SCALE_VALUE(event.caxis.value);
                    if (movement_allowed) {
                        y = ry;
                        // printf("Right Stick X: %+.3f Y: %+.3f\n", x, y);
                    }
                } break;
                case SDL_CONTROLLER_AXIS_LEFTX: {
                    lx = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_LEFTY: {
                    ly = SCALE_VALUE(event.caxis.value);
                } break;
                case SDL_CONTROLLER_AXIS_TRIGGERLEFT: {
                    lz = SCALE_VALUE(event.caxis.value);
                    movement_allowed = lz > 0.5f;
                } break;
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: {
                    rz = SCALE_VALUE(event.caxis.value);
                } break;
                default:
                    printf("unhandled AXIS [0x%X]\n", event.caxis.axis);
                    break;
                }
                // if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
                //     // rx = SCALE_VALUE(event.caxis.value);
                //     // if (movement_allowed) {
                //     //     x = rx;
                //     //     // printf("Right Stick X: %+.3f Y: %+.3f\n", x, y);
                //     // }
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
                //     // ry = SCALE_VALUE(event.caxis.value);
                //     // if (movement_allowed) {
                //     //     y = ry;
                //     //     // printf("Right Stick X: %+.3f Y: %+.3f\n", x, y);
                //     // }
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                //     // lx = SCALE_VALUE(event.caxis.value);
                //     // printf("Left Stick X: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                //     // ly = SCALE_VALUE(event.caxis.value);
                //     // printf("Left Stick Y: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
                //     // movement_allowed = SCALE_VALUE(event.caxis.value) > 0.5f;
                //     // printf("movment %s \n", movement_allowed? "allowed" : "disallowed");
                //     // printf("Left Trigg: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                //     // printf("Right Trigg: %+.3f\n", SCALE_VALUE(event.caxis.value));
                // }
            }
        }
        usleep(1000);
        //TODO: remove
    }
}