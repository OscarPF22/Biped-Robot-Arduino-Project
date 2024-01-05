#include "servo_lineal.h"

void ServoLineal::WriteLineal(double Servo_Nuevos_Valores[], double step_time)
{
  const double inc_time = 20000.0; // 50 Hz
  double inc[NUM_SERVOS]; // array que contiene los incrementos de posición en cada ciclo
  
  unsigned long previous; // tiempo previo para calcular cuando se hace el siguiente incremento
  unsigned out = 0; // variable de control para salir del bucle
  char buf[50];

  for(int i = 0; i < NUM_SERVOS; i++) // comparo los valores que tengo que actualizar y calculo sus incrementos
  {
    inc[i] = ((Servo_Nuevos_Valores[i] - Servo_Valores[i])*inc_time)/step_time;

    //Para debugear
    sprintf(buf, "Incremento [%d] vale ... %f", i, inc[i]);
    Serial.println(buf);
    
    if(inc[i] == 0) // si es cero es porque los valores eran igual (se mantienen)
    {
      out++;
    }
  }

  previous = micros(); // calculo tiempo previo al cambio
  
  while( out != NUM_SERVOS )
  {
    for(int i = 0; i < NUM_SERVOS; i++) 
    {
      if(inc[i] != 0)
      {
        Servo_Valores[i] += inc[i]; // actualizo valor
        Servo_Array[i].Servo::write(Servo_Valores[i]); // ejecuto movimiento
        
        if((abs(Servo_Nuevos_Valores[i] - Servo_Valores[i]) < abs(inc[i]))) // si ya he llegado a la posición deseada
        {
          inc[i] = 0; // cancelo el incremento
          
          Servo_Valores[i] = Servo_Nuevos_Valores[i]; // igualo valor (casos en los que no se llega al valor exacto)
          Servo_Array[i].Servo::write(Servo_Valores[i]); // ejecuto movimiento
          
          out++; // aumento la cuenta de salida
        }

        //Para debugear
        sprintf(buf, "Servo %d a .... %f, con inc %f", i,  Servo_Valores[i], inc[i]);
        Serial.println(buf);
      }
    }
    while( micros() < (previous + inc_time) ) // espera activa
    {
    }
    
    previous = micros();
  }
}

void ServoLineal::SetServoValues(double Servo_Nuevos_Valores[])
{
  for(int i = 0; i < NUM_SERVOS; i++)
  {
     Servo_Valores[i] = Servo_Nuevos_Valores[i];
  }
  Serial.println("Valores de servo asignados...");
}

void ServoLineal::SetServoArray(Servo MyServo_Array[])
{
  for(int i = 0; i < NUM_SERVOS; i++)
  {
     Servo_Array[i] = MyServo_Array[i];
  }
  Serial.println("Array de servo asignado...");
}
void ServoLineal::RobotInitialize()
{
  char buf[60];
  for(int i = 0; i < (NUM_SERVOS/2); i++)
  {
     Servo_Array[i].write(Servo_Valores[i]);
     Servo_Array[i+(NUM_SERVOS/2)].write(Servo_Valores[i+(NUM_SERVOS/2)]);

     //Para debugear
     sprintf(buf, "Servos %d y %d inicializados a .... %f y %f",  i,  (i+(NUM_SERVOS/2)), Servo_Valores[i], Servo_Valores[i+(NUM_SERVOS/2)]);
     Serial.println(buf);
     
     delay(100); // delay de 100 ms entre cada movimiento de motor para no demandar excesiva corriente
  }
}
