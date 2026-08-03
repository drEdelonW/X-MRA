// #include "CLI.h"
#include "CLI_tools.h"
#include "common_tools.h"
#include <stdlib.h>

#include "i2c_cWrap.h"      // busScan() dumpAddr()
#include "dpad.h"           // dpad()
#include "jpad.hpp"         // jpad()
#include "X-MRA_cWrap.h"    // xArm() xDisarm()


static void _busScan() { busScan(); }
static void _busD4() {
    dumpAddr(0x40);
    dumpAddr(0x41);
    dumpAddr(0x70);
    dumpAddr(0x48);
    dumpAddr(0x68);
}
static void _ARM() { xArm(); }
static void _DISARM() { xDisarm(); }

void test();
static void _test() { test(); }

static void _ver_info() { ver_info(); }
static void _exit() { exit(0); }

const CommandCLI Cmds[] = {
    {.name = "i2cScan", .function = _busScan,   .description = "I2C Bus scan Tool"},
    {.name = "i2cD4",   .function = _busD4,     .description = "I2C Bus dump 0x40 device"},

    {.name = "dpad",    .function = dpad,       .description = "keyboard control Tool"},
    {.name = "jpad",    .function = jpad,       .description = "SDL GamePad control Tool"},
    // {.name = "wpad",    .function = wpad,       .description = "Web GamePad control Tool"},

    {.name = "arm",     .function = _ARM},
    {.name = "disarm",  .function = _DISARM},
    {.name = "test",    .function = _test},
    // {.name = "imu",     .function = _IMU_Init,    .description = "init IMU"},

    {.name = "ver",     .function = _ver_info,  .description = "Show curent Version Information"},
    // {.name = "reboot",  .function = reboot,     .description = "Reboot System"},
    // {.name = "exit",  .function = exit_,     .description = "Quit app"},    // risk LOGOUT from ssh  =)
    {.name = "quit",  .function = _exit,     .description = "Quit app"},

    {.name = "help",    .function = cliHelp,    .description = "command list"},
    {.name = "?",       .function = cliHelp,    .description = "command list"},

    EndOfCmds  // should be last
};