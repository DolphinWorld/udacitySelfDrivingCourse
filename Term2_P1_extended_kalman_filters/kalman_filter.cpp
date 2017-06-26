#include "kalman_filter.h"
#include "tools.h"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;

Tools tools;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void calcKF(const VectorXd &z, const MatrixXd &z_pred);

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
    x_ = F_ * x_;
    MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;
}


void KalmanFilter::calcKF(const VectorXd &z, const MatrixXd &z_pred) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
    //VectorXd z_pred = H_ * x_;
    VectorXd y = z - z_pred;
    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = P_ * Ht;
    MatrixXd K = PHt * Si;

    //new estimate
    x_ = x_ + (K * y);
    long x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;
}

void KalmanFilter::Update(const VectorXd &x) {
    calcKF(x, H_ * x_);
}

void KalmanFilter::UpdateEKF(const VectorXd &x) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */

  VectorXd z = VectorXd(3);
  z(0) = sqrt(x_(0) * x_(0) + x_(1) * x_(1));
  float z0 = z(0);
  if (z0 == 0) {
	z0 = 0.0000001; // avoid being 0
  }
  float v = x_(0);
  if (v == 0) {
	v = 0.000001; // avoid being 0
  }
  z(1) = atan(x_(1) / v);
  z(2) = (x_(0) * x_(2) + x_(1) * x_(3)) / z0;
  calcKF(x, z);
}

