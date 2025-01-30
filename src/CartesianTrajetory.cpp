//
// Created by shobhit on 26.10.24.
//

#include <CartesianTrajectory.h>
#include <coder_array.h>
#include <Trajectory.h>
#include <privateUtils.h>
#include <memory>

using namespace AndreiUtils;
using namespace ObstacleAvoidance;
using namespace std;

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
CartesianTrajectory::orientationTrajectory(
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 4> const &orientations,
        Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
        double const &ts, std::string const &trajType) {
    const ::coder::array<double, 2U> orientationsMatlab = EigenToCoder(orientations);
    const ::coder::array<double, 2U> waypointTimesMatlab = EigenToCoder(waypointTimes);
    const ::coder::array<char, 2U> trajectoryTypeMatlab(trajType);
    const auto trajectoryPtr = std::make_shared<trajectoryGeneration::Trajectory>();
    ::coder::array<double, 2U> desiredQuaternionsMatlab;
    ::coder::array<double, 2U> desiredAngularVelocityMatlab;
    ::coder::array<double, 2U> desiredAngularAccelMatlab;
    trajectoryPtr->orientationTrajectory(orientationsMatlab, waypointTimesMatlab, ts, trajectoryTypeMatlab,
                                         desiredQuaternionsMatlab,
                                         desiredAngularVelocityMatlab, desiredAngularAccelMatlab);
    Eigen::MatrixXd desiredQuaternions = coderToEigen(desiredQuaternionsMatlab);
    Eigen::MatrixXd desiredAngularVelocity = coderToEigen(desiredAngularVelocityMatlab);
    Eigen::MatrixXd desiredAngularAccel = coderToEigen(desiredAngularAccelMatlab);
    return std::make_tuple(desiredQuaternions, desiredAngularVelocity, desiredAngularAccel);
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
CartesianTrajectory::positionTrajectory(Eigen::Matrix3Xd const &waypoints,
                                        Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
                                        double const &ts, std::string const &trajectoryType,
                                        Eigen::Matrix3Xd const &waypointVels, Eigen::Matrix3Xd const &waypointAccels) {

    ::coder::array<double, 2U> waypointsMatlab = EigenToCoder(waypoints);
    ::coder::array<double, 2U> waypointTimesMatlab = EigenToCoder(waypointTimes);
    ::coder::array<char, 2U> trajTypeMatlab(trajectoryType);
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

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
CartesianTrajectory::positionTrajectory(ConfigurationParameters const &trajConfig) {

    auto waypointsParams = trajConfig.get<std::vector<std::vector<double>>>("Waypoints");
    auto Waypoints = vectorMatrixToEigenMatrix(waypointsParams);
    Eigen::MatrixXd waypointVelocities = Eigen::MatrixXd::Zero(Waypoints.rows(), Waypoints.cols());
    Eigen::MatrixXd waypointAccelerations = Eigen::MatrixXd::Zero(Waypoints.rows(), Waypoints.cols());
    auto ts = trajConfig.get<double>("trajectorySampleTime");
    auto positionTrajectoryType = trajConfig.get<std::string>("positionTrajectoryType");
    auto WaypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double>>("waypointTimes"));

    return CartesianTrajectory::positionTrajectory(Waypoints, WaypointTimes, ts, positionTrajectoryType,
                                                   waypointVelocities, waypointAccelerations);

}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
CartesianTrajectory::orientationTrajectory(ConfigurationParameters const &trajConfig) {


    auto orientationParams = trajConfig.get<vector<vector<double>>>("orientations");
    auto orientations = vectorMatrixToEigenMatrix(orientationParams);
    auto orientationTrajectoryType = trajConfig.get<std::string>("orientationTrajectoryType");
    auto WaypointTimes = vectorToEigenMatrixRow(trajConfig.get<std::vector<double>>("waypointTimes"));
    auto ts = trajConfig.get<double>("trajectorySampleTime");

    return CartesianTrajectory::orientationTrajectory(orientations, WaypointTimes, ts, orientationTrajectoryType);
}

