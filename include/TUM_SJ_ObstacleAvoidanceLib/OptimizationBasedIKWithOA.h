//
// Created by shobhit on 18.01.25.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OPTIMIZATIONBASEDIKWITHOA_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OPTIMIZATIONBASEDIKWITHOA_H

#include <vector>
#include <Eigen/Dense>
#include <tuple>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

namespace ObstacleAvoidance {
    /**
     * @brief Configuration of the optimization-based inverse kinematics solver with obstacle avoidance.
     *
     * Mirrors the JSON settings (objective terms, equality/inequality constraints, slack, and
     * joint-limit avoidance) consumed by the underlying generated solver.
     */
    struct structIkWithOA {
        bool useObjectiveNormInfinity;          /**< Enable the infinity-norm joint-velocity objective. */
        double weightNormInfinity;              /**< Weight of the infinity-norm objective. */
        bool useObjectiveNormL2;                /**< Enable the weighted L2 joint-velocity objective. */
        double weightNormL2;                    /**< Weight of the L2-norm objective. */
        bool useObjectiveTrajectoryFollowing;   /**< Enable the trajectory-following objective. */
        double weightTrajectoryFollowing;       /**< Weight of the trajectory-following objective. */
        bool useObjectiveJointAcceleration;     /**< Enable the joint-acceleration (smoothing) objective. */
        double weightJointAcceleration;         /**< Weight of the joint-acceleration objective. */
        bool useObjectiveManipulability;        /**< Enable the manipulability objective. */
        double weightManipulability;            /**< Weight of the manipulability objective. */
        bool applyEqualityConstraints;          /**< Apply task-space (trajectory tracking) equality constraints. */
        bool applyInequalityConstraints;        /**< Apply obstacle-avoidance inequality constraints. */
        bool applySlack;                        /**< Enable slack variables. */
        double SlackLowerBound[6];              /**< Lower bounds of the slack search space. */
        double SlackUpperBound[6];              /**< Upper bounds of the slack search space. */
        double SlackPenaltyWeight[6];           /**< Per-direction slack penalty weights. */
        double SlackObjectiveWeight;            /**< Cost multiplier for slack variables in the objective. */
        bool obstacleAvoidanceScheme;           /**< Whether slack terms are used specifically for obstacle avoidance. */
        double k;                               /**< Smoothing coefficient k of the obstacle-avoidance scheme. */
        double gamma;                           /**< Gain used by the joint-limit avoidance scheme. */
        bool dynamicSlackForOA;                 /**< Enable dynamic slack for obstacle avoidance. */
        bool applyVelocityDamper;               /**< Enable the velocity damper for joint-limit avoidance. */
        double jointLimitActivationDistance;    /**< Activation distance for joint-limit avoidance. */
        double jointLimitStopDistance;          /**< Stop distance for joint-limit avoidance. */
        double jointLimitGain;                  /**< Gain for joint-limit avoidance. */
    };
    class OptimizationBasedIKWithOA {
    public:
        /**
         * @brief Construct the solver and load its configuration.
         * @param configOA Configuration parameters with the "ObjectiveConstraints", "EquationParameters",
         *                 "SlackParameters", and "jointLimitAvoidanceParameters" sections.
         */
        OptimizationBasedIKWithOA(AndreiUtils::ConfigurationParameters const &configOA);

        /**
         * @brief Solve the optimization-based inverse kinematics problem with obstacle avoidance.
         * @param jointValues Current joint values.
         * @param jacobiMatrix Task Jacobian at the current configuration.
         * @param poseVelocityEffective Effective desired Cartesian pose velocity (6x1: linear then angular).
         * @param jointMinValues Lower joint-position limits.
         * @param jointMaxValues Upper joint-position limits.
         * @param jointMinVelValues Lower joint-velocity limits.
         * @param jointMaxVelValues Upper joint-velocity limits.
         * @param jG Obstacle-avoidance constraint Jacobian.
         * @param bG Obstacle-avoidance constraint bound vector.
         * @param jointVelocityMatrix Weight matrix for the joint-velocity objective.
         * @param constraintTolerance Solver constraint tolerance.
         * @param maxIterations Maximum solver iterations.
         * @param optimalityTolerance Solver optimality tolerance.
         * @param stepTolerance Solver step tolerance.
         * @return Tuple of (optimal joint velocity, solver exit flag; negative indicates failure).
         */
        std::tuple<Eigen::VectorXd, double>
        inverseKinematicsWithOA(Eigen::VectorXd const &jointValues, Eigen::MatrixXd const &jacobiMatrix,
                                Eigen::Matrix<double, 6, 1> const &poseVelocityEffective,
                                Eigen::VectorXd const &jointMinValues, Eigen::VectorXd const &jointMaxValues,
                                Eigen::VectorXd const &jointMinVelValues,
                                Eigen::VectorXd const &jointMaxVelValues, Eigen::MatrixXd const &jG,
                                Eigen::VectorXd const &bG, Eigen::MatrixXd const &jointVelocityMatrix,
                                double constraintTolerance,int maxIterations,double optimalityTolerance,double stepTolerance);

        /**
         * @brief Set the slack objective weight in the solver configuration.
         * @param slackObjectiveWeight New slack objective weight.
         */
        void setConfigSlackWeight(double const &slackObjectiveWeight);

        /**
         * @brief Enable or disable the dynamic-slack-for-obstacle-avoidance flag.
         * @param value New flag value.
         */
        void setDynamicSlackFlagValueForOA(bool const &value);

        /**
         * @brief Enable or disable the obstacle-avoidance inequality constraints.
         * @param value New flag value.
         */
        void setValueInequalityConstraints(bool const &value);

    protected:


        structIkWithOA config;


    };

} // ObstacleAvoidance

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OPTIMIZATIONBASEDIKWITHOA_H
