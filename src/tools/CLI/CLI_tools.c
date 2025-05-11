#include "CLI.h"
#include "CLI_tools.h"

void cliHelp() {
    LOG("Command list:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        LOG("- %s", commands[i].name);
        if (commands[i].description != NULL) {
            LOG(
                ":\t%s",
                commands[i].description
            );
        }
        LOG("\n");
    }
}

void hexDump(const char* label, const void* data, size_t size) {
    const unsigned char* bytes = (const unsigned char*)data;
    LOG("%s (%zu bytes):\n", label, size);

    for (size_t i = 0; i < size; i += 16) {
        LOG("  %04zx  ", i);
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < size) {
                LOG("%02X ", bytes[i + j]);
            } else {
                LOG("   ");
            }
        }
        LOG(" ");
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < size)
                LOG(
                    "%c",
                    ((bytes[i + j] >= 32) &&
                     (bytes[i + j] <= 126) ) ?
                        bytes[i + j] : '.'
                );
        }
        LOG("\n");
    }
}

void trimNewline(char* str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

void executeCommand(char *input) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(commands[i].name, input) == 0) {
            if (commands[i].function != NULL) {
                commands[i].function();
                return;
            } else {
                LOG("\n!!!ACHTUNG!!!\n");
                return;
            }
        }
    }
    LOG("\n \"%s\": command not found\n", input);
    return;
}
