#include "function.h"
#include "math.h"
#include "VEigen.h"
#include <iostream>
using namespace std;
double i = 0.1;

Eigen::VectorXd toEigen(double*);
double* toArray(Eigen::VectorXd);

double changeValue(double x) {
    return sin(0.1 * x + (i = 0.1 + i));
}
// void testVector(double* i, double* o) {
//    for (int j = 0; j < 2; j++) {
//        o[j] = changeValue(i[j]);
//    }
//}

void addVectors(double* a, double* b, double* c) {
    Eigen::VectorXd eigenA = toEigen(a);
    Eigen::VectorXd eigenB = toEigen(b);
    Eigen::VectorXd eigenC = eigenA + eigenB;
    cout << "SAMPLE\n" << eigenA << "\n\n" << eigenB << "\n\n" << eigenC << "\n\n";
    memcpy(c, toArray(eigenC), sizeof(double) * 3);
    for (int z = 0; z < 3; z++) {
        cout << c[z] << "\n";
    }
}

Eigen::VectorXd toEigen(double* arr) {
    int size = 3;
    return Eigen::VectorXd::Map(arr, size);
}

double* toArray(Eigen::VectorXd eigen) {
    return eigen.data();
}


// void testVector(const vector<double> i, vector<double> o) {
//    for (int j = 0; j < 2; j++) {
//       o[j] = changeValue(i[j]);
//    }
// }