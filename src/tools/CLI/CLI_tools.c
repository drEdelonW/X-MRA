#include "CLI.h"
#include "CLI_tools.h"
#include "terminal_tools.h"

#include <string.h> // strlen()
static cStr_p _trimLine(cStr_p str) {
    int len = strlen(str);
    while (len--) {
        if ((str[len] == ' ') ||
            isLineEnd(str[len])
        )   str[len] = ASCII_NULL;
    }
    return str;
}

void cliHelp() {
    LOG("Command list:\n");
    for (int i = 0; Cmds[i].name; i++)
        LOG("- %s\t\t%s\n",
            Cmds[i].name,
            (Cmds[i].description)?
                Cmds[i].description : ""
        );
}

void executeCommand(cStr_p input) {
    // hexDump("input", input, strlen(input));
    _trimLine(input);
    if (input[0]) {
        for (int i = 0; Cmds[i].name; i++) {
            if (strcmp(Cmds[i].name, input) == 0) {
                if (Cmds[i].function)   Cmds[i].function(); // TODO: add handle last executed
                else    LOG("\n!!!ACHTUNG!!!\n");
                return;
            }
        }
        LOG("\n \"%s\": command not found\n", input);
    }
    return;
}
