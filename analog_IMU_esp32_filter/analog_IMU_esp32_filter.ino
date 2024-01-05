#include "funciones_analog.h"
#include "FuncionesControl.h"
#include "servo_robot.h"
#include "MPU6050.h"

unsigned int galgas[8] = {34, 35, 32, 33, 25, 26, 27, 14}; // 4 primeras izq, 4 ultimas der

Servo IniServo_Array[NUM_SERVOS];

void setup() 
{
  Serial.begin(115200); //Para depurar

  //INICIALIZO MPU6050:
  while(!mpu.begin()) 
  {
    Serial.println("Failed to find MPU6050 chip");
    delay(10);
  }
  mpuSetConfig(MPU6050_RANGE_16_G, MPU6050_RANGE_500_DEG, MPU6050_BAND_21_HZ);

  //INICIALIZO SERVOS:
  Servo_Array[RODILLA_R].setPeriodHertz(50); // Standard 50hz servo
  Servo_Array[CADERA_R].setPeriodHertz(50); 
  Servo_Array[TOBILLO_R].setPeriodHertz(50);
  Servo_Array[CADERA2_R].setPeriodHertz(50);
  Servo_Array[RODILLA_L].setPeriodHertz(50);
  Servo_Array[CADERA_L].setPeriodHertz(50);
  Servo_Array[TOBILLO_L].setPeriodHertz(50);
  Servo_Array[CADERA2_L].setPeriodHertz(50);

  //Adjudico pines
  Servo_Array[RODILLA_R].attach(16, 500, 2500); //19
  Servo_Array[CADERA_R].attach(13, 500, 2500);    
  Servo_Array[TOBILLO_R].attach(23, 500, 2500); //18
  Servo_Array[CADERA2_R].attach(17, 500, 2500); 
  Servo_Array[RODILLA_L].attach(19, 500, 2500); //17
  Servo_Array[CADERA_L].attach(18, 500, 2500);  //23  
  Servo_Array[TOBILLO_L].attach(15, 500, 2500); 
  Servo_Array[CADERA2_L].attach(4, 500, 2500);
  
  delay(3000);
  
  RobotInitialize();
  get_MPU_Values();
  
  delay(5000);
}

void loop() 
{
  // Lectura de galgas:
  /*
  ReadVolt(galgas);
  ReadResist();
  ReadWeight(lt2_resist_left, W_left, GALGAS_L);
  ReadWeight(lt2_resist_right, W_right, GALGAS_R);
  CalculoCoG();
  */
  
  //Veo lecturas
  /*Serial.print("Peso (g) Galgas Izq: ");
  Serial.print(W_left[0]);
  Serial.print("  ");
  Serial.print(W_left[1]);
  Serial.print("  ");
  Serial.print(W_left[2]);
  Serial.print("  ");
  Serial.println(W_left[3]);
  /////
  Serial.print("Peso (g) Galgas Der: ");
  Serial.print(W_right[0]);
  Serial.print("  ");
  Serial.print(W_right[1]);
  Serial.print("  ");
  Serial.print(W_right[2]);
  Serial.print("  ");
  Serial.println(W_right[3]);
  Serial.print("\n\n");*/

  //Correción de pose con IMU:
  CorregirPose();

}
