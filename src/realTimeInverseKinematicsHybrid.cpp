//
// Created by shobhit on 12.12.24.
//


#include <iostream>
#include <TUM_SJ_ObstacleAvoidanceLib/InverseKinematics.h>
#include <TUM_SJ_ObstacleAvoidanceLib/Robot.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <cmath>
#include <TUM_SJ_ObstacleAvoidanceLib/CartesianTrajectory.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utilsJointValuesMatlab.h>
#include <fstream>
#include <Eigen/Dense>
#include <TUM_SJ_ObstacleAvoidanceLib/ControllerFranka.h>

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
    cout << "HomeJointPosition:" << HomeJointPosition << endl;

    FILE *fp;
    char const *filename = "frankaData.txt";

    // Open the file for writing in binary mode
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

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


        Vector7d startingMeasuredJointPosition = Eigen::Map<Eigen::Matrix<double, 7,
            1> >(realRobot.readOnce().q.data());
        actualJointValuesMatrix(all, 0) = startingMeasuredJointPosition;
        cout << "Starting Joint Position: \n" << startingMeasuredJointPosition << endl;
        desiredJointValuesMatrix(all, 0) = HomeJointPosition;
        auto jointVelMinValues = robot.getJoints()->velMinValues;
        auto jointVelMaxValues = robot.getJoints()->velMaxValues;
        auto jointMinValues = robot.getJoints()->minValues;
        auto jointMaxValues = robot.getJoints()->maxValues;

        Vector7d maxTorques = {80, 80, 80, 80, 9, 9, 9};
        Vector7d minTorques = {-80, -80, -80, -80, -9, -9, -9};

        ControllerFranka controller(maxTorques, minTorques, {600.0, 600.0, 600.0, 600.0, 250.0, 150.0, 50.0},
                                    {50.0, 50.0, 50.0, 50.0, 30.0, 25.0, 15.0});

        InverseKinematics ik(inverseKinematicsConfig);

        // Load the kinematics and dynamics model.

        franka::Model model = realRobot.loadModel();


        long index = 0;

        //Just Using External Torque Controller
        std::function<franka::Torques(const franka::RobotState &robotState,
                                      franka::Duration period)> torqueCallback = [&](
            const franka::RobotState &robotState, franka::Duration period) -> franka::Torques {
            auto coriolisData = model.coriolis(robotState);
            auto const &jointValues = robotState.q;
            auto const &jointVelocities = robotState.dq;


            /*if (period.toMSec()> 0) {
index += static_cast<long>(period.toMSec());
}*/
            if (period.toMSec() == 0) {
                /*cout << "First value of Period: " << period.toMSec() << endl;
                cout << "First measured Value: " << Eigen::Map<const Eigen::Matrix<double, 7, 1>>(robotState.q.data())
                        << endl;*/
            }
            index += static_cast<long>(period.toMSec());
            /*if (index > trajTimes.size()-2) {
                index = desiredJointValuesMatrix.cols() - 2;
            }*/
            //cout << "index: " << index << endl;
            fprintf(fp, "%s %ld:", "At timestep", index);
            fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf; ", coriolisData[0], coriolisData[1], coriolisData[2],
                    coriolisData[3], coriolisData[4], coriolisData[5], coriolisData[6]);
            fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf; ", jointValues[0], jointValues[1], jointValues[2],
                    jointValues[3], jointValues[4], jointValues[5], jointValues[6]);
            fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf\n", jointVelocities[0], jointVelocities[1], jointVelocities[2],
                    jointVelocities[3], jointVelocities[4], jointVelocities[5], jointVelocities[6]);
            if (index < trajTimes.size() - 1) {
                actualJointValuesMatrix(all, index) = Eigen::Map<const Eigen::Matrix<double, 7, 1>>(
                    robotState.q.data());
                auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, index));
                VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

                auto xdEffective = desiredVelocityTCP(all, index) + (
                                       Kp * (desiredPositionTCP(all, index) - positionTcpCurrent));
                auto orientationError = computeOrientationError(
                    transformTcpToBase,
                    desiredQuaternionsTCP(all, index));

                auto angularVelocityEffective = desiredAngularVelocityTCP(all, index) + (Ko * orientationError);

                //cout << "Position Error: " << (desiredPositionTCP(all, index) - positionTcpCurrent) << endl;
                //cout << "Orientation Error: " << orientationError << endl;
                auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(all, index));
                Eigen::VectorXd poseVelocityEffective(6);
                poseVelocityEffective(seq(0, 2)) = xdEffective;
                // poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
                poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
                //cout << "poseVelocityEffective: " << poseVelocityEffective << endl;
                auto ikResult = ik.ikWithConstraints(
                    actualJointValuesMatrix(all, index), JacobiMatrix,
                    poseVelocityEffective,
                    jointMinValues, jointMaxValues,
                    jointVelMinValues,
                    jointVelMaxValues);
                auto optimalJointVelocity = std::get<0>(ikResult);
                auto exitFlag = std::get<1>(ikResult);
                //cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
                if (exitFlag < 0) {
                    /*cout << "Motion failed at index: " << index << endl;
                    cout << "ExitFlag: " << ExitFlag << endl;
                    cout << "optimalJointVelocity: \n" << optimalJointVelocity<<endl;
                    cout << "index: " << index << endl;
                    cout<<"positionTcpCurrent: "<<positionTcpCurrent<<endl;
                    cout << "desiredPositionTCP : \n" << desiredPositionTCP.col(index) << endl;
                    cout<< "desiredQuaternionsTCP: \n"<<desiredQuaternionsTCP.col(index) << endl;
                    cout<< "JacobiMatrix: \n" << JacobiMatrix<<endl;
                    cout<< "orientationError: \n"<<orientationError<<endl;
                    cout<<"positionError: \n"<< (desiredPositionTCP(all, index) - positionTcpCurrent)<<endl;
                    cout<< "angularVelocityEffective: \n"<<angularVelocityEffective<<endl;
                    cout<<"poseVelocityEffective: \n"<<poseVelocityEffective<<endl; */

                    throw std::runtime_error(
                        "ExitFlag: " + std::to_string(exitFlag) + "Motion failed at index: " + std::to_string(index));
                }

                desiredJointVelocityMatrix(all, index) = optimalJointVelocity;

                double tStart = trajTimes(index);
                double tEnd = trajTimes(index + 1);
                double timespan[2] = {tStart, tEnd};
                //        double dt = (tEnd - tStart) / 10;

                desiredJointValuesMatrix(all, index + 1) = integrateConstantRungeKutta(
                    desiredJointVelocityMatrix(all, index), timespan, actualJointValuesMatrix(all, index));

                //cout << "desired Joint Value: \n" << desiredJointValuesMatrix(all, index) << endl;
            }
            VectorXd desiredJointPositionCurrent = (desiredJointValuesMatrix(all, index));
            VectorXd desiredJointVelocityCurrent = (desiredJointVelocityMatrix(all, index));
            /*cout << "index: " << index << endl;
            cout << "control command success rate: " << robotState.control_command_success_rate << endl;*/
            if (index >= trajTimes.size() - 1) {
                return franka::MotionFinished(controller.torquePD(robotState, desiredJointPositionCurrent,
                                                                  desiredJointVelocityCurrent, true,
                                                                  model.coriolis(robotState)));
            }
            return controller.torquePD(robotState, desiredJointPositionCurrent, desiredJointVelocityCurrent, true,
                                       coriolisData);
            //return controller.advancedTorquePD(robot_state,desiredJointPositon,model.coriolis(robot_state),model.mass(robot_state));
        };

        realRobot.control(torqueCallback, true);
    } catch (franka::Exception const &ex) {
        std::cerr << ex.what() << std::endl;
    } catch (std::runtime_error &ex) {
        std::cerr << "Non-franka related exception: " << ex.what() << std::endl;
    }

    fclose(fp);

    return 0;
}
