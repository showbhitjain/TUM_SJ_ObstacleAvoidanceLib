//
// Created by shobhit on 04.07.25.
//
//
// Created by shobhit on 02.02.25.
//


#include <AndreiUtils/utils.hpp>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsJsonEigen.hpp>
#include <AndreiUtils/utilsThread.h>
#include <ConceptLibrary/concepts/AgentConcept.h>
#include <ConceptLibrary/concepts/GripperConcept.h>
#include <ConceptLibrary/instances/geometry/BoxShape.h>
#include <ConceptLibrary/instances/geometry/PlaneShape.h>
#include <ConceptLibrary/instances/geometry/PyramidShape.h>
#include <ConceptLibrary/instances/geometry/SphereShape.h>
#include <ConceptLibrary/instances/geometry/CylinderShape.h>
#include <ConceptLibrary/valueDomains/Instance.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <TaskRepresentationLib/utils.h>
#include <FrankaPandaExecutor.h>
#include <FrankaGripperExecutor.h>
#include <ranges>
#include <VisualInput.h>
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
using namespace ConceptLibrary;
using namespace TaskRepresentationLib;

using Vector7d = Eigen::Matrix<double, 7, 1>;
using ObjectInstance = Instance<ConceptList<ObjectConcept> >;
using AgentInstance = Instance<ConceptList<AgentConcept> >;
using GripperInstance = Instance<ConceptList<GripperConcept> >;




std::map<std::string, Obstacle> convertToObstacleMap(std::map<string, VisualInput::ObjectDetectionData> &detectionMap,
                                                     AndreiUtils::Posed const &worldInRobotFrame) {
    std::map<std::string, Obstacle> obstaclesMap;
    for (auto const &object: detectionMap) {
        Instance<ConceptList<ObjectConcept> > instance(object.first);
        instance.setGlobalPose(worldInRobotFrame * object.second.detectionPose);
        instance.shape->setEnvironmentPose(worldInRobotFrame * object.second.detectionPose);
        if (AndreiUtils::pointerInstanceOf<ConceptLibrary::BoxShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::BoxShape> boxShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::BoxShape>(instance.shape);
            auto boxObstacle = Obstacle(*boxShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, boxObstacle);
        } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::CylinderShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::CylinderShape> cylinderShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::CylinderShape>(instance.shape);
            auto cylinderObstacle = Obstacle(*cylinderShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, cylinderObstacle);
        } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::PlaneWidthShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::PlaneWidthShape> planeWidthShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::PlaneWidthShape>(instance.shape);
            auto planeObstacle = Obstacle(*planeWidthShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, planeObstacle);
        } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::SphereShape>(instance.shape)) {
            std::shared_ptr<ConceptLibrary::SphereShape> sphereShapeObject = std::dynamic_pointer_cast<
                    ConceptLibrary::SphereShape>(instance.shape);
            auto sphereObstacle = Obstacle(*sphereShapeObject);
            obstaclesMap.emplace(instance.instanceId.s, sphereObstacle);
        } else {
            std::cerr << "Error: Shape type not recognized for object: " << std::endl;
        }
    }

    return obstaclesMap;
}


std::map<std::string, Obstacle> processObstaclesFromVisualData(nlohmann::json const &arguments,
                                                               AndreiUtils::Posed worldInRobotFrame) {
    auto visualInputConfig = arguments.at("VisualInput123");
    VisualInput input(ConfigurationParameters(visualInputConfig, "VisualInput"));


    ConfigurationParameters visualInputConfigParams(visualInputConfig, "VisualInput");
    auto const &visualInputConfigParamsJson = visualInputConfigParams.getJson();

    /*std::vector<double> coefficients;
    coefficients = {
        0.7071067811865476, 0.0, 0.0, 0.7071067811865475, 0.0032605735368182627, 0.06401332101374885,
        -0.34142961248072945, -0.0032605735368182636
    };
    AndreiUtils::Posed worldInRobotFrame = AndreiUtils::Posed::createFromCoefficients(coefficients);*/
    std::map<std::string, Obstacle> obstaclesStaticMap;
//    input.getTime();
    while (input.processInput()) {
        cout << "First detection started" << endl;
        auto cameraPose = input.getCameraPose();
        cout << "Camera is at pose " << input.getCameraPose().toString() << " relative to the origin" << endl;
        if (cameraPose != AndreiUtils::Posed::one) {
            for (auto const &object: input.getObjectInstanceDetections()) {
                cout << "Object " << object.first << " is at pose " << object.second.detectionPose.toString()
                     << " relative to the origin!" << endl;
                Instance<ConceptList<ObjectConcept> > instance(object.first);
                instance.setGlobalPose(worldInRobotFrame * object.second.detectionPose);
                instance.shape->setEnvironmentPose(worldInRobotFrame * object.second.detectionPose);
                if (AndreiUtils::pointerInstanceOf<ConceptLibrary::BoxShape>(instance.shape)) {
                    std::shared_ptr<ConceptLibrary::BoxShape> boxShapeObject = std::dynamic_pointer_cast<
                            ConceptLibrary::BoxShape>(instance.shape);
                    auto dimension = boxShapeObject->dim;
                    auto center = boxShapeObject->p;
                    auto orientation = boxShapeObject->q;
                    cout << "Dimension of detected " << instance.instanceId.s << " Box Obstacle = " << dimension <<
                         endl;
                    cout << "Center of detected " << instance.instanceId.s << " Box Obstacle =  " << center << endl;
                    cout << "Orientation of detected " << instance.instanceId.s << " Box Obstacle = " << orientation <<
                         endl;
                } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::CylinderShape>(instance.shape)) {
                    std::shared_ptr<ConceptLibrary::CylinderShape> cylinderShapeObject = std::dynamic_pointer_cast<
                            ConceptLibrary::CylinderShape>(instance.shape);
                    auto height = cylinderShapeObject->height;
                    auto center = cylinderShapeObject->center;
                    auto axis = cylinderShapeObject->axis;
                    auto radius = cylinderShapeObject->radius;
                    cout << "Height of detected " << instance.instanceId.s << " Cylinder Obstacle = " << height << endl;
                    cout << "Center of detected " << instance.instanceId.s << " Cylinder Obstacle =  " << center <<
                         endl;
                    cout << "axis of detected " << instance.instanceId.s << " Cylinder Obstacle = " << axis << endl;
                } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::PlaneWidthShape>(instance.shape)) {
                    std::shared_ptr<ConceptLibrary::PlaneWidthShape> planeWidthShapeObject = std::dynamic_pointer_cast<
                            ConceptLibrary::PlaneWidthShape>(instance.shape);
                    auto width = planeWidthShapeObject->width;
                    auto center = planeWidthShapeObject->point;
                    auto axis = planeWidthShapeObject->axis;
                    cout << "Width of detected " << instance.instanceId.s << " PlaneWithWidth Obstacle= " << width <<
                         endl;
                    cout << "Center of detected " << instance.instanceId.s << " PlaneWithWidth Obstacle=  " << center <<
                         endl;
                    cout << "axis of detected " << instance.instanceId.s << " PlaneWithWidth Obstacle= " << axis <<
                         endl;
                } else if (AndreiUtils::pointerInstanceOf<ConceptLibrary::SphereShape>(instance.shape)) {
                    std::shared_ptr<ConceptLibrary::SphereShape> sphereShapeObject = std::dynamic_pointer_cast<
                            ConceptLibrary::SphereShape>(instance.shape);
                    auto radius = sphereShapeObject->radius;
                    auto center = sphereShapeObject->center;
                    cout << "Radius of detected " << instance.instanceId.s << " Sphere Obstacle= " << radius << endl;
                    cout << "Center of detected " << instance.instanceId.s << " Sphere Obstacle=  " << center << endl;
                }

                cout << "Shape of " << object.first << " = " << instance.parameters->getValue<
                        ObjectConcept::basicShapeProperty>().s << endl;
                Eigen::Matrix<double, 2, 3> bb = dynamic_pointer_cast<PerceptionData::ObjectTagModel>(instance.geometry)
                        ->getBoundingBoxParameters();
            }
            cv::imshow("Color Image", input.getVisualColor());
            cv::imshow("Depth Image", input.getVisualDepth());
        } else {
            std::cerr << "Failed to process input " << std::endl;
        }

        cv::imshow("Color Image", input.getVisualColor());
        cv::imshow("Depth Image", input.getVisualDepth());
        int k = cv::waitKey(1);

        if (k == 'q' || k == 27) {
            obstaclesStaticMap = convertToObstacleMap(input.getObjectInstanceDetections(),worldInRobotFrame);
            break;
        }
    }
}

void goToStartPositionNew(nlohmann::json const &arguments, std::atomic<bool> &exitFlag) {
    auto robotControlConfig = arguments.at("RobotControlConfig");
    ConfigurationParameters config(robotControlConfig, "RobotControlConfig");

    auto trajConfig = config.getSubConfig("Trajectory");
    double preTrajectoryWayPointTime = trajConfig.get<double>("preTrajectoryTime");

    auto inverseKinematicsConfig = config.getSubConfig("inverseKinematicsPreTrajectory");

    auto robotConfig = arguments.at("Robot");
    ConfigurationParameters pandaRobotConfig(robotConfig.at("PandaRobot"), "PandaRobot");
    cout << pandaRobotConfig.getJson().dump(4) << endl;
    auto robot = ObstacleAvoidance::Robot(pandaRobotConfig);

    auto robotParametersJson = pandaRobotConfig.getJson("robotParameters");
    auto robotIpAddress = robotParametersJson["robotIpAddress"].get<std::string>();
    ObstacleAvoidance::Vector7d maxTorques = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["maximumTorques"].get<std::vector<double> >());
    ObstacleAvoidance::Vector7d minTorques = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["minimumTorques"].get<std::vector<double> >());
    ObstacleAvoidance::Vector7d pdControllerKp = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kp"].get<std::vector<double> >());
    ObstacleAvoidance::Vector7d pdControllerKd = ObstacleAvoidance::stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kd"].get<std::vector<double> >());


    double sampleTime = trajConfig.get<double>("trajectorySampleTime");
    double preEndTimeTrajectory = trajConfig.get<double>("preTrajectoryTime");
    Eigen::RowVector2d preWaypointTimes;
    preWaypointTimes << 0, preEndTimeTrajectory;


    Eigen::VectorXd trajTimes = ObstacleAvoidance::generateSequence(0, sampleTime, preEndTimeTrajectory);
    int numberJoints = static_cast<int>(robot.getNumberJoints());
    Eigen::MatrixXd actualJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    Eigen::MatrixXd actualJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    Eigen::MatrixXd desiredJointValuesMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());
    Eigen::MatrixXd desiredJointVelocityMatrix = Eigen::MatrixXd::Zero(numberJoints, trajTimes.size());

    auto diagValuesKp = ObstacleAvoidance::stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("positionKp").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKp(diagValuesKp);
    Eigen::MatrixXd Kp = diagMatrixKp.toDenseMatrix();

    auto diagValuesKo = ObstacleAvoidance::stdVectorToEigenVector(
            trajConfig.getJson("inverseKinematics").at("orientationKo").get<vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagMatrixKo(diagValuesKo);
    Eigen::MatrixXd Ko = diagMatrixKo.toDenseMatrix();
    auto waypoints = ObstacleAvoidance::vectorMatrixToEigenMatrix(
            trajConfig.get<std::vector<std::vector<double> > >("Waypoints"));
    auto orientations = ObstacleAvoidance::vectorMatrixToEigenMatrix(
            trajConfig.get<std::vector<std::vector<double> > >("orientations"));
    Eigen::MatrixXd jg = Eigen::MatrixXd::Zero(1, numberJoints);
    Eigen::VectorXd bg = Eigen::VectorXd::Zero(1);
    double staticWeightSlack = inverseKinematicsConfig.getJson("SlackParameters").at(
            "SlackObjectiveWeight").get<double>();
    bg.resize(0);
    jg.resize(0, 0);


    try {
        franka::Robot realRobot(robotIpAddress);
        realRobot.automaticErrorRecovery();
        setDefaultBehavior(realRobot);

        std::cout << "WARNING: This example will move the robot! "
                  << "Please make sure to have the user stop button at hand!" << std::endl
                  << "Press Enter to continue..." << std::endl;
        std::cin.ignore();

        ObstacleAvoidance::Vector7d startingMeasuredJointPosition = Eigen::Map<Eigen::Matrix<double, 7, 1> >(
                realRobot.readOnce().q.data());
        actualJointValuesMatrix(Eigen::all, 0) = startingMeasuredJointPosition;
        desiredJointValuesMatrix(Eigen::all, 0) = startingMeasuredJointPosition;

        Eigen::Matrix4d transformationTCP = robot.fkmCartesianTCP(startingMeasuredJointPosition);


        Eigen::Vector3d preWayPoint = transformationTCP(Eigen::seq(0, 2), 3);

        Eigen::Matrix<double, 3, 2> preWaypointMatrix;
        preWaypointMatrix(Eigen::all, 0) = preWayPoint;
        preWaypointMatrix(Eigen::all, 1) = waypoints(Eigen::all, 0);

        Eigen::Matrix<double, 4, 2> preOrientations;
        preOrientations(Eigen::all, 0) = ObstacleAvoidance::computeQuaternionFromMatrix(transformationTCP);
        preOrientations(Eigen::all, 1) = orientations(Eigen::all, 0);

        auto [desiredPositionTCP, desiredVelocityTCP, xdAccel] =
                ObstacleAvoidance::CartesianTrajectory::positionTrajectory(
                        preWaypointMatrix, preWaypointTimes, sampleTime, "cubic",
                        Eigen::MatrixXd::Zero(3, 2), Eigen::MatrixXd::Zero(3, 2));
        auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
                ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(
                        preOrientations, preWaypointTimes, sampleTime, "cubic");


        auto jointVelMinValues = robot.getJoints()->velMinValues;
        auto jointVelMaxValues = robot.getJoints()->velMaxValues;
        auto jointMinValues = robot.getJoints()->minValues;
        auto jointMaxValues = robot.getJoints()->maxValues;

        ObstacleAvoidance::OptimizationBasedIKWithOA ik(inverseKinematicsConfig);
        ObstacleAvoidance::ControllerFranka controller(maxTorques, minTorques, pdControllerKp, pdControllerKd);
        auto jointVelocityWeight = ObstacleAvoidance::stdVectorToEigenVector(
                inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
                        "jointVelocityWeight").get<std::vector<double> >());
        Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
        Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();
        // Load the kinematics and dynamics model.
        franka::Model model = realRobot.loadModel();

        long index = 0;
        ObstacleAvoidance::Vector7d desiredJointPositionUpdate;
        desiredJointPositionUpdate = startingMeasuredJointPosition;

        ObstacleAvoidance::Vector7d desiredJointVelocityUpdate = ObstacleAvoidance::Vector7d::Zero();

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
                actualJointValuesMatrix(Eigen::all, index) = Eigen::Map<const Eigen::Matrix<double, 7, 1>>(
                        robotState.q.data());
                auto measuredJointValueThisCylcle = Eigen::Map<const Eigen::Matrix<double, 7, 1>>(robotState.q.data());

                auto transformTcpToBase = robot.fkmCartesianTCP(actualJointValuesMatrix(Eigen::all, index));
                Eigen::VectorXd positionTcpCurrent = transformTcpToBase(Eigen::seq(0, 2), 3);

                auto errorCartesian = (desiredPositionTCP(Eigen::all, index) - positionTcpCurrent);
                auto xdEffective = desiredVelocityTCP(Eigen::all, index) + (
                        Kp * errorCartesian);
                auto orientationError = ObstacleAvoidance::computeOrientationError(
                        transformTcpToBase,
                        desiredQuaternionsTCP(Eigen::all, index));

                auto angularVelocityEffective = desiredAngularVelocityTCP(Eigen::all, index) + (Ko * orientationError);

                //cout << "Position Error: " << (desiredPositionTCP(all, index) - positionTcpCurrent) << endl;
                //cout << "Orientation Error: " << orientationError << endl;
                auto JacobiMatrix = robot.jacobianCartesianTCP(actualJointValuesMatrix(Eigen::all, index));
                Eigen::VectorXd poseVelocityEffective(6);
                poseVelocityEffective(Eigen::seq(0, 2)) = xdEffective;
                // poseVelocityEffective(seq(3, 5)) = angularVelocityEffective;
                poseVelocityEffective(Eigen::seq(3, 5)) = angularVelocityEffective;
                //cout << "poseVelocityEffective: " << poseVelocityEffective << endl;

                // cout << "error between final and current: " << ((waypoints(all, 0) - positionTcpCurrent).norm()) <<endl;

                // cout << "error current: " << errorCartesian.norm() << endl;

                double dynamicSlackObjective = (staticWeightSlack / ((waypoints(Eigen::all, 0) - positionTcpCurrent).
                        norm()));
                // cout << dynamicSlackObjective << endl;
                ik.setConfigSlackWeight(dynamicSlackObjective);
                ik.setValueInequalityConstraints();
                auto [optimalJointVelocity, exitFlag] = ik.inverseKinematicsWithOA(
                        actualJointValuesMatrix(Eigen::all, index), JacobiMatrix,
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

                desiredJointVelocityMatrix(Eigen::all, index) = optimalJointVelocity;


                double tEnd = trajTimes(index);
                double tStart = trajTimes(index - lastPeriod);

                double timespan[2] = {tStart, tEnd};

                if (index > 0) {
                    desiredJointValuesMatrix(Eigen::all, index) = ObstacleAvoidance::integrateConstantRungeKutta(
                            desiredJointVelocityMatrix(Eigen::all, index), timespan, desiredJointPositionUpdate);
                    desiredJointPositionUpdate = desiredJointValuesMatrix(Eigen::all, index);
                }
                desiredJointVelocityUpdate = optimalJointVelocity;
                //cout << "desired Joint Value: \n" << desiredJointValuesMatrix(all, index) << endl;
            }
            Eigen::VectorXd desiredJointPositionCurrent = desiredJointPositionUpdate;
            Eigen::VectorXd desiredJointVelocityCurrent = desiredJointVelocityUpdate;
            /*cout << "index: " << index << endl;
            cout << "control command success rate: " << robotState.control_command_success_rate << endl;*/
            if (index >= trajTimes.size() - 1) {
                return franka::MotionFinished(controller.torquePD(robotState, desiredJointPositionCurrent,
                                                                  desiredJointVelocityCurrent, true,
                                                                  model.coriolis(robotState)));
            }
            return controller.torquePD(robotState, desiredJointPositionCurrent, desiredJointVelocityCurrent, true,
                                       coriolisData);
        };
        realRobot.control(torqueCallback, true);
    } catch (franka::Exception const &ex) {
        std::cerr << ex.what() << std::endl;
        exitFlag.store(true);
    } catch (std::runtime_error &ex) {
        std::cerr << "Non-franka related exception: " << ex.what() << std::endl;
        exitFlag.store(true);
    }
}

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
    cout << (robot.getJoints() == nullptr) << endl;
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

                // cout << "error between final and current: " << ((waypoints(all, 0) - positionTcpCurrent).norm()) <<endl;

                // cout << "error current: " << errorCartesian.norm() << endl;
                double dynamicSlackObjective = (staticWeightSlack / ((waypoints(all, 0) - positionTcpCurrent).norm()));
                // cout << dynamicSlackObjective << endl;
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

        };
        realRobot.control(torqueCallback, true);
    } catch (franka::Exception const &ex) {
        std::cerr << ex.what() << std::endl;
    } catch (std::runtime_error &ex) {
        std::cerr << "Non-franka related exception: " << ex.what() << std::endl;
    }
}

void offlineObstacleAvoidanceAndTrajectoryTracking(std::map<std::string, Obstacle> const &obstaclesMap) {
}


int main() {
    auto arguments1 = readJsonFile("../config/configShobhit/realTimeObstacleAvoidanceArguments.json");
    std::atomic<bool> exitFlag(false);

    /*goToStartPositionNew(arguments1, exitFlag);
    if (exitFlag.load()) {
        cout << "Did not succeed to go to the starting position!" << endl;
        return 0;
    }
    return 0;*/
    goToStartPosition();
    auto arguments = readJsonFile("../config/playWithVisualInputArguments.json");
    auto taskRepresentationLibConfig = arguments.at("TaskRepresentationLib");
    // arguments.contains("Hello World!");
    TaskRepresentationLib::setConfigurationParameters(
            ConfigurationParameters(taskRepresentationLibConfig, "TaskRepresentationLib"));
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


    auto worldInRobotFrame = readJsonFile("../config/robotWorldCalibration.json").at("q_world_base").get<Posed>();
    auto detectedObstaclesMap = processObstaclesFromVisualData(arguments, worldInRobotFrame);
    for (auto const &obstacle : detectedObstaclesMap) {
        auto obstacleId = obstacle.first ;
        auto obstacleObject = obstacle.second;
        cout<< "obstacleId: " << obstacleId << ", obstacleObject type: " << obstacleObject.obstacleType << endl;
        cout<<"obstacle Dimensions: \n"<< obstacleObject.dimensions << endl;
        cout<<"Obstacle Center: \n"<< obstacleObject.center << endl;

    }
    auto robot = ObstacleAvoidance::Robot("../config/configShobhit/robotModellingParameters.json", "RobotModelling",
                                          "PandaRobot");
    auto numberJoints = robot.getNumberJoints();

    //for real robot get the position from robot and do forward kinematics
    Eigen::VectorXd jointPositionAtStart(numberJoints);

    try {
        franka::Robot realRobot(robotIpAddress);
        setDefaultBehavior(realRobot);
        jointPositionAtStart = Eigen::Map<Eigen::Matrix<double, 7, 1> >(
                realRobot.readOnce().q.data());
    } catch (franka::Exception const &ex) {
        std::cerr << ex.what() << std::endl;
    }

    Matrix4d transformationTCP = robot.fkmCartesianTCP(jointPositionAtStart);
    Vector3d waypointOne = transformationTCP(seq(0, 2), 3);

    auto waypoints = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double> > >("Waypoints"));

    waypoints(all, 0) = waypointOne;
    //cout << waypoints(all, 0) << endl;
    auto wayPointVelocities = vectorMatrixToEigenMatrix(
            trajConfig.get<std::vector<std::vector<double> > >("WayPointVelocity"));

    auto ts = trajConfig.get<double>("trajectorySampleTime");
    auto positionTrajectoryType = trajConfig.get<std::string>("positionTrajectoryType");
    auto waypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double> >("waypointTimes"));

    auto [desiredPositionTCP,desiredVelocityTCP,xdAccel] = ObstacleAvoidance::CartesianTrajectory::positionTrajectory(
            waypoints, waypointTimes, ts, positionTrajectoryType,
            wayPointVelocities, Eigen::MatrixXd::Zero(3, 2));

    auto orientations = vectorMatrixToEigenMatrix(trajConfig.get<std::vector<std::vector<double> > >("orientations"));
    orientations(all, 0) = computeQuaternionFromMatrix(transformationTCP);

    //preOrientations<<
    auto [desiredQuaternionsTCP, desiredAngularVelocityTCP, desiredAngularAccel] =
            ObstacleAvoidance::CartesianTrajectory::orientationTrajectory(orientations, waypointTimes, ts, "cubic");

    auto ikJson = readJsonFile("../config/configShobhit/inverseKinematicsWithOAParameters.json");
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

    actualJointValuesMatrix(all, 0) = jointPositionAtStart;
    desiredJointValuesMatrix(all, 0) = jointPositionAtStart;
    auto jointVelMinValues = robot.getJoints()->velMinValues;
    auto jointVelMaxValues = robot.getJoints()->velMaxValues;
    auto jointMinValues = robot.getJoints()->minValues;
    auto jointMaxValues = robot.getJoints()->maxValues;

    OptimizationBasedIKWithOA ik(ikConfig);
    auto jointVelocityWeight = stdVectorToEigenVector(inverseKinematicsConfig.getJson("ObjectiveConstraints").at(
            "jointVelocityWeight").get<std::vector<double> >());
    Eigen::DiagonalMatrix<double, Eigen::Dynamic> jointVelocityMatrix(jointVelocityWeight);
    Eigen::MatrixXd jointVelocityWeightMatrix = jointVelocityMatrix.toDenseMatrix();


    VectorXd jointVelocityObstacleAvoidance = VectorXd::Zero(numberJoints);

    /*Vector3d center1 = {0.6000, 0.0, 0.1211};
    VectorXd dimensions1(3);
    dimensions1 << 0.04, 0.04, 0.12;
    auto obstacle1 = Obstacle("Box", center1, dimensions1, {1, 0, 0, 0});

     Vector3d center2 = {0.5545, 0.20, 0.3211};

     VectorXd dimensions2(1);
     dimensions2(0) = 0.05;
     cout << dimensions2 << endl;
     auto obstacle2 = Obstacle("Sphere", center2, dimensions2,{1,0,0,0});
    std::vector<Obstacle> obstaclesArray;
     obstaclesArray.push_back(obstacle1);
            obstaclesArray.push_back(obstacle2);
    auto obstaclesMap = conversionObstaclesVectorToMap(obstaclesArray);*/
    auto obstaclesMap = detectedObstaclesMap;
    /*
        Eigen::MatrixXd jg = Eigen::MatrixXd::Zero(1, numberJoints);
       Eigen::VectorXd bg = Eigen::VectorXd::Zero(1);

       jg.resize(0,0);
       bg.resize(0);

    */


    for (int i = 0; i < trajTimes.size(); i++) {
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

        // cout << "error current: " << errorCartesian.norm() << endl;


        //
        auto [jg, bg, minDistance] = robot.obstacleAvoidanceEquation(obstaclesMap, actualJointValuesMatrix(all, i),
                                                                     jointVelocityObstacleAvoidance);


        /*
        cout << "jg at " + std::to_string(i) + ": \n" << jg << endl;

        cout << "bg at " + std::to_string(i) + ": \n" << bg << endl;

        cout << "minimum distance at " + std::to_string(i) + ": " << minDistance << endl;
        */


        auto [optimalJointVelocity, ExitFlag] = ik.inverseKinematicsWithOA(actualJointValuesMatrix(all, i),
                                                                           JacobiMatrix,
                                                                           poseVelocityEffective,
                                                                           jointMinValues, jointMaxValues,
                                                                           jointVelMinValues,
                                                                           jointVelMaxValues, jg, bg,
                                                                           jointVelocityWeightMatrix,1e-6,400,1e-6,1e-6);

        //cout<<"optimal Joint Velocity: \n" <<optimalJointVelocity<<endl;
        desiredJointVelocityMatrix(all, i) = optimalJointVelocity;
        jointVelocityObstacleAvoidance = optimalJointVelocity;
        if (ExitFlag < 0) {
            cout << "At this step: " << i << endl;
            cout << "ExitFlag: " << ExitFlag << endl;
            cout << "optimal Joint Velocity: \n" << optimalJointVelocity << endl;
            break;
        }

        if (minDistance < 0.01) {
            cout << "minDistance has become less than 0.01 at " + std::to_string(i) + ": " << minDistance << endl;
            break;
        }

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
        /*actualJointValuesMatrix(all, i) = addRandomNoisetoJointsSignal(desiredJointValuesMatrix(all, i), -0.005,
                                                                       0.005);*/

        actualJointValuesMatrix(all, i) =  desiredJointValuesMatrix(all, i);
        bool valid = robot.getJoints()->isConfigurationValid(actualJointValuesMatrix(all, i), true);
        if (!valid) {
            cout << "At this step: " << i << endl;
            break;
        }
        //cout<<"desired Joint Value: \n"<< desiredJointValuesMatrix(all, i+1)<<endl;
    }

    cout << "actualJointValues:  \n" << actualJointValuesMatrix(all, last) << endl;
    writeMatrixToCSV("../outputDesiredJointValuesDemoPartTwo.csv", desiredJointValuesMatrix);
    writeMatrixToCSV("../outputDesiredJointVelocityDemoPartTwo.csv",desiredJointVelocityMatrix);

    Vector7d maxTorques = stdVectorToEigenVector(robotParametersJson["maximumTorques"].get<std::vector<double> >());
    Vector7d minTorques = stdVectorToEigenVector(robotParametersJson["minimumTorques"].get<std::vector<double> >());
    Vector7d pdControllerKp = stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kp"].get<std::vector<double> >());
    Vector7d pdControllerKd = stdVectorToEigenVector(
            robotParametersJson["Controller"]["Kd"].get<std::vector<double> >());

    try {
        desiredJointValuesMatrix = readMatrixFromCSV("../outputDesiredJointValuesDemoPartTwo.csv");
        desiredJointVelocityMatrix = readMatrixFromCSV("../outputDesiredJointVelocityDemoPartTwo.csv");
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
