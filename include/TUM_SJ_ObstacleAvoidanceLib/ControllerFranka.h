//
// Created by shobhit on 04.12.24.
//

#ifndef CONTROLLERPD_H
#define CONTROLLERPD_H

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
        Matrix7d Kp;
        Matrix7d Kd;
        Vector7d maxTorques;
        Vector7d minTorques;

    public:
        explicit ControllerFranka(Vector7d const &cutOffTorquesMax, Vector7d const &cutOffTorquesMin,
                                  Vector7d const &Kp = Vector7d::Ones(), Vector7d const &Kd = Vector7d::Ones());

        franka::Torques torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                 bool const &withCoriolisVector, std::array<double, 7> const &
                                         coriolis = {});

        franka::Torques torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                 Vector7d const &desiredJointVelocity, bool const &withCoriolisVector,
                                 std::array<double, 7> const &coriolis = {});

        franka::Torques advancedTorquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                         std::array<double, 7> const &coriolis,
                                         std::array<double, 49> const &dynamicMass);

        //franka::Torques torquePD(franka::RobotState const & state, Vector7d const & desiredJointVelocity);

        //franka::Torques advancedTorquePD(franka::RobotState const & state, Vector7d const & desiredJointVelocity);

        franka::Torques torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                 Vector7d const &desiredJointVelocity);

        franka::Torques advancedTorquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                         Vector7d const &desiredJointVelocity);


        /*franka::Torques torqueControlPD(franka::RobotState const &);
        torqueControlAdvancedPD();*/
    };
}

#endif //CONTROLLERPD_H
