#pragma once
// #include "BSP.h"
#include "keyConst.h"
#include "terminal_tools.h"
#include <stdbool.h>
#include <stdint.h>
#include <termios.h>

#define KEY_TIMEOUT_MS 15

#define KEY_REPEAT_TIMOUT   (25)
// #define KEY_REPEAT_TIMOUT   (30)
// #define KEY_REPEAT_TIMOUT   (35)

// extern char _keyBuff[10];
extern uint64_t _keyBuff;

extern struct termios orig;

#ifdef __cplusplus
extern "C" {
#endif

void dpad();

void enableRawMode(struct termios* orig);
void restoreTerminal(const struct termios* orig);

typedef void (*KeyFunction)(void);

extern KeyFunction fArray[KEY_COUNT];

#ifdef __cplusplus
}
#endif