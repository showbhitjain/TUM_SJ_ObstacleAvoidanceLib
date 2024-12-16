//
// Created by shobhit on 11.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
#include <Eigen/Dense>

class CriticalPoints {
public:
    Eigen::Vector3d distVectorA;
    Eigen::Vector3d distVectorD;
    bool hasCriticalpointsA;
    bool hasCriticalpointsD;
    Eigen::MatrixXd jacobiDistMaxA;
    Eigen::MatrixXd jacobiDistMaxD;
    Eigen::VectorXd jointAnglesCriticalA;
    Eigen::VectorXd jointAnglesCriticalD;
    Eigen::VectorXd jointVelocityCriticalA;
    Eigen::VectorXd jointVelocityCriticalD;
    Eigen::MatrixXd jacobiDistanceA;
    Eigen::MatrixXd jacobiDistanceD;
    Eigen::MatrixXd jacobiCriticalA;
    Eigen::MatrixXd jacobiCriticalMaxA;
    Eigen::MatrixXd jacobiCriticalD;
    Eigen::MatrixXd jacobiCriticalMaxD;
    double distance;

    CriticalPoints(int const &numberLinks);

};


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
