//
// Created by shobhit on 26.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H

#include <Eigen/Dense>
#include "Obstacle.h"

namespace ObstacleAvoidance {

    double smoothingConstraintScheme(double const &d, double const &dStop, double const &dActivate, double const &k);

    std::map<std::string,Obstacle> conversionObstaclesVectorToMap(std::vector<Obstacle> const &obstaclesArray);

    std::tuple<Eigen::Vector3d, Eigen::Vector3d> decomposeVector(
            Eigen::Vector3d const &v0,
            Eigen::Vector3d const &rc);

}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
