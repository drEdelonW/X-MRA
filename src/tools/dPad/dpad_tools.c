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

bool isKeyPressed(void) {
uint8_t *base = (uint8_t *)&_keyBuff;   /* first byte of the word */
    uint8_t *ptr  = base;
    uint8_t *end  = base + sizeof(_keyBuff);

    memset(&_keyBuff, 0, sizeof(_keyBuff));

    for (;;) {
        fd_set fds;
        struct timeval tv = {
            .tv_sec = 0,
            .tv_usec = KEY_TIMEOUT_MS * 1000
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
/*  0 KEY_UNKNOWN  */ SEQ8(0,0,0,0,0,0,0,0),
/*  1 KEY_LEFT     */ SEQ3(0x1B,0x5B,0x44),          /* ESC [ D */
/*  2 KEY_RIGHT    */ SEQ3(0x1B,0x5B,0x43),          /* ESC [ C */
/*  3 KEY_UP       */ SEQ3(0x1B,0x5B,0x41),          /* ESC [ A */
/*  4 KEY_DOWN     */ SEQ3(0x1B,0x5B,0x42),          /* ESC [ B */

/*  5 KEY_INSERT   */ SEQ4(0x1B,0x5B,0x32,0x7E),     /* ESC [ 2 ~ */
/*  6 KEY_DELETE   */ SEQ4(0x1B,0x5B,0x33,0x7E),     /* ESC [ 3 ~ */
/*  7 KEY_PAGE_UP  */ SEQ4(0x1B,0x5B,0x35,0x7E),     /* ESC [ 5 ~ */
/*  8 KEY_PAGE_DOWN*/ SEQ4(0x1B,0x5B,0x36,0x7E),     /* ESC [ 6 ~ */
/*  9 KEY_HOME     */ SEQ3(0x1B,0x5B,0x48),          /* ESC [ H */
/* 10 KEY_END      */ SEQ3(0x1B,0x5B,0x46),          /* ESC [ F */

/* 11 KEY_BACKSPACE*/ SEQ1(0x7F),
/* 12 KEY_ENTER    */ SEQ1(0x0A),
/* 13 KEY_SPACE    */ SEQ1(0x20),
/* 14 KEY_TAB      */ SEQ1(0x09),
/* 15 KEY_ESCAPE   */ SEQ1(0x1B),
/* 16 KEY_TILDA    */ SEQ1(0x60),                    /* `  */

/* 17 KEY_F1       */ SEQ5(0x1B,0x5B,0x31,0x31,0x7E),/* ESC [ 11 ~ */
/* 18 KEY_F2       */ SEQ3(0x1B,0x4F,0x51),/* ESC [ 13 ~ */
/* 19 KEY_F3       */ SEQ5(0x1B,0x5B,0x31,0x34,0x7E),/* ESC [ 14 ~ */
/* 20 KEY_F4       */ SEQ5(0x1B,0x5B,0x31,0x35,0x7E),/* ESC [ 15 ~ */
/* 21 KEY_F5       */ SEQ5(0x1B,0x5B,0x31,0x36,0x7E),/* ESC [ 16 ~ */
/* 22 KEY_F6       */ SEQ5(0x1B,0x5B,0x31,0x37,0x7E),/* ESC [ 17 ~ */
/* 23 KEY_F7       */ SEQ5(0x1B,0x5B,0x31,0x38,0x7E),/* ESC [ 18 ~ */
/* 24 KEY_F8       */ SEQ5(0x1B,0x5B,0x31,0x39,0x7E),/* ESC [ 19 ~ */
/* 25 KEY_F9       */ SEQ5(0x1B,0x5B,0x32,0x30,0x7E),/* ESC [ 20 ~ */
/* 26 KEY_F10      */ SEQ5(0x1B,0x5B,0x32,0x31,0x7E),/* ESC [ 21 ~ */
/* 27 KEY_F11      */ SEQ5(0x1B,0x5B,0x32,0x32,0x7E),/* ESC [ 22 ~ */
/* 28 KEY_F12      */ SEQ5(0x1B,0x5B,0x32,0x33,0x7E),/* ESC [ 23 ~ */

/* 29‒38  digits '0'..'9' */
             SEQ1('0'), SEQ1('1'), SEQ1('2'), SEQ1('3'), SEQ1('4'),
             SEQ1('5'), SEQ1('6'), SEQ1('7'), SEQ1('8'), SEQ1('9'),

/* 39‒48  shifted digits ")!@#$%^&*(" */
             SEQ1(')'), SEQ1('!'), SEQ1('@'), SEQ1('#'), SEQ1('$'),
             SEQ1('%'), SEQ1('^'), SEQ1('&'), SEQ1('*'), SEQ1('('),

/* 49‒74  letters a..z (lower-case chosen as базовый) */
             SEQ1('a'), SEQ1('b'), SEQ1('c'), SEQ1('d'), SEQ1('e'),
             SEQ1('f'), SEQ1('g'), SEQ1('h'), SEQ1('i'), SEQ1('j'),
             SEQ1('k'), SEQ1('l'), SEQ1('m'), SEQ1('n'), SEQ1('o'),
             SEQ1('p'), SEQ1('q'), SEQ1('r'), SEQ1('s'), SEQ1('t'),
             SEQ1('u'), SEQ1('v'), SEQ1('w'), SEQ1('x'), SEQ1('y'),
             SEQ1('z'),
/*  75 KEY__LEFT     */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x44),
/*  76 KEY__RIGHT    */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x43),
/*  77 KEY__UP       */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x41),
/*  78 KEY__DOWN     */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x42),

/*  79 KEY__INSERT   */ SEQ4(0x1B,0x5B,0x32,0x7E),     /* ESC [ 2 ~ */
/*  80 KEY__DELETE   */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x1B),
/*  81 KEY__PAGE_UP  */ SEQ4(0x1B,0x5B,0x35,0x7E),     /* ESC [ 5 ~ */
/*  82 KEY__PAGE_DOWN*/ SEQ4(0x1B,0x5B,0x36,0x7E),     /* ESC [ 6 ~ */
/*  83 KEY__HOME     */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x48),
/*  84 KEY__END      */ SEQ6(0x1B,0x5B,0x31,0x3B,0x32,0x46),
};

Key getKeyFromBuffer() {
    for (int k = 1; k < KEY_COUNT; ++k){
        if (_keyBuff == keyTable[k]) {
            return (Key)k;
        }
    }
    return KEY_UNKNOWN;
}
