//
// Created by shobhit on 26.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_CARTESIANTRAJETORY_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_CARTESIANTRAJETORY_H

#include <Eigen/Dense>
#include <tuple>
#include <string>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>


namespace ObstacleAvoidance {
    class CartesianTrajectory {
    public:
        /**
         * @brief Generate a TCP orientation trajectory by interpolating between waypoint orientations.
         * @param orientations Waypoint orientations as quaternion columns (w, x, y, z).
         * @param waypointTimes Time stamp of each waypoint (row vector).
         * @param ts Trajectory sample time.
         * @param trajectoryType Time-scaling/interpolation type (e.g. "cubic").
         * @return Tuple of (desired orientations as quaternions, desired angular velocities, desired angular accelerations).
         */
        static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
        orientationTrajectory(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 4> const &orientations,
                              Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
                              double const &ts, std::string const &trajectoryType
        );


        /**
         * @brief Generate a TCP position trajectory by interpolating between waypoints.
         * @param waypoints 3D waypoints as columns [x, y, z].
         * @param waypointTimes Time stamp of each waypoint (row vector).
         * @param ts Trajectory sample time.
         * @param trajectoryType Interpolation method (e.g. "cubic", "quintic").
         * @param waypointVels Desired velocities at the waypoints.
         * @param waypointAccels Desired accelerations at the waypoints.
         * @return Tuple of (desired positions, desired velocities, desired accelerations).
         */
        static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> positionTrajectory(
            Eigen::Matrix3Xd const &waypoints,
            Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
            double const &ts, std::string const &trajectoryType, Eigen::Matrix3Xd const &waypointVels,
            Eigen::Matrix3Xd const &waypointAccels
        );

        /**
         * @brief Generate a TCP position trajectory from a trajectory configuration.
         * @param trajConfig Trajectory configuration (waypoints, sample time, type, waypoint times).
         * @return Tuple of (desired positions, desired velocities, desired accelerations).
         */
        static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> positionTrajectory(
            AndreiUtils::ConfigurationParameters const &trajConfig);

        /**
         * @brief Generate a TCP orientation trajectory from a trajectory configuration.
         * @param trajConfig Trajectory configuration (orientations, sample time, type, waypoint times).
         * @return Tuple of (desired orientations as quaternions, desired angular velocities, desired angular accelerations).
         */
        static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> orientationTrajectory(
            AndreiUtils::ConfigurationParameters const &trajConfig);
    };
}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CARTESIANTRAJETORY_H
