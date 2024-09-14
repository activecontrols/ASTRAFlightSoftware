#ifndef ESC_H

#define ESC_H

#include <Error.h>
#include <Servo.h>

class ESC {
    public:
        ESC(int _throttle_pin);

        int setThrottle(float throttle);
        int setTLMBufferLocation(int8_t* buffer_loc);
        bool checkAttached();
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