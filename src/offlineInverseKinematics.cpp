#include <iostream>
#include <inverseKinematics.h>
#include <Robot.h>
#include "utils.h"
#include <cmath>
#include <CartesianTrajectory.h>
#include <boost/numeric/odeint.hpp>
//#include <AndreiUtils/utilsGeometry.h>
#include <utilsJointValuesMatlab.h>
#include <fstream>
#include <Eigen/Dense>
using namespace ObstacleAvoidance;
using namespace std;
using namespace Eigen;
using namespace AndreiUtils;
using namespace boost::numeric::odeint;


/*using State = Eigen::VectorXd; // Use Eigen::VectorXd as the state

void jointDynamics(const State &x, State &dxdt, const double *//* t *//*, const State &velocity) {
    dxdt = velocity;  // Constant dynamics
}*/

/*std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>*/
void printLinkSegments(std::vector<LinkSegment> const &LinkSegments){

    for (auto &Link:LinkSegments) {
        cout << "av0: " << Link.aSegmentV0.transpose() << "\t" << "av1: " << Link.aSegmentV1.transpose() << "\tdv0: " << Link.dSegmentV0.transpose()
             << "\tdv1: " << Link.dSegmentV1.transpose() << endl;
        if (Link.toolV0.size() != 0) {
            cout << "ToolV0: " << Link.toolV0.transpose() << "\tToolV1: " << Link.toolV1.transpose() << endl;

        }
    }
}

tuple<MatrixXd, MatrixXd, MatrixXd> positionTrajectory(ConfigurationParameters const &trajConfig) {

    auto waypointsParams = trajConfig.get<std::vector<std::vector<double>>>("Waypoints");
    auto Waypoints = vectorMatrixToEigenMatrix(waypointsParams);
    Eigen::MatrixXd waypointVelocities = Eigen::MatrixXd::Zero(Waypoints.rows(), Waypoints.cols());
    Eigen::MatrixXd waypointAccelerations = Eigen::MatrixXd::Zero(Waypoints.rows(), Waypoints.cols());
    auto ts = trajConfig.get<double>("trajectorySampleTime");
    auto positionTrajectoryType = trajConfig.get<std::string>("positionTrajectoryType");
    auto WaypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double>>("waypointTimes"));


    return CartesianTrajectory::positionTrajectory(Waypoints, WaypointTimes, ts, positionTrajectoryType, waypointVelocities,
                                             waypointAccelerations);

}

tuple<MatrixXd, MatrixXd, MatrixXd> orientationTrajectory(ConfigurationParameters const &trajConfig) {


    auto orientationParams = trajConfig.get<vector<vector<double>>>("orientations");
    auto orientations = vectorMatrixToEigenMatrix(orientationParams);
    auto orientationTrajectoryType = trajConfig.get<std::string>("orientationTrajectoryType");
    auto WaypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double>>("waypointTimes"));
    auto ts = trajConfig.get<double>("trajectorySampleTime");

    return CartesianTrajectory::orientationTrajectory(orientations, WaypointTimes, ts, orientationTrajectoryType);
}


int main() {

    Eigen::Vector3d trvec(0, 0, 0.103399);
    Eigen::Vector3d eul(0, 0, -0.785398);
    auto tform = trvec2tform(trvec) * convertEulerToTransform(eul);
    std::cout << "The trafo_matrix between endeffector and Tcp is: \n" << tform << std::endl;


    ConfigurationParameters Config(static_cast<std::string const &>("../config/configurationParameters.json"),
                                   static_cast<std::string const &>("Configuration"));
    auto trajConfig = Config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematics");

    auto [desiredPositionTCP, desiredVelocityTCP, desiredAccelerationTCP] = positionTrajectory(trajConfig);
    cout << "desiredPositionTCP: /n:" << desiredPositionTCP.col(15488) << endl;
    auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] = orientationTrajectory(trajConfig);
    VectorXd wayPointTimes = stdVectorToEigenVector(trajConfig.get<vector<double>>("waypointTimes"));
    auto ts = trajConfig.get<double>("trajectorySampleTime");
    VectorXd trajTimes = generateSequence(wayPointTimes(0), ts, wayPointTimes(last));
    cout << "Number of Samples: " << trajTimes.size() << endl;
    AndreiUtils::Posed baseFrame = DualQuaternion<double>::identity();
    auto robot = Robot("../config/robotModellingParameters.json", "RobotModelling", "PandaRobot");
    Eigen::VectorXd Homejointpositions(robot.getNumberJoints());
    Homejointpositions << 0, 0, 0, -M_PI / 2, 0, M_PI / 2, M_PI / 4;


    auto trafo = robot.fkmCartesian(Homejointpositions, 7);
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
            trajConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double>>());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();

    //get the starting Joint position from the sensor value of the robot or manually
    actualJointValuesMatrix(all, 0) = Homejointpositions;
    desiredJointValuesMatrix(all, 0) = Homejointpositions;
    auto jointVelMinValues = robot.getJoints()->velMinValues;
    auto jointVelMaxValues = robot.getJoints()->velMaxValues;
    auto jointMinValues = robot.getJoints()->minValues;
    auto jointMaxValues = robot.getJoints()->maxValues;
    cout<<"jointMaxValues"<<jointMaxValues<<endl;
    inverseKinematics ik(inverseKinematicsConfig);
//    using Stepper = runge_kutta_dopri5<State, double, State, double>;
//    auto controlled_stepper = make_controlled<Stepper>(1e-6, 1e-6);
    VectorXd radius(numberJoints);
    radius = Eigen::VectorXd::Constant(numberJoints+1,0.1);
    cout<<"ForwardKinematics: \n"<<robot.fkmCartesianTCP(Homejointpositions)(seq(0,2),2)<<endl;
    auto LinkSegments = robot.createLineSegments(Homejointpositions);

    for (int i = 0; i < trajTimes.size()-1; i++) {
        auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, i));
        cout<<"transformTcpToBase: \n" <<transformTcpToBase<< endl;
        VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

        auto xdEffective = desiredVelocityTCP(all, i) + (Kp * (desiredPositionTCP(all, i) - positionTcpCurrent));

        auto angularVelocityEffective = desiredAngularVelocityTCP(all, i) + (Ko * computeOrientationError(transformTcpToBase,
                                                                                                     desiredQuaternionsTCP(all, i)));

        cout<<"orientation error : \n"<<computeOrientationError(transformTcpToBase,desiredQuaternionsTCP(all, i))<<endl;
        cout<<"actualJointValues: \n"<<actualJointValuesMatrix(all, i)<<endl;
        auto JacobiMatrix = robot.jacobianCartesianTCP(desiredJointValuesMatrix(all, i));
        Eigen::VectorXd poseVelocityEffective(6);
        poseVelocityEffective(seq(0, 2)) = xdEffective;
        poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
        cout<<"poseVelocityEffective: \n"<<poseVelocityEffective<<endl;

        auto [optimalJointVelocity, ExitFlag] = ik.ikWithConstraints(desiredJointValuesMatrix(all, i), JacobiMatrix,
                                                                     poseVelocityEffective,
                                                                     jointMinValues, jointMaxValues, jointVelMinValues,
                                                                     jointVelMaxValues);
        cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;
        if (ExitFlag<0) {
            cout<< i<<endl;
            cout << "ExitFlag: " << ExitFlag << endl;
            break;
        }

        desiredJointVelocityMatrix(all, i) = optimalJointVelocity;

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

/*
std::shared_ptr<Eigen::Vector3d> x = std::make_shared<Eigen::Vector3d>();
(*x) << 0, 1, 2;
cout << x << endl;
cout << x->transpose() << endl;
auto y = x;  // copy
cout << x << endl;
auto z = std::move(x); // move
auto w = x;  // copy after move
cout << x << " " << z << endl;
cout << y->transpose() << "; " << z->transpose() << endl;
cout << x << " " << w <<y<< endl;*/
/*
std::cout << "The trafo_matrix is: \n" << trafo << std::endl;
auto Jacobi_matrix = robot.jacobianCartesian(Homejointpositions, 7);
std::cout << "The Jacobi_matrix is: \n" << Jacobi_matrix << std::endl;
Eigen::Vector3d trvec(0, 0, 0.103399);
Eigen::Vector3d eul(0, 0, -M_PI_4);
auto tform = trvec2tform(trvec) * convertEulerToTransform(eul);
std::cout << "The trafo_matrix between endeffector and Tcp is: \n" << tform << std::endl;
auto JACOBI = robot.jacobianCartesianOnLink(Homejointpositions, 7, tform);
std::cout << "The Jacobian matrix on link is: \n" << JACOBI << std::endl;

auto Joints = robot.getJoints();


auto Joints_max_values = Joints->maxValues;
cout<< "Maximum Joint Values: \n" << Joints_max_values<<endl;

cout<< "Jacobian at TCP: \n" <<robot.jacobianCartesianTCP(Homejointpositions)<<endl;*/
/*  config.weight1 = 0.1;
    config.useObjective1 = true;
    config.weight2 = 0.2 ;
    config.useObjective2 = false;
    config.weight3 = 0;
    config.useObjective3 = false;
    config.weight4 = 0.1;
    config.useObjective4 = true ;
    config.weight5 = 0.7;
    config.useObjective5 = true ;
    config.weight6 = 0.5;
    config.useObjective6 = true;*/


/*
std::ofstream outFile("output.xyz");
cout << "actualJointValues:  \n" << actualJointValuesMatrix(all, seq(0,50)) << endl;
// Check if the file is opened correctly
if (!outFile) {
std::cout << "Unable to open file";
exit(1); // terminate with error
}

// Iterate through the matrix and print each element to outFile
for (int i = 0; i < desiredJointValuesMatrix.rows(); i++)
{
for (int j = 0; j < desiredJointValuesMatrix.cols(); j++)
{
outFile << desiredJointValuesMatrix(i,j);

// Add space if not the last column.
if (j != desiredJointValuesMatrix.cols() - 1)
{
outFile << " ";
}
}

// Add newline character if not the last row
if (i != desiredJointValuesMatrix.rows() - 1)
{
outFile << "\n";
}
}
*/
