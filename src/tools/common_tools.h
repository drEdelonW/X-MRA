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

#define BYTE_TO_BINARY_PATTERN \
        "[0b" \
        TEXT_BOLD  "%c%c%c%c" \
        TEXT_RESET "%c%c%c%c" \
        "]"
#define BYTE_TO_BINARY(byte)  \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x08 ? '1' : '0'), \
    ((byte) & 0x04 ? '1' : '0'), \
    ((byte) & 0x02 ? '1' : '0'), \
    ((byte) & 0x01 ? '1' : '0')

#define WORD_TO_BINARY_PATTERN \
        "[0b" \
        TEXT_BOLD  "%c%c%c%c" \
        TEXT_RESET "%c%c%c%c" \
        TEXT_BOLD  "%c%c%c%c" \
        TEXT_RESET "%c%c%c%c" \
        "]"
#define WORD_TO_BINARY(word)  \
    ((word) & 0x8000 ? '1' : '0'), \
    ((word) & 0x4000 ? '1' : '0'), \
    ((word) & 0x2000 ? '1' : '0'), \
    ((word) & 0x1000 ? '1' : '0'), \
    ((word) & 0x0800 ? '1' : '0'), \
    ((word) & 0x0400 ? '1' : '0'), \
    ((word) & 0x0200 ? '1' : '0'), \
    ((word) & 0x0100 ? '1' : '0'), \
    ((word) & 0x0080 ? '1' : '0'), \
    ((word) & 0x0040 ? '1' : '0'), \
    ((word) & 0x0020 ? '1' : '0'), \
    ((word) & 0x0010 ? '1' : '0'), \
    ((word) & 0x0008 ? '1' : '0'), \
    ((word) & 0x0004 ? '1' : '0'), \
    ((word) & 0x0002 ? '1' : '0'), \
    ((word) & 0x0001 ? '1' : '0')

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    inline void ver_info() {
        LOG("Branch: %s\n" TEXT_RESET, TEXT_GREEN TEXT_BOLD GIT_BRANCH);
        LOG("Commit: %s\n" TEXT_RESET, TEXT_BOLD GIT_COMMIT_HASH);
        LOG("Compile date: %s  %s\n" TEXT_RESET, TEXT_BOLD __DATE__, __TIME__);
    }

#ifdef __cplusplus
}
#endif