#ifndef CONTROL_MODE_H
#define CONTROL_MODE_H

#define X_VECTOR_LENGTH (6) /* estimated state x length */
#define ERROR_VECTOR_LENGTH (X_VECTOR_LENGTH) /* integrated X_VECTOR that tells us our error */
#define U_ROW_LENGTH (4) /* controllerInputU length (vector for controlling servos and torque) */

enum CONTROL_MODE {
    TRACK_MODE = 0,
    STABILIZE_MODE = 1,
    LAND_MODE = 2,
    FINAL_APPROACH_MODE = 3
};

class ControlMode {
public:
  CONTROL_MODE id; //id to identify the control mode
  virtual int init() = 0;
  virtual void switchTo() = 0;
  virtual void update() = 0;
private:

};

#endif