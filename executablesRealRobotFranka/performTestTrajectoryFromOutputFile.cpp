//
// Created by shobhit on 07.02.25.
//
#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsThread.h>

#include <iostream>

#include <ranges>

#include<TUM_SJ_ObstacleAvoidanceLib/utils.h>


#include <TUM_SJ_ObstacleAvoidanceLib/OptimizationBasedIKWithOA.h>
#include <TUM_SJ_ObstacleAvoidanceLib/Robot.h>

#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <cmath>
#include <TUM_SJ_ObstacleAvoidanceLib/CartesianTrajectory.h>
#include <TUM_SJ_ObstacleAvoidanceLib/ObstacleAvoidanceUtils.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utilsJointValuesMatlab.h>
#include <TUM_SJ_ObstacleAvoidanceLib/ControllerFranka.h>
#include <fstream>
#include <Eigen/Dense>


using namespace ObstacleAvoidance;
using namespace std;
using namespace Eigen;
using namespace AndreiUtils;
using namespace franka;


using Vector7d = Eigen::Matrix<double, 7, 1>;


void goToStartPosition() {
    ConfigurationParameters Config(
            static_cast<std::string const &>("../config/configShobhit/configurationParameters.json"),
            static_cast<std::string const &>("Configuration"));


    auto trajConfig = Config.getSubConfig("Trajectory");
    double preTrajectoryWayPointTime = trajConfig.get<double>("preTrajectoryTime");

    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematicsPreTrajectory");
    auto robot = ObstacleAvoidance::Robot("../config/configShobhit/robotModellingParameters.json", "RobotModelling",
                                          "PandaRobot");

    auto robotJsonFile = readJsonFile("../config/configShobhit/pandaRobot.json");
    auto robotConfig = ConfigurationParameters(robotJsonFile, "PandaRobot");
    auto robotParametersJson = robotConfig.getJson("robotParameters");
    auto robotIpAddress = robotParametersJson["robotIpAddress"].get<std::string>();
    Vector7d maxTorques = stdVectorToEigenVector(robotParametersJson["maximumTorques"].get<std::vector<double> >());
    Vector7d minTorques = stdVectorToEigenVector(robotParametersJson["minimumTorques"].get<std::vector<double> >());
    Vector7d pdControllerKp = stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kp"].get<std::vector<double> >());
    Vector7d pdControllerKd = stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kd"].get<std::vector<double> >());


    double sampleTime = trajConfig.get<double>("trajectorySampleTime");
    double preEndTimeTrajectory = trajConfig.get<double>("preTrajectoryTime");
    Eigen::RowVector2d preWaypointTimes;
    preWaypointTimes << 0, preEndTimeTrajectory;


    VectorXd trajTimes = generateSequence(0, sampleTime, preEndTimeTrajectory);
    int numberJoints = static_cast<int>(robot.getNumberJoints());
    MatrixXd actualJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    MatrixXd actualJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    MatrixXd desiredJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    MatrixXd desiredJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    auto diagValuesKp = stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("positionKp").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKp(diagValuesKp);
    Eigen::MatrixXd Kp = diagMatrixKp.toDenseMatrix();

    auto diagValuesKo = stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();
    auto waypoints = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double> > >("Waypoints"));
    auto orientations = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double> > >("orientations"));
    Eigen::MatrixXd jg = Eigen::MatrixXd::Zero(1, numberJoints);
    Eigen::VectorXd bg = Eigen::VectorXd::Zero(1);
    double staticWeightSlack = inverseKinematicsConfig.getJson("SlackParameters").at(
            "SlackObjectiveWeight").get<double>();
    bg.resize(0);
    jg.resize(0, 0);


    try {
        franka::Robot realRobot(robotIpAddress);
        setDefaultBehavior(realRobot);


        std::cout << "WARNING: This example will move the robot! "
                  << "Please make sure to have the user stop button at hand!" << std::endl
                  << "Press Enter to continue..." << std::endl;
        std::cin.ignore();

        Vector7d startingMeasuredJointPosition = Eigen::Map<Eigen::Matrix<double, 7, 1> >(
                realRobot.readOnce().q.data());
        actualJointValuesMatrix(all, 0) = startingMeasuredJointPosition;
        desiredJointValuesMatrix(all, 0) = startingMeasuredJointPosition;

        Matrix4d transformationTCP = robot.fkmCartesianTCP(startingMeasuredJointPosition);


        Vector3d preWayPoint = transformationTCP(seq(0, 2), 3);

        Matrix<double, 3, 2> preWaypointMatrix;
        preWaypointMatrix(all, 0) = preWayPoint;
        preWaypointMatrix(all, 1) = waypoints(all, 0);

        auto [desiredPositionTCP, desiredVelocityTCP, xdAccel] =
                ObstacleAvoidance::CartesianTrajectory::positionTrajectory(
                        preWaypointMatrix, preWaypointTimes, sampleTime, "cubic",
                        Eigen::MatrixXd::Zero(3, 2), Eigen::MatrixXd::Zero(3, 2));

        Matrix<double, 4, 2> preOrientations;
        preOrientations(all, 0) = computeQuaternionFromMatrix(transformationTCP);
        preOrientations(all, 1) = orientations(all, 0);

        auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
                ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(
                        preOrientations, preWaypointTimes, sampleTime, "cubic");


        auto jointVelMinValues = robot.getJoints()->velMinValues;
        auto jointVelMaxValues = robot.getJoints()->velMaxValues;
        auto jointMinValues = robot.getJoints()->minValues;
        auto jointMaxValues = robot.getJoints()->maxValues;

        OptimizationBasedIKWithOA ik(inverseKinematicsConfig);
        ControllerFranka controller(maxTorques, minTorques, pdControllerKp, pdControllerKd);
        auto jointVelocityWeight = stdVectorToEigenVector(inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
                "jointVelocityWeight").get<std::vector<double> >());
        Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
        Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();
        // Load the kinematics and dynamics model.
        franka::Model model = realRobot.loadModel();

        long index = 0;
        Vector7d desiredJointPositionUpdate;
        desiredJointPositionUpdate = startingMeasuredJointPosition;

        Vector7d desiredJointVelocityUpdate = Vector7d::Zero();

        //Just Using External Torque Controller
        std::function<franka::Torques(const franka::RobotState &robotState,
                                      franka::Duration period)> torqueCallback = [&](
                const franka::RobotState &robotState, const franka::Duration period) -> franka::Torques {
            auto coriolisData = model.coriolis(robotState);
            auto const &jointValues = robotState.q;
            auto const &jointVelocities = robotState.dq;

            auto massMatrix = model.mass(robotState);
            Eigen::Map<const Eigen::Matrix<double, 7, 7>> massInertiaMatrix(massMatrix.data());
            //            jointVelocityWeightMatrix = massInertiaMatrix;
            long lastPeriod = static_cast<long>(period.toMSec());
            index += lastPeriod;

            if (index < trajTimes.size()) {
                actualJointValuesMatrix(all, index) = Eigen::Map<const Eigen::Matrix<double, 7, 1>>(
                        robotState.q.data());
                auto measuredJointValueThisCylcle = Eigen::Map<const Eigen::Matrix<double, 7, 1>>(robotState.q.data());


                auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, index));
                VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);


                auto errorCartesian = (desiredPositionTCP(all, index) - positionTcpCurrent);
                auto xdEffective = desiredVelocityTCP(all, index) + (
                        Kp * errorCartesian);
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

                // cout << "error between final and current: " << ((waypoints(all, 0) - positionTcpCurrent).norm()) <<
                //endl;

                //cout << "error current: " << errorCartesian.norm() << endl;
                double dynamicSlackObjective = (staticWeightSlack / ((waypoints(all, 0) - positionTcpCurrent).norm()));
                cout << dynamicSlackObjective << endl;
                ik.setConfigSlackWeight(dynamicSlackObjective);

                auto [optimalJointVelocity, exitFlag] = ik.inverseKinematicsWithOA(
                        actualJointValuesMatrix(all, index), JacobiMatrix,
                        poseVelocityEffective,
                        jointMinValues, jointMaxValues,
                        jointVelMinValues,
                        jointVelMaxValues, jg, bg, jointVelocityWeightMatrix,1e-6,400,1e-6,1e-6);
                //cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
                if (exitFlag < 0) {
                    throw std::runtime_error(
                            "ExitFlag: " + std::to_string(exitFlag) + "Motion failed at index: " +
                            std::to_string(index));
                }

                desiredJointVelocityMatrix(all, index) = optimalJointVelocity;


                double tEnd = trajTimes(index);
                double tStart = trajTimes(index - lastPeriod);

                double timespan[2] = {tStart, tEnd};
                //        double dt = (tEnd - tStart) / 10;


                if (index > 0) {
                    desiredJointValuesMatrix(all, index) = integrateConstantRungeKutta(
                            desiredJointVelocityMatrix(all, index), timespan, desiredJointPositionUpdate);
                    desiredJointPositionUpdate = desiredJointValuesMatrix(all, index);
                }
                desiredJointVelocityUpdate = optimalJointVelocity;
                //cout << "desired Joint Value: \n" << desiredJointValuesMatrix(all, index) << endl;
            }
            VectorXd desiredJointPositionCurrent = desiredJointPositionUpdate;
            VectorXd desiredJointVelocityCurrent = desiredJointVelocityUpdate;
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
}



int main(){
    goToStartPosition();
    auto arguments = readJsonFile("../config/playWithVisualInputArguments.json");
    auto taskRepresentationLibConfig = arguments.at("TaskRepresentationLib");
    // arguments.contains("Hello World!");

    // processVisualData(arguments);
    ConfigurationParameters Config(
            static_cast<std::string const &>("../config/configShobhit/configurationParameters.json"),
            static_cast<std::string const &>("Configuration"));

    auto trajConfig = Config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematicsWithOA");
    auto robotJsonFile = readJsonFile("../config/configShobhit/pandaRobot.json");
    auto robotConfig = ConfigurationParameters(robotJsonFile, "PandaRobot");
    auto robotParametersJson = robotConfig.getJson("robotParameters");
    auto robotIpAddress = robotParametersJson["robotIpAddress"].get<std::string>();

    Vector7d maxTorques = stdVectorToEigenVector(robotParametersJson["maximumTorques"].get<std::vector<double> >());
    Vector7d minTorques = stdVectorToEigenVector(robotParametersJson["minimumTorques"].get<std::vector<double> >());
    Vector7d pdControllerKp = stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kp"].get<std::vector<double> >());
    Vector7d pdControllerKd = stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kd"].get<std::vector<double> >());

    try {
        auto desiredJointValuesMatrix = readMatrixFromCSV("../outputDesiredJointValuesDemoPartTwo.csv");
        auto desiredJointVelocityMatrix = readMatrixFromCSV("../outputDesiredJointVelocityDemoPartTwo.csv");
        franka::Robot realRobot(robotIpAddress);
        setDefaultBehavior(realRobot);

        std::cout << "WARNING: This example will move the robot! "
                  << "Please make sure to have the user stop button at hand!" << std::endl
                  << "Press Enter to continue..." << std::endl;
        std::cin.ignore();

        franka::Model model = realRobot.loadModel();

        size_t index = 0;

        /// Set additional parameters always before the control loop, NEVER in the control loop

        ControllerFranka controller(maxTorques,minTorques,pdControllerKp,pdControllerKd);

        std::function<franka::Torques(const franka::RobotState &robot_state,
                                      franka::Duration period)> torqueCallback = [&](
                const franka::RobotState &robot_state, franka::Duration period) -> franka::Torques {


            index += period.toMSec();
            if (index >= desiredJointValuesMatrix.cols()) {
                index = desiredJointValuesMatrix.cols() - 1;
            }
            Vector7d currentDesiredJointValues = desiredJointValuesMatrix(all, index);
            Vector7d currentDesiredJointVelocity = desiredJointVelocityMatrix(all, index);
            if (index >= desiredJointValuesMatrix.cols()) {


                return franka::MotionFinished(controller.torquePD(robot_state, currentDesiredJointValues,currentDesiredJointVelocity, true, model.coriolis(robot_state)));

            }
            return controller.torquePD(robot_state, currentDesiredJointValues,currentDesiredJointVelocity, true, model.coriolis(robot_state));
            //return controller.advancedTorquePD(robot_state,desiredJointPositon,model.coriolis(robot_state),model.mass(robot_state));
        };


        //realRobot.control(torqueCallback, jointMotionCallback, true);
        realRobot.control(torqueCallback, true);


    } catch (const franka::Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }



    return 0;
}
