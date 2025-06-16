#include "joystick.hpp"
#include <thread>

volatile GamePad gp;
std::thread jThread;
volatile bool jQuit;
volatile bool joy_echo = false;
// void (*thJoyFunc)() = NULL;
// void (*thJoyFunc)() = SDL_GCHandler;
void (*thJoyFunc)() = Web_GCHandler;

bool GCInit() {
    if (thJoyFunc){
        jQuit = false;
        jThread = std::thread(thJoyFunc);
        LOG(TEXT_BOLD "Joy-Pad started\n\a" TEXT_RESET);
        return true;
    }
    ERROR("thJoyFunc is empty!!\n");
    return false;
}


void GCDeinit() {
    jQuit = true;
    if (jThread.joinable()) {
        jThread.join();
    }

    LOG(TEXT_BOLD "Joy-Pad END\n\a" TEXT_RESET);
}