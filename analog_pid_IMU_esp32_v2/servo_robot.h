#ifndef SERVO_ROBOT_H
#define SERVO_ROBOT_H

  #include <Arduino.h>
  #include <ESP32Servo.h>
  #include <ESP32Tone.h>
  #include <ESP32PWM.h>
  
  #define NUM_SERVOS 8
  
  #define RODILLA_R 0
  #define TOBILLO_R 1
  #define CADERA_R 2
  #define CADERA2_R 3
  #define RODILLA_L 4
  #define TOBILLO_L 5
  #define CADERA_L 6
  #define CADERA2_L 7

  #define rodillaR_ini 92//90
  #define caderaR_ini 87 //95 
  #define tobilloR_ini 80 //55
  #define cadera2R_ini 85 //85 
  
  #define rodillaL_ini 88 //90 
  #define caderaL_ini 95 // 90
  #define tobilloL_ini 55 //80
  #define cadera2L_ini 87 //82
   
  extern Servo Servo_Array[NUM_SERVOS];
  void RobotInitialize();

#endif
