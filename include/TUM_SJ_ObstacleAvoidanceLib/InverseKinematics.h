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

    class InverseKinematics {
    public:
        /**
         * @brief Construct the constrained inverse-kinematics solver and load its objective configuration.
         * @param ikConfig Configuration parameters with the "ObjectiveConstraints" section.
         */
        InverseKinematics(AndreiUtils::ConfigurationParameters const & ikConfig);

        /**
         * @brief Solve the inverse-kinematics problem subject to joint position/velocity constraints.
         * @param jointValues Current joint values.
         * @param jacobiMatrix Task Jacobian at the current configuration.
         * @param poseVelocityEffective Effective desired Cartesian pose velocity (6x1).
         * @param jointMinValues Lower joint-position limits.
         * @param jointMaxValues Upper joint-position limits.
         * @param jointMinVelValues Lower joint-velocity limits.
         * @param jointMaxVelValues Upper joint-velocity limits.
         * @return Tuple of (optimal joint velocity, solver exit flag).
         */
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
