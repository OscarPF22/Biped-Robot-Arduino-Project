#ifndef FUNCIONESCONTROL_H
#define FUNCIONESCONTROL_H

  #include <Arduino.h>
  #include "funciones_analog.h"
  #include "MPU6050.h"
  #include "servo_robot.h"
  
  //DECLARO ClASE CONTROLADOR PID:
  class PID_Controller
  {
    public:
    void SetTuningValues(double new_Setpoint, double new_Kp, double new_Ki, double new_Kd);
    void Update(double Input);
    double Output;
    
    private:
    //Variables controlador PID
    unsigned long lastTime; // ultima lectura
    double Setpoint;
    double errSum, lastErr; // errSum es el error acumulado (integral del error), lastErr para la derivada del error 
    double kp, ki, kd;
  };
  
  extern PID_Controller pidControllerTobilloR;
  extern PID_Controller pidControllerRodillaR;
  extern PID_Controller pidControllerTobilloL;
  extern PID_Controller pidControllerRodillaL;
  
  void CorregirPose();


#endif
