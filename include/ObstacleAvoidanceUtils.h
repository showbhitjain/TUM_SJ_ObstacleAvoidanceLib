//
// Created by shobhit on 26.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H

#include <Eigen/Dense>


namespace ObstacleAvoidance {
    class ObstacleAvoidanceUtils {
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
        int obstacleType;
        int obstacleId;

    };

}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
