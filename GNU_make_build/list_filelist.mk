
# Перечислите ваши исходные файлы здесь или используйте wildcard для автоматического определения
# Например: SRCS=$(SRC_DIR)/file1.c $(SRC_DIR)/file2.c
# или для всех .c файлов в папке src: SRCS=$(wildcard $(SRC_DIR)/*.c)
# SRCS=
# SRCS += $(SRC_DIR)/main.c
# SRCS += $(SRC_DIR)/i2c/i2c.c
# SRCS += $(SRC_DIR)/X11_lib/x11.c

# SRCS += $(wildcard $(SRC_DIR)/*.c)

# SRCS += $(shell find $(SRC_DIR) -name '*.c' -or -name '*.cpp')

INCLUDE_PATHS += $(SRC_DIR)/sys_libs/i2c
         SRCS += $(SRC_DIR)/sys_libs/i2c/i2c.c

# INCLUDE_PATHS += $(SRC_DIR)/X11_lib
# SRCS += $(SRC_DIR)/sys_libs/X11_lib/x11.c

INCLUDE_PATHS += $(SRC_DIR)
INCLUDE_PATHS += $(SRC_DIR)/tools
INCLUDE_PATHS += $(SRC_DIR)/tools/Math
         SRCS += $(SRC_DIR)/tools/Math/Mach_Angle.cpp

INCLUDE_PATHS += $(SRC_DIR)/tools/Phys

SRCS += $(SRC_DIR)/main.cpp

INCLUDE_PATHS += $(SRC_DIR)/tools/CLI
         SRCS += $(SRC_DIR)/tools/CLI/CLI.c
         SRCS += $(SRC_DIR)/tools/CLI/CLI_tools.c

INCLUDE_PATHS += $(SRC_DIR)/tools/dPad
         SRCS += $(SRC_DIR)/tools/dPad/dpad.c
         SRCS += $(SRC_DIR)/tools/dPad/dpad_tools.c
         SRCS += $(SRC_DIR)/tools/dPad/keyConst.c

# INCLUDE_PATHS += $(SRC_DIR)/joy
# SRCS += $(SRC_DIR)/sys_libs/joy/sdl_joystick.cpp
INCLUDE_PATHS += /usr/include/SDL2
INCLUDE_PATHS += $(SRC_DIR)/sys_libs/Joystick
         SRCS += $(SRC_DIR)/sys_libs/Joystick/joystick.cpp
         SRCS += $(SRC_DIR)/sys_libs/Joystick/joystick_sys.cpp
         SRCS += $(SRC_DIR)/sys_libs/Joystick/joystick_tools.cpp

INCLUDE_PATHS += $(SRC_DIR)/drv

INCLUDE_PATHS += $(SRC_DIR)/drv/PwmArry_PCA9685
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685_freq.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685_duty.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685_durat.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685_power.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685_i2c.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PA_PCA9685_tools.cpp
          SRCS += $(SRC_DIR)/drv/PwmArry_PCA9685/PWMChannel.cpp


# INCLUDE_PATHS += $(SRC_DIR)/drv/Servos/pwm_servo
        #  SRCS += $(SRC_DIR)/drv/Servos/pwm_servo/pwm_servo.cpp

INCLUDE_PATHS += $(SRC_DIR)/drv/Servos
INCLUDE_PATHS += $(SRC_DIR)/drv/Servos/PwmServo_MG996R
         SRCS += $(SRC_DIR)/drv/Servos/PwmServo_MG996R/PS_MG996R.cpp

INCLUDE_PATHS += $(SRC_DIR)/X-MRA/robot_spec
         SRCS += $(SRC_DIR)/X-MRA/robot_spec/robot_spec.cpp
         SRCS += $(SRC_DIR)/X-MRA/CLI_cmdList.c
        #  SRCS += $(SRC_DIR)/X-MRA/dpad_cmds_12servos.c
         SRCS += $(SRC_DIR)/X-MRA/dpad_cmds_leg.c

INCLUDE_PATHS += $(SRC_DIR)/tools/Math/vectorMath
         SRCS += $(SRC_DIR)/tools/Math/vectorMath/Vector3d.cpp
         SRCS += $(SRC_DIR)/tools/Math/vectorMath/Matrix4x4.cpp

INCLUDE_PATHS += $(SRC_DIR)/X-MRA/JointBase
INCLUDE_PATHS += $(SRC_DIR)/X-MRA/ArachnidLeg
         SRCS += $(SRC_DIR)/X-MRA/ArachnidLeg/ArachnidLeg.cpp

INCLUDE_PATHS += $(SRC_DIR)/X-MRA/ArachnidBody
         SRCS += $(SRC_DIR)/X-MRA/ArachnidBody/ArachnidBody.cpp
         SRCS += $(SRC_DIR)/X-MRA/ArachnidBody/ArachnidBody_activation.cpp
         SRCS += $(SRC_DIR)/X-MRA/ArachnidBody/ArachnidBody_coordinator.cpp
         SRCS += $(SRC_DIR)/X-MRA/ArachnidBody/ArachnidBody_choreographer.cpp

DEFINE_LIST=
# DEFINE_LIST += DEBUD=2
DEFINE_LIST += DEBUG=2

LDFLAGS += -lSDL2
# LDFLAGS += -lX11