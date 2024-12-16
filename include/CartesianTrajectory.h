//
// Created by shobhit on 26.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_CARTESIANTRAJETORY_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_CARTESIANTRAJETORY_H

#include <Eigen/Dense>
#include <tuple>
#include <string>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <utils.h>

class CartesianTrajectory {
public:
    static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd>
    orientationTrajectory(Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 4> const &orientations,
                          Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
                          double const &ts, std::string const &trajectoryType
    );


    static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> positionTrajectory(
        Eigen::Matrix3Xd const &waypoints,
        Eigen::Matrix<double, 1, Eigen::Dynamic> const &waypointTimes,
        double const &ts, std::string const &trajectoryType, Eigen::Matrix3Xd const &waypointVels,
        Eigen::Matrix3Xd const &waypointAccels
    );

    static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> positionTrajectory(
        AndreiUtils::ConfigurationParameters const &trajConfig);

    static std::tuple<Eigen::MatrixXd, Eigen::MatrixXd, Eigen::MatrixXd> orientationTrajectory(AndreiUtils::ConfigurationParameters const &trajConfig);
};

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CARTESIANTRAJETORY_H
