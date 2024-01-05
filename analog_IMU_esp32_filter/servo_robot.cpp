#include "servo_robot.h"

Servo Servo_Array[NUM_SERVOS];

void RobotInitialize()
{
  //Creado dentro de la función para uso mas eficiente de la meoria
  double Servo_Valores[NUM_SERVOS] = { rodillaR_ini, tobilloR_ini, caderaR_ini, cadera2R_ini, rodillaL_ini, tobilloL_ini, caderaL_ini, cadera2L_ini };
                                          
  for(int i = 0; i < (NUM_SERVOS/2); i++)
  {
     Servo_Array[i].write(Servo_Valores[i]);
     Servo_Array[i+(NUM_SERVOS/2)].write(Servo_Valores[i+(NUM_SERVOS/2)]);
     
     Serial.println("Servos Inicializados...");
     
     delay(200); // delay de 100 ms entre cada movimiento de motor para no demandar excesiva corriente
  }
  /*//Para que no consuman corriente ya que no los uso una vez inicializados
  Servo_Array[CADERA_R].detach();
  Servo_Array[CADERA2_R].detach();
  Servo_Array[CADERA_L].detach();
  Servo_Array[CADERA2_L].detach();*/
}
