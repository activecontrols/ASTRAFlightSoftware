/**
 * This module defines the Basic Controller (BController), a simple proof-of-concept
 * Model-Predictive Controller (MPC) using Acados. This controller uses simplified
 * EOMs, is written by someone who doesn't know control theory, and generally should
 * not be used in production.
 * - Vincent Wang <vwangsf@gmail.com>
 */
#ifndef BCONTROLLER_MODULE_H
#define BCONTROLLER_MODULE_H

#include "FlightModule.h"
#include "VEigen.h"
#include "ASTRAError.h"

// /** Constants to constrain control values */
// #define BETA_MAX (8) /* outer gimbal max*/
// #define BETA_MIN (-BETA_MAX) /* outer gimbal min */
// #define GAMMA_MAX (8) /* inner gimbal max */
// #define GAMMA_MIN (-GAMMA_MAX) /* inner gimbal min */
// #define THROTTLE_MIN (0) /* Must change in ESC.cpp (cannot include controller.h because circular dependencies)*/
// #define THROTTLE_MAX (1) /* Must change in ESC.cpp (cannot include controller.h because circular dependencies)*/
// #define ALPHA_MAX (8) /* left and right torque vane max */
// #define ALPHA_MIN (-ALPHA_MAX) /* left and right torque vane min */

namespace flightData {
    extern Eigen::VectorXd controllerInputU;
    extern Eigen::VectorXd estimatedStateX;
}

class ControllerModule : FlightModule {
public:

  int init() override; /** initialize */
  void update(unsigned long time) override; /* Control loop */
  bool often = true;
  int every_us = 200;
  int timeout_ms = 20;
  
private:
  void update_state();
};

#endif // BCONTROLLER_MODULE_H