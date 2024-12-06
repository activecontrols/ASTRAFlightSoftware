#include "EncoderModule.h"
#include "Error.h"

int Encoder::init()
{
    for (int i = 0; i < 4; i++) {
        encoders[i].begin();
        encoders[i].setDirection(AS5600_CLOCK_WISE);
        if (!encoders[i].isConnected()) {
            return ENCODER_NOT_ATTACHED;
        }
    }
    return 0;
}

void Encoder::update(unsigned long time) {

    for (int i = 0; i < 4; i++) {
        flightData::encoderPos[i] = encoders[i].readAngle();
    }
    for (int i = 0; i < 4; i++) {
        flightData::encoderSpeeds[i] = encoders[i].getAngularSpeed();
    }
}
