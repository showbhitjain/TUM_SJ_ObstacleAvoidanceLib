//
// Created by shobhit on 12.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
//#include <dqrobotics/DQ.h>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <fstream>


namespace ObstacleAvoidance {
//    Eigen::Vector3d tFromDQ(DQ_robotics::DQ const &q);
//
//    DQ_robotics::DQ fromPoseToDQ(AndreiUtils::Pose const &pose);
//
//    AndreiUtils::Pose fromDQToPose(DQ_robotics::DQ const &pose);

    Eigen::Matrix4d trvec2tform(Eigen::Vector3d const &translation);

    Eigen::Matrix4d eul2tform(Eigen::Vector3d const &eulerAngles);


    Eigen::Matrix4d convertEulerToTransform(Eigen::Vector3d const &angles, std::string const &seq = "ZYX");

    Eigen::Matrix3d skewSymmetric(Eigen::Vector3d const &v);

    Eigen::MatrixXd computeAdjoint(Eigen::Matrix4d const &T);


    Eigen::MatrixXd vectorMatrixToEigenMatrix(std::vector<std::vector<double>> const &vec);

    Eigen::Matrix<double, 1, Eigen::Dynamic> vectorToEigenMatrixRow(std::vector<double> const &vec);

    Eigen::VectorXd stdVectorToEigenVector(std::vector<double> const &vec);

    std::vector<double> EigenVectorToStdVector(Eigen::VectorXd const &eigen_vector);

    Eigen::VectorXd generateSequence(double startValue, double ts, double lastValue);

    Eigen::Vector3d computeOrientationError(const Eigen::Matrix4d &T_current, const Eigen::Quaterniond &desired_q);

    Eigen::Vector3d computeOrientationError(const Eigen::Matrix4d &T_current, const Eigen::VectorXd &q);


    void writeMatrixToCSV(std::string const &filename, Eigen::MatrixXd const &matrix);

    Eigen::MatrixXd readMatrixFromCSV(const std::string& filename);

    Eigen::Vector4d computeQuaternionFromMatrix(Eigen::Matrix4d const& transformationMatrix);

}


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H
