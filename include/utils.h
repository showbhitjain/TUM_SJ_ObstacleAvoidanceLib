//
// Created by shobhit on 12.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <dqrobotics/DQ.h>
#include <Eigen/Dense>
#include <vector>

namespace RobotModelling{
    Eigen::Vector3d tFromDQ(DQ_robotics::DQ const &q);

    DQ_robotics::DQ fromPoseToDQ(AndreiUtils::Pose const &pose);

    AndreiUtils::Pose fromDQToPose(DQ_robotics::DQ const &pose);

    Eigen::Matrix4d trvec2tform(Eigen::Vector3d const &translation);

    Eigen::Matrix4d eul2tform(Eigen::Vector3d const &eulerAngles);


    Eigen::Matrix4d convertEulerToTransform(Eigen::Vector3d const &angles, std::string const &seq = "ZYX");
}


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H
