#include <iostream>
#include <inverseKinematics.h>
#include <Robot.h>
#include "utils.h"
#include <cmath>
#include "cartesianTrajetory.h"
#include <boost/numeric/odeint.hpp>


using namespace ObstacleAvoidance;
using namespace std;
using namespace Eigen;
using namespace AndreiUtils;

/*std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>*/
tuple<MatrixXd,MatrixXd,MatrixXd> positionTrajectory(ConfigurationParameters const &trajConfig) {

    auto waypointsParams = trajConfig.get<std::vector<std::vector<double>>>("Waypoints");
    auto Waypoints = vectorMatrixToEigenMatrix(waypointsParams);
    Eigen::MatrixXd waypointVelocities = Eigen::MatrixXd::Zero(Waypoints.rows(), Waypoints.cols());
    Eigen::MatrixXd waypointAccelerations = Eigen::MatrixXd::Zero(Waypoints.rows(), Waypoints.cols());
    auto ts = trajConfig.get<double>("trajectorySampleTime");
    auto positionTrajectoryType = trajConfig.get<std::string>("positionTrajectoryType");
    auto WaypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double>>("waypointTimes"));

    std::shared_ptr<cartesianTrajetory> Trajectoryptr;
    Trajectoryptr = std::make_shared<cartesianTrajetory>();
    return Trajectoryptr->positionTrajectory(Waypoints,WaypointTimes,ts,positionTrajectoryType,waypointVelocities,waypointAccelerations);

}

tuple<MatrixXd,MatrixXd,MatrixXd> orientationTrajectory(ConfigurationParameters const &trajConfig) {


    auto orientationParams = trajConfig.get<vector<vector<double>>>("orientations");
    auto orientations = vectorMatrixToEigenMatrix(orientationParams);
    auto orientationTrajectoryType = trajConfig.get<std::string>("orientationTrajectoryType");
    auto WaypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double>>("waypointTimes"));
    auto ts = trajConfig.get<double>("trajectorySampleTime");
    std::shared_ptr<cartesianTrajetory> Trajectoryptr;
    Trajectoryptr = std::make_shared<cartesianTrajetory>();
    return Trajectoryptr->orientationTrajectory(orientations, WaypointTimes, ts, orientationTrajectoryType);
}


int main() {
    ConfigurationParameters Config(static_cast<std::string const &>("../config/configurationParameters.json"),
                                   static_cast<std::string const &>("Configuration"));
    auto trajConfig = Config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematics");

    auto [desiredPositionTCP,desiredVelocityTCP,desiredAccelerationTCP] = positionTrajectory(trajConfig);
    cout<<"desiredPositionTCP: /n:"<<desiredPositionTCP.col(15488)<<endl;
    auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel ] = orientationTrajectory(trajConfig);
    VectorXd wayPointTimes = stdVectorToEigenVector(trajConfig.get<vector<double>>("waypointTimes"));
    double ts = trajConfig.get<double>("trajectorySampleTime");
    VectorXd trajTimes = generateSequence(wayPointTimes(0),ts,wayPointTimes(last));
    cout<< "Number of Samples: \n"<<trajTimes.size()<<endl;
    AndreiUtils::Posed baseFrame = DualQuaternion<double>::identity();
    auto robot = Robot("../config/robotModellingParameters.json", "RobotModelling", "PandaRobot", baseFrame);
    Eigen::VectorXd Homejointpositions(robot.getNumberJoints());
    Homejointpositions << 0, 0, 0, -M_PI / 2, 0, M_PI / 2, M_PI / 4;
    auto trafo = robot.forwardKinematics(Homejointpositions, 7);
    int numberJoints = robot.getNumberJoints();
    MatrixXd actualJointValuesMatrix = Eigen::MatrixXd ::Zero(numberJoints,trajTimes.size());
    MatrixXd actualJointVelocityMatrix = Eigen::MatrixXd ::Zero(numberJoints,trajTimes.size());

    MatrixXd desiredJointValuesMatrix = Eigen::MatrixXd ::Zero(numberJoints,trajTimes.size());
    MatrixXd desiredJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints,trajTimes.size());

    auto diagValuesKp = stdVectorToEigenVector(trajConfig.getJson("inverseKinematics").at("positionKp").get<vector<double>>());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKp(diagValuesKp);
    Eigen::MatrixXd Kp = diagMatrixKp.toDenseMatrix();

    auto diagValuesKo = stdVectorToEigenVector(trajConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double>>());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();

    //get the starting Joint position from the sensor value of the robot or manually
    actualJointValuesMatrix(all,0) = Homejointpositions;
    auto jointVelMinValues = robot.getJoints()->velMinValues;
    auto jointVelMaxValues = robot.getJoints()->velMaxValues;
    auto jointMinValues = robot.getJoints()->minValues;
    auto jointMaxValues = robot.getJoints()->maxValues;

    inverseKinematics ik(inverseKinematicsConfig);





    for (int i = 0;i<trajTimes.size(),i++;)
    {
        auto transformTcpToBase = robot.forwardKinematics(actualJointValuesMatrix(all,i),numberJoints);
        auto positionTcpCurrent = transformTcpToBase(seq(0,2),3);
        auto xdEffective = desiredVelocityTCP(all,i) + (Kp *(desiredPositionTCP(all,i)-positionTcpCurrent) );

        auto angularVelocityEffective = desiredAngularVelocityTCP(all,i) + (Ko * computeOrientationError(transformTcpToBase,desiredQuaternionsTCP(all,i)));

        auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(all,i));
        Eigen::VectorXd poseVelocityEffective(6);
        poseVelocityEffective(seq(0,2)) = xdEffective;
        poseVelocityEffective(seq(3,5)) = angularVelocityEffective;

//        ik.ikWithConstraints()


    }
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