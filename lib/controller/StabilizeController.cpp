#include "StabilizeController.h"
#include "Error.h"

int Stabilize::init() {

    stabilizationGain << 0.010175896789397, -0.999948223723789, -0.000031563385498, 0.010270474752111, -1.009237389930116, -0.000033700327955, 
                -0.006293635967089, -0.000032482311507, -0.999980194349455, -0.006350485144209, -0.000034619292106, -1.009010144715387, 
                0.000000000000000, 0.000000000000001, -0.000000000000003, 0.000000000000000, -0.000000000000000, 0.000000000000004, 
                99.992841807351638, 1.017589389543643, -0.629364064139043, 99.992889040319923, 1.017583962544098, -0.629360802065534;


    return NO_ERROR_CODE;
}

void Stabilize::update() {

    flightData::controllerInputU = -(stabilizationGain * flightData::estimatedStateX);

    flightData::controllerInputU(0) = 180.0 * flightData::controllerInputU(0)/PI;
        
    flightData::controllerInputU(1) = 180.0 * flightData::controllerInputU(1)/PI;

    return;
}