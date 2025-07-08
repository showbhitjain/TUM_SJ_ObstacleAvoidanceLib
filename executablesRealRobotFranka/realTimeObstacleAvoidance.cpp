//
// Created by shobhit on 20.02.25.
//

#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <ConceptLibrary/valueDomains/Instance.hpp>
#include <cmath>
#include <Eigen/Dense>
#include <iostream>
#include <thread>
#include <AndreiUtils/utilsThread.h>
#include <AndreiUtils/utilsTime.h>
#include <opencv2/opencv.hpp>

#include <TUM_SJ_ObstacleAvoidanceLib/Robot.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <TUM_SJ_ObstacleAvoidanceLib/CartesianTrajectory.h>
#include <TUM_SJ_ObstacleAvoidanceLib/utilsJointValuesMatlab.h>
#include <TUM_SJ_ObstacleAvoidanceLib/ControllerFranka.h>
#include <TaskRepresentationLib/utils.h>
#include <TUM_SJ_ObstacleAvoidanceLib/OptimizationBasedIKWithOA.h>
#include<TUM_SJ_ObstacleAvoidanceLib/examples_common.h>

#include "VisualInput.h"

using namespace AndreiUtils;
using namespace ConceptLibrary;
using namespace cv;
using namespace franka;
using namespace std;


class ObstacleState {
protected:
    ObstacleAvoidance::Obstacle obstacleEstimate;
    ObstacleAvoidance::Obstacle obstacleCloned;
    double detectionTime;

public:
    ObstacleState(ObstacleAvoidance::Obstacle const &obstacle, double time): obstacleEstimate(obstacle),
                                                                             obstacleCloned(obstacle),
                                                                             detectionTime(time) {
    }

    void updateInformation(ObstacleAvoidance::Obstacle obstacle, double time) {
        // getEstimate
        //do some calculations i.e first call getEstimate and then use this measurement to update information (call getEstimate first)
        this->obstacleEstimate.center = obstacle.center;
        this->obstacleCloned.center = obstacle.center;

        this->detectionTime = time;
    }

    ObstacleAvoidance::Obstacle getEstimate(bool predictWithKalman, double time) {
        //define a predictionModel
        if (predictWithKalman) {
            auto A = Eigen::Matrix3d::Identity();
            this->obstacleEstimate.center = A * (time - this->detectionTime) * this->obstacleEstimate.center;
            this->detectionTime = time;
        }
        return this->obstacleEstimate;
    }
};

class ObstacleCollection {
public:
    using ObstacleMap = std::map<std::string, ObstacleAvoidance::Obstacle>;
    using ObstacleStateMap = std::map<std::string, ObstacleState>;

    // this function gets called at ~33ms
    void set(ObstacleMap newObstacles, double detectionTime, bool discardUnseenValues = false) {
        this->m.lock();
        for (auto &newObstacle: newObstacles) {
            ObstacleState *existingObstacleState;
            if (AndreiUtils::mapGetIfContains(this->obstacles, newObstacle.first, existingObstacleState)) {
                // TODO: when updating the information; correct the state of the estimate!
                existingObstacleState->updateInformation(std::move(newObstacle.second), detectionTime);
            } else {
                AndreiUtils::mapEmplace(this->obstacles, newObstacle.first, std::move(newObstacle.second),
                                        detectionTime);
            }
        }
        this->m.unlock();
    }

    // gives the "estimated" state of the obstacles (incl. Kalman filter prediction (+/- update)
    // this function gets called at 1ms
    [[nodiscard]] ObstacleMap get(double predictionTime) {
        this->m.lock();
        // here the estimate of the obstacle data (center, orientation, axis, etc.) is requested
        ObstacleMap o;

        for (auto &obstacle: this->obstacles) {
            // if predictWithKalman == false, "return *this;" in the getEstimate method
            // otherwise, run the KalmanFilter prediction step (with the corresponding deltaT!) and return the prediction in getEstimate
            AndreiUtils::mapEmplace(o, obstacle.first, obstacle.second.getEstimate(false, predictionTime));
        }
        this->m.unlock();
        return o;
    }

protected:
    ObstacleStateMap obstacles;
    std::mutex m;
};

ObstacleCollection::ObstacleMap convertToObstacleMap(
        std::map<string, VisualInput::ObjectDetectionData> const &detectionMap,
        AndreiUtils::Posed const &worldInRobotFrame) {
    ObstacleCollection::ObstacleMap obstaclesMap;
    for (auto const &object: detectionMap) {
        Instance<ConceptList<ObjectConcept> > instance(object.first);
        instance.setGlobalPose(worldInRobotFrame * object.second.detectionPose);
        instance.shape->setEnvironmentPose(worldInRobotFrame * object.second.detectionPose);
        if (AndreiUtils::pointerInstanceOf<ConceptLibrary::BoxShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::BoxShape> boxShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::BoxShape>(instance.shape);
            auto boxObstacle = ObstacleAvoidance::Obstacle(*boxShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, boxObstacle);
        } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::CylinderShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::CylinderShape> cylinderShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::CylinderShape>(instance.shape);
            auto cylinderObstacle = ObstacleAvoidance::Obstacle(*cylinderShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, cylinderObstacle);
        } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::PlaneWidthShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::PlaneWidthShape> planeWidthShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::PlaneWidthShape>(instance.shape);
            auto planeObstacle = ObstacleAvoidance::Obstacle(*planeWidthShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, planeObstacle);
        } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::SphereShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::SphereShape> sphereShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::SphereShape>(instance.shape);
            auto sphereObstacle = ObstacleAvoidance::Obstacle(*sphereShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, sphereObstacle);
        } else {
            std::cerr << "Error: Shape type not recognized for object: " << instance.instanceId.s << std::endl;
        }
    }

    return obstaclesMap;
}

void processObstaclesFromVisualData(
        std::atomic<bool> &exitFlag, ObstacleCollection &obstacles, nlohmann::json const &arguments,
        AndreiUtils::Posed const &worldInRobotFrame, bool verbose = false) {
    auto visualInputConfig = arguments.at("VisualInput123");
    VisualInput input(ConfigurationParameters(visualInputConfig, "VisualInput"));
    cout << "Starting while loop!" << endl;
    while (!exitFlag.load() && input.processInput()) {
        cout << "Processed camera input!" << endl;
        auto cameraPose = input.getCameraPose();
        if (cameraPose == AndreiUtils::Posed::one) {
            break;
        }
        if (verbose) {
            cout << "Detection started" << endl;
            cout << "Camera is at pose " << input.getCameraPose().toString() << " relative to the origin" << endl;
        }
        cv::imshow("Color Image", input.getVisualColor());
        //cv::imshow("Depth Image", input.getVisualDepth());
        int key = cv::waitKey(1);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }

        obstacles.set(convertToObstacleMap(input.getObjectInstanceDetections(), worldInRobotFrame), input.getTime());
        //obstacles.set({}, input.getTime());
    }
    cout << "Finishing vision thread!" << endl;
    exitFlag.store(true);
}

std::function<franka::Torques(franka::RobotState const &robotState, franka::Duration period)>
getRobotControlFunction(franka::Model const &model, FILE *&fp, ObstacleAvoidance::Robot &robot,
                        ObstacleCollection &obstacles,
                        Eigen::MatrixXd &actualJointValuesMatrix,
                        Eigen::MatrixXd &desiredJointValuesMatrix, Eigen::MatrixXd &desiredJointVelocityMatrix,
                        long &index, int const &firstWayPointIndex, Eigen::VectorXd &trajTimes,
                        Eigen::MatrixXd &desiredPositionTCP, Eigen::MatrixXd &desiredVelocityTCP,
                        Eigen::MatrixXd &desiredQuaternionsTCP, Eigen::MatrixXd &desiredAngularVelocityTCP,
                        Eigen::MatrixXd const &Kp, Eigen::MatrixXd const &Ko,
                        ObstacleAvoidance::OptimizationBasedIKWithOA &ik,
                        ObstacleAvoidance::ControllerFranka &controller,
                        Eigen::VectorXd const &jointMinValues, Eigen::VectorXd const &jointMaxValues,
                        Eigen::VectorXd const &jointVelMinValues, Eigen::VectorXd const &jointVelMaxValues,
                        ObstacleAvoidance::Vector7d &previousDesiredJointPositionUpdate,
                        ObstacleAvoidance::Vector7d &previousDesiredJointVelocityUpdate,
                        Eigen::MatrixXd const &jointVelocityWeightMatrix,
                        Eigen::VectorXd &jointVelocityObstacleAvoidance,
                        bool &useDynamicSlackObjectiveWeightPreTrajectory, double const &staticWeightSlack,
                        double const &constraintTolerance, int const &maxIterations, double const &optimalityTolerance,
                        double const &stepTolerance, Eigen::MatrixXd &actualPositionTCP, Eigen::MatrixXd &errorPositionTCP, Eigen::MatrixXd &distanceAllLinks,
                        Eigen::MatrixXd &minimumDistanceRobotObstacles) {
    return [&](franka::RobotState const &robotState, const franka::Duration period) -> franka::Torques {
        AndreiUtils::Timer t;
        auto coriolisData = model.coriolis(robotState);
        auto mass = model.mass(robotState);
        Eigen::Map<const Eigen::Matrix<double, 7, 7>> dynamicMassMatrix(mass.data());
        auto const &jointValuesData = robotState.q;
        auto const &jointVelocities = robotState.dq;

        auto const indexString = std::to_string(index);
        cout << AndreiUtils::convertChronoToStringWithSubseconds(AndreiUtils::now()) << endl;

        long lastPeriod(static_cast<long>(period.toMSec()));
        index += lastPeriod;
        /*
        fprintf(fp, "%s %ld:", "At timestep", index);
        fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf; ", coriolisData[0], coriolisData[1], coriolisData[2],
                coriolisData[3], coriolisData[4], coriolisData[5], coriolisData[6]);
        fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf; ", jointValuesData[0], jointValuesData[1], jointValuesData[2],
                jointValuesData[3], jointValuesData[4], jointValuesData[5], jointValuesData[6]);
        fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf\n", jointVelocities[0], jointVelocities[1], jointVelocities[2],
                jointVelocities[3], jointVelocities[4], jointVelocities[5], jointVelocities[6]);
        //*/
        cout << "At index " + indexString + " Initial time " << t.measure() << endl;
        t.reset();
        if (index < trajTimes.size()) {
            AndreiUtils::Timer ifTimer;
            Eigen::VectorXd jointValues = Eigen::Map<Eigen::Matrix<double, 7, 1> const>(robotState.q.data());
            actualJointValuesMatrix(Eigen::all, index) = jointValues;

            auto transformTcpToBase = robot.fkmCartesianTCP(jointValues);
            Eigen::VectorXd positionTcpCurrent = transformTcpToBase(Eigen::seq(0, 2), 3);
            actualPositionTCP(Eigen::all,index) = positionTcpCurrent;
            auto errorTCP = desiredPositionTCP(Eigen::all, index) - positionTcpCurrent;
            errorPositionTCP(Eigen::all, index) = errorTCP;
            auto xdEffective = desiredVelocityTCP(Eigen::all, index) + (
                    Kp * (errorTCP));


            //cout << "Before orientation error: " << ifTimer.measure() << endl;
            //ifTimer.reset();
            auto orientationError = ObstacleAvoidance::computeOrientationError(
                    transformTcpToBase, desiredQuaternionsTCP(Eigen::all, index));
            //cout << "Orientation error time: " << ifTimer.measure() << endl;
            //ifTimer.reset();

            auto angularVelocityEffective = desiredAngularVelocityTCP(Eigen::all, index) + (Ko * orientationError);

            auto JacobiMatrix = robot.jacobianCartesianTCP(jointValues);
            Eigen::VectorXd poseVelocityEffective(6);
            poseVelocityEffective(Eigen::seq(0, 2)) = xdEffective;
            poseVelocityEffective(Eigen::seq(3, 5)) = angularVelocityEffective;

            cout << "At index " + indexString + " and time passed Before obstacle avoidance equation: " <<
                 ifTimer.measure() << endl;
            ifTimer.reset();
            auto [jg, bg, minDistance] = robot.obstacleAvoidanceEquation(
                    obstacles.get(AndreiUtils::getTime(AndreiUtils::now(), "s")),
                    actualJointValuesMatrix(Eigen::all, index),
                    jointVelocityObstacleAvoidance);
            cout << "At index " + indexString + " and time taken for and passed After obstacle avoidance equation: "
                 << ifTimer.measure() << endl;
            ifTimer.reset();
            //store distance results
            distanceAllLinks(Eigen::all, index) = robot.minimumDistanceAllLinks();
            minimumDistanceRobotObstacles(0,index) = minDistance;
            cout << "At index " + indexString + " and time taken for and passed After distance results: "
                 << ifTimer.measure() << endl;
            ifTimer.reset();

            /*Eigen::MatrixXd jg;
            Eigen::MatrixXd bg;*/

            if (useDynamicSlackObjectiveWeightPreTrajectory) {
                double dynamicSlackObjectiveWeight =
                        staticWeightSlack / (desiredPositionTCP(Eigen::all, firstWayPointIndex) - positionTcpCurrent).
                                norm();
                // cout << dynamicSlackObjective << endl;
                ik.setConfigSlackWeight(dynamicSlackObjectiveWeight);
                ik.setDynamicSlackFlagValueForOA(false);
                ik.setValueInequalityConstraints(false);
                if (index >= firstWayPointIndex && useDynamicSlackObjectiveWeightPreTrajectory) {
                    ik.setConfigSlackWeight(staticWeightSlack);
                    ik.setDynamicSlackFlagValueForOA(true);
                    ik.setValueInequalityConstraints(true);
                    useDynamicSlackObjectiveWeightPreTrajectory = false;
                }
            }
            /*cout<<"jointValues: \n"<<jointValues<<endl;
            cout<<"JacobiMatrix: \n"<<JacobiMatrix<<endl;
            cout<<"poseVelocityEffective: \n"<<poseVelocityEffective<<endl;
            cout<<"jg: " <<jg<<endl;
            cout<<"bg:"<<bg<<endl;
            cout<<"jointVelocityWeightMatrix:"<<jointVelocityWeightMatrix<<endl;
            cout<<"jointMInvalues: "<<jointMinValues<<endl;
            cout<<"jointMaxvalues: "<<jointMaxValues<<endl;
            cout<<"jointVelocityMin: "<<jointVelMinValues<<endl;
            cout<<"jointVelocityMax: "<<jointVelMaxValues<<endl;
            cout<< "constraintTolerance: "<<constraintTolerance<<endl;*/
            auto [optimalJointVelocity, exitFlag] = ik.inverseKinematicsWithOA(
                    jointValues, JacobiMatrix, poseVelocityEffective,
                    jointMinValues, jointMaxValues, jointVelMinValues, jointVelMaxValues, jg, bg,
                    dynamicMassMatrix, constraintTolerance, maxIterations, optimalityTolerance, stepTolerance);
            //cout<<"exitFlag: "<<exitFlag<<endl;
            cout << "At index " + indexString + "After inverse kinematics: " << ifTimer.measure() << endl;
            ifTimer.reset();
            if (exitFlag < 0) {
                cout << "jointValues: \n" << jointValues << endl;
                cout << "JacobiMatrix: \n" << JacobiMatrix << endl;
                cout << "poseVelocityEffective: \n" << poseVelocityEffective << endl;
                cout << "jg: " << jg << endl;
                cout << "bg:" << bg << endl;
                cout << "jointVelocityWeightMatrix:" << jointVelocityWeightMatrix << endl;
                cout << "jointMInvalues: " << jointMinValues << endl;
                cout << "jointMaxvalues: " << jointMaxValues << endl;
                cout << "jointVelocityMin: " << jointVelMinValues << endl;
                cout << "jointVelocityMax: " << jointVelMaxValues << endl;
                cout << "constraintTolerance: " << constraintTolerance << endl;

                throw std::runtime_error(
                        "ExitFlag: " + std::to_string(exitFlag) + "Motion failed at index: " + std::to_string(index));
            }

            desiredJointVelocityMatrix(Eigen::all, index) = optimalJointVelocity;

            double tEnd = trajTimes(index);
            double tStart = trajTimes(index - lastPeriod);

            double timespan[2] = {tStart, tEnd};

            if (index > 0) {
                desiredJointValuesMatrix(Eigen::all, index) = ObstacleAvoidance::integrateConstantRungeKutta(
                        desiredJointVelocityMatrix(Eigen::all, index), timespan, previousDesiredJointPositionUpdate);
                previousDesiredJointPositionUpdate = desiredJointValuesMatrix(Eigen::all, index);
            }
            previousDesiredJointVelocityUpdate = optimalJointVelocity;
            jointVelocityObstacleAvoidance = optimalJointVelocity;
            cout << "At index " + indexString + " After integration: " << ifTimer.measure() << endl;
        }
        cout << "At index " + indexString + " After if-statement: " << t.measure() << endl;
        t.reset();
        Eigen::VectorXd const &desiredJointPositionCurrent = previousDesiredJointPositionUpdate;
        Eigen::VectorXd const &desiredJointVelocityCurrent = previousDesiredJointVelocityUpdate;
        if (index >= trajTimes.size() - 1) {
            return franka::MotionFinished(controller.torquePD(
                    robotState, desiredJointPositionCurrent, desiredJointVelocityCurrent, true, coriolisData));
        }
        return controller.torquePD(robotState, desiredJointPositionCurrent, desiredJointVelocityCurrent, true,
                                   coriolisData);
    };
}

void robotControlTrajectory(nlohmann::json const &arguments) {
    // first go to start position
    // TODO: remove this part!!

    std::atomic<bool> exitFlag(false);
    /*std::atomic<bool> exitFlag(false);

    goToStartPosition(arguments, exitFlag);
    if (exitFlag.load()) {
        cout << "Did not succeed to go to the starting position!" << endl;
        return;
    }*/

    auto robotControlConfig = arguments.at("RobotControlConfig");
    ConfigurationParameters config(robotControlConfig, "RobotControlConfig");

    auto worldInRobotFrame = readJsonFile("../config/robotWorldCalibration.json").at("q_world_base").get<Posed>();

    ObstacleCollection obstacles;
    std::thread visualInformationThread(processObstaclesFromVisualData, std::ref(exitFlag), std::ref(obstacles),
                                        arguments, worldInRobotFrame, false);
    //

    auto trajConfig = config.getSubConfig("Trajectory");
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
            ObstacleAvoidance::CartesianTrajectory::positionTrajectory(trajConfig);
    auto [desiredQuaternionsTCPTrajectory, desiredAngularVelocityTCPTrajectory, desiredAngularAccelTrajectory] =
            ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(trajConfig);

    Eigen::VectorXd wayPointTimes = ObstacleAvoidance::stdVectorToEigenVector(
            trajConfig.get<vector<double> >("waypointTimes"));
    auto sampleTime = trajConfig.get<double>("trajectorySampleTime");


    Eigen::VectorXd trajTimesTrajectory = ObstacleAvoidance::generateSequence(wayPointTimes(0), sampleTime,
                                                                              wayPointTimes(Eigen::last));

    ConfigurationParameters pandaRobotConfig(arguments.at("Robot").at("PandaRobot"), "PandaRobot");
    auto robot = ObstacleAvoidance::Robot(pandaRobotConfig);


    auto robotParametersJson = pandaRobotConfig.getJson("robotParameters");
    auto robotResetJointPosition = robotParametersJson["resetJointPosition"].get<std::array<double, 7> >();
    auto robotIpAddress = robotParametersJson["robotIpAddress"].get<std::string>();
    ObstacleAvoidance::Vector7d maxTorques = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["maximumTorques"].get<std::vector<double> >());
    ObstacleAvoidance::Vector7d minTorques = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["minimumTorques"].get<std::vector<double> >());
    ObstacleAvoidance::Vector7d pdControllerKp = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kp"].get<std::vector<double> >());
    ObstacleAvoidance::Vector7d pdControllerKd = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kd"].get<std::vector<double> >());

    int numberJoints = static_cast<int>(robot.getNumberJoints());


    auto diagValuesKp = ObstacleAvoidance::stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("positionKp").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKp(diagValuesKp);
    Eigen::MatrixXd Kp = diagMatrixKp.toDenseMatrix();

    auto diagValuesKo = ObstacleAvoidance::stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();

    Eigen::VectorXd jointPositionAtStart(numberJoints);

    auto jointVelocityWeight = ObstacleAvoidance::stdVectorToEigenVector(
            inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
                    "jointVelocityWeight").get<std::vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();

    FILE *fp;
    char const *filename = "frankaData.txt";

    // Open the file for writing in binary mode
    fp = fopen(filename, "wb");
    if (fp == nullptr) {
        perror("Failed to open file");
        return;
    }

    try {
        franka::Robot realRobot(robotIpAddress);
        realRobot.automaticErrorRecovery();
        setDefaultBehavior(realRobot);

        /*std::cout << "WARNING: This example will move the robot! "
               << "Please make sure to have the user stop button at hand!" << std::endl
               << "Press Enter to continue..." << std::endl;
        std::cin.ignore();*/
        MotionGenerator resetJointPosition(0.2, robotResetJointPosition);
        realRobot.control(resetJointPosition);
        std::cout << "Finished moving to initial reset Joint configuration." << std::endl;

        jointPositionAtStart = Eigen::Map<Eigen::Matrix<double, 7, 1> >(
                realRobot.readOnce().q.data());

        Eigen::Matrix4d transformationTCP = robot.fkmCartesianTCP(jointPositionAtStart);

        Eigen::Vector3d preWayPoint = transformationTCP(Eigen::seq(0, 2), 3);

        Eigen::Matrix<double, 3, 2> preWaypointMatrix;
        preWaypointMatrix(Eigen::all, 0) = preWayPoint;
        preWaypointMatrix(Eigen::all, 1) = desiredPositionTCPTrajectory(Eigen::all, 0);

        Eigen::Matrix<double, 4, 2> preOrientations;
        preOrientations(Eigen::all, 0) = ObstacleAvoidance::computeQuaternionFromMatrix(transformationTCP);
        preOrientations(Eigen::all, 1) = desiredQuaternionsTCPTrajectory(Eigen::all, 0);

        double preEndTimeTrajectory = trajConfig.get<double>("preTrajectoryTime");
        Eigen::RowVector2d preWaypointTimes;
        preWaypointTimes << 0, preEndTimeTrajectory;

        auto [desiredPositionTCPPre, desiredVelocityTCPPre, xdAccelPre] =
                ObstacleAvoidance::CartesianTrajectory::positionTrajectory(
                        preWaypointMatrix, preWaypointTimes, sampleTime, "cubic",
                        Eigen::MatrixXd::Zero(3, 2), Eigen::MatrixXd::Zero(3, 2));
        auto [desiredQuaternionsTCPPre, desiredAngularVelocityTCPPre, desiredAngularAccelPre] =
                ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(
                        preOrientations, preWaypointTimes, sampleTime, "cubic");

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

        actualJointValuesMatrix(Eigen::all, 0) = jointPositionAtStart;
        cout << "Starting Joint Position: \n" << jointPositionAtStart << endl;
        desiredJointValuesMatrix(Eigen::all, 0) = jointPositionAtStart;
        auto jointVelMinValues = robot.getJoints()->velMinValues;
        auto jointVelMaxValues = robot.getJoints()->velMaxValues;
        auto jointMinValues = robot.getJoints()->minValues;
        auto jointMaxValues = robot.getJoints()->maxValues;

        ObstacleAvoidance::ControllerFranka controller(maxTorques, minTorques, pdControllerKp, pdControllerKd);

        ObstacleAvoidance::OptimizationBasedIKWithOA ik(inverseKinematicsConfig);

        // Load the kinematics and dynamics model.
        franka::Model model = realRobot.loadModel();

        long index = 0;
        ObstacleAvoidance::Vector7d desiredJointPositionUpdate = jointPositionAtStart;
        ObstacleAvoidance::Vector7d desiredJointVelocityUpdate = ObstacleAvoidance::Vector7d::Zero();

        Eigen::VectorXd jointVelocityObstacleAvoidance = Eigen::VectorXd::Zero(numberJoints);

        ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/desiredTcpTrajectory.csv",
                                            desiredPositionTCP);
        // all the variables for plotting results:

        Eigen::MatrixXd distanceAllLinks(numberJoints + 1, trajTimes.size());
        distanceAllLinks.setConstant(std::numeric_limits<double>::quiet_NaN());


        Eigen::MatrixXd minimumDistanceRobotObstacles(1, trajTimes.size());

        minimumDistanceRobotObstacles.setConstant(std::numeric_limits<double>::quiet_NaN());

        Eigen::MatrixXd actualPositionTCP(desiredPositionTCP.rows(), trajTimes.size());
        actualPositionTCP.setConstant(std::numeric_limits<double>::quiet_NaN());

        Eigen::MatrixXd errorPositionTCP(desiredPositionTCP.rows(), trajTimes.size());
        errorPositionTCP.setConstant(std::numeric_limits<double>::quiet_NaN());

        // TODO: compute the index at which the trajectory starts!
        // TODO: pass the slackWeight variable
        // TODO: create the goToFirstWayPoint bool variable and pass it to function

        // Just Using External Torque Controller
        realRobot.control(getRobotControlFunction(
                                  model, fp, robot, obstacles, actualJointValuesMatrix, desiredJointValuesMatrix,
                                  desiredJointVelocityMatrix, index, startingIndexTrajectory, trajTimes, desiredPositionTCP,
                                  desiredVelocityTCP,
                                  desiredQuaternionsTCP, desiredAngularVelocityTCP, Kp, Ko, ik, controller, jointMinValues,
                                  jointMaxValues, jointVelMinValues, jointVelMaxValues, desiredJointPositionUpdate,
                                  desiredJointVelocityUpdate, jointVelocityWeightMatrix, jointVelocityObstacleAvoidance,
                                  useDynamicSlackObjectiveWeightPre, staticWeightSlack, constraintTolerance, maxIterations,
                                  optimalityTolerance, stepTolerance, actualPositionTCP, errorPositionTCP, distanceAllLinks,
                                  minimumDistanceRobotObstacles),
                          true);

        ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/actualPositionTCP.csv",actualPositionTCP);
        ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/errorPositionTCP.csv",errorPositionTCP);
        ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/distanceAllLinks.csv",distanceAllLinks);
        ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/minimumDistanceRobotObstacles.csv",minimumDistanceRobotObstacles);
        ObstacleAvoidance::writeMatrixToCSV("../ExperimentsShobhit/Experiment1/actualJointValuesMatrix.csv",actualJointValuesMatrix);



    } catch (franka::Exception const &ex) {
        std::cerr << ex.what() << std::endl;
    } catch (std::runtime_error &ex) {
        std::cerr << "Non-franka related exception: " << ex.what() << std::endl;
    } /*catch (franka::ControlException const &ex) {
        std::cerr << logToCSV(ex.log) << std::endl;
    }*/


    fclose(fp);

    exitFlag.store(true);
    if (visualInformationThread.joinable()) {
        visualInformationThread.join();
    }
    //
}

int main() {
    auto arguments = readJsonFile("../config/configShobhit/realTimeObstacleAvoidanceArguments.json");

    auto taskRepresentationLibConfig = arguments.at("TaskRepresentationLib");
    TaskRepresentationLib::setConfigurationParameters(
            ConfigurationParameters(taskRepresentationLibConfig, "TaskRepresentationLib"));

    robotControlTrajectory(arguments);

    return 0;
}
