//
// Created by shobhit on 26.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_DISTANCEOBJECTS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_DISTANCEOBJECTS_H

#include <Eigen/Dense>
#include <tuple>

namespace ObstacleAvoidance {
    /**
     * @brief Compute the distance and closest points between two spheres.
     * @param radius1 Radius of the first sphere.
     * @param center1 Center of the first sphere.
     * @param radius2 Radius of the second sphere.
     * @param center2 Center of the second sphere.
     * @return Tuple of (signed surface distance, closest point on sphere 1, closest point on sphere 2).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereSphere(const double &radius1, const Eigen::Vector3d &center1, const double &radius2,
                                  const Eigen::Vector3d &center2);

    /**
     * @brief Compute the distance and closest points between a sphere and a cylinder.
     * @param radiusSphere Radius of the sphere.
     * @param sphereCenter Center of the sphere.
     * @param radiusCylinder Radius of the cylinder.
     * @param cylinderHeight Total height (length) of the cylinder.
     * @param cylinderCenter Center of the cylinder (midpoint along its axis).
     * @param cylinderAxis Direction vector of the cylinder's axis.
     * @return Tuple of (signed surface distance, closest point on sphere, closest point on cylinder).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereCylinder(double const &radiusSphere, Eigen::Vector3d const &sphereCenter,
                                    double const &radiusCylinder, double const &cylinderHeight,
                                    Eigen::Vector3d const &cylinderCenter, Eigen::Vector3d const &cylinderAxis);

    /**
     * @brief Compute the distance and closest points between a sphere and an oriented box.
     * @param radiusSphere Radius of the sphere.
     * @param sphereCenter Center of the sphere.
     * @param boxCenter Center of the box.
     * @param boxDimensions Full dimensions (width, height, depth) of the box.
     * @param boxOrientation Orientation of the box.
     * @return Tuple of (surface distance (>= 0), closest point on sphere, closest point on box).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereBox(double const &radiusSphere, Eigen::Vector3d const &sphereCenter,
                               Eigen::Vector3d const &boxCenter, Eigen::Vector3d const &boxDimensions,
                               Eigen::Quaterniond const &boxOrientation);

    /**
     * @brief Compute the signed distance and closest points between two oriented boxes.
     * @param boxCenter1 Center of the first box.
     * @param boxDimensions1 Full dimensions of the first box.
     * @param boxOrientation1 Orientation of the first box.
     * @param boxCenter2 Center of the second box.
     * @param boxDimensions2 Full dimensions of the second box.
     * @param boxOrientation2 Orientation of the second box.
     * @return Tuple of (signed distance, closest point on box 1, closest point on box 2).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceBoxBox(Eigen::Vector3d const &boxCenter1, Eigen::Vector3d const &boxDimensions1,
                            Eigen::Quaterniond const &boxOrientation1, Eigen::Vector3d const &boxCenter2,
                            Eigen::Vector3d const &boxDimensions2,
                            Eigen::Quaterniond const &boxOrientation2);

    /**
     * @brief Compute the signed distance and closest points between an oriented box and a cylinder.
     * @param boxCenter Center of the box.
     * @param boxDimensions Full dimensions of the box.
     * @param boxOrientation Orientation of the box.
     * @param radiusCylinder Radius of the cylinder.
     * @param cylinderHeight Total height (length) of the cylinder.
     * @param cylinderCenter Center of the cylinder.
     * @param cylinderAxis Direction vector of the cylinder's axis.
     * @return Tuple of (signed distance, closest point on box, closest point on cylinder).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceBoxCylinder(Eigen::Vector3d const &boxCenter, Eigen::Vector3d const &boxDimensions,
                                 Eigen::Quaterniond const &boxOrientation, double const &radiusCylinder,
                                 double const &cylinderHeight,
                                 Eigen::Vector3d const &cylinderCenter, Eigen::Vector3d const &cylinderAxis);

    /**
     * @brief Compute the signed distance and closest points between two oriented cylinders.
     * @param radiusCylinder1 Radius of the first cylinder.
     * @param cylinderHeight1 Total height of the first cylinder.
     * @param cylinderCenter1 Center of the first cylinder.
     * @param cylinderAxis1 Direction vector of the first cylinder's axis.
     * @param radiusCylinder2 Radius of the second cylinder.
     * @param cylinderHeight2 Total height of the second cylinder.
     * @param cylinderCenter2 Center of the second cylinder.
     * @param cylinderAxis2 Direction vector of the second cylinder's axis.
     * @return Tuple of (signed distance, closest point on cylinder 1, closest point on cylinder 2).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceCylinderCylinder(double radiusCylinder1, double cylinderHeight1,
                                      const Eigen::Vector3d &cylinderCenter1,
                                      const Eigen::Vector3d &cylinderAxis1,
                                      double radiusCylinder2, double cylinderHeight2,
                                      const Eigen::Vector3d &cylinderCenter2,
                                      const Eigen::Vector3d &cylinderAxis2);

    /**
     * @brief Compute the distance and closest points between a sphere and a line-swept-sphere (capsule) link.
     * @param sphereRadius Radius of the sphere.
     * @param sphereCenter Center of the sphere.
     * @param linkSegmentV0 Start vertex of the link segment.
     * @param linkSegmentV1 End vertex of the link segment.
     * @param linkRadius Radius of the swept sphere (link thickness).
     * @return Tuple of (signed surface distance, distance vector, closest point on sphere, closest point on link).
     */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereLineSweptSphere(double const &sphereRadius, Eigen::Vector3d const &sphereCenter,
                                           Eigen::Vector3d const &linkSegmentV0,
                                           Eigen::Vector3d const &linkSegmentV1,
                                           double const &linkRadius);

}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_DISTANCEOBJECTS_H
