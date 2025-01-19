//
// Created by shobhit on 08.10.24.
//
#include <utils.h>
#include <Robot.h>
#include <AndreiUtils/utils.h>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsGeometry.h>
#include <AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsJson.h>
//#include <utility>
#include <cmath>
#include <ObstacleAvoidanceUtils.h>


using namespace AndreiUtils;
using namespace Eigen;
using namespace ObstacleAvoidance;
using namespace std;
using json = nlohmann::json;


//configFile_Path : path to the file which contains paths to config file for the type of robot
//which_robot = type of robot
Robot::Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot) {
    auto Config_file = ConfigurationParameters(configFile_Path,
                                               parameterFor); //get the file which has path to the robotconfigfile
    this->Config = Config_file.getSubConfig(
        whichrobot); // create config file from class Configurationparameters for the type of robot

    auto eul = Config.getJson("displacementEEtoTCP").at("rotationXYZ").get<std::vector<double> >();
    for (int i = 0; i < eul.size(); i++) {
        eul[i] = deg2Rad(static_cast<double>(eul[i]));
    }
    this->mdhMatrix = vectorMatrixToEigenMatrix(Config.get<vector<vector<double> > >("mdhParameters"));
    auto translation = Config.getJson("displacementEEtoTCP").at(
        "translation").get<std::vector<double> >();

    this->transformationEEToTCP =
            trvec2tform(stdVectorToEigenVector(translation)) * convertEulerToTransform(stdVectorToEigenVector(eul));
    std::cout << "The trafo_matrix between end effector and Tcp is: \n" << transformationEEToTCP << std::endl;

    auto obstacleAvoidanceParameters = Config.getJson("ObstacleAvoidanceParameters");
    this->radiusLinks = stdVectorToEigenVector(obstacleAvoidanceParameters["radiusLinks"].get<vector<double> >());
    //    this->radiusLinks = stdVectorToEigenVector(Config.get<vector<double>>("radiusLinks"));
    this->radiusJoints = stdVectorToEigenVector(obstacleAvoidanceParameters["radiusJoints"].get<vector<double> >());
    this->finalLinkType = obstacleAvoidanceParameters["finalLinkType"].get<std::string>();
    this->finalLinkDimensions = stdVectorToEigenVector(
        obstacleAvoidanceParameters.at("finalLinkDimensions").get<vector<double> >());
    this->bSplitRegion = obstacleAvoidanceParameters.at("bSplitRegion").get<bool>();
    this->bSplitRegionFinalLink = obstacleAvoidanceParameters.at("bSplitRegionFinalLink").get<bool>();
    this->bConsiderFinalLink = obstacleAvoidanceParameters.at("considerFinalLinkOA").get<bool>();
    this->bConsiderBaseToFirstJoint = obstacleAvoidanceParameters.at("considerBaseToFirstJoint").get<bool>();

    this->distanceActivateOA = obstacleAvoidanceParameters.at("distanceActivate").get<double>();
    this->distanceStopOA = obstacleAvoidanceParameters.at("distanceStop").get<double>();
    this->smootheningCoefficient = obstacleAvoidanceParameters.at("smootheningCoefficient").get<double>();
    this->distanceBuffer = obstacleAvoidanceParameters.at("distanceBuffer").get<double>();
    std::cout << "Fetching joints config under: " << whichrobot + "/Joints" << std::endl;

    auto Config_joints = Config.getSubConfig("Joints");
    this->joints = make_shared<Joints>(Config_joints);
    /* this->number_joints = Config_joints.get<size_t>("numberOfJoints");
     auto nr = number_joints;*/
}


VectorXd Robot::getRobotJointValues() const {
    return this->joints->values;
}


VectorXd Robot::getCurrentRobotJointValues() const {
    return this->joints->getCurrentJointValues();
}


/*void Robot::updateRobotJointValues() {
    this->joints->update();
}*/

void Robot::setJointValues(VectorXd const &jointValues) {
    this->joints->setJointValues(jointValues);
}


int Robot::getNumberJoints() const {
    return this->joints->number_joints;
}

const shared_ptr<Joints> &Robot::getJoints() const {
    return joints;
}

Eigen::MatrixXd Robot::jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const {
    const int numJoints = jointValues.size(); // Number of joints
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

Eigen::Matrix4d Robot::transformMdh(double const a, double const alpha, double const d, double const theta) const {
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


std::pair<finalLinkRobot, std::vector<LinkSegment> > Robot::createLineSegments(Eigen::VectorXd const &jointValues) {
    int num_links = mdhMatrix.rows();
    vector<LinkSegment> linkSegments(num_links);

    finalLinkRobot finalLink;
    finalLink.type = this->finalLinkType;


    Eigen::MatrixXd prevTransform;

    for (int i = 1; i <= num_links; ++i) {
        if (i == 1)
            prevTransform = Eigen::MatrixXd::Identity(4, 4);
        else
            prevTransform = fkmCartesian(jointValues, i - 1);


        Eigen::MatrixXd aTransform = Eigen::MatrixXd::Identity(4, 4);


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
            if (i == num_links) {
                finalLink.finalV0 = linkSegments[i - 1].dSegmentV1;
                /* finalLink.finalV0 = this->fkmCartesian(jointValues,num_links)(seq(0,2),3);
                 finalLink.finalV1 = this->fkmCartesianTCP(jointValues)(seq(0,2),3);
                */ /* linkSegments[i].toolV0 = linkSegments[i - 1].dSegmentV1; */
                Eigen::MatrixXd tcpTransform = dTransform * transformationEEToTCP;
                finalLink.finalV1 = tcpTransform.block<3, 1>(0, 3);
                //linkSegments[i].toolV1 = tcpTransform.block<3, 1>(0, 3);
                if (finalLink.type == "Box") {
                    finalLink.dimensions = this->finalLinkDimensions;
                    auto distanceVector = finalLink.finalV1 - finalLink.finalV0;
                    finalLink.orientation = Quaterniond(tcpTransform.block<3, 3>(0, 0));
                    auto dotProductZ = distanceVector.normalized().transpose() * tcpTransform.block<3, 1>(0, 2);
                    auto dotProductY = distanceVector.normalized().transpose() * tcpTransform.block<3, 1>(0, 1);
                    auto dotProductX = distanceVector.normalized().transpose() * tcpTransform.block<3, 1>(0, 0);
                    if (dotProductX > 0 || dotProductX < 0 && dotProductY == 0 && dotProductZ == 0) {
                        finalLink.center =
                                finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[0] / 2);
                    }
                    if (dotProductX == 0 && dotProductY > 0 || dotProductY < 0 && dotProductZ == 0) {
                        finalLink.center =
                                finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[1] / 2);
                    }
                    if (dotProductX == 0 && dotProductY == 0 && dotProductZ > 0 || dotProductZ < 0) {
                        finalLink.center =
                                finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[2] / 2);
                    }
                }
            }
        } else {
            if (i == num_links) {
                //linkSegments[i].toolV0 = linkSegments[i - 1].aSegmentV1;
                Eigen::MatrixXd tcpTransform = aTransform * transformationEEToTCP;
                //linkSegments[i].toolV1 = tcpTransform(seq(0, 2), 3);
                finalLink.finalV0 = linkSegments[i - 1].aSegmentV1;
                finalLink.finalV1 = tcpTransform.block<3, 1>(0, 3);
                if (finalLink.type == "Box") {
                    finalLink.dimensions = this->finalLinkDimensions;
                    auto distanceVector = finalLink.finalV1 - finalLink.finalV0;
                    finalLink.orientation = Quaterniond(tcpTransform.block<3, 3>(0, 0));
                    auto dotProductZ = distanceVector.normalized().transpose() * tcpTransform.block<3, 1>(0, 2);
                    auto dotProductY = distanceVector.normalized().transpose() * tcpTransform.block<3, 1>(0, 1);
                    auto dotProductX = distanceVector.normalized().transpose() * tcpTransform.block<3, 1>(0, 0);
                    if ((dotProductX > 0 || dotProductX < 0) && dotProductY == 0 && dotProductZ == 0) {
                        finalLink.center =
                                finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[0] / 2);
                    }
                    if (dotProductX == 0 && (dotProductY > 0 || dotProductY < 0) && dotProductZ == 0) {
                        finalLink.center =
                                finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[1] / 2);
                    }
                    if (dotProductX == 0 && dotProductY == 0 && (dotProductZ > 0 || dotProductZ < 0)) {
                        finalLink.center =
                                finalLink.finalV0 + (distanceVector.normalized() * finalLink.dimensions[2] / 2);
                    }
                }
            }
        }

        linkSegments[i - 1].radiusLink = this->radiusLinks[i - 1];
        linkSegments[i - 1].radiusJoint = this->radiusJoints[i - 1];
    }

    return std::make_pair(finalLink, linkSegments);
}


std::tuple<Eigen::MatrixXd, Eigen::VectorXd, double> Robot::obstacleAvoidanceEquation(
    std::map<std::string, Obstacles> const &obstaclesMap, Eigen::VectorXd const &jointAngles,
    Eigen::VectorXd const &jointVelocityOA) {
    double minimumDistance = std::numeric_limits<double>::infinity();
    double distanceBuffer = 0.005;
    int numberLinksRobot = this->mdhMatrix.rows();
    auto [finalLinkTCP, linkSegments] = this->createLineSegments(jointAngles);


    if (this->obstaclesDynamicMap.empty()) {
        obstaclesDynamicMap = obstaclesMap;
    }
    /* if (this->bConsiderBaseToFirstJoint == false) {
         numberLinksRobot = numberLinksRobot - 1;
     }*/
    if (this->bConsiderFinalLink == true) {
        numberLinksRobot = numberLinksRobot + 1;
    }
    int numberJoints = this->getNumberJoints();

    int maxEntries = 2 * obstaclesDynamicMap.size() * numberLinksRobot;

    Eigen::MatrixXd j0 = Eigen::MatrixXd::Zero(maxEntries, numberJoints);
    Eigen::VectorXd b0 = Eigen::VectorXd::Zero(maxEntries);

    int jCounter = 0;
    int bCounter = 0;

    /*    static std::vector<Obstacles> ObstaclesDynamicArray;
        if (ObstaclesDynamicArray.empty()) {
            ObstaclesDynamicArray = obstacles;
        }*/


    //for dynamic obstacles compare the persistent obstacles with
    // new obstacles and do necessary changes to the ObstaclesDynamicArray

    int numberObstacles = obstaclesDynamicMap.size();
    if (this->criticalPointsDynamicMap.empty()) {
        // Prepare the default vector to be used for all entries
        std::vector<CriticalPoints> defaultVector(numberLinksRobot, CriticalPoints(numberJoints));

        // Use std::transform to set obstacle keys into criticalPointsDynamicMap (init with default value)
        std::transform(obstaclesDynamicMap.begin(), obstaclesDynamicMap.end(),
                       std::inserter(this->criticalPointsDynamicMap, this->criticalPointsDynamicMap.end()),
                       [&defaultVector](const auto &pair) {
                           return std::make_pair(pair.first, defaultVector);
                       });
    }

    // TODO: if bConsiderBaseToFirstJoint == false, add first link to the obstacles
    if (!this->bConsiderBaseToFirstJoint) {
        if (linkSegments[0].aSegmentV0.size() > 0) {
            obstaclesDynamicMap.emplace("RobotLinkASegment_0",Obstacles(linkSegments[0].aSegmentV0,
                                                                   linkSegments[0].aSegmentV1,
                                                                   linkSegments[0].radiusLink));
        }
        if (linkSegments[0].dSegmentV0.size() > 0) {
            obstaclesDynamicMap.emplace("RobotLinkDSegment_0", Obstacles(linkSegments[0].dSegmentV0,
                                                                   linkSegments[0].dSegmentV1,
                                                                   linkSegments[0].radiusLink));
        }
    }

    /*static std::vector<std::vector<CriticalPoints> > criticalPointsDynamicArray;
    if (criticalPointsDynamicArray.empty()) {
        criticalPointsDynamicArray.resize(numberObstacles,
                                          std::vector<CriticalPoints>(numberLinksRobot, CriticalPoints(numberJoints)));
    }*/

    // auto robotLinkIndex = AndreiUtils::stringToInteger(AndreiUtils::splitString().back());
    for (const auto &[obstacleId, obstacleArray]: obstaclesDynamicMap) {
        auto key = obstacleId;
        int startIndex = 0;
        std::vector<std::string> keySplit = AndreiUtils::splitString(key, "_");
        if (keySplit[0] == "RobotLinkASegment" || keySplit[0] == "RobotLinkDSegment") {
            int linkObstacleIndex = AndreiUtils::stringToInteger(keySplit[1]);
            if ((linkSegments[linkObstacleIndex + 1].aSegmentV0.size() != 0) || (
                    linkSegments[linkObstacleIndex + 1].dSegmentV0.size() != 0)) {
                startIndex = linkObstacleIndex + 2;
            } else if (linkSegments[linkObstacleIndex + 1].aSegmentV0.size() == 0 && linkSegments[linkObstacleIndex + 1]
                       .dSegmentV0.size() == 0) {
                startIndex = linkObstacleIndex + 3;
            }
        } else {
            startIndex = !bConsiderBaseToFirstJoint;
        }
        for (int i = startIndex; i < numberLinksRobot; ++i) {
            criticalPointsDynamicMap[key][i].distVectorA = Eigen::MatrixXd::Constant(3, 1,
                std::numeric_limits<double>::quiet_NaN());
            criticalPointsDynamicMap[key][i].distVectorD = Eigen::MatrixXd::Constant(3, 1,
                std::numeric_limits<double>::quiet_NaN());

            if (criticalPointsDynamicMap[key][i].
                hasCriticalPointA) {
                auto closestPointLinkTransform = this->fkmCartesian(jointAngles, i) *
                                                 criticalPointsDynamicMap[key][i].
                                                 closestPointRelativeRobotLinkA;
                Vector3d closestPointLink = closestPointLinkTransform.block<3, 1>(0, 3);
                criticalPointsDynamicMap[key][i].
                        jacobiCriticalA = this->jacobianCartesianOnLink(jointAngles, i + 1,
                                                                        criticalPointsDynamicMap[key][i].
                                                                        closestPointRelativeRobotLinkA)(
                            seq(0, 2), all);

                //for static obstacles the position of obstacle point in the safety zone remains the same.
                criticalPointsDynamicMap[key][i].
                        distVectorA =
                        closestPointLink - criticalPointsDynamicMap[key][i].
                        closestPointObstacleA;


                criticalPointsDynamicMap[key][i].
                        jacobiDistanceA =
                        (-(criticalPointsDynamicMap[key][i].
                            distVectorA.normalized())).transpose() *
                        criticalPointsDynamicMap[key][i].jacobiCriticalA;

                b0(bCounter) = (-(criticalPointsDynamicMap[key][i].
                                   distVectorA.normalized())).transpose() *
                               criticalPointsDynamicMap[key][i].
                               jacobiCriticalMaxA *
                               criticalPointsDynamicMap[key][i].
                               jointVelocityCriticalA;
                b0(bCounter) *= smoothingConstraintScheme(criticalPointsDynamicMap[key][i].
                                                          distVectorA.norm(),
                                                          this->distanceStopOA,
                                                          this->distanceActivateOA,
                                                          this->smootheningCoefficient);
                bCounter += 1;

                j0(jCounter, all) = (-(criticalPointsDynamicMap[key][i].
                                        distVectorA.normalized())).transpose() *
                                    criticalPointsDynamicMap[key][i].
                                    jacobiCriticalA;

                jCounter += 1;
            }

            if (criticalPointsDynamicMap[key][i].hasCriticalPointD) {
                auto closestPointLinkTransform = this->fkmCartesian(jointAngles, i) *
                                                 criticalPointsDynamicMap[key][i].
                                                 closestPointRelativeRobotLinkD;
                Vector3d closestPointLink = closestPointLinkTransform.block<3, 1>(0, 3);
                criticalPointsDynamicMap[key][i].
                        jacobiCriticalD = this->jacobianCartesianOnLink(jointAngles, i + 1,
                                                                        criticalPointsDynamicMap[key][i].
                                                                        closestPointRelativeRobotLinkD)(
                            seq(0, 2), all);

                //for static obstacles the position of obstacle point in the safety zone remains the same.
                criticalPointsDynamicMap[key][i].
                        distVectorD =
                        closestPointLink - criticalPointsDynamicMap[key][i].
                        closestPointObstacleD;

                criticalPointsDynamicMap[key][i].
                        jacobiDistanceD =
                        (-(criticalPointsDynamicMap[key][i].
                            distVectorD.normalized())).transpose() *
                        criticalPointsDynamicMap[key][i].
                        jacobiCriticalD;

                b0(bCounter) = (-(criticalPointsDynamicMap[key][i].
                                   distVectorD.normalized())).transpose() *
                               criticalPointsDynamicMap[key][i].
                               jacobiCriticalMaxD *
                               criticalPointsDynamicMap[key][i].
                               jointVelocityCriticalD;
                b0(bCounter) *= smoothingConstraintScheme(criticalPointsDynamicMap[key][i].
                                                          distVectorD.norm(),
                                                          this->distanceStopOA,
                                                          this->distanceActivateOA,
                                                          this->smootheningCoefficient);
                bCounter += 1;

                j0(jCounter, all) = -(criticalPointsDynamicMap[key][i].
                                        distVectorD.normalized()).transpose() *
                                    criticalPointsDynamicMap[key][i].
                                    jacobiCriticalD;

                jCounter += 1;
            }

            if (criticalPointsDynamicMap[key][i].
                hasCriticalPointFinalLink && bConsiderFinalLink &&
                i == numberLinksRobot - 1) {
                auto closestPointLinkTransform = this->fkmCartesian(jointAngles, numberJoints) *
                                                 criticalPointsDynamicMap[key][i].
                                                 closestPointRelativeFinalLink;
                Vector3d closestPointLink = closestPointLinkTransform.block<3, 1>(0, 3);
                criticalPointsDynamicMap[key][i].
                        jacobiCriticalFinalLink = this->jacobianCartesianOnLink(jointAngles,
                            numberJoints,
                            criticalPointsDynamicMap[key][i].
                            closestPointRelativeFinalLink)(
                            seq(0, 2), all);

                criticalPointsDynamicMap[key][i].
                        distVectorFinalLink =
                        closestPointLink - criticalPointsDynamicMap[key][i].
                        closestPointObstacleFinalLink;


                criticalPointsDynamicMap[key][i].
                        jacobiDistanceFinalLink =
                        (-(criticalPointsDynamicMap[key][i].
                            distVectorFinalLink.normalized())).transpose() *
                        criticalPointsDynamicMap[key][i].
                        jacobiCriticalFinalLink;

                b0(bCounter) = (-(criticalPointsDynamicMap[key][i].
                                   distVectorFinalLink.normalized())).transpose() *
                               criticalPointsDynamicMap[key][i].
                               jacobiCriticalMaxFinalLink *
                               criticalPointsDynamicMap[key][i].
                               jointVelocityCriticalFinalLink;
                b0(bCounter) *= smoothingConstraintScheme(criticalPointsDynamicMap[key][i].
                                                          distVectorFinalLink.norm(),
                                                          this->distanceStopOA,
                                                          this->distanceActivateOA,
                                                          this->smootheningCoefficient);
                bCounter += 1;

                j0(jCounter, all) = -(criticalPointsDynamicMap[key][i].
                                        distVectorFinalLink.normalized()).transpose() *
                                    criticalPointsDynamicMap[key][i].
                                    jacobiCriticalFinalLink;

                jCounter += 1;
            }

            if (!criticalPointsDynamicMap[key][i].
                hasCriticalPointFinalLink && bConsiderFinalLink &&
                i == numberJoints) {
                double distance;
                Vector3d closestPointObstacle, closestPointLink;
                auto resultDistanceCalculation = obstacleArray.calculateDistanceFinalLinkObstacle(
                    finalLinkTCP);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);
                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                criticalPointsDynamicMap[key][i].
                        distance = distance;

                if (distanceVector.norm() <= this->distanceActivateOA + this->distanceBuffer) {
                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, numberJoints);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);

                    criticalPointsDynamicMap[key][i].
                            hasCriticalPointFinalLink = true;
                    criticalPointsDynamicMap[key][i].
                            distVectorFinalLink = distanceVector;
                    criticalPointsDynamicMap[key][i].
                            jacobiCriticalMaxFinalLink = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicMap[key][i].
                            jacobiCriticalFinalLink = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicMap[key][i].
                            jointAnglesCriticalFinalLink = jointAngles;
                    criticalPointsDynamicMap[key][i].
                            jacobiDistMaxFinalLink =
                            (-(criticalPointsDynamicMap[key][i].distVectorFinalLink.normalized())).transpose() *
                            criticalPointsDynamicMap[key][i].jacobiCriticalMaxFinalLink;
                    criticalPointsDynamicMap[key][i].
                            jacobiDistanceFinalLink =
                            (-(criticalPointsDynamicMap[key][i].distVectorFinalLink.normalized())).transpose() *
                            criticalPointsDynamicMap[key][i].
                            jacobiCriticalFinalLink;
                    criticalPointsDynamicMap[key][i].
                            closestPointObstacleFinalLink = closestPointObstacle;
                    criticalPointsDynamicMap[key][i].
                            closestPointRelativeFinalLink = relativeTransformation;
                    criticalPointsDynamicMap[key][i].
                            jointVelocityCriticalFinalLink = jointVelocityOA;
                    b0(bCounter) = (criticalPointsDynamicMap[key][i].jacobiDistMaxFinalLink * criticalPointsDynamicMap[
                                        key][i].jointVelocityCriticalFinalLink).value();
                    b0(bCounter) *= smoothingConstraintScheme(distance, this->distanceStopOA,
                                                              this->distanceActivateOA,
                                                              this->smootheningCoefficient);
                    bCounter += 1;
                    j0(jCounter, all) = criticalPointsDynamicMap[key][i].
                            jacobiDistanceFinalLink;
                    jCounter += 1;
                }
            }
            if ((!criticalPointsDynamicMap[key][i].hasCriticalPointA) && (i < numberJoints) &&
                (linkSegments[i].aSegmentV0.size() != 0)) {
                double distance;
                Vector3d closestPointObstacle, closestPointLink;

                auto resultDistanceCalculation = obstacleArray.calculateDistanceRobotLinkObstacle(
                    linkSegments[i].aSegmentV0,
                    linkSegments[i].aSegmentV1,
                    linkSegments[i].radiusLink,
                    linkSegments[i].radiusJoint);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);

                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                criticalPointsDynamicMap[key][i].distance = distance;
                if (distanceVector.norm() <= this->distanceActivateOA + this->distanceBuffer) {
                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);
                    criticalPointsDynamicMap[key][i].hasCriticalPointA = true;
                    criticalPointsDynamicMap[key][i].distVectorA = distanceVector;
                    criticalPointsDynamicMap[key][i].jacobiCriticalMaxA = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicMap[key][i].jacobiCriticalA = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicMap[key][i].jointAnglesCriticalA = jointAngles;
                    criticalPointsDynamicMap[key][i].jacobiDistMaxA =
                            (-(criticalPointsDynamicMap[key][i].distVectorA.normalized())).transpose() *
                            criticalPointsDynamicMap[key][i].jacobiCriticalMaxA;
                    criticalPointsDynamicMap[key][i].jacobiDistanceA =
                            (-(criticalPointsDynamicMap[key][i].distVectorA.normalized())).transpose() *
                            criticalPointsDynamicMap[key][i].jacobiCriticalA;
                    criticalPointsDynamicMap[key][i].closestPointObstacleA = closestPointObstacle;
                    criticalPointsDynamicMap[key][i].closestPointRelativeRobotLinkA = relativeTransformation;
                    criticalPointsDynamicMap[key][i].jointVelocityCriticalA = jointVelocityOA;
                    b0(bCounter) = (criticalPointsDynamicMap[key][i].jacobiDistMaxA *
                                    criticalPointsDynamicMap[key][i].jointVelocityCriticalA).value();
                    b0(bCounter) *= smoothingConstraintScheme(distance, this->distanceStopOA,
                                                              this->distanceActivateOA,
                                                              this->smootheningCoefficient);
                    bCounter += 1;
                    j0(jCounter, all) = criticalPointsDynamicMap[key][i].jacobiDistanceA;
                    jCounter += 1;
                }
            }

            if ((!criticalPointsDynamicMap[key][i].hasCriticalPointD) && (i < numberJoints) &&
                (linkSegments[i].dSegmentV0.size() != 0)) {
                double distance;
                Vector3d closestPointObstacle, closestPointLink;


                auto resultDistanceCalculation = obstacleArray.calculateDistanceRobotLinkObstacle(
                    linkSegments[i].dSegmentV0,
                    linkSegments[i].dSegmentV1,
                    linkSegments[i].radiusLink,
                    linkSegments[i].radiusJoint);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);

                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                if ((distance < criticalPointsDynamicMap[key][i].distance &&
                     !(linkSegments[i].aSegmentV0.size() == 0)) || (linkSegments[i].aSegmentV0.size() == 0)) {
                    criticalPointsDynamicMap[key][i].distance = distance;
                }

                if (distance <= this->distanceActivateOA + this->distanceBuffer) {
                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;

                    auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);

                    criticalPointsDynamicMap[key][i].hasCriticalPointD = true;
                    criticalPointsDynamicMap[key][i].distVectorD = distanceVector;
                    criticalPointsDynamicMap[key][i].jacobiCriticalMaxD = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicMap[key][i].jacobiCriticalD = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicMap[key][i].jointAnglesCriticalD = jointAngles;
                    criticalPointsDynamicMap[key][i].jacobiDistMaxD =
                            (-(criticalPointsDynamicMap[key][i].distVectorD.normalized())).transpose() *
                            criticalPointsDynamicMap[key][i].jacobiCriticalMaxD;
                    criticalPointsDynamicMap[key][i].jacobiDistanceD =
                            (-(criticalPointsDynamicMap[key][i].distVectorD.normalized())).transpose() *
                            criticalPointsDynamicMap[key][i].jacobiCriticalD;
                    criticalPointsDynamicMap[key][i].closestPointObstacleD = closestPointObstacle;
                    criticalPointsDynamicMap[key][i].closestPointRelativeRobotLinkD = relativeTransformation;
                    criticalPointsDynamicMap[key][i].jointVelocityCriticalD = jointVelocityOA;
                    b0(bCounter) = (criticalPointsDynamicMap[key][i].jacobiDistMaxD *
                                    criticalPointsDynamicMap[key][i].jointVelocityCriticalD).value();
                    b0(bCounter) *= smoothingConstraintScheme(distance, this->distanceStopOA,
                                                              this->distanceActivateOA,
                                                              this->smootheningCoefficient);
                    bCounter += 1;
                    j0(jCounter, all) = criticalPointsDynamicMap[key][i].jacobiDistanceD;
                    jCounter += 1;
                }
            }

            if (criticalPointsDynamicMap[key][i].
                hasCriticalPointA &&
                criticalPointsDynamicMap[key][i].
                hasCriticalPointD) {
                criticalPointsDynamicMap[key][i].
                        distance = std::min(
                            criticalPointsDynamicMap[key][i].
                            distVectorA.norm(),
                            criticalPointsDynamicMap[key][i].
                            distVectorD.norm());
            } else if (criticalPointsDynamicMap[key][i].
                       hasCriticalPointA &&
                       !criticalPointsDynamicMap[key][i].
                       hasCriticalPointD) {
                criticalPointsDynamicMap[key][i].
                        distance = criticalPointsDynamicMap[key][i].
                        distVectorA.norm();
            } else if (!criticalPointsDynamicMap[key][i].
                       hasCriticalPointA &&
                       criticalPointsDynamicMap[key][i].
                       hasCriticalPointD) {
                criticalPointsDynamicMap[key][i].
                        distance = criticalPointsDynamicMap[key][i].
                        distVectorD.norm();
            }

            //Delete Critical points if distance becomes greater
            if (criticalPointsDynamicMap[key][i].
                hasCriticalPointA &&
                (criticalPointsDynamicMap[key][i].
                 distVectorA.norm() >
                 (this->distanceActivateOA + this->distanceBuffer))) {
                deleteCriticalPoint(criticalPointsDynamicMap[key][i]
                                    , true, false, false);
            }
            if (criticalPointsDynamicMap[key][i].
                hasCriticalPointD &&
                (criticalPointsDynamicMap[key][i].
                 distVectorD.norm() >
                 (this->distanceActivateOA + this->distanceBuffer))) {
                deleteCriticalPoint(criticalPointsDynamicMap[key][i]
                                    , false, true, false);
            }

            if (criticalPointsDynamicMap[key][i].
                hasCriticalPointFinalLink &&
                (criticalPointsDynamicMap[key][i].
                 distVectorFinalLink.norm() >
                 (this->distanceActivateOA + this->distanceBuffer))) {
                deleteCriticalPoint(criticalPointsDynamicMap[key][i]
                                    , false, false, true);
            }

            if (criticalPointsDynamicMap[key][i].
                distance < minimumDistance) {
                minimumDistance = criticalPointsDynamicMap[key][i].
                        distance;
            }
        }
    }

    j0.conservativeResize(jCounter, numberJoints);
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
        criticalPoint.closestPointRelativeRobotLinkA = Eigen::MatrixXd::Constant(3, 1,
            std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalA = Eigen::VectorXd::Constant(numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());
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
        criticalPoint.closestPointRelativeRobotLinkD = Eigen::MatrixXd::Constant(3, 1,
            std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalD = Eigen::VectorXd::Constant(numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());
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
        criticalPoint.closestPointRelativeFinalLink = Eigen::MatrixXd::Constant(3, 1,
            std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalFinalLink = Eigen::VectorXd::Constant(numberOfJoints,
            std::numeric_limits<double>::quiet_NaN());
    }
}


Eigen::MatrixXd
Robot::jacobianCriticalPoint(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                             int const &indexLink) {
    Eigen::Isometry3d transformation;
    Eigen::MatrixXd jacobiCriticalPoint;
    transformation.matrix() = this->fkmCartesian(jointAngles, indexLink);
    Eigen::Isometry3d inverseTransformation = transformation.inverse();
    auto criticalPointRelativeToLink = inverseTransformation * closestPointLink;
    auto relativeTranslation = trvec2tform(criticalPointRelativeToLink);
    return this->jacobianCartesianOnLink(jointAngles, indexLink, relativeTranslation);
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>
Robot::criticalPointInformation(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                                int const &indexLink) {
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
