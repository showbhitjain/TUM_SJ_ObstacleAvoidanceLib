//
// Created by shobhit on 20.01.25.
//


#include <iostream>
#include <OptimizationBasedIKWithOA.h>
#include <Robot.h>
#include <utils.h>
#include <cmath>
#include <CartesianTrajectory.h>
#include <ObstacleAvoidanceUtils.h>
#include <utilsJointValuesMatlab.h>
#include <fstream>
#include <Eigen/Dense>

using namespace ObstacleAvoidance;
using namespace std;
using namespace Eigen;
using namespace AndreiUtils;

int main(){

        ConfigurationParameters Config(static_cast<std::string const &>("../config/configurationParameters.json"),
        static_cast<std::string const &>("Configuration"));
        auto trajConfig = Config.getSubConfig("Trajectory");
        auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematicsWithOA");

        auto[desiredPositionTCP, desiredVelocityTCP, desiredAccelerationTCP] =
        CartesianTrajectory::positionTrajectory(trajConfig);

//      cout << "desiredPositionTCP at 15.487 s: \n" << desiredPositionTCP.col(15488) << endl;
//      cout << " desiredVelocityTCP at 15.469 s: \n" << desiredVelocityTCP.col(15469) << endl;
        auto[desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
        CartesianTrajectory::orientationTrajectory(trajConfig);
//      cout << "desiredQuaternion TCP at 15.488 s: /n:" << desiredQuaternionsTCP.col(15488) << endl;

        VectorXd wayPointTimes = stdVectorToEigenVector(trajConfig.get<vector<double> >("waypointTimes"));
        auto ts = trajConfig.get<double>("trajectorySampleTime");
        VectorXd trajTimes = generateSequence(wayPointTimes(0), ts, wayPointTimes(last));
        //cout << "Number of Samples: " << trajTimes.size() << endl;
        auto robot = ObstacleAvoidance::Robot("../config/robotModellingParameters.json", "RobotModelling",
        "PandaRobot");
        int numberJoints = static_cast<int>(robot.getNumberJoints());

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

        Eigen::VectorXd HomeJointPosition(numberJoints);
        HomeJointPosition << 0, 0, 0, -M_PI / 2, 0, M_PI / 2, M_PI / 4;
        std::array<double, 7> firstJointPosition{};
        std::copy_n(HomeJointPosition.data(), 7, firstJointPosition.begin());
//      cout << "HomeJointPosition:" << HomeJointPosition << endl;
        //get the starting Joint position from the sensor value of the robot or manually
        actualJointValuesMatrix(all, 0) = HomeJointPosition;
        desiredJointValuesMatrix(all, 0) = HomeJointPosition;
        auto jointVelMinValues = robot.getJoints()->velMinValues;
        auto jointVelMaxValues = robot.getJoints()->velMaxValues;
        auto jointMinValues = robot.getJoints()->minValues;
        auto jointMaxValues = robot.getJoints()->maxValues;
        cout<<"jointMaxValues"<<jointMaxValues<<endl;
        OptimizationBasedIKWithOA ik(inverseKinematicsConfig);
        auto jointVelocityWeight = stdVectorToEigenVector(inverseKinematicsConfig.getJson("ObjectiveConstraints").at("jointVelocityWeight").get<std::vector<double>>());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd  jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();

        //using Stepper = runge_kutta_dopri5<State, double, State, double>;

        //auto controlled_stepper = make_controlled<Stepper>(1e-6, 1e-6);
        VectorXd radius(numberJoints);
        radius = Eigen::VectorXd::Constant(numberJoints+1,0.1);

        //cout<<"ForwardKinematics: \n"<<robot.fkmCartesianTCP(Homejointpositions)(seq(0,2),2)<<endl;
        auto LinkSegments = robot.createLineSegments(HomeJointPosition);

        Vector3d center1 = {0.5545, 0.20, 0.3};
        VectorXd dimensions1(1);
        dimensions1(0) = 0.05;
        auto obstacle1 = Obstacles("Sphere",center1,dimensions1);

        Vector3d center2 = {0.5545, 0.30, 0.6211};
        VectorXd dimensions2(1);
        dimensions2(0) = 0.04;
        auto obstacle2 = Obstacles("Sphere",center2,dimensions2);

        std::vector<Obstacles> obstaclesArray= {obstacle1,obstacle2};
        auto obstaclesMap = conversionObstaclesVectorToMap(obstaclesArray);

        VectorXd jointVelocityObstacleAvoidance = VectorXd::Zero(numberJoints);


    for (int i = 0; i < trajTimes.size()-1; i++) {
            auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, i));
            //cout<<"transformTcpToBase: \n" <<transformTcpToBase<< endl;
            VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

            auto xdEffective = desiredVelocityTCP(all, i) + (Kp * (desiredPositionTCP(all, i) - positionTcpCurrent));

            auto angularVelocityEffective = desiredAngularVelocityTCP(all, i) + (Ko * computeOrientationError(transformTcpToBase,
                                                                                                              desiredQuaternionsTCP(all, i)));

            //cout<<"orientation error : \n"<<computeOrientationError(transformTcpToBase,desiredQuaternionsTCP(all, i))<<endl;
            //cout<<"actualJointValues: \n"<<actualJointValuesMatrix(all, i)<<endl;
            auto JacobiMatrix = robot.jacobianCartesianTCP(desiredJointValuesMatrix(all, i));
            Eigen::VectorXd poseVelocityEffective(6);
            poseVelocityEffective(seq(0, 2)) = xdEffective;
            poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
            cout<<"poseVelocityEffective: \n"<<poseVelocityEffective<<endl;

            auto [jg,bg,minDistance] = robot.obstacleAvoidanceEquation(obstaclesMap,actualJointValuesMatrix(all,i),jointVelocityObstacleAvoidance);


            /*robot.obstacleAvoidanceEquation()*/

            auto [optimalJointVelocity, ExitFlag] = ik.inverseKinematicsWithOA(desiredJointValuesMatrix(all, i), JacobiMatrix,
                                                                         poseVelocityEffective,
                                                                         jointMinValues, jointMaxValues, jointVelMinValues,
                                                                         jointVelMaxValues,jg,bg,jointVelocityWeightMatrix);

            cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;
            if (ExitFlag<0) {
                cout<< i<<endl;
                cout << "ExitFlag: " << ExitFlag << endl;
                break;
            }

            desiredJointVelocityMatrix(all, i) = optimalJointVelocity;
            jointVelocityObstacleAvoidance = optimalJointVelocity;
            double tStart = trajTimes(i);
            double tEnd = trajTimes(i + 1);
            double timespan[2] = {tStart,tEnd};
//        double dt = (tEnd - tStart) / 10;
            VectorXd desiredJointPosition = (desiredJointValuesMatrix.col(i));
            VectorXd desiredJointVelocityCurrent = (desiredJointVelocityMatrix.col(i));

            /*integrate_adaptive(
                    controlled_stepper,
                    [&desiredJointVelocityCurrent](const State &x, State &dxdt, double t) {
                        jointDynamics(x, dxdt, t, desiredJointVelocityCurrent);
                    },
                    desiredJointPosition, tStart, tEnd, 0.01  // Provide an initial step size estimate
            );*/
            desiredJointValuesMatrix(all, i+1)  = integrateConstantRungeKutta(desiredJointVelocityCurrent,timespan,desiredJointPosition);


            actualJointValuesMatrix(all, i+1) = addRandomNoisetoJointsSignal(desiredJointValuesMatrix(all, i+1),-0.005,0.005);


            cout<<"desired Joint Value: \n"<< desiredJointValuesMatrix(all, i+1)<<endl;

    }

        cout << "actualJointValues:  \n" << actualJointValuesMatrix(all, 1458) << endl;
        writeMatrixToCSV("../outputDesiredJoints.csv",desiredJointValuesMatrix);
        writeMatrixToCSV("../outputDesiredJointVelocity.csv",desiredJointVelocityMatrix);


        return 0;

}
