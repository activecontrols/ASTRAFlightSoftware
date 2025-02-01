#ifndef ESTIMATOR_H_INCLUDED
#define ESTIMATOR_H_INCLUDED
void run(double* input, double time, double *output);
Eigen::VectorXd toEigen(double* arr);
double* toArray(Eigen::VectorXd eigen, int size);
#endif // ESTIMATOR_H