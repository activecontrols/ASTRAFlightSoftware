#include <MathFunctions.h>

/*
MathFunctions.cpp
Description:  Defines several helpful math functions, including those declared in MathFunctions.h
Author: Ani Gokhale
*/

namespace math{
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

    Eigen::Quaterniond getConstrainedQuaternion(Eigen::Matrix4Xd* v) {
        //magnitude constraint of quaternion
        //prone to precision float errors
        double constraint = std::sqrt(
            1-
            (*v)(2,0)*(*v)(2,0)-
            (*v)(2,1)*(*v)(2,1)-
            (*v)(2,2)*(*v)(2,2)
        );

        Eigen::Quaterniond e(
            constraint,
            v[2](0),
            v[2](1),
            v[2](0)
        );
        return e;
    }

    void getDeltaX(Eigen::Matrix4Xd* x, Eigen::Matrix4Xd* xRef, Eigen::VectorXd* deltaX) {

        //magnitude of vector in first row
        (*deltaX)(0) = ((*x).row(0) - (*xRef).row(0)).norm();

        //magnitude of vector in second row
        (*deltaX)(1) = ((*x).row(1) - (*xRef).row(1)).norm();

        Eigen::Quaterniond e = getConstrainedQuaternion(x) * getConstrainedQuaternion(xRef);
        //theta (vector of length 1)
        (*deltaX)(2) = 2*std::acos(e.w());

        //magnitude of vector in fourth row
        (*deltaX)(3) = ((*x).row(3) - (*xRef).row(3)).norm();

        return;
    }

    Eigen::Vector3d quatToEuler(Eigen::Quaterniond q){
        Eigen::Vector3d v(0.0, 0.0, 0.0);
        v.x() = atan2(2*(q.w()*q.x() + q.y()*q.z()), 1 - 2*(q.x()*q.x() + q.y()*q.y()));
        v.y() = asin(2*(q.w()*q.y() - q.z()*q.x()));
        v.z() = atan2(2*(q.w()*q.z() + q.x()*q.y()), 1 - 2*(q.y()*q.y() + q.z()*q.z()));
        return v;
    }
}