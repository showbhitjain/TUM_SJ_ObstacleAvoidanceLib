//
// Created by shobhit on 11.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_INVERSEKINEMATICS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_INVERSEKINEMATICS_H


#include <inverseKinematics_types.h>
#include <vector>
#include <Eigen/Dense>
#include <memory>
#include <tuple>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

namespace ObstacleAvoidance {

    class inverseKinematics {
    public:
        inverseKinematics(AndreiUtils::ConfigurationParameters const & ikConfig);
        std::tuple<Eigen::VectorXd, double> ikWithConstraints(Eigen::VectorXd const &jointValues,
                                                           Eigen::MatrixXd const &jacobiMatrix,
                                                           Eigen::VectorXd const &poseVelocityEffective,
                                                           Eigen::VectorXd const &jointMinValues,
                                                           Eigen::VectorXd const &jointMaxValues,
                                                           Eigen::VectorXd const &jointMinVelValues,
                                                           Eigen::VectorXd const &jointMaxVelValues);
    protected:
        struct0_T ikStructConfig;

    };

} // ObstacleAvoidance

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_INVERSEKINEMATICS_H
