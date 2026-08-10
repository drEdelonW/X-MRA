#pragma once
#include "keyConst.h"
#include "terminal_tools.h"
#include <stdbool.h>
#include <stdint.h>

typedef void (*vFv)(void);
typedef vFv* vFv_p;
typedef vFv dPadBind[KEY_COUNT];

#ifdef __cplusplus
extern "C" {
#endif

    void dpad(vFv_p pfArray);
    void dPadQuit();

#ifdef __cplusplus
}
#endif