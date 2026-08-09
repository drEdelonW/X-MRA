#include "dpad.h"
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

typedef union {
    uint64_t val;
    uint8_t  b[8];
} kBuff_t;

#define SEQ(...) (kBuff_t){.b = {__VA_ARGS__}}

/* ----- lookup table exactly in enum order ----- */
static const kBuff_t keyTable[KEY_COUNT] = {
    [KEY_UNKNOWN] = SEQ(0x00),

    [KEY_LEFT]  = SEQ(0x1B, 0x5B, 0x44),    /* ESC [ D */
    [KEY_RIGHT] = SEQ(0x1B, 0x5B, 0x43),    /* ESC [ C */
    [KEY_UP]    = SEQ(0x1B, 0x5B, 0x41),    /* ESC [ A */
    [KEY_DOWN]  = SEQ(0x1B, 0x5B, 0x42),    /* ESC [ B */

    [KEY__LEFT] = SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x44),  /* ESC [ 1 ; 2 D */
    [KEY__RIGHT]= SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x43),  /* ESC [ 1 ; 2 C */
    [KEY__UP]   = SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x41),  /* ESC [ 1 ; 2 A */
    [KEY__DOWN] = SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x42),  /* ESC [ 1 ; 2 B */

    [KEY_INSERT]    = SEQ(0x1B, 0x5B, 0x32, 0x7E),  /* ESC [ 2 ~ */
    [KEY_DELETE]    = SEQ(0x1B, 0x5B, 0x33, 0x7E),  /* ESC [ 3 ~ */
    [KEY_PAGE_UP]   = SEQ(0x1B, 0x5B, 0x35, 0x7E),  /* ESC [ 5 ~ */
    [KEY_PAGE_DOWN] = SEQ(0x1B, 0x5B, 0x36, 0x7E),  /* ESC [ 6 ~ */
    [KEY_HOME]      = SEQ(0x1B, 0x5B, 0x48),        /* ESC [ H */
    [KEY_END]       = SEQ(0x1B, 0x5B, 0x46),        /* ESC [ F */

    [KEY_BACKSPACE] = SEQ(0x7F),    // ASCII_DEL
    [KEY_ENTER]     = SEQ(0x0A),    // ASCII_NEWLINE
    [KEY_SPACE]     = SEQ(0x20),    // ASCII_SPACE
    [KEY_TAB]       = SEQ(0x09),    // ASCII_TAB
    [KEY_ESCAPE]    = SEQ(0x1B),    // ASCII_ESCAPE
    [KEY_TILDA]     = SEQ(0x60),    /* ` */

    [KEY_F1]    = SEQ(0x1B, 0x5B, 0x31, 0x31, 0x7E),    /* ESC [ 11 ~ */
    [KEY_F2]    = SEQ(0x1B, 0x5B, 0x31, 0x32, 0x7E),    /* ESC [ 13 ~ */
    [KEY_F3]    = SEQ(0x1B, 0x5B, 0x31, 0x33, 0x7E),    /* ESC [ 14 ~ */
    [KEY_F4]    = SEQ(0x1B, 0x5B, 0x31, 0x34, 0x7E),    /* ESC [ 15 ~ */
    [KEY_F5]    = SEQ(0x1B, 0x5B, 0x31, 0x35, 0x7E),    /* ESC [ 16 ~ */
    [KEY_F6]    = SEQ(0x1B, 0x5B, 0x31, 0x37, 0x7E),    /* ESC [ 17 ~ */
    [KEY_F7]    = SEQ(0x1B, 0x5B, 0x31, 0x38, 0x7E),    /* ESC [ 18 ~ */
    [KEY_F8]    = SEQ(0x1B, 0x5B, 0x31, 0x39, 0x7E),    /* ESC [ 19 ~ */
    [KEY_F9]    = SEQ(0x1B, 0x5B, 0x32, 0x30, 0x7E),    /* ESC [ 20 ~ */
    [KEY_F10]   = SEQ(0x1B, 0x5B, 0x32, 0x31, 0x7E),    /* ESC [ 21 ~ */
    [KEY_F11]   = SEQ(0x1B, 0x5B, 0x32, 0x33, 0x7E),    /* ESC [ 22 ~ */
    [KEY_F12]   = SEQ(0x1B, 0x5B, 0x32, 0x34, 0x7E),    /* ESC [ 23 ~ */

/* 29‒38  digits '0'..'9' */
    [KEY_0] = SEQ('0'), [KEY_1] = SEQ('1'),
    [KEY_2] = SEQ('2'), [KEY_3] = SEQ('3'),
    [KEY_4] = SEQ('4'), [KEY_5] = SEQ('5'),
    [KEY_6] = SEQ('6'), [KEY_7] = SEQ('7'),
    [KEY_8] = SEQ('8'), [KEY_9] = SEQ('9'),

/* 39‒48  shifted digits ")!@#$%^&*(" */
    [KEY__0] = SEQ(')'), [KEY__1] = SEQ('!'),
    [KEY__2] = SEQ('@'), [KEY__3] = SEQ('#'),
    [KEY__4] = SEQ('$'), [KEY__5] = SEQ('%'),
    [KEY__6] = SEQ('^'), [KEY__7] = SEQ('&'),
    [KEY__8] = SEQ('*'), [KEY__9] = SEQ('('),

/* 49‒74  letters a..z (lower-case chosen as base) */
    [KEY_A] = SEQ('a'), [KEY_B] = SEQ('b'),
    [KEY_C] = SEQ('c'), [KEY_D] = SEQ('d'),
    [KEY_E] = SEQ('e'), [KEY_F] = SEQ('f'),
    [KEY_G] = SEQ('g'), [KEY_H] = SEQ('h'),
    [KEY_I] = SEQ('i'), [KEY_J] = SEQ('j'),
    [KEY_K] = SEQ('k'), [KEY_I] = SEQ('l'),
    [KEY_M] = SEQ('m'), [KEY_N] = SEQ('n'),
    [KEY_O] = SEQ('o'), [KEY_P] = SEQ('p'),
    [KEY_Q] = SEQ('q'), [KEY_R] = SEQ('r'),
    [KEY_S] = SEQ('s'), [KEY_T] = SEQ('t'),
    [KEY_U] = SEQ('u'), [KEY_V] = SEQ('v'),
    [KEY_W] = SEQ('w'), [KEY_X] = SEQ('x'),
    [KEY_Y] = SEQ('y'), [KEY_Z] = SEQ('z'),

    [KEY__INSERT]   = SEQ(0x1B, 0x5B, 0x32, 0x7E),  /* ESC [ 2 ~ */
    [KEY__DELETE]   = SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x1B),
    [KEY__PAGE_UP]  = SEQ(0x1B, 0x5B, 0x35, 0x7E),  /* ESC [ 5 ~ */
    [KEY__PAGE_DOWN]= SEQ(0x1B, 0x5B, 0x36, 0x7E),  /* ESC [ 6 ~ */
    [KEY__HOME]     = SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x48),
    [KEY__END]      = SEQ(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x46),

/* 49‒74  letters a..z (lower-case chosen as базовый) */
    [KEY__A] = SEQ('A'),[KEY__B] = SEQ('B'),
    [KEY__C] = SEQ('C'),[KEY__D] = SEQ('D'),
    [KEY__E] = SEQ('E'),[KEY__F] = SEQ('F'),
    [KEY__G] = SEQ('G'),[KEY__H] = SEQ('H'),
    [KEY__I] = SEQ('I'),[KEY__J] = SEQ('J'),
    [KEY__K] = SEQ('K'),[KEY__L] = SEQ('L'),
    [KEY__M] = SEQ('M'),[KEY__N] = SEQ('N'),
    [KEY__O] = SEQ('O'),[KEY__P] = SEQ('P'),
    [KEY__Q] = SEQ('Q'),[KEY__R] = SEQ('R'),
    [KEY__S] = SEQ('S'),[KEY__T] = SEQ('T'),
    [KEY__U] = SEQ('U'),[KEY__V] = SEQ('V'),
    [KEY__W] = SEQ('W'),[KEY__X] = SEQ('X'),
    [KEY__Y] = SEQ('Y'),[KEY__Z] = SEQ('Z'),

};

static struct timeval _tv = { .tv_usec = KEY_TIMEOUT_MS * 20 };
static kBuff_t _keyBuff;
bool isKeyPressed() {
    fd_set fds; FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    _keyBuff.val = 0;
    for (int i = 0; i < 8; i++)
        if (
            !(
                (select(STDOUT_FILENO, &fds, NULL, NULL, &_tv) > 0) &&
                (read(STDIN_FILENO, &_keyBuff.b[i], 1) == 1)
            )
        )   return i > 0;
    return true;
}

Key getKeyFromBuffer() {
    for (int k = 1; k < KEY_COUNT; ++k) {
        if (_keyBuff.val == keyTable[k].val) {
            // LOG("%s\n", getKeyName(k));
            return (Key)k;
        }
    }
    return KEY_UNKNOWN;
}

void printKeyBuf() {
    LOG("KEY_UNKNOWN: 0x%016llX\n",
        (unsigned long long)_keyBuff.val
    ); /* -> 0xDEADBEEFCAFEBAB */
}
