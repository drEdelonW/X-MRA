#pragma once
// #include "BSP.h"
#include "keyConst.h"
#include "terminal_tools.h"

#define KEY_REPEAT_TIMOUT   (25)
// #define KEY_REPEAT_TIMOUT   (30)
// #define KEY_REPEAT_TIMOUT   (35)

#ifdef __cplusplus
extern "C" {
#endif

void dpad();

typedef void (*KeyFunction)(void);

extern KeyFunction fArray[KEY_COUNT];

#ifdef __cplusplus
}
#endif