#pragma once

#include <string.h>
#include <stdbool.h>
#include "terminal_tools.h"
#include "types.h"

extern bool fGetEcho;

#define EndOfCmds { .name = NULL, .function = NULL }

typedef void (*Func_p)();
typedef struct {
    cStrRO  name;
    Func_p  function;
    cStrRO  description;
} CommandCLI;

extern const CommandCLI Cmds[];

#ifdef __cplusplus
extern "C" {
#endif

    void executeCommand(cStr_p input);
    void trimNewline(cStr_p str);
    void hexDump(cStrRO label, const Any_p data, size_t size);
    void cliHelp();

#ifdef __cplusplus
}
#endif