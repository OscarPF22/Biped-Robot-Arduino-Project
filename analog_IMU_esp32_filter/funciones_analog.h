#ifndef FUNCIONES_ANALOG_H
#define FUNCIONES_ANALOG_H

  #include <Arduino.h>

  #define GALGAS_R 1
  #define GALGAS_L 0
  #define TAM2 11
  
  extern double volts[8];
  extern double R[8];
  extern double W_left[4];
  extern double W_right[4];
  extern double lt2_resist_left[TAM2];
  extern double lt2_resist_right[TAM2];
  
  void ReadVolt(unsigned int *pins);
  void ReadResist();
  void ReadWeight(double lt2_resit[], double W_array[], int pie);
  void CalculoCoG();
  
#endif
