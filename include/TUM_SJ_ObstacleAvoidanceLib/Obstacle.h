//
// Created by shobhit on 30.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H


#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <string>
#include <tuple>
#include <map>


#include "robotLinkDataTypes.h"
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

/**
 * @brief Flags selecting which parts of a robot link participate in a (partial) self-collision check.
 */
struct linkProperty{
    bool useLinkSegmentA;   /**< Consider the link's A segment. */
    bool useLinkSegmentD;   /**< Consider the link's D segment. */
    bool useFinalLink;      /**< Consider the final link. */
};


namespace ObstacleAvoidance {
    class Obstacle {
    public:

        /**
         * @brief Construct an obstacle from explicit geometric parameters.
         * @param obstacleType Primitive type ("Sphere", "Box", or "Cylinder").
         * @param center Center of the obstacle.
         * @param dimensions Type-specific dimensions (e.g. radius, box extents, or [radius, height]).
         * @param orientation Orientation quaternion (w, x, y, z); defaults to identity.
         * @param axis Axis direction (used for cylinders); defaults to zero.
         */
        Obstacle(std::string const &obstacleType, Eigen::Vector3d const &center, Eigen::VectorXd const &dimensions,
                  Eigen::Vector4d const &orientation = {1, 0, 0, 0}, Eigen::Vector3d const &axis = {0, 0, 0});

        /**
         * @brief Construct an obstacle from a JSON description.
         * @param obstacleJson JSON object with "type", "dimensions", "center", "axis", and "orientation".
         */
       Obstacle( nlohmann::json const &obstacleJson);


        /**
         * @brief Construct a cylindrical obstacle (e.g. a robot link) from two end vertices and a radius.
         * @param vertexV0 Start vertex of the cylinder axis.
         * @param vertexV1 End vertex of the cylinder axis.
         * @param radius Cylinder radius.
         */
        Obstacle(Eigen::Vector3d const &vertexV0,Eigen::Vector3d const &vertexV1,double const &radius);

        /**
         * @brief Compute distance information to another obstacle.
         * @param otherObstacle The other obstacle.
         * @return Tuple of (closest point on this obstacle, closest point on the other obstacle, distance).
         */
        std::tuple<Eigen::VectorXd, Eigen::VectorXd, double> distanceCalculation(Obstacle const &otherObstacle);

        /**
         * @brief Compute distance and closest points between this obstacle and the robot final link.
         * @param linkEEtoTCP Final-link representation (end-effector to TCP).
         * @param usePartialBoxLink If true, only the outer half of a box final link is considered.
         * @return Tuple of (signed distance, closest point on obstacle, closest point on final link).
         */
        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceFinalLinkObstacle(FinalLinkRobot const &linkEEtoTCP, bool const &usePartialBoxLink) const ;


        /**
         * @brief Compute distance and closest points between a line-swept-sphere link and a spherical obstacle.
         * @param linkSegmentV0 Start vertex of the link segment.
         * @param linkSegmentV1 End vertex of the link segment.
         * @param linkRadius Radius of the swept sphere (link thickness).
         * @return Tuple of (signed distance, closest point on obstacle, closest point on link).
         */
        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkLineSweptObstacleSphere(Eigen::Vector3d const &linkSegmentV0,
                                                          Eigen::Vector3d const &linkSegmentV1,
                                                          double const &linkRadius) const;


        /**
         * @brief Compute distance and closest points between this obstacle and a robot link segment.
         * @param startVertex Start vertex of the link segment.
         * @param endVertex End vertex of the link segment.
         * @param radiusLink Radius of the cylindrical part of the link.
         * @param radiusJoint Radius of the spherical joint at the segment start.
         * @param robotLinkAsLineSwept If true, model the link as a line-swept sphere; otherwise as cylinder + sphere.
         * @param usePartialLink If true, only the outer half of the link segment is considered.
         * @return Tuple of (signed distance, closest point on obstacle, closest point on link).
         */
        std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
        calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &startVertex, Eigen::Vector3d const &endVertex,
                                           double const &radiusLink, double const &radiusJoint,bool const &robotLinkAsLineSwept,bool const &usePartialLink) const;


        std::string obstacleType;               /**< Primitive type ("Sphere", "Box", or "Cylinder"). */
        Eigen::Vector3d center;                  /**< Center of the obstacle. */
        Eigen::VectorXd dimensions;              /**< Type-specific dimensions. */
        Eigen::Quaterniond orientation;          /**< Orientation of the obstacle. */
        Eigen::Vector3d axis;                    /**< Axis direction (cylinders). */
        std::string obstacleName;                /**< Optional obstacle name. */
        double distanceActivate;                 /**< Per-obstacle activation distance. */
        std::map<int,linkProperty> partialSelfCollision; /**< Per-link partial self-collision settings (key: link start index). */
        int startIndex;                          /**< First link index considered against this obstacle. */


    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
