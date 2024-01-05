#include "servo_lineal.h"
#include "funciones_paso.h"

Servo IniServo_Array[NUM_SERVOS];
int zancadas = 1;

void setup() 
{
  Serial.begin(115200); //Para depurar
  
  IniServo_Array[RODILLA_R].setPeriodHertz(50); // Standard 50hz servo
  IniServo_Array[CADERA_R].setPeriodHertz(50); 
  IniServo_Array[TOBILLO_R].setPeriodHertz(50);
  IniServo_Array[CADERA2_R].setPeriodHertz(50);
  IniServo_Array[RODILLA_L].setPeriodHertz(50);
  IniServo_Array[CADERA_L].setPeriodHertz(50);
  IniServo_Array[TOBILLO_L].setPeriodHertz(50);
  IniServo_Array[CADERA2_L].setPeriodHertz(50);

  //Adjudico pines
  IniServo_Array[RODILLA_R].attach(16, 500, 2500); //19
  IniServo_Array[CADERA_R].attach(13, 500, 2500);    
  IniServo_Array[TOBILLO_R].attach(23, 500, 2500); //18
  IniServo_Array[CADERA2_R].attach(17, 500, 2500); 
  IniServo_Array[RODILLA_L].attach(19, 500, 2500); //17
  IniServo_Array[CADERA_L].attach(18, 500, 2500);  //23  
  IniServo_Array[TOBILLO_L].attach(15, 500, 2500); 
  IniServo_Array[CADERA2_L].attach(4, 500, 2500);
  
  Servos.SetServoValues(Servo_Nuevos_Valores);
  Servos.SetServoArray(IniServo_Array);
  
  delay(3000);
  
  Servos.RobotInitialize();

  delay(5000);
}


void loop() 
{

  Swinging(); // BALANCEO INICIAL
  Calculate_Step(PASO); // CALCULO LA IK DEL PASO Y LA EJECUTO
  Support(); // APOYO FINAL
  
  Serial.println("FIN PASO");
  
  zancadas++;
  
  // Doy 16 pasos
  if(zancadas == 12)//8
  {
    zancadas = 0;
    while (zancadas <= 4) // giro 5 veces (90º aprox) //4
    {
      Rotation_Right();
      zancadas++;
      Serial.println("giro");
    }
    zancadas = 0; // Reinicio Cuenta y sigo andando
    delay(200);
  }
}
