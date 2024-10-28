//
// Created by shobhit on 12.10.24.
//

#include <utils.h>

using namespace AndreiUtils;
using namespace DQ_robotics;
using namespace Eigen;
using namespace RobotModelling;
using namespace std;

Vector3d RobotModelling::tFromDQ(DQ const &q) {
    return q.translation().q.segment(1, 3);
}

DQ RobotModelling::fromPoseToDQ(Pose const &pose) {
    Eigen::Quaterniond r = pose.getRotation(), d = pose.getDual();
    DQ q(r.w(), r.x(), r.y(), r.z(), d.w(), d.x(), d.y(), d.z());
    return q.normalize();
}

Pose RobotModelling::fromDQToPose(DQ const &pose) {
    return {{pose.q[0], pose.q[1], pose.q[2], pose.q[3]}, Quaterniond{pose.q[4], pose.q[5], pose.q[6], pose.q[7]}};
}

Eigen::Matrix4d RobotModelling::trvec2tform(const Eigen::Vector3d& translation) {
    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();  // Start with an identity matrix
    transform.block<3, 1>(0, 3) = translation;               // Insert the translation vector
    return transform;
}

Eigen::Matrix4d RobotModelling::eul2tform(const Eigen::Vector3d& eulerAngles) {
    // Extract individual angles
    double roll = eulerAngles(0);  // Roll (rotation about X-axis)
    double pitch = eulerAngles(1); // Pitch (rotation about Y-axis)
    double yaw = eulerAngles(2);   // Yaw (rotation about Z-axis)

    // Calculate rotation matrix components
    Eigen::Matrix3d rotation;
    rotation = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
               Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
               Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());

    // Create transformation matrix
    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
    transform.block<3, 3>(0, 0) = rotation;

    return transform;
}

Eigen::Matrix4d RobotModelling::convertEulerToTransform(const Eigen::Vector3d& angles, const std::string& seq ) {


    Eigen::Matrix3d Rz = Eigen::AngleAxisd(angles(2), Eigen::Vector3d::UnitZ()).toRotationMatrix();
    Eigen::Matrix3d Ry = Eigen::AngleAxisd(angles(1), Eigen::Vector3d::UnitY()).toRotationMatrix();
    Eigen::Matrix3d Rx = Eigen::AngleAxisd(angles(0), Eigen::Vector3d::UnitX()).toRotationMatrix();

    // Assuming ZYX order
    Eigen::Matrix3d rotation = Rz * Ry * Rx;

    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
    transform.block<3, 3>(0, 0) = rotation;
    return transform;
}




