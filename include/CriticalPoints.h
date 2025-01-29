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
    Eigen::Vector3d distVectorFinalLink;

    double distanceA;
    double distanceD;
    double distanceFinalLink;
    double distance;

    bool hasCriticalPointA;
    bool hasCriticalPointD;
    bool hasCriticalPointFinalLink;

    Eigen::MatrixXd jacobiDistMaxA;
    Eigen::MatrixXd jacobiDistMaxD;
    Eigen::MatrixXd jacobiDistMaxFinalLink;

    Eigen::VectorXd jointAnglesCriticalA;
    Eigen::VectorXd jointAnglesCriticalD;
    Eigen::VectorXd jointAnglesCriticalFinalLink;

    Eigen::VectorXd jointVelocityCriticalA;
    Eigen::VectorXd jointVelocityCriticalD;
    Eigen::VectorXd jointVelocityCriticalFinalLink;
    Eigen::MatrixXd jacobiDistanceA;
    Eigen::MatrixXd jacobiDistanceD;
    Eigen::MatrixXd jacobiDistanceFinalLink;

    Eigen::MatrixXd jacobiCriticalA;
    Eigen::MatrixXd jacobiCriticalMaxA;
    Eigen::MatrixXd jacobiCriticalD;
    Eigen::MatrixXd jacobiCriticalMaxD;
    Eigen::MatrixXd jacobiCriticalFinalLink;
    Eigen::MatrixXd jacobiCriticalMaxFinalLink;

    Eigen::Vector3d  closestPointObstacleA;
    Eigen::Matrix4d  closestPointRelativeRobotLinkA;
    Eigen::Vector3d  closestPointObstacleD;
    Eigen::Matrix4d closestPointRelativeRobotLinkD;
    Eigen::Vector3d closestPointObstacleFinalLink;
    Eigen::Matrix4d  closestPointRelativeFinalLink;






    CriticalPoints(int const &numberOfJoints);

};


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
