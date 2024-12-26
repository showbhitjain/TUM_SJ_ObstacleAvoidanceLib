//
// Created by shobhit on 15.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_LINKSEGMENT_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_LINKSEGMENT_H

#include <Eigen/Dense>

namespace ObstacleAvoidance {
    class LinkSegment {
    public:
        Eigen::VectorXd aSegmentV0;
        Eigen::VectorXd aSegmentV1;
        Eigen::VectorXd dSegmentV0;
        Eigen::VectorXd dSegmentV1;
        Eigen::VectorXd toolV0;
        Eigen::VectorXd toolV1;
        double radiusAJoint;
        double radiusDJoint;
        Eigen::Vector3d finalLinkDimensionsTCP;
        Eigen::Quaterniond finalLinkOrientationTCP;
        Eigen::Vector3d CenterFinalLink;
        double radiusLink;


    };

}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_LINKSEGMENT_H
