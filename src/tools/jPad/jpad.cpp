#include "jpad.hpp"
#include "terminal_tools.h"
#include "joystick.hpp"
#include <unistd.h>  //usleep
#include "robot_spec.hpp"

void jpad() {
    GameControllerInit();
    LOG(TEXT_BOLD "Joy-Pad started\n\a" TEXT_RESET);
    joy_echo = false;
    bool _run = true;
    bool triAct = false;
    while(_run){
        if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_PS))) {
            printf("pressed BUTTON_PS. exit\n");
            _run = false;
            continue;
        }

        IF_BTN_HIT(BUTTON_R1,{
            triAct = !triAct;
            if (triAct) {
                LOG("Tripod mode ON\n");
                XMRA.setPatMask(0, TRIPOD_A);
                XMRA.setPatMask(1, TRIPOD_B);
            } else {
                LOG("Tripod mode OFF\n");
                XMRA.setPatMask(0, LEGS_ALL);
            }
        })
        if (triAct) { // non Tripod mode
            XMRA.setOffs({
                 ly * 50.0f,
                -lx * 50.0f,
                -lz * 50.0f
            },0) &&
            XMRA.applyPose(0);
            XMRA.setOffs({
                 ry * 50.0f,
                -rx * 50.0f,
                -rz * 50.0f
            },1) &&
            XMRA.applyPose(1);
        } else {    // non Tripod mode
            if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_L1))){
                // LOG("%f %f %f\n", lx, ly, lz);
                XMRA.setOffs({
                    -ly * 50.0f,
                        lx * 50.0f,
                    (rz * 50.0f) - (lz * 50.0f)
                }) &&
                XMRA.applyPose();
            }
        }



        IF_BTN_HIT(BUTTON_TRIANGLE,{
            if (XMRA.isArmed()){
                XMRA.DISARM();
            } else {
                XMRA.ARM();
            }
        } )
    }

    LOG(TEXT_BOLD "Joy-Pad END\n\a" TEXT_RESET);
    GameControllerDeinit();
}
