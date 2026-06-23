//
// Created by shobhit on 29.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEDATATYPES_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEDATATYPES_H

#include <Eigen/Dense>


namespace ObstacleAvoidance {
    /**
     * @brief Geometric representation of a single robot link (its A and D segments and optional final link/tool).
     */
    struct LinkSegment {
        Eigen::VectorXd aSegmentV0;     /**< Start vertex of the A segment. */
        Eigen::VectorXd aSegmentV1;     /**< End vertex of the A segment. */
        Eigen::VectorXd dSegmentV0;     /**< Start vertex of the D segment. */
        Eigen::VectorXd dSegmentV1;     /**< End vertex of the D segment. */
        Eigen::VectorXd toolV0;         /**< Start vertex of the tool segment. */
        Eigen::VectorXd toolV1;         /**< End vertex of the tool segment. */
        double radiusJoint;             /**< Radius of the spherical joint at the segment start. */
        bool hasSegmentA;               /**< Whether this link has an A segment. */
        bool hasSegmentD;               /**< Whether this link has a D segment. */

        Eigen::Vector3d finalLinkDimensionsTCP;     /**< Final-link dimensions expressed at the TCP. */
        Eigen::Quaterniond finalLinkOrientationTCP; /**< Final-link orientation expressed at the TCP. */
        Eigen::Vector3d CenterFinalLink;            /**< Center of the final link. */
        double radiusLink;                          /**< Radius of the cylindrical part of the link. */
    };
    /**
     * @brief Geometric representation of the final link (end-effector to TCP), modeled as a box or sphere.
     */
    struct FinalLinkRobot{
        Eigen::Vector3d finalV0;                /**< Start vertex of the final link (end-effector). */
        Eigen::Vector3d finalV1;                /**< End vertex of the final link (TCP). */
        std::string type;                       /**< Primitive type ("Box" or "Sphere"). */
        Eigen::Vector3d dimensions;             /**< Dimensions of the final-link primitive. */
        Eigen::Quaterniond orientation;         /**< Orientation of the final link. */
        Eigen::Vector3d center;                 /**< Center of the final-link primitive. */
        std::string directionPartialFinalLink;  /**< Axis ("X", "Y", or "Z") used for the partial final-link check. */
    };

}


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEDATATYPES_H
