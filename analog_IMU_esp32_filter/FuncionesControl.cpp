#include "FuncionesControl.h"

//Controlador Pose
float tobilloL_max = 65, tobilloL_min = 45, servo_range = 10;
float tobilloR_max = 90, tobilloR_min = 70;
float rodillaL_max = 98, rodillaL_min = 78;
float rodillaR_max = 102, rodillaR_min = 82;


//PASO BAJO
float last_tobilloR = tobilloR_ini;
float last_tobilloL = tobilloL_ini;
float last_rodillaL = rodillaL_ini;
float last_rodillaR = rodillaR_ini;


//Controlador Empleando Filtro Paso Bajo
void CorregirPose()
{
  get_MPU_Values(); // leo los valores del MPU y los almaceno
  calculate_tilt_angle(); // traduzco los datos a grados de inclinacion

  float scaled_tobilloR = map(tiltY, (-2*servo_range), (2*servo_range), tobilloR_min, tobilloR_max);
  float scaled_rodillaR = map(tiltZ, (-1*servo_range) , servo_range, rodillaR_max, rodillaR_min);
  float scaled_tobilloL = map(tiltY, (-2*servo_range), (2*servo_range), tobilloL_min, tobilloL_max);
  float scaled_rodillaL = map(tiltZ, (-1*servo_range) , servo_range, rodillaL_min, rodillaL_max);

  float set_tobilloL = (scaled_tobilloL * 0.2) + (last_tobilloL * 0.8);
  float set_tobilloR = (scaled_tobilloR * 0.2) + (last_tobilloR * 0.8); //0.15
  float set_rodillaR = (scaled_rodillaR * 0.05) + (last_rodillaR * 0.95); //0.05
  float set_rodillaL = (scaled_rodillaL * 0.05) + (last_rodillaL * 0.95);

  //Comprobación de límites por seguridad:
  set_tobilloL = constrain(set_tobilloL, tobilloL_min, tobilloL_max);
  set_tobilloR = constrain(set_tobilloR, tobilloR_min, tobilloR_max);
  set_rodillaL = constrain(set_rodillaL, rodillaL_min, rodillaL_max);
  set_rodillaR = constrain(set_rodillaR, rodillaR_min, rodillaR_max);
  
  Servo_Array[TOBILLO_L].write(set_tobilloL);
  Servo_Array[TOBILLO_R].write(set_tobilloR);
  delay(100); // sobrecarga de corriente
  Servo_Array[RODILLA_L].write(set_rodillaL);
  Servo_Array[RODILLA_R].write(set_rodillaR);

  last_tobilloL = set_tobilloL;
  last_tobilloR = set_tobilloR;
  last_rodillaL = set_rodillaL;
  last_rodillaR = set_rodillaR;
  
  Serial.print("Tobillo L & Rodilla L: ");
  Serial.print(set_tobilloL, 3);
  Serial.print("  //  ");
  Serial.println(set_rodillaL, 3);
  Serial.print("Tobillo R & Rodilla R: ");
  Serial.print(set_tobilloR, 3);
  Serial.print("  //  ");
  Serial.println(set_rodillaR, 3);
  Serial.print("\n\n");
}
