#include "dpad.h"

#include <termios.h>
#include <unistd.h>

void restoreTerminal(const struct termios* orig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

static void _enableRawMode(struct termios* orig) {
    tcgetattr(STDIN_FILENO, orig);

    struct termios raw = *orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    restoreTerminal(&raw);
}

// private Fn-s
bool isKeyPressed();
Key getKeyFromBuffer();
void printKeyBuf();

void dpad(vFv_p pfArray) {
    if (pfArray) {
        if (pfArray[KEY_UNKNOWN])
            pfArray[KEY_UNKNOWN]();

        LOG(TEXT_BOLD "D-Pad started\n\a" TEXT_RESET);
        struct termios orig;
        _enableRawMode(&orig);
        while (1)
            if (isKeyPressed())
                switch (getKeyFromBuffer()) {
                    // case KEY_Q:
                    case KEY_ESCAPE: {
                        LOG("exit\n");
                        restoreTerminal(&orig);
                        return;
                    }

                    case KEY_UNKNOWN: printKeyBuf(); break;

                    default: {
                        Key lastKey = getKeyFromBuffer();
                        if (pfArray[lastKey]) { pfArray[lastKey](); }
                        else  LOG("[%s]\n", getKeyName(lastKey));
                    } break;
                }

    }
}
