//
// Created by shobhit on 18.01.25.
//

#include <OptimizationBasedIKWithOA.h>
#include <inverseKinematicsMatlabObstacleAvoidance.h>
#include <privateUtils.h>
#include <AndreiUtils/utilsGeometry.h>

namespace ObstacleAvoidance {
    OptimizationBasedIKWithOA::OptimizationBasedIKWithOA(AndreiUtils::ConfigurationParameters const &configOA){
        auto objectiveFunctionJson = configOA.getJson("ObjectiveConstraints");
        config.useObjectiveNormInfinity = objectiveFunctionJson["useObjectiveNormInfinity"].get<bool>();
        config.weightNormInfinity = objectiveFunctionJson.at("weightNormInfinity").get<double>();

        config.useObjectiveNormL2 = objectiveFunctionJson["useObjectiveNormL2"].get<bool>();
        config.weightNormL2 = objectiveFunctionJson["weightNormL2"].get<double>();

        config.useObjectiveJointAcceleration = objectiveFunctionJson["useObjectiveJointAcceleration"].get<bool>();
        config.weightJointAcceleration = objectiveFunctionJson["weightJointAcceleration"].get<double>();

        config.useObjectiveTrajectoryFollowing = objectiveFunctionJson["useObjectiveTrajectoryFollowing"].get<bool>();
        config.weightTrajectoryFollowing = objectiveFunctionJson["weightTrajectoryFollowing"].get<double>();

        config.useObjectiveManipulability = objectiveFunctionJson["useObjectiveManipulability"].get<bool>();
        config.weightManipulability = objectiveFunctionJson["weightManipulability"].get<double>();

        auto equationParametersJson = configOA.getJson("EquationParameters");
        config.applyEqualityConstraints = equationParametersJson["applyEqualityConstraints"].get<bool>();
        config.applyInequalityConstraints = equationParametersJson["applyInequalityConstraints"].get<bool>();

        auto slackParametersJson = configOA.getJson("SlackParameters");
        config.Slack_objective_weight = slackParametersJson["SlackObjectiveWeight"].get<double>();
        config.applySlack = slackParametersJson["applySlack"].get<bool>();
        auto slackLowerArray = slackParametersJson["Slacklowerbound"].get<std::array<double,6>>();

        for(int i = 3; i < 6; ++i){
            slackLowerArray[i] = AndreiUtils::deg2Rad(slackLowerArray[i]);
        }

        for(int i = 0; i<6 ; ++i){
            config.Slacklowerbound[i] = slackLowerArray[i];
        }

//        std::copy(slackLowerArray.begin(),slackLowerArray.end(),config.Slacklowerbound);

        auto slackUpperArray = slackParametersJson["Slackupperbound"].get<std::array<double,6>>();
        for(int i = 3; i < 6; ++i){
            slackUpperArray[i] = AndreiUtils::deg2Rad(slackUpperArray[i]);
        }
        for(int i = 0; i<6 ; ++i){
            config.Slackupperbound[i] = slackUpperArray[i];
        }
//        std::copy(slackUpperArray.begin(),slackUpperArray.end(),config.Slackupperbound);

        auto slackWeightArray = slackParametersJson["SlackPenaltyWeight"].get<std::array<double,6>>();

        for(int i{0}; i<36; ++i ){
            config.Slack_penalty_weightmatrix[i]  = 0;
        }

        for(int i{0}; i<6; ++i){
            config.Slack_penalty_weightmatrix[7*i] = slackWeightArray[i];
        }

        auto jointLimitAvoidanceJson = configOA.getJson("jointLimitAvoidanceParameters");
        config.gamma = jointLimitAvoidanceJson["gamma"].get<double>();
        config.applyVelocityDamper = jointLimitAvoidanceJson["applyVelocityDamper"].get<bool>();
        config.jointLimitActivationDistance = jointLimitAvoidanceJson["jointLimitActivationDistance"].get<double>();
        config.jointLimitStopDistance = jointLimitAvoidanceJson["jointLimitStopDistance"].get<double>();
        config.jointLimitGain = jointLimitAvoidanceJson["jointLimitGain"].get<double>();

    }

    std::tuple<Eigen::VectorXd, double>
    OptimizationBasedIKWithOA::inverseKinematicsWithOA( Eigen::VectorXd const &jointValues,
                                                        Eigen::MatrixXd const &jacobiMatrix,
                                                        Eigen::Matrix<double,6,1> const &poseVelocityEffective,
                                                        Eigen::VectorXd const &jointMinValues,
                                                        Eigen::VectorXd const &jointMaxValues,
                                                        Eigen::VectorXd const &jointMinVelValues,
                                                        Eigen::VectorXd const &jointMaxVelValues,
                                                        Eigen::MatrixXd const &jG, Eigen::VectorXd const &bG,
                                                        Eigen::MatrixXd const &jointVelocityMatrix) {

        double cartesianVelocityEffective[6];
        std::copy(poseVelocityEffective.data(), poseVelocityEffective.data() + poseVelocityEffective.size(),
                  cartesianVelocityEffective);

        coder::array<double,1U> optimalJointVelocity;
        double exitFlag;
        auto configInput = this->config;
        inverseKinematicsMatlabObstacleAvoidance ikWithOA;
        ikWithOA.inverseKinematicsOA(EigenVectorToCoder1U(jointValues), EigenToCoder(jacobiMatrix), cartesianVelocityEffective, EigenVectorToCoder1U(jointMinValues), EigenVectorToCoder1U(jointMaxValues),
                                     EigenVectorToCoder1U(jointMinVelValues), EigenVectorToCoder1U(jointMaxVelValues), EigenToCoder(jG), EigenVectorToCoder1U(bG),
                                     EigenToCoder(jointVelocityMatrix),
                                     &configInput, optimalJointVelocity, &exitFlag);

        return std::make_tuple(coder1UtoEigenVector(optimalJointVelocity),exitFlag);

    }

}