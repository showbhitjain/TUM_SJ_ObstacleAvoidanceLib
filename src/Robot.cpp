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

using namespace AndreiUtils;
using namespace DQ_robotics;
using namespace Eigen;
using namespace ObstacleAvoidance;
using namespace std;
using json = nlohmann::json;

DQ_SerialManipulatorMDH
Robot::createRobotMdhFromPath(std::string const &configFile_Path, std::string const &parameterFor,
                              std::string const &whichrobot, AndreiUtils::Posed const &baseFrame) {
    auto Config_file = ConfigurationParameters(configFile_Path,
                                               parameterFor); //get the file which has path to the robotconfigfile
    auto Config = Config_file.getSubConfig(
            whichrobot); // create config file from class Configurationparameters for the type of robot
    return createRobotMDH(Config, fromPoseToDQ(baseFrame));
}

DQ_SerialManipulatorMDH Robot::createRobotMDH(const ConfigurationParameters &config, DQ const &q) {
    auto mdhParameters = config.get<vector<vector<double>>>("mdhParameters");
    size_t nrJoints = mdhParameters.size();
    MatrixXd mdhMatrix(4, nrJoints);
    bool withHand = config.get<bool>("dhWithHand");

    for (int i = 0; i < nrJoints; i++) {
        for (int j = 0; j < 4; j++) {
            mdhMatrix(j, i) = (j == 0 || j == 3) ? deg2Rad(mdhParameters[i][j]) : mdhParameters[i][j];
//            std::cout << "mdhMatrix(" << j << ", " << i << ") = " << mdhMatrix(j, i) << std::endl;
            /*if (i == nrJoints - 1 && j == 2 && withHand)
            {
                mdhMatrix(j, i) += config.get<double>("dhConnectorToHand");
            }*/
        }
    }
    MatrixXd newMdhMatrix(5, nrJoints);
    newMdhMatrix << mdhMatrix, MatrixXd::Zero(1, nrJoints);
    mdhMatrix = newMdhMatrix;
    std::cout << "The matrix is:\n" << mdhMatrix << std::endl;
    DQ_SerialManipulatorMDH robot(mdhMatrix);
    robot.set_base_frame(q);
    robot.set_reference_frame(q);
    return robot;
}

DQ_SerialManipulator Robot::createRobot_fromconfigfile(const string &configFile, const DQ_robotics::DQ &q) {
    json config = readJsonFile(configFile);
    vector<vector<double>> dhParameters = config["dhParameters"];
    size_t nrJoints = dhParameters.size();
    MatrixXd dhMatrix(5, nrJoints);
    bool withHand = config["dhWithHand"];

    for (int i = 0; i < nrJoints; i++) {
        for (int j = 0; j < 5; j++) {
            dhMatrix(j, i) = (j == 0 || j == 3) ? deg2Rad(dhParameters[i][j]) : dhParameters[i][j];
            if (i == nrJoints - 1 && j == 2 && withHand) {
                dhMatrix(j, i) += double(config["dhConnectorToHand"]);
            }
        }
    }

    DQ_SerialManipulator robot(dhMatrix, "standard");
    robot.set_base_frame(q);
    robot.set_reference_frame(q);
    return robot;
}

//configFile_Path : path to the file which containes paths to config file for the type of robot
//which_robot = type of robot
Robot::Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot,
             const AndreiUtils::Posed &baseFrame) : robotmdh(
        createRobotMdhFromPath(configFile_Path, parameterFor, whichrobot, baseFrame)) {
    auto Config_file = ConfigurationParameters(configFile_Path,
                                               parameterFor); //get the file which has path to the robotconfigfile
    this->Config = Config_file.getSubConfig(
            whichrobot); // create config file from class Configurationparameters for the type of robot
    auto s = createRobotMDH(Config, fromPoseToDQ(baseFrame));
    std::shared_ptr<DQ_SerialManipulator> ro;
    this->robot = make_shared<DQ_SerialManipulatorMDH>(std::move(s));
    std::vector<double> eul = Config.getJson("displacementEEtoTCP").at("rotationXYZ").get<std::vector<double>>();
    for (int i =0; i<eul.size(),i++;){
        eul[i] = deg2Rad(eul[i]);
    }
    std::vector<double> translation = Config.getJson("displacementEEtoTCP").at(
            "translation").get<std::vector<double>>();

    this->transformationEEToTCP =
            trvec2tform(stdVectorToEigenVector(translation)) * convertEulerToTransform(stdVectorToEigenVector(eul));

    std::cout << "Fetching joints config under: " << whichrobot + "/Joints" << std::endl;

    auto Config_joints = Config.getSubConfig("Joints");
    this->joints = make_shared<Joints>(Config_joints);
   /* this->number_joints = Config_joints.get<size_t>("numberOfJoints");
    auto nr = number_joints;*/

}

VectorXd Robot::getRobotJointValues() const {
    return this->joints->values;
}

DQ Robot::getRobotPose() const {
    return this->fkm();
}

VectorXd Robot::getCurrentRobotJointValues() {
    return this->joints->getCurrentJointValues();
}

DQ Robot::getCurrentRobotPose() {
    return this->fkm(this->getCurrentRobotJointValues());
}

/*void Robot::updateRobotJointValues() {
    this->joints->update();
}*/

void Robot::setJointValues(VectorXd const &jointValues) {
    this->joints->setJointValues(jointValues);
}

DQ Robot::fkm() const {
    return this->fkm(this->joints->values);
}

DQ Robot::fkm(VectorXd const &jointValues) const {
    return this->robot->fkm(jointValues);
}

MatrixXd Robot::jacobian() const {
    return this->jacobian(this->joints->values);
}

MatrixXd Robot::jacobian(Eigen::VectorXd const &jointValues) const {
    return this->robot->pose_jacobian(jointValues);
}

//Endeffector Position in x,y Z
Eigen::VectorXd Robot::fkm_cartesian(const VectorXd &jointValues) const {
    auto endeffector_dq = this->robot->fkm(jointValues);
    auto endeffector_pose = fromDQToPose(endeffector_dq);
    return endeffector_pose.getTranslation();
}

//ith link position in x.y,z
Eigen::VectorXd Robot::fkm_cartesian(Eigen::VectorXd const &jointValues, const int &to_ith_link) const {
    auto ithlink_pose_dq = this->robot->fkm(jointValues, to_ith_link);
    auto ithlink_pose = fromDQToPose(ithlink_pose_dq);
    return ithlink_pose.getTranslation();
}

//Transformation from i to 0 (i th link to base frame)
Eigen::MatrixXd Robot::forwardKinematics(const Eigen::VectorXd &jointValues, const int &toIthLink) const {
    int i = toIthLink - 1;
    auto ithlink_transformation_dq = this->robotmdh.raw_fkm(jointValues, i);
    return fromDQToPose(ithlink_transformation_dq).getTransformationMatrix();
}

size_t Robot::getNumberJoints() const {
    return this->joints->number_joints;
}

const shared_ptr<Joints> &Robot::getJoints() const {
    return joints;
}

Eigen::MatrixXd Robot::jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const {

    int numJoints = jointValues.size(); // Number of joints
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, numJoints); // Initialize Jacobian matrix with zeros

    // Compute the transformation to the end-effector or toIthLink
    Eigen::Matrix4d T_end_effector = forwardKinematics(jointValues, toIthLink);
    Eigen::Vector3d end_effector_position = T_end_effector.block<3, 1>(0, 3);


    for (int i = 1; i <= toIthLink; ++i) {
        // Get the transformation matrix from the base frame to the ith joint

        Eigen::Matrix4d T_i = forwardKinematics(jointValues, i);

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


    int numJoints = jointValues.size(); // Number of joints
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(6, numJoints); // Initialize Jacobian matrix with zeros

    // Compute the transformation to the end-effector or toIthLink

    Eigen::Matrix4d T_end_effector = forwardKinematics(jointValues, toIthLink) * transformationRelative;
    Eigen::Vector3d end_effector_position = T_end_effector.block<3, 1>(0, 3);


    for (int i = 1; i <= toIthLink; ++i) {
        // Get the transformation matrix from the base frame to the ith joint

        Eigen::Matrix4d T_i = forwardKinematics(jointValues, i);

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

    return jacobianCartesianOnLink(jointValues, static_cast<int>(this->joints->number_joints), this->transformationEEToTCP);
}


