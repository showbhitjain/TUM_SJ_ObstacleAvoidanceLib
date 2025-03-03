//
// Created by shobhit on 18.01.25.
//

#ifndef OPTIMIZATIONBASEDIKWITHOA_H
#define OPTIMIZATIONBASEDIKWITHOA_H

#include <Eigen/Dense>

#include <vector>
#include <Eigen/Dense>
#include <tuple>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

namespace ObstacleAvoidance {
    struct structIkWithOA {
        bool useObjectiveNormInfinity;
        double weightNormInfinity;
        bool useObjectiveNormL2;
        double weightNormL2;
        bool useObjectiveTrajectoryFollowing;
        double weightTrajectoryFollowing;
        bool useObjectiveJointAcceleration;
        double weightJointAcceleration;
        bool useObjectiveManipulability;
        double weightManipulability;
        bool applyEqualityConstraints;
        bool applyInequalityConstraints;
        bool applySlack;
        double SlackLowerBound[6];
        double SlackUpperBound[6];
        double SlackPenaltyWeight[6];
        double SlackObjectiveWeight;
        bool obstacleAvoidanceScheme;
        double k;
        double gamma;
        bool dynamicSlackForOA;
        bool applyVelocityDamper;
        double jointLimitActivationDistance;
        double jointLimitStopDistance;
        double jointLimitGain;
    };
    class OptimizationBasedIKWithOA {
    public:
        OptimizationBasedIKWithOA(AndreiUtils::ConfigurationParameters const &configOA);

        std::tuple<Eigen::VectorXd, double>
        inverseKinematicsWithOA(Eigen::VectorXd const &jointValues, Eigen::MatrixXd const &jacobiMatrix,
                                Eigen::Matrix<double, 6, 1> const &poseVelocityEffective,
                                Eigen::VectorXd const &jointMinValues, Eigen::VectorXd const &jointMaxValues,
                                Eigen::VectorXd const &jointMinVelValues,
                                Eigen::VectorXd const &jointMaxVelValues, Eigen::MatrixXd const &jG, Eigen::VectorXd const &bG, Eigen::MatrixXd const &jointVelocityMatrix);

        void setConfigSlackWeight(double const &slackObjectiveWeight);

        void setDynamicSlackFlagValueForOA(bool const &value);

        void setValueInequalityConstraints(bool const &value);

    protected:


        structIkWithOA config;


    };

} // ObstacleAvoidance

#endif //OPTIMIZATIONBASEDIKWITHOA_H
