#include "joystick.hpp"
#include <thread>

// std::thread joystickThread(GameControllerHandler);
std::thread joystickThread;

int GameControllerInit () {
     if (SDL_Init( SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
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
        return 1;
    }
    // joystickThread.detach();
    jQuit = false;
    joystickThread = std::thread(GameControllerHandler);
    return 0;
}


int GameControllerDeinit () {
    jQuit = true;
    if (joystickThread.joinable()) {
        joystickThread.join();
    }
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    return 0;
}