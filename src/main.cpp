
#include "common_tools.h"   // ver_info
#include "signal_tools.hpp" // sysInit
#include "CLI.h"            // startCLI

int main(int argc, char* argv[]) {
    ver_info(); // print git info
    sysInit();  // sys signal handle
    startCLI(); // main command processor

    return 0;
}
