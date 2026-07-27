#include "dpad.h"
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

static uint8_t bCnt = 0;

void enableRawMode(struct termios* orig) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig);
    raw = *orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void restoreTerminal(const struct termios* orig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

bool isKeyPressed() {
uint8_t *base = (uint8_t *)&_keyBuff;   /* first byte of the word */
    uint8_t *ptr = base;
    uint8_t *end = base + sizeof(_keyBuff);

    memset(&_keyBuff, 0, sizeof(_keyBuff));

    for (;;) {
        fd_set fds;
        struct timeval tv = {
            .tv_sec = 0,
            .tv_usec = KEY_TIMEOUT_MS * 20
        };

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (ready <= 0 || ptr >= end) break;

        char c;
        if (read(STDIN_FILENO, &c, 1) == 1)
            *ptr++ = (uint8_t)c;
    }

    bCnt = (size_t)(ptr - base);  /* 0…8 */
    return bCnt > 0;
}

/* ───── helpers ───── */
#define SEQ8(b0,b1,b2,b3,b4,b5,b6,b7) \
    ((uint64_t)(b0)       | ((uint64_t)(b1)<< 8) | ((uint64_t)(b2)<<16) | ((uint64_t)(b3)<<24) | \
     ((uint64_t)(b4)<<32) | ((uint64_t)(b5)<<40) | ((uint64_t)(b6)<<48) | ((uint64_t)(b7)<<56))

#define SEQ1(b0)                        SEQ8(b0,0,0,0,0,0,0,0)
#define SEQ3(b0,b1,b2)                  SEQ8(b0,b1,b2,0,0,0,0,0)
#define SEQ4(b0,b1,b2,b3)               SEQ8(b0,b1,b2,b3,0,0,0,0)
#define SEQ5(b0,b1,b2,b3,b4)            SEQ8(b0,b1,b2,b3,b4,0,0,0)
#define SEQ6(b0,b1,b2,b3,b4,b5)         SEQ8(b0,b1,b2,b3,b4,b5,0,0)

/* ───── lookup table exactly in enum order ───── */
static const uint64_t keyTable[KEY_COUNT] = {
    [KEY_UNKNOWN] = SEQ8(0,0,0,0,0,0,0,0),

    [KEY_LEFT] =    SEQ3(0x1B,0x5B,0x44),          /* ESC [ D */
    [KEY_RIGHT] =   SEQ3(0x1B,0x5B,0x43),          /* ESC [ C */
    [KEY_UP] =      SEQ3(0x1B,0x5B,0x41),          /* ESC [ A */
    [KEY_DOWN] =    SEQ3(0x1B,0x5B,0x42),          /* ESC [ B */

    [KEY__LEFT] =   SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x44),
    [KEY__RIGHT] =  SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x43),
    [KEY__UP] =     SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x41),
    [KEY__DOWN] =   SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x42),

    [KEY_INSERT] =      SEQ4(0x1B,0x5B,0x32,0x7E),     /* ESC [ 2 ~ */
    [KEY_DELETE] =      SEQ4(0x1B,0x5B,0x33,0x7E),     /* ESC [ 3 ~ */
    [KEY_PAGE_UP] =     SEQ4(0x1B,0x5B,0x35,0x7E),     /* ESC [ 5 ~ */
    [KEY_PAGE_DOWN] =   SEQ4(0x1B,0x5B,0x36,0x7E),     /* ESC [ 6 ~ */
    [KEY_HOME] =        SEQ3(0x1B,0x5B,0x48),          /* ESC [ H */
    [KEY_END] =         SEQ3(0x1B,0x5B,0x46),          /* ESC [ F */

    [KEY_BACKSPACE] =   SEQ1(0x7F),
    [KEY_ENTER] =       SEQ1(0x0A),
    [KEY_SPACE] =       SEQ1(0x20),
    [KEY_TAB] =         SEQ1(0x09),
    [KEY_ESCAPE] =      SEQ1(0x1B),
    [KEY_TILDA] =       SEQ1(0x60),                    /* `  */

    [KEY_F1] =  SEQ5(0x1B,0x5B,0x31,0x31,0x7E),/* ESC [ 11 ~ */
    // [KEY_F2] =  SEQ3(0x1B,0x4F,0x51),/* ESC [ 13 ~ */
    [KEY_F2] =  SEQ5(0x1B,0x5B,0x31,0x32,0x7E),
    [KEY_F3] =  SEQ5(0x1B,0x5B,0x31,0x33,0x7E),/* ESC [ 14 ~ */
    [KEY_F4] =  SEQ5(0x1B,0x5B,0x31,0x34,0x7E),/* ESC [ 15 ~ */
    // [KEY_F4] =  SEQ3(0x1B,0x4F,0x53),
    [KEY_F5] =  SEQ5(0x1B,0x5B,0x31,0x35,0x7E),/* ESC [ 16 ~ */
    [KEY_F6] =  SEQ5(0x1B,0x5B,0x31,0x37,0x7E),/* ESC [ 17 ~ */
    [KEY_F7] =  SEQ5(0x1B,0x5B,0x31,0x38,0x7E),/* ESC [ 18 ~ */
    [KEY_F8] =  SEQ5(0x1B,0x5B,0x31,0x39,0x7E),/* ESC [ 19 ~ */
    [KEY_F9] =  SEQ5(0x1B,0x5B,0x32,0x30,0x7E),/* ESC [ 20 ~ */
    [KEY_F10] = SEQ5(0x1B,0x5B,0x32,0x31,0x7E),/* ESC [ 21 ~ */
    [KEY_F11] = SEQ5(0x1B,0x5B,0x32,0x33,0x7E),/* ESC [ 22 ~ */
    [KEY_F12] = SEQ5(0x1B,0x5B,0x32,0x34,0x7E),/* ESC [ 23 ~ */

/* 29‒38  digits '0'..'9' */
    [KEY_0] = SEQ1('0'), [KEY_1] = SEQ1('1'),
    [KEY_2] = SEQ1('2'), [KEY_3] = SEQ1('3'),
    [KEY_4] = SEQ1('4'), [KEY_5] = SEQ1('5'),
    [KEY_6] = SEQ1('6'), [KEY_7] = SEQ1('7'),
    [KEY_8] = SEQ1('8'), [KEY_9] = SEQ1('9'),

/* 39‒48  shifted digits ")!@#$%^&*(" */
    [KEY__0] = SEQ1(')'), [KEY__1] = SEQ1('!'),
    [KEY__2] = SEQ1('@'), [KEY__3] = SEQ1('#'),
    [KEY__4] = SEQ1('$'), [KEY__5] = SEQ1('%'),
    [KEY__6] = SEQ1('^'), [KEY__7] = SEQ1('&'),
    [KEY__8] = SEQ1('*'), [KEY__9] = SEQ1('('),

/* 49‒74  letters a..z (lower-case chosen as базовый) */
    [KEY_A] = SEQ1('a'), [KEY_B] = SEQ1('b'),
    [KEY_C] = SEQ1('c'), [KEY_D] = SEQ1('d'),
    [KEY_E] = SEQ1('e'), [KEY_F] = SEQ1('f'),
    [KEY_G] = SEQ1('g'), [KEY_H] = SEQ1('h'),
    [KEY_I] = SEQ1('i'), [KEY_J] = SEQ1('j'),
    [KEY_K] = SEQ1('k'), [KEY_I] = SEQ1('l'),
    [KEY_M] = SEQ1('m'), [KEY_N] = SEQ1('n'),
    [KEY_O] = SEQ1('o'), [KEY_P] = SEQ1('p'),
    [KEY_Q] = SEQ1('q'), [KEY_R] = SEQ1('r'),
    [KEY_S] = SEQ1('s'), [KEY_T] = SEQ1('t'),
    [KEY_U] = SEQ1('u'), [KEY_V] = SEQ1('v'),
    [KEY_W] = SEQ1('w'), [KEY_X] = SEQ1('x'),
    [KEY_Y] = SEQ1('y'), [KEY_Z] = SEQ1('z'),

    [KEY__INSERT] =     SEQ4(0x1B,0x5B,0x32,0x7E),     /* ESC [ 2 ~ */
    [KEY__DELETE] =     SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x1B),
    [KEY__PAGE_UP] =    SEQ4(0x1B,0x5B,0x35,0x7E),     /* ESC [ 5 ~ */
    [KEY__PAGE_DOWN] =  SEQ4(0x1B,0x5B,0x36,0x7E),     /* ESC [ 6 ~ */
    [KEY__HOME] =       SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x48),
    [KEY__END] =        SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x46),

/* 49‒74  letters a..z (lower-case chosen as базовый) */
    [KEY__A] = SEQ1('A'), [KEY__B] = SEQ1('B'),
    [KEY__C] = SEQ1('C'), [KEY__D] = SEQ1('D'),
    [KEY__E] = SEQ1('E'), [KEY__F] = SEQ1('F'),
    [KEY__G] = SEQ1('G'), [KEY__H] = SEQ1('H'),
    [KEY__I] = SEQ1('I'), [KEY__J] = SEQ1('J'),
    [KEY__K] = SEQ1('K'), [KEY__L] = SEQ1('L'),
    [KEY__M] = SEQ1('M'), [KEY__N] = SEQ1('N'),
    [KEY__O] = SEQ1('O'), [KEY__P] = SEQ1('P'),
    [KEY__Q] = SEQ1('Q'), [KEY__R] = SEQ1('R'),
    [KEY__S] = SEQ1('S'), [KEY__T] = SEQ1('T'),
    [KEY__U] = SEQ1('U'), [KEY__V] = SEQ1('V'),
    [KEY__W] = SEQ1('W'), [KEY__X] = SEQ1('X'),
    [KEY__Y] = SEQ1('Y'), [KEY__Z] = SEQ1('Z'),

};

Key getKeyFromBuffer() {
    for (int k = 1; k < KEY_COUNT; ++k){
        if (_keyBuff == keyTable[k]) {
            // LOG("%s\n", getKeyName(k));
            return (Key)k;
        }
    }
    return KEY_UNKNOWN;
}
