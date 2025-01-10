//
// Created by shobhit on 08.10.24.
//
#include <utils.h>
#include <Robot.h>
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


//configFile_Path : path to the file which containes paths to config file for the type of robot
//which_robot = type of robot
Robot::Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot) {
    auto Config_file = ConfigurationParameters(configFile_Path,
                                               parameterFor); //get the file which has path to the robotconfigfile
    this->Config = Config_file.getSubConfig(
            whichrobot); // create config file from class Configurationparameters for the type of robot

    std::vector<double> eul = Config.getJson("displacementEEtoTCP").at("rotationXYZ").get<std::vector<double> >();
    for (int i = 0; i < eul.size(); i++) {
        eul[i] = deg2Rad(static_cast<double>(eul[i]));
    }
    this->mdhMatrix = vectorMatrixToEigenMatrix(Config.get<vector<vector<double> > >("mdhParameters"));
    std::vector<double> translation = Config.getJson("displacementEEtoTCP").at(
            "translation").get<std::vector<double> >();

    this->transformationEEToTCP =
            trvec2tform(stdVectorToEigenVector(translation)) * convertEulerToTransform(stdVectorToEigenVector(eul));
    std::cout << "The trafo_matrix between endeffector and Tcp is: \n" << transformationEEToTCP << std::endl;

    auto obstacleAvoidanceParameters = Config.getJson("ObstacleAvoidanceParameters");
    this->radiusLinks = stdVectorToEigenVector(obstacleAvoidanceParameters["radiusLinks"].get<vector<double>>());
//    this->radiusLinks = stdVectorToEigenVector(Config.get<vector<double>>("radiusLinks"));
    this->radiusJoints = stdVectorToEigenVector(obstacleAvoidanceParameters["radiusJoints"].get<vector<double>>());
    this->finalLinkType = obstacleAvoidanceParameters["finalLinkType"].get<std::string>();
    this->finalLinkDimensions = stdVectorToEigenVector(
            obstacleAvoidanceParameters.at("finalLinkDimensions").get<vector<double>>());
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


VectorXd Robot::getCurrentRobotJointValues() {
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
    int numJoints = jointValues.size(); // Number of joints
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, numJoints); // Initialize Jacobian matrix with zeros

    // Compute the transformation to the end-effector or toIthLink
    Eigen::Matrix4d T_end_effector = fkmCartesian(jointValues, toIthLink);
    Eigen::Vector3d end_effector_position = T_end_effector.block<3, 1>(0, 3);


    for (int i = 1; i <= toIthLink; ++i) {
        // Get the transformation matrix from the base frame to the ith joint

        Eigen::Matrix4d T_i = fkmCartesian(jointValues, i);

        // Extract the z-axis of the ith joint in base frame coordinates
        Eigen::Vector3d z_i = T_i.block<3, 3>(0, 0) * Eigen::Vector3d(0, 0, 1);

        // Position of the ith joint in base frame coordinates
        Eigen::Vector3d d_i = T_i.block<3, 1>(0, 3);

        // Linear velocity contribution: cross product of z-axis with the vector from joint i to the end-effector
        Eigen::Vector3d Jv_i = z_i.cross(end_effector_position - d_i);

        // Angular velocity contribution: simply the z-axis for revolute joints
        Eigen::Vector3d Jw_i = z_i;

        // Fill in the ith column of the Jacobian matrix for joint i
        J.block<3, 1>(0, i - 1) = Jv_i; // Upper part for linear velocity
        J.block<3, 1>(3, i - 1) = Jw_i; // Lower part for angular velocity
    }

    // Columns of Jacobian corresponding to joints beyond toIthLink are already initialized to zero
    return J;
}

Eigen::MatrixXd Robot::jacobianCartesianOnLink(VectorXd const &jointValues, int const &toIthLink,
                                               Matrix4d const &transformationRelative) {
    const int numJoints = static_cast<int>(jointValues.size()); // Number of joints
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, numJoints); // Initialize Jacobian matrix with zeros

    // Compute the transformation to the end-effector or toIthLink

    Eigen::Matrix4d T_end_effector = fkmCartesian(jointValues, toIthLink) * transformationRelative;
    Eigen::Vector3d end_effector_position = T_end_effector.block<3, 1>(0, 3);


    for (int i = 1; i <= toIthLink; ++i) {
        // Get the transformation matrix from the base frame to the ith joint

        Eigen::Matrix4d T_i = fkmCartesian(jointValues, i);

        // Extract the z-axis of the ith joint in base frame coordinates
        Eigen::Vector3d z_i = T_i.block<3, 3>(0, 0) * Eigen::Vector3d(0, 0, 1);

        // Position of the ith joint in base frame coordinates
        Eigen::Vector3d d_i = T_i.block<3, 1>(0, 3);

        // Linear velocity contribution: cross product of z-axis with the vector from joint i to the end-effector
        Eigen::Vector3d Jv_i = z_i.cross(end_effector_position - d_i);

        // Angular velocity contribution: simply the z-axis for revolute joints
        Eigen::Vector3d Jw_i = z_i;

        // Fill in the ith column of the Jacobian matrix for joint i
        J.block<3, 1>(0, i - 1) = Jv_i; // Upper part for linear velocity
        J.block<3, 1>(3, i - 1) = Jw_i; // Lower part for angular velocity
    }

    // Columns of Jacobian corresponding to joints beyond toIthLink are already initialized to zero
    return J;
}

Eigen::MatrixXd Robot::jacobianCartesianTCP(const VectorXd &jointValues) {
    return jacobianCartesianOnLink(jointValues, static_cast<int>(this->joints->number_joints),
                                   this->transformationEEToTCP);
}

Eigen::Matrix4d Robot::transformMdh(double a, double alpha, double d, double theta) const {
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


std::pair<finalLinkRobot, std::vector<LinkSegment>> Robot::createLineSegments(Eigen::VectorXd const &jointValues) {
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
            MatrixXd dTransform;
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
                MatrixXd tcpTransform = dTransform * transformationEEToTCP;
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
                MatrixXd tcpTransform = aTransform * transformationEEToTCP;
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
        std::vector<Obstacles> obstacles, Eigen::VectorXd jointAngles, Eigen::VectorXd jointVelocityOA) {

    double minimumDistance = std::numeric_limits<double>::infinity();
    double distanceBuffer = 0.005;
    int numberLinksRobot = this->mdhMatrix.rows();

    if (this->bConsiderBaseToFirstJoint == false) {
        numberLinksRobot = numberLinksRobot - 1;
    }
    if (this->bConsiderFinalLink == true) {
        numberLinksRobot = numberLinksRobot + 1;
    }
    int numberJoints = this->getNumberJoints();

    int maxEntries = 2 * obstacles.size() * numberLinksRobot;

    MatrixXd j0 = MatrixXd::Zero(maxEntries, numberJoints);
    VectorXd b0 = VectorXd::Zero(maxEntries);

    int jCounter = 0;
    int bCounter = 0;

    static std::vector<Obstacles> ObstaclesDynamicArray;
    if (ObstaclesDynamicArray.empty()) {
        ObstaclesDynamicArray = obstacles;
    }

    //for dynamic obstacles compare the persistant obstacles with
    // new obstacles and do necessary changes to the ObstaclesDynamicArray

    int numberObstacles = ObstaclesDynamicArray.size();
    static std::vector<std::vector<CriticalPoints> > criticalPointsDynamicArray;
    if (criticalPointsDynamicArray.empty()) {
        criticalPointsDynamicArray.resize(numberObstacles,
                                          std::vector<CriticalPoints>(numberLinksRobot, CriticalPoints(numberJoints)));

    }

    /*  static bool criticalFlag;

      if (criticalFlag == false) {
          criticalFlag = true;
      }*/

    auto [finalLinkTCP, linkSegments] = this->createLineSegments(jointAngles);
    for (int l = 0; l < numberObstacles; ++l) {
        for (int i = 0; i < numberLinksRobot; ++i) {


            criticalPointsDynamicArray[l][i].distVectorA = Eigen::MatrixXd::Constant(3, 1,
                                                                                     std::numeric_limits<double>::quiet_NaN());
            criticalPointsDynamicArray[l][i].distVectorD = Eigen::MatrixXd::Constant(3, 1,
                                                                                     std::numeric_limits<double>::quiet_NaN());

            Vector3d closestPointLink;


            if (criticalPointsDynamicArray[l][i].hasCriticalPointA) {
                if (!bConsiderBaseToFirstJoint) {
                    auto closestPointLinkTranform = this->fkmCartesian(jointAngles, i + 1) *
                                                    criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkA;
                    closestPointLink = closestPointLinkTranform.block<3, 1>(0, 3);
                    criticalPointsDynamicArray[l][l].jacobiCriticalA = this->jacobianCartesianOnLink(jointAngles, i + 1,
                                                                                                     criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkA)(
                            seq(0, 2), all);
                } else if (bConsiderBaseToFirstJoint) {
                    auto closestPointLinkTranform = this->fkmCartesian(jointAngles, i) *
                                                    criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkA;
                    closestPointLink = closestPointLinkTranform.block<3, 1>(0, 3);
                    criticalPointsDynamicArray[l][l].jacobiCriticalA = this->jacobianCartesianOnLink(jointAngles, i + 1,
                                                                                                     criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkA)(
                            seq(0, 2), all);
                }

                //for static obstacles the position of obstacle point in the safety zone remains the same.

                criticalPointsDynamicArray[l][i].distVectorA =
                        closestPointLink - criticalPointsDynamicArray[l][i].closestPointObstacleA;


                criticalPointsDynamicArray[l][i].jacobiDistanceA =
                        (-(criticalPointsDynamicArray[l][i].distVectorA.normalized())).transpose() *
                        criticalPointsDynamicArray[l][l].jacobiCriticalA;

                b0(bCounter) = (-(criticalPointsDynamicArray[l][i].distVectorA.normalized())).transpose() *
                               criticalPointsDynamicArray[l][i].jacobiCriticalMaxA *
                               criticalPointsDynamicArray[l][i].jointVelocityCriticalA;
                b0(bCounter) *= smoothingConstraintScheme(criticalPointsDynamicArray[l][i].distVectorA.norm(),
                                                          this->distanceStopOA,
                                                          this->distanceActivateOA,
                                                          this->smootheningCoefficient);
                bCounter += 1;

                j0(jCounter, all) = (-(criticalPointsDynamicArray[l][i].distVectorA.normalized())).transpose() *
                                    criticalPointsDynamicArray[l][i].jacobiCriticalA;

                jCounter += 1;


            }


            if (criticalPointsDynamicArray[l][i].hasCriticalPointD) {
                if (!bConsiderBaseToFirstJoint) {
                    auto closestPointLinkTranform = this->fkmCartesian(jointAngles, i + 1) *
                                                    criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkD;
                    closestPointLink = closestPointLinkTranform.block<3, 1>(0, 3);
                    criticalPointsDynamicArray[l][l].jacobiCriticalD = this->jacobianCartesianOnLink(jointAngles, i + 1,
                                                                                                     criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkD)(
                            seq(0, 2), all);
                } else if (bConsiderBaseToFirstJoint) {
                    auto closestPointLinkTranform = this->fkmCartesian(jointAngles, i) *
                                                    criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkD;
                    closestPointLink = closestPointLinkTranform.block<3, 1>(0, 3);
                    criticalPointsDynamicArray[l][l].jacobiCriticalD = this->jacobianCartesianOnLink(jointAngles, i + 1,
                                                                                                     criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkD)(
                            seq(0, 2), all);
                }



                //for static obstacles the position of obstacle point in the safety zone remains the same.

                criticalPointsDynamicArray[l][i].distVectorD =
                        closestPointLink - criticalPointsDynamicArray[l][i].closestPointObstacleD;


                criticalPointsDynamicArray[l][i].jacobiDistanceD =
                        (-(criticalPointsDynamicArray[l][i].distVectorD.normalized())).transpose() *
                        criticalPointsDynamicArray[l][l].jacobiCriticalD;

                b0(bCounter) = (-(criticalPointsDynamicArray[l][i].distVectorD.normalized())).transpose() *
                               criticalPointsDynamicArray[l][i].jacobiCriticalMaxD *
                               criticalPointsDynamicArray[l][i].jointVelocityCriticalD;
                b0(bCounter) *= smoothingConstraintScheme(criticalPointsDynamicArray[l][i].distVectorD.norm(),
                                                          this->distanceStopOA,
                                                          this->distanceActivateOA,
                                                          this->smootheningCoefficient);
                bCounter += 1;

                j0(jCounter, all) = -(criticalPointsDynamicArray[l][i].distVectorD.normalized()).transpose() *
                                    criticalPointsDynamicArray[l][i].jacobiCriticalD;

                jCounter += 1;

            }

            if (criticalPointsDynamicArray[l][i].hasCriticalPointFinalLink && bConsiderFinalLink &&
                i == numberLinksRobot - 1) {

                auto closestPointLinkTranform = this->fkmCartesian(jointAngles, numberJoints) *
                                                criticalPointsDynamicArray[l][i].closestPointRelativeFinalLink;
                closestPointLink = closestPointLinkTranform.block<3, 1>(0, 3);
                criticalPointsDynamicArray[l][l].jacobiCriticalFinalLink = this->jacobianCartesianOnLink(jointAngles,
                                                                                                         numberJoints,
                                                                                                         criticalPointsDynamicArray[l][i].closestPointRelativeFinalLink)(
                        seq(0, 2), all);

                criticalPointsDynamicArray[l][i].distVectorFinalLink =
                        closestPointLink - criticalPointsDynamicArray[l][i].closestPointObstacleFinalLink;


                criticalPointsDynamicArray[l][i].jacobiDistanceFinalLink =
                        (-(criticalPointsDynamicArray[l][i].distVectorFinalLink.normalized())).transpose() *
                        criticalPointsDynamicArray[l][l].jacobiCriticalFinalLink;

                b0(bCounter) = (-(criticalPointsDynamicArray[l][i].distVectorFinalLink.normalized())).transpose() *
                               criticalPointsDynamicArray[l][i].jacobiCriticalMaxFinalLink *
                               criticalPointsDynamicArray[l][i].jointVelocityCriticalFinalLink;
                b0(bCounter) *= smoothingConstraintScheme(criticalPointsDynamicArray[l][i].distVectorFinalLink.norm(),
                                                          this->distanceStopOA,
                                                          this->distanceActivateOA,
                                                          this->smootheningCoefficient);
                bCounter += 1;

                j0(jCounter, all) = -(criticalPointsDynamicArray[l][i].distVectorFinalLink.normalized()).transpose() *
                                    criticalPointsDynamicArray[l][i].jacobiCriticalFinalLink;

                jCounter += 1;

            }


            if (!criticalPointsDynamicArray[l][i].hasCriticalPointFinalLink && bConsiderFinalLink &&
                i == numberLinksRobot - 1) {
                double distance(std::numeric_limits<double>::quiet_NaN());
                Vector3d closestPointObstacle(
                        Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())), closestPointLink(
                        Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN()));
                auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceFinalLinkObstacle(
                        finalLinkTCP);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);
                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                criticalPointsDynamicArray[l][i].distance = distance;

                if (distanceVector.norm() <= this->distanceActivateOA + this->distanceBuffer) {
                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, numberJoints);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);

                    criticalPointsDynamicArray[l][i].hasCriticalPointFinalLink = true;
                    criticalPointsDynamicArray[l][i].distVectorFinalLink = distanceVector;
                    criticalPointsDynamicArray[l][i].jacobiCriticalMaxFinalLink = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicArray[l][i].jacobiCriticalFinalLink = jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicArray[l][i].jointAnglesCriticalFinalLink = jointAngles;
                    criticalPointsDynamicArray[l][i].jacobiDistMaxFinalLink =
                            (-(distanceVector.normalized())).transpose() * jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicArray[l][i].jacobiDistanceFinalLink =
                            (-(distanceVector.normalized())).transpose() * jacobiCriticalPoint(seq(0, 2), all);
                    criticalPointsDynamicArray[l][i].closestPointObstacleFinalLink = closestPointObstacle;
                    criticalPointsDynamicArray[l][i].closestPointRelativeFinalLink = relativeTransformation;
                    criticalPointsDynamicArray[l][i].jointVelocityCriticalFinalLink = jointVelocityOA;
                    b0(bCounter) = (-(distanceVector.normalized())).transpose() *
                                   criticalPointsDynamicArray[l][i].jacobiCriticalMaxFinalLink *
                                   criticalPointsDynamicArray[l][i].jointVelocityCriticalFinalLink;
                    b0(bCounter) *= smoothingConstraintScheme(distance, this->distanceStopOA,
                                                              this->distanceActivateOA,
                                                              this->smootheningCoefficient);
                    bCounter += 1;
                    j0(jCounter, all) = (-(distanceVector.normalized())).transpose() *
                                        criticalPointsDynamicArray[l][i].jacobiCriticalFinalLink;
                    jCounter += 1;

                }

            }

            if (!criticalPointsDynamicArray[l][i].hasCriticalPointA) {
                double distance(std::numeric_limits<double>::quiet_NaN());
                Vector3d closestPointObstacle(
                        Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())), closestPointLink(
                        Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN()));
                if (bConsiderFinalLink && !bConsiderBaseToFirstJoint && i < numberLinksRobot - 1) {
                    if (linkSegments[i + 1].aSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i + 1].aSegmentV0,
                                linkSegments[i + 1].aSegmentV1,
                                linkSegments[i + 1].radiusLink,
                                linkSegments[i + 1].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }

                } else if (bConsiderFinalLink && bConsiderBaseToFirstJoint && i < numberLinksRobot - 1) {
                    if (linkSegments[i].aSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i].aSegmentV0,
                                linkSegments[i].aSegmentV1,
                                linkSegments[i].radiusLink,
                                linkSegments[i].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                } else if (!bConsiderFinalLink && !bConsiderBaseToFirstJoint) {
                    if (linkSegments[i + 1].aSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i + 1].aSegmentV0,
                                linkSegments[i + 1].aSegmentV1,
                                linkSegments[i + 1].radiusLink,
                                linkSegments[i + 1].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                } else if (!bConsiderFinalLink && bConsiderBaseToFirstJoint) {
                    if (linkSegments[i].aSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i].aSegmentV0,
                                linkSegments[i].aSegmentV1,
                                linkSegments[i].radiusLink,
                                linkSegments[i].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                }

                if (!std::isnan(distance) && !std::isnan(closestPointObstacle[0]) && !std::isnan(closestPointLink[0])) {
                    Vector3d distanceVector = closestPointLink - closestPointObstacle;
                    criticalPointsDynamicArray[l][i].distance = distance;
                    if (distanceVector.norm() <= this->distanceActivateOA + this->distanceBuffer) {
                        Eigen::Isometry3d transformation;
                        Eigen::MatrixXd jacobiCriticalPoint;
                        Eigen::Matrix4d relativeTransformation;
                        if (!bConsiderBaseToFirstJoint) {

                            auto result = criticalPointInformation(jointAngles, closestPointLink, i + 1);
                            relativeTransformation = get<0>(result);
                            jacobiCriticalPoint = get<1>(result);

                        } else if (bConsiderBaseToFirstJoint) {

                            auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                            relativeTransformation = get<0>(result);
                            jacobiCriticalPoint = get<1>(result);
                        }

                        criticalPointsDynamicArray[l][i].hasCriticalPointA = true;
                        criticalPointsDynamicArray[l][i].distVectorA = distanceVector;
                        criticalPointsDynamicArray[l][i].jacobiCriticalMaxA = jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].jacobiCriticalA = jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].jointAnglesCriticalA = jointAngles;
                        criticalPointsDynamicArray[l][i].jacobiDistMaxA =
                                (-(distanceVector.normalized())).transpose() * jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].jacobiDistanceA =
                                (-(distanceVector.normalized())).transpose() * jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].closestPointObstacleA = closestPointObstacle;
                        criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkA = relativeTransformation;
                        criticalPointsDynamicArray[l][i].jointVelocityCriticalA = jointVelocityOA;
                        b0(bCounter) = (-(distanceVector.normalized())).transpose() *
                                       criticalPointsDynamicArray[l][i].jacobiCriticalMaxA *
                                       criticalPointsDynamicArray[l][i].jointVelocityCriticalA;
                        b0(bCounter) *= smoothingConstraintScheme(distance, this->distanceStopOA,
                                                                  this->distanceActivateOA,
                                                                  this->smootheningCoefficient);
                        bCounter += 1;
                        j0(jCounter, all) = (-(distanceVector.normalized())).transpose() *
                                            criticalPointsDynamicArray[l][i].jacobiCriticalA;
                        jCounter += 1;
                    }
                }
                /*  if (i < numberLinksRobot-1) {}*/
            }

            if (!criticalPointsDynamicArray[l][i].hasCriticalPointD) {
                double distance(std::numeric_limits<double>::quiet_NaN());
                Vector3d closestPointObstacle(
                        Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())), closestPointLink(
                        Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN()));
                if (bConsiderFinalLink && !bConsiderBaseToFirstJoint && i < numberLinksRobot - 1) {
                    if (linkSegments[i + 1].dSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i + 1].dSegmentV0,
                                linkSegments[i + 1].dSegmentV1,
                                linkSegments[i + 1].radiusLink,
                                linkSegments[i + 1].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }

                } else if (bConsiderFinalLink && bConsiderBaseToFirstJoint && i < numberLinksRobot - 1) {
                    if (linkSegments[i].dSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i].dSegmentV0,
                                linkSegments[i].dSegmentV1,
                                linkSegments[i].radiusLink,
                                linkSegments[i].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                } else if (!bConsiderFinalLink && !bConsiderBaseToFirstJoint) {
                    if (linkSegments[i + 1].dSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i + 1].dSegmentV0,
                                linkSegments[i + 1].dSegmentV1,
                                linkSegments[i + 1].radiusLink,
                                linkSegments[i + 1].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                } else if (!bConsiderFinalLink && bConsiderBaseToFirstJoint) {
                    if (linkSegments[i].dSegmentV0.size() != 0) {
                        auto resultDistanceCalculation = ObstaclesDynamicArray[l].calculateDistanceRobotLinkObstacle(
                                linkSegments[i].dSegmentV0,
                                linkSegments[i].dSegmentV1,
                                linkSegments[i].radiusLink,
                                linkSegments[i].radiusJoint);
                        distance = std::get<0>(resultDistanceCalculation);
                        closestPointObstacle = std::get<1>(resultDistanceCalculation);
                        closestPointLink = std::get<2>(resultDistanceCalculation);
                    }
                }

                if (!std::isnan(distance) && !std::isnan(closestPointObstacle[0]) && !std::isnan(closestPointLink[0])) {
                    Vector3d distanceVector = closestPointLink - closestPointObstacle;
                    if ((distance < criticalPointsDynamicArray[l][i].distance &&
                         !(linkSegments[i].aSegmentV0.size() == 0)) || (linkSegments[i].aSegmentV0.size() == 0)) {
                        criticalPointsDynamicArray[l][i].distance = distance;
                    }

                    if (distance <= this->distanceActivateOA + this->distanceBuffer) {
                        Eigen::Isometry3d transformation;
                        Eigen::MatrixXd jacobiCriticalPoint;
                        Eigen::Matrix4d relativeTransformation;

                        if (!bConsiderBaseToFirstJoint) {

                            auto result = criticalPointInformation(jointAngles, closestPointLink, i + 1);
                            relativeTransformation = get<0>(result);
                            jacobiCriticalPoint = get<1>(result);

                        } else {

                            auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                            relativeTransformation = get<0>(result);
                            jacobiCriticalPoint = get<1>(result);

                        }
                        //inverseTransformation = transformation.inverse();

                        criticalPointsDynamicArray[l][i].hasCriticalPointD = true;
                        criticalPointsDynamicArray[l][i].distVectorD = distanceVector;
                        criticalPointsDynamicArray[l][i].jacobiCriticalMaxD = jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].jacobiCriticalD = jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].jointAnglesCriticalD = jointAngles;
                        criticalPointsDynamicArray[l][i].jacobiDistMaxD =
                                (-(distanceVector.normalized())).transpose() * jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].jacobiDistanceD =
                                (-(distanceVector.normalized())).transpose() * jacobiCriticalPoint(seq(0, 2), all);
                        criticalPointsDynamicArray[l][i].closestPointObstacleD = closestPointObstacle;
                        criticalPointsDynamicArray[l][i].closestPointRelativeRobotLinkD = relativeTransformation;
                        criticalPointsDynamicArray[l][i].jointVelocityCriticalD = jointVelocityOA;
                        b0(bCounter) = (-(distanceVector.normalized())).transpose() *
                                       criticalPointsDynamicArray[l][i].jacobiCriticalMaxD *
                                       criticalPointsDynamicArray[l][i].jointVelocityCriticalD;
                        b0(bCounter) *= smoothingConstraintScheme(distance, this->distanceStopOA,
                                                                  this->distanceActivateOA,
                                                                  this->smootheningCoefficient);
                        bCounter += 1;
                        j0(jCounter, all) = (-(distanceVector.normalized())).transpose() *
                                            criticalPointsDynamicArray[l][i].jacobiCriticalD;
                        jCounter += 1;

                    }
                }
            }

            if (criticalPointsDynamicArray[l][i].hasCriticalPointA &&
                criticalPointsDynamicArray[l][i].hasCriticalPointD) {
                criticalPointsDynamicArray[l][i].distance = std::min(
                        criticalPointsDynamicArray[l][i].distVectorA.norm(),
                        criticalPointsDynamicArray[l][i].distVectorD.norm());
            } else if (criticalPointsDynamicArray[l][i].hasCriticalPointA &&
                       !criticalPointsDynamicArray[l][i].hasCriticalPointD) {
                criticalPointsDynamicArray[l][i].distance = criticalPointsDynamicArray[l][i].distVectorA.norm();
            } else if (!criticalPointsDynamicArray[l][i].hasCriticalPointA &&
                       criticalPointsDynamicArray[l][i].hasCriticalPointD) {
                criticalPointsDynamicArray[l][i].distance = criticalPointsDynamicArray[l][i].distVectorD.norm();
            }

            //Delete Critical points if distance becomes greater
            if (criticalPointsDynamicArray[l][i].hasCriticalPointA &&
                (criticalPointsDynamicArray[l][i].distVectorA.norm() >
                 (this->distanceActivateOA + this->distanceBuffer))) {
                deleteCriticalPoint(criticalPointsDynamicArray[l][i], true, false, false);
            }
            if (criticalPointsDynamicArray[l][i].hasCriticalPointD &&
                (criticalPointsDynamicArray[l][i].distVectorD.norm() >
                 (this->distanceActivateOA + this->distanceBuffer))) {
                deleteCriticalPoint(criticalPointsDynamicArray[l][i], false, true, false);
            }

            if (criticalPointsDynamicArray[l][i].hasCriticalPointFinalLink &&
                (criticalPointsDynamicArray[l][i].distVectorFinalLink.norm() >
                 (this->distanceActivateOA + this->distanceBuffer))) {
                deleteCriticalPoint(criticalPointsDynamicArray[l][i], false, false, true);
            }

            if (criticalPointsDynamicArray[l][i].distance < minimumDistance) {
                minimumDistance = criticalPointsDynamicArray[l][i].distance;
            }
        }
    }

    j0.conservativeResize(jCounter, numberJoints);
    b0.conservativeResize(bCounter);

    return std::make_tuple(j0, b0, minimumDistance);
}

void
Robot::deleteCriticalPoint(CriticalPoints &criticalPoint, bool const &bDeleteCritcalA, bool const &bDeleteCritcalD,
                           bool const &bDeleteCriticalFinalLink) {
    int numberOfJoints = this->getNumberJoints();
    if (bDeleteCritcalA) {
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
        criticalPoint.closestPointObstacleA = Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointRelativeRobotLinkA = Eigen::MatrixXd::Constant(3, 1,
                                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalA = Eigen::VectorXd::Constant(numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());


    }
    if (bDeleteCritcalD) {
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
        criticalPoint.closestPointObstacleD = Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.closestPointRelativeRobotLinkD = Eigen::MatrixXd::Constant(3, 1,
                                                                                 std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointVelocityCriticalD = Eigen::VectorXd::Constant(numberOfJoints,
                                                                         std::numeric_limits<double>::quiet_NaN());
    }

    if (bDeleteCriticalFinalLink) {
        criticalPoint.hasCriticalPointFinalLink = false;
        criticalPoint.distVectorFinalLink = Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalMaxFinalLink = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                             std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jacobiCriticalFinalLink = Eigen::MatrixXd::Constant(3, numberOfJoints,
                                                                          std::numeric_limits<double>::quiet_NaN());
        criticalPoint.jointAnglesCriticalFinalLink = Eigen::VectorXd::Constant(numberOfJoints,
                                                                               std::numeric_limits<double>::quiet_NaN());
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
    auto jacobiCritical = this->jacobianCartesianOnLink(jointAngles, indexLink, relativeTransformationCriticalPoint);
    return std::make_tuple(relativeTransformationCriticalPoint, jacobiCritical);
}


