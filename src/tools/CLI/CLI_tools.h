#pragma once

#include <string.h>
#include <stdbool.h>
#include "terminal_tools.h"


#ifdef __cplusplus
extern "C" {
#endif

extern bool fGetEcho;

typedef struct {
    const char *name;
    void (*function)();
    const char *description;
} Command;

extern Command commands[];

void executeCommand(char *input);
void trimNewline(char* str);
void hexDump(const char* label, const void* data, size_t size);
void cliHelp();

#ifdef __cplusplus
}
#endif