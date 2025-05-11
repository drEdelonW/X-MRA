#include "dpad.h"
#include "terminal_tools.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define KEY_TIMEOUT_MS 100
static char _keyBuff[10] = {0};
static uint8_t bCnt = 0;

static void enableRawMode(struct termios* orig) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig);
    raw = *orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void restoreTerminal(const struct termios* orig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

bool isKeyPressed() {
    struct termios orig;
    enableRawMode(&orig);

    bCnt = 0;
    memset(_keyBuff, 0, sizeof(_keyBuff));

    while (1) {
        fd_set fds;
        struct timeval tv;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = KEY_TIMEOUT_MS * 1000;

        int r = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (r <= 0) break; // timeout or error

        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (bCnt < sizeof(_keyBuff)) {
                _keyBuff[bCnt++] = c;
            }
        }
    }

    restoreTerminal(&orig);
    return bCnt > 0;
}

Key getKeyFromBuffer() {
    if (_keyBuff[0] == 0x1B) {
        if (_keyBuff[1] == 0x5B) {
            if ((_keyBuff[2] >= '1') && (_keyBuff[2] <= '6')) {
                switch (_keyBuff[2]) {
                    case '1': return
                        (_keyBuff[3] == 0x7E) ? KEY_HOME :
                        ((_keyBuff[3] == '1') && (_keyBuff[4] == 0x7E)) ? KEY_F1 :
                        ((_keyBuff[3] == '2') && (_keyBuff[4] == 0x7E)) ? KEY_F2 :
                        ((_keyBuff[3] == '3') && (_keyBuff[4] == 0x7E)) ? KEY_F3 :
                        ((_keyBuff[3] == '4') && (_keyBuff[4] == 0x7E)) ? KEY_F4 :
                        ((_keyBuff[3] == '5') && (_keyBuff[4] == 0x7E)) ? KEY_F5 :
                        ((_keyBuff[3] == '6') && (_keyBuff[4] == 0x7E)) ? KEY_F6 :
                        ((_keyBuff[3] == '7') && (_keyBuff[4] == 0x7E)) ? KEY_F7 :
                        ((_keyBuff[3] == '8') && (_keyBuff[4] == 0x7E)) ? KEY_F8 :
                        ((_keyBuff[3] == '9') && (_keyBuff[4] == 0x7E)) ? KEY_F9 :
                        KEY_UNKNOWN;
                    case '2': return
                        (_keyBuff[3] == 0x7E)? KEY_INSERT :
                        ((_keyBuff[3] == '0') && (_keyBuff[4] == 0x7E)) ? KEY_F9 :
                        ((_keyBuff[3] == '1') && (_keyBuff[4] == 0x7E)) ? KEY_F10 :
                        ((_keyBuff[3] == '2') && (_keyBuff[4] == 0x7E)) ? KEY_F11 :
                        ((_keyBuff[3] == '3') && (_keyBuff[4] == 0x7E)) ? KEY_F12 :
                        KEY_UNKNOWN;
                    case '3': return (_keyBuff[3] == 0x7E) ? KEY_DELETE : KEY_UNKNOWN;
                    case '4': return (_keyBuff[3] == 0x7E) ? KEY_END : KEY_UNKNOWN;
                    case '5': return (_keyBuff[3] == 0x7E) ? KEY_PAGE_UP : KEY_UNKNOWN;
                    case '6': return (_keyBuff[3] == 0x7E) ? KEY_PAGE_DOWN : KEY_UNKNOWN;
                }
            } else {
                switch (_keyBuff[2]) {
                    case 'A': return KEY_UP;
                    case 'B': return KEY_DOWN;
                    case 'C': return KEY_RIGHT;
                    case 'D': return KEY_LEFT;
                }
            }
        } else if (_keyBuff[1] == '\0') {
            return KEY_ESCAPE;
        }
    } else {
        switch (_keyBuff[0]) {
            case 0x7F: return KEY_BACKSPACE;
            case 0x0D: return KEY_ENTER;
            case 0x20: return KEY_SPACE;
            case 0x09: return KEY_TAB;
            case '0': return KEY_0;
            case '1': return KEY_1;
            case '2': return KEY_2;
            case '3': return KEY_3;
            case '4': return KEY_4;
            case '5': return KEY_5;
            case '6': return KEY_6;
            case '7': return KEY_7;
            case '8': return KEY_8;
            case '9': return KEY_9;

            case ')': return KEY__0;
            case '!': return KEY__1;
            case '@': return KEY__2;
            case '#': return KEY__3;
            case '$': return KEY__4;
            case '%': return KEY__5;
            case '^': return KEY__6;
            case '&': return KEY__7;
            case '*': return KEY__8;
            case '(': return KEY__9;

            case 'a': case 'A': return KEY_A;
            case 'b': case 'B': return KEY_B;
            case 'c': case 'C': return KEY_C;
            case 'd': case 'D': return KEY_D;
            case 'e': case 'E': return KEY_E;
            case 'f': case 'F': return KEY_F;
            case 'g': case 'G': return KEY_G;
            case 'h': case 'H': return KEY_H;
            case 'i': case 'I': return KEY_I;
            case 'j': case 'J': return KEY_J;
            case 'k': case 'K': return KEY_K;
            case 'l': case 'L': return KEY_L;
            case 'm': case 'M': return KEY_M;
            case 'n': case 'N': return KEY_N;
            case 'o': case 'O': return KEY_O;
            case 'p': case 'P': return KEY_P;
            case 'q': case 'Q': return KEY_Q;
            case 'r': case 'R': return KEY_R;
            case 's': case 'S': return KEY_S;
            case 't': case 'T': return KEY_T;
            case 'u': case 'U': return KEY_U;
            case 'v': case 'V': return KEY_V;
            case 'w': case 'W': return KEY_W;
            case 'x': case 'X': return KEY_X;
            case 'y': case 'Y': return KEY_Y;
            case 'z': case 'Z': return KEY_Z;
        }
    }
    return KEY_UNKNOWN;
}

void dpad() {
    LOG(TEXT_BOLD "D-Pad started\n\a" TEXT_RESET);
    bool isPressed_ = false;

    while (1) {
        bool isPressed = isKeyPressed();
        if (isPressed) {
            Key lastKey = getKeyFromBuffer();

            switch (lastKey) {
                case KEY_ESCAPE:
                case KEY_Q:
                    LOG("exit\n");
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

        if ((isPressed_ == true) && (isPressed == false)) {
            // Optionally: _doStop();
        }
        isPressed_ = isPressed;
    }
}
