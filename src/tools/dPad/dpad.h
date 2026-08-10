#pragma once
#include "keyConst.h"
#include "types.h"

typedef Func dPadBind[KEY_COUNT];

#ifdef __cplusplus
extern "C" {
#endif

    void dpad(Func_p pfArray);
    void dPadQuit();

#ifdef __cplusplus
}
#endif