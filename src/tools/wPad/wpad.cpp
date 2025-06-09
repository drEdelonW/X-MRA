#include "wpad.hpp"
#include "terminal_tools.h"
#include "webJoystick.hpp"

#include "http_socket.hpp"

void wpad() {
    webGameControllerInit();
    LOG(TEXT_BOLD "Web Joy-Pad started\n\a" TEXT_RESET);
    bool _run = false;

    sMain();

    LOG(TEXT_BOLD "Web Joy-Pad END\n\a" TEXT_RESET);
    webGameControllerDeinit();
}
