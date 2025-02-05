//
// Created by shobhit on 02.02.25.
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
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematicsPreTrajectory");


    auto robot = ObstacleAvoidance::Robot("../config/configDemo/robotModellingParameters.json", "RobotModelling",
                                          "PandaRobot");
    auto numberJoints = robot.getNumberJoints();
    //for real robot get the position from robot and do forward kinematics
    Eigen::VectorXd HomeJointPosition(numberJoints);
    HomeJointPosition << 0, 0, 0, -M_PI / 2, 0, M_PI / 2, M_PI / 4;

    Matrix4d transformationTCP = robot.fkmCartesianTCP(HomeJointPosition);
    Vector3d preWaypoint = transformationTCP(seq(0, 2), 3);

    auto waypoints = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double>>>("Waypoints"));

    Vector3d waypointOne = waypoints(all, 0);
    Matrix<double, 3, 2> preWaypointMatrix;
    preWaypointMatrix(all, 0) = preWaypoint;
    preWaypointMatrix(all, 1) = waypointOne;
    Eigen::RowVector2d preWaypointTimes;
    preWaypointTimes << 0, 2;

    double ts = 0.001;
    auto [desiredPositionTCP,desiredVelocityTCP,xdAccel] = CartesianTrajectory::positionTrajectory(preWaypointMatrix, preWaypointTimes, ts, "cubic",
                                            Eigen::MatrixXd::Zero(3, 2), Eigen::MatrixXd::Zero(3, 2));

    Matrix<double,4,2> preOrientations;
    preOrientations(all,0) <<0,1,0,0;
    preOrientations(all,1) <<0,1,0,0;
    cout<<preOrientations(all,0)<<endl;
    //preOrientations<<
     auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] = CartesianTrajectory::orientationTrajectory(preOrientations,preWaypointTimes,ts,"cubic");

    auto ikJson = readJsonFile("../config/configDemo/inverseKinematicsPreTrajectory.json");
    ConfigurationParameters ikConfigPre(ikJson,"inverseKinematicsPreTrajectory");

    VectorXd trajTimes = generateSequence(preWaypointTimes(0), 0.001, preWaypointTimes(last));

    MatrixXd actualJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    MatrixXd actualJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    MatrixXd desiredJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    MatrixXd desiredJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    auto diagValuesKp = stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("positionKp").get<vector<double>>());
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

    OptimizationBasedIKWithOA ik(ikConfigPre);
    auto jointVelocityWeight = stdVectorToEigenVector(inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
            "jointVelocityWeight").get<std::vector<double>>());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();

    Eigen::MatrixXd jg = Eigen::MatrixXd::Zero(1, numberJoints);
    Eigen::VectorXd bg = Eigen::VectorXd::Zero(1);
    double staticWeightSlack = inverseKinematicsConfig.getJson("SlackParameters").at("SlackObjectiveWeight").get<double>();
    bg.resize(0);
    jg.resize(0,0);

    for (int i = 0; i < trajTimes.size(); i++) {
        if (i>0) {
            actualJointValuesMatrix(all,i) = actualJointValuesMatrix(all,i-1);
            //cout<<"transformTcpToBase: \n" <<transformTcpToBase<< endl;
        }

        auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, i));
        VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

        auto errorCartesian = (desiredPositionTCP(all, i) - positionTcpCurrent);
        auto xdEffective = desiredVelocityTCP(all, i) + (Kp * errorCartesian);

        auto orientationError = computeOrientationError(transformTcpToBase,desiredQuaternionsTCP(all, i));


        auto angularVelocityEffective =
                desiredAngularVelocityTCP(all, i) + (Ko * orientationError);

        auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(all, i));
        Eigen::VectorXd poseVelocityEffective(6);
        poseVelocityEffective(seq(0, 2)) = xdEffective;
        poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
        cout<<"error between final and current: "<<((waypointOne - positionTcpCurrent).norm())<<endl;

        cout<<"error current: "<<errorCartesian.norm()<<endl;
        double dynamicSlackObjective = (staticWeightSlack/ ((waypointOne - positionTcpCurrent).norm()));
        cout<<dynamicSlackObjective<<endl;
        ik.setConfigSlackWeight(dynamicSlackObjective);

        auto [optimalJointVelocity, ExitFlag] = ik.inverseKinematicsWithOA(actualJointValuesMatrix(all, i),
                                                                           JacobiMatrix,
                                                                           poseVelocityEffective,
                                                                           jointMinValues, jointMaxValues,
                                                                           jointVelMinValues,
                                                                           jointVelMaxValues, jg, bg,
                                                                           jointVelocityWeightMatrix);

        //cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;
        desiredJointVelocityMatrix(all, i) = optimalJointVelocity;

        if (ExitFlag < 0) {
            cout << i << endl;
            cout << "ExitFlag: " << ExitFlag << endl;
            cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
            break;
        }

        /*integrate_adaptive(
                controlled_stepper,
                [&desiredJointVelocityCurrent](const State &x, State &dxdt, double t) {
                    jointDynamics(x, dxdt, t, desiredJointVelocityCurrent);
                },
                desiredJointPosition, tStart, tEnd, 0.01  // Provide an initial step size estimate
        );*/
        if(i>0) {
            double tStart = trajTimes(i-1);
            double tEnd = trajTimes(i );
            double timespan[2] = {tStart, tEnd};
            VectorXd desiredJointPosition = (desiredJointValuesMatrix.col(i-1));
            VectorXd desiredJointVelocityCurrent = (desiredJointVelocityMatrix.col(i));
            desiredJointValuesMatrix(all, i) = integrateConstantRungeKutta(desiredJointVelocityCurrent, timespan,
                                                                           desiredJointPosition);
        }
        actualJointValuesMatrix(all, i) = addRandomNoisetoJointsSignal(desiredJointValuesMatrix(all, i), -0.005,
                                                                           0.005);
        //cout<<"desired Joint Value: \n"<< desiredJointValuesMatrix(all, i+1)<<endl;
    }


    cout << "actualJointValues:  \n" << actualJointValuesMatrix(all, last) << endl;
    writeMatrixToCSV("../outputDesiredJointsDemo.csv", desiredJointValuesMatrix);

    return 0;

}