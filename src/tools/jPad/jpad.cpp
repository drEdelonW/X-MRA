#include "jpad.hpp"
#include "terminal_tools.h"
#include "joystick.hpp"
#include <unistd.h>  //usleep
#include "robot_spec.hpp"

void jpad() {
    GCInit();
    bool _run = true;

    bool triAct = false;
    float scaleStep = 50.0f;

    Matrix4x4 buMx;
    while(_run){
        if (GCGetButton(BUTTON_PS) &&
            GCGetButton(BUTTON_CROSS)
        ){
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
            if (GCGetButton(BUTTON_L3)) {
                lStep = 1.0f;
            } else { lStep = gp.left.z; }
            if (GCGetButton(BUTTON_R3)) {
                rStep = 1.0f;
            } else { rStep = gp.right.z; }
            XMRA.trySetOffs(Vector3D{gp.left.y ,-gp.left.x, -lStep} * scaleStep, 0);
            XMRA.trySetOffs(Vector3D{gp.right.y, -gp.right.x, -rStep} * scaleStep, 1);
        } else {    // non Tripod mode
            if (GCGetButton(BUTTON_L3)){
                // LOG("%f %f %f\n", gp.left.x, gp.left.y, gp.left.z);
                XMRA.trySetOffs(Vector3D{0.0f, 0.0f, 0.0f});
            } else {
                if ((fabsf(gp.left.x) > 0.038f) ||
                    (fabsf(gp.left.y) > 0.038f) ||
                    (fabsf(gp.right.z - gp.left.z) > 0.03f)) {
                    // LOG("%f %f %f\n", gp.left.x, gp.left.y, gp.left.z);

                    XMRA.tryAddOffs(Vector3D{-gp.left.y, gp.left.x, gp.right.z - gp.left.z} * 3.0f);
                }
#if 1
                IF_BTN_HIT(BUTTON_R3,{
                    if (XMRA.AimSetAngle(deg(0), deg(0))) {
                        XMRA.applyPose();
                    }
                })
                if ((fabsf(gp.right.x) > 0.038f) ||
                    (fabsf(gp.right.y) > 0.038f)) {
                    if (XMRA.AimAddAngle(deg(-gp.right.x), deg(-gp.right.y))) {
                        XMRA.applyPose();
                    }
                }
#else
                Vector3D zOffs = {0.0, 0.0, 0.0};
                if ((fabsf(gp.right.x) > 0.038f)) {
                    // XMRA.tryAddRotationOZ(deg(-gp.right.x));
                    XMRA.getMatrix(&buMx);
                    XMRA.addOffs(zOffs);
                    XMRA.addRotationOX(deg(gp.right.x * 2.0f));
                    XMRA.addOffs(-zOffs);
                    if (!XMRA.applyPose()) {
                        XMRA.setMatrix(&buMx);
                    }
                }
                if ((fabsf(gp.right.y) > 0.038f)) {
                    XMRA.getMatrix(&buMx);
                    XMRA.addOffs(zOffs);
                    XMRA.addRotationOY(deg(-gp.right.y * 2.0f));
                    XMRA.addOffs(-zOffs);
                    if (!XMRA.applyPose()) {
                        XMRA.setMatrix(&buMx);
                    }
                }
#endif
            }
        }
    }

    GCDeinit();
}
