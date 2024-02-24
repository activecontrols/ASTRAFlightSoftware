#include <Encoder.h>

AS5600 magEncoder1(&Wire); /* outer gimbal */
AS5600 magEncoder2(&Wire1); /* inner gimbal */

bool encoderSetup()
{
    magEncoder1.begin();  //  set direction pin.
    magEncoder1.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool a = magEncoder1.isConnected();

    magEncoder2.begin();  //  set direction pin.
    magEncoder2.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
    bool b = magEncoder2.isConnected();

    return a && b;
}

float* getAngleEncoder1() {
    float* angle = (float*)malloc(1 * sizeof(float));
    angle[0] = (float) magEncoder1.readAngle();
    return angle;
}

float* getAngleEncoder2() {
    float* angle = (float*)malloc(1 * sizeof(float));
    angle[0] = (float) magEncoder2.readAngle();
    return angle;
}

float getSpeedEncoder2() {
    return magEncoder2.getAngularSpeed();
}

float getSpeedEncoder1() {
    return magEncoder1.getAngularSpeed();
}