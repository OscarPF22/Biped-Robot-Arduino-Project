#ifndef FUNCIONES_ANALOG_H
#define FUNCIONES_ANALOG_H

  #include <Arduino.h>

  #define GALGAS_R 1
  #define GALGAS_L 0
  
  // LOOK-UP TABLES:
  #define TAM 19
  extern double lt_volts[TAM];
  extern double lt_resists[TAM];

  #define TAM2 11
  extern double lt_peso[TAM2];
  extern double lt2_resist_left[TAM2];
  extern double lt2_resist_right[TAM2];
  
  // ARRAYS CON LECTURAS: 
  extern double volts[8];
  extern double R[8];
  extern double W_left[4];
  extern double W_right[4];

  // FUNCIONES:
  void ReadVolt(unsigned int *pins);
  void ReadResist();
  void ReadWeight( double lt2_resit[], double W_array[], int pie);
  void CalculoCoG();
  
#endif
