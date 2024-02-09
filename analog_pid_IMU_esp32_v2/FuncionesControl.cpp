#include "FuncionesControl.h"

//Controlador PID
PID_Controller pidControllerTobilloR, pidControllerTobilloL, pidControllerRodillaR, pidControllerRodillaL;
float tobilloL_max = 65, tobilloL_min = 45, servo_range = 10;
float tobilloR_max = 90, tobilloR_min = 70;
float rodillaL_max = 98, rodillaL_min = 78;
float rodillaR_max = 102, rodillaR_min = 82;


void PID_Controller::SetTuningValues(double new_Setpoint, double new_Kp, double new_Ki, double new_Kd)
{
   Setpoint = new_Setpoint;
   kp = new_Kp;
   ki = new_Ki;
   kd = new_Kd;
}

void PID_Controller::Update(double Input)
{
   /*How long since we last calculated*/
   unsigned long now = millis();
   double timeChange = (double)(now - lastTime);
  
   /*Compute all the working error variables*/
   double error = Setpoint - Input;
   errSum += (error * timeChange);
   double dErr = (error - lastErr) / timeChange;
  
   /*Compute PID Output*/
   Output = kp * error + ki * errSum + kd * dErr;
  
   /*Remember some variables for next time*/
   lastErr = error;
   lastTime = now;
}

//Controlador PID
void CorregirPose()
{
  get_MPU_Values(); // leo los valores del MPU y los almaceno
  calculate_tilt_angle(); // traduzco los datos a grados de inclinacion
  
  pidControllerTobilloR.Update(tiltY);
  pidControllerRodillaR.Update(tiltZ);
  pidControllerTobilloL.Update(tiltY);
  pidControllerRodillaL.Update(tiltZ);

  Serial.print("PID TobilloL & RodillaL: ");
  Serial.print(pidControllerTobilloL.Output, 3);
  Serial.print("  //  ");
  Serial.println(pidControllerRodillaL.Output, 3);
  Serial.print("Tobillo R & Rodilla R: ");
  Serial.print(pidControllerTobilloR.Output, 3);
  Serial.print("  //  ");
  Serial.println(pidControllerRodillaR.Output, 3);
  Serial.print("\n"); 

  float set_tobilloR = map(pidControllerTobilloR.Output, (-1*servo_range), servo_range, tobilloR_max, tobilloR_min);
  float set_rodillaR = map(pidControllerRodillaR.Output, (-1*servo_range) , servo_range, rodillaR_min, rodillaR_max);
  float set_tobilloL = map(pidControllerTobilloL.Output, (-1*servo_range), servo_range, tobilloL_max, tobilloL_min);
  float set_rodillaL = map(pidControllerRodillaL.Output, (-1*servo_range) , servo_range, rodillaL_max, rodillaL_min);

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
