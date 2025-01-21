//
// Created by shobhit on 18.01.25.
//

#ifndef OPTIMIZATIONBASEDIKWITHOA_H
#define OPTIMIZATIONBASEDIKWITHOA_H

#include <Eigen/Dense>
#include <inverseKinematicsOA_types.h>
#include <vector>
#include <Eigen/Dense>
#include <tuple>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

namespace ObstacleAvoidance {

    class OptimizationBasedIKWithOA {
    public:
        OptimizationBasedIKWithOA(AndreiUtils::ConfigurationParameters const &configOA);

        std::tuple<Eigen::VectorXd, double>
        inverseKinematicsWithOA(Eigen::VectorXd const &jointValues, Eigen::MatrixXd const &jacobiMatrix,
                                Eigen::Matrix<double, 6, 1> const &poseVelocityEffective,
                                Eigen::VectorXd const &jointMinValues, Eigen::VectorXd const &jointMaxValues,
                                Eigen::VectorXd const &jointMinVelValues,
                                Eigen::VectorXd const &jointMaxVelValues, Eigen::MatrixXd const &jG, Eigen::VectorXd const &bG, Eigen::MatrixXd const &jointVelocityMatrix);

    protected:
        structIkWithOA config;


    };

} // ObstacleAvoidance

#endif //OPTIMIZATIONBASEDIKWITHOA_H
