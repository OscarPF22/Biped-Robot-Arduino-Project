#include "funciones_analog.h"

//LOOK-UP TABLES:
#define TAM 19
double lt_volts[TAM] =   {0.738, 0.915, 1.10, 1.257, 1.43, 1.61, 1.81, 1.97, 2.14, 2.27, 2.39, 2.47, 2.55, 2.61, 2.66, 2.95, 3.15, 3.30, 3.35};
double lt_resists[TAM] = { 900,   1100, 1300, 1500,  1700, 1900, 2200, 2600, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 9000, 9500, 11000, 12000};

#define TAM2 11
double lt_peso[TAM2] =    {100,    200,   300,  400,  500,  600,  700,  800,  900, 1000, 1100};
double lt2_resist_left[TAM2] = {12000, 11000, 9650, 8500, 5700, 3800, 2800, 2350, 2000, 1100, 900};
double lt2_resist_right[TAM2] = {12000, 8000,  6500, 5000, 4500, 4000, 3500, 3000, 2800, 2600, 2300};

double volts[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double R[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double W_left[4] = {0.0, 0.0, 0.0, 0.0};
double W_right[4] = {0.0, 0.0, 0.0, 0.0};

// Coordenadas de las galgas en los extremos del pie (en cm)
double x[4] = {-3.0, 3.0, 3.0, -3.0};
double y[4] = {2.5, 2.5, -2.5, -2.5};

// Resultado del cálculo del centro de gravedad
double R_CoGx, R_CoGy, L_CoGx, L_CoGy; 

//FUNCIONES LECTURA VOLTAJE, RESISTENCIA Y PESO DE LA GALGA:
void ReadVolt(unsigned int *pins)
{
  volts[0] = 3.3*(double)analogRead(pins[0])/4095.0;
  volts[1] = 3.3*(double)analogRead(pins[1])/4095.0;
  volts[2] = 3.3*(double)analogRead(pins[2])/4095.0;
  volts[3] = 3.3*(double)analogRead(pins[3])/4095.0;
  volts[4] = 3.3*(double)analogRead(pins[4])/4095.0;
  volts[5] = 3.3*(double)analogRead(pins[5])/4095.0;
  volts[6] = 3.3*(double)analogRead(pins[6])/4095.0;
  volts[7] = 3.3*(double)analogRead(pins[7])/4095.0;
}

void ReadResist()
{
  unsigned int j = 0;
  double n = 0.0;
  double Rdif = 0.0;
  
  for(unsigned int i = 0; i < 8; i++)
  {
    if(volts[i] <= lt_volts[0]) // compruebo que entre en los limites de la tabla
    {
      R[i] = lt_resists[0];
    }
    else if(volts[i] >= lt_volts[TAM-1])
    {
      R[i] = lt_resists[TAM-1];
    }
    else // si entra 
    {
      while(j < TAM) //encuentro el punto superior al valor e interpolo
      {
        if(volts[i] > lt_volts[j]) j++;
        else
        {
          n = (volts[i]-lt_volts[j-1]) / (lt_volts[j]-lt_volts[j-1]);
          Rdif = lt_resists[j] - lt_resists[j-1];
          R[i] = lt_resists[j-1] + n * Rdif;
          j = TAM; // me salgo del bucle
        }
      }
    }
    j = 0;
  }
}

void ReadWeight( double lt2_resit[], double W_array[], int pie)
{
  unsigned int j = 0;
  double n = 0.0;
  double Wdif = 0.0;

  for(unsigned int i = 0; i < 4; i++)
  {
    if(R[i + 4*pie] >= lt2_resit[0]) // compruebo que entre en los limites de la tabla
    {
      W_array[i] = lt_peso[0];
    }
    else if(R[i + 4*pie] <= lt2_resit[TAM2-1])
    {
      W_array[i] = lt_peso[TAM2-1];
    }
    else // si entra 
    {
      while(j < TAM2) //encuentro el punto superior al valor e interpolo
      {
        if(R[i+ 4*pie] < lt2_resit[j]) j++;
        else
        {
          n = (R[i + 4*pie]-lt2_resit[j-1]) / (lt2_resit[j]-lt2_resit[j-1]);
          Wdif = lt_peso[j] - lt_peso[j-1];
          W_array[i] = lt_peso[j-1] + n * Wdif;
          j = TAM2; // me salgo del bucle
        }
      }
    }
    j = 0;
  }
}

void CalculoCoG()
{
  double W_right_total = W_right[0] + W_right[1] + W_right[2] + W_right[3];
  double W_left_total = W_left[0] + W_left[1] + W_left[2] + W_left[3];
  
  R_CoGx = ( (W_right[0] * x[0]) + (W_right[1] * x[1]) + (W_right[2] * x[2]) + (W_right[3] * x[3]) ) / (W_right_total); 
  R_CoGy = ( (W_right[0] * y[0]) + (W_right[1] * y[1]) + (W_right[2] * y[2]) + (W_right[3] * y[3]) ) / (W_right_total);
  L_CoGx = ( (W_left[0] * x[0]) + (W_left[1] * x[1]) + (W_left[2] * x[2]) + (W_left[3] * x[3]) ) / (W_left_total);
  L_CoGy = ( (W_left[0] * y[0]) + (W_left[1] * y[1]) + (W_left[2] * y[2]) + (W_left[3] * y[3]) ) / (W_left_total);
}
