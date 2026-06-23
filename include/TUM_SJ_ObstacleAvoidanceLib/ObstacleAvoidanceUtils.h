//
// Created by shobhit on 26.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H

#include <Eigen/Dense>
#include "Obstacle.h"

namespace ObstacleAvoidance {

    /**
     * @brief Smoothing constraint scheme that scales an obstacle-avoidance bound based on distance.
     * @param d Current distance to the obstacle.
     * @param dStop Inner stop distance threshold d1.
     * @param dActivate Outer activation distance threshold d2.
     * @param k Smoothing coefficient (clamped to a valid range internally).
     * @return Scaling factor in [-1, 1] (1 when far, -1 when within the stop zone).
     */
    double smoothingConstraintScheme(double const &d, double const &dStop, double const &dActivate, double const &k);

    /**
     * @brief Convert a vector of obstacles into a map keyed by "Obstacle<index>".
     * @param obstaclesArray Obstacles to convert.
     * @return Map from generated key to obstacle.
     */
    std::map<std::string, Obstacle> conversionObstaclesVectorToMap(std::vector<Obstacle> const &obstaclesArray);

    /**
     * @brief Decompose a vector into components parallel and perpendicular to a reference direction.
     * @param v0 Vector to decompose.
     * @param rc Reference direction.
     * @return Tuple of (component parallel to @p rc, component perpendicular to @p rc).
     */
    std::tuple<Eigen::Vector3d, Eigen::Vector3d> decomposeVector(
            Eigen::Vector3d const &v0,
            Eigen::Vector3d const &rc);

    /**
     * @brief Read obstacles from a JSON file.
     * @param obstacleFilePath Path to the JSON file with an "Obstacles" object.
     * @return Vector of parsed obstacles.
     */
    std::vector<Obstacle> readObstaclesFromJson(std::string const &obstacleFilePath);

    /**
     * @brief Read obstacles from a JSON file and return them as a map.
     * @param obstacleFilePath Path to the JSON file with an "Obstacles" object.
     * @return Map from generated key to obstacle.
     */
    std::map<std::string, Obstacle> obstaclesMapFromJson(std::string const &obstacleFilePath);

}
#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLEAVOIDANCEUTILS_H
