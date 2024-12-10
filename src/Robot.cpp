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
#include <criticalPoints.h>

using namespace AndreiUtils;
using namespace Eigen;
using namespace ObstacleAvoidance;
using namespace std;
using json = nlohmann::json;




//configFile_Path : path to the file which containes paths to config file for the type of robot
//which_robot = type of robot
Robot::Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot,
             const AndreiUtils::Posed &baseFrame) {
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

std::vector<LinkSegment> Robot::createLineSegments(Eigen::VectorXd const &jointValues, Eigen::VectorXd const &radius) {
    int num_links = mdhMatrix.rows();
    vector<LinkSegment> link_segments(num_links + 1);


    Eigen::MatrixXd prevTransform;
    for (int i = 1; i <= num_links; ++i) {
        if (i == 1)
            prevTransform = Eigen::MatrixXd::Identity(4, 4);
        else
            prevTransform = fkmCartesian(jointValues, i - 1);


        Eigen::MatrixXd a_transform = Eigen::MatrixXd::Identity(4, 4);


        if (mdhMatrix(i - 1, 2) != 0) {
            link_segments[i - 1].aSegmentV0 = prevTransform.block<3, 1>(0, 3);
            a_transform = prevTransform * trvec2tform({mdhMatrix(i - 1, 2), 0, 0});
            link_segments[i - 1].aSegmentV1 = a_transform.block<3, 1>(0, 3);
        }

        if (mdhMatrix(i - 1, 1) != 0) {
            MatrixXd d_transform;
            if (mdhMatrix(i - 1, 2) != 0) {
                link_segments[i - 1].dSegmentV0 = link_segments[i - 1].aSegmentV1;
                d_transform = a_transform * convertEulerToTransform({deg2Rad(mdhMatrix(i - 1, 3)), 0, 0})
                              * trvec2tform({0, 0, mdhMatrix(i - 1, 1)});
                link_segments[i - 1].dSegmentV1 = d_transform(seq(0, 2), 3);
            } else {
                link_segments[i - 1].dSegmentV0 = prevTransform(seq(0, 2), 3);
                d_transform = prevTransform * convertEulerToTransform({deg2Rad(mdhMatrix(i - 1, 3)), 0, 0})
                              * trvec2tform({0, 0, mdhMatrix(i - 1, 1)});
                link_segments[i - 1].dSegmentV1 = d_transform(seq(0, 2), 3);
            }
            if (i == num_links) {
                link_segments[i].Tool_V0 = link_segments[i - 1].dSegmentV1;
                MatrixXd tcp_transform = d_transform * transformationEEToTCP;
                link_segments[i].Tool_V1 = tcp_transform.block<3, 1>(0, 3);
            }
        } else {
            if (i == num_links) {
                link_segments[i].Tool_V0 = link_segments[i - 1].aSegmentV1;
                MatrixXd tcp_transform = a_transform * transformationEEToTCP;
                link_segments[i].Tool_V1 = tcp_transform(seq(0, 2), 3);
            }
        }

        link_segments[i].radius = radius(i);
    }

    return link_segments;
}


/*
std::tuple<Eigen::MatrixXd, Eigen::VectorXd, double> Robot::obstacleAvoidanceEquation(
    std::vector<Obstacles> obstacles, Eigen::VectorXd jointAngles, Eigen::VectorXd const &radiusLinks, double distOuter,
    double distStop, double k) {
    int numLinks = this->mdhMatrix.rows() + 1;
    int numObstalces = obstacles.size();
    static std::vector<Obstacles> ObstaclesDynamicArray;
    if (ObstaclesDynamicArray.empty()) {
        ObstaclesDynamicArray = obstacles;
    }

    static std::vector<std::vector<criticalPoints> > criticalPointsDynamicArray;
    if (criticalPointsDynamicArray.empty()) {
        //
    }
    static bool criticalFlag;

    if (criticalFlag == false) {
        criticalFlag = true;
    }
}
*/
