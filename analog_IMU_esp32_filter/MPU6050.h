#ifndef MPU6050_H
#define MPU6050_H

  #include <Arduino.h>
  #include <Adafruit_MPU6050.h>
  #include <Adafruit_Sensor.h>

  #define TAM_MEDIANA 10
  
  extern Adafruit_MPU6050 mpu;
  extern sensors_event_t a, g, temp;
  extern float tiltX, tiltY, tiltZ;
  void get_MPU_Values();
  void mpuSetConfig(mpu6050_accel_range_t accel_range, mpu6050_gyro_range_t gyro_range, mpu6050_bandwidth_t filter_bandwidth);
  void calculate_tilt_angle();
  void calculate_median(float acc_array[], float &acc_value);
  void array_push_back(float acc_array[], float acc_value);
#endif
