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
    //this->bSplitRegion = obstacleAvoidanceParameters.at("bSplitRegion").get<bool>();
    //this->bSplitRegionFinalLink = obstacleAvoidanceParameters.at("bSplitRegionFinalLink").get<bool>();
    this->bConsiderFinalLink = obstacleAvoidanceParameters.at("considerFinalLinkOA").get<bool>();
    this->bConsiderBaseToFirstJoint = obstacleAvoidanceParameters.at("considerBaseToFirstJoint").get<bool>();
    this->bConsiderRobotLinkAsLineSwept = obstacleAvoidanceParameters.at("robotLinkAsLineSwept").get<bool>();
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
            /* finalLink.finalV0 = this->fkmCartesian(jointValues,num_links)(seq(0,2),3);
             finalLink.finalV1 = this->fkmCartesianTCP(jointValues)(seq(0,2),3);
            */ /* linkSegments[i].toolV0 = linkSegments[i - 1].dSegmentV1; */
            /*Eigen::MatrixXd tcpTransform = dTransform * transformationEEToTCP;
            finalLink.finalV1 = tcpTransform.block<3, 1>(0, 3);*/
            //linkSegments[i].toolV1 = tcpTransform.block<3, 1>(0, 3);
            if (finalLink.type == "Box") {
                finalLink.dimensions = this->finalLinkDimensions;
                auto distanceVector = finalLink.finalV1 - finalLink.finalV0;
                finalLink.orientation = Quaterniond(tcpTransform.block<3, 3>(0, 0));
                auto dotProductZ = distanceVector.normalized().dot(tcpTransform.block<3, 1>(0, 2));
                auto dotProductY = distanceVector.normalized().dot(tcpTransform.block<3, 1>(0, 1));
                auto dotProductX = distanceVector.normalized().dot(tcpTransform.block<3, 1>(0, 0));

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
        std::map<std::string, Obstacles> const &obstaclesMap, Eigen::VectorXd const &jointAngles,
        Eigen::VectorXd const &jointVelocityOA) {

    double minimumDistance = std::numeric_limits<double>::infinity();

    int numberLinksRobot = this->getNumberJoints();
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


    int numberObstacles = obstaclesDynamicMap.size();
    if (this->criticalPointsDynamicMap.empty()) {
        // Prepare the default vector to be used for all entries
        std::vector<CriticalPoints> defaultVector(numberLinksRobot, CriticalPoints(numberJoints));

        //set obstacle keys into this->criticalPointsDynamicMap (init with default value)
        for (auto const &obstaclesObject: obstaclesDynamicMap) {
            this->criticalPointsDynamicMap.emplace(obstaclesObject.first, defaultVector);
        }

        // Use std::transform to set obstacle keys into this->criticalPointsDynamicMap (init with default value)
        /* std::transform(obstaclesDynamicMap.begin(), obstaclesDynamicMap.end(),
                        std::inserter(this->this->criticalPointsDynamicMap, this->this->criticalPointsDynamicMap.end()),
                        [&defaultVector](const auto &pair) {
                            return std::make_pair(pair.first, defaultVector);
                        });*/
    }
    //cout the this->criticalPointsDynamicMap keys
    /*for (const auto &[key, value]: this->criticalPointsDynamicMap) {
        std::cout << key << std::endl;
    }*/

    // TODO: if bConsiderBaseToFirstJoint == false, add first link to the obstacles
    if (!this->bConsiderBaseToFirstJoint) {
        if (linkSegments[0].aSegmentV0.size() > 0) {
            obstaclesDynamicMap.emplace("RobotLinkASegment_0", Obstacles(linkSegments[0].aSegmentV0,
                                                                         linkSegments[0].aSegmentV1,
                                                                         linkSegments[0].radiusLink));
        }
        if (linkSegments[0].dSegmentV0.size() > 0) {
            obstaclesDynamicMap.emplace("RobotLinkDSegment_0", Obstacles(linkSegments[0].dSegmentV0,
                                                                         linkSegments[0].dSegmentV1,
                                                                         linkSegments[0].radiusLink));
        }
    }

    // auto robotLinkIndex = AndreiUtils::stringToInteger(AndreiUtils::splitString().back());
    for (const auto &[obstacleId, obstacleArray]: obstaclesDynamicMap) {
        auto key = obstacleId;
        int startIndex = 0;
        auto keySplit = AndreiUtils::splitString(key, "_");
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
            /*this->criticalPointsDynamicMap[key][i].distVectorA = Eigen::MatrixXd::Constant(3, 1,
                                                                                     std::numeric_limits<double>::quiet_NaN());
            this->criticalPointsDynamicMap[key][i].distVectorD = Eigen::MatrixXd::Constant(3, 1,
                                                                                     std::numeric_limits<double>::quiet_NaN());*/
            //cout<<"link index: "<<i<<" obstacle id:" <<key<<endl;



            //Delete Critical points if distance becomes greater

            if (this->criticalPointsDynamicMap[key][i].
                    hasCriticalPointA && (this->criticalPointsDynamicMap[key][i].distanceA >
                                          (this->distanceActivateOA + this->distanceBuffer))) {
                cout << "checking if distance A > distance critical zone: "
                     << this->criticalPointsDynamicMap[key][i].distanceA << endl;

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


            if (this->criticalPointsDynamicMap[key][i].
                    hasCriticalPointA) {

                double distance;
                Vector3d closestPointObstacle, closestPointLink;
                auto resultDistanceCalculation = obstacleArray.calculateDistanceRobotLinkObstacle(
                        linkSegments[i].aSegmentV0,
                        linkSegments[i].aSegmentV1,
                        linkSegments[i].radiusLink,
                        linkSegments[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);

                Eigen::MatrixXd jacobiCriticalPoint;
                Eigen::Matrix4d relativeTransformation;

                auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                relativeTransformation = get<0>(result);
                jacobiCriticalPoint = get<1>(result);

                this->criticalPointsDynamicMap[key][i].
                        distVectorA = closestPointLink - closestPointObstacle;

                std::cout << "A linkindex: " << i << this->criticalPointsDynamicMap[key][i].hasCriticalPointA
                          << std::endl;
                //cout << closestPointLinkTransform << endl;

                this->criticalPointsDynamicMap[key][i].
                        jacobiCriticalA = jacobiCriticalPoint(seq(0,2),all);

                this->criticalPointsDynamicMap[key][i].distanceA = distance;

                this->criticalPointsDynamicMap[key][i].
                        jacobiDistanceA =
                        (-(this->criticalPointsDynamicMap[key][i].
                                distVectorA.normalized())).transpose() *
                        this->criticalPointsDynamicMap[key][i].jacobiCriticalA;

                /* double bFirst =  (-(this->criticalPointsDynamicMap[key][i].
                         distVectorA.normalized())).transpose() * this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxA * this->criticalPointsDynamicMap[key][i].jointVelocityCriticalA;
                */ double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxA *
                                    this->criticalPointsDynamicMap[key][i].jointVelocityCriticalA).value();
                b0(bCounter) = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceA);
                bCounter += 1;

                j0(jCounter, all) = this->criticalPointsDynamicMap[key][i].jacobiDistanceA;

                jCounter += 1;
            }

            if (this->criticalPointsDynamicMap[key][i].hasCriticalPointD) {

                double distance;
                Vector3d closestPointObstacle, closestPointLink;

                auto resultDistanceCalculation = obstacleArray.calculateDistanceRobotLinkObstacle(
                        linkSegments[i].dSegmentV0, linkSegments[i].dSegmentV1, linkSegments[i].radiusLink,
                        linkSegments[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);

                Vector3d distanceVector = closestPointLink - closestPointObstacle;

                Eigen::MatrixXd jacobiCriticalPoint;
                Eigen::Matrix4d relativeTransformation;

                auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                relativeTransformation = get<0>(result);
                jacobiCriticalPoint = get<1>(result);

                std::cout << "D" << i << this->criticalPointsDynamicMap[key][i].hasCriticalPointD << std::endl;

                this->criticalPointsDynamicMap[key][i].
                        jacobiCriticalD = jacobiCriticalPoint(seq(0, 2), all);
                /*  cout<< "closestPointLink"<<closestPointLink<<endl;
                  cout<<"Jacobi Critical if it has d critical point: \n"<<this->criticalPointsDynamicMap[key][i].jacobiCriticalD<<endl;*/

                this->criticalPointsDynamicMap[key][i].
                        distVectorD = distanceVector;
                this->criticalPointsDynamicMap[key][i].distanceD = distance;
                this->criticalPointsDynamicMap[key][i].
                        jacobiDistanceD =
                        (-(this->criticalPointsDynamicMap[key][i].
                                distVectorD.normalized())).transpose() *
                        this->criticalPointsDynamicMap[key][i].
                                jacobiCriticalD;

                /*   double bFirst =  (-(this->criticalPointsDynamicMap[key][i].
                           distVectorD.normalized())).transpose() * this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxD * this->criticalPointsDynamicMap[key][i].jointVelocityCriticalD;
                */            double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxD *
                                               this->criticalPointsDynamicMap[key][i].jointVelocityCriticalD).value();
                b0(bCounter) = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].
                        distanceD);
                bCounter += 1;

                j0(jCounter, all) = this->criticalPointsDynamicMap[key][i].jacobiDistanceD;
                jCounter += 1;
/*

                cout<<"j0: \n"<<j0<<endl;
                cout<<"b0: \n"<<b0<<endl;
*/
            }

            if (this->criticalPointsDynamicMap[key][i].
                    hasCriticalPointFinalLink && bConsiderFinalLink &&
                i == numberLinksRobot - 1) {

                double distance;
                Vector3d closestPointObstacle, closestPointLink;
                auto resultDistanceCalculation = obstacleArray.calculateDistanceFinalLinkObstacle(
                        finalLinkTCP);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);
                Vector3d distanceVector = closestPointLink - closestPointObstacle;

                Eigen::MatrixXd jacobiCriticalPoint;
                Eigen::Matrix4d relativeTransformation;

                auto result = criticalPointInformation(jointAngles, closestPointLink, numberJoints);
                relativeTransformation = get<0>(result);
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


                /* double bFirst =  (-(this->criticalPointsDynamicMap[key][i].
                    distVectorFinalLink.normalized())).transpose() * this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxFinalLink * this->criticalPointsDynamicMap[key][i].jointVelocityCriticalFinalLink;
                */
                double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxFinalLink *
                                 this->criticalPointsDynamicMap[key][i].jointVelocityCriticalFinalLink).value();

                b0(bCounter) = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceFinalLink);
                bCounter += 1;

                j0(jCounter, all) = this->criticalPointsDynamicMap[key][i].jacobiDistanceFinalLink;
                jCounter += 1;
            }

            if (!this->criticalPointsDynamicMap[key][i].
                    hasCriticalPointFinalLink && bConsiderFinalLink &&
                i == numberLinksRobot - 1) {
                double distance;
                Vector3d closestPointObstacle, closestPointLink;
                auto resultDistanceCalculation = obstacleArray.calculateDistanceFinalLinkObstacle(
                        finalLinkTCP);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);
                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                this->criticalPointsDynamicMap[key][i].
                        distance = distance;

                if (distance <= this->distanceActivateOA + this->distanceBuffer) {
                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, numberJoints);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);
                    this->criticalPointsDynamicMap[key][i].distanceFinalLink = distance;
                    this->criticalPointsDynamicMap[key][i].
                            hasCriticalPointFinalLink = true;
                    this->criticalPointsDynamicMap[key][i].
                            distVectorFinalLink = distanceVector;
                    this->criticalPointsDynamicMap[key][i].
                            jacobiCriticalMaxFinalLink = jacobiCriticalPoint(seq(0, 2), all);
                    this->criticalPointsDynamicMap[key][i].
                            jacobiCriticalFinalLink = jacobiCriticalPoint(seq(0, 2), all);
                    this->criticalPointsDynamicMap[key][i].
                            jointAnglesCriticalFinalLink = jointAngles;
                    this->criticalPointsDynamicMap[key][i].
                            jacobiDistMaxFinalLink =
                            (-(this->criticalPointsDynamicMap[key][i].distVectorFinalLink.normalized())).transpose() *
                            this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxFinalLink;
                    this->criticalPointsDynamicMap[key][i].
                            jacobiDistanceFinalLink =
                            (-(this->criticalPointsDynamicMap[key][i].distVectorFinalLink.normalized())).transpose() *
                            this->criticalPointsDynamicMap[key][i].
                                    jacobiCriticalFinalLink;
                    this->criticalPointsDynamicMap[key][i].
                            closestPointObstacleFinalLink = closestPointObstacle;
                    this->criticalPointsDynamicMap[key][i].
                            closestPointRelativeFinalLink = relativeTransformation;
                    this->criticalPointsDynamicMap[key][i].
                            jointVelocityCriticalFinalLink = jointVelocityOA;
                    double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxFinalLink *
                                     this->criticalPointsDynamicMap[key][i].jointVelocityCriticalFinalLink).value();
                    b0(bCounter) = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceFinalLink);
                    bCounter += 1;

                    j0(jCounter, all) = this->criticalPointsDynamicMap[key][i].
                            jacobiDistanceFinalLink;
                    jCounter += 1;
                }
            }
            if ((!this->criticalPointsDynamicMap[key][i].hasCriticalPointA) && (i < numberJoints) &&
                (linkSegments[i].aSegmentV0.size() != 0)) {
                double distance;
                Vector3d closestPointObstacle, closestPointLink;

                auto resultDistanceCalculation = obstacleArray.calculateDistanceRobotLinkObstacle(
                        linkSegments[i].aSegmentV0,
                        linkSegments[i].aSegmentV1,
                        linkSegments[i].radiusLink,
                        linkSegments[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);

                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                this->criticalPointsDynamicMap[key][i].distance = distance;
                if (distance <= this->distanceActivateOA + this->distanceBuffer) {
                    cout << "distanceA Link upon entering critical Zone: " << distance << endl;

                    this->criticalPointsDynamicMap[key][i].distanceA = distance;

                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;


                    auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);
                    this->criticalPointsDynamicMap[key][i].hasCriticalPointA = true;
                    this->criticalPointsDynamicMap[key][i].distVectorA = distanceVector;
                    //cout << criticalPointsDynamicMap[key][i].distVectorA << endl;
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
                    double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxA *
                                     this->criticalPointsDynamicMap[key][i].jointVelocityCriticalA).value();
                    b0(bCounter) = this->computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceA);
                    bCounter += 1;

                    j0(jCounter, all) = this->criticalPointsDynamicMap[key][i].jacobiDistanceA;
                    jCounter += 1;
                }
            }

            if ((!this->criticalPointsDynamicMap[key][i].hasCriticalPointD) && (i < numberJoints) &&
                (linkSegments[i].dSegmentV0.size() != 0)) {
                double distance;
                Vector3d closestPointObstacle, closestPointLink;


                auto resultDistanceCalculation = obstacleArray.calculateDistanceRobotLinkObstacle(
                        linkSegments[i].dSegmentV0, linkSegments[i].dSegmentV1, linkSegments[i].radiusLink,
                        linkSegments[i].radiusJoint, this->bConsiderRobotLinkAsLineSwept);
                distance = std::get<0>(resultDistanceCalculation);
                closestPointObstacle = std::get<1>(resultDistanceCalculation);
                closestPointLink = std::get<2>(resultDistanceCalculation);

                Vector3d distanceVector = closestPointLink - closestPointObstacle;
                if ((distance < this->criticalPointsDynamicMap[key][i].distance &&
                     !(linkSegments[i].aSegmentV0.size() == 0)) || (linkSegments[i].aSegmentV0.size() == 0)) {
                    this->criticalPointsDynamicMap[key][i].distance = distance;
                }


                if (distance <= this->distanceActivateOA + this->distanceBuffer) {
                    //   cout<<"closestPointLink on the entry of critical zone: \n" <<closestPointLink<<endl;
                    Eigen::Isometry3d transformation;
                    Eigen::MatrixXd jacobiCriticalPoint;
                    Eigen::Matrix4d relativeTransformation;
                    cout << "distanceD Link upon entering critical Zone for " + std::to_string(i) + ": " << distance
                         << endl;

                    auto result = criticalPointInformation(jointAngles, closestPointLink, i);
                    relativeTransformation = get<0>(result);
                    jacobiCriticalPoint = get<1>(result);
                    this->criticalPointsDynamicMap[key][i].distanceD = distance;
                    this->criticalPointsDynamicMap[key][i].hasCriticalPointD = true;
                    this->criticalPointsDynamicMap[key][i].distVectorD = distanceVector;
                    this->criticalPointsDynamicMap[key][i].jacobiCriticalMaxD = jacobiCriticalPoint(seq(0, 2), all);
                    this->criticalPointsDynamicMap[key][i].jacobiCriticalD = jacobiCriticalPoint(seq(0, 2), all);
                    //cout<<"jacobi critical on the entry of critical zone: \n" <<this->criticalPointsDynamicMap[key][i].jacobiCriticalD <<endl;
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
                    double bFirst = (this->criticalPointsDynamicMap[key][i].jacobiDistMaxD *
                                     this->criticalPointsDynamicMap[key][i].jointVelocityCriticalD).value();
                    b0(bCounter) = computeB0(bFirst, this->criticalPointsDynamicMap[key][i].distanceD);
                    bCounter += 1;

                    j0(jCounter, all) = this->criticalPointsDynamicMap[key][i].jacobiDistanceD;
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

    if (jCounter == 0) {
        j0.conservativeResize(0, 0);
    } else {
        j0.conservativeResize(jCounter, numberJoints);
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


double Robot::computeB0(double const &bFirst, double const &distance) {
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

