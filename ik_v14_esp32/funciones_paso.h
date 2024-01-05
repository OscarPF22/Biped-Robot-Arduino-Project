#ifndef FUNCIONES_PASO_H
#define FUNCIONES_PASO_H

  #include <Arduino.h>
  #include "servo_lineal.h"
  
  const double Long_Pierna = 140;
  
  #define Long_Cadera 65
  #define Long_Rodilla 75
  extern int PASO;
  
  #define SwingingChange 12 //10
  #define SwingingChangeRot 10 //10
  #define SupportChange 5
  #define RotationAngle 20
  
  #define DERECHA 0
  #define IZQUIERDA 1
  extern int PIERNA;
  
  extern double Servo_Nuevos_Valores[NUM_SERVOS];
  
  extern ServoLineal Servos;
  
  // *** FUNCIONES AUX:
  void Swinging(); //balanceo antes de dar un paso, para cambiar el peso de pierna
  void Calculate_Step(double dist); //basicamente teorema de Pitagoras
  void Inverse_Kinematics(double x,double y,double l1,double l2); // calculo ik del paso y lo ejecuto
  void Support(); // apoyo para finalizar la zancada
  void Rotation_Right(); //giro a la der apoyando sobre la izq
  void Error_Func();

#endif
