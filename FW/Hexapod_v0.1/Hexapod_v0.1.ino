//include "move_lib.h"
//
//#define DEF_DEBUG
//
////***********************************************************************
//// Constant Declarations
////***********************************************************************
////12V Battery analog voltage input port
//#define BATT_VOLTAGE 0
//
//PS2X ps2x;              //PS2 gamepad controller
////gamepad port definitions
//#define PS2_DAT 2
//#define PS2_ATT 3
//#define PS2_CMD 4
//#define PS2_CLK 5
//const int RUMBLE = true;
//const int PRESSURES = false;
//
//int gamepad_error;                    //gamepad variables
//byte gamepad_type;
//byte gamepad_vibrate;
//
////***********************************************************************
//// Variable Declarations
////***********************************************************************
//unsigned long currentTime;            //frame timer variables
//unsigned long previousTime;
//
//int batt_voltage;
//int batt_voltage_index;
//int batt_voltage_array[50];
//long batt_voltage_sum;
//
////***********************************************************************
//// Object Declarations
////***********************************************************************
//
////***********************************************************************
//// Process gamepad controller inputs
////***********************************************************************
//void process_gamepad() {
//  if (ps2x.ButtonPressed(PSB_PAD_DOWN)) {  //stop & select gait tripod
//    mode = wm_default;
//    gait = ga_tripod;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_PAD_LEFT)) {  //stop & select gait wave
//    mode = wm_default;
//    gait = ga_wave;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_PAD_UP))   { //stop & select gait ripple
//    mode = wm_default;
//    gait = ga_ripple;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) { //stop & select gait tetrapod
//    mode = wm_default;
//    gait = ga_tetrapod;
//    reset_position = true;
//  }
//#ifdef DEF_LEDS
//  if (mode == wm_default) {                          //display selected gait on LEDs if button held
//    if (batt_LEDs > 3)
//      gait_LED_color = 0; //display gait using red LEDs if battery strong
//    else
//      gait_LED_color = 1;              //display gait using green LEDs if battery weak
//    if (ps2x.Button(PSB_PAD_DOWN))  //display gait 0
//      LED_Bar(gait_LED_color, 1);
//    if (ps2x.Button(PSB_PAD_LEFT))  //display gait 1
//      LED_Bar(gait_LED_color, 2);
//    if (ps2x.Button(PSB_PAD_UP))    //display gait 2
//      LED_Bar(gait_LED_color, 3);
//    if (ps2x.Button(PSB_PAD_RIGHT)) //display gait 3
//      LED_Bar(gait_LED_color, 4);
//  }
//#endif
//  //vibrate controller if walk button held
//  gamepad_vibrate = ps2x.Button(PSB_TRIANGLE) ? 64 : 0;
//
//  if (ps2x.ButtonPressed(PSB_TRIANGLE)) {   //select walk mode
//    mode = wm_select;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_SQUARE)) {     //control x-y-z with joysticks mode
//    mode = wm_xyz;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_CIRCLE)) {     //control y-p-r with joysticks mode
//    mode = wm_ypr;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_CROSS)) {     //one leg lift mode
//    mode = wm_one_leg;
//    reset_position = true;
//  }
//  if (ps2x.ButtonPressed(PSB_START)) {     //change gait speed
//    gait_speed = !gait_speed;
//  }
//#ifdef DEF_LEDS
//  if (ps2x.Button(PSB_START)) {             //display gait speed on LEDs if button held
//    //use red LEDs for slow
//    //use green LEDs for fast
//    LED_Bar(!gait_speed, 8);
//  }
//#endif
//  if (ps2x.ButtonPressed(PSB_SELECT)) {     //set all servos to 90 degrees for calibration
//    mode = wm_90_deg;
//  }
//  if ((ps2x.ButtonPressed(PSB_L1)) || (ps2x.ButtonPressed(PSB_R1))) {
//    //capture offsets in translate, rotate, and translate/rotate modes
//    capture_offsets = true;
//  }
//  if ((ps2x.ButtonPressed(PSB_L2)) || (ps2x.ButtonPressed(PSB_R2))) {
//    move_lib_reset_offset();
//    leg1_IK_control = true;               //reset leg lift first pass flags
//    leg6_IK_control = true;
//    step_height_multiplier = 1.0;         //reset step height multiplier
//  }
//}
//
//
////***********************************************************************
//// Battery monitor routine
//// Note: my hexapod uses a 3S LiPo battery
//// (fully charged = 12.6V, nominal = 11.4V, discharged = 10.2V)
////***********************************************************************
//void battery_monitor() {
//  //update voltage sum (remove oldest value and insert new value into array)
//  batt_voltage_sum -= batt_voltage_array[batt_voltage_index];
//  //scale voltage reading to 0 to 14.97V (slight recalibration due to resistor tolerances)
//  batt_voltage_array[batt_voltage_index] = map(analogRead(BATT_VOLTAGE), 0, 1023, 0, 1497);
//  batt_voltage_sum += batt_voltage_array[batt_voltage_index];
//  batt_voltage_index++;
//  if (batt_voltage_index > 49)
//    batt_voltage_index = 0;
//
//  //compute average battery voltage over the 50 samples
//  batt_voltage = batt_voltage_sum / 50;
//
//#ifdef DEF_LEDS
//  //remap battery voltage for display on the LEDs
//  //minimum = 10.2V, maximum (full) = 12.3V
//  batt_LEDs = map(constrain(batt_voltage, 1020, 1230), 1020, 1230, 1, 8);
//  if (batt_LEDs > 3)
//    LED_Bar(1, batt_LEDs); //display green if voltage >= 11.40V
//  else
//    LED_Bar(0, batt_LEDs);             //display red if voltage < 11.40V
//#endif
//}
//
////***********************************************************************
//// Print Debug Data
////***********************************************************************
//void print_debug() {
//#ifdef DEF_DEBUG
//  //output IK data
//  Serial.print("tc:");
//   Serial.print(int(theta_coxa));
//   Serial.print(",tf:");
//   Serial.print(int(theta_femur));
//   Serial.print(",tt:");
//   Serial.print(int(theta_tibia));
//   Serial.print("; x:");
//  //output XYZ coordinates for all legs
//   for(cur_leg=0; cur_leg<LEG_NUM; cur_leg++)
//   {
//     Serial.print(int(current_[cur_leg].x));
//     if(cur_leg<5) Serial.print(",");
//   }
//   Serial.print("  y:");
//   for(cur_leg=0; cur_leg<LEG_NUM; cur_leg++)
//   {
//     Serial.print(int(current_[cur_leg].y));
//     if(cur_leg<5) Serial.print(",");
//   }
//   Serial.print("  z:");
//   for(cur_leg=0; cur_leg<LEG_NUM; cur_leg++)
//   {
//     Serial.print(int(current_[cur_leg].z));
//     if(cur_leg<5) Serial.print(",");
//   }
//   Serial.print("  time:");
//#endif
//
//  //display elapsed frame time (ms) and battery voltage (V)
//  currentTime = millis();
//  Serial.print(currentTime - previousTime);
//  // Serial.print(",");
//  // Serial.print(float(batt_voltage) / 100.0);
//  Serial.print("\r");
//}
//
//
////***********************************************************************
//// Initialization Routine
////***********************************************************************
//void setup() {
//  previousTime = currentTime = millis();
//  //start serial
//  Serial.begin(115200);
//
//  //attach servos
//  leg_init();
//
//  //connect the gamepad
//  gamepad_error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, PRESSURES, RUMBLE);
//  if (gamepad_error == 0)
//    Serial.println("Controller attached");
//  else if (gamepad_error == 1)
//    Serial.println("No controller found");
//  else if (gamepad_error == 2)
//    Serial.println("Controller found but not accepting commands");
//  else if (gamepad_error == 3)
//    Serial.println("Controller refusing to enter Pressures mode");
//
//  //verify the gamepad type
//  gamepad_type = ps2x.readType();
//  if (gamepad_type == 0)
//    Serial.println("Unknown Controller type found");
//  else if (gamepad_type == 1)
//    Serial.println("DualShock Controller found");
//  else if (gamepad_type == 2)
//    Serial.println("GuitarHero Controller found");
//  else if (gamepad_type == 3)
//    Serial.println("Wireless Sony DualShock Controller found");
//
//  //turn off gamepad vibration
//  gamepad_vibrate = 0;
//
//#ifdef DEF_LEDS
//  //set up LED pins as outputs
//  for (int i = 0; i < 8; i++) {
//    pinMode((RED_LED1 + (4 * i)), OUTPUT);
//    pinMode((GREEN_LED1 + (4 * i)), OUTPUT);
//  }
//#endif
//
//  //set up battery monitor average array
//  for (batt_voltage_index = 0; batt_voltage_index < 50; batt_voltage_index++)
//    batt_voltage_array[batt_voltage_index] = 0;
//  batt_voltage_sum = 0;
//  batt_voltage_index = 0;
//
//  move_lib_init();
//  Serial.println("BOOT OK");
//}
//
//
////***********************************************************************
//// Main Program
////***********************************************************************
//void loop() {
//  //set up frame time
//  currentTime = millis();
//  if ((currentTime - previousTime) < FRAME_TIME_MS)
//    return;
//  previousTime = currentTime;
//
//  //exit if no controller found or GuitarHero controller
//  if ((gamepad_error == 1) || (gamepad_type == 2)) {
//    // Serial.print ("Invalid Controller!");
//    // mode = wm_90_deg;
//    // return;
//  } else {
//    //read controller and process inputs
//    ps2x.read_gamepad(false, gamepad_vibrate);
//    process_gamepad();
//  }
//  //reset legs to home position when commanded
//  if (reset_position) {
//    move_lib_reset_current();
//    reset_position = false;
//  }
//
//  //position legs using IK calculations - unless set all to 90 degrees mode
//  if (mode < wm_90_deg) {
//    for (cur_leg = 0; cur_leg < LEG_NUM; cur_leg++)
//      leg_IK(cur_leg,
//        current_[cur_leg].x + offset_[cur_leg].x + 50,
//        current_[cur_leg].y + offset_[cur_leg].y,
//        current_[cur_leg].z + offset_[cur_leg].z);
//  }
//  //reset leg lift first pass flags if needed
//  if (mode != wm_one_leg) {
//    leg1_IK_control = true;
//    leg6_IK_control = true;
//  }
//
//  // battery_monitor();                        //battery monitor and output to LEDs
//  print_debug();                            //print debug data
//
//  //process modes (mode 0 is default 'home idle' do-nothing mode)
//  switch (mode){
//    case wm_select: {                           //walking mode
//      v3i iinp = {
//        x:map(ps2x.Analog(PSS_RY), 0, 255, 127, -127),
//        y:map(ps2x.Analog(PSS_RX), 0, 255, -127, 127),
//        z:map(ps2x.Analog(PSS_LX), 0, 255, 127, -127)
//      };
//      switch (gait){
//        case ga_tripod:   tripod_gait(iinp);    break;   //walk using gait tripod
//        case ga_wave:     wave_gait(iinp);      break;   //walk using gait wave
//        case ga_ripple:   ripple_gait(iinp);    break;   //walk using gait ripple
//        case ga_tetrapod: tetrapod_gait(iinp);  break;   //walk using gait tetrapod
//      }
//      break;
//    }
//    case wm_xyz: {        //joystick control x-y-z mode
//      translate_control( {
//          x:ps2x.Analog(PSS_RY),
//          y:ps2x.Analog(PSS_RX),
//          z:ps2x.Analog(PSS_LY)
//        });
//      break;
//    }
//    case wm_ypr: {        //joystick control y-p-r mode
//      rotate_control( {
//          x:ps2x.Analog(PSS_RX),
//          y:ps2x.Analog(PSS_RY),
//          z:ps2x.Analog(PSS_LX)
//        }, ps2x.Analog(PSS_LY));
//      break;
//    }
//    case wm_one_leg: {    //one leg lift mode
//      one_leg_lift( {
//          x:ps2x.Analog(PSS_RX),
//          y:ps2x.Analog(PSS_RY),
//          z:ps2x.Analog(PSS_LX)
//        }, ps2x.Analog(PSS_LY));
//      break;
//    }
//    case wm_90_deg: {     //set all servos to 90 degrees mode
//      set_all_90();
//      break;
//    }
//    case wm_default:{
//      wave_gait({x:100, y:100, z:100});
//      break;
//    }
//  }
//
//}
#include <Servo.h>

#define SERVO_MIN 610
#define SERVO_MAX 2400
Servo myservo[3];

int axis[2];
bool SW;

void updateinput() {
  axis[0] = analogRead(A0);
  axis[1] = analogRead(A1);
  SW = !digitalRead(2);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  myservo[0].attach(29);//, SERVO_MIN, SERVO_MAX);
  myservo[1].attach(31);//, SERVO_MIN, SERVO_MAX);
  myservo[2].attach(33);//, SERVO_MIN, SERVO_MAX);
  Serial.print("BOOT ok\n");
}

void loop() {
  updateinput();
  //  Serial.print("A0:");
  Serial.print(axis[0]);
  Serial.print(", ");
  Serial.print(axis[1]);
  Serial.print(", ");
  Serial.print(SW);
  Serial.print("\n");
  myservo[1].write(map(axis[0], 0, 1024, 0, 180));
  myservo[0].write(map(axis[1], 0, 1024, 180, 0));

  delay(1);
}
