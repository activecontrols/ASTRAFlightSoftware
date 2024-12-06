#ifndef CONTROLLER_MODULE_H
#define CONTROLLER_MODULE_H

#include "FlightModule.h"
#include "ControlMode.h"
#include "StabilizeController.h"
#include "Error.h"

/** Constants to constrain control values */
#define BETA_MAX (8) /* outer gimbal max*/
#define BETA_MIN (-BETA_MAX) /* outer gimbal min */
#define GAMMA_MAX (8) /* inner gimbal max */
#define GAMMA_MIN (-GAMMA_MAX) /* inner gimbal min */
#define THROTTLE_MIN (0) /* Must change in ESC.cpp (cannot include controller.h because circular dependencies)*/
#define THROTTLE_MAX (1) /* Must change in ESC.cpp (cannot include controller.h because circular dependencies)*/
#define ALPHA_MAX (8) /* left and right torque vane max */
#define ALPHA_MIN (-ALPHA_MAX) /* left and right torque vane min */


namespace flightData {
    extern Eigen::VectorXd controllerInputU;
    extern CONTROL_MODE currentMode;
}

class Controller : FlightModule {
/**
 * Module to handle switching between control modes and updating the current control mode's respective control loop
 * @author Vincent Palmerio <vpalmeri@purdue.edu>
 */

  Stabilize stabilize = Stabilize(CONTROL_MODE::STABILIZE_MODE);

public:

  int init() override; /** initialize */
  void update(unsigned long time) override; /** check switch to different control mode is needed, else update control loop of current control mode */
  bool often = true;
  int every_us = 200;
  int timeout_ms = 20;
  
private:

  void saturate();

  ControlMode* modes[4];

};

#endif // CONTROLLER_MODULE_H