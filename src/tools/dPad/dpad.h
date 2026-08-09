#pragma once
#include "keyConst.h"
#include "terminal_tools.h"
#include <stdbool.h>
#include <stdint.h>

#define KEY_TIMEOUT_MS 15

#define KEY_REPEAT_TIMOUT   (25)
// #define KEY_REPEAT_TIMOUT   (30)
// #define KEY_REPEAT_TIMOUT   (35)

typedef void (*vFv)(void);
typedef vFv* vFv_p;
typedef vFv dPadBind[KEY_COUNT];

#ifdef __cplusplus
extern "C" {
#endif

    void dpad(vFv_p pfArray);

#ifdef __cplusplus
}
#endif