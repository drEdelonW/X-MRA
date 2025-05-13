#include "dpad.h"
#include <termios.h>

char _keyBuff[10] = {0};
bool isKeyPressed();
Key getKeyFromBuffer();

void enableRawMode(struct termios* orig);
void restoreTerminal(const struct termios* orig);

void dpad() {
    LOG(TEXT_BOLD "D-Pad started\n\a" TEXT_RESET);
    // bool isPressed_ = false;
    struct termios orig;
    enableRawMode(&orig);

    while (1) {
        bool isPressed = isKeyPressed();
        if (isPressed) {
            Key lastKey = getKeyFromBuffer();
            // LOG("%s\n", getKeyName(lastKey));
            switch (lastKey) {
                case KEY_ESCAPE:
                // case KEY_Q:
                    LOG("exit\n");
                    restoreTerminal(&orig);
                    return;

                case KEY_UNKNOWN:
                    for (int i = 0; _keyBuff[i]; i++)
                        LOG("0x%.2X ", _keyBuff[i]);
                    LOG("\n");
                    break;

                default:
                    if (fArray[lastKey] != NULL) {
                        fArray[lastKey]();
                    }
                    break;
            }
        }
        // else{LOG("not\n")}

        // if ((isPressed_ == true) && (isPressed == false)) {
        //     // Optionally: _doStop();
        // }
        // isPressed_ = isPressed;
    }
}
