//
// Created by shobhit on 26.10.24.
//

#include <cartesianTrajetory.h>
#include <coder_array.h>
#include <Trajectory.h>
#include <privateUtils.hpp>
#include <memory>

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
cartesianTrajetory::orientationTrajectory(
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 4> const &orientations,
        Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
        double const &ts, std::string const &trajType) {

    ::coder::array<double, 2U> orientationsMatlab = EigenToCoder(orientations);
    ::coder::array<double, 2U> waypointTimesMatlab = EigenToCoder(waypointTimes);
    ::coder::array<char, 2U> trajTypeMatlab(trajType);
    std::shared_ptr<trajectoryGeneration::Trajectory> traj;
    traj = std::make_shared<trajectoryGeneration::Trajectory>();
    ::coder::array<double, 2U> desiredQuaternionsMatlab;
    ::coder::array<double, 2U> desiredAngularVelocityMatlab;
    ::coder::array<double, 2U> desiredAngularAccelMatlab;
    traj->orientationTrajectory(orientationsMatlab, waypointTimesMatlab, ts, trajTypeMatlab, desiredQuaternionsMatlab,
                               desiredAngularVelocityMatlab, desiredAngularAccelMatlab);
    Eigen::MatrixXd desiredQuaternions = coderToEigen(desiredQuaternionsMatlab);
    Eigen::MatrixXd desiredAngularVelocity = coderToEigen(desiredAngularVelocityMatlab);
    Eigen::MatrixXd desiredAngularAccel = coderToEigen(desiredAngularAccelMatlab);
    return std::make_tuple(desiredQuaternions, desiredAngularVelocity, desiredAngularAccel);
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
cartesianTrajetory::positionTrajectory(Eigen::Matrix3Xd const &waypoints,
                                       Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
                                       double const &ts,  std::string const &trajType,
                                       Eigen::Matrix3Xd const &waypointVels, Eigen::Matrix3Xd const &waypointAccels) {

    ::coder::array<double, 2U> waypointsMatlab = EigenToCoder(waypoints);
    ::coder::array<double, 2U> waypointTimesMatlab = EigenToCoder(waypointTimes);
    ::coder::array<char, 2U> trajTypeMatlab(trajType);
    ::coder::array<double, 2U> waypointVelsMatlab = EigenToCoder(waypointVels);
    ::coder::array<double, 2U> waypointAccelsMatlab = EigenToCoder(waypointAccels);
    ::coder::array<double, 2U> xdMatlab;
    ::coder::array<double, 2U> xd_velMatlab;
    ::coder::array<double, 2U> xddMatlab;
    std::shared_ptr<trajectoryGeneration::Trajectory> traj;
    traj = std::make_shared<trajectoryGeneration::Trajectory>();
    traj->positionTrajectory(waypointsMatlab, waypointTimesMatlab, ts, trajTypeMatlab, waypointVelsMatlab,
                            waypointAccelsMatlab, xdMatlab, xd_velMatlab, xddMatlab);
    Eigen::MatrixXd xd = coderToEigen(xdMatlab);
    Eigen::MatrixXd xd_vel = coderToEigen(xd_velMatlab);
    Eigen::MatrixXd xdd = coderToEigen(xddMatlab);
    return std::make_tuple(xd, xd_vel, xdd);

}
