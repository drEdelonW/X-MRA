#include "joystick.hpp"
#include "common_tools.h"
#include <SDL2/SDL.h>

SDL_GameController* gPad = NULL;

#define J_SCALE_FACTOR      (0x7FFF)
static inline float ScaleValue(float val) { return val / J_SCALE_FACTOR; }

void SDL_GCHandler() {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        ERROR("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return;
    }

    int numJoysticks = SDL_NumJoysticks();
    INFO("Number of joysticks: %d\n", numJoysticks);

    for (int i = 0; i < numJoysticks; i++) {
        if (SDL_IsGameController(i)) {
            gPad = SDL_GameControllerOpen(i);
            if (gPad) { LOG("Game controller connected: %s\n", SDL_GameControllerName(gPad)); break; }
            else        LOG("Could not open game controller %i: %s\n", i, SDL_GetError());
        } else          LOG("Joystick %i is not a game controller.\n", i);
    }

    if (gPad == NULL) {
        ERROR("No game controller found.\n");
        return;
    }
    jQuit = false;

    INFO("GameController Started\n");
    SDL_Event event;
    while (!jQuit) {
        while (SDL_PollEvent(&event) != 0) {
            // LOG("GameController Tick 0x%X\n", event.type);
            switch (event.type) {
                case SDL_QUIT:  jQuit = true;   break;

                case SDL_CONTROLLERDEVICEADDED: {
                    int devIdx = event.cdevice.which;
                    if (!(gPad) &&
                        (SDL_IsGameController(devIdx)) &&
                        (gPad = SDL_GameControllerOpen(devIdx))
                    )   WARNING(
                            "Game controller connected: %s\n",
                            SDL_GameControllerName(gPad)
                        );
                } break;
                case SDL_CONTROLLERDEVICEREMOVED: {
                    SDL_JoystickID removed_id = event.cdevice.which;
                    if ((gPad) &&
                        (
                            SDL_JoystickInstanceID(
                                SDL_GameControllerGetJoystick(gPad)
                            ) == removed_id
                        )
                    ) {
                        WARNING("Game controller disconnected\n");
                        SDL_GameControllerClose(gPad);
                        gPad = nullptr;
                    }
                } break;

                case SDL_CONTROLLERAXISMOTION: {
                    float tVal = ScaleValue(event.caxis.value);
                    switch (event.caxis.axis) {
                        case SDL_CONTROLLER_AXIS_LEFTX:         gp.left.x =  tVal;  break;
                        case SDL_CONTROLLER_AXIS_LEFTY:         gp.left.y =  tVal;  break;
                        case SDL_CONTROLLER_AXIS_RIGHTX:        gp.right.x = tVal;  break;
                        case SDL_CONTROLLER_AXIS_RIGHTY:        gp.right.y = tVal;  break;
                        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:   gp.left.z =  tVal;  break;
                        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:  gp.right.z = tVal;  break;
                        default: WARNING("unhandled AXIS [0x%X]\n", event.caxis.axis);   break;
                    }
                } break;


                case SDL_CONTROLLERBUTTONDOWN:
                case SDL_CONTROLLERBUTTONUP: {
                    if (event.jbutton.button < 16) {
                        gp.btns =
                            (event.type == SDL_CONTROLLERBUTTONDOWN) ?
                                (gp.btns | ( (uint16_t)1 << event.jbutton.button)):
                                (gp.btns & (~(uint16_t)1 << event.jbutton.button));
#if 0
                        LOG(WORD_TO_BINARY_PATTERN "\n",
                            WORD_TO_BINARY(gp.btns)
                        );
#endif
                        printAllButtons();
                    }
                } break;

                default: break;
            }
        }
        usleep(1000);
        //TODO: remove
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    INFO("GameController ENDED\n");
}