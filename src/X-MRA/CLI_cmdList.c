// #include "CLI.h"
#include "CLI_tools.h"
#include "common_tools.h"

#include "dpad.h"
#include "jpad.hpp"
#include "i2c.h"     // busScan() dumpAddr()

void busScan_() {
    busScan();
}

void busD4_() {
    dumpAddr(0x40);
}

void ver_info_() {
    ver_info();
}

Command commands[] = {
    {.name = "i2cScan", .function = busScan_,    .description = "I2C Bus scan Tool"},
    {.name = "i2cD4",   .function = busD4_,      .description = "I2C Bus dump 0x40 device"},
    {.name = "dpad",    .function = dpad,       .description = "keyboard control Tool"},
    {.name = "jpad",    .function = jpad,       .description = "GamePad control Tool"},

    // {.name = "arm",     .function = cwARM},
    // {.name = "disarm",  .function = cwDISARM},
    // {.name = "imu",     .function = _IMU_Init,    .description = "init IMU"},

    {.name = "ver",     .function = ver_info_,     .description = "Show curent Version Information"},
    // {.name = "reboot",  .function = reboot,     .description = "Reboot System"},

    {.name = "help",    .function = cliHelp,    .description = "command list"},
    {.name = "?",       .function = cliHelp,    .description = "command list"},
    {NULL, NULL}  // should be last
};