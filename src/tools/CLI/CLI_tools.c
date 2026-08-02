#include "CLI.h"
#include "CLI_tools.h"

void cliHelp() {
    LOG("Command list:\n");
    for (int i = 0; Cmds[i].name; i++) {
        LOG("- %s", Cmds[i].name);
        if (Cmds[i].description)
            LOG(":\t%s", Cmds[i].description);
        LOG("\n");
    }
}

void hexDump(cStringRO label, const Any_p data, size_t size) {
    const uint8_p bytes = data;
    LOG("%s (%zu bytes):\n", label, size);
    for (size_t i = 0; i < size; i += 16) {
        LOG("  %04zx  ", i);
        for (int j = 0; j < 16; ++j) {
            if ((i + j) < size)     LOG("%02X ", bytes[i + j]);
            else                    LOG("   ");
        }
        LOG(" ");
        for (int j = 0; j < 16; ++j) {
            if ((i + j) < size)
                LOG("%c",
                    isPrintable(bytes[i + j]) ?
                        bytes[i + j] : '.'
                );
        }
        LOG("\n");
    }
}

void trimNewline(cStr_p str) {
    int len = strlen(str);
    while (
        (len > 0) &&
        isLineEnd(str[len - 1])
    ) str[--len] = ASCII_NULL;
}

void executeCommand(cStr_p input) {
    for (int i = 0; Cmds[i].name; i++) {
        if (strcmp(Cmds[i].name, input) == 0) {
            if (Cmds[i].function)   Cmds[i].function();
            else                    LOG("\n!!!ACHTUNG!!!\n");

            return;
        }
    }
    LOG("\n \"%s\": command not found\n", input);
    return;
}
