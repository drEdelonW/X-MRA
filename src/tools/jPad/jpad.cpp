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
    float scaleStep = 50.0f;

    while(_run){
        if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_PS))) {
            printf("pressed BUTTON_PS. exit\n");
            _run = false;
            continue;
        }

        IF_BTN_HIT(BUTTON_TRIANGLE,{
            if (XMRA.isArmed()){ XMRA.DISARM(); }
            else               { XMRA.ARM(); }
        } )

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

        if (triAct) { // Tripod mode
            float lStep = 0.0f;
            float rStep = 0.0f;
            if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_L3))) {
                lStep = 1.0f;
            } else { lStep = lz; }
            if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_R3))) {
                rStep = 1.0f;
            } else { rStep = rz; }
            XMRA.trySetOffs(Vector3D{ly ,-lx, -lStep} * scaleStep, 0);
            XMRA.trySetOffs(Vector3D{ry, -rx, -rStep} * scaleStep, 1);
        } else {    // non Tripod mode
            if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_L1))){
                // LOG("%f %f %f\n", lx, ly, lz);
                XMRA.trySetOffs(Vector3D{-ly, lx, rz -lz}* scaleStep);
            } else {
                if ((fabsf(lx) > 0.028f) ||
                    (fabsf(ly) > 0.028f) ||
                    (fabsf(rz - lz) > 0.02f)) {
                    // LOG("%f %f %f\n", lx, ly, lz);

                    XMRA.tryAddOffs(Vector3D{-ly, lx, rz - lz} * 5.0f);
                }
            }
        }
    }

    LOG(TEXT_BOLD "Joy-Pad END\n\a" TEXT_RESET);
    GameControllerDeinit();
}
