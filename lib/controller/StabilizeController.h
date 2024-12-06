#ifndef STABILIZE_CONTROLLER_H
#define STABILIZE_CONTROLLER_H

#include "VEigen.h"
#include "ControlMode.h"

namespace flightData {
  extern Eigen::VectorXd controllerInputU;
  extern Eigen::VectorXd estimatedStateX;
}

class Stabilize : ControlMode {
/**
 * 
 * @author Vincent Palmerio (vpalmeri@purdue.edu)
 */
  Eigen::MatrixXd stabilizationGain;
public:
  Stabilize(CONTROL_MODE id) : stabilizationGain(U_ROW_LENGTH, X_VECTOR_LENGTH) { this->id = id; }
  int init() override;
  void switchTo() override {}; /** sets conditions in data.h variables before switching to TRACK mode */ 
  void update() override; /** runs through control loop every iteration of FlightModule loop */
private:

};

#endif // STABILIZE_CONTROLLER_H