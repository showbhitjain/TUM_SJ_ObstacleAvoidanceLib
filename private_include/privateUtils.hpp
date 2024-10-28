//
// Created by shobhit on 26.10.24.
//
#ifndef PRIVATE_UTILS
#define PRIVATE_UTILS

#include <coder_array.h>
#include <Eigen/Dense>
#include <vector>
#include <string>



Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> coderToEigen(::coder::array<double, 2U> const &inputArray) {
    // Set the size of the Eigen matrix to match the coder array's dimensions.
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> outputEigenArray(inputArray.size(0), inputArray.size(1));

    // Copy elements from the coder array to the Eigen matrix.
    for (int idx0{0}; idx0 < inputArray.size(0); idx0++) {
        for (int idx1{0}; idx1 < inputArray.size(1); idx1++) {
            outputEigenArray(idx0, idx1) = inputArray.at(idx0,idx1);
        }
    }
    return outputEigenArray;
}


::coder::array<double, 2U> EigenToCoder(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> const &inputEigenArray) {
    coder::array<double, 2U> outputArray;
    // Set the size of the coder array to match the Eigen matrix's dimensions
    outputArray.set_size(inputEigenArray.rows(), inputEigenArray.cols());

    // Copy elements from the Eigen matrix to the coder array.
    for (int idx0{0}; idx0 < inputEigenArray.rows(); idx0++) {
        for (int idx1{0}; idx1 < inputEigenArray.cols(); idx1++) {
            outputArray.at(idx0, idx1) = inputEigenArray(idx0, idx1);
        }
    }
    return outputArray;
}


#endif