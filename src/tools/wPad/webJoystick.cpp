#include "webJoystick.hpp"
#include <thread>

std::thread webJystickThread(webGameControllerHandler);

int webGameControllerInit() {
    LOG("init WebPad\n");
    return 0;
}
int webGameControllerDeinit(){
    LOG("deinit WebPad\n");
    return 0;
}
void webGameControllerHandler(){

}