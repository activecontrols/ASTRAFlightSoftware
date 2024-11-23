#include "MotorModule.h"
#include "settings.h"
#include "EncoderModule.h"

int MotorModule::init() {
  #if ENABLE_MOTOR_CONTROL
    innerGimbal.attach(INNER_GIMBAL_PIN);
    outerGimbal.attach(OUTER_GIMBAL_PIN);
    torqueVaneLeft.attach(LEFT_TORQUE_VANE_PIN);
    torqueVaneRight.attach(RIGHT_TORQUE_VANE_PIN);

    if (!innerGimbal.attached()) {
      return INNER_GIMBAL_NOT_ATTACHED;
    }

    if (!outerGimbal.attached()) {
      return OUTER_GIMBAL_NOT_ATTACHED;
    }

    if (!torqueVaneLeft.attached()) {
      return LEFT_TORQUE_VANE_NOT_ATTACHED;
    }

    if (!torqueVaneRight.attached()) {
      return RIGHT_TORQUE_VANE_NOT_ATTACHED;
    }

    innerGimbal.write(INNER_GIMBAL_INITIAL_SETTING);
    outerGimbal.write(OUTER_GIMBAL_INITIAL_SETTING);
    torqueVaneLeft.write(LEFT_TORQUE_VANE_INITIAL_SETTING);
    torqueVaneRight.write(RIGHT_TORQUE_VANE_INITIAL_SETTING);
  #endif

  #if USE_EDF
    if (!edfFan.checkAttached()) {
      return ESC_NOT_ATTACHED;
    }
  #endif

  return 0;
}

void MotorModule::update(unsigned long time) {
    dampenControls();
    controlServos();
}

void MotorModule::dampenControls() {
  #if USE_ENCODER
    float beta_dot = flightData::encoderSpeeds[OUTER_GIMBAL_INDEX];
    float gamma_dot = flightData::encoderSpeeds[INNER_GIMBAL_INDEX];

    //TODO: servo dampening needs to happen for the vanes

    flightData::controllerInputU(1) = flightData::controllerInputU(1) + BETA_DAMPENING_CONSTANT*beta_dot;
    flightData::controllerInputU(0) = flightData::controllerInputU(0) + GAMMA_DAMPENING_CONSTANT*gamma_dot;
  #endif
}

void MotorModule::controlServos() {
  #if ENABLE_MOTOR_CONTROL
    innerGimbal.write(flightData::controllerInputU(0) + INNER_GIMBAL_INITIAL_SETTING); //write gamma to inner gimbal
    outerGimbal.write(flightData::controllerInputU(1) + OUTER_GIMBAL_INITIAL_SETTING); //write beta to outer gimbal
    
    torqueVaneLeft.write(flightData::controllerInputU(3) + LEFT_TORQUE_VANE_INITIAL_SETTING); //write alpha to left torque vane
    torqueVaneRight.write(-flightData::controllerInputU(3) + RIGHT_TORQUE_VANE_INITIAL_SETTING); //write -alpha to right torque vane
  #endif

  #if USE_EDF
    int errorCode = edfFan.setThrottle(flightData::controllerInputU(2)); //write throttle to ESC (power), which then talks to EDF (fan))
  #endif
}
