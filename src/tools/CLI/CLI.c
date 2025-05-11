#include "CLI.h"
#include "CLI_tools.h"

#include "common_tools.h"
#define HOSTNAME "X-MRA01"

void startCLI(){
#if DEBUG > 0
    LOG(TEXT_BOLD TEXT_GREEN"CLI started:\n" TEXT_RESET);

    // executeCommand("mems");

    for(;;) {
        char cli_buffer[128] = {0};
        LOG(TEXT_BRIGHT_GREEN "root" TEXT_RESET "@%s:/# ", HOSTNAME);

        if (fgets(BUFF_AND_SIZE(cli_buffer), stdin) != NULL) {
            if (ferror(stdin)) {
                clearerr(stdin);
            } else {
                // hexDump("cli_buffer", cli_buffer, strlen(cli_buffer));
                trimNewline(cli_buffer);
                if (cli_buffer[0]) {
                    executeCommand(cli_buffer);
                }
            }
        }

    }
#else
    printf("DEBUG unset or less than 1. CLI not starting!\n");
#endif
}


