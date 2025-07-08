//
// Created by shobhit on 04.07.25.
//

#include <iostream>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsGeometry.h>
#include <TUM_SJ_ObstacleAvoidanceLib/OptimizationBasedIKWithOA.h>
#include <TUM_SJ_ObstacleAvoidanceLib/Robot.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <cmath>
#include <TUM_SJ_ObstacleAvoidanceLib/CartesianTrajectory.h>
#include <TUM_SJ_ObstacleAvoidanceLib/ObstacleAvoidanceUtils.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utilsJointValuesMatlab.h>
#include <fstream>
#include <Eigen/Dense>



using namespace std;
using namespace Eigen;


int main() {

    //Read obstacles from Json file
    auto obstaclesMap = ObstacleAvoidance::obstaclesMapFromJson("../config/configDemo/Obstacles.json");


    AndreiUtils::ConfigurationParameters config(static_cast<std::string>("../config/configDemo/configurationParameters.json"),"Configuration");


    auto robotConfig = config.getSubConfig("PandaRobot");
    auto robot = ObstacleAvoidance::Robot(robotConfig);
    auto initialJointPositions = robotConfig.getJson("Joints").at("jointPositionAtBeginning").get<std::vector<double>>();
    for (auto &angle: initialJointPositions) {
        angle = AndreiUtils::deg2Rad(static_cast<double>(angle));
    }
    int const &numberJoints = robot.getNumberJoints();
    auto jointPositionsAtBeginning = ObstacleAvoidance::stdVectorToEigenVector(initialJointPositions);


    auto trajectoryConfig = config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = config.getSubConfig("inverseKinematicsWithOA");
    // constraintTolerance
    auto constraintTolerance = inverseKinematicsConfig.get<double>("constraintTolerance");
    auto stepTolerance = inverseKinematicsConfig.get<double>("stepTolerance");
    auto optimalityTolerance = inverseKinematicsConfig.get<double>("optimalityTolerance");
    auto maxIterations = inverseKinematicsConfig.get<int>("maxIterations");

    // maxIterations,optimalityTolerance,stepTolerance
    double staticWeightSlack = inverseKinematicsConfig.getJson("SlackParameters").at(
            "SlackObjectiveWeight").get<double>();

    bool useDynamicSlackObjectiveWeightPre = inverseKinematicsConfig.getJson("SlackParameters").at(
            "useDynamicSlackObjectiveWeightPreTrajectory").get<bool>();

    auto [desiredPositionTCPTrajectory, desiredVelocityTCPTrajectory, desiredAccelerationTCPTrajectory] =
            ObstacleAvoidance::CartesianTrajectory::positionTrajectory(trajectoryConfig);
    auto [desiredQuaternionsTCPTrajectory, desiredAngularVelocityTCPTrajectory, desiredAngularAccelTrajectory] =
            ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(trajectoryConfig);

    Eigen::VectorXd wayPointTimes = ObstacleAvoidance::stdVectorToEigenVector(
            trajectoryConfig.get<vector<double> >("waypointTimes"));
    auto sampleTime = trajectoryConfig.get<double>("trajectorySampleTime");


    Eigen::VectorXd trajTimesTrajectory = ObstacleAvoidance::generateSequence(wayPointTimes(0), sampleTime,
                                                                              wayPointTimes(Eigen::last));


    auto diagValuesKp = ObstacleAvoidance::stdVectorToEigenVector(
            trajectoryConfig.getJson("inverseKinematics").at("positionKp").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKp(diagValuesKp);
    Eigen::MatrixXd Kp = diagMatrixKp.toDenseMatrix();

    auto diagValuesKo = ObstacleAvoidance::stdVectorToEigenVector(
            trajectoryConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();

    auto jointVelocityWeight = ObstacleAvoidance::stdVectorToEigenVector(
            inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
                    "jointVelocityWeight").get<std::vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();


    //Current TCP Waypoint before the trajectory begins (Pre-Waypoint)
    auto transformationTCP = robot.fkmCartesianTCP(jointPositionsAtBeginning);
    VectorXd preWaypoint = transformationTCP(Eigen::seq(0,2),3);
    cout<<preWaypoint<<endl;

    //Current TCP Orientation before the trajectory begins (Pre-Orientation)
    auto preOrientation = ObstacleAvoidance::computeQuaternionFromMatrix(transformationTCP);
    cout<<preOrientation<<endl;

    Eigen::Matrix<double, 3, 2> preWaypointMatrix;
    preWaypointMatrix(Eigen::all, 0) = preWaypoint;
    preWaypointMatrix(Eigen::all, 1) = desiredPositionTCPTrajectory(Eigen::all, 0);

    Eigen::Matrix<double, 4, 2> preOrientationsMatrix;
    preOrientationsMatrix(Eigen::all, 0) = preOrientation;
    preOrientationsMatrix(Eigen::all, 1) = desiredQuaternionsTCPTrajectory(Eigen::all, 0);

    double preEndTimeTrajectory = trajectoryConfig.get<double>("preTrajectoryTime");
    Eigen::RowVector2d preWaypointTimes;
    preWaypointTimes << 0, preEndTimeTrajectory;

    auto [desiredPositionTCPPre, desiredVelocityTCPPre, xdAccelPre] =
            ObstacleAvoidance::CartesianTrajectory::positionTrajectory(
                    preWaypointMatrix, preWaypointTimes, sampleTime, "cubic",
                    Eigen::MatrixXd::Zero(3, 2), Eigen::MatrixXd::Zero(3, 2));
    auto [desiredQuaternionsTCPPre, desiredAngularVelocityTCPPre, desiredAngularAccelPre] =
            ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(
                    preOrientationsMatrix, preWaypointTimes, sampleTime, "cubic");

    Eigen::VectorXd trajTimesTrajectoryPre = ObstacleAvoidance::generateSequence(preWaypointTimes(0), sampleTime,
                                                                                 preWaypointTimes(Eigen::last));

    int startingIndexTrajectory = trajTimesTrajectoryPre.size();


    // TODO: stitch desired...TCPPre and desired...TCPTrajectory
    Eigen::MatrixXd desiredPositionTCP(desiredPositionTCPPre.rows(),
                                       desiredPositionTCPPre.cols() + desiredPositionTCPTrajectory.cols());
    // 3 rows, sum of column count
    desiredPositionTCP << desiredPositionTCPPre, desiredPositionTCPTrajectory;

    Eigen::MatrixXd desiredVelocityTCP(desiredVelocityTCPPre.rows(),
                                       desiredVelocityTCPPre.cols() + desiredVelocityTCPTrajectory.cols());
    // 3 rows, sum of column count
    desiredVelocityTCP << desiredVelocityTCPPre, desiredVelocityTCPTrajectory;

    Eigen::MatrixXd desiredQuaternionsTCP(desiredQuaternionsTCPPre.rows(),
                                          desiredQuaternionsTCPPre.cols() + desiredQuaternionsTCPTrajectory.cols());
    // 3 rows, sum of column count
    desiredQuaternionsTCP << desiredQuaternionsTCPPre, desiredQuaternionsTCPTrajectory;

    Eigen::MatrixXd desiredAngularVelocityTCP(desiredAngularVelocityTCPPre.rows(),
                                              desiredAngularVelocityTCPPre.cols() +
                                              desiredAngularVelocityTCPTrajectory.cols());
    // 3 rows, sum of column count
    desiredAngularVelocityTCP << desiredAngularVelocityTCPPre, desiredAngularVelocityTCPTrajectory;

    int sizeTrajectory = trajTimesTrajectory.size();
    Eigen::VectorXd trajTimes(trajTimesTrajectoryPre.size() + sizeTrajectory);
    auto addTime = (preEndTimeTrajectory + sampleTime) * Eigen::VectorXd::Ones(sizeTrajectory);
    trajTimesTrajectory += addTime;
    trajTimes << trajTimesTrajectoryPre, trajTimesTrajectory;

    Eigen::MatrixXd actualJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    Eigen::MatrixXd actualJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    Eigen::MatrixXd desiredJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    Eigen::MatrixXd desiredJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    actualJointValuesMatrix(Eigen::all, 0) = jointPositionsAtBeginning;
    cout << "Starting Joint Position: \n" << jointPositionsAtBeginning << endl;
    desiredJointValuesMatrix(Eigen::all, 0) = jointPositionsAtBeginning;

    auto jointVelMinValues = robot.getJoints()->velMinValues;
    auto jointVelMaxValues = robot.getJoints()->velMaxValues;
    auto jointMinValues = robot.getJoints()->minValues;
    auto jointMaxValues = robot.getJoints()->maxValues;

    ObstacleAvoidance::OptimizationBasedIKWithOA ik(inverseKinematicsConfig);

    long index = 0;

    Eigen::VectorXd desiredJointVelocityUpdate = Eigen::VectorXd::Zero(numberJoints);

    Eigen::VectorXd jointVelocityObstacleAvoidance = Eigen::VectorXd::Zero(numberJoints);

   /* ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/desiredTcpTrajectory.csv",
                                        desiredPositionTCP);*/
    // all the variables for plotting results:

    Eigen::MatrixXd distanceAllLinks(numberJoints + 1, trajTimes.size());
    distanceAllLinks.setConstant(std::numeric_limits<double>::quiet_NaN());


    Eigen::MatrixXd minimumDistanceRobotObstacles(1, trajTimes.size());

    minimumDistanceRobotObstacles.setConstant(std::numeric_limits<double>::quiet_NaN());

    Eigen::MatrixXd actualPositionTCP(desiredPositionTCP.rows(), trajTimes.size());
    actualPositionTCP.setConstant(std::numeric_limits<double>::quiet_NaN());

    Eigen::MatrixXd errorPositionTCP(desiredPositionTCP.rows(), trajTimes.size());
    errorPositionTCP.setConstant(std::numeric_limits<double>::quiet_NaN());


    for (int i = 0; i < trajTimes.size(); i++) {

        if (i > 0) {
            actualJointValuesMatrix(all, i) = actualJointValuesMatrix(all, i - 1);
            //cout<<"transformTcpToBase: \n" <<transformTcpToBase<< endl;
        }

        auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, i));
        VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

        auto errorCartesian = (desiredPositionTCP(all, i) - positionTcpCurrent);
        errorPositionTCP(Eigen::all,i) = errorCartesian;
        auto xdEffective = desiredVelocityTCP(all, i) + (Kp * errorCartesian);

        auto orientationError = ObstacleAvoidance::computeOrientationError(transformTcpToBase, desiredQuaternionsTCP(all, i));


        auto angularVelocityEffective =
                desiredAngularVelocityTCP(all, i) + (Ko * orientationError);

        auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(all, i));
        Eigen::VectorXd poseVelocityEffective(6);
        poseVelocityEffective(seq(0, 2)) = xdEffective;
        poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
        //        cout<<"error between final and current: "<<((waypointOne - positionTcpCurrent).norm())<<endl;

        //cout << "error current: " << errorCartesian.norm() << endl;


        //
        auto [jg, bg, minDistance] = robot.obstacleAvoidanceEquation(obstaclesMap, actualJointValuesMatrix(Eigen::all, i),
                                                                     jointVelocityObstacleAvoidance);




        if (useDynamicSlackObjectiveWeightPre) {
            double dynamicSlackObjectiveWeight =
                    staticWeightSlack / (desiredPositionTCP(Eigen::all, startingIndexTrajectory) - positionTcpCurrent).
                            norm();
            // cout << dynamicSlackObjective << endl;
            ik.setConfigSlackWeight(dynamicSlackObjectiveWeight);
            ik.setDynamicSlackFlagValueForOA(false);
            ik.setValueInequalityConstraints(false);
            if (index >= startingIndexTrajectory && useDynamicSlackObjectiveWeightPre) {
                ik.setConfigSlackWeight(staticWeightSlack);
                ik.setDynamicSlackFlagValueForOA(true);
                ik.setValueInequalityConstraints(true);
                useDynamicSlackObjectiveWeightPre = false;
            }
        }
        auto [optimalJointVelocity, ExitFlag] = ik.inverseKinematicsWithOA(actualJointValuesMatrix(all, i),
                                                                           JacobiMatrix,
                                                                           poseVelocityEffective,
                                                                           jointMinValues, jointMaxValues,
                                                                           jointVelMinValues,
                                                                           jointVelMaxValues, jg, bg,
                                                                           jointVelocityWeightMatrix, constraintTolerance, maxIterations, optimalityTolerance, stepTolerance);

        //cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;
        desiredJointVelocityMatrix(all, i) = optimalJointVelocity;
        jointVelocityObstacleAvoidance = optimalJointVelocity;
        if (ExitFlag < 0) {
            cout << "Motion failed at index: " << i << endl;
            cout << "ExitFlag: " << ExitFlag << endl;
            cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
            cout << "jg at index " + std::to_string(i) + ": \n" << jg << endl;

            cout << "bg at index " + std::to_string(i) + ": \n" << bg << endl;

            cout<< "minimum distance at index " + std::to_string(i) + ": "<<minDistance<<endl;

            break;
        }

        /*if (minDistance < 0.01){
            cout<<"minDistance has become less than 0.01 at " +std::to_string(i)+ ": "<<minDistance<<endl;
            break;

        }*/

        if (i > 0) {
            double tStart = trajTimes(i - 1);
            double tEnd = trajTimes(i);
            double timespan[2] = {tStart, tEnd};
            VectorXd desiredJointPosition = (desiredJointValuesMatrix.col(i - 1));
            VectorXd desiredJointVelocityCurrent = (desiredJointVelocityMatrix.col(i));
            desiredJointValuesMatrix(all, i) = ObstacleAvoidance::integrateConstantRungeKutta(desiredJointVelocityCurrent, timespan,
                                                                           desiredJointPosition);
        }
        actualJointValuesMatrix(all, i) = desiredJointValuesMatrix(all, i);
        bool valid = robot.getJoints()->isConfigurationValid(actualJointValuesMatrix(all, i), true);
        if (!valid) {
            cout << "At this index: " << i <<" joint limits were violated"<< endl;
            break;
        }
        //cout<<"desired Joint Value: \n"<< desiredJointValuesMatrix(all, i+1)<<endl;
    }

    ObstacleAvoidance::writeMatrixToCSV("../outputDemo/robotJointPositions.csv",actualJointValuesMatrix);



    return 0;
}