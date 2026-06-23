//
// Created by shobhit on 26.10.24.
//
#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_PRIVATEUTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_PRIVATEUTILS_H

#include <coder_array.h>
#include <Eigen/Dense>
#include <vector>
#include <string>


/**
 * @brief Convert a 2D MATLAB coder array into an Eigen matrix.
 * @param inputArray Coder array (rows x cols).
 * @return Equivalent Eigen matrix.
 */
Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> coderToEigen(::coder::array<double, 2U> const &inputArray);


/**
 * @brief Convert an Eigen matrix into a 2D MATLAB coder array.
 * @param inputEigenArray Eigen matrix.
 * @return Equivalent coder array.
 */
::coder::array<double, 2U> EigenToCoder(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> const &inputEigenArray) ;

/**
 * @brief Convert an Eigen vector into a 1D MATLAB coder array.
 * @param vec Eigen vector.
 * @return Equivalent 1D coder array.
 */
::coder::array<double, 1U> EigenVectorToCoder1U(Eigen::VectorXd const &vec) ;

/**
 * @brief Convert a 1D MATLAB coder array into an Eigen vector.
 * @param vec 1D coder array.
 * @return Equivalent Eigen vector.
 */
Eigen::VectorXd coder1UtoEigenVector(::coder::array<double, 1U> const &vec) ;

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_PRIVATEUTILS_H
