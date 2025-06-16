#include "joystick.hpp"
#include <thread>

std::thread joystickThread;
volatile bool jQuit;

void GCInit() {
    jQuit = false;
    joystickThread = std::thread(SDL_GCHandler);
    LOG(TEXT_BOLD "Joy-Pad started\n\a" TEXT_RESET);
}


void GCDeinit() {
    jQuit = true;
    if (joystickThread.joinable()) {
        joystickThread.join();
    }

    LOG(TEXT_BOLD "Joy-Pad END\n\a" TEXT_RESET);
}