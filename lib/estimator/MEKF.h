#ifndef MEKF_H
#define MEKF_H

#include "VEigen.h"

extern Eigen::Quaterniond estimate;

void initKalman(Eigen::Quaterniond init_est, double estimate_covar, double gyro_cov, double gyro_bias_cov, double accel_proc_cov, 
                double accel_bias_cov, double mag_proc_cov, double mag_bias_cov, double accel_obs_cov, double mag_obs_cov);
Eigen::MatrixXd process_covariance(double time_delta);
void updateKalman(Eigen::VectorXd gyro_meas, Eigen::VectorXd acc_meas, Eigen::VectorXd mag_meas, double time_delta);
Eigen::MatrixXd skewSymmetric(Eigen::VectorXd v);
Eigen::Vector3d rotateVector3ByQuat(Eigen::Vector3d v, Eigen::Quaterniond q);
#endif