#include "joystick.hpp"
#include <thread>

GamePad gp;
static std::thread _joyThr;
volatile bool jQuit = true;
volatile bool joy_echo = false;

void (*thJoyFunc)() = NULL;
// void (*thJoyFunc)() = SDL_GCHandler;
// void (*thJoyFunc)() = Web_GCHandler;

bool GCInit() {
    if (thJoyFunc){
        jQuit = false;
        _joyThr = std::thread(thJoyFunc);

        LOG(TEXT_BOLD "Joy-Pad started\n\a" TEXT_RESET);
        return true;
    } else {
        ERROR("thJoyFunc is empty!!\n");
        return false;
    }
}


void GCDeinit() {
    jQuit = true;
    if (_joyThr.joinable())
        _joyThr.join();


    LOG(TEXT_BOLD "Joy-Pad END\n\a" TEXT_RESET);
}