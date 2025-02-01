//
// Created by shobhit on 14.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_UTILSJOINTVALUESMATLAB_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_UTILSJOINTVALUESMATLAB_H
#include <Eigen/Dense>


namespace ObstacleAvoidance {

   Eigen::VectorXd integrateConstantRungeKutta(Eigen::VectorXd const & jointVelocity, double const timespan[2], Eigen::VectorXd const & startJointVector );

    Eigen::VectorXd addRandomNoisetoJointsSignal(Eigen::VectorXd  const & jointsSignal,double const &minNoise, double const &maxNoise );

} // ObstacleAvoidance

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_UTILSJOINTVALUESMATLAB_H
