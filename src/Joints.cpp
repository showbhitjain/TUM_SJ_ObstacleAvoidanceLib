//
// Created by shobhit on 10.10.24.
//

#include<Joints.h>
#include <iostream>
#include <utils.h>
using namespace ObstacleAvoidance;
using namespace  std;

Joints::Joints(const ConfigurationParameters &config) {

    if (config.has("jointMinValues"))
    {
        this->minValues = stdVectorToEigenVector(config.get<vector<double>>("jointMinValues"));
    }
    if (config.has("jointMaxValues"))
    {
        this->maxValues = stdVectorToEigenVector(config.get<vector<double>>("jointMaxValues"));
    }

    if (config.has("jointVelMinValues"))
    {
        this->velMinValues = stdVectorToEigenVector(config.get<vector<double>>("jointVelMinValues"));
    }
    if (config.has("jointVelMaxValues"))
    {
        this->velMaxValues = stdVectorToEigenVector(config.get<vector<double>>("jointVelMaxValues"));
    }
    if (config.has("numberOfJoints")){
        this->number_joints = config.get<int>("numberOfJoints");
    }

}

Joints::Joints(Eigen::VectorXd const &minValues, Eigen::VectorXd const &maxValues, Eigen::VectorXd const &velMinValues, Eigen::VectorXd const &velMaxValues, int const &numJoints) : minValues(minValues),
                                                                                                       maxValues(maxValues), number_joints(numJoints),velMaxValues(velMaxValues),velMinValues(velMinValues) {}

/**
* @brief Get the current joint values of the robot.
*
* This function returns the current joint values of the robot.
*
* @return A vector containing the current joint values.
*/

Eigen::VectorXd const &Joints::getCurrentJointValues() {
    return this->getJointValues();
}

/**
 * @brief Get the joint values for the current state of the Joints object.
 *
 * This method returns the joint values for the current state of the Joints object. The joint values
 * represent the angles or positions of the joints in the current state.
 *
 * @return std::vector<double> The joint values.
 */

Eigen::VectorXd const &Joints::getJointValues() const {
    return this->values;
}

/**
 * @brief Sets the joint values of the robot.
 *
 * This function sets the joint values for the robot to the given values.
 * The joint values are passed as an Eigen VectorXd object, which contains
 * the values for each joint in the same order as the robot's joint hierarchy.
 *
 * @param jointValues The new joint values for the robot.
 */

void Joints::setJointValues(const Eigen::VectorXd &jointValues) {
    assert(jointValues.size() == number_joints);
    assert(this->isConfigurationValid(jointValues, true));
    this->values = jointValues;
}

/**
* @brief Checks whether the given joint configuration is valid.
*
* This function checks whether the given joint configuration is valid based on the constraints
* defined for the robot's joints.
*
* @param jointValues The joint configuration to be checked.
* @param verbose Optionally, set to true to print verbose information about the validation process.
* @return True if the joint configuration is valid, false otherwise.
*/

bool Joints::isConfigurationValid(Eigen::VectorXd jointValues, bool verbose) const {
    assert(jointValues.size() == number_joints);
    bool valid = true;
    for(int i = 0; i < number_joints; i++){
        if(jointValues[i] < minValues(i) && jointValues[i] > maxValues(i)){
            valid = false;
            if(verbose) std::cout << "Limit on joint" << i << " violated: " << jointValues[i] << endl;
        }
    }
    return valid;

}

