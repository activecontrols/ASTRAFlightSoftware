#ifndef CONTROLLER_MODULE_H
#define CONTROLLER_MODULE_H

#include "FlightModule.h"
#include "ControlMode.h"
#include "StabilizeController.h"
#include "Error.h"

#if IS_ARDUINO
    #include <Servo.h>
    #include "ESC.h"
#endif

/** Constants to constrain control values */
#define BETA_MAX (8) /* outer gimbal max*/
#define BETA_MIN (-BETA_MAX) /* outer gimbal min */
#define GAMMA_MAX (8) /* inner gimbal max */
#define GAMMA_MIN (-GAMMA_MAX) /* inner gimbal min */
#define THROTTLE_MIN (0) /* Must change in ESC.cpp (cannot include controller.h because circular dependencies)*/
#define THROTTLE_MAX (1) /* Must change in ESC.cpp (cannot include controller.h because circular dependencies)*/
#define ALPHA_MAX (8) /* left and right torque vane max */
#define ALPHA_MIN (-ALPHA_MAX) /* left and right torque vane min */

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


namespace flightData {
    extern Eigen::VectorXd controllerInputU;
    extern CONTROL_MODE currentMode;
}

class Controller : FlightModule {
/**
 * Module to handle switching between control modes and updating the current control mode's respective control loop
 * @author Vincent Palmerio <vpalmeri@purdue.edu>
 */

  Servo innerGimbal;
  Servo outerGimbal;
  Servo torqueVaneLeft;
  Servo torqueVaneRight;

  ESC edfFan = ESC(ESC_PIN);
  Stabilize stabilize = Stabilize(CONTROL_MODE::STABILIZE_MODE);

public:

  int init() override; /** initialize */
  void update(unsigned long time) override; /** check switch to different control mode is needed, else update control loop of current control mode */
  bool often = true;
  int every_us = 200;
  int timeout_ms = 20;
  
private:

  void saturate();
  void controlServos();
  void dampenControls();

  ControlMode* modes[4];

};

#endif // CONTROLLER_MODULE_H