//
// Created by shobhit on 20.01.25.
//


#include <iostream>
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

void printLinkSegments(std::vector<LinkSegment> const &LinkSegments){

    for (auto &Link:LinkSegments) {
        cout << "av0: " << Link.aSegmentV0.transpose() << "\t" << "av1: " << Link.aSegmentV1.transpose() << "\tdv0: " << Link.dSegmentV0.transpose()
             << "\tdv1: " << Link.dSegmentV1.transpose() << endl;
        if (Link.toolV0.size() != 0) {
            cout << "ToolV0: " << Link.toolV0.transpose() << "\tToolV1: " << Link.toolV1.transpose() << endl;

        }
    }
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>
criticalPointInformation( ObstacleAvoidance::Robot const &robot,Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                                int const &indexLink) {
    Eigen::Isometry3d transformation;
    Eigen::MatrixXd jacobiCriticalPoint;
    transformation.matrix() = robot.fkmCartesian(jointAngles, indexLink);
    Eigen::Isometry3d inverseTransformation = transformation.inverse();
    Vector3d criticalPointRelativeToLink = inverseTransformation * closestPointLink;
    auto relativeTransformationCriticalPoint = trvec2tform(criticalPointRelativeToLink);
    auto jacobiCritical = robot.
            jacobianCartesianOnLink(jointAngles, indexLink, relativeTransformationCriticalPoint);
    return std::make_tuple(relativeTransformationCriticalPoint, jacobiCritical);
}

int main() {
    //auto matrixCSV = readMatrixFromCSV("../outputDesiredJointsDemoPartTwo.csv");
    ConfigurationParameters Config(static_cast<std::string const &>("../config/configurationParameters.json"),
                                   static_cast<std::string const &>("Configuration"));



    auto trajConfig = Config.getSubConfig("Trajectory");
    auto inverseKinematicsConfig = Config.getSubConfig("inverseKinematicsWithOA");

    auto [desiredPositionTCP, desiredVelocityTCP, desiredAccelerationTCP] =
            CartesianTrajectory::positionTrajectory(trajConfig);

//      cout << "desiredPositionTCP at 15.487 s: \n" << desiredPositionTCP.col(15488) << endl;
//      cout << " desiredVelocityTCP at 15.469 s: \n" << desiredVelocityTCP.col(15469) << endl;
    auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
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
    //HomeJointPosition = matrixCSV(all,last);
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


    auto[finalLinkTesting, linkSegmentsTesting] = robot.createLinkSegments(HomeJointPosition);
    cout<<"finalLink center: \n"<<finalLinkTesting.center<<endl;
    cout<<"finalLink dimensions: \n"<<finalLinkTesting.dimensions<<endl;
    cout<<"finalLink orientations: \n"<<finalLinkTesting.orientation<<endl;

    cout<<"norm:" << (linkSegmentsTesting[2].dSegmentV1 - linkSegmentsTesting[4].dSegmentV0).norm()<<endl  ;
    cout<<"norm:" << (linkSegmentsTesting[2].dSegmentV1 - linkSegmentsTesting[4].aSegmentV1).norm()<<endl  ;
    cout<<"norm:" << (linkSegmentsTesting[4].dSegmentV1 - robot.fkmCartesianTCP(HomeJointPosition)(seq(0,2),3)).norm()<<endl  ;

    VectorXd sphereDimension(1);
    sphereDimension(0)  = 0.08;
    auto sphereObstacle = Obstacle("Sphere",{0.5545,0.20,0.5211},sphereDimension,{1, 0, 0, 0});
    auto [distance, closestPointObstacleTesting, closestPointLinkTesting]= sphereObstacle.calculateDistanceRobotLinkObstacle(linkSegmentsTesting[6].aSegmentV0,linkSegmentsTesting[6].aSegmentV1,0.1,0.1,true,false);
    cout<<"distance testing: "<<distance<<endl;
    cout<<"closestPointObstacle testing: "<<closestPointObstacleTesting<<endl;
    cout<<"closestPointLinkTesting testing: "<<closestPointLinkTesting<<endl;
    auto[relativeTransformation, relativeJacobi] = criticalPointInformation(robot,HomeJointPosition,closestPointLinkTesting,6);
    cout <<"relativeTransformation point : \n"<< relativeTransformation.block<3,1>(0,3)<<endl;
    cout<<"relativeJacobi: \n" <<relativeJacobi<<endl;
    cout<<"avo: \n"<<linkSegmentsTesting[6].aSegmentV0<<"av1: \n"<<linkSegmentsTesting[6].aSegmentV1<<endl;
    cout<<"avo: \n"<<linkSegmentsTesting[7].aSegmentV0<<"av1: \n"<<linkSegmentsTesting[7].aSegmentV1<<endl;

    OptimizationBasedIKWithOA ik(inverseKinematicsConfig);
    auto jointVelocityWeight = stdVectorToEigenVector(inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
            "jointVelocityWeight").get<std::vector<double>>());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();



    //cout<<"ForwardKinematics: \n"<<robot.fkmCartesianTCP(Homejointpositions)(seq(0,2),2)<<endl;
    auto LinkSegments = robot.createLinkSegments(HomeJointPosition);

    Vector3d center1 = {0.5545, 0.20, 0.5211};
    VectorXd dimensions1(1);
    dimensions1(0) = 0.04;
    auto obstacle1 = Obstacle("Sphere", center1, dimensions1,{1,0,0,0});

    Vector3d center2 = {0.5545, 0.20, 0.3211};

    VectorXd dimensions2(1);
    dimensions2(0) = 0.05;
    cout << dimensions2 << endl;
    auto obstacle2 = Obstacle("Sphere", center2, dimensions2,{1,0,0,0});


    std::vector<Obstacle> obstaclesArray;
    obstaclesArray.push_back(obstacle1);
    obstaclesArray.push_back(obstacle2);
    auto obstaclesMap = conversionObstaclesVectorToMap(obstaclesArray);

    VectorXd jointVelocityObstacleAvoidance = VectorXd::Zero(numberJoints);


    for (int i = 0; i < trajTimes.size() - 1; i++) {
        auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(all, i));
        //cout<<"transformTcpToBase: \n" <<transformTcpToBase<< endl;
        VectorXd positionTcpCurrent = transformTcpToBase(seq(0, 2), 3);

        auto xdEffective = desiredVelocityTCP(all, i) + (Kp * (desiredPositionTCP(all, i) - positionTcpCurrent));

        auto angularVelocityEffective =
                desiredAngularVelocityTCP(all, i) + (Ko * computeOrientationError(transformTcpToBase,
                                                                                  desiredQuaternionsTCP(all, i)));

        //cout<<"orientation error : \n"<<computeOrientationError(transformTcpToBase,desiredQuaternionsTCP(all, i))<<endl;
        //cout<<"actualJointValues: \n"<<actualJointValuesMatrix(all, i)<<endl;
        auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(all, i));
        Eigen::VectorXd poseVelocityEffective(6);
        poseVelocityEffective(seq(0, 2)) = xdEffective;
        poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
        //cout<<"poseVelocityEffective: \n"<<poseVelocityEffective<<endl;

        auto [jg, bg, minDistance] = robot.obstacleAvoidanceEquation(obstaclesMap, actualJointValuesMatrix(all, i),
                                                                     jointVelocityObstacleAvoidance);



            cout<<"jg at " +std::to_string(i)+ ": \n"<<jg<<endl;

          cout<<"bg at " +std::to_string(i)+ ": \n"<<bg<<endl;

          cout<< "minimum distance at " + std::to_string(i) + ": "<<minDistance<<endl;



        /*robot.obstacleAvoidanceEquation()*/

        auto [optimalJointVelocity, ExitFlag] = ik.inverseKinematicsWithOA(actualJointValuesMatrix(all, i),
                                                                           JacobiMatrix,
                                                                           poseVelocityEffective,
                                                                           jointMinValues, jointMaxValues,
                                                                           jointVelMinValues,
                                                                           jointVelMaxValues, jg, bg,
                                                                           jointVelocityWeightMatrix);

        //cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;


        desiredJointVelocityMatrix(all, i) = optimalJointVelocity;
        jointVelocityObstacleAvoidance = optimalJointVelocity;
        if(jg.size()!=0) {
            auto product = jg * jointVelocityObstacleAvoidance;
            cout << "jg * jointVelocityObstacle Avoidance: \n" << product << endl;
        }
        if (ExitFlag < 0) {
            cout << i << endl;
            cout << "ExitFlag: " << ExitFlag << endl;
            cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
            break;
        }




        if (minDistance < 0.01){
            cout<<"minDistance has become less than 0.01 at " +std::to_string(i)+ ": "<<minDistance<<endl;
            break;

        }

        double tStart = trajTimes(i);
        double tEnd = trajTimes(i + 1);
        double timespan[2] = {tStart, tEnd};
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
        desiredJointValuesMatrix(all, i + 1) = integrateConstantRungeKutta(desiredJointVelocityCurrent, timespan,
                                                                           desiredJointPosition);

        actualJointValuesMatrix(all, i + 1) = addRandomNoisetoJointsSignal(desiredJointValuesMatrix(all, i + 1), -0.005,
                                                                           0.005);


        //cout<<"desired Joint Value: \n"<< desiredJointValuesMatrix(all, i+1)<<endl;

    }

    cout << "actualJointValues:  \n" << actualJointValuesMatrix(all, 1458) << endl;
    writeMatrixToCSV("../outputDesiredJoints.csv", desiredJointValuesMatrix);
    writeMatrixToCSV("../outputDesiredJointVelocity.csv", desiredJointVelocityMatrix);


    return 0;

}
