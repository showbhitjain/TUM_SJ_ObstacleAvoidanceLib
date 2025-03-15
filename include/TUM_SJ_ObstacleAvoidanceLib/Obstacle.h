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

#include "robotLinkDataTypes.h"

struct linkProperty{
    bool useLinkSegmentA;
    bool useLinkSegmentD;
    bool useFinalLink;
};


namespace ObstacleAvoidance {
    class Obstacle {
    public:
        /*Obstacles(const std::string &obstacleType, const Eigen::VectorXd &center, const Eigen::VectorXd &dimensions,
                  Eigen::Vector4d orientation = {1, 0, 0, 0}, Eigen::Vector3d axis = {0, 0, 0});*/
        Obstacle(std::string const &obstacleType, Eigen::Vector3d const &center, Eigen::VectorXd const &dimensions,
                  Eigen::Vector4d const &orientation = {1, 0, 0, 0}, Eigen::Vector3d const &axis = {0, 0, 0});

        Obstacle(ConceptLibrary::SphereShape const &sphereObstacle);

        Obstacle(ConceptLibrary::CylinderShape const &);

        Obstacle(ConceptLibrary::BoxShape const &);

        Obstacle(ConceptLibrary::PlaneWidthShape const &);

        Obstacle(Eigen::Vector3d const &vertexV0,Eigen::Vector3d const &vertexV1,double const &radius);

        std::tuple<Eigen::VectorXd, Eigen::VectorXd, double> distanceCalculation(Obstacle const &otherObstacle);

        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceFinalLinkObstacle(FinalLinkRobot const &linkEEtoTCP, bool const &usePartialBoxLink) const ;


        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkLineSweptObstacleSphere(Eigen::Vector3d const &linkSegmentV0,
                                                          Eigen::Vector3d const &linkSegmentV1,
                                                          double const &linkRadius) const;


        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &startVertex, Eigen::Vector3d const &endVertex,
                                           double const &radiusLink, double const &radiusJoint,bool const &robotLinkAsLineSwept,bool const &usePartialLink) const;

//        calculateDistanceLinkToLink()
        /*RobotController(config, exitFlag,
                       readJsonFile("../config/robotWorldCalibration.json").at("q_world_base").get<Posed>())*/


        std::string obstacleType;
        Eigen::Vector3d center;
        Eigen::VectorXd dimensions;
        Eigen::Quaterniond orientation;
        Eigen::Vector3d axis;
        std::string obstacleName;
        double distanceActivate;
        //int = partialLinkStartIndex
        //bool = doPartialSelfCollision
        std::map<int,linkProperty> partialSelfCollision;
        int startIndex;


    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
