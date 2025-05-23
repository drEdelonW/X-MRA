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

    Matrix4x4 buMx;
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
            if (SDL_GameControllerGetButton(gPad, toPS(BUTTON_L3))){
                // LOG("%f %f %f\n", lx, ly, lz);
                XMRA.trySetOffs(Vector3D{0.0f, 0.0f, 0.0f});
            } else {
                if ((fabsf(lx) > 0.038f) ||
                    (fabsf(ly) > 0.038f) ||
                    (fabsf(rz - lz) > 0.03f)) {
                    // LOG("%f %f %f\n", lx, ly, lz);

                    XMRA.tryAddOffs(Vector3D{-ly, lx, rz - lz} * 3.0f);
                }
#if 1
                IF_BTN_HIT(BUTTON_R3,{
                    if (XMRA.AimSetAngle(deg(0), deg(0))) {
                        XMRA.applyPose();
                    }
                })
                if ((fabsf(rx) > 0.038f) ||
                    (fabsf(ry) > 0.038f)) {
                    if (XMRA.AimAddAngle(deg(-rx), deg(-ry))) {
                        XMRA.applyPose();
                    }
                }
#else
                Vector3D zOffs = {0.0, 0.0, 0.0};
                if ((fabsf(rx) > 0.038f)) {
                    // XMRA.tryAddRotationOZ(deg(-rx));
                    XMRA.getMatrix(&buMx);
                    XMRA.addOffs(zOffs);
                    XMRA.addRotationOX(deg(rx * 2.0f));
                    XMRA.addOffs(-zOffs);
                    if (!XMRA.applyPose()) {
                        XMRA.setMatrix(&buMx);
                    }
                }
                if ((fabsf(ry) > 0.038f)) {
                    XMRA.getMatrix(&buMx);
                    XMRA.addOffs(zOffs);
                    XMRA.addRotationOY(deg(-ry * 2.0f));
                    XMRA.addOffs(-zOffs);
                    if (!XMRA.applyPose()) {
                        XMRA.setMatrix(&buMx);
                    }
                }
#endif
            }
        }
    }

    LOG(TEXT_BOLD "Joy-Pad END\n\a" TEXT_RESET);
    GameControllerDeinit();
}
