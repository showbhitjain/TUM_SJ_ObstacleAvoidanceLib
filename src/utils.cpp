//
// Created by shobhit on 12.10.24.
//

#include "utils.h"

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




