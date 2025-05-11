// #include "CLI.h"
#include "CLI_tools.h"
// #include "dpad.h"
#include "common_tools.h"
#include "i2c.h"     // busScan() dumpAddr()


Command commands[] = {
    {.name = "i2cScan", .function = busScan_,    .description = "I2C Bus scan Tool"},
    // {.name = "dpad",    .function = dpad,       .description = "keyboard control Tool"},

    // {.name = "arm",     .function = cwARM},
    // {.name = "disarm",  .function = cwDISARM},
    // {.name = "imu",     .function = _IMU_Init,    .description = "init IMU"},

    // {.name = "ver",     .function = ver_info,     .description = "Show curent Version Information"},
    // {.name = "reboot",  .function = reboot,     .description = "Reboot System"},

    {.name = "help",    .function = cliHelp,    .description = "command list"},
    {.name = "?",       .function = cliHelp,    .description = "command list"},
    {NULL, NULL}  // should be last
};