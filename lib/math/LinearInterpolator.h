#ifndef LINEAR_INTERPOLATOR_H
#define LINEAR_INTERPOLATOR_H

#include <ArduinoEigenDense.h>

/*
 * Estimates curve based on linear equation y-y1 = m(x-x1)
 */

class Interpolator {

public:

    Interpolator(int ySize);

    /*
     * Calculates slope, sets parameter y1 to Interpolator::y1 and x1 to Interpolator::x1
     */
    int rebuildLine(float* y1, float* y2, float x1, float x2);

    /*
     * Calculates the the new vector x based on line given by slope and stores it in Interpolator::resultY
     */
    int calculatePoint(float x);

    Eigen::VectorXd resultY;

private:
    float* slope;
    float x1;
    float* y1;
    int ySize;


};


#endif