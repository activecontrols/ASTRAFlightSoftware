#include "MEKF.h"
#include "VEigen.h"


Eigen::Quaterniond estimate;
Eigen::Vector3d gyro_bias;
Eigen::Vector3d accel_bias;
Eigen::Vector3d mag_bias;

Eigen::Matrix3d gyro_cov_mat;
Eigen::Matrix3d gyro_bias_cov_mat;
Eigen::Matrix3d accel_cov_mat;
Eigen::Matrix3d accel_bias_cov_mat;
Eigen::Matrix3d mag_cov_mat;
Eigen::Matrix3d mag_bias_cov_mat;

Eigen::Matrix<double, 18, 18> estimate_covariance;
Eigen::Matrix<double, 6, 6> observation_covariance;
Eigen::Matrix<double, 18, 18> G; // part of the state transition matrix x-dot = Gx
Eigen::Matrix<double, 18, 18> F_mat;
Eigen::Matrix<double, 6, 18> H;
Eigen::Matrix<double, 6, 6> inverse_cov;
Eigen::Matrix<double, 18, 6> K;
Eigen::Matrix<double, 18, 1> aposteriori_state;

void initKalman(Eigen::Quaterniond init_est, double estimate_covar, double gyro_cov, double gyro_bias_cov, double accel_proc_cov, 
                double accel_bias_cov, double mag_proc_cov, double mag_bias_cov, double accel_obs_cov, double mag_obs_cov){
    estimate = init_est;
    estimate_covariance = Eigen::Matrix<double, 18, 18>::Identity(18,18) * estimate_covar; // huh
    observation_covariance = Eigen::Matrix<double, 6, 6>::Identity(6,6);
    observation_covariance.topLeftCorner(3, 3) = Eigen::Matrix<double, 3, 3>::Identity(3, 3) * accel_obs_cov;
    observation_covariance.bottomRightCorner(3, 3) = Eigen::Matrix<double, 3, 3>::Identity(3, 3) * mag_obs_cov;
    gyro_bias.setZero();
    accel_bias.setZero();
    mag_bias.setZero();
    
    F_mat = Eigen::MatrixXd::Zero(18, 18);
    H = Eigen::MatrixXd::Zero(6, 18);
    inverse_cov = Eigen::MatrixXd::Zero(6, 6);
    K = Eigen::MatrixXd::Zero(18, 6);

    G = Eigen::MatrixXd::Zero(18,18);
    G.block<3, 3>(0, 9) = -Eigen::MatrixXd::Identity(3,3);
    G.block<3, 3>(6, 3) = Eigen::MatrixXd::Identity(3,3);

    gyro_cov_mat = Eigen::Matrix3d::Identity(3,3) * gyro_cov;
    gyro_bias_cov_mat = Eigen::Matrix3d::Identity(3,3) * gyro_bias_cov;
    accel_cov_mat = Eigen::Matrix3d::Identity(3,3) * accel_proc_cov;
    accel_bias_cov_mat = Eigen::Matrix3d::Identity(3,3) * accel_bias_cov;
    mag_cov_mat = Eigen::Matrix3d::Identity(3,3) * mag_proc_cov;
    mag_bias_cov_mat = Eigen::Matrix3d::Identity(3,3) * mag_bias_cov;
}

// taylor series approximation of the process covariance matrix, linearized around omega = 0 = accelerometer reading, and rotation = Identity matrix
// what does that mean: I'm only about 50% sure..
Eigen::Matrix<double, 18, 18> process_covariance(double time_delta){
    Eigen::Matrix<double, 18, 18> Q = Eigen::MatrixXd::Zero(18, 18);

    Q.block<3, 3>(0, 0) = gyro_cov_mat * time_delta + gyro_bias_cov_mat * (pow(time_delta, 3)/3.0);
    Q.block<3, 3>(0, 9) = gyro_bias_cov_mat * (-pow(time_delta, 2)/2.0);
    Q.block<3, 3>(3, 3) = accel_cov_mat * time_delta + accel_bias_cov_mat * (pow(time_delta, 3)/3.0);
    Q.block<3, 3>(3, 6) = accel_cov_mat * (pow(time_delta, 2)/2.0) + accel_bias_cov_mat * (pow(time_delta, 4)/8.0);
    Q.block<3, 3>(3, 12) = accel_bias_cov_mat * (-pow(time_delta, 2)/2.0);
    Q.block<3, 3>(6, 3) = accel_cov_mat * (pow(time_delta, 2)/2.0) + accel_bias_cov_mat * (pow(time_delta, 4)/8.0);
    Q.block<3, 3>(6, 6) = accel_cov_mat * (pow(time_delta, 3)/3.0) + accel_bias_cov_mat * (pow(time_delta, 5)/20.0);
    Q.block<3, 3>(6, 12) = accel_bias_cov_mat * (-pow(time_delta, 3)/6.0);
    Q.block<3, 3>(9, 0) = gyro_bias_cov_mat * (-pow(time_delta, 2)/2.0);
    Q.block<3, 3>(9, 9) = gyro_bias_cov_mat * time_delta;
    Q.block<3, 3>(12, 3) = accel_bias_cov_mat * (-pow(time_delta, 2)/2.0);
    Q.block<3, 3>(12, 6) = accel_bias_cov_mat * (-pow(time_delta, 3)/6.0);
    Q.block<3, 3>(12, 12) = accel_bias_cov_mat * time_delta;
    Q.block<3, 3>(15, 15) = mag_bias_cov_mat * time_delta;

    return Q;
}

void updateKalman(Eigen::Vector3d gyro_meas, Eigen::Vector3d acc_meas, Eigen::Vector3d mag_meas, double time_delta){
    gyro_meas = gyro_meas - gyro_bias;
    acc_meas = acc_meas - accel_bias;
    mag_meas = mag_meas - mag_bias;

    Eigen::Quaterniond angular_velocity;
    angular_velocity.w() = 0;
    angular_velocity.vec() = gyro_meas;
    angular_velocity = estimate * angular_velocity;
    Eigen::Quaterniond q_temp;
    q_temp = angular_velocity.coeffs() * time_delta * 0.5;
    estimate.coeffs() += q_temp.coeffs();
    estimate.normalize();

    G.block<3, 3>(0, 0) = -skewSymmetric(gyro_meas);
    G.block<3, 3>(3, 0) = -estimate.toRotationMatrix() * skewSymmetric(acc_meas);
    G.block<3, 3>(3, 12) = -estimate.toRotationMatrix();
    F_mat = Eigen::MatrixXd::Identity(18, 18) + G * time_delta;


    estimate_covariance = (F_mat * estimate_covariance * F_mat.transpose()) + process_covariance(time_delta);
    //Serial.println(process_covariance(time_delta)(0, 0) * pow(10, 7));

    H = Eigen::MatrixXd::Zero(6, 18);
    Eigen::Vector3d acc_dir(0.0, 0.0, -1.0);
    Eigen::Vector3d mag_dir(1.0, 0.0, 0.0);

    H.block<3, 3>(0, 0) = skewSymmetric(rotateVector3ByQuat(acc_dir, estimate.inverse()));
    H.block<3, 3>(0, 12) = Eigen::MatrixXd::Identity(3, 3);
    H.block<3, 3>(3, 0) = skewSymmetric(rotateVector3ByQuat(mag_dir, estimate.inverse()));
    H.block<3, 3>(3, 15) = Eigen::MatrixXd::Identity(3, 3);
    Eigen::Matrix<double, 18, 6> PH_T = Eigen::MatrixXd::Zero(18, 6);
    PH_T = estimate_covariance * H.transpose();
    inverse_cov = H * PH_T + observation_covariance;
    K = PH_T * inverse_cov.inverse();
    
    estimate_covariance = (Eigen::MatrixXd::Identity(18, 18) - (K * H)) * estimate_covariance;

    Eigen::Matrix<double, 1, 6> observation = Eigen::MatrixXd::Zero(1, 6);
    observation.block<1, 3>(0, 0) = acc_meas;
    observation.block<1, 3>(0, 3) = mag_meas;
    
    Eigen::Matrix<double, 1, 6> predicted_observation = Eigen::MatrixXd::Zero(1, 6);
    predicted_observation.block<1, 3>(0, 0) = rotateVector3ByQuat(acc_dir, estimate.inverse());
    predicted_observation.block<1, 3>(0, 3) = rotateVector3ByQuat(mag_dir, estimate.inverse());
    /*Eigen::Quaterniond inv = estimate.inverse();

    Serial.print("acc, mag, ");
    Serial.print(inv.w(), 5);
    Serial.print(",");
    Serial.print(inv.x(), 5);
    Serial.print(",");
    Serial.print(inv.y(), 5);
    Serial.print(",");
    Serial.print(inv.z(), 5);
    Serial.print(",");
    Serial.print(predicted_observation(0, 0), 5);
    Serial.print(",");
    Serial.print(predicted_observation(0, 1), 5);
    Serial.print(",");
    Serial.print(predicted_observation(0, 2), 5);
    Serial.println("");*/

    aposteriori_state = K * (observation - predicted_observation).transpose();

    Eigen::Quaterniond estimate_to_fold;
    estimate_to_fold.w() = 1.0;
    estimate_to_fold.vec() = 0.5 * aposteriori_state.block<3, 1>(0, 0);
    estimate = estimate * estimate_to_fold;
    estimate.normalize();
    gyro_bias += aposteriori_state.block<3, 1>(9, 0);
    accel_bias += aposteriori_state.block<3, 1>(12, 0);
    mag_bias += aposteriori_state.block<3, 1>(15, 0);
}

Eigen::Matrix3d skewSymmetric(Eigen::VectorXd v){
    Eigen::Matrix3d m(3, 3);
    m << 0.0, -v(2), v(1),
        v(2), 0.0, -v(0),
        -v(1), v(0), 0.0;
    return m;
}

Eigen::Vector3d rotateVector3ByQuat(Eigen::Vector3d v, Eigen::Quaterniond q){
    Eigen::Quaterniond v_q;
    v_q.w() = 0.0;
    v_q.vec() = v;
    Eigen::Quaterniond rotated_v_q;
    rotated_v_q = q * v_q * q.inverse();
    Eigen::Vector3d vec_ret(rotated_v_q.x(), rotated_v_q.y(), rotated_v_q.z());
    return vec_ret;
}