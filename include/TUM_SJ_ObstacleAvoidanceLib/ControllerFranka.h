//
// Created by shobhit on 04.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_CONTROLLERFRANKA_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_CONTROLLERFRANKA_H

#include <array>
#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <franka/control_types.h>
#include <franka/duration.h>
#include <franka/robot.h>
#include <franka/robot_state.h>
#include <franka/model.h>
#include <franka/exception.h>
#include "examples_common.h"


namespace ObstacleAvoidance {
    typedef Eigen::Matrix<double, 7, 1> Vector7d;
    typedef Eigen::Matrix<double, 7, 7> Matrix7d;

    class ControllerFranka {
    protected:
        Matrix7d Kp;            /**< Proportional gain matrix. */
        Matrix7d Kd;            /**< Derivative gain matrix. */
        Vector7d maxTorques;    /**< Per-joint maximum torque (saturation upper bound). */
        Vector7d minTorques;    /**< Per-joint minimum torque (saturation lower bound). */

    public:
        /**
         * @brief Construct the Franka PD torque controller.
         * @param cutOffTorquesMax Per-joint maximum torque (saturation upper bound).
         * @param cutOffTorquesMin Per-joint minimum torque (saturation lower bound).
         * @param Kp Diagonal proportional gains (defaults to ones).
         * @param Kd Diagonal derivative gains (defaults to ones).
         */
        explicit ControllerFranka(Vector7d const &cutOffTorquesMax, Vector7d const &cutOffTorquesMin,
                                  Vector7d const &Kp = Vector7d::Ones(), Vector7d const &Kd = Vector7d::Ones());

        /**
         * @brief Compute saturated PD torques tracking a desired joint position (desired velocity taken from the state).
         * @param state Current robot state.
         * @param desiredJointPosition Desired joint positions.
         * @param withCoriolisVector If true, add the provided Coriolis compensation.
         * @param coriolis Coriolis vector (used when @p withCoriolisVector is true).
         * @return Saturated joint torques.
         */
        franka::Torques torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                 bool const &withCoriolisVector, std::array<double, 7> const &
                                         coriolis = {});

        /**
         * @brief Compute saturated PD torques tracking a desired joint position and velocity.
         * @param state Current robot state.
         * @param desiredJointPosition Desired joint positions.
         * @param desiredJointVelocity Desired joint velocities.
         * @param withCoriolisVector If true, add the provided Coriolis compensation.
         * @param coriolis Coriolis vector (used when @p withCoriolisVector is true).
         * @return Saturated joint torques.
         */
        franka::Torques torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                 Vector7d const &desiredJointVelocity, bool const &withCoriolisVector,
                                 std::array<double, 7> const &coriolis = {});

        /**
         * @brief Compute PD torques with inertia-shaping (mass matrix) and Coriolis compensation.
         * @param state Current robot state.
         * @param desiredJointPosition Desired joint positions.
         * @param coriolis Coriolis vector.
         * @param dynamicMass Row-major 7x7 mass matrix.
         * @return Joint torques (not additionally saturated).
         */
        franka::Torques advancedTorquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                         std::array<double, 7> const &coriolis,
                                         std::array<double, 49> const &dynamicMass);

    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CONTROLLERFRANKA_H
