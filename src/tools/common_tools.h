#pragma once
#include <stdio.h>
#include "terminal_tools.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CUT_MAX(v, max) {v = (v > max)? max : v; }
#define CUT_MIN(v, min) {v = (v < min)? min : v; }

#define DIMOF(original) (sizeof(original) / sizeof(original[0]))

#define BUFF_AND_SIZE(x)    x, sizeof(x)
// #define TODO(x) _Pragma(TO_STRING(message("TODO: " x)));

#define BYTE_TO_BINARY_PATTERN "[0b%c%c%c%c%c%c%c%c]"
#define BYTE_TO_BINARY(byte)  \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x08 ? '1' : '0'), \
    ((byte) & 0x04 ? '1' : '0'), \
    ((byte) & 0x02 ? '1' : '0'), \
    ((byte) & 0x01 ? '1' : '0')

#include <stdio.h>

inline void ver_info() {
    printf("Branch: %s\n" TEXT_RESET, TEXT_BOLD GIT_BRANCH);
    printf("Commit: %s\n" TEXT_RESET, TEXT_BOLD GIT_COMMIT_HASH);
    printf("Compile date: %s  %s\n" TEXT_RESET, TEXT_BOLD __DATE__, __TIME__);
}
