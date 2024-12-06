/* 
MathFunctions.h - Math Functions Header file
Description: Header file to MathFunctions.cpp
Author: Ani Gokhale
*/

#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include "VEigen.h"

namespace math {
    extern Eigen::MatrixXd CBI;

    void calculateCBI(Eigen::VectorXd q);
    Eigen::VectorXd quaternionProduct(Eigen::VectorXd q1, Eigen::VectorXd q2);
    Eigen::VectorXd quaternionConjugate(Eigen::VectorXd q);
    Eigen::Vector3d quatToEuler(Eigen::Quaterniond q);
}

#endif