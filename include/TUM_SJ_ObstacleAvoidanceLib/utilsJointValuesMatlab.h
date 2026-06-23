//
// Created by shobhit on 14.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_UTILSJOINTVALUESMATLAB_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_UTILSJOINTVALUESMATLAB_H
#include <Eigen/Dense>


namespace ObstacleAvoidance {

   /**
    * @brief Integrate joint velocities over a time span using the generated constant Runge-Kutta integrator.
    * @param jointVelocity Joint velocity to integrate.
    * @param timespan Two-element [start, end] integration time span.
    * @param startJointVector Initial joint values at the start of the span.
    * @return Joint values at the end of the time span.
    */
   Eigen::VectorXd integrateConstantRungeKutta(Eigen::VectorXd const & jointVelocity, double const timespan[2], Eigen::VectorXd const & startJointVector );

    /**
     * @brief Add uniform random noise to a joint signal using the generated MATLAB function.
     * @param jointsSignal Input joint signal.
     * @param minNoise Lower bound of the noise.
     * @param maxNoise Upper bound of the noise.
     * @return Joint signal with added noise.
     */
    Eigen::VectorXd addRandomNoisetoJointsSignal(Eigen::VectorXd  const & jointsSignal,double const &minNoise, double const &maxNoise );

} // ObstacleAvoidance

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_UTILSJOINTVALUESMATLAB_H
