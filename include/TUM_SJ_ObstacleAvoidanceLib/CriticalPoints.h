//
// Created by shobhit on 11.12.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H

#include <Eigen/Dense>


/**
 * @brief Per-obstacle, per-link state of the closest (critical) points used for obstacle avoidance.
 *
 * Holds, for each link channel (A segment, D segment, and final link), the distance vectors,
 * distances, activation flags, Jacobians, and cached joint angles/velocities required to build
 * the obstacle-avoidance constraints across optimization steps.
 */
class CriticalPoints {
public:
    Eigen::Vector3d distVectorA;            /**< Distance vector for the A segment. */
    Eigen::Vector3d distVectorD;            /**< Distance vector for the D segment. */
    Eigen::Vector3d distVectorFinalLink;    /**< Distance vector for the final link. */

    double distanceA;                       /**< Distance for the A segment. */
    double distanceD;                       /**< Distance for the D segment. */
    double distanceFinalLink;               /**< Distance for the final link. */
    double distance;                        /**< Minimum distance across the link channels. */

    bool hasCriticalPointA;                 /**< Whether the A segment currently has a critical point. */
    bool hasCriticalPointD;                 /**< Whether the D segment currently has a critical point. */
    bool hasCriticalPointFinalLink;         /**< Whether the final link currently has a critical point. */

    Eigen::MatrixXd jacobiDistMaxA;         /**< Distance Jacobian at the recorded critical configuration (A). */
    Eigen::MatrixXd jacobiDistMaxD;         /**< Distance Jacobian at the recorded critical configuration (D). */
    Eigen::MatrixXd jacobiDistMaxFinalLink; /**< Distance Jacobian at the recorded critical configuration (final link). */

    Eigen::VectorXd jointAnglesCriticalA;           /**< Joint angles recorded at the A critical point. */
    Eigen::VectorXd jointAnglesCriticalD;           /**< Joint angles recorded at the D critical point. */
    Eigen::VectorXd jointAnglesCriticalFinalLink;   /**< Joint angles recorded at the final-link critical point. */

    Eigen::VectorXd jointVelocityCriticalA;         /**< Joint velocity recorded at the A critical point. */
    Eigen::VectorXd jointVelocityCriticalD;         /**< Joint velocity recorded at the D critical point. */
    Eigen::VectorXd jointVelocityCriticalFinalLink; /**< Joint velocity recorded at the final-link critical point. */
    Eigen::VectorXd jointVelocityCurrentA;          /**< Current joint velocity for the A segment. */
    Eigen::VectorXd jointVelocityCurrentD;          /**< Current joint velocity for the D segment. */
    Eigen::VectorXd jointVelocityCurrentFinalLink;  /**< Current joint velocity for the final link. */


    Eigen::MatrixXd jacobiDistanceA;            /**< Current distance Jacobian (A). */
    Eigen::MatrixXd jacobiDistanceD;            /**< Current distance Jacobian (D). */
    Eigen::MatrixXd jacobiDistanceFinalLink;    /**< Current distance Jacobian (final link). */

    Eigen::MatrixXd jacobiCriticalA;            /**< Current critical-point Jacobian (A). */
    Eigen::MatrixXd jacobiCriticalMaxA;         /**< Critical-point Jacobian at the recorded configuration (A). */
    Eigen::MatrixXd jacobiCriticalD;            /**< Current critical-point Jacobian (D). */
    Eigen::MatrixXd jacobiCriticalMaxD;         /**< Critical-point Jacobian at the recorded configuration (D). */
    Eigen::MatrixXd jacobiCriticalFinalLink;    /**< Current critical-point Jacobian (final link). */
    Eigen::MatrixXd jacobiCriticalMaxFinalLink; /**< Critical-point Jacobian at the recorded configuration (final link). */

    Eigen::MatrixXd jacobiDistanceDynamicMaxA;          /**< Dynamic distance Jacobian at recorded configuration (A). */
    Eigen::MatrixXd jacobiDistanceDynamicMaxD;          /**< Dynamic distance Jacobian at recorded configuration (D). */
    Eigen::MatrixXd jacobiDistanceDynamicMaxFinalLink;  /**< Dynamic distance Jacobian at recorded configuration (final link). */
    Eigen::MatrixXd jacobiDistanceDynamicA;             /**< Current dynamic distance Jacobian (A). */
    Eigen::MatrixXd jacobiDistanceDynamicD;             /**< Current dynamic distance Jacobian (D). */
    Eigen::MatrixXd jacobiDistanceDynamicFinalLink;     /**< Current dynamic distance Jacobian (final link). */

    Eigen::Vector3d  closestPointObstacleA;             /**< Closest obstacle point for the A segment. */
    Eigen::Matrix4d  closestPointRelativeRobotLinkA;    /**< Relative transform of the A critical point on its link. */
    Eigen::Vector3d  closestPointObstacleD;             /**< Closest obstacle point for the D segment. */
    Eigen::Matrix4d closestPointRelativeRobotLinkD;     /**< Relative transform of the D critical point on its link. */
    Eigen::Vector3d closestPointObstacleFinalLink;      /**< Closest obstacle point for the final link. */
    Eigen::Matrix4d  closestPointRelativeFinalLink;     /**< Relative transform of the final-link critical point. */

    Eigen::Vector3d obstacleVelocityForLinkA;       /**< Estimated obstacle velocity for the A segment. */
    Eigen::Vector3d obstacleVelocityForLinkD;       /**< Estimated obstacle velocity for the D segment. */
    Eigen::Vector3d obstacleVelocityForFinalLink;   /**< Estimated obstacle velocity for the final link. */



    /**
     * @brief Construct a CriticalPoints entry with all channels initialized to their default (inactive) state.
     * @param numberOfJoints Number of robot joints (sizes the cached Jacobians and joint vectors).
     */
    CriticalPoints(int const &numberOfJoints);

};


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_CRITICALPOINTS_H
