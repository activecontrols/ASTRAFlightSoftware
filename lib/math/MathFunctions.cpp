#include <MathFunctions.h>

/*
MathFunctions.cpp
Description:  Defines several helpful math functions, including those declared in MathFunctions.h
Author: Ani Gokhale
*/

namespace math_functions{
    Eigen::MatrixXd CBI(3, 3);

    void calculateCBI(Eigen::VectorXd q) {
        CBI <<  1.0 - 2.0*(q(2)*q(2) + q(3)*q(3)),  2*(q(0)*q(3) + q(1)*q(2)),          2*(q(1)*q(3) - q(0)*q(2)),
                2*(q(1)*q(2) - q(0)*q(3)),          1.0 - 2.0*(q(1)*q(1) + q(3)*q(3)),  2*(q(0)*q(1) + q(3)*q(2)),
                2*(q(0)*q(2) + q(1)*q(3)),          2*(q(2)*q(3) - q(0)*q(1)),          1.0 - 2.0*(q(1)*q(1) + q(2)*q(2));
    }

    Eigen::VectorXd quaternionProduct(Eigen::VectorXd p, Eigen::VectorXd q) {
        Eigen::VectorXd qproduct(4);
        qproduct << (p(0)*q(0) - p(1)*q(1) - p(2)*q(2) - q(3)*q(3)), 
                    (p(0)*q(1) + p(1)*q(0) + p(2)*q(3) - p(3)*q(2)),
                    (p(0)*q(2) - p(1)*q(3) + p(2)*q(0) + p(3)*q(1)),
                    (p(0)*q(3) + p(1)*q(2) - p(2)*q(1) + p(3)*q(0));
        return qproduct;
    }

    Eigen::VectorXd quaternionConjugate(Eigen::VectorXd q) {
        Eigen::VectorXd qstar(4);
        qstar << q(0), -q(1), -q(2), -q(3);
        return qstar;
    }
}