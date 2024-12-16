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
        Eigen::VectorXd Tool_V0;
        Eigen::VectorXd Tool_V1;
        double radius;


    };

}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_LINKSEGMENT_H
