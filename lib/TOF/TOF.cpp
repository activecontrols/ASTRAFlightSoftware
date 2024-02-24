#include <TOF.h>

/*
TOF.cpp 
Description: Function definititions for declarations in TOF.h
Author: Henry Hellman, Ani Gokhale

Last updated: 2/21/2024

*/

Buffer TOF_buffer(1, NUM_TOF_MEASUREMENTS, updateTOFDistance);
volatile int interruptCount;
VL53L4CX sensor_vl53l4cx_sat;
float* latestData;

int setupTOFSensor(unsigned int interruptPin, unsigned int XSHUTPin) {
    interruptCount = 0;
    float initialData = 0;
    latestData = &initialData;

    sensor_vl53l4cx_sat = VL53L4CX(&DEV_I2C, XSHUTPin);

    VL53L4CX_Error status;
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), measure, FALLING);

    // Initialize I2C bus.
    // DEV_I2C.begin();

    // Configure VL53L4CX satellite component.
    sensor_vl53l4cx_sat.begin();

    // Switch off VL53L4CX satellite component.
    sensor_vl53l4cx_sat.VL53L4CX_Off();

    // Initialize VL53L4CX satellite component.
    status = sensor_vl53l4cx_sat.InitSensor(0x29);

    sensor_vl53l4cx_sat.VL53L4CX_StartMeasurement();

    return status;
}

float* updateTOFDistance() {
    if (interruptCount == 1) {
        interruptCount = 0;
        float* temp = (float*)malloc(1 * sizeof(float));
        VL53L4CX_MultiRangingData_t MultiRangingData;
        VL53L4CX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
        uint8_t NewDataReady = 0;
        int no_of_object_found = 0, j;

        int status = sensor_vl53l4cx_sat.VL53L4CX_GetMeasurementDataReady(&NewDataReady);

        if (!status) {
            status = sensor_vl53l4cx_sat.VL53L4CX_GetMultiRangingData(pMultiRangingData);

            temp[0] = pMultiRangingData->RangeData[j].RangeMilliMeter / 1000.0;
            Serial.println(pMultiRangingData->RangeData[j].RangeMilliMeter / 1000.0);

            if (status == 0) {
                status = sensor_vl53l4cx_sat.VL53L4CX_ClearInterruptAndStartMeasurement();
            }
        } else {
            return latestData;
        }
    } else {
        return latestData;
    }
    return latestData;
}

void measure() {
    interruptCount = 1;
    Serial.println("Getting here");
}