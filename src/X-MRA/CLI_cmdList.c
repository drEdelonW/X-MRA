// #include "CLI.h"
#include "CLI_tools.h"
#include "common_tools.h"
#include <stdlib.h>

#include "i2c.h"     // busScan() dumpAddr()
#include "dpad.h"
#include "jpad.hpp"
#include "robot_spec_cwrap.h"


void busScan_() {
    busScan();
}

void busD4_() {
    dumpAddr(0x40);
}
void ARM_() {
    xArm();
}
void DISARM_() {
    xDisarm();
}

void test();
void test_() {
    test();
}

void ver_info_() {
    ver_info();
}
void exit_() {
    exit(0);
}

Command commands[] = {
    {.name = "i2cScan", .function = busScan_,   .description = "I2C Bus scan Tool"},
    {.name = "i2cD4",   .function = busD4_,     .description = "I2C Bus dump 0x40 device"},
    {.name = "dpad",    .function = dpad,       .description = "keyboard control Tool"},
    {.name = "jpad",    .function = jpad,       .description = "SDL GamePad control Tool"},
    // {.name = "wpad",    .function = wpad,       .description = "Web GamePad control Tool"},

    {.name = "arm",     .function = ARM_},
    {.name = "disarm",  .function = DISARM_},
    {.name = "test",    .function = test_},
    // {.name = "imu",     .function = _IMU_Init,    .description = "init IMU"},

    {.name = "ver",     .function = ver_info_,  .description = "Show curent Version Information"},
    // {.name = "reboot",  .function = reboot,     .description = "Reboot System"},
    // {.name = "exit",  .function = exit_,     .description = "Quit app"},    // risk LOGOUT from ssh  =)
    {.name = "quit",  .function = exit_,     .description = "Quit app"},

    {.name = "help",    .function = cliHelp,    .description = "command list"},
    {.name = "?",       .function = cliHelp,    .description = "command list"},
    {NULL, NULL}  // should be last
};