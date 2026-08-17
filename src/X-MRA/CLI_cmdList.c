// #include "CLI.h"
#include "CLI_tools.h"
#include "common_tools.h"
#include <stdlib.h>

#include "i2c_cWrap.h"  // busScan() dumpAddr()
static void _busD4() {
    dumpAddr(0x40); dumpAddr(0x41); dumpAddr(0x70); // PCA
    dumpAddr(0x48); dumpAddr(0x68);
}

#include "X-MRA_cWrap.h" // xArm() xDisarm()
static void _ARM()  { xArm(); }
static void _DISARM() { xDisarm(); }

#include "ADS7830_cWrap.h" // adcReadAll()
static void _adc() { adcReadAll(); }

#include "WS2812_cWrap.h" // wsLedTest()

#include "dpad.h" // dpad()
extern dPadBind bS12;   static void _dpadS() { dpad(bS12); }
extern dPadBind bL;     static void _dpadL() { dpad(bL); }
extern dPadBind bB;     static void _dpadB() { dpad(bB); }

static void _ver_info() { ver_info(); }
static void _exit() { exit(0); }

#include "jpad.hpp"         // jpad()
const CommandCLI Cmds[] = {
    {.name = "i2cScan", .function = busScan,    .description = "I2C Bus scan Tool"},
    {.name = "i2cD4",   .function = _busD4,     .description = "I2C Bus dump 0x40 device"},

    {.name = "adc",     .function = _adc,       .description = "Read ADS7830 ADC all channels"},
    {.name = "led",     .function = wsLedTest,  .description = "WS2812 addressable LED test"},

    // {.name = "dpad",    .function = _dpadL,     .description = "keyboard control Tool"},
    {.name = "dpadS",   .function = _dpadS,     .description = "Servo 18 keyboard control Tool"},
    {.name = "dpadL",   .function = _dpadL,     .description = "Leg keyboard control Tool"},
    {.name = "dpadB",   .function = _dpadB,     .description = "Body keyboard control Tool"},
    {.name = "jpad",    .function = jpad,       .description = "SDL GamePad control Tool"},
    // {.name = "wpad",    .function = wpad,       .description = "Web GamePad control Tool"},

    {.name = "arm",     .function = _ARM},
    {.name = "disarm",  .function = _DISARM},
    // {.name = "imu",     .function = _IMU_Init,    .description = "init IMU"},

    {.name = "ver",     .function = _ver_info,  .description = "Show curent Version Information"},
    // {.name = "reboot",  .function = reboot,     .description = "Reboot System"},
    // {.name = "exit",    .function = exit_,      .description = "Quit app"},    // risk LOGOUT from ssh  =)
    {.name = "quit",    .function = _exit,      .description = "Quit app"},

    {.name = "help",    .function = cliHelp,    .description = "command list"},
    {.name = "?",       .function = cliHelp,    .description = "command list"},

    EndOfCmds  // should be last
};