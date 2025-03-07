//
// Created by shobhit on 03.02.25.
//

#include <iostream>
#include <AndreiUtils/utilsJson.h>
#include <TUM_SJ_ObstacleAvoidanceLib/OptimizationBasedIKWithOA.h>
#include <TUM_SJ_ObstacleAvoidanceLib/Robot.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <cmath>
#include <TUM_SJ_ObstacleAvoidanceLib/CartesianTrajectory.h>
#include <TUM_SJ_ObstacleAvoidanceLib/ObstacleAvoidanceUtils.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utilsJointValuesMatlab.h>
#include <fstream>
#include <Eigen/Dense>


using namespace ObstacleAvoidance;
using namespace std;
using namespace Eigen;
using namespace AndreiUtils;


int main() {
    ConfigurationParameters Config(
        static_cast<std::string const &>("../config/configDemo/configurationParameters.json"),
        static_cast<std::string const &>("Configuration"));

    auto trajConfig = Config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematicsWithOA");


    auto robot = ObstacleAvoidance::Robot("../config/configDemo/robotModellingParameters.json", "RobotModelling",
                                          "PandaRobot");
    auto numberJoints = robot.getNumberJoints();

    //for real robot get the position from robot and do forward kinematics
    Eigen::VectorXd HomeJointPosition(numberJoints);

    HomeJointPosition << 0.196728, 0.247911, 0.155609, -2.03656, -0.0501048 , 2.28091, 1.1657;
    Matrix4d transformationTCP = robot.fkmCartesianTCP(HomeJointPosition);
    Vector3d waypointOne = transformationTCP(seq(0, 2), 3);

    auto waypoints = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double>>>("Waypoints"));

    waypoints(all, 0) = waypointOne;
    cout << waypoints(all, 0) << endl;
    auto wayPointVelocities = vectorMatrixToEigenMatrix(
        trajConfig.get<std::vector<std::vector<double> > >("WayPointVelocity"));

    auto ts = trajConfig.get<double>("trajectorySampleTime");
    auto positionTrajectoryType = trajConfig.get<std::string>("positionTrajectoryType");
    auto waypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double> >("waypointTimes"));

    auto [desiredPositionTCP,desiredVelocityTCP,xdAccel] = CartesianTrajectory::positionTrajectory(
        waypoints, waypointTimes, ts, positionTrajectoryType,
        wayPointVelocities, Eigen::MatrixXd::Zero(3, 2));

    auto orientations = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double> > >("orientations"));
    orientations(all,0)= computeQuaternionFromMatrix(transformationTCP);

    //preOrientations<<
    auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
            CartesianTrajectory::orientationTrajectory(orientations, waypointTimes, ts, "cubic");

    auto ikJson = readJsonFile("../config/configDemo/inverseKinematicsWithOAParameters.json");
    ConfigurationParameters ikConfig(ikJson, "inverseKinematicsWithOA");

    VectorXd trajTimes = generateSequence(waypointTimes(0), 0.001, waypointTimes(last));

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

    actualJointValuesMatrix(all, 0) = HomeJointPosition;
    desiredJointValuesMatrix(all, 0) = HomeJointPosition;
    auto jointVelMinValues = robot.getJoints()->velMinValues;
    auto jointVelMaxValues = robot.getJoints()->velMaxValues;
    auto jointMinValues = robot.getJoints()->minValues;
    auto jointMaxValues = robot.getJoints()->maxValues;

    OptimizationBasedIKWithOA ik(ikConfig);
    auto jointVelocityWeight = stdVectorToEigenVector(inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
        "jointVelocityWeight").get<std::vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();


    double staticWeightSlack = inverseKinematicsConfig.getJson("SlackParameters").at("SlackObjectiveWeight").get<
        double>();



    VectorXd jointVelocityObstacleAvoidance = VectorXd::Zero(numberJoints);

    Vector3d center1 = {0.6000, 0.0, 0.1211};
    VectorXd dimensions1(3);
    dimensions1 << 0.04, 0.04, 0.12;
    auto obstacle1 = Obstacle("Box", center1, dimensions1, {1, 0, 0, 0});

    /* Vector3d center2 = {0.5545, 0.20, 0.3211};

     VectorXd dimensions2(1);
     dimensions2(0) = 0.05;
     cout << dimensions2 << endl;
     auto obstacle2 = Obstacles("Sphere", center2, dimensions2,{1,0,0,0});
*/

    std::vector<Obstacle> obstaclesArray;
    obstaclesArray.push_back(obstacle1);
    //        obstaclesArray.push_back(obstacle2);
    auto obstaclesMap = conversionObstaclesVectorToMap(obstaclesArray);
    Eigen::MatrixXd jg = Eigen::MatrixXd::Zero(1, numberJoints);
    Eigen::VectorXd bg = Eigen::VectorXd::Zero(1);


    jg.resize(0,0);
    bg.resize(0);


    for (int i = 0; i < trajTimes.size(); i++) {
        std::map<std::string, Obstacle> obstaclesMapNew;
        if (i > 0) {
            actualJointValuesMatrix(all, i) = actualJointValuesMatrix(all, i - 1);
            //cout<<"transformTcpToBase: \n" <<transformTcpToBase<< endl;
        }

        auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, i));
        VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

        auto errorCartesian = (desiredPositionTCP(all, i) - positionTcpCurrent);
        auto xdEffective = desiredVelocityTCP(all, i) + (Kp * errorCartesian);

        auto orientationError = computeOrientationError(transformTcpToBase, desiredQuaternionsTCP(all, i));


        auto angularVelocityEffective =
                desiredAngularVelocityTCP(all, i) + (Ko * orientationError);

        auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(all, i));
        Eigen::VectorXd poseVelocityEffective(6);
        poseVelocityEffective(seq(0, 2)) = xdEffective;
        poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
        //        cout<<"error between final and current: "<<((waypointOne - positionTcpCurrent).norm())<<endl;

        cout << "error current: " << errorCartesian.norm() << endl;


        //
          auto [jg, bg, minDistance] = robot.obstacleAvoidanceEquation(obstaclesMapNew, actualJointValuesMatrix(all, i),
                                                                       jointVelocityObstacleAvoidance);


        cout << "jg at " + std::to_string(i) + ": \n" << jg << endl;

        cout << "bg at " + std::to_string(i) + ": \n" << bg << endl;

        cout<< "minimum distance at " + std::to_string(i) + ": "<<minDistance<<endl;


        auto [optimalJointVelocity, ExitFlag] = ik.inverseKinematicsWithOA(actualJointValuesMatrix(all, i),
                                                                           JacobiMatrix,
                                                                           poseVelocityEffective,
                                                                           jointMinValues, jointMaxValues,
                                                                           jointVelMinValues,
                                                                           jointVelMaxValues, jg, bg,
                                                                           jointVelocityWeightMatrix, 1e-6,400,1e-6,1e-6);

        //cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;
        desiredJointVelocityMatrix(all, i) = optimalJointVelocity;
        jointVelocityObstacleAvoidance = optimalJointVelocity;
        if (ExitFlag < 0) {
            cout << "At this step: " << i << endl;
            cout << "ExitFlag: " << ExitFlag << endl;
            cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
            break;
        }

        /*if (minDistance < 0.01){
            cout<<"minDistance has become less than 0.01 at " +std::to_string(i)+ ": "<<minDistance<<endl;
            break;

        }*/

        /*integrate_adaptive(
                controlled_stepper,
                [&desiredJointVelocityCurrent](const State &x, State &dxdt, double t) {
                    jointDynamics(x, dxdt, t, desiredJointVelocityCurrent);
                },
                desiredJointPosition, tStart, tEnd, 0.01  // Provide an initial step size estimate
        );*/
        if (i > 0) {
            double tStart = trajTimes(i - 1);
            double tEnd = trajTimes(i);
            double timespan[2] = {tStart, tEnd};
            VectorXd desiredJointPosition = (desiredJointValuesMatrix.col(i - 1));
            VectorXd desiredJointVelocityCurrent = (desiredJointVelocityMatrix.col(i));
            desiredJointValuesMatrix(all, i) = integrateConstantRungeKutta(desiredJointVelocityCurrent, timespan,
                                                                           desiredJointPosition);
        }
        actualJointValuesMatrix(all, i) = addRandomNoisetoJointsSignal(desiredJointValuesMatrix(all, i), -0.005,
                                                                       0.005);
        bool valid = robot.getJoints()->isConfigurationValid(actualJointValuesMatrix(all, i), true);
        if (!valid) {
            cout << "At this step: " << i << endl;
            break;
        }
        //cout<<"desired Joint Value: \n"<< desiredJointValuesMatrix(all, i+1)<<endl;
    }

    cout << "actualJointValues:  \n" << actualJointValuesMatrix(all, last) << endl;
    writeMatrixToCSV("../outputDesiredJointsDemoPartTwo.csv", desiredJointValuesMatrix);


    return 0;
}
