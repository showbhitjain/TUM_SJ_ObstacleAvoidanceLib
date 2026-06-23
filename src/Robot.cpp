//
// Created by shobhit on 08.10.24.
//
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <TUM_SJ_ObstacleAvoidanceLib/Robot.h>
#include <AndreiUtils/utils.h>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsGeometry.h>
#include <AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsJson.h>

#include <cmath>
#include <TUM_SJ_ObstacleAvoidanceLib/ObstacleAvoidanceUtils.h>
#include <iostream>

using namespace AndreiUtils;
using namespace Eigen;
using namespace ObstacleAvoidance;
using namespace std;
using json = nlohmann::json;

namespace {
    // Distance below which two robot link segments are considered adjacent when
    // building the self-collision avoidance constraints.
    constexpr double kSelfCollisionAdjacencyThreshold = 0.2;
    // Tolerance for treating a dot product as zero when detecting the principal axis
    // of a box-shaped final link.
    constexpr double kAxisAlignmentTolerance = 0.0001;
}


//configFile_Path : path to the file which contains paths to config file for the type of robot
//which_robot = type of robot
Robot::Robot(const std::string &configFile_Path, const std::string &parameterFor,
             const std::string &whichrobot) : Robot(
        ConfigurationParameters(configFile_Path, parameterFor).getSubConfig(whichrobot)) {
}

Robot::Robot(AndreiUtils::ConfigurationParameters const &config) {
    auto eul = config.getJson("displacementEEtoTCP").at("rotationXYZ").get<std::vector<double> >();
    for (auto &angle: eul) {
        angle = deg2Rad(static_cast<double>(angle));
    }
    this->mdhMatrix = vectorMatrixToEigenMatrix(config.get<vector<vector<double> > >("mdhParameters"));
    auto translation = config.getJson("displacementEEtoTCP").at(
            "translation").get<std::vector<double> >();

    this->transformationEEToTCP =
            trvec2tform(stdVectorToEigenVector(translation)) * convertEulerToTransform(stdVectorToEigenVector(eul));

    auto obstacleAvoidanceParameters = config.getJson("ObstacleAvoidanceParameters");
    this->radiusLinks = stdVectorToEigenVector(obstacleAvoidanceParameters["radiusLinks"].get<vector<double> >());
    this->radiusJoints = stdVectorToEigenVector(
            obstacleAvoidanceParameters["radiusJoints"].get<vector<double> >());
    this->finalLinkType = obstacleAvoidanceParameters["finalLinkType"].get<std::string>();
    this->finalLinkDimensions = stdVectorToEigenVector(
            obstacleAvoidanceParameters.at("finalLinkDimensions").get<vector<double> >());
    this->bConsiderFinalLink = obstacleAvoidanceParameters.at("considerFinalLinkOA").get<bool>();
    this->bConsiderBaseToFirstJoint = obstacleAvoidanceParameters.at("considerBaseToFirstJoint").get<bool>();
    this->bConsiderRobotLinkAsLineSwept = obstacleAvoidanceParameters.at("robotLinkAsLineSwept").get<bool>();
    this->distanceActivateOA = obstacleAvoidanceParameters.at("distanceActivate").get<double>();
    this->distanceStopOA = obstacleAvoidanceParameters.at("distanceStop").get<double>();
    this->smootheningCoefficient = obstacleAvoidanceParameters.at("smootheningCoefficient").get<double>();
    this->distanceBuffer = obstacleAvoidanceParameters.at("distanceBuffer").get<double>();
    this->convergenceBeta = obstacleAvoidanceParameters.at("convergenceBetaDynamicObstacleAvoidance").get<double>();

    this->doSelfCollisionAvoidance = obstacleAvoidanceParameters.at("selfCollision").get<bool>();

    auto Config_joints = config.getSubConfig("Joints");
    this->joints = make_shared<Joints>(Config_joints);

}


VectorXd Robot::getRobotJointValues() const {
    return this->joints->values;
}

AndreiUtils::Posed Robot::getWorldInBaseFrameRobot() const {
    return this->worldInBaseFrameRobot;
}

VectorXd Robot::getCurrentRobotJointValues() const {
    return this->joints->getCurrentJointValues();
}


void Robot::setJointValues(VectorXd const &jointValues) const {
    this->joints->setJointValues(jointValues);
}


int Robot::getNumberJoints() const {
    return this->joints->number_joints;
}

const shared_ptr<Joints> &Robot::getJoints() const {
    return joints;
}

Eigen::MatrixXd Robot::jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const {
    const int numJoints = static_cast<int>(jointValues.size()); // Number of joints
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, numJoints); // Initialize Jacobian matrix with zeros

    // Compute the transformation to the end-effector or toIthLink
    Eigen::Matrix4d transformEndEffector = fkmCartesian(jointValues, toIthLink);
    const Eigen::Vector3d iLinkPosition = transformEndEffector.block<3, 1>(0, 3);


    for (int i = 1; i <= toIthLink; ++i) {
        // Get the transformation matrix from the base frame to the ith joint

        Eigen::Matrix4d ti = fkmCartesian(jointValues, i);

        // Extract z-axis of the ith joint in base frame coordinates
        Eigen::Vector3d zi = ti.block<3, 3>(0, 0) * Eigen::Vector3d(0, 0, 1);

        // Position of the ith joint in base frame coordinates
        Eigen::Vector3d di = ti.block<3, 1>(0, 3);

        // Linear velocity contribution: cross product of z-axis with the vector from joint i to the end-effector
        const Eigen::Vector3d jvI = zi.cross(iLinkPosition - di);

        // Angular velocity contribution: simply the z-axis for revolute joints
        const Eigen::Vector3d &jwI = zi;

        // Fill in the ith column of the Jacobian matrix for joint i
        J.block<3, 1>(0, i - 1) = jvI; // Upper part for linear velocity
        J.block<3, 1>(3, i - 1) = jwI; // Lower part for angular velocity
    }

    // Columns of Jacobian corresponding to joints beyond toIthLink are already initialized to zero
    return J;
}

Eigen::MatrixXd Robot::jacobianCartesianOnLink(VectorXd const &jointValues, int const &toIthLink,
                                               Matrix4d const &transformationRelative) const {
    const int numJoints = static_cast<int>(jointValues.size()); // Number of joints
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, numJoints); // Initialize Jacobian matrix with zeros

    // Compute the transformation of a point on the toIthLink in global frame

    Eigen::Matrix4d transformPointOnIthLink = fkmCartesian(jointValues, toIthLink) * transformationRelative;
    Eigen::Vector3d pointOnIthLink = transformPointOnIthLink.block<3, 1>(0, 3);


    for (int i = 1; i <= toIthLink; ++i) {
        // Get the transformation matrix from the base frame to the ith joint

        Eigen::Matrix4d ti = fkmCartesian(jointValues, i);

        // Extract the z-axis of the ith joint in base frame coordinates
        Eigen::Vector3d zi = ti.block<3, 3>(0, 0) * Eigen::Vector3d(0, 0, 1);

        // Position of the ith joint in base frame coordinates
        Eigen::Vector3d di = ti.block<3, 1>(0, 3);

        // Linear velocity contribution: cross product of z-axis with the vector from joint i to the end-effector
        const Eigen::Vector3d jvI = zi.cross(pointOnIthLink - di);

        // Angular velocity contribution: simply the z-axis for revolute joints
        const Eigen::Vector3d &jwI = zi;

        // Fill in the ith column of the Jacobian matrix for joint i
        J.block<3, 1>(0, i - 1) = jvI; // Upper part for linear velocity
        J.block<3, 1>(3, i - 1) = jwI; // Lower part for angular velocity
    }

    // Columns of Jacobian corresponding to joints beyond toIthLink are already initialized to zero
    return J;
}

Eigen::MatrixXd Robot::jacobianCartesianTCP(const VectorXd &jointValues) const {
    return jacobianCartesianOnLink(jointValues, static_cast<int>(this->joints->number_joints),
                                   this->transformationEEToTCP);
}

Eigen::Matrix4d Robot::transformMdh(double const &a, double const &alpha, double const &d, double const &theta) {
    Eigen::Matrix4d transform;

    transform << std::cos(theta), -std::sin(theta), 0, a,
            std::sin(theta) * std::cos(alpha), std::cos(theta) * std::cos(alpha), -std::sin(alpha), -std::sin(alpha) *
                                                                                                    d,
            std::sin(theta) * std::sin(alpha), std::cos(theta) * std::sin(alpha), std::cos(alpha), std::cos(alpha) * d,
            0, 0, 0, 1;

    return transform;
}


Eigen::MatrixXd Robot::fkmCartesian(Eigen::VectorXd const &joint_positions, int const &ith_link) const {
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();
    for (int i = 0; i < ith_link; i++) {
        double d = this->mdhMatrix(i, 1);
        double a = this->mdhMatrix(i, 2);
        double alpha = deg2Rad(this->mdhMatrix(i, 3)); // convert to radians
        double theta = joint_positions(i);
        Eigen::Matrix4d Ti;
        Ti = transformMdh(a, alpha, d, theta);
        T = T * Ti; // Matrix Multiplication
    }
    return T;
}

Eigen::MatrixXd Robot::fkmCartesianTCP(const Eigen::VectorXd &jointValues) const {
    int numJoints = static_cast<int>(this->getNumberJoints());
    return this->fkmCartesian(jointValues, numJoints) * this->transformationEEToTCP;
}


std::pair<FinalLinkRobot, std::vector<LinkSegment> > Robot::createLinkSegments(const Eigen::VectorXd &jointValues) {
    int num_links = static_cast<int>(mdhMatrix.rows());
    vector<LinkSegment> linkSegments(num_links);

    FinalLinkRobot finalLink;
    finalLink.type = this->finalLinkType;


    Eigen::MatrixXd prevTransform;

    for (int i = 1; i <= num_links; ++i) {
        if (i == 1)
            prevTransform = Eigen::MatrixXd::Identity(4, 4);
        else
            prevTransform = fkmCartesian(jointValues, i - 1);


        Eigen::MatrixXd aTransform = Eigen::MatrixXd::Identity(4, 4);

        //if a parameter is not zero
        if (mdhMatrix(i - 1, 2) != 0) {
            linkSegments[i - 1].aSegmentV0 = prevTransform.block<3, 1>(0, 3);
            aTransform = prevTransform * trvec2tform({mdhMatrix(i - 1, 2), 0, 0});
            linkSegments[i - 1].aSegmentV1 = aTransform.block<3, 1>(0, 3);
        }

        if (mdhMatrix(i - 1, 1) != 0) {
            Eigen::MatrixXd dTransform;
            if (mdhMatrix(i - 1, 2) != 0) {
                linkSegments[i - 1].dSegmentV0 = linkSegments[i - 1].aSegmentV1;
                dTransform = aTransform * convertEulerToTransform({deg2Rad(mdhMatrix(i - 1, 3)), 0, 0})
                             * trvec2tform({0, 0, mdhMatrix(i - 1, 1)});
                linkSegments[i - 1].dSegmentV1 = dTransform(seq(0, 2), 3);
            } else {
                linkSegments[i - 1].dSegmentV0 = prevTransform(seq(0, 2), 3);
                dTransform = prevTransform * convertEulerToTransform({deg2Rad(mdhMatrix(i - 1, 3)), 0, 0})
                             * trvec2tform({0, 0, mdhMatrix(i - 1, 1)});
                linkSegments[i - 1].dSegmentV1 = dTransform(seq(0, 2), 3);
            }
        }
        if (i == num_links) {
            finalLink.finalV0 = this->fkmCartesian(jointValues, num_links)(seq(0, 2), 3);
            Matrix4d tcpTransform = this->fkmCartesianTCP(jointValues);
            finalLink.finalV1 = tcpTransform(seq(0, 2), 3);

            if (finalLink.type == "Box") {
                finalLink.dimensions = this->finalLinkDimensions;
                auto distanceVector = finalLink.finalV1 - finalLink.finalV0;
                finalLink.orientation = Quaterniond(tcpTransform.block<3, 3>(0, 0));
                auto dotProductZ = distanceVector.normalized().dot(tcpTransform.block<3, 1>(0, 2));
                auto dotProductY = distanceVector.normalized().dot(tcpTransform.block<3, 1>(0, 1));
                auto dotProductX = distanceVector.normalized().dot(tcpTransform.block<3, 1>(0, 0));


                if (((dotProductX > 0) || (dotProductX < 0)) && (abs(dotProductY) < kAxisAlignmentTolerance) &&
                    (abs(dotProductZ) < kAxisAlignmentTolerance)) {
                    finalLink.center =
                            finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[0] / 2);
                    finalLink.directionPartialFinalLink = "X";
                }
                if ((abs(dotProductX) < kAxisAlignmentTolerance) && ((dotProductY > 0) || (dotProductY < 0)) &&
                    (abs(dotProductZ) < kAxisAlignmentTolerance)) {
                    finalLink.center =
                            finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[1] / 2);
                    finalLink.directionPartialFinalLink = "Y";
                }
                if ((abs(dotProductX) < kAxisAlignmentTolerance) && (abs(dotProductY) < kAxisAlignmentTolerance) &&
                    ((dotProductZ > 0) || (dotProductZ < 0))) {
                    finalLink.center =
                            finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[2] / 2);
                    finalLink.directionPartialFinalLink = "Z";
                }
            }
            if (finalLink.type == "Sphere") {
                finalLink.dimensions = this->finalLinkDimensions;
                finalLink.center = finalLink.finalV0;
            }
        }


        linkSegments[i - 1].radiusLink = this->radiusLinks[i - 1];
        linkSegments[i - 1].radiusJoint = this->radiusJoints[i - 1];
    }

    return std::make_pair(finalLink, linkSegments);
}


std::tuple<Eigen::MatrixXd, Eigen::VectorXd, double> Robot::obstacleAvoidanceEquation(
        std::map<std::string, Obstacle> const &obstaclesMap, Eigen::VectorXd const &jointAngles,
        Eigen::VectorXd const &jointVelocityOA) {
    double minimumDistance = std::numeric_limits<double>::infinity();

    int numberLinksRobot = this->getNumberJoints();

    auto [finalLinkTCP, robotLink] = this->createLinkSegments(jointAngles);


    if (this->bConsiderFinalLink == true) {
        numberLinksRobot = numberLinksRobot + 1;
    }
    int numberJoints = this->getNumberJoints();

    setObstacleMap(obstaclesMap);

    //self collision avoidance: add all the links as obstacles
    if (this->doSelfCollisionAvoidance) {
        if (this->bConsiderBaseToFirstJoint) {
            if (robotLink[0].aSegmentV0.size() > 0) {
                obstaclesDynamicMap.insert_or_assign("RobotLinkASegment_0", Obstacle(robotLink[0].aSegmentV0,
                                                                                     robotLink[0].aSegmentV1,
                                                                                     robotLink[0].radiusLink));
            }
            if (robotLink[0].dSegmentV0.size() > 0) {
                obstaclesDynamicMap.insert_or_assign("RobotLinkDSegment_0", Obstacle(robotLink[0].dSegmentV0,
                                                                                     robotLink[0].dSegmentV1,
                                                                                     robotLink[0].radiusLink));
            }
        }
        for (int i = 1; i < robotLink.size() - 1; i++) {
            if (robotLink[i].aSegmentV0.size() > 0) {
                obstaclesDynamicMap.insert_or_assign("RobotLinkASegment_" + std::to_string(i),
                                                     Obstacle(robotLink[i].aSegmentV0,
                                                              robotLink[i].aSegmentV1,
                                                              robotLink[i].radiusLink));
            }
            if (robotLink[i].dSegmentV0.size() > 0) {
                obstaclesDynamicMap.insert_or_assign("RobotLinkDSegment_" + std::to_string(i),
                                                     Obstacle(robotLink[i].dSegmentV0,
                                                              robotLink[i].dSegmentV1,
                                                              robotLink[i].radiusLink));
            }
        }
    }


    // TODO: if bConsiderBaseToFirstJoint == false, add first link to the obstacles
    if (!this->bConsiderBaseToFirstJoint) {
        if (robotLink[0].aSegmentV0.size() > 0) {
            obstaclesDynamicMap.insert_or_assign("RobotLinkASegment_0", Obstacle(robotLink[0].aSegmentV0,
                                                                                 robotLink[0].aSegmentV1,
                                                                                 robotLink[0].radiusLink));
        }
        if (robotLink[0].dSegmentV0.size() > 0) {
            obstaclesDynamicMap.insert_or_assign("RobotLinkDSegment_0", Obstacle(robotLink[0].dSegmentV0,
                                                                                 robotLink[0].dSegmentV1,
                                                                                 robotLink[0].radiusLink));
        }
    }

    synchronizeMaps(this->obstaclesDynamicMap);

    int maxEntries = 2 * static_cast<int>(obstaclesDynamicMap.size()) * numberLinksRobot;

    Eigen::MatrixXd j0 = Eigen::MatrixXd::Zero(maxEntries, numberJoints);
    Eigen::VectorXd b0 = Eigen::VectorXd::Zero(maxEntries);

    int jCounter = 0;
    int bCounter = 0;

    if (!obstaclesDynamicMap.empty()) {
        // auto robotLinkIndex = AndreiUtils::stringToInteger(AndreiUtils::splitString().back());
        for (auto &[obstacleId, obstacleFromMap]: obstaclesDynamicMap) {
            auto key = obstacleId;
            int startIndex;
            auto keySplit = AndreiUtils::splitString(key, "_");
            if (keySplit[0] == "RobotLinkASegment" || keySplit[0] == "RobotLinkDSegment") {
                int linkObstacleIndex = AndreiUtils::stringToInteger(keySplit.back());
                if (linkObstacleIndex < numberJoints - 1) {
                    if ((robotLink[linkObstacleIndex + 1].aSegmentV0.size() != 0) || (
                            robotLink[linkObstacleIndex + 1].dSegmentV0.size() != 0)) {
                        startIndex = linkObstacleIndex + 2;
                        //ToDO: consider a segment collisions as well
                        //
                        if (startIndex < numberJoints) {
                            if (robotLink[startIndex].dSegmentV1.size() != 0) {
                                if (robotLink[linkObstacleIndex].dSegmentV1.size() != 0) {
                                    if (((robotLink[startIndex].dSegmentV0 -
                                          robotLink[linkObstacleIndex].dSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) &&
                                        ((robotLink[startIndex].dSegmentV1 -
                                          robotLink[linkObstacleIndex].dSegmentV1).norm() > kSelfCollisionAdjacencyThreshold)) {
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      false,
                                                                                                      true,
                                                                                                      false
                                                                                              });
                                    } else if ((robotLink[startIndex].dSegmentV1 -
                                                robotLink[linkObstacleIndex].dSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) {
                                        startIndex += 1;
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      true, true,
                                                                                                      false
                                                                                              });
                                    }
                                } else if (robotLink[linkObstacleIndex].aSegmentV1.size() != 0) {
                                    if (((robotLink[startIndex].dSegmentV0 -
                                          robotLink[linkObstacleIndex].aSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) &&
                                        ((robotLink[startIndex].dSegmentV1 -
                                          robotLink[linkObstacleIndex].aSegmentV1).norm() > kSelfCollisionAdjacencyThreshold)) {
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      false,
                                                                                                      true,
                                                                                                      false
                                                                                              });
                                    } else if ((robotLink[startIndex].dSegmentV1 -
                                                robotLink[linkObstacleIndex].aSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) {
                                        startIndex += 1;
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      true, true,
                                                                                                      false
                                                                                              });
                                    }
                                }
                            } else if (robotLink[startIndex].aSegmentV1.size() != 0) {
                                if (robotLink[linkObstacleIndex].dSegmentV1.size() != 0) {

                                    if (((robotLink[startIndex].aSegmentV0 -
                                          robotLink[linkObstacleIndex].dSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) &&
                                        ((robotLink[startIndex].aSegmentV1 -
                                          robotLink[linkObstacleIndex].dSegmentV1).norm() > kSelfCollisionAdjacencyThreshold)) {
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      true,
                                                                                                      false,
                                                                                                      false
                                                                                              });
                                    } else if ((robotLink[startIndex].aSegmentV1 -
                                                robotLink[linkObstacleIndex].dSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) {
                                        startIndex += 1;
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      true, true,
                                                                                                      false
                                                                                              });
                                    }
                                } else if (robotLink[linkObstacleIndex].aSegmentV1.size() != 0) {
                                    if (((robotLink[startIndex].aSegmentV0 -
                                          robotLink[linkObstacleIndex].aSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) &&
                                        ((robotLink[startIndex].aSegmentV1 -
                                          robotLink[linkObstacleIndex].aSegmentV1).norm() > kSelfCollisionAdjacencyThreshold)) {
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      true,
                                                                                                      false,
                                                                                                      false
                                                                                              });
                                    } else if ((robotLink[startIndex].aSegmentV1 -
                                                robotLink[linkObstacleIndex].aSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) {
                                        startIndex += 1;
                                        obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                              linkProperty{
                                                                                                      true, true,
                                                                                                      false
                                                                                              });
                                    }
                                }

                            }
                        }
                    } else if (robotLink[linkObstacleIndex + 1].aSegmentV0.size() == 0 &&
                               robotLink[linkObstacleIndex + 1].dSegmentV0.size() == 0) {
                        startIndex = linkObstacleIndex + 3;


                        if (startIndex == numberJoints) {
                            if (robotLink[linkObstacleIndex].dSegmentV1.size() != 0) {
                                if ((this->fkmCartesian(jointAngles, startIndex)(seq(0, 2), 3) -
                                     robotLink[linkObstacleIndex].dSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) {
                                    obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                          linkProperty{
                                                                                                  false,
                                                                                                  false,
                                                                                                  true
                                                                                          });
                                }
                            } else if (robotLink[linkObstacleIndex].aSegmentV1.size() != 0) {
                                if ((this->fkmCartesian(jointAngles, startIndex)(seq(0, 2), 3) -
                                     robotLink[linkObstacleIndex].aSegmentV1).norm() < kSelfCollisionAdjacencyThreshold) {
                                    obstacleFromMap.partialSelfCollision.insert_or_assign(startIndex,
                                                                                          linkProperty{
                                                                                                  false, false,
                                                                                                  true
                                                                                          });
                                }
                            }
                        }
                    }
                }
            } else {
                startIndex = !bConsiderBaseToFirstJoint;
            }


            obstacleFromMap.startIndex = startIndex;

            for (int i = startIndex; i < numberLinksRobot; ++i) {


                //Delete Critical points if distance becomes greater

                if (this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointA && (this->criticalPointsDynamicMap[key][i].distanceA >
                                              (this->distanceActivateOA + this->distanceBuffer))) {


                    deleteCriticalPoint(this->criticalPointsDynamicMap[key][i], true, false, false);
                }

                if (this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointD &&
                    (this->criticalPointsDynamicMap[key][i].distanceD >
                     (this->distanceActivateOA + this->distanceBuffer))) {
                    deleteCriticalPoint(this->criticalPointsDynamicMap[key][i], false, true, false);
                }

                if (this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointFinalLink &&
                    (this->criticalPointsDynamicMap[key][i].
                            distanceFinalLink >
                     (this->distanceActivateOA + this->distanceBuffer))) {
                    deleteCriticalPoint(this->criticalPointsDynamicMap[key][i], false, false, true);
                }


                if (this->criticalPointsDynamicMap[key][i].hasCriticalPointA) {
                    double distance;
                    Vector3d closestPointObstacle, closestPointLink;

                    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) &&
                        (!obstacleFromMap.partialSelfCollision.empty()) &&
                        (obstacleFromMap.partialSelfCollision.count(i))) {
                        if (!obstacleFromMap.partialSelfCollision[i].useLinkSegmentA) {
                            distance = std::numeric_limits<double>::infinity();
                        } else {
                            auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                    robotLink[i].aSegmentV0,
                                    robotLink[i].aSegmentV1,
                                    robotLink[i].radiusLink,
                                    robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept,
                                    obstacleFromMap.partialSelfCollision[i].useLinkSegmentA);
                            distance = std::get<0>(resultDistanceCalculation);
                            closestPointObstacle = std::get<1>(resultDistanceCalculation);
                            closestPointLink = std::get<2>(resultDistanceCalculation);
                        }
                    } else {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                robotLink[i].aSegmentV0,
                                robotLink[i].aSegmentV1,
                                robotLink[i].radiusLink,
                                robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept, false);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                    Eigen::MatrixXd jacobiCriticalPoint;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, i);

                    jacobiCriticalPoint = get<1>(result);

                    this->criticalPointsDynamicMap[key][i].
                            distVectorA = closestPointLink - closestPointObstacle;


                    this->criticalPointsDynamicMap[key][i].
                            jacobiCriticalA = jacobiCriticalPoint(seq(0, 2), all);

                    this->criticalPointsDynamicMap[key][i].distanceA = distance;

                    this->criticalPointsDynamicMap[key][i].
                            jacobiDistanceA =
                            (-(this->criticalPointsDynamicMap[key][i].
                                    distVectorA.normalized())).transpose() *
                            this->criticalPointsDynamicMap[key][i].jacobiCriticalA;


                    criticalPointsDynamicMap[key][i].closestPointObstacleA = closestPointObstacle;
                    criticalPointsDynamicMap[key][i].jointAnglesCriticalA = jointAngles;
                    criticalPointsDynamicMap[key][i].jointVelocityCurrentA = jointVelocityOA;

                    auto [bSecond, lambda] = calculateDynamicSelfCollisionParameters(criticalPointsDynamicMap[key][i],
                                                                                     keySplit, false, false, true,
                                                                                     false);

                    bSecond = this->computeB0(bSecond, this->criticalPointsDynamicMap[key][i].distanceA);
                    double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxA *
                                     this->criticalPointsDynamicMap[key][i].jointVelocityCriticalA).value();
                    bFirst = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceA);
                    b0(bCounter) = (bFirst * lambda) + ((1 - lambda) * bSecond);
                    bCounter += 1;

                    j0(jCounter, all) = (lambda * this->criticalPointsDynamicMap[key][i].jacobiDistanceA) + (
                            (1 - lambda) * this->criticalPointsDynamicMap[key][i].
                                    jacobiDistanceDynamicA);

                    jCounter += 1;
                }

                if (this->criticalPointsDynamicMap[key][i].hasCriticalPointD) {
                    double distance;
                    Vector3d closestPointObstacle, closestPointLink;

                    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) &&
                        (!obstacleFromMap.partialSelfCollision.empty()) &&
                        (obstacleFromMap.partialSelfCollision.count(i))) {
                        if (!obstacleFromMap.partialSelfCollision[i].useLinkSegmentD) {
                            distance = std::numeric_limits<double>::infinity();
                        } else {
                            auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                    robotLink[i].dSegmentV0, robotLink[i].dSegmentV1, robotLink[i].radiusLink,
                                    robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept,
                                    obstacleFromMap.partialSelfCollision[i].useLinkSegmentD);
                            distance = std::get<0>(resultDistanceCalculation);
                            closestPointObstacle = std::get<1>(resultDistanceCalculation);
                            closestPointLink = std::get<2>(resultDistanceCalculation);
                        }
                    } else {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                robotLink[i].dSegmentV0, robotLink[i].dSegmentV1, robotLink[i].radiusLink,
                                robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept, false);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                    Vector3d distanceVector = closestPointLink - closestPointObstacle;

                    Eigen::MatrixXd jacobiCriticalPoint;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, i);

                    jacobiCriticalPoint = get<1>(result);


                    this->criticalPointsDynamicMap[key][i].
                            jacobiCriticalD = jacobiCriticalPoint(seq(0, 2), all);

                    this->criticalPointsDynamicMap[key][i].
                            distVectorD = distanceVector;
                    this->criticalPointsDynamicMap[key][i].distanceD = distance;

                    this->criticalPointsDynamicMap[key][i].
                            jacobiDistanceD =
                            (-(this->criticalPointsDynamicMap[key][i].
                                    distVectorD.normalized())).transpose() *
                            this->criticalPointsDynamicMap[key][i].
                                    jacobiCriticalD;

                    criticalPointsDynamicMap[key][i].closestPointObstacleD = closestPointObstacle;
                    criticalPointsDynamicMap[key][i].jointAnglesCriticalD = jointAngles;
                    criticalPointsDynamicMap[key][i].jointVelocityCurrentD = jointVelocityOA;

                    auto [bSecond, lambda] = calculateDynamicSelfCollisionParameters(criticalPointsDynamicMap[key][i],
                                                                                     keySplit, false, false, false,
                                                                                     true);

                    bSecond = this->computeB0(bSecond, this->criticalPointsDynamicMap[key][i].distanceD);

                    double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxD *
                                     this->criticalPointsDynamicMap[key][i].jointVelocityCriticalD).value();
                    bFirst = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].
                            distanceD);
                    b0(bCounter) = lambda * bFirst + ((1 - lambda) * bSecond);
                    bCounter += 1;

                    j0(jCounter, all) = lambda * this->criticalPointsDynamicMap[key][i].jacobiDistanceD + (1 - lambda) *
                                                                                                          this->criticalPointsDynamicMap[key][i].jacobiDistanceDynamicD;
                    jCounter += 1;

                }

                if (this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointFinalLink && bConsiderFinalLink &&
                    i == numberLinksRobot - 1) {
                    double distance;
                    Vector3d closestPointObstacle, closestPointLink;
                    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) &&
                        (!obstacleFromMap.partialSelfCollision.empty()) &&
                        (obstacleFromMap.partialSelfCollision.count(i))) {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceFinalLinkObstacle(
                                finalLinkTCP, obstacleFromMap.partialSelfCollision[i].useFinalLink);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    } else {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceFinalLinkObstacle(
                                finalLinkTCP, false);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }

                    Vector3d distanceVector = closestPointLink - closestPointObstacle;

                    Eigen::MatrixXd jacobiCriticalPoint;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, numberJoints);

                    jacobiCriticalPoint = get<1>(result);

                    this->criticalPointsDynamicMap[key][i].
                            jacobiCriticalFinalLink = jacobiCriticalPoint(seq(0, 2), all);

                    this->criticalPointsDynamicMap[key][i].
                            distVectorFinalLink = distanceVector;

                    this->criticalPointsDynamicMap[key][i].distanceFinalLink = distance;

                    this->criticalPointsDynamicMap[key][i].
                            jacobiDistanceFinalLink =
                            (-(this->criticalPointsDynamicMap[key][i].
                                    distVectorFinalLink.normalized())).transpose() *
                            this->criticalPointsDynamicMap[key][i].jacobiCriticalFinalLink;


                    criticalPointsDynamicMap[key][i].closestPointObstacleFinalLink = closestPointObstacle;
                    criticalPointsDynamicMap[key][i].jointAnglesCriticalFinalLink = jointAngles;
                    criticalPointsDynamicMap[key][i].jointVelocityCurrentFinalLink = jointVelocityOA;

                    auto [bSecond, lambda] = calculateDynamicSelfCollisionParameters(criticalPointsDynamicMap[key][i],
                                                                                     keySplit, false, true, false,
                                                                                     false);

                    bSecond = this->computeB0(bSecond, this->criticalPointsDynamicMap[key][i].distanceFinalLink);


                    double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxFinalLink *
                                     this->criticalPointsDynamicMap[key][i].jointVelocityCriticalFinalLink).value();
                    bFirst = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceFinalLink);
                    b0(bCounter) = lambda * bFirst + ((1 - lambda) * bSecond);
                    bCounter += 1;

                    j0(jCounter, all) =
                            lambda * this->criticalPointsDynamicMap[key][i].jacobiDistanceFinalLink + (1 - lambda) *
                                                                                                      this->criticalPointsDynamicMap[key][i].
                                                                                                              jacobiDistanceDynamicFinalLink;
                    jCounter += 1;
                }

                if (!this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointFinalLink && bConsiderFinalLink &&
                    i == numberLinksRobot - 1) {
                    double distance;
                    Vector3d closestPointObstacle, closestPointLink;
                    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) &&
                        (!obstacleFromMap.partialSelfCollision.empty()) &&
                        (obstacleFromMap.partialSelfCollision.count(i))) {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceFinalLinkObstacle(
                                finalLinkTCP, obstacleFromMap.partialSelfCollision[i].useFinalLink);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    } else {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceFinalLinkObstacle(
                                finalLinkTCP, false);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                    Vector3d distanceVector = closestPointLink - closestPointObstacle;
                    this->criticalPointsDynamicMap[key][i].distance = distance;


                    if (distance <= this->distanceActivateOA + this->distanceBuffer) {
                        // cout << "distance Final Link upon entering critical Zone: " << distance << endl;
                        Eigen::MatrixXd jacobiCriticalPoint;
                        Eigen::Matrix4d relativeTransformation;
                        auto result = criticalPointInformation(jointAngles, closestPointLink, numberJoints);
                        relativeTransformation = get<0>(result);
                        jacobiCriticalPoint = get<1>(result);
                        this->criticalPointsDynamicMap[key][i].distanceFinalLink = distance;
                        this->criticalPointsDynamicMap[key][i].
                                hasCriticalPointFinalLink = true;
                        this->criticalPointsDynamicMap[key][i].distVectorFinalLink = distanceVector;
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxFinalLink = jacobiCriticalPoint(
                                seq(0, 2), all);
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalFinalLink =
                                jacobiCriticalPoint(seq(0, 2), all);
                        this->criticalPointsDynamicMap[key][i].jointAnglesCriticalFinalLink = jointAngles;
                        this->criticalPointsDynamicMap[key][i].
                                jacobiDistMaxFinalLink =
                                (-(this->criticalPointsDynamicMap[key][i].distVectorFinalLink.normalized())).transpose()
                                *
                                this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxFinalLink;
                        this->criticalPointsDynamicMap[key][i].
                                jacobiDistanceFinalLink =
                                (-(this->criticalPointsDynamicMap[key][i].distVectorFinalLink.normalized())).transpose()
                                *
                                this->criticalPointsDynamicMap[key][i].
                                        jacobiCriticalFinalLink;
                        this->criticalPointsDynamicMap[key][i].closestPointObstacleFinalLink = closestPointObstacle;
                        this->criticalPointsDynamicMap[key][i].closestPointRelativeFinalLink = relativeTransformation;
                        this->criticalPointsDynamicMap[key][i].jointVelocityCriticalFinalLink = jointVelocityOA;
                        this->criticalPointsDynamicMap[key][i].jointVelocityCurrentFinalLink = jointVelocityOA;


                        auto [bSecond, lambda] = calculateDynamicSelfCollisionParameters(
                                criticalPointsDynamicMap[key][i],
                                keySplit, true, true, false, false);

                        bSecond = this->computeB0(bSecond, this->criticalPointsDynamicMap[key][i].distanceFinalLink);

                        //calculation of lambda

                        double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxFinalLink *
                                         this->criticalPointsDynamicMap[key][i].jointVelocityCriticalFinalLink).value();
                        bFirst = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceFinalLink);


                        b0(bCounter) = lambda * bFirst + (1 - lambda) * bSecond;
                        j0(jCounter, all) = lambda * this->criticalPointsDynamicMap[key][i].jacobiDistanceFinalLink +
                                            (1 - lambda) * this->criticalPointsDynamicMap[key][i].
                                                    jacobiDistanceDynamicFinalLink;
                        bCounter += 1;
                        jCounter += 1;
                    }
                }

                if ((!this->criticalPointsDynamicMap[key][i].hasCriticalPointA) && (i < numberJoints) &&
                    (robotLink[i].aSegmentV0.size() != 0)) {
                    double distance;
                    Vector3d closestPointObstacle, closestPointLink;
                    Vector3d distanceVector;


                    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) &&
                        (!obstacleFromMap.partialSelfCollision.empty()) &&
                        (obstacleFromMap.partialSelfCollision.count(i))) {
                        if (!obstacleFromMap.partialSelfCollision[i].useLinkSegmentA) {
                            distance = std::numeric_limits<double>::infinity();
                        } else {
                            auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                    robotLink[i].aSegmentV0,
                                    robotLink[i].aSegmentV1,
                                    robotLink[i].radiusLink,
                                    robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept,
                                    obstacleFromMap.partialSelfCollision[i].useLinkSegmentA);
                            distance = std::get<0>(resultDistanceCalculation);
                            closestPointObstacle = std::get<1>(resultDistanceCalculation);
                            closestPointLink = std::get<2>(resultDistanceCalculation);
                        }
                    } else {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                robotLink[i].aSegmentV0,
                                robotLink[i].aSegmentV1,
                                robotLink[i].radiusLink,
                                robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept, false);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }


                    distanceVector = closestPointLink - closestPointObstacle;
                    this->criticalPointsDynamicMap[key][i].distance = distance;
                    if (distance <= this->distanceActivateOA + this->distanceBuffer) {


                        this->criticalPointsDynamicMap[key][i].distanceA = distance;


                        Eigen::MatrixXd jacobiCriticalPoint;
                        Eigen::Matrix4d relativeTransformation;


                        auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                        relativeTransformation = get<0>(result);
                        jacobiCriticalPoint = get<1>(result);
                        this->criticalPointsDynamicMap[key][i].hasCriticalPointA = true;
                        this->criticalPointsDynamicMap[key][i].distVectorA = distanceVector;
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxA = jacobiCriticalPoint(seq(0, 2), all);
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalA = jacobiCriticalPoint(seq(0, 2), all);
                        this->criticalPointsDynamicMap[key][i].jointAnglesCriticalA = jointAngles;
                        this->criticalPointsDynamicMap[key][i].jacobiDistMaxA =
                                (-(this->criticalPointsDynamicMap[key][i].distVectorA.normalized())).transpose() *
                                this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxA;
                        this->criticalPointsDynamicMap[key][i].jacobiDistanceA =
                                (-(this->criticalPointsDynamicMap[key][i].distVectorA.normalized())).transpose() *
                                this->criticalPointsDynamicMap[key][i].jacobiCriticalA;
                        this->criticalPointsDynamicMap[key][i].closestPointObstacleA = closestPointObstacle;
                        this->criticalPointsDynamicMap[key][i].closestPointRelativeRobotLinkA = relativeTransformation;
                        this->criticalPointsDynamicMap[key][i].jointVelocityCriticalA = jointVelocityOA;
                        this->criticalPointsDynamicMap[key][i].jointVelocityCurrentA = jointVelocityOA;
                        auto [bSecond, lambda] = calculateDynamicSelfCollisionParameters(
                                criticalPointsDynamicMap[key][i],
                                keySplit, true, false, true, false);

                        bSecond = this->computeB0(bSecond, this->criticalPointsDynamicMap[key][i].distanceA);


                        double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxA *
                                         this->criticalPointsDynamicMap[key][i].jointVelocityCriticalA).value();
                        bFirst = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceA);
                        b0(bCounter) = lambda * bFirst + (1 - lambda) * bSecond;
                        bCounter += 1;

                        j0(jCounter, all) = lambda * this->criticalPointsDynamicMap[key][i].jacobiDistanceA +
                                            (1 - lambda) *
                                            this->criticalPointsDynamicMap[key][i].jacobiDistanceDynamicA;
                        jCounter += 1;
                    }
                }

                if ((!this->criticalPointsDynamicMap[key][i].hasCriticalPointD) && (i < numberJoints) &&
                    (robotLink[i].dSegmentV0.size() != 0)) {
                    double distance;
                    Vector3d closestPointObstacle, closestPointLink;
                    Vector3d distanceVector;
                    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) &&
                        (!obstacleFromMap.partialSelfCollision.empty()) &&
                        (obstacleFromMap.partialSelfCollision.count(i))) {
                        if (!obstacleFromMap.partialSelfCollision[i].useLinkSegmentD) {
                            distance = std::numeric_limits<double>::infinity();
                        } else {
                            auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                    robotLink[i].dSegmentV0, robotLink[i].dSegmentV1, robotLink[i].radiusLink,
                                    robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept,
                                    obstacleFromMap.partialSelfCollision[i].useLinkSegmentD);
                            distance = std::get<0>(resultDistanceCalculation);
                            closestPointObstacle = std::get<1>(resultDistanceCalculation);
                            closestPointLink = std::get<2>(resultDistanceCalculation);
                        }
                    } else {
                        auto resultDistanceCalculation = obstacleFromMap.calculateDistanceRobotLinkObstacle(
                                robotLink[i].dSegmentV0, robotLink[i].dSegmentV1, robotLink[i].radiusLink,
                                robotLink[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept, false);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                    distanceVector = closestPointLink - closestPointObstacle;
                    if ((distance < this->criticalPointsDynamicMap[key][i].distance &&
                         robotLink[i].aSegmentV0.size() != 0) || (robotLink[i].aSegmentV0.size() == 0)) {
                        this->criticalPointsDynamicMap[key][i].distance = distance;
                    }


                    if (distance <= this->distanceActivateOA + this->distanceBuffer) {


                        Eigen::MatrixXd jacobiCriticalPoint;
                        Eigen::Matrix4d relativeTransformation;


                        auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                        relativeTransformation = get<0>(result);
                        jacobiCriticalPoint = get<1>(result);
                        this->criticalPointsDynamicMap[key][i].distanceD = distance;
                        this->criticalPointsDynamicMap[key][i].hasCriticalPointD = true;
                        this->criticalPointsDynamicMap[key][i].distVectorD = distanceVector;
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxD = jacobiCriticalPoint(seq(0, 2), all);
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalD = jacobiCriticalPoint(seq(0, 2), all);
                        this->criticalPointsDynamicMap[key][i].jointAnglesCriticalD = jointAngles;
                        this->criticalPointsDynamicMap[key][i].jacobiDistMaxD =
                                (-(this->criticalPointsDynamicMap[key][i].distVectorD.normalized())).transpose() *
                                this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxD;
                        this->criticalPointsDynamicMap[key][i].jacobiDistanceD =
                                (-(this->criticalPointsDynamicMap[key][i].distVectorD.normalized())).transpose() *
                                this->criticalPointsDynamicMap[key][i].jacobiCriticalD;
                        this->criticalPointsDynamicMap[key][i].closestPointObstacleD = closestPointObstacle;
                        this->criticalPointsDynamicMap[key][i].closestPointRelativeRobotLinkD = relativeTransformation;
                        this->criticalPointsDynamicMap[key][i].jointVelocityCriticalD = jointVelocityOA;
                        this->criticalPointsDynamicMap[key][i].jointVelocityCurrentD = jointVelocityOA;

                        auto [bSecond, lambda] = calculateDynamicSelfCollisionParameters(
                                criticalPointsDynamicMap[key][i],
                                keySplit, true, false, false, true);

                        bSecond = this->computeB0(bSecond, this->criticalPointsDynamicMap[key][i].distanceD);


                        double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxD *
                                         this->criticalPointsDynamicMap[key][i].jointVelocityCriticalD).value();
                        bFirst = computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceD);
                        b0(bCounter) = lambda * bFirst + (1 - lambda) * bSecond;
                        bCounter += 1;

                        j0(jCounter, all) =
                                lambda * this->criticalPointsDynamicMap[key][i].jacobiDistanceD + (1 - lambda) *
                                                                                                  this->criticalPointsDynamicMap[key][i].
                                                                                                          jacobiDistanceDynamicD;
                        jCounter += 1;
                    }
                }

                if (this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointA &&
                    this->criticalPointsDynamicMap[key][i].
                            hasCriticalPointD) {
                    this->criticalPointsDynamicMap[key][i].
                            distance = std::min(
                            this->criticalPointsDynamicMap[key][i].
                                    distanceA,
                            this->criticalPointsDynamicMap[key][i].
                                    distanceD);
                } else if (this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointA &&
                           !this->criticalPointsDynamicMap[key][i].
                                   hasCriticalPointD) {
                    this->criticalPointsDynamicMap[key][i].
                            distance = this->criticalPointsDynamicMap[key][i].
                            distanceA;
                } else if (!this->criticalPointsDynamicMap[key][i].
                        hasCriticalPointA &&
                           this->criticalPointsDynamicMap[key][i].
                                   hasCriticalPointD) {
                    this->criticalPointsDynamicMap[key][i].
                            distance = this->criticalPointsDynamicMap[key][i].
                            distanceD;
                }


                if (this->criticalPointsDynamicMap[key][i].
                        distance < minimumDistance) {
                    minimumDistance = this->criticalPointsDynamicMap[key][i].distance;
                }
            }
        }
    }
    if (jCounter == 0) {
        j0.conservativeResize(jCounter, jCounter);
    } else {
        j0.conservativeResize(jCounter, numberJoints);
    }
    if (std::isinf(minimumDistance)) {
        minimumDistance = std::numeric_limits<double>::quiet_NaN();
    }
    b0.conservativeResize(bCounter);
    return std::make_tuple(j0, b0, minimumDistance);
}


void Robot::deleteCriticalPoint(CriticalPoints &criticalPoint, bool const &bDeleteCriticalA,
                                bool const &bDeleteCriticalD,
                                bool const &bDeleteCriticalFinalLink) const {
    const int numberOfJoints = this->getNumberJoints();
    if (bDeleteCriticalA) {
        criticalPoint.hasCriticalPointA = false;
        criticalPoint.distVectorA = Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalMaxA = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                     std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalA = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                  std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalA = Eigen::VectorXd::Constant(numberOfJoints,
                                                                       std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiDistMaxA = Eigen::MatrixXd::Constant(1, numberOfJoints,
                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiDistanceA = Eigen::MatrixXd::Constant(1, numberOfJoints,
                                                                  std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointObstacleA = Eigen::MatrixXd::Constant(
                3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointRelativeRobotLinkA = Eigen::MatrixXd::Constant(4, 4,
                                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalA = Eigen::VectorXd::Constant(numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());
        criticalPoint.distanceA = std::numeric_limits<double>::quiet_NaN();
        criticalPoint.jacobiCriticalMaxA = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                     std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalA = Eigen::VectorXd::Constant(numberOfJoints,
                                                                       std::numeric_limits<double>::quiet_NaN());

        criticalPoint.jointVelocityCurrentA = Eigen::VectorXd::Constant(
                numberOfJoints, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.obstacleVelocityForLinkA = Eigen::MatrixXd::Constant(3, 1, 0);
        criticalPoint.jacobiDistanceDynamicA = Eigen::MatrixXd::Constant(1, numberOfJoints, 0);
        criticalPoint.jacobiDistanceDynamicMaxA = Eigen::MatrixXd::Constant(1, numberOfJoints, 0);
    }
    if (bDeleteCriticalD) {
        criticalPoint.hasCriticalPointD = false;
        criticalPoint.distVectorD = Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalMaxD = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                     std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalD = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                  std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalD = Eigen::VectorXd::Constant(numberOfJoints,
                                                                       std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiDistMaxD = Eigen::MatrixXd::Constant(1, numberOfJoints,
                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiDistanceD = Eigen::MatrixXd::Constant(1, numberOfJoints,
                                                                  std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointObstacleD = Eigen::MatrixXd::Constant(
                3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointRelativeRobotLinkD = Eigen::MatrixXd::Constant(4, 4,
                                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalD = Eigen::VectorXd::Constant(numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());
        criticalPoint.distanceD = std::numeric_limits<double>::quiet_NaN();
        criticalPoint.jacobiCriticalMaxD = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                     std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalD = Eigen::VectorXd::Constant(numberOfJoints,
                                                                       std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCurrentD = Eigen::VectorXd::Constant(
                numberOfJoints, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.obstacleVelocityForLinkD = Eigen::MatrixXd::Constant(3, 1, 0);
        criticalPoint.jacobiDistanceDynamicD = Eigen::MatrixXd::Constant(1, numberOfJoints, 0);
        criticalPoint.jacobiDistanceDynamicMaxD = Eigen::MatrixXd::Constant(1, numberOfJoints, 0);
    }

    if (bDeleteCriticalFinalLink) {
        criticalPoint.hasCriticalPointFinalLink = false;
        criticalPoint.distVectorFinalLink = Eigen::MatrixXd::Constant(
                3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalMaxFinalLink = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                             std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalFinalLink = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                          std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalFinalLink = Eigen::VectorXd::Constant(numberOfJoints,
                                                                               std::numeric_limits<
                                                                                       double>::quiet_NaN());
        criticalPoint.jacobiDistMaxFinalLink = Eigen::MatrixXd::Constant(1, numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiDistanceFinalLink = Eigen::MatrixXd::Constant(1, numberOfJoints,
                                                                          std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointObstacleFinalLink = Eigen::MatrixXd::Constant(3, 1,
                                                                                std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointRelativeFinalLink = Eigen::MatrixXd::Constant(4, 4,
                                                                                std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalFinalLink = Eigen::VectorXd::Constant(numberOfJoints,
                                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.distanceFinalLink = std::numeric_limits<double>::quiet_NaN();
        criticalPoint.jacobiCriticalMaxFinalLink = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                             std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalFinalLink = Eigen::VectorXd::Constant(numberOfJoints,
                                                                               std::numeric_limits<
                                                                                       double>::quiet_NaN());
        criticalPoint.jointVelocityCurrentFinalLink = Eigen::VectorXd::Constant(
                numberOfJoints, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.obstacleVelocityForFinalLink = Eigen::MatrixXd::Constant(3, 1, 0);
        criticalPoint.jacobiDistanceDynamicFinalLink = Eigen::MatrixXd::Constant(1, numberOfJoints, 0);
        criticalPoint.jacobiDistanceDynamicMaxFinalLink = Eigen::MatrixXd::Constant(1, numberOfJoints, 0);
    }
}

std::tuple<double, double> Robot::calculateDynamicSelfCollisionParameters(
        CriticalPoints &criticalPoint, std::vector<string> const &keySplit, bool const &enterCriticalZone,
        bool const &forFinalLink, bool const &forLinkA, bool const &forLinkD) {
    if (((keySplit.front() == "RobotLinkASegment") || (keySplit.front() == "RobotLinkDSegment")) && this->
            doSelfCollisionAvoidance) {
        const int linkObstacleIndex = AndreiUtils::stringToInteger(keySplit.back());

        if (forFinalLink && !forLinkA && !forLinkD) {
            const auto resultObstacle = criticalPointInformation(
                    criticalPoint.jointAnglesCriticalFinalLink,
                    criticalPoint.closestPointObstacleFinalLink,
                    linkObstacleIndex);
            const MatrixXd jacobiObstaclePoint = get<1>(resultObstacle);
            criticalPoint.obstacleVelocityForFinalLink = jacobiObstaclePoint(seq(0, 2), all) *
                                                         criticalPoint.jointVelocityCurrentFinalLink;
            //projection on this vector
            // = this->criticalPointsDynamicMap[key][i].obstacleVelocityFinalLink * distanceVector.normalized();
            auto [vObstacleParallel, vObstaclePerpendicular] = decomposeVector(
                    criticalPoint.obstacleVelocityForFinalLink, criticalPoint.distVectorFinalLink);
            criticalPoint.jacobiDistanceDynamicFinalLink =
                    (vObstaclePerpendicular.normalized()).transpose() *
                    criticalPoint.jacobiCriticalFinalLink;

            if (enterCriticalZone) {
                criticalPoint.jacobiDistanceDynamicMaxFinalLink =
                        (vObstaclePerpendicular.normalized()).transpose() *
                        criticalPoint.
                                jacobiCriticalFinalLink;
            }
            double bSecond = (criticalPoint.jacobiDistanceDynamicMaxFinalLink *
                              criticalPoint.jointVelocityCriticalFinalLink).value();

            bSecond = computeB0(bSecond, criticalPoint.distanceFinalLink);
            double lambda = std::exp(
                    -convergenceBeta * criticalPoint.distanceFinalLink *
                    criticalPoint.obstacleVelocityForFinalLink.norm());

            return std::make_tuple(bSecond, lambda);
        } else if (forLinkA && !forLinkD && !forFinalLink) {
            const auto resultObstacle = criticalPointInformation(
                    criticalPoint.jointAnglesCriticalA,
                    criticalPoint.closestPointObstacleA,
                    linkObstacleIndex);

            const MatrixXd jacobiObstaclePoint = get<1>(resultObstacle);
            criticalPoint.obstacleVelocityForLinkA = jacobiObstaclePoint(seq(0, 2), all) *
                                                     criticalPoint.jointVelocityCurrentA;
            //projection on this vector
            // = this->criticalPointsDynamicMap[key][i].obstacleVelocityFinalLink * distanceVector.normalized();
            auto [vObstacleParallel, vObstaclePerpendicular] = decomposeVector(
                    criticalPoint.obstacleVelocityForLinkA, criticalPoint.distVectorA);
            criticalPoint.jacobiDistanceDynamicA =
                    (vObstaclePerpendicular.normalized()).transpose() *
                    criticalPoint.
                            jacobiCriticalA;

            if (enterCriticalZone) {
                criticalPoint.jacobiDistanceDynamicMaxA =
                        (vObstaclePerpendicular.normalized()).transpose() *
                        criticalPoint.jacobiCriticalA;
            }
            double bSecond = (criticalPoint.jacobiDistanceDynamicMaxA *
                              criticalPoint.jointVelocityCriticalA).value();

            bSecond = computeB0(bSecond, criticalPoint.distanceA);
            double lambda = std::exp(
                    -convergenceBeta * criticalPoint.distanceA *
                    criticalPoint.obstacleVelocityForLinkA.norm());

            return std::make_tuple(bSecond, lambda);
        } else if (forLinkD && !forLinkA && !forFinalLink) {
            const auto resultObstacle = criticalPointInformation(
                    criticalPoint.jointAnglesCriticalD,
                    criticalPoint.closestPointObstacleD,
                    linkObstacleIndex);

            const MatrixXd jacobiObstaclePoint = get<1>(resultObstacle);

            criticalPoint.obstacleVelocityForLinkD = jacobiObstaclePoint(seq(0, 2), all) *
                                                     criticalPoint.jointVelocityCurrentD;
            //projection on this vector
            // = this->criticalPointsDynamicMap[key][i].obstacleVelocityFinalLink * distanceVector.normalized();
            auto [vObstacleParallel, vObstaclePerpendicular] = decomposeVector(
                    criticalPoint.obstacleVelocityForLinkD, criticalPoint.distVectorD);
            criticalPoint.jacobiDistanceDynamicD =
                    (vObstaclePerpendicular.normalized()).transpose() *
                    criticalPoint.
                            jacobiCriticalD;

            if (enterCriticalZone) {
                criticalPoint.jacobiDistanceDynamicMaxD =
                        (vObstaclePerpendicular.normalized()).transpose() *
                        criticalPoint.
                                jacobiCriticalD;
            }
            double bSecond = (criticalPoint.jacobiDistanceDynamicMaxD *
                              criticalPoint.jointVelocityCriticalD).value();

            bSecond = computeB0(bSecond, criticalPoint.distanceD);
            double lambda = std::exp(
                    -convergenceBeta * criticalPoint.distanceD *
                    criticalPoint.obstacleVelocityForLinkD.norm());

            return std::make_tuple(bSecond, lambda);
        } else {
            return std::make_tuple(0, 1);
        }
    } else {
        return std::make_tuple(0, 1);
    }
}

Eigen::MatrixXd
Robot::jacobianCriticalPoint(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                             int const &indexLink) const {
    Eigen::Isometry3d transformation;
    Eigen::MatrixXd jacobiCriticalPoint;
    transformation.matrix() = this->fkmCartesian(jointAngles, indexLink);
    const Eigen::Isometry3d inverseTransformation = transformation.inverse();
    const auto criticalPointRelativeToLink = inverseTransformation * closestPointLink;
    const auto relativeTranslation = trvec2tform(criticalPointRelativeToLink);
    return this->jacobianCartesianOnLink(jointAngles, indexLink, relativeTranslation);
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>
Robot::criticalPointInformation(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                                int const &indexLink) const {
    Eigen::Isometry3d transformation;
    Eigen::MatrixXd jacobiCriticalPoint;
    transformation.matrix() = this->fkmCartesian(jointAngles, indexLink);
    Eigen::Isometry3d inverseTransformation = transformation.inverse();
    Vector3d criticalPointRelativeToLink = inverseTransformation * closestPointLink;
    auto relativeTransformationCriticalPoint = trvec2tform(criticalPointRelativeToLink);
    auto jacobiCritical = this->
            jacobianCartesianOnLink(jointAngles, indexLink, relativeTransformationCriticalPoint);
    return std::make_tuple(relativeTransformationCriticalPoint, jacobiCritical);
}


double Robot::computeB0(double const &bFirst, double const &distance) const {
    double b0;
    if (bFirst > 0) {
        b0 = smoothingConstraintScheme(distance,
                                       this->distanceStopOA,
                                       this->distanceActivateOA,
                                       this->smootheningCoefficient) * bFirst;
    } else if (bFirst <= 0) {
        b0 = bFirst;
    } else {
        b0 = std::numeric_limits<double>::quiet_NaN();
    }
    return b0;
}


void Robot::synchronizeMaps(std::map<std::string, Obstacle> const &obstaclesDynamicMap) {
    // Remove keys from criticalPointsDynamicMap that are not in obstaclesDynamicMap
    for (auto it = this->criticalPointsDynamicMap.begin(); it != this->criticalPointsDynamicMap.end();) {
        if (!obstaclesDynamicMap.contains(it->first)) {
            // Remove the key if it is no longer in obstaclesDynamicMap
            it = this->criticalPointsDynamicMap.erase(it);
        } else {
            ++it; // Else, just move to the next element
        }
    }
    // Calculate number of links and initialize the default vector (once)
    const int numberJoints = this->getNumberJoints();
    const int numberLinksRobot = (this->bConsiderFinalLink) ? numberJoints + 1 : numberJoints;
    const std::vector<CriticalPoints> defaultVector(numberLinksRobot, CriticalPoints(numberJoints));


    // Add keys from obstaclesDynamicMap that are not already in criticalPointsDynamicMap
    for (const auto &obstaclesObject: obstaclesDynamicMap) {
        this->criticalPointsDynamicMap.emplace(obstaclesObject.first, defaultVector);
    }
}


void Robot::setObstacleMap(std::map<std::string, Obstacle> const &obstacleMap, bool saveLastSeenObstacle) {
    // Step 1: Add or update keys from obstacleMap
    for (const auto &[key, value]: obstacleMap) {
        this->obstaclesDynamicMap.insert_or_assign(key, value); // Add new keys or update existing ones
    }

    //remove keys that are not in obstacleMap if saveLastSeenObstacle is false
    if (!saveLastSeenObstacle) {
        for (auto it = this->obstaclesDynamicMap.begin(); it != this->obstaclesDynamicMap.end();) {
            if (!obstacleMap.contains(it->first)) {
                it = this->obstaclesDynamicMap.erase(it); // Remove key if it doesn't exist in obstacleMap
            } else {
                ++it; // Skip keys that exist in both maps
            }
        }
    }
}


// linkIndex should be less than 8 for franka: 7 for last joint
//call this equation only after obstacleAvoidance equartion (used for only plotting purposes)
double Robot::getMinimumDistanceLinkAllObstacles(int const &linkIndex) {
    if (linkIndex >= getNumberJoints() + 1) {
        throw std::invalid_argument("Link index out of range");
    }
    double minimumDistanceLinkAllObstacles = std::numeric_limits<double>::infinity();

    // If we have no obstacles in the map, return NaN immediately
    if (criticalPointsDynamicMap.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    for (auto const &[obstacleId, criticalPointInformation]: criticalPointsDynamicMap) {
        auto it = obstaclesDynamicMap.find(obstacleId);
        if (it == obstaclesDynamicMap.end()) {
            throw std::runtime_error(
                    "Map Synchronization Error: A Key in criticalPointsMap not found in obstacle map");
        }

        const Obstacle &obs = it->second;
        // If the linkIndex qualifies for checking this obstacle
        if (linkIndex >= obs.startIndex) {
            // Update the minimum distance
            if (linkIndex < criticalPointInformation.size()) {
                double distanceRobotLinkObstacle = criticalPointInformation[linkIndex].distance;
                if (distanceRobotLinkObstacle < minimumDistanceLinkAllObstacles) {
                    minimumDistanceLinkAllObstacles = distanceRobotLinkObstacle;
                }
            }
        }
        // else: we skip updating the distance
    }

    // After checking all obstacles:
    // If we never updated it from infinity, something was not valid for any obstacle
    if (std::isinf(minimumDistanceLinkAllObstacles)) {
        return std::numeric_limits<double>::quiet_NaN();
    } else {
        return minimumDistanceLinkAllObstacles;
    }
}

Eigen::VectorXd Robot::minimumDistanceAllLinks() {
    const int numberLinks = getNumberJoints() + 1;
    Eigen::VectorXd distanceAllLinksFromAllObstacles(numberLinks);
    for (int i = 0; i < numberLinks; i++) {
        distanceAllLinksFromAllObstacles[i] = getMinimumDistanceLinkAllObstacles(i);
    }
    return distanceAllLinksFromAllObstacles;
}

double Robot::getDistanceStopOA() const {
    return this->distanceStopOA;
}
