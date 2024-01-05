#include "MPU6050.h"
#include <Vector.h>

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
unsigned long last_time = 0;
const float alpha = 0.98;
float tiltX, tiltY, tiltZ;
float offsetX = -83.3, offsetY = -0.4, offsetZ = 83.2;

float accX_array[TAM_MEDIANA] = {-9.4, -9.4, -9.4, -9.4, -9.4, -9.4, -9.4, -9.4, -9.4, -9.4}, 
      accY_array[TAM_MEDIANA] = {-0.45, -0.45, -0.45, -0.45, -0.45, -0.45, -0.45, -0.45, -0.45, -0.45}, 
      accZ_array[TAM_MEDIANA] = {0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6}; 

void mpuSetConfig(mpu6050_accel_range_t accel_range, mpu6050_gyro_range_t gyro_range, mpu6050_bandwidth_t filter_bandwidth)
{
  mpu.setAccelerometerRange(accel_range);
  mpu.setGyroRange(gyro_range);
  mpu.setFilterBandwidth(filter_bandwidth);
}

void get_MPU_Values()
{
  mpu.getEvent(&a, &g, &temp);
  // Por seguridad, fallos en conexiones producen pulsos de alto valor
  a.acceleration.x = constrain(a.acceleration.x, -16, 16);
  a.acceleration.y = constrain(a.acceleration.y, -16, 16);
  a.acceleration.z = constrain(a.acceleration.z, -16, 16);

  array_push_back(accX_array, a.acceleration.x); // guardo el nuevo valor leido en el array
  array_push_back(accY_array, a.acceleration.y);
  array_push_back(accZ_array, a.acceleration.z);

  calculate_median(accX_array, a.acceleration.x); // guardo el valor de la mediana en la variable
  calculate_median(accY_array, a.acceleration.y);
  calculate_median(accZ_array, a.acceleration.z);

  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.println(a.acceleration.z);
}

void calculate_median(float acc_array[], float &acc_value)
{
  float copy_array[TAM_MEDIANA]; // necesito copiar el array porque sino modifico el orden y arruino la función push
  for(unsigned int i = 0; i < TAM_MEDIANA; i++)
  {
    copy_array[i] = acc_array[i];
  }
  
  for(unsigned int i = 0; i < TAM_MEDIANA; i++)
  {
    for(unsigned int j = 0; j < TAM_MEDIANA; j++)
    {
      if (copy_array[i] > copy_array[j]) 
      {
        float temp = copy_array[i];
        copy_array[i] = copy_array[j];
        copy_array[j] = temp;
      }
    }
  }
  
  acc_value = copy_array[TAM_MEDIANA/2];
}

void array_push_back(float acc_array[], float acc_value)
{ 
  for(unsigned int i = 0; i < (TAM_MEDIANA - 1); i++)
  {
    acc_array[i] = acc_array[i + 1];
  }
  acc_array[TAM_MEDIANA - 1] = acc_value;
}

void calculate_tilt_angle()
{
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float dt;
  unsigned long actual_time = millis();
  
  dt = ((float)actual_time - (float)last_time) / 1000.0;
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  
  gyroX = g.gyro.x;
  gyroY = g.gyro.y;
  gyroZ = g.gyro.z;
  
  tiltX = atan2(accX, sqrt(pow(accZ, 2) + pow(accY, 2))) * RAD_TO_DEG;
  tiltY = atan2(accY, sqrt(pow(accZ, 2) + pow(accX, 2))) * RAD_TO_DEG;
  tiltZ = atan2(sqrt(pow(accX, 2) + pow(accY, 2)), accZ) * RAD_TO_DEG;
  
  //Complementary filter:
  tiltX = alpha *(tiltX + gyroX * dt) + (1 - alpha) * accX;
  tiltY = alpha *(tiltY + gyroY * dt) + (1 - alpha) * accY;
  tiltZ = alpha *(tiltZ + gyroZ * dt) + (1 - alpha) * accZ;

  tiltX -= offsetX;
  tiltY -= offsetY;
  tiltZ -= offsetZ;
  
  //Serial.println(dt, 5);
  Serial.print("\nAngle X: ");
  Serial.print(tiltX, 3);
  Serial.print("\nAngle Y: ");
  Serial.print(tiltY, 3);
  Serial.print("\nAngle Z: ");
  Serial.println(tiltZ, 3);

  last_time = actual_time;
}
