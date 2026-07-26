
# Перечислите ваши исходные файлы здесь или используйте wildcard для автоматического определения
# Например: SRCS=$(SRC_DIR)/file1.c $(SRC_DIR)/file2.c
# или для всех .c файлов в папке src: SRCS=$(wildcard $(SRC_DIR)/*.c)
# SRCS=
# SRCS += $(SRC_DIR)/main.c
# SRCS += $(SRC_DIR)/i2c/i2c.c
# INCLUDE_PATHS += $(SRC_DIR)/X11_lib
# SRCS += $(SRC_DIR)/X11_lib/x11.c

# SRCS += $(wildcard $(SRC_DIR)/*.c)
# SRCS += $(shell find $(SRC_DIR) -name '*.c' -or -name '*.cpp')


$(eval SYSLIB_DIR := $(SRC_DIR)/sys_libs) $(eval INCLUDE_PATHS += $(SYSLIB_DIR))
    $(eval POSIX_DIR := $(SYSLIB_DIR)/POSIX_system) $(eval INCLUDE_PATHS += $(POSIX_DIR))
        SRCS += $(POSIX_DIR)/signal_tools.cpp

    $(eval I2C_DIR := $(SYSLIB_DIR)/i2c) $(eval INCLUDE_PATHS += $(I2C_DIR))
        SRCS += $(I2C_DIR)/i2c.c
    #     SRCS += $(I2C_DIR)/joy/sdl_joystick.cpp
    #     SRCS += $(I2C_DIR)/X11_lib/x11.c

$(eval TOOLS_DIR := $(SRC_DIR)/tools) $(eval INCLUDE_PATHS += $(TOOLS_DIR))
    $(eval SHELLPG_DIR := $(TOOLS_DIR)/shellProcGuard) $(eval INCLUDE_PATHS += $(SHELLPG_DIR))

    $(eval MATH_DIR := $(TOOLS_DIR)/Math) $(eval INCLUDE_PATHS += $(MATH_DIR))
         SRCS += $(MATH_DIR)/Mach_Angle.cpp

        $(eval VECMATH_DIR := $(MATH_DIR)/vectorMath) $(eval INCLUDE_PATHS += $(VECMATH_DIR))
            SRCS += $(VECMATH_DIR)/Vector3d.cpp
            SRCS += $(VECMATH_DIR)/Matrix4x4.cpp

        $(eval PHYS_DIR := $(TOOLS_DIR)/Phys) $(eval INCLUDE_PATHS += $(PHYS_DIR))

    $(eval CLI_DIR := $(TOOLS_DIR)/CLI) $(eval INCLUDE_PATHS += $(CLI_DIR))
        SRCS += $(CLI_DIR)/CLI.c
        SRCS += $(CLI_DIR)/CLI_tools.c

    $(eval DPAD_DIR := $(TOOLS_DIR)/dPad) $(eval INCLUDE_PATHS += $(DPAD_DIR))
        SRCS += $(DPAD_DIR)/dpad.c
        SRCS += $(DPAD_DIR)/dpad_tools.c
        SRCS += $(DPAD_DIR)/keyConst.c

    $(eval JPAD_DIR := $(TOOLS_DIR)/jPad) $(eval INCLUDE_PATHS += $(JPAD_DIR))
        SRCS += $(JPAD_DIR)/jpad.cpp

        $(eval JOYS_DIR := $(JPAD_DIR)/Joystick) $(eval INCLUDE_PATHS += $(JOYS_DIR))
            SRCS += $(JOYS_DIR)/joystick_sys.cpp
            SRCS += $(JOYS_DIR)/joystick_tools.cpp
            SRCS += $(JOYS_DIR)/SDL/sdl_joystick.cpp

            $(eval WJOYS_DIR := $(JOYS_DIR)/Web) $(eval INCLUDE_PATHS += $(WJOYS_DIR))
                # SRCS += $(WJOYS_DIR)/webJoystick.cpp
                SRCS += $(WJOYS_DIR)/socket_ctrl.cpp
                SRCS += $(WJOYS_DIR)/http_socket.cpp

$(eval DRV_DIR := $(SRC_DIR)/drv) $(eval INCLUDE_PATHS += $(DRV_DIR))
    $(eval PCAPWM_DIR := $(DRV_DIR)/PwmArry_PCA9685) $(eval INCLUDE_PATHS += $(PCAPWM_DIR))
        SRCS += $(PCAPWM_DIR)/PA_PCA9685.cpp
        SRCS += $(PCAPWM_DIR)/PA_PCA9685_freq.cpp
        SRCS += $(PCAPWM_DIR)/PA_PCA9685_duty.cpp
        SRCS += $(PCAPWM_DIR)/PA_PCA9685_durat.cpp
        SRCS += $(PCAPWM_DIR)/PA_PCA9685_power.cpp
        SRCS += $(PCAPWM_DIR)/PA_PCA9685_i2c.cpp
        SRCS += $(PCAPWM_DIR)/PA_PCA9685_tools.cpp
        SRCS += $(PCAPWM_DIR)/PWMChannel.cpp

    $(eval SERVO_DIR := $(DRV_DIR)/Servos) $(eval INCLUDE_PATHS += $(SERVO_DIR))
        $(eval PWM_SERVO_DIR := $(SERVO_DIR)/pwm_servo) $(eval INCLUDE_PATHS += $(PWM_SERVO_DIR))
            # SRCS += $(PWM_SERVO_DIR)/pwm_servo.cpp

        $(eval MG996_SRV_DIR := $(SERVO_DIR)/PwmServo_MG996R) $(eval INCLUDE_PATHS += $(MG996_SRV_DIR))
            SRCS += $(MG996_SRV_DIR)/PS_MG996R.cpp

$(eval XMRA_DIR := $(SRC_DIR)/X-MRA) $(eval INCLUDE_PATHS += $(XMRA_DIR))
    $(eval RSPEC_DIR := $(XMRA_DIR)/robot_spec) $(eval INCLUDE_PATHS += $(RSPEC_DIR))
#         SRCS += $(RSPEC_DIR)/robot_spec_old_chassis.cpp
        SRCS += $(RSPEC_DIR)/robot_spec_Freenove_chassis.cpp
        
        SRCS += $(RSPEC_DIR)/robot_spec_cwrap.cpp

    $(eval JOINT_DIR := $(XMRA_DIR)/JointBase) $(eval INCLUDE_PATHS += $(JOINT_DIR))

    $(eval AR_DIR := $(XMRA_DIR)/Arachnid) $(eval INCLUDE_PATHS += $(AR_DIR))
        $(eval AR_LEG_DIR := $(AR_DIR)/Leg) $(eval INCLUDE_PATHS += $(AR_LEG_DIR))
            SRCS += $(AR_LEG_DIR)/ArachnidLeg.cpp
            SRCS += $(AR_LEG_DIR)/ArachnidLeg_wrap.cpp
            SRCS += $(AR_LEG_DIR)/ArachnidLeg_FK_IK.cpp

        $(eval AR_BODY_DIR := $(AR_DIR)/Body) $(eval INCLUDE_PATHS += $(AR_BODY_DIR))
            SRCS += $(AR_BODY_DIR)/ArachnidBody.cpp
            SRCS += $(AR_BODY_DIR)/ArachnidBody_activation.cpp
            SRCS += $(AR_BODY_DIR)/ArachnidBody_coordinator.cpp
            SRCS += $(AR_BODY_DIR)/ArachnidBody_choreographer.cpp
            SRCS += $(AR_BODY_DIR)/ArachnidBody_Aim.cpp

    SRCS += $(XMRA_DIR)/CLI_cmdList.c
    SRCS += $(XMRA_DIR)/test.c
    SRCS += $(XMRA_DIR)/dpad_cmds_leg.c
    # SRCS += $(XMRA_DIR)/dpad_cmds_12servos.c

SRCS += $(SRC_DIR)/main.cpp
INCLUDE_PATHS += /usr/include/SDL2

DEFINE_LIST=
# DEFINE_LIST += DEBUD=2
DEFINE_LIST += DEBUG=2
DEFINE_LIST += CLI_AUTOEXEC_CMD=\"jpad\"
# DEFINE_LIST += CLI_AUTOEXEC_CMD=\"wpad\"
# DEFINE_LIST += CLI_AUTOEXEC_CMD=\"test\"

LDFLAGS += -lSDL2
# LDFLAGS += -lX11