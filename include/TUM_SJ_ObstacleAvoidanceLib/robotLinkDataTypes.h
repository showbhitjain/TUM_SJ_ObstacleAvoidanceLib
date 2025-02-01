//
// Created by shobhit on 29.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEDATATYPES_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEDATATYPES_H

#include <Eigen/Dense>


namespace ObstacleAvoidance {
    struct LinkSegment {
        Eigen::VectorXd aSegmentV0;
        Eigen::VectorXd aSegmentV1;
        Eigen::VectorXd dSegmentV0;
        Eigen::VectorXd dSegmentV1;
        Eigen::VectorXd toolV0;
        Eigen::VectorXd toolV1;
        double radiusJoint;

        Eigen::Vector3d finalLinkDimensionsTCP;
        Eigen::Quaterniond finalLinkOrientationTCP;
        Eigen::Vector3d CenterFinalLink;
        double radiusLink;
    };
    struct finalLinkRobot{
        Eigen::Vector3d finalV0;
        Eigen::Vector3d finalV1;
        std::string type;
        Eigen::Vector3d dimensions;
        Eigen::Quaterniond orientation;
        Eigen::Vector3d center;
    };

}


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEDATATYPES_H
