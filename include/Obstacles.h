//
// Created by shobhit on 30.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H


#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <string>
#include <tuple>

#include <ConceptLibrary/instances/geometry/SphereShape.h>
#include <ConceptLibrary/instances/geometry/BoxShape.h>
#include <ConceptLibrary/instances/geometry/CylinderShape.h>
#include <robotLinkDataTypes.h>

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

        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceFinalLinkObstacle(finalLinkRobot const &linkEEtoTCP);


        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkLineSweptObstacleSphere(Eigen::Vector3d const &linkSegmentV0,
                                                          Eigen::Vector3d const &linkSegmentV1,
                                                          double const &linkRadius);


        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &startVertex, Eigen::Vector3d const &endVertex,
                                           double const &radiusLink, double const &radiusJoint);




        std::string obstacleType;
        Eigen::VectorXd center;
        Eigen::VectorXd dimensions;
        Eigen::Quaterniond orientation;
        Eigen::Vector3d axis;
        std::string obstacleName;


    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
