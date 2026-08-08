#pragma once
#include "keyConst.h"
#include "terminal_tools.h"
#include <stdbool.h>
#include <stdint.h>

#define KEY_TIMEOUT_MS 15

#define KEY_REPEAT_TIMOUT   (25)
// #define KEY_REPEAT_TIMOUT   (30)
// #define KEY_REPEAT_TIMOUT   (35)

typedef void (*KeyFunction)(void);
typedef KeyFunction* KeyFn_p;

extern KeyFunction fArray[KEY_COUNT];

#ifdef __cplusplus
extern "C" {
#endif

    void dpad(KeyFn_p pfArray);

#ifdef __cplusplus
}
#endif