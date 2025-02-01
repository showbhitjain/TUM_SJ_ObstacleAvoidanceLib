//
// Created by shobhit on 26.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_DISTANCEOBJECTS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_DISTANCEOBJECTS_H

#include <Eigen/Dense>
#include <tuple>

namespace ObstacleAvoidance {
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereSphere(const double &radius1, const Eigen::Vector3d &center1, const double &radius2,
                                  const Eigen::Vector3d &center2);

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereCylinder(double const &radiusSphere, Eigen::Vector3d const &sphereCenter,
                                    double const &radiusCylinder, double const &cylinderHeight,
                                    Eigen::Vector3d const &cylinderCenter, Eigen::Vector3d const &cylinderAxis);

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereBox(double const &radiusSphere, Eigen::Vector3d const &sphereCenter,
                               Eigen::Vector3d const &boxCenter, Eigen::Vector3d const &boxDimensions,
                               Eigen::Quaterniond const &boxOrientation);

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceBoxBox(Eigen::Vector3d const &boxCenter1, Eigen::Vector3d const &boxDimensions1,
                            Eigen::Quaterniond const &boxOrientation1, Eigen::Vector3d const &boxCenter2,
                            Eigen::Vector3d const &boxDimensions2,
                            Eigen::Quaterniond const &boxOrientation2);

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceBoxCylinder(Eigen::Vector3d const &boxCenter, Eigen::Vector3d const &boxDimensions,
                                 Eigen::Quaterniond const &boxOrientation, double const &radiusCylinder,
                                 double const &cylinderHeight,
                                 Eigen::Vector3d const &cylinderCenter, Eigen::Vector3d const &cylinderAxis);

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceCylinderCylinder(double radiusCylinder1, double cylinderHeight1,
                                      const Eigen::Vector3d &cylinderCenter1,
                                      const Eigen::Vector3d &cylinderAxis1,
                                      double radiusCylinder2, double cylinderHeight2,
                                      const Eigen::Vector3d &cylinderCenter2,
                                      const Eigen::Vector3d &cylinderAxis2);

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereLineSweptSphere(double const &sphereRadius, Eigen::Vector3d const &sphereCenter,
                                           Eigen::Vector3d const &linkSegmentV0,
                                           Eigen::Vector3d const &linkSegmentV1,
                                           double const &linkRadius);

}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_DISTANCEOBJECTS_H
