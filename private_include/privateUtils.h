//
// Created by shobhit on 26.10.24.
//
#ifndef PRIVATE_UTILS
#define PRIVATE_UTILS

#include <coder_array.h>
#include <Eigen/Dense>
#include <vector>
#include <string>


Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> coderToEigen(::coder::array<double, 2U> const &inputArray);


::coder::array<double, 2U> EigenToCoder(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> const &inputEigenArray) ;

::coder::array<double, 1U> EigenVectorToCoder1U(Eigen::VectorXd const &vec) ;

Eigen::VectorXd coder1UtoEigenVector(::coder::array<double, 1U> const &vec) ;

#endif
