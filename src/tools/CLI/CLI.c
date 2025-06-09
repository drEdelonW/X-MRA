#include "CLI.h"
#include "CLI_tools.h"

#include "common_tools.h"
#define HOSTNAME "X-MRA01"

#define CLI_AUTOEXEC_CMD "dpad"

void startCLI(){
#if DEBUG > 0
    LOG(TEXT_BOLD TEXT_GREEN"CLI started:\n" TEXT_RESET);

#ifdef CLI_AUTOEXEC_CMD
    executeCommand(CLI_AUTOEXEC_CMD);
#endif

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


