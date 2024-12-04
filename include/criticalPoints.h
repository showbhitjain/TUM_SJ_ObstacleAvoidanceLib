//
// Created by shobhit on 01.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H

#include <Eigen/Dense>

class criticalPoints {
public:
    Eigen::Vector3d roc_A;
    Eigen::Vector3d roc_D;
    bool has_criticalpoints_A;
    bool has_criticalpoints_D;
    Eigen::MatrixXd jacobi_dist_max_A;
    Eigen::MatrixXd jacobi_dist_max_D;
    Eigen::VectorXd jointAngles_critical_A;
    Eigen::VectorXd jointAngles_critical_D;
    Eigen::VectorXd jointVelocity_critical_A;
    Eigen::VectorXd jointVelocity_critical_D;
    Eigen::MatrixXd jacobi_distance_A;
    Eigen::MatrixXd jacobi_distance_D;
    Eigen::MatrixXd jacobi_critical_A;
    Eigen::MatrixXd jacobi_critical_A_max;
    Eigen::MatrixXd jacobi_critical_D;
    Eigen::MatrixXd jacobi_critical_D_max;
    double distance;
};


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
