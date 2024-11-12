//
// Created by shobhit on 11.11.24.
//

#include "inverseKinematics.h"
#include <inverseKinematicsWithConstraints.h>
#include <privateUtils.hpp>

using namespace ObstacleAvoidance;
using namespace Eigen;
using namespace AndreiUtils;
using namespace std;

inverseKinematics::inverseKinematics(const AndreiUtils::ConfigurationParameters &ikConfig) {
    auto objectiveConstraints = ikConfig.getJson("ObjectiveConstraints");
    ikStructConfig.useObjective1 = objectiveConstraints["useObjective1"].get<bool>();
    ikStructConfig.useObjective2 = objectiveConstraints["useObjective2"].get<bool>();
    ikStructConfig.useObjective3 = objectiveConstraints["useObjective3"].get<bool>();
    ikStructConfig.useObjective4 = objectiveConstraints["useObjective4"].get<bool>();
    ikStructConfig.useObjective5 = objectiveConstraints["useObjective5"].get<bool>();
    ikStructConfig.useObjective6 = objectiveConstraints["useObjective6"].get<bool>();

    ikStructConfig.weight1 = objectiveConstraints["weight1"].get<double>();
    ikStructConfig.weight2 = objectiveConstraints["weight2"].get<double>();
    ikStructConfig.weight3 = objectiveConstraints["weight3"].get<double>();
    ikStructConfig.weight4 = objectiveConstraints["weight4"].get<double>();
    ikStructConfig.weight5 = objectiveConstraints["weight5"].get<double>();
    ikStructConfig.weight6 = objectiveConstraints["weight6"].get<double>();

}

std::tuple<Eigen::VectorXd, int>
inverseKinematics::ikWithConstraints(const VectorXd &jointValues, const MatrixXd &jacobiMatrix,
                                     const VectorXd &poseVelocityEffective, const VectorXd &jointMinValues,
                                     const VectorXd &jointMaxValues, const VectorXd &jointMinVelValues,
                                     const VectorXd &jointMaxVelValues) {

    double xdEffVelocity[6];
    int xdEffVelocitySize[1] = {6};
    for (int i = 0; i < 6, i++;) {
        xdEffVelocity[i] = poseVelocityEffective(i);
    }
    coder::array<double, 1U> optimalJointVelocity;
    double exitFlag;
    inverseKinematicsWithConstraints ik;
    ik.inverseKinematics(EigenVectorToCoder1U(jointValues), EigenToCoder(jacobiMatrix), xdEffVelocity,
                         xdEffVelocitySize,
                         EigenVectorToCoder1U(jointMinValues), EigenVectorToCoder1U(jointMaxValues),
                         EigenVectorToCoder1U(jointMinVelValues), EigenVectorToCoder1U(jointMaxVelValues),
                         &(this->ikStructConfig), optimalJointVelocity, &exitFlag);

    return make_tuple(coder1UtoEigenVector(optimalJointVelocity), exitFlag);
}



// ObstacleAvoidance