#ifndef SERVO_LINEAL_H
#define SERVO_LINEAL_H

  #include <Arduino.h>
  #include <ESP32Servo.h>
  #include <ESP32Tone.h>
  #include <ESP32PWM.h>
  
  #define NUM_SERVOS 8
  
  #define RODILLA_R 0
  #define CADERA_R  1
  #define TOBILLO_R 2
  #define CADERA2_R 3
  #define RODILLA_L 4
  #define CADERA_L  5
  #define TOBILLO_L 6
  #define CADERA2_L 7

  #define rodillaR_ini 82 //90
  #define caderaR_ini 87 //95 
  #define tobilloR_ini 80 //55
  #define cadera2R_ini 85 //85 
  
  #define rodillaL_ini 90 //90 
  #define caderaL_ini 95 // 90
  #define tobilloL_ini 55 //80
  #define cadera2L_ini 87 //82
   
  class ServoLineal: public Servo
  {     
      Servo Servo_Array[NUM_SERVOS];
      double Servo_Valores[NUM_SERVOS];
    public:
      void WriteLineal(double Servo_Nuevos_Valores[], double step_time);
      void SetServoValues(double Servo_Nuevos_Valores[]);
      void SetServoArray(Servo MyServo_Array[]);
      void RobotInitialize();
  };
#endif
