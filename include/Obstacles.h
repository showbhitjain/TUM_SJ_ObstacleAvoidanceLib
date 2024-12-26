//
// Created by shobhit on 30.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H


#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <string>
#include <tuple>
#include <LinkSegment.h>
#include <ConceptLibrary/instances/geometry/SphereShape.h>
#include <ConceptLibrary/instances/geometry/BoxShape.h>
#include <ConceptLibrary/instances/geometry/CylinderShape.h>
#include <ccd/ccd.h>

namespace ObstacleAvoidance {
    class Obstacles {
    public:
        /*Obstacles(const std::string &obstacleType, const Eigen::VectorXd &center, const Eigen::VectorXd &dimensions,
                  Eigen::Vector4d orientation = {1, 0, 0, 0}, Eigen::Vector3d axis = {0, 0, 0});*/
        Obstacles(std::string const &obstacleType, Eigen::VectorXd const &center, Eigen::VectorXd const &dimensions,
                  Eigen::Vector4d const &orientation = {1, 0, 0, 0}, Eigen::Vector3d const &axis = {0, 0, 0});

        Obstacles(ConceptLibrary::SphereShape const &sphereObstacle);

        Obstacles(ConceptLibrary::CylinderShape const &);

        Obstacles(ConceptLibrary::BoxShape const &);

        std::tuple<Eigen::VectorXd, Eigen::VectorXd, double> distanceCalculation(Obstacles const &otherObstacle);

        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &linkSegmentV0, Eigen::Vector3d const &linkSegmentV1,
                                           double const &linkRadius);

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

    protected:
        std::string obstacleType;
        Eigen::VectorXd Center;
        Eigen::VectorXd dimensions;
        Eigen::Vector4d orientation;
        Eigen::Vector3d axis;
        std::string obstacleName;


    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
