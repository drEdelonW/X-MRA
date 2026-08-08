#include "dpad.h"

// private Fn-s
bool isKeyPressed();
Key getKeyFromBuffer();
void printKeyBuf();


#include <termios.h>
#include <unistd.h>
static void _enableRawMode(struct termios* orig) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig);
    raw = *orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void restoreTerminal(const struct termios* orig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

void dpad(KeyFn_p pfArray) {
    if (pfArray) {
        LOG(TEXT_BOLD "D-Pad started\n\a" TEXT_RESET);
        struct termios orig;
        tcgetattr(STDIN_FILENO, &orig);
        _enableRawMode(&orig);
        while (1) {
            if (isKeyPressed()) {
                Key lastKey = getKeyFromBuffer();
                switch (lastKey) {
                    // case KEY_Q:
                    case KEY_ESCAPE: {
                        LOG("exit\n");
                        restoreTerminal(&orig);
                        return;
                    }

                    case KEY_UNKNOWN: printKeyBuf(); break;

                    default: {
                        if (pfArray[lastKey]) { pfArray[lastKey](); }
                        else  LOG("[%s]\n", getKeyName(lastKey));
                    } break;
                }
            }
        }
    }
}
