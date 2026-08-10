#include "dpad.h"

#include "types.h"
typedef union {
    uint64_t val;
    uint8_t  b[8];
} kBuff_t;
#define B(...) (kBuff_t){.b = {__VA_ARGS__}}
/* ----- lookup table exactly in enum order ----- */
static const kBuff_t keyTable[KEY_COUNT] = {
    [KEY_UNKNOWN] = B(0x00),

    [KEY_LEFT]  = B(0x1B, 0x5B, 0x44),  /* ESC [ D */
    [KEY_RIGHT] = B(0x1B, 0x5B, 0x43),  /* ESC [ C */
    [KEY_UP]    = B(0x1B, 0x5B, 0x41),  /* ESC [ A */
    [KEY_DOWN]  = B(0x1B, 0x5B, 0x42),  /* ESC [ B */

    [KEY__LEFT] = B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x44),  /* ESC [ 1 ; 2 D  */
    [KEY__RIGHT]= B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x43),  /* ESC [ 1 ; 2 C  */
    [KEY__UP]   = B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x41),  /* ESC [ 1 ; 2 A  */
    [KEY__DOWN] = B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x42),  /* ESC [ 1 ; 2 B  */

    [KEY_INSERT]    = B(0x1B, 0x5B, 0x32, 0x7E),  /* ESC [ 2 ~  */
    [KEY_DELETE]    = B(0x1B, 0x5B, 0x33, 0x7E),  /* ESC [ 3 ~  */
    [KEY_PAGE_UP]   = B(0x1B, 0x5B, 0x35, 0x7E),  /* ESC [ 5 ~  */
    [KEY_PAGE_DOWN] = B(0x1B, 0x5B, 0x36, 0x7E),  /* ESC [ 6 ~  */
    [KEY_HOME]      = B(0x1B, 0x5B, 0x48),        /* ESC [ H    */
    [KEY_END]       = B(0x1B, 0x5B, 0x46),        /* ESC [ F    */

    [KEY_BACKSPACE] = B(0x7F),    /* ASCII_DEL      */
    [KEY_ENTER]     = B(0x0A),    /* ASCII_NEWLINE  */
    [KEY_SPACE]     = B(0x20),    /* ASCII_SPACE    */
    [KEY_TAB]       = B(0x09),    /* ASCII_TAB      */
    [KEY_ESCAPE]    = B(0x1B),    /* ASCII_ESCAPE   */
    [KEY_TILDA]     = B(0x60),    /* ` */

    [KEY_F1]    = B(0x1B, 0x5B, 0x31, 0x31, 0x7E),    /* ESC [ 11 ~ */
    [KEY_F2]    = B(0x1B, 0x5B, 0x31, 0x32, 0x7E),    /* ESC [ 13 ~ */
    [KEY_F3]    = B(0x1B, 0x5B, 0x31, 0x33, 0x7E),    /* ESC [ 14 ~ */
    [KEY_F4]    = B(0x1B, 0x5B, 0x31, 0x34, 0x7E),    /* ESC [ 15 ~ */
    [KEY_F5]    = B(0x1B, 0x5B, 0x31, 0x35, 0x7E),    /* ESC [ 16 ~ */
    [KEY_F6]    = B(0x1B, 0x5B, 0x31, 0x37, 0x7E),    /* ESC [ 17 ~ */
    [KEY_F7]    = B(0x1B, 0x5B, 0x31, 0x38, 0x7E),    /* ESC [ 18 ~ */
    [KEY_F8]    = B(0x1B, 0x5B, 0x31, 0x39, 0x7E),    /* ESC [ 19 ~ */
    [KEY_F9]    = B(0x1B, 0x5B, 0x32, 0x30, 0x7E),    /* ESC [ 20 ~ */
    [KEY_F10]   = B(0x1B, 0x5B, 0x32, 0x31, 0x7E),    /* ESC [ 21 ~ */
    [KEY_F11]   = B(0x1B, 0x5B, 0x32, 0x33, 0x7E),    /* ESC [ 22 ~ */
    [KEY_F12]   = B(0x1B, 0x5B, 0x32, 0x34, 0x7E),    /* ESC [ 23 ~ */

/* 29‒38  digits '0'..'9' */
    [KEY_0] = B('0'), [KEY_1] = B('1'), [KEY_2] = B('2'), [KEY_3] = B('3'), [KEY_4] = B('4'),
    [KEY_5] = B('5'), [KEY_6] = B('6'), [KEY_7] = B('7'), [KEY_8] = B('8'), [KEY_9] = B('9'),

/* 39‒48  shifted digits ")!@#$%^&*(" */
    [KEY__0] = B(')'), [KEY__1] = B('!'), [KEY__2] = B('@'), [KEY__3] = B('#'), [KEY__4] = B('$'),
    [KEY__5] = B('%'), [KEY__6] = B('^'), [KEY__7] = B('&'), [KEY__8] = B('*'), [KEY__9] = B('('),

/* 49‒74  letters a..z (lower-case chosen as base) */
    [KEY_A] = B('a'), [KEY_B] = B('b'), [KEY_C] = B('c'), [KEY_D] = B('d'),
    [KEY_E] = B('e'), [KEY_F] = B('f'), [KEY_G] = B('g'), [KEY_H] = B('h'),
    [KEY_I] = B('i'), [KEY_J] = B('j'), [KEY_K] = B('k'), [KEY_L] = B('l'),
    [KEY_M] = B('m'), [KEY_N] = B('n'), [KEY_O] = B('o'), [KEY_P] = B('p'),
    [KEY_Q] = B('q'), [KEY_R] = B('r'), [KEY_S] = B('s'), [KEY_T] = B('t'),
    [KEY_U] = B('u'), [KEY_V] = B('v'), [KEY_W] = B('w'), [KEY_X] = B('x'),
    [KEY_Y] = B('y'), [KEY_Z] = B('z'),

    [KEY__INSERT]   = B(0x1B, 0x5B, 0x32, 0x7E),    /* ESC [ 2 ~    */
    [KEY__DELETE]   = B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x1B),
    [KEY__PAGE_UP]  = B(0x1B, 0x5B, 0x35, 0x7E),    /* ESC [ 5 ~    */
    [KEY__PAGE_DOWN]= B(0x1B, 0x5B, 0x36, 0x7E),    /* ESC [ 6 ~    */
    [KEY__HOME]     = B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x48),
    [KEY__END]      = B(0x1B, 0x5B, 0x31, 0x3B, 0x32, 0x46),

/* 49‒74  letters a..z (lower-case chosen as base) */
    [KEY__A] = B('A'),[KEY__B] = B('B'),[KEY__C] = B('C'),[KEY__D] = B('D'),
    [KEY__E] = B('E'),[KEY__F] = B('F'),[KEY__G] = B('G'),[KEY__H] = B('H'),
    [KEY__I] = B('I'),[KEY__J] = B('J'),[KEY__K] = B('K'),[KEY__L] = B('L'),
    [KEY__M] = B('M'),[KEY__N] = B('N'),[KEY__O] = B('O'),[KEY__P] = B('P'),
    [KEY__Q] = B('Q'),[KEY__R] = B('R'),[KEY__S] = B('S'),[KEY__T] = B('T'),
    [KEY__U] = B('U'),[KEY__V] = B('V'),[KEY__W] = B('W'),[KEY__X] = B('X'),
    [KEY__Y] = B('Y'),[KEY__Z] = B('Z'),
};

static kBuff_t _keyBuff;
Key getKeyFromBuffer() {
    for (int i = 1; i < KEY_COUNT; ++i)
        if (keyTable[i].val == _keyBuff.val)
            return (Key)i;
    return KEY_UNKNOWN;
}

#include <unistd.h>     // FD_SET() STDIN_FILENO STDOUT_FILENO
#include <sys/select.h> // select()
#define KEY_TIMEOUT_MS 0
bool isKeyPressed() {
    static struct timeval _tv = { .tv_usec = KEY_TIMEOUT_MS };
    fd_set fds; FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    _keyBuff.val = 0;
    for (int i = 0; i < 8; i++)
        if (!(
            (select(STDOUT_FILENO, &fds, NULL, NULL, &_tv) > 0) &&
            (read(STDIN_FILENO, &_keyBuff.b[i], 1) == 1)
        ))  return i > 0;
    return true;
}

#include "terminal_tools.h"
void printKeyBuf() {
    LOG("KEY_UNKNOWN: 0x%016llX\n", /* -> 0xDEADBEEFCAFEBAB */
        (unsigned long long int)_keyBuff.val
    );
}
