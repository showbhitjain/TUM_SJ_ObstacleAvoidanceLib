//
// Created by shobhit on 26.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H

#include <Eigen/Dense>


namespace ObstacleAvoidance {

    double smoothingConstraintScheme(double d, double dStop, double dActivate, double& k);

}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
