//
// Created by shobhit on 10.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_JOINTS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_JOINTS_H

#include <Eigen/Dense>
#include <string>
#include <vector>
#include <memory>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

using namespace AndreiUtils;

namespace ObstacleAvoidance {
    class Joints {
    public:
//        Joints(std::string const &configFile, std::string const &parametersFor);

        Joints(const ConfigurationParameters &config);





        Joints(Eigen::VectorXd minValues, Eigen::VectorXd maxValues, Eigen::VectorXd velMinValues, Eigen::VectorXd velMaxValues, int const &numJoints);

// this function internally updates the joint values so that they are up-to-date at the calling time
        virtual Eigen::VectorXd const &getCurrentJointValues();

        // this function returns the last-known joint values without updating the values
        [[nodiscard]] virtual Eigen::VectorXd const &getJointValues() const;

        virtual void setJointValues(Eigen::VectorXd const &jointValues);

        [[nodiscard]] bool isConfigurationValid(Eigen::VectorXd jointValues, bool verbose) const;

        Eigen::VectorXd values;

        Eigen::VectorXd minValues, maxValues;
        Eigen::VectorXd velMinValues, velMaxValues;

        int number_joints ;
    protected:

    };
}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_JOINTS_H
