//
// Created by shobhit on 04.12.24.
//

#include <controllerFranka.h>

using namespace std;
using namespace franka;

controllerFranka::controllerFranka(const Vector7d &Kp, const Vector7d &Kd) {
    const Eigen::DiagonalMatrix<double, 7> diagMatrixKp(Kp);
    this->Kp = diagMatrixKp.toDenseMatrix();
    const Eigen::DiagonalMatrix<double, 7> diagMatrixKd(Kd);
    this->Kd = diagMatrixKd.toDenseMatrix();
}

franka::Torques controllerFranka::torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                           bool const &withCoriolisVector, std::array<double, 7> const &coriolis) {
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> measuredJointPosition(state.q.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> measuredJointVelocity(state.dq.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> desiredJointVelocity(state.dq_d.data());
    Vector7d Torques;
    if (withCoriolisVector == true) {

        Eigen::Map<const Eigen::Matrix<double, 7, 1>> coriolisVector(coriolis.data());
         Torques = this->Kp * (desiredJointPosition - measuredJointPosition) + this->Kd * (
                               desiredJointVelocity - measuredJointVelocity) + coriolisVector;
    } else {
         Torques = this->Kp * (desiredJointPosition - measuredJointPosition) + this->Kd * (
                               desiredJointVelocity - measuredJointVelocity);
    }

    std::array<double, 7> desiredTorques{};
    std::copy_n(Torques.data(), 7, desiredTorques.begin());
    return desiredTorques;
}


franka::Torques controllerFranka::advancedTorquePD(franka::RobotState const &state,
                                                   Vector7d const &desiredJointPosition,
                                                   std::array<double, 7> const &coriolis,
                                                   std::array<double, 49> const &dynamicMass) {
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> measuredJointPosition(state.q.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> measuredJointVelocity(state.dq.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> desiredJointVelocity(state.dq_d.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> coriolisVector(coriolis.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> desiredJointAcceleration(state.ddq_d.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 7>> dynamicMassMatrix(dynamicMass.data());


    Vector7d Torques = this->Kp * (desiredJointPosition - measuredJointPosition) + this->Kd * (
                           desiredJointVelocity - measuredJointVelocity) + dynamicMassMatrix * desiredJointAcceleration
                       + coriolisVector;

    std::array<double, 7> desiredTorques{};
    std::copy_n(Torques.data(), 7, desiredTorques.begin());
    return desiredTorques;
}


/*
franka::Torques controllerFranka::torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
    Vector7d const &desiredJointVelocity) {
}

franka::Torques controllerFranka::advancedTorquePD(franka::RobotState const &state,
    Vector7d const &desiredJointPosition, Vector7d const &desiredJointVelocity) {
}
*/
