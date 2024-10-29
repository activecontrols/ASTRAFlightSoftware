#include "MEKF.h"
#include "ArduinoEigenDense.h"


Eigen::Quaterniond estimate;
Eigen::Vector3d pos_estimate;
Eigen::Vector3d vel_estimate;
Eigen::VectorXd gyro_bias(3);
Eigen::VectorXd accel_bias(3);
Eigen::VectorXd mag_bias(3);

Eigen::Matrix3d gyro_cov_mat(3, 3);
Eigen::Matrix3d gyro_bias_cov_mat(3, 3);
Eigen::Matrix3d accel_cov_mat(3,3);
Eigen::Matrix3d accel_bias_cov_mat(3,3);
Eigen::Matrix3d mag_cov_mat(3,3);
Eigen::Matrix3d mag_bias_cov_mat(3,3);

Eigen::MatrixXd estimate_covariance(18,18);
Eigen::MatrixXd observation_covariance(6,6);
Eigen::MatrixXd G(18,18); // part of the state transition matrix x-dot = Gx
Eigen::MatrixXd F_mat(18,18);
Eigen::MatrixXd H(6, 18);
Eigen::MatrixXd inverse_cov(6, 6);
Eigen::MatrixXd K(18, 6);
Eigen::MatrixXd aposteriori_state(18, 1);

// initializes kalmna filter.
void initKalman(Eigen::Quaterniond init_est, double estimate_covar, double gyro_cov, double gyro_bias_cov, double accel_proc_cov, 
                double accel_bias_cov, double mag_proc_cov, double mag_bias_cov, double accel_obs_cov, double mag_obs_cov){
    estimate = init_est;
    estimate_covariance = Eigen::MatrixXd::Identity(18,18) * estimate_covar; // huh
    observation_covariance = Eigen::MatrixXd::Identity(6,6);
    observation_covariance.topLeftCorner(3, 3) = Eigen::MatrixXd::Identity(3, 3) * accel_obs_cov;
    observation_covariance.bottomRightCorner(3, 3) = Eigen::MatrixXd::Identity(3, 3) * mag_obs_cov;
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
Eigen::MatrixXd process_covariance(double time_delta){
    Eigen::MatrixXd Q = Eigen::MatrixXd::Zero(18, 18);

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

void updateKalman(Eigen::VectorXd gyro_meas, Eigen::VectorXd acc_meas, Eigen::VectorXd mag_meas, double time_delta){
    // subtracting biases from measured values
    gyro_meas = gyro_meas - gyro_bias;
    acc_meas = acc_meas - accel_bias;
    mag_meas.normalize();
    mag_meas = mag_meas - mag_bias;

    // using quaternion math to update quaternion estimate with measured angular velocity
    Eigen::Quaterniond angular_velocity;
    angular_velocity.w() = 0;
    angular_velocity.vec() = gyro_meas;
    angular_velocity = estimate * angular_velocity;
    Eigen::Quaterniond q_temp;
    q_temp = angular_velocity.coeffs() * time_delta * 0.5;
    estimate.coeffs() += q_temp.coeffs();
    estimate.normalize();

    Eigen::Vector3d acc_dir(0.0, 0.0, -9.8);
    Eigen::Vector3d mag_dir(1.0, 0.0, 0.0);

    Eigen::Vector3d acc_rot = rotateVector3ByQuat(acc_meas, estimate) - acc_dir;
    vel_estimate += acc_rot * time_delta;
    pos_estimate += vel_estimate * time_delta;

    // constructing current state matrix with new measurements
    G.block<3, 3>(0, 0) = -skewSymmetric(gyro_meas);
    G.block<3, 3>(3, 0) = -estimate.toRotationMatrix() * skewSymmetric(acc_meas);
    G.block<3, 3>(3, 12) = -estimate.toRotationMatrix();
    F_mat = Eigen::MatrixXd::Identity(18, 18) + G * time_delta;


    // generating a priori estimate covariance from our estimate
    // process covariance is added, which is our uncertainty in our measurements as a function of time. Basically, as our
    // time between measurements get longer, the more uncertain we are about what our current measurement is
    estimate_covariance = (F_mat * estimate_covariance * F_mat.transpose()) + process_covariance(time_delta);

    //Serial.println(process_covariance(time_delta)(0, 0) * pow(10, 7));

    // generating Kalman gain based on our observation covariance, which is basically how much we trust our accelerometer and
    // magnetometer over our gyro based estimate. 
    H = Eigen::MatrixXd::Zero(6, 18);

    // creating a matrix with our predicted measurement states to create Kalman gain
    H.block<3, 3>(0, 0) = skewSymmetric(rotateVector3ByQuat(acc_dir, estimate.inverse()));
    H.block<3, 3>(0, 12) = Eigen::MatrixXd::Identity(3, 3);
    H.block<3, 3>(3, 0) = skewSymmetric(rotateVector3ByQuat(mag_dir, estimate.inverse()));
    H.block<3, 3>(3, 15) = Eigen::MatrixXd::Identity(3, 3);
    Eigen::MatrixXd PH_T = Eigen::MatrixXd::Zero(18, 6);
    PH_T = estimate_covariance * H.transpose();
    Eigen::MatrixXd current_obs_covariance = observation_covariance;
    // additionally, we can change our observation covariance based on external factors -> in this case if there is lots of external
    // acceleration, we do not trust our accelerometer attitude measurement.
    current_obs_covariance.topLeftCorner(3, 3) *= getAccelHealth(acc_meas);
    inverse_cov = H * PH_T + current_obs_covariance;
    K = PH_T * inverse_cov.inverse();
    
    // generating a posteriori covariance with Kalman gain, basically updating our confidence in our estimate based on our confidence
    // in our new accelerometer and magnetometer values
    estimate_covariance = (Eigen::MatrixXd::Identity(18, 18) - (K * H)) * estimate_covariance;

    Eigen::MatrixXd observation = Eigen::MatrixXd::Zero(1, 6);
    observation.block<1, 3>(0, 0) = acc_meas;
    observation.block<1, 3>(0, 3) = mag_meas;
    
    Eigen::MatrixXd predicted_observation = Eigen::MatrixXd::Zero(1, 6);
    predicted_observation.block<1, 3>(0, 0) = rotateVector3ByQuat(acc_dir, estimate.inverse());
    predicted_observation.block<1, 3>(0, 3) = rotateVector3ByQuat(mag_dir, estimate.inverse());
    /*Eigen::Quaterniond inv = estimate.inverse();

    // correcting our predicted gyro integrated observation with our magnetometer and accelerometer data
    // using Kalman gain
    aposteriori_state = K * (observation - predicted_observation).transpose();

    // applying the Kalman filter data to our current state.
    // Basically the aposteriori_state returns out a difference between our old estimate and what we think 
    // our new estimate should be. Here we apply those to our values.
    // These are the outputs of this filter.
    Eigen::Quaterniond estimate_to_fold;
    estimate_to_fold.w() = 1.0;
    estimate_to_fold.vec() = 0.5 * aposteriori_state.block<3, 1>(0, 0);
    estimate = estimate * estimate_to_fold;
    estimate.normalize();
    vel_estimate += aposteriori_state.block<3, 1>(3, 0);
    pos_estimate += aposteriori_state.block<3, 1>(6, 0);
    gyro_bias += aposteriori_state.block<3, 1>(9, 0);
    accel_bias += aposteriori_state.block<3, 1>(12, 0);
    mag_bias += aposteriori_state.block<3, 1>(15, 0);
}

float getAccelHealth(Eigen::VectorXd acc){
    float mag = acc.squaredNorm()/pow(9.81, 2.0);
    return 1.0 + (abs(mag - 1.0) * ACCEL_HEALTH_COEFFICIENT);
}

Eigen::MatrixXd skewSymmetric(Eigen::VectorXd v){
    Eigen::MatrixXd m(3, 3);
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