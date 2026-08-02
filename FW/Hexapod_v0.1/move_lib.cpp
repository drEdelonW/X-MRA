#include "move_lib.h"

v3f amplitude;
v3f stride;
v3i commanded;

int z_height_left, z_height_right;
float leg1_coxa, leg1_femur, leg1_tibia;
float leg6_coxa, leg6_femur, leg6_tibia;

float gamma_femur;
float phi_tibia, phi_femur;
// float theta_tibia, theta_femur, theta_coxa;
float L0, L3;                         //inverse kinematics variables

int tick, duration, numTicks;

v3f rotOffset;
v3f sinRot, cosRot;
v3i total;

int tripod_case[LEG_NUM]   = {1, 2, 1, 2, 1, 2}; //for tripod gait walking
int ripple_case[LEG_NUM]   = {2, 6, 4, 1, 3, 5}; //for ripple gait
int wave_case[LEG_NUM]     = {1, 2, 3, 4, 5, 6}; //for wave gait
int tetrapod_case[LEG_NUM] = {1, 3, 2, 1, 2, 3}; //for tetrapod gait

#ifdef DEF_LEDS
//***********************************************************************
// LED Bar Graph Routine
// Note: 8 dual-color red/green LEDs in a row
// LED_color: 0=Red, 1=Green
// LED_count: 0 to 8
//***********************************************************************
void LED_Bar(int LED_color, int LED_count) {
  //display a red bar
  if (LED_color == 0) {
    for (int i = 0; i < LED_count; i++) {
      digitalWrite((RED_LED1 + (4 * i)), HIGH);
      digitalWrite((GREEN_LED1 + (4 * i)), LOW);
    }
    for (int i = LED_count; i < 8; i++) {
      digitalWrite((RED_LED1 + (4 * i)), LOW);
      digitalWrite((GREEN_LED1 + (4 * i)), LOW);
    }
  }

  //display a green bar
  else {
    for (int i = 0; i < LED_count; i++) {
      digitalWrite((GREEN_LED1 + (4 * i)), HIGH);
      digitalWrite((RED_LED1 + (4 * i)), LOW);
    }
    for (int i = LED_count; i < 8; i++) {
      digitalWrite((GREEN_LED1 + (4 * i)), LOW);
      digitalWrite((RED_LED1 + (4 * i)), LOW);
    }
  }
}
#endif

void move_lib_init(){
  //clear offsets
  for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
    offset_[cur_leg] = {x:0.0, y:0.0, z:0.0};
  }
  capture_offsets = false;
  step_height_multiplier = 1.0;

  //initialize mode and gait variables
  mode = wm_default;
  gait = ga_tripod;
  gait_speed = false;
  reset_position = true;
  leg1_IK_control = true;
  leg6_IK_control = true;
}

void move_lib_reset_current(){
  for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
    current_[cur_leg] = leg_home[cur_leg];
  }
}

void move_lib_reset_offset(){
  for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
    offset_[cur_leg] = {x:0, y:0, z:0};
  }
}

//***********************************************************************
// Compute walking amplitudes
//***********************************************************************
void compute_amplitudes(int l_num) {
  //compute total distance from center of body to toe
  total.x = leg_home[l_num].x + leg_base[l_num].x;
  total.y = leg_home[l_num].y + leg_base[l_num].y;

  //compute rotational offset
  rotOffset.x = total.y * sinRot.z + total.x * cosRot.z - total.x;
  rotOffset.y = total.y * cosRot.z - total.x * sinRot.z - total.y;

  //compute X and Y amplitude and constrain to prevent legs from crashing into each other
  amplitude.x = ((stride.x + rotOffset.x) / 2.0);
  amplitude.y = ((stride.y + rotOffset.y) / 2.0);
  amplitude.x = constrain(amplitude.x, -50, 50);
  amplitude.y = constrain(amplitude.y, -50, 50);

  //compute Z amplitude
  if (abs(stride.x + rotOffset.x) > abs(stride.y + rotOffset.y))
    amplitude.z = step_height_multiplier * (stride.x + rotOffset.x) / 4.0;
  else
    amplitude.z = step_height_multiplier * (stride.y + rotOffset.y) / 4.0;
}

//***********************************************************************
// Compute walking stride lengths
//***********************************************************************
void compute_strides() {
  //compute stride lengths
  stride.x = 90 * commanded.x / 127;
  stride.y = 90 * commanded.y / 127;
  stride.z = 35 * commanded.z / 127;

  //compute rotation trig
  sinRot.z = sin(radians(stride.z));
  cosRot.z = cos(radians(stride.z));

  //set duration for normal and slow speed modes
      duration = (gait_speed) ? 3240 : 1080;
}

//***********************************************************************
// Set all servos to 90 degrees
// Note: this is useful for calibration/alignment of the servos
// i.e: set COXA_CAL[6], FEMUR_CAL[6], and TIBIA_CAL[6] values in
//      constants section above so all angles appear as 90 degrees
//***********************************************************************
void set_all_90() {
  Serial.println("set_all_90:139");
  for (int i = 0; i < LEG_NUM; i++)  {
  legs[i].SCOXA.write(45 + COXA_CAL[i]);
  legs[i].SFEMUR.write(45 + FEMUR_CAL[i]);
  legs[i].STIBIA.write(45 + TIBIA_CAL[i]);
  }
}

//***********************************************************************
// Tripod Gait
// Group of 3 legs move forward while the other 3 legs provide support
//***********************************************************************
void tripod_gait(v3i inp) {
  commanded = inp;
  //if commands more than deadband then process
  if ((abs(commanded.x) > 15) || (abs(commanded.y) > 15) || (abs(commanded.z) > 15) || (tick > 0)) {
    compute_strides();
    numTicks = round(duration / FRAME_TIME_MS / 2.0); //total ticks divided into the two cases
    double cosPI = cos(M_PI * tick / numTicks);
    double sinPI = sin(M_PI * tick / numTicks);
    for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
      compute_amplitudes(cur_leg);
      switch (tripod_case[cur_leg]) {
        case 1:                               //move foot forward (raise and lower)
          current_[cur_leg].x = leg_home[cur_leg].x - amplitude.x * cosPI;
          current_[cur_leg].y = leg_home[cur_leg].y - amplitude.y * cosPI;
          current_[cur_leg].z = leg_home[cur_leg].z + abs(amplitude.z) * sinPI;
          if (tick >= numTicks - 1) tripod_case[cur_leg] = 2;
          break;
        case 2:                               //move foot back (on the ground)
          current_[cur_leg].x = leg_home[cur_leg].x + amplitude.x * cosPI;
          current_[cur_leg].y = leg_home[cur_leg].y + amplitude.y * cosPI;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) tripod_case[cur_leg] = 1;
          break;
      }
    }
    //increment tick
    if (tick < numTicks - 1)
      tick++;
    else
      tick = 0;
  }
}

//***********************************************************************
// Wave Gait
// Legs move forward one at a time while the other 5 legs provide support
//***********************************************************************
void wave_gait(v3i inp) {
  commanded = inp;

  //if commands more than deadband then process
  if ((abs(commanded.x) > 15) || (abs(commanded.y) > 15) || (abs(commanded.z) > 15) || (tick > 0)) {
    compute_strides();
    numTicks = round(duration / FRAME_TIME_MS / 6.0); //total ticks divided into the six cases
    double cosPI = cos(M_PI * tick / numTicks);
    double sinPI = sin(M_PI * tick / numTicks);
    for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
      compute_amplitudes(cur_leg);
      switch (wave_case[cur_leg]) {
        case 1:                               //move foot forward (raise and lower)
          current_[cur_leg].x = leg_home[cur_leg].x - amplitude.x * cosPI;
          current_[cur_leg].y = leg_home[cur_leg].y - amplitude.y * cosPI;
          current_[cur_leg].z = leg_home[cur_leg].z + abs(amplitude.z) * sinPI;
          if (tick >= numTicks - 1) wave_case[cur_leg] = 6;
          break;
        case 2:                               //move foot back one-fifth (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.5;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.5;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) wave_case[cur_leg] = 1;
          break;
        case 3:                               //move foot back one-fifth (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.5;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.5;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) wave_case[cur_leg] = 2;
          break;
        case 4:                               //move foot back one-fifth (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.5;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.5;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1)
            wave_case[cur_leg] = 3;
          break;
        case 5:                               //move foot back one-fifth (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.5;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.5;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) wave_case[cur_leg] = 4;
          break;
        case 6:                               //move foot back one-fifth (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.5;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.5;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) wave_case[cur_leg] = 5;
          break;
      }
    }
    //increment tick
    if (tick < numTicks - 1)
      tick++;
    else
      tick = 0;
  }
}

//***********************************************************************
// Ripple Gait
// Left legs move forward rear-to-front while right also do the same,
// but right side is offset so RR starts midway through the LM stroke
//***********************************************************************
void ripple_gait(v3i inp) {
  commanded = inp;
    //if commands more than deadband then process
  if ((abs(commanded.x) > 15) ||
      (abs(commanded.y) > 15) ||
      (abs(commanded.z) > 15) ||
      (tick > 0)
      ) {
    compute_strides();
    numTicks = round(duration / FRAME_TIME_MS / 6.0); //total ticks divided into the six cases
    for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
      compute_amplitudes(cur_leg);
      switch (ripple_case[cur_leg]) {
        case 1:                               //move foot forward (raise)
          current_[cur_leg].x = leg_home[cur_leg].x - amplitude.x * cos(M_PI * tick / (numTicks * 2));
          current_[cur_leg].y = leg_home[cur_leg].y - amplitude.y * cos(M_PI * tick / (numTicks * 2));
          current_[cur_leg].z = leg_home[cur_leg].z + abs(amplitude.z) * sin(M_PI * tick / (numTicks * 2));
          if (tick >= numTicks - 1) ripple_case[cur_leg] = 2;
          break;
        case 2:                               //move foot forward (lower)
          current_[cur_leg].x = leg_home[cur_leg].x - amplitude.x * cos(M_PI * (numTicks + tick) / (numTicks * 2));
          current_[cur_leg].y = leg_home[cur_leg].y - amplitude.y * cos(M_PI * (numTicks + tick) / (numTicks * 2));
          current_[cur_leg].z = leg_home[cur_leg].z + abs(amplitude.z) * sin(M_PI * (numTicks + tick) / (numTicks * 2));
          if (tick >= numTicks - 1) ripple_case[cur_leg] = 3;
          break;
        case 3:                               //move foot back one-quarter (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.0;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.0;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) ripple_case[cur_leg] = 4;
          break;
        case 4:                               //move foot back one-quarter (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.0;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.0;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) ripple_case[cur_leg] = 5;
          break;
        case 5:                               //move foot back one-quarter (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.0;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.0;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) ripple_case[cur_leg] = 6;
          break;
        case 6:                               //move foot back one-quarter (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks / 2.0;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks / 2.0;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) ripple_case[cur_leg] = 1;
          break;
      }
    }
    //increment tick
    if (tick < numTicks - 1) tick++;
    else tick = 0;
  }
}

//***********************************************************************
// Tetrapod Gait
// Right front and left rear legs move forward together, then right
// rear and left middle, and finally right middle and left front.
//***********************************************************************
void tetrapod_gait(v3i inp) {
  commanded = inp;
  //if commands more than deadband then process
  if ((abs(commanded.x) > 15) || (abs(commanded.y) > 15) ||
      (abs(commanded.z) > 15) || (tick > 0)) {
    compute_strides();
    numTicks = round(duration / FRAME_TIME_MS / 3.0); //total ticks divided into the three cases
    for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
      compute_amplitudes(cur_leg);
      switch (tetrapod_case[cur_leg]) {
        case 1:                               //move foot forward (raise and lower)
          current_[cur_leg].x = leg_home[cur_leg].x - amplitude.x * cos(M_PI * tick / numTicks);
          current_[cur_leg].y = leg_home[cur_leg].y - amplitude.y * cos(M_PI * tick / numTicks);
          current_[cur_leg].z = leg_home[cur_leg].z + abs(amplitude.z) * sin(M_PI * tick / numTicks);
          if (tick >= numTicks - 1) tetrapod_case[cur_leg] = 2;
          break;
        case 2:                               //move foot back one-half (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) tetrapod_case[cur_leg] = 3;
          break;
        case 3:                               //move foot back one-half (on the ground)
          current_[cur_leg].x = current_[cur_leg].x - amplitude.x / numTicks;
          current_[cur_leg].y = current_[cur_leg].y - amplitude.y / numTicks;
          current_[cur_leg].z = leg_home[cur_leg].z;
          if (tick >= numTicks - 1) tetrapod_case[cur_leg] = 1;
          break;
      }
    }
    //increment tick
    if (tick < numTicks - 1)
      tick++;
    else
      tick = 0;
  }
}

//***********************************************************************
// Body translate with controller (xyz axes)
//***********************************************************************
void translate_control(v3i inp) {
  v3i translate;

  translate.x = map(inp.x, 0, 255, -2 * TRAVEL, 2 * TRAVEL);  //compute X direction move
  translate.y = map(inp.y, 0, 255, 2 * TRAVEL, -2 * TRAVEL);  //compute Y direction move
  translate.z = inp.z;    //compute Z direction move
  if (translate.z > 127)
    translate.z = map(translate.z, 128, 255, 0, TRAVEL);
  else
    translate.z = map(translate.z, 0, 127, -3 * TRAVEL, 0);

  for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++){
    current_[cur_leg] = {
      x:leg_home[cur_leg].x + translate.x,
      y:leg_home[cur_leg].y + translate.y,
      z:leg_home[cur_leg].z + translate.z
      };
  }

  //lock in offsets if commanded
  if (capture_offsets) {
    for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
      offset_[cur_leg].x += translate.x;
      offset_[cur_leg].y += translate.y;
      offset_[cur_leg].z += translate.z;
      current_[cur_leg] = leg_home[cur_leg];
    }
  }

  //if offsets were commanded, exit current mode
  if (capture_offsets) {
    capture_offsets = false;
    mode = wm_default;
  }
}

//***********************************************************************
// Body rotate with controller (xyz axes)
//***********************************************************************
void rotate_control(v3i inp, int ext) {
  //compute rotation sin/cos values using controller inputs
  v3f rotat = {
    x:(map(inp.x, 0, 255, A12DEG, -A12DEG)) / 1000000.0,
    y:(map(inp.y, 0, 255, A12DEG, -A12DEG)) / 1000000.0,
    z:(map(inp.z, 0, 255, -A30DEG, A30DEG)) / 1000000.0
  };
  sinRot = {
    x:sin(rotat.x),
    y:sin(rotat.y),
    z:sin(rotat.z)
    };
  cosRot = {
    x:cos(rotat.x),
    y:cos(rotat.y),
    z:cos(rotat.z)
    };

  //compute Z direction move
  int translateZ = ext;
  if (translateZ > 127)
    translateZ = map(translateZ, 128, 255, 0, TRAVEL);
  else
    translateZ = map(translateZ, 0, 127, -3 * TRAVEL, 0);

  for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++) {
    //compute total distance from center of body to toe
    //TODO: total = leg_home[cur_leg] + leg_base[cur_leg];
    total.x = leg_home[cur_leg].x + leg_base[cur_leg].x;
    total.y = leg_home[cur_leg].y + leg_base[cur_leg].y;
    total.z = leg_home[cur_leg].y + leg_base[cur_leg].z;

    //perform 3 axis rotations
    rotOffset.x =
      total.x * cosRot.y * cosRot.z +
      total.y * sinRot.x * sinRot.y * cosRot.z +
      total.y * cosRot.x * sinRot.z -
      total.z * cosRot.x * sinRot.y * cosRot.z +
      total.z * sinRot.x * sinRot.z -
      total.x;
    rotOffset.y =
     -total.x * cosRot.y * sinRot.z -
      total.y * sinRot.x * sinRot.y * sinRot.z +
      total.y * cosRot.x * cosRot.z +
      total.z * cosRot.x * sinRot.y * sinRot.z +
      total.z * sinRot.x * cosRot.z -
      total.y;
    rotOffset.z =
      total.x * sinRot.y -
      total.y * sinRot.x * cosRot.y +
      total.z * cosRot.x * cosRot.y -
      total.z;

    // Calculate foot positions to achieve desired rotation
    current_[cur_leg].x = leg_home[cur_leg].x + rotOffset.x;
    current_[cur_leg].y = leg_home[cur_leg].y + rotOffset.y;
    current_[cur_leg].z = leg_home[cur_leg].z + rotOffset.z + translateZ;

    //lock in offsets if commanded
    if (capture_offsets) {
      offset_[cur_leg].x += rotOffset.x;
      offset_[cur_leg].y += rotOffset.y;
      offset_[cur_leg].z += rotOffset.z + translateZ;
      current_[cur_leg] = leg_home[cur_leg];
    }
  }

  //if offsets were commanded, exit current mode
  if (capture_offsets) {
    capture_offsets = false;
    mode = wm_default;
  }
}
//***********************************************************************
// One leg lift mode
// also can set z step height using capture offsets
//***********************************************************************
void  one_leg_lift(v3i inp, int ext) {
  Serial.println("one_leg_lift:469");
  // int temp;                             //mode and control variables
  //read current leg servo 1 positions the first time
  if (leg1_IK_control == true) {
    leg1_coxa  = legs[0].SCOXA.read();
    leg1_femur = legs[0].SFEMUR.read();
    leg1_tibia = legs[0].STIBIA.read();
    leg1_IK_control = false;
  }

  //read current leg servo 6 positions the first time
  if (leg6_IK_control == true) {
    leg6_coxa  = legs[5].SCOXA.read();
    leg6_femur = legs[5].SFEMUR.read();
    leg6_tibia = legs[5].STIBIA.read();
    leg6_IK_control = false;
  }

  //process right joystick left/right axis
  int temp = map(inp.x, 0, 255, 45, -45);
  legs[0].SCOXA.write(constrain(int(leg1_coxa + temp), 45, 135));

  //process right joystick up/down axis
  temp = inp.y;
  if (temp < 117) {                               //if joystick moved up
    temp = map(temp, 116, 0, 0, 24);            //move leg 1
    legs[0].SFEMUR.write(constrain(int(leg1_femur + temp), 0, 170));
    legs[0].STIBIA.write(constrain(int(leg1_tibia + 4 * temp), 0, 170));
  } else {                                          //if joystick moved down
    z_height_right = constrain(temp, 140, 255); //set Z step height
    z_height_right = map(z_height_right, 140, 255, 1, 8);
  }

  //process left joystick left/right axis
  temp = map(inp.z, 0, 255, 45, -45);
  legs[5].SCOXA.write(constrain(int(leg6_coxa + temp), 45, 135));

  //process left joystick up/down axis
  temp = ext;
  if (temp < 117) {                              //if joystick moved up
    temp = map(temp, 116, 0, 0, 24);            //move leg 6
    legs[5].SFEMUR.write(constrain(int(leg6_femur + temp), 0, 170));
    legs[5].STIBIA.write(constrain(int(leg6_tibia + 4 * temp), 0, 170));
  } else {                                         //if joystick moved down
    z_height_left = constrain(temp, 140, 255);  //set Z step height
    z_height_left = map(z_height_left, 140, 255, 1, 8);
  }

  //process z height adjustment
  if (z_height_left > z_height_right)
    z_height_right = z_height_left;             //use max left or right value
#ifdef DEF_LEDS
  if (batt_LEDs > 3)
    z_height_LED_color = 0;    //use red LEDs if battery strong
  else
    z_height_LED_color = 1;                  //use green LEDs if battery weak
  LED_Bar(z_height_LED_color, z_height_right);  //display Z height
#endif
  if (capture_offsets) {                 //lock in Z height if commanded
    step_height_multiplier = 1.0 + ((z_height_right - 1.0) / 3.0);
    capture_offsets = false;
  }
}

//***********************************************************************
// Leg IK Routine
//***********************************************************************
void leg_IK(int cur_legber, float X, float Y, float Z) {
  Serial.print("leg_IK:536 ");  Serial.print(cur_legber);
  Serial.print(" x:");  Serial.print(X);
  Serial.print(" y:");  Serial.print(Y);
  Serial.print(" z:");  Serial.print(Z);
  Serial.print("\r");
  //compute target femur-to-toe (L3) length
  L0 = sqrt(sq(X) + sq(Y)) - COXA_LENGTH;
  L3 = sqrt(sq(L0) + sq(Z));

  //process only if reach is within possible range (not too long or too short!)
  if ((L3 < (TIBIA_LENGTH + FEMUR_LENGTH)) && (L3 > (TIBIA_LENGTH - FEMUR_LENGTH))) {
    //compute tibia angle
    phi_tibia = acos((sq(FEMUR_LENGTH) + sq(TIBIA_LENGTH) - sq(L3)) / (2 * FEMUR_LENGTH * TIBIA_LENGTH));
    theta_tibia = phi_tibia * RAD_TO_DEG - 23.0 + TIBIA_CAL[cur_legber];
    theta_tibia = constrain(theta_tibia, 0.0, 180.0);

    //compute femur angle
    gamma_femur = atan2(Z, L0);
    phi_femur = acos((sq(FEMUR_LENGTH) + sq(L3) - sq(TIBIA_LENGTH)) / (2 * FEMUR_LENGTH * L3));
    theta_femur = (phi_femur + gamma_femur) * RAD_TO_DEG + 14.0 + 90.0 + FEMUR_CAL[cur_legber];
    theta_femur = constrain(theta_femur, 0.0, 180.0);

    //compute coxa angle
    theta_coxa = atan2(X, Y) * RAD_TO_DEG + COXA_CAL[cur_legber];

    //output to the appropriate leg
    switch (cur_legber) {
      case 0:
        if (leg1_IK_control == true) {                     //flag for IK or manual control of leg
          theta_coxa = theta_coxa + 45.0;                 //compensate for leg mounting
          theta_coxa = constrain(theta_coxa, 0.0, 180.0);
          legs[0].SCOXA.write(int(theta_coxa));
          legs[0].SFEMUR.write(int(theta_femur));
          legs[0].STIBIA.write(int(theta_tibia));
        }
        break;
      case 1:
        theta_coxa = theta_coxa + 90.0;                 //compensate for leg mounting
        theta_coxa = constrain(theta_coxa, 0.0, 180.0);
        legs[1].SCOXA.write(int(theta_coxa));
        legs[1].SFEMUR.write(int(theta_femur));
        legs[1].STIBIA.write(int(theta_tibia));
        break;
      case 2:
        theta_coxa = theta_coxa + 135.0;                 //compensate for leg mounting
        theta_coxa = constrain(theta_coxa, 0.0, 180.0);
        legs[2].SCOXA.write(int(theta_coxa));
        legs[2].SFEMUR.write(int(theta_femur));
        legs[2].STIBIA.write(int(theta_tibia));
        break;
      case 3:
        if (theta_coxa < 0)                               //compensate for leg mounting
          theta_coxa = theta_coxa + 225.0;                // (need to use different
        else                                              //  positive and negative offsets
          theta_coxa = theta_coxa - 135.0;                //  due to atan2 results above!)
        theta_coxa = constrain(theta_coxa, 0.0, 180.0);
        legs[3].SCOXA.write(int(theta_coxa));
        legs[3].SFEMUR.write(int(theta_femur));
        legs[3].STIBIA.write(int(theta_tibia));
        break;
      case 4:
        if (theta_coxa < 0)                               //compensate for leg mounting
          theta_coxa = theta_coxa + 270.0;                // (need to use different
        else                                              //  positive and negative offsets
          theta_coxa = theta_coxa - 90.0;                 //  due to atan2 results above!)
        theta_coxa = constrain(theta_coxa, 0.0, 180.0);
        legs[4].SCOXA.write(int(theta_coxa));
        legs[4].SFEMUR.write(int(theta_femur));
        legs[4].STIBIA.write(int(theta_tibia));
        break;
      case 5:
        if (leg6_IK_control == true) {                     //flag for IK or manual control of leg
          if (theta_coxa < 0)                             //compensate for leg mounting
            theta_coxa = theta_coxa + 315.0;              // (need to use different
          else                                            //  positive and negative offsets
            theta_coxa = theta_coxa - 45.0;               //  due to atan2 results above!)
          theta_coxa = constrain(theta_coxa, 0.0, 180.0);
          legs[5].SCOXA.write(int(theta_coxa));
          legs[5].SFEMUR.write(int(theta_femur));
          legs[5].STIBIA.write(int(theta_tibia));
        }
        break;
    }
  }
}
