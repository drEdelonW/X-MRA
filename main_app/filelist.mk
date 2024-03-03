
# Перечислите ваши исходные файлы здесь или используйте wildcard для автоматического определения
# Например: SRCS=$(SRC_DIR)/file1.c $(SRC_DIR)/file2.c
# или для всех .c файлов в папке src: SRCS=$(wildcard $(SRC_DIR)/*.c)
SRCS=
# SRCS += $(SRC_DIR)/main.c
# SRCS += $(SRC_DIR)/i2c/i2c.c
# SRCS += $(SRC_DIR)/X11_lib/x11.c

# SRCS += $(wildcard $(SRC_DIR)/*.c)

# SRCS += $(shell find $(SRC_DIR) -name '*.c' -or -name '*.cpp')
SRCS += $(SRC_DIR)/i2c/i2c.c
SRCS += $(SRC_DIR)/X11_lib/x11.c
SRCS += $(SRC_DIR)/main.cpp
SRCS += $(SRC_DIR)/robot_spec/robot_spec.cpp
SRCS += $(SRC_DIR)/hexapod_leg_obj/hexapod_leg_obj.cpp

SRCS += $(SRC_DIR)/pca9685_obj/pca9685_obj.cpp
SRCS += $(SRC_DIR)/pwm_servo_obj/pwm_servo_obj.cpp
SRCS += $(SRC_DIR)/joy/sdl_joystick.cpp