#include "ControllerModule.h"
#include "settings.h"
// #include "EncoderModule.h"
#include "MathFunctions.h"

int Controller::init() {

    modes[STABILIZE_MODE] = (ControlMode*) &stabilize;

  return NO_ERROR_CODE;
}

void Controller::update(unsigned long time) {
    //call current control law based on value in flight data
    modes[flightData::currentMode]->update();

    saturate();
}

void Controller::saturate() {
  //beta
  flightData::controllerInputU(0) = math::constrain(flightData::controllerInputU(0), BETA_MIN, BETA_MAX);

  //gamma
  flightData::controllerInputU(1) = math::constrain(flightData::controllerInputU(1), GAMMA_MIN, GAMMA_MAX);

  //throttle
  flightData::controllerInputU(2) = math::constrain(flightData::controllerInputU(2), THROTTLE_MIN, THROTTLE_MAX);

  //alpha
  flightData::controllerInputU(3) = math::constrain(flightData::controllerInputU(3), ALPHA_MIN, ALPHA_MAX);
}
