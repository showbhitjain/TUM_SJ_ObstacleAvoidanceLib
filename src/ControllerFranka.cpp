//
// Created by shobhit on 04.12.24.
//

#include <TUM_SJ_ObstacleAvoidanceLib/ControllerFranka.h>

#include <utility>

using namespace std;
using namespace franka;
using namespace ObstacleAvoidance;

ControllerFranka::ControllerFranka(Vector7d const &cutOffTorquesMax, Vector7d const &cutOffTorquesMin,
                                   Vector7d const &Kp, Vector7d const &Kd): maxTorques(cutOffTorquesMax),
                                                                            minTorques(cutOffTorquesMin) {
    const Eigen::DiagonalMatrix<double, 7> diagMatrixKp(Kp);
    this->Kp = diagMatrixKp.toDenseMatrix();
    const Eigen::DiagonalMatrix<double, 7> diagMatrixKd(Kd);
    this->Kd = diagMatrixKd.toDenseMatrix();
}

franka::Torques ControllerFranka::torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
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

    for (int i=0; i<Torques.size(); ++i) {

        if (Torques(i) > maxTorques(i)) {
            Torques(i) = maxTorques(i);
        }else if (Torques(i) < minTorques(i)) {
            Torques(i) = minTorques(i);
        }
    }

    std::array<double, 7> desiredTorques{};
    std::copy_n(Torques.data(), 7, desiredTorques.begin());
    return desiredTorques;
}

franka::Torques ControllerFranka::torquePD(franka::RobotState const &state, Vector7d const &desiredJointPosition,
                                           Vector7d const &desiredJointVelocity, bool const &withCoriolisVector,
                                           std::array<double, 7> const &coriolis) {
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> measuredJointPosition(state.q.data());
    Eigen::Map<const Eigen::Matrix<double, 7, 1>> measuredJointVelocity(state.dq.data());

    Vector7d Torques;
    if (withCoriolisVector == true) {
        Eigen::Map<const Eigen::Matrix<double, 7, 1>> coriolisVector(coriolis.data());
        Torques = this->Kp * (desiredJointPosition - measuredJointPosition) + this->Kd * (
                      desiredJointVelocity - measuredJointVelocity) + coriolisVector;
    } else {
        Torques = this->Kp * (desiredJointPosition - measuredJointPosition) + this->Kd * (
                      desiredJointVelocity - measuredJointVelocity);
    }
    for (int i=0; i<Torques.size(); ++i) {

        if (Torques(i) > maxTorques(i)) {
            Torques(i) = maxTorques(i);
        }else if (Torques(i) < minTorques(i)) {
            Torques(i) = minTorques(i);
        }
    }

    std::array<double, 7> desiredTorques{};
    std::copy_n(Torques.data(), 7, desiredTorques.begin());
    return desiredTorques;
}


franka::Torques ControllerFranka::advancedTorquePD(franka::RobotState const &state,
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



