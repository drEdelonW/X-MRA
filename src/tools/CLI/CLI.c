#include "CLI.h"
#include "CLI_tools.h"

#define DEBUG 3
#include "common_tools.h"   // for fgets()
// #include "eth_config.h"     // for HOSTNAME
#define HOSTNAME "X-MRA01"


void startCLI(){
    // executeCommand("mems");

    for(;;) {
        char cli_buffer[128] = {0};
        LOG("%sroot@%s:/# ", TEXT_RESET, HOSTNAME);

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
}


