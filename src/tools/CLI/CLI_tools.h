#pragma once
#include "types.h"

#define EndOfCmds { .name = NULL, .function = NULL }
typedef struct {
    cStrRO  name;
    Func    function;
    cStrRO  description;
} CommandCLI;
extern const CommandCLI Cmds[];

#ifdef __cplusplus
extern "C" {
#endif

    void executeCommand(cStr_p input);
    void cliHelp();

#ifdef __cplusplus
}
#endif