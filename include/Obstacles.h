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
#include <ConceptLibrary/instances/geometry/PlaneShape.h>

#include <robotLinkDataTypes.h>

namespace ObstacleAvoidance {
    class Obstacles {
    public:
        /*Obstacles(const std::string &obstacleType, const Eigen::VectorXd &center, const Eigen::VectorXd &dimensions,
                  Eigen::Vector4d orientation = {1, 0, 0, 0}, Eigen::Vector3d axis = {0, 0, 0});*/
        Obstacles(std::string const &obstacleType, Eigen::Vector3d const &center, Eigen::VectorXd const &dimensions,
                  Eigen::Vector4d const &orientation = {1, 0, 0, 0}, Eigen::Vector3d const &axis = {0, 0, 0});

        Obstacles(ConceptLibrary::SphereShape const &sphereObstacle);

        Obstacles(ConceptLibrary::CylinderShape const &);

        Obstacles(ConceptLibrary::BoxShape const &);

        Obstacles(ConceptLibrary::PlaneWidthShape const &);

        Obstacles(Eigen::Vector3d const &vertexV0,Eigen::Vector3d const &vertexV1,double const &radius);

        std::tuple<Eigen::VectorXd, Eigen::VectorXd, double> distanceCalculation(Obstacles const &otherObstacle);

        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceFinalLinkObstacle(finalLinkRobot const &linkEEtoTCP) const ;


        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkLineSweptObstacleSphere(Eigen::Vector3d const &linkSegmentV0,
                                                          Eigen::Vector3d const &linkSegmentV1,
                                                          double const &linkRadius) const;


        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &startVertex, Eigen::Vector3d const &endVertex,
                                           double const &radiusLink, double const &radiusJoint,bool const &robotLinkAsLineSwept) const;



        /*RobotController(config, exitFlag,
                       readJsonFile("../config/robotWorldCalibration.json").at("q_world_base").get<Posed>())*/


        std::string obstacleType;
        Eigen::Vector3d center;
        Eigen::VectorXd dimensions;
        Eigen::Quaterniond orientation;
        Eigen::Vector3d axis;
        std::string obstacleName;


    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
