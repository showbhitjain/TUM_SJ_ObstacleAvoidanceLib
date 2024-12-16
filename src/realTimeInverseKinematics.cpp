//
// Created by shobhit on 12.12.24.
//


#include <iostream>
#include <inverseKinematics.h>
#include <Robot.h>
#include "utils.h"
#include <cmath>
#include <CartesianTrajectory.h>
#include <utilsJointValuesMatlab.h>
#include <fstream>
#include <Eigen/Dense>
#include <controllerFranka.h>

using namespace ObstacleAvoidance;
using namespace std;
using namespace Eigen;
using namespace AndreiUtils;
using namespace franka;

int main() {
    ConfigurationParameters Config(static_cast<std::string const &>("../config/configurationParameters.json"),
                                   static_cast<std::string const &>("Configuration"));
    auto trajConfig = Config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematics");

    auto [desiredPositionTCP, desiredVelocityTCP, desiredAccelerationTCP] =
            CartesianTrajectory::positionTrajectory(trajConfig);
    cout << "desiredPositionTCP at 15.487 s: \n" << desiredPositionTCP.col(15488) << endl;
    cout << " desiredVelocityTCP at 15.469 s: \n" << desiredVelocityTCP.col(15469) << endl;
    auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
            CartesianTrajectory::orientationTrajectory(trajConfig);
    cout << "desiredQuaternion TCP at 15.488 s: /n:" << desiredQuaternionsTCP.col(15488) << endl;

    VectorXd wayPointTimes = stdVectorToEigenVector(trajConfig.get<vector<double> >("waypointTimes"));
    auto ts = trajConfig.get<double>("trajectorySampleTime");
    VectorXd trajTimes = generateSequence(wayPointTimes(0), ts, wayPointTimes(last));
    cout << "Number of Samples: " << trajTimes.size() << endl;
    auto robot = ObstacleAvoidance::Robot("../config/robotModellingParameters.json", "RobotModelling",
                                          "PandaRobot");
    int numberJoints = static_cast<int>(robot.getNumberJoints());

    MatrixXd actualJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    MatrixXd actualJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    MatrixXd desiredJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    MatrixXd desiredJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    auto diagValuesKp = stdVectorToEigenVector(
        trajConfig.getJson("inverseKinematics").at("positionKp").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKp(diagValuesKp);
    Eigen::MatrixXd Kp = diagMatrixKp.toDenseMatrix();
    cout << Kp << endl;
    auto diagValuesKo = stdVectorToEigenVector(
        trajConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();
    cout << Ko << endl;
    Eigen::VectorXd HomeJointPosition(numberJoints);
    HomeJointPosition << 0, 0, 0, -M_PI / 2, 0, M_PI / 2, M_PI / 4;
    std::array<double, 7> firstJointPosition{};
    std::copy_n(HomeJointPosition.data(), 7, firstJointPosition.begin());
    cout<<"HomeJointPosition:" <<HomeJointPosition<<endl;

    try {
        franka::Robot realRobot("192.168.5.10");
        setDefaultBehavior(realRobot);


        //Either give it manually or use analytical Inverse Kinematics to get the first
        //joint position at the first waypoint of the trajectory
        //to send the robot to the starting position of the trajectory


        MotionGenerator pointToPoint(0.2, firstJointPosition);
        std::cout << "WARNING: This example will move the robot! "
                << "Please make sure to have the user stop button at hand!" << std::endl
                << "Press Enter to continue..." << std::endl;
        std::cin.ignore();
        realRobot.control(pointToPoint);
        std::cout << "Finished moving to initial joint configuration." << std::endl;


        Eigen::Map<Eigen::Matrix<double, 7, 1> > startingMeasuredJointPosition(realRobot.readOnce().q.data());
        actualJointValuesMatrix(all, 0) = startingMeasuredJointPosition;
        cout << "Starting Joint Position: \n" << startingMeasuredJointPosition << endl;
        desiredJointValuesMatrix(all, 0) = HomeJointPosition;
        auto jointVelMinValues = robot.getJoints()->velMinValues;
        auto jointVelMaxValues = robot.getJoints()->velMaxValues;
        auto jointMinValues = robot.getJoints()->minValues;
        auto jointMaxValues = robot.getJoints()->maxValues;

        Vector7d maxTorques = {87, 87, 87, 87, 12, 12, 12};
        Vector7d minTorques = {-87, -87, -87, -87, -12, -12, -12};

        controllerFranka controller(maxTorques, minTorques, {20, 20, 20, 30, 30, 10, 10}, {15, 15, 15, 10, 10, 5, 5});

        inverseKinematics ik(inverseKinematicsConfig);

        // Load the kinematics and dynamics model.
        franka::Model model = realRobot.loadModel();


        long index = 0;

        //Just Using External Torque Controller
        std::function<franka::Torques(const franka::RobotState &robot_state,
                                      franka::Duration period)> torqueCallback = [&](
            const franka::RobotState &robot_state, franka::Duration period) -> franka::Torques {
            /*if (period.toMSec()> 0) {
index += static_cast<long>(period.toMSec());
}*/
            if (period.toMSec() == 0) {
                cout << "First value of Period: " << period.toMSec() << endl;
                cout << "First measured Value: " << Eigen::Map<const Eigen::Matrix<double, 7, 1>>(robot_state.q.data())
                        << endl;
            }
            index += static_cast<long>(period.toMSec());
            if (index >= desiredJointValuesMatrix.cols()) {
                index = desiredJointValuesMatrix.cols() - 1;
            }
            cout << "index: " << index << endl;
            actualJointValuesMatrix(all, index) = Eigen::Map<const Eigen::Matrix<double, 7, 1>>(robot_state.q.data());
            auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, index));
            VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

            auto xdEffective = desiredVelocityTCP(all, index) + (
                                   Kp * (desiredPositionTCP(all, index) - positionTcpCurrent));
            auto orientationError = computeOrientationError(
                transformTcpToBase,
                desiredQuaternionsTCP(all, index));

            auto angularVelocityEffective = desiredAngularVelocityTCP(all, index) + (Ko * orientationError);

            cout << "Position Error: " << (desiredPositionTCP(all, index) - positionTcpCurrent) << endl;
            cout << "Orientation Error: " << orientationError << endl;
            auto JacobiMatrix = robot.jacobianCartesianTCP(desiredJointValuesMatrix(all, index));
            Eigen::VectorXd poseVelocityEffective(6);
            poseVelocityEffective(seq(0, 2)) = xdEffective;
            // poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
            poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
            cout << "poseVelocityEffective: " << poseVelocityEffective << endl;
            auto [optimalJointVelocity, ExitFlag] = ik.ikWithConstraints(
                desiredJointValuesMatrix(all, index), JacobiMatrix,
                poseVelocityEffective,
                jointMinValues, jointMaxValues,
                jointVelMinValues,
                jointVelMaxValues);
            cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
            if (ExitFlag < 0) {
                cout << "Motion failed at index: " << index << endl;
                cout << "ExitFlag: " << ExitFlag << endl;
                throw std::runtime_error(
                    "ExitFlag: " + std::to_string(ExitFlag) + "Motion failed at index: " + std::to_string(index));
            }

            desiredJointVelocityMatrix(all, index) = optimalJointVelocity;

            double tStart = trajTimes(index);
            double tEnd = trajTimes(index + 1);
            double timespan[2] = {tStart, tEnd};
            //        double dt = (tEnd - tStart) / 10;
            VectorXd desiredJointPositionCurrent = (desiredJointValuesMatrix(all,index));
            VectorXd desiredJointVelocityCurrent = (desiredJointVelocityMatrix(all,index));
            desiredJointValuesMatrix(all, index + 1) = integrateConstantRungeKutta(
                desiredJointVelocityCurrent, timespan, desiredJointPositionCurrent);

            cout << "desired Joint Value: \n" << desiredJointValuesMatrix(all, index) << endl;

            auto torques = controller.torquePD(robot_state, desiredJointPositionCurrent, desiredJointVelocityCurrent,
                                               true,
                                               model.coriolis(robot_state));
            if (index >= desiredJointValuesMatrix.cols()) {
                return franka::MotionFinished(controller.torquePD(robot_state, desiredJointPositionCurrent,
                                                                  desiredJointVelocityCurrent, true,
                                                                  model.coriolis(robot_state)));
            }
            return controller.torquePD(robot_state, desiredJointPositionCurrent, desiredJointVelocityCurrent, true,
                                       model.coriolis(robot_state));
            //return controller.advancedTorquePD(robot_state,desiredJointPositon,model.coriolis(robot_state),model.mass(robot_state));
        };

        realRobot.control(torqueCallback, true);
    } catch (const franka::Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
