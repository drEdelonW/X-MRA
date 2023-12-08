#ifndef _HW_SPEC_H_
#define _HW_SPEC_H_

// #include <arduino.h>
#include <Servo.h>

typedef struct {
  float x,y,z;
} v3f;
typedef struct {
  int x,y,z;
} v3i;

#define LEG_NUM 6
#define SERVO_MIN 610
#define SERVO_MAX 2400

  //leg part lengths
#define COXA_LENGTH 51
#define FEMUR_LENGTH 65
#define TIBIA_LENGTH 121

//enum legname {
//  lCOXA = 0,
//  lFEMUR = 1,
//  lTIBIA = 2
//};

typedef struct {
  int COXA, FEMUR, TIBIA;
  Servo SCOXA, SFEMUR, STIBIA;
} hexaleg;

//translate and rotate travel limit constant
#define TRAVEL 30

const long A12DEG = 209440;           //12 degrees in radians x 1,000,000
const long A30DEG = 523599;           //30 degrees in radians x 1,000,000

const int FRAME_TIME_MS = 20;         //frame time (20msec = 50Hz)

static hexaleg legs[LEG_NUM] = { //servo port definitions
  {COXA: 31, FEMUR: 29, TIBIA: 27}, // 0
  {COXA: 30, FEMUR: 28, TIBIA: 26}, // 1
  {COXA: 25, FEMUR: 23, TIBIA: 21}, // 2
  {COXA: 24, FEMUR: 22, TIBIA: 20}, // 3
  {COXA: 19, FEMUR: 17, TIBIA: 15}, // 4
  {COXA: 18, FEMUR: 16, TIBIA: 14}  // 5
};

const v3f leg_home[LEG_NUM] = { //coxa-to-toe home positions
  {x: 82.0, y: 82.0,  z:-80.0},  // 0
  {x: 0.0,  y: 116.0, z:-80.0},  // 1
  {x:-82.0, y: 82.0,  z:-80.0},  // 2
  {x:-82.0, y:-82.0,  z:-80.0},  // 3
  {x: 0.0,  y:-116.0, z:-80.0},  // 4
  {x: 82.0, y:-82.0,  z:-80.0}   // 5
};

const v3f leg_base[LEG_NUM] = { //body center-to-coxa servo distances
  {x: 110.4,  y: 58.4,  z: 0.0},  // 0
  {x: 0.0,    y: 90.8,  z: 0.0},  // 1
  {x:-110.4,  y: 58.4,  z: 0.0},  // 2
  {x:-110.4,  y:-58.4,  z: 0.0},  // 3
  {x: 0.0,    y: 90.8,  z: 0.0},  // 4
  {x: 110.4,  y:-58.4,  z: 0.0}   // 5
};

static const int COXA_CAL[LEG_NUM]  = {0, 0, 0, 0, 0, 0};                       //servo calibration constants
static const int FEMUR_CAL[LEG_NUM] = {0, 0, 0, 0, 0, 0};
static const int TIBIA_CAL[LEG_NUM] = {0, 0, 0, 0, 0, 0};

void leg_init();

#endif //_HW_SPEC_H_
