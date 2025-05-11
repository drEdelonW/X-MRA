#pragma once
// #include "BSP.h"
#include "keyConst.h"
#include "terminal_tools.h"
#include <stdbool.h>

#define KEY_TIMEOUT_MS 100

#define KEY_REPEAT_TIMOUT   (25)
// #define KEY_REPEAT_TIMOUT   (30)
// #define KEY_REPEAT_TIMOUT   (35)

extern char _keyBuff[10];

#ifdef __cplusplus
extern "C" {
#endif

void dpad();

typedef void (*KeyFunction)(void);

extern KeyFunction fArray[KEY_COUNT];

#ifdef __cplusplus
}
#endif