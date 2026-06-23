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

namespace ObstacleAvoidance {
    class Joints {
    public:
        /**
         * @brief Construct joints from configuration parameters.
         * @param config Configuration with optional "jointMinValues", "jointMaxValues",
         *               "jointVelMinValues", "jointVelMaxValues", and "numberOfJoints".
         */
        Joints(const AndreiUtils::ConfigurationParameters &config);

        /**
         * @brief Construct joints from explicit limit vectors.
         * @param minValues Lower joint-position limits.
         * @param maxValues Upper joint-position limits.
         * @param velMinValues Lower joint-velocity limits.
         * @param velMaxValues Upper joint-velocity limits.
         * @param numJoints Number of joints.
         */
        Joints(Eigen::VectorXd minValues, Eigen::VectorXd maxValues, Eigen::VectorXd velMinValues, Eigen::VectorXd velMaxValues, int const &numJoints);

        /**
         * @brief Get the current joint values, refreshing them at call time.
         * @return Reference to the current joint values.
         */
        virtual Eigen::VectorXd const &getCurrentJointValues();

        /**
         * @brief Get the last-known joint values without updating them.
         * @return Reference to the stored joint values.
         */
        [[nodiscard]] virtual Eigen::VectorXd const &getJointValues() const;

        /**
         * @brief Set the joint values.
         * @param jointValues New joint values (must be valid and match the joint count).
         */
        virtual void setJointValues(Eigen::VectorXd const &jointValues);

        /**
         * @brief Check whether a joint configuration respects the position limits.
         * @param jointValues Joint configuration to validate.
         * @param verbose If true, print which joint limits are violated.
         * @return True if every joint is within its limits, false otherwise.
         */
        [[nodiscard]] bool isConfigurationValid(Eigen::VectorXd jointValues, bool verbose) const;

        Eigen::VectorXd values;                     /**< Current joint values. */

        Eigen::VectorXd minValues, maxValues;       /**< Joint-position limits. */
        Eigen::VectorXd velMinValues, velMaxValues; /**< Joint-velocity limits. */

        int number_joints ;                         /**< Number of joints. */
    protected:

    };
}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_JOINTS_H
