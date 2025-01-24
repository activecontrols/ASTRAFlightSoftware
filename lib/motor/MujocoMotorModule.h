#if IS_MUJOCO
#ifndef MJC_MOTOR_MODULE_H
#define MJC_MOTOR_MODULE_H

#include <Servo.h>
#include "ESC.h"
#include "FlightModule.h"
#include "VEigen.h"
#include "Error.h"

/** Pins for servos and ESC */
#define INNER_GIMBAL_PIN (29)
#define OUTER_GIMBAL_PIN (39)
#define LEFT_TORQUE_VANE_PIN (33)
#define RIGHT_TORQUE_VANE_PIN (36)
#define ESC_PIN (9)

/** Initial settings for servos */
#define INNER_GIMBAL_INITIAL_SETTING (82)
#define OUTER_GIMBAL_INITIAL_SETTING (110)
#define LEFT_TORQUE_VANE_INITIAL_SETTING (140)
#define RIGHT_TORQUE_VANE_INITIAL_SETTING (140)

/** Servo control dampening */
#define BETA_DAMPENING_CONSTANT (0.03)
#define GAMMA_DAMPENING_CONSTANT (0.02)

enum MJC_ACTUATOR {
  MJC_ROLL_MOTOR = 0;
  MJC_BETA_SERVO = 1;
  MJC_GAMMA_SERVO = 2;
  MJC_MAIN_MOTOR = 3;
};


namespace flightData {
    extern Eigen::VectorXd controllerInputU;
}

class MujocoMotorModule : FlightModule {
/**
 * Module to handle sending commands to the servos and ESC in the sim
 * @author Vincent Wang <wang5528@purdue.edu>
 */
public:

  int init() override; /** initialize */
  void update(unsigned long time) override; /** check switch to different control mode is needed, else update control loop of current control mode */
  bool often = true;
  int every_us = 200;
  int timeout_ms = 20;
  
private:

  void controlServos();
  void dampenControls();
};

#endif // MJC_MOTOR_MODULE_H