#ifndef _MOVE_LIB_H_
#define _MOVE_LIB_H_
#include <math.h>
#include <arduino.h>

// #define DEF_LEDS

#include "hw_spec.h"

#ifdef DEF_LEDS
static int gait_LED_color;
static int batt_LEDs;                        //battery monitor variables
static int z_height_LED_color;
void LED_Bar(int LED_color, int LED_count);
#endif

typedef enum {
  wm_default,
  wm_select,
  wm_xyz,
  wm_ypr,
  wm_one_leg,
  wm_90_deg
} en_walk_mode;

typedef enum {
  ga_tripod,   
  ga_wave,
  ga_ripple,
  ga_tetrapod
} en_gait;

static int cur_leg;
static en_walk_mode mode;
static en_gait gait;
static bool gait_speed;
static bool leg1_IK_control, leg6_IK_control; //leg lift mode variables
static bool reset_position;
static bool capture_offsets;
static v3f offset_[LEG_NUM];
static v3f current_[LEG_NUM];
static float step_height_multiplier;

static float theta_tibia, theta_femur, theta_coxa;

void move_lib_init();
void move_lib_reset_current();
void move_lib_reset_offset();

void compute_amplitudes(int l_num);
void compute_strides();

void set_all_90();                      //set all servos to 90 degrees mode
void tripod_gait(v3i inp);             //walk using gait 0
void wave_gait(v3i inp);               //walk using gait 1
void ripple_gait(v3i inp);             //walk using gait 2
void tetrapod_gait(v3i inp);           //walk using gait 3
void translate_control(v3i inp);       //joystick control x-y-z mode
void rotate_control(v3i inp, int ext);          //joystick control y-p-r mode
void one_leg_lift(v3i inp, int ext);            //one leg lift mode

void leg_IK(int cur_legber, float X, float Y, float Z);

#endif
