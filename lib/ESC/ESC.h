#ifndef ESC_H

#define ESC_H

#include <Servo.h>

enum ESC_Status {
    OK,
    INVALID_THROTTLE,
    INVALID_BUFFER_LOC
};

class ESC {
    public:
        ESC(int _throttle_pin);

        ESC_Status setThrottle(float throttle);
        ESC_Status setTLMBufferLocation(int8_t* buffer_loc);
        int8_t getThrottle();
        int8_t getTemp();
        int8_t getVoltageHigh();
        int8_t getVoltageLow();
        int8_t getCurrentHigh();
        int8_t getCurrentLow();
        int8_t getConsumptionHigh();
        int8_t getConsumptionLow();
        int8_t getERMPHigh();
        int8_t getERMPLow();
        int8_t getCRC8();

    private:

    protected:
        int throttle_pin;
        float throttle;
        int8_t* buffer_loc;
        Servo throttle_servo;
};

#endif