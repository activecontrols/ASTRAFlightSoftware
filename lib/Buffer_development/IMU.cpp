#include "IMU.h"
#include "DriverError.h"
#include "LSM6DS_LIS3MDL.h"

/*
IMU.cpp 
Description: Function definititions for declarations in IMU.h
Author: Vincent Palmerio
Last updated: 11/4/2023
*/
float* values;
float roll, pitch, heading = 0;
float gx, gy, gz = 0; //degrees per second on gyro
float qw, qx, qy, qz = 0; //quaternarion
float* getValues() {
  values = (float*)malloc(3 * sizeof(float));
  values[0] = roll;
  values[1] = pitch;
  values[2] = heading;
  return values;
}


int loadPresetCalibration() {
  cal.mag_hardiron[0] = 0.0;
  cal.mag_hardiron[1] = 0.0;
  cal.mag_hardiron[2] = 0.0;

  // in uTesla
  cal.mag_softiron[0] = 1.0;
  cal.mag_softiron[1] = 0.0;
  cal.mag_softiron[2] = 0.0;  
  cal.mag_softiron[3] = 0.0;
  cal.mag_softiron[4] = 1.0;
  cal.mag_softiron[5] = 0.0;  
  cal.mag_softiron[6] = 0.0;
  cal.mag_softiron[7] = 0.0;
  cal.mag_softiron[8] = 1.0;  

  // in Radians/s
  cal.gyro_zerorate[0] = 0.0;
  cal.gyro_zerorate[1] = 0.0;
  cal.gyro_zerorate[2] = 0.0;

  cal.saveCalibration();
}


int initializeIMU() {
  Serial.begin(115200);
  while (!Serial) yield();

  if (!cal.begin()) {
    //Failed to initialize calibration helper
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
    Serial.print("Failed to initialize calibration helper");
#endif
    return FAILED_CALIBRATION_HELPER_INIT;
  }

  if (!cal.loadCalibration()) {
    //No calibration loaded/found
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
    Serial.println("Failed to load calibration");
#endif
    return FAILED_LOAD_CALIBRATION;
  }

  if (!init_sensors()) {
    //Failed to find sensors
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
    Serial.println("Failed to find sensors");
#endif
    return FAILED_SENSOR_INIT;
  }
  
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
  accelerometer->printSensorDetails();
  gyroscope->printSensorDetails();
  magnetometer->printSensorDetails();
#endif

  setup_sensors();
  filter.begin(FILTER_UPDATE_RATE_HZ);
  timestamp = millis();

  Wire.setClock(400000); // 400KHz

  return NO_ERROR_CODE;
}

int updateIMU() {
  
  timestamp = millis();

  // Read the motion sensors
  sensors_event_t accel, gyro, mag;
  accelerometer->getEvent(&accel);
  gyroscope->getEvent(&gyro);
  magnetometer->getEvent(&mag);

  cal.calibrate(mag);
  cal.calibrate(accel);
  cal.calibrate(gyro);

  // Gyroscope needs to be converted from Rad/s to Degree/s
  // the rest are not unit-important
  gx = gyro.gyro.x * SENSORS_RADS_TO_DPS;
  gy = gyro.gyro.y * SENSORS_RADS_TO_DPS;
  gz = gyro.gyro.z * SENSORS_RADS_TO_DPS;

  // Update the SensorFusion filter
  filter.update(gx, gy, gz, 
                accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, 
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);

  // print the heading, pitch and roll
  roll = filter.getRoll();
  pitch = filter.getPitch();
  heading = filter.getYaw();


  //float qw, qx, qy, qz;
  filter.getQuaternion(&qw, &qx, &qy, &qz);


#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)

  Serial.print("I2C took "); Serial.print(millis()-timestamp); Serial.println(" ms");

  Serial.print("Update took "); Serial.print(millis()-timestamp); Serial.println(" ms");

  Serial.print("Raw: ");
  Serial.print(accel.acceleration.x, 4); Serial.print(", ");
  Serial.print(accel.acceleration.y, 4); Serial.print(", ");
  Serial.print(accel.acceleration.z, 4); Serial.print(", ");
  Serial.print(gx, 4); Serial.print(", ");
  Serial.print(gy, 4); Serial.print(", ");
  Serial.print(gz, 4); Serial.print(", ");
  Serial.print(mag.magnetic.x, 4); Serial.print(", ");
  Serial.print(mag.magnetic.y, 4); Serial.print(", ");
  Serial.print(mag.magnetic.z, 4); Serial.println("");

  Serial.print("Orientation: ");
  Serial.print(heading);
  Serial.print(", ");
  Serial.print(pitch);
  Serial.print(", ");
  Serial.println(roll);

  Serial.print("Quaternion: ");
  Serial.print(qw, 4);
  Serial.print(", ");
  Serial.print(qx, 4);
  Serial.print(", ");
  Serial.print(qy, 4);
  Serial.print(", ");
  Serial.println(qz, 4);  
  Serial.print("Took "); Serial.print(millis()-timestamp); Serial.println(" ms");
#endif


  return NO_ERROR_CODE;

}