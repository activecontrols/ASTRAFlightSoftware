#include <debug.h>
#include <Error.h>

#include "IMU.h"
#include "LSM6DS_LIS3MDL.h"

/*
IMU.cpp 
Description: Function definititions for declarations in IMU.h
Author: Vincent Palmerio

*/

//#define ASTRA_IMU_DEBUG

Eigen::VectorXd linearAccelVector(3);
float linearAccelX, linearAccelY, linearAccelZ = 0;
Eigen::VectorXd gyroscopeVector(3);
Eigen::VectorXd magnetometerVector(3);
float roll, pitch, yaw = 0;
float gx, gy, gz = 0; //degrees per second on gyro
float qw, qx, qy, qz = 0; //quaternarion
elapsedMillis totalMillis = 0;
int lastMillis = 0;
elapsedMillis filterMillis = 0;
int lastFilterMillis = 0;
Eigen::Quaterniond testA(1.0, 0.5, 0.5, 0.5);
Eigen::Quaterniond testB(1.0, 1.2, 0.2, 0.2);
Eigen::Vector3d vecTest(5.6, 6.8, -1.0);
Eigen::Matrix3d rotMat;
Eigen::Quaterniond testC;
Eigen::MatrixXd proc_cov;

/*
 * You must free the pointer and set it to NULL after using the pointer!
 */
float* getValues() {
  float* values = (float*)malloc(3 * sizeof(float));
  values[0] = roll;
  values[1] = pitch;
  values[2] = yaw;
  return values;
}


//loads a predetermined calibration into the EEPROM
int loadPresetCalibration() {

#if IMU_NUMBER == 1
  //Magnetic Hard Offset
    cal.mag_hardiron[0] = 18.00;
    cal.mag_hardiron[1] = 37.79;
    cal.mag_hardiron[2] = -84.00;

    //Magnetic Soft Offset
    // in uTesla
    cal.mag_softiron[0] = 0.98;
    cal.mag_softiron[1] = 0.05;
    cal.mag_softiron[2] = 0.01;  
    cal.mag_softiron[3] = 0.05;
    cal.mag_softiron[4] = 1.06;
    cal.mag_softiron[5] = 0.0;  
    cal.mag_softiron[6] = 0.01;
    cal.mag_softiron[7] = 0.0;
    cal.mag_softiron[8] = 0.96;  

  //Gyro zero rate offset
  // in Radians/s
  cal.gyro_zerorate[0] = 0.0;
  cal.gyro_zerorate[1] = 0.0;
  cal.gyro_zerorate[2] = 0.0;

  cal.accel_zerog[0] = 0.25;
  cal.accel_zerog[1] = -0.17;
  cal.accel_zerog[2] = 10.01 - 9.81;
#endif

#if IMU_NUMBER == 2 /* NOT CALIBRATED YET */
  //Magnetic Hard Offset
  cal.mag_hardiron[0] = 0.0;
  cal.mag_hardiron[1] = 0.0;
  cal.mag_hardiron[2] = 0.0;

  //Magnetic Soft Offset
  // in uTesla
  cal.mag_softiron[0] = 0.0;
  cal.mag_softiron[1] = 0.0;
  cal.mag_softiron[2] = 0.0;  
  cal.mag_softiron[3] = 0.0;
  cal.mag_softiron[4] = 0.0;
  cal.mag_softiron[5] = 0.0;  
  cal.mag_softiron[6] = 0.0;
  cal.mag_softiron[7] = 0.0;
  cal.mag_softiron[8] = 0.0;  

  //Gyro zero rate offset
  // in Radians/s
  cal.gyro_zerorate[0] = 0.0;
  cal.gyro_zerorate[1] = 0.0;
  cal.gyro_zerorate[2] = 0.0;
#endif

  if (cal.saveCalibration()) {
    return FAILED_LOAD_CALIBRATION;
  }

  return NO_ERROR_CODE;
}



int initializeIMU() {
  //testA.coeffs() += testB.coeffs();
  //testA.normalize();
  /*rotMat = testA.toRotationMatrix();
  Eigen::Quaterniond v_q;
  v_q.w() = 0.0;
  v_q.vec() = vecTest;
  Eigen::Quaterniond rotated_v_q;
  rotated_v_q = testB.inverse() * v_q * testB.inverse().inverse();
  vecTest = rotated_v_q.vec();*/
  proc_cov = skewSymmetric(vecTest);
  //Serial.begin(115200);
  //while (!Serial) yield();
  //Serial.println("test");
  for (int i = 0; i < linearAccelVector.size(); i++) {
    linearAccelVector(i) = 0;
  }


  if (!cal.begin()) {
    //Failed to initialize calibration helper
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
    Serial.print("Failed to initialize calibration helper");
#endif
    return FAILED_CALIBRATION_HELPER_INIT;
  }
  loadPresetCalibration();
  if (!cal.loadCalibration()) {
    //No calibration loaded/found
    static bool triedLoadPresetCalibration = false;
    if (!triedLoadPresetCalibration) {
      if (loadPresetCalibration() == FAILED_LOAD_CALIBRATION) {
        return FAILED_LOAD_CALIBRATION;
      }
    }
#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)
    Serial.println("Failed to load calibration");
#endif
  
  }

  if (!init_sensors(&IMU_WIRE)) {
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
  initKalman(Eigen::Quaterniond::Identity(), 0.0, 0.0000194955, 0.000000000000000025032, 0.0000000024616355 * 6.0, 0.0, 0.0000000024616355, 0.0, 0.0003 * 6.0, 0.00003);
  //proc_cov = process_covariance(0.03);
  filter.begin(FILTER_UPDATE_RATE_HZ); // not the rate this filter is running at. that is why changing the sample rate is fucking w the reading
  
  Wire.setClock(25000); // 400KHz

  return NO_ERROR_CODE;
}

//sets varaibles declared in IMU.h based on latest data IMU
int updateIMU() {
  
  // Read the motion sensors
  sensors_event_t accel, gyro, mag;
  magnetometer->getEvent(&mag);
  accelerometer->getEvent(&accel);
  gyroscope->getEvent(&gyro);

  
  

  cal.calibrate(mag);
  cal.calibrate(accel);
  cal.calibrate(gyro);
  gx = gyro.gyro.x; //omega x
  gy = gyro.gyro.y; //omega y
  gz = gyro.gyro.z; //omega z

  // Gyroscope needs to be converted from Rad/s to Degree/s
  // the rest are not unit-important

  
  
  // Update the SensorFusion filter
  updateKalman(Eigen::Vector3d(gx, gy, gz), Eigen::Vector3d(-accel.acceleration.x, -accel.acceleration.y, -accel.acceleration.z), 
                Eigen::Vector3d(0.0, 0.0, 0.0), (double)(filterMillis - lastFilterMillis)/1000.0);
  /*filter.update(gx, gy, gz, 
                accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, 
                mag.magnetic.x,mag.magnetic.y,mag.magnetic.z);*/
  
  lastFilterMillis = filterMillis;
  float heading = atan2(mag.magnetic.y, mag.magnetic.x) * 180/(3.14);

  // print the heading, pitch and roll
  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();

  qw = estimate.w();
  qx = estimate.x();
  qy = estimate.y();
  qz = estimate.z();
  //float qw, qx, qy, qz;
  //filter.getQuaternion(&qw, &qx, &qy, &qz);

  
if(totalMillis - lastMillis > 10){

    Serial.print("Quaternion,");
    Serial.print(qw, 5);
    Serial.print(",");
    Serial.print(qx, 5);
    Serial.print(",");
    Serial.print(qy, 5);
    Serial.print(",");
    Serial.print(qz, 5);
    Serial.print(",");
    Serial.print(pos_estimate.x(), 5);
    Serial.print(",");
    Serial.print(pos_estimate.y(), 5);
    Serial.print(",");
    Serial.print(pos_estimate.z(), 5);
    Serial.println();
    Serial.print(proc_cov(1, 0), 5);
    Serial.print(",");
    Serial.print(proc_cov(1, 1), 5);
    Serial.print(",");
    Serial.print(proc_cov(1, 2), 5);
    Serial.println();
    Serial.print(proc_cov(2, 0), 5);
    Serial.print(",");
    Serial.print(proc_cov(2, 1), 5);
    Serial.print(",");
    Serial.print(proc_cov(2, 2), 5);
    Serial.println();
    /*Serial.print(magnetometer->x_gauss, 5);
    Serial.print(",");
    Serial.print(magnetometer->y_gauss, 5);
    Serial.print(",");
    Serial.print(magnetometer->z_gauss, 5);
    Serial.println("");
    Serial.print(magnetometer->xu, 10);
    Serial.print(",");
    Serial.print(magnetometer->yu, 10);
    Serial.print(",");
    Serial.print(magnetometer->zu, 10);
    Serial.println("");
    Serial.print(heading, 5);
    Serial.println("");*/
    
    /*Serial.print("imu,");
    Serial.print(gx, 5);
    Serial.print(",");
    Serial.print(gy, 5);
    Serial.print(",");
    Serial.print(gz, 5);
    Serial.print(",");
    Serial.print(accel.acceleration.x, 5);
    Serial.print(",");
    Serial.print(accel.acceleration.y, 5);
    Serial.print(",");
    Serial.print(accel.acceleration.z, 5);
    Serial.print(",");
    Serial.print((totalMillis - lastMillis)/1000.0, 5);
    Serial.println();*/
    lastMillis = totalMillis;
  }
  



  filter.getLinearAcceleration(&linearAccelX, &linearAccelY, &linearAccelZ); //"a" -  linear acceleration

  linearAccelVector << linearAccelX, linearAccelY, linearAccelZ;
  gyroscopeVector << gx, gy, gz;
  magnetometerVector << mag.magnetic.x, mag.magnetic.y, mag.magnetic.z;

#if defined(ASTRA_FULL_DEBUG) or defined(ASTRA_IMU_DEBUG)

  //Serial.print("I2C took "); Serial.print(millis()-timestamp); Serial.println(" ms");

  //Serial.print("Update took "); Serial.print(millis()-timestamp); Serial.println(" ms");
  Serial.print("Raw: ");
  Serial.print(accel.acceleration.x, 4); Serial.print(", ");
  Serial.print(accel.acceleration.y, 4); Serial.print(", ");
  Serial.print(accel.acceleration.z, 4); Serial.print(", ");
  Serial.println();
  Serial.print(gx, 4); Serial.print(", ");
  Serial.print(gy, 4); Serial.print(", ");
  Serial.print(gz, 4); Serial.print(", ");
  Serial.println();
  Serial.print(mag.magnetic.x, 4); Serial.print(", ");
  Serial.print(mag.magnetic.y, 4); Serial.print(", ");
  Serial.print(mag.magnetic.z, 4); Serial.println("");

  Serial.print("Orientation: ");
  Serial.print(roll);
  Serial.print(", ");
  Serial.print(pitch);
  Serial.print(", ");
  Serial.println(yaw);

  Serial.print("Quaternion: ");
  Serial.print(qw, 4);
  Serial.print(", ");
  Serial.print(qx, 4);
  Serial.print(", ");
  Serial.print(qy, 4);
  Serial.print(", ");
  Serial.println(qz, 4);  
  //Serial.print("Took "); Serial.print(millis()-timestamp); Serial.println(" ms");
  //delay(50);
#endif


  return NO_ERROR_CODE;

}

